import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const put = vi.fn()
const deleteRequest = vi.fn()
const userStore = {
  tenantId: '1001' as string | number | null,
  getUser: { id: 'doctor-1', username: 'admin' }
}

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, put, delete: deleteRequest })
}))

vi.mock('@/stores/user', () => ({
  useUserStore: () => userStore
}))

import {
  realCreateInpatientMedicalOrder,
  realDeleteInpatientMedicalOrders,
  realPageInpatientMedicalOrders,
  realUpdateInpatientMedicalOrder
} from './orders-real'

describe('inpatient medical order real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    userStore.tenantId = '1001'
  })

  it('查询时发送租户和患者参数，并保留历次住院记录为只读', async () => {
    get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 2,
        pages: 1,
        rows: [
          {
            request_id: 'order-current',
            encounter_id: 'enc-1',
            patient_id: 'patient-1',
            medicine_type: 'DRUG',
            therapy_type: 'LONG_TERM',
            medicine_name: '阿司匹林',
            dose: '1',
            dose_unit: '片',
            method: '口服',
            rate: '每日一次',
            status: 'SIGNED'
          },
          {
            request_id: 'order-history',
            encounter_id: 'enc-old',
            patient_id: 'patient-1',
            medicine_type: 'LAB',
            therapy_type: 'TEMPORARY',
            medicine_name: '血常规',
            status: 'DRAFT'
          }
        ]
      }
    })

    const result = await realPageInpatientMedicalOrders({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'enc-1',
      patientId: 'patient-1'
    })

    expect(get).toHaveBeenCalledWith(
      '/enter-orders/get',
      {
        pageIndex: 1,
        pageSize: 20,
        tenant_id: '1001',
        'patient-id': 'patient-1'
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.rows?.[0]).toEqual(
      expect.objectContaining({
        id: 'order-current',
        category: 'DRUG',
        therapyType: 'LONG_TERM',
        status: 'SIGNED',
        dose: 1,
        readonly: false
      })
    )
    expect(result.rows?.[1]).toEqual(
      expect.objectContaining({
        id: 'order-history',
        category: 'LAB',
        encounterMismatch: true,
        readonly: true
      })
    )
  })

  it('新增医嘱映射后端必填字段并保存返回 ID', async () => {
    post.mockResolvedValue({ data: 'order-new' })

    const result = await realCreateInpatientMedicalOrder({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      catalogId: 'catalog-1',
      code: 'DRUG-001',
      name: '阿司匹林',
      category: 'DRUG',
      categoryName: '药品',
      dose: 1,
      unit: '片',
      frequency: '每日一次',
      route: '口服',
      quantity: 1,
      quantityUnit: '片',
      therapyType: 'LONG_TERM',
      orderNo: 1
    })

    expect(post).toHaveBeenCalledWith(
      '/enter-orders/add',
      expect.objectContaining({
        tenant_id: '1001',
        encounter_id: 'enc-1',
        patient_id: 'patient-1',
        status: 'DRAFT',
        medicine_type: 'DRUG',
        therapy_type: 'LONG_TERM',
        medicine_name: '阿司匹林',
        dose: '1',
        created_by: 'doctor-1',
        taotal_price: '0',
        skin_text: false
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual(expect.objectContaining({ id: 'order-new', source: 'real' }))
  })

  it('修改医嘱发送 request_id 和后端要求的完整字段', async () => {
    put.mockResolvedValue({ data: 'updated' })
    const current = {
      id: 'order-1',
      source: 'real' as const,
      encounterId: 'enc-1',
      patientId: 'patient-1',
      catalogId: 'catalog-1',
      code: 'DRUG-001',
      name: '阿司匹林',
      category: 'DRUG' as const,
      categoryName: '药品',
      dose: 1,
      unit: '片',
      frequency: '每日一次',
      route: '口服',
      quantity: 1,
      quantityUnit: '片',
      therapyType: 'LONG_TERM' as const,
      therapyTypeName: '长期',
      status: 'DRAFT' as const,
      statusName: '待签发',
      orderNo: 1,
      authoredAt: '2026-08-08 10:00:00',
      updatedAt: '2026-08-08 10:00:00',
      version: 0
    }

    await realUpdateInpatientMedicalOrder(
      current.id,
      {
        catalogId: current.catalogId,
        code: current.code,
        name: '阿司匹林肠溶片',
        category: current.category,
        categoryName: current.categoryName,
        specification: '100mg',
        dose: 2,
        unit: '片',
        frequency: '每日一次',
        route: '口服',
        quantity: 1,
        quantityUnit: '片',
        therapyType: current.therapyType,
        orderNo: 1,
        version: 0
      },
      current
    )

    expect(put).toHaveBeenCalledWith(
      '/enter-orders/modify',
      expect.objectContaining({
        request_id: 'order-1',
        medicine_type: 'DRUG',
        therapy_type: 'LONG_TERM',
        medicine_name: '阿司匹林肠溶片',
        dose: '2'
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('删除医嘱使用 ID 数组作为 DELETE 请求体', async () => {
    deleteRequest.mockResolvedValue({ data: ['order-1', 'order-2'] })

    const result = await realDeleteInpatientMedicalOrders({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      items: [
        { id: 'order-1', version: 0 },
        { id: 'order-2', version: 0 }
      ]
    })

    expect(deleteRequest).toHaveBeenCalledWith(
      '/enter-orders/delete',
      ['order-1', 'order-2'],
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.updatedCount).toBe(2)
  })
})
