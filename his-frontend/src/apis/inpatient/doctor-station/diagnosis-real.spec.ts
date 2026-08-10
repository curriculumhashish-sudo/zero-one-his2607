import { beforeEach, describe, expect, it, vi } from 'vitest'

const post = vi.fn()
const put = vi.fn()
const deleteRequest = vi.fn()
const userStore = { tenantId: '1001' as string | number | null }

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ post, put, delete: deleteRequest })
}))

vi.mock('@/stores/user', () => ({
  useUserStore: () => userStore
}))

import {
  realCreateInpatientDiagnosis,
  realDeleteInpatientDiagnosis,
  realUpdateInpatientDiagnosis
} from './diagnosis-real'

describe('inpatient diagnosis real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    userStore.tenantId = '1001'
  })

  it('新增诊断时映射后端字段并保存返回的真实 ID', async () => {
    post.mockResolvedValue({ data: 'diag-real-1' })

    const result = await realCreateInpatientDiagnosis({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      catalogId: 'catalog-1',
      code: '123',
      name: '高血压',
      typeName: '西医诊断',
      primary: true,
      orderNo: 1
    })

    expect(post).toHaveBeenCalledWith(
      '/diagnosis/add',
      {
        tenant_id: '1001',
        encounter_id: 'enc-1',
        description: '高血压',
        tcm_flag: 0,
        primary_flag: 1,
        rank: 1,
        condition_code: 123
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual(
      expect.objectContaining({
        id: 'diag-real-1',
        backendId: 'diag-real-1',
        source: 'real',
        readonlyFields: true,
        encounterId: 'enc-1'
      })
    )
  })

  it('租户上下文缺失时不发送新增请求', async () => {
    userStore.tenantId = null

    await expect(
      realCreateInpatientDiagnosis({
        encounterId: 'enc-1',
        patientId: 'patient-1',
        catalogId: 'catalog-1',
        code: 'I10',
        name: '高血压',
        typeName: '西医诊断',
        primary: true,
        orderNo: 1
      })
    ).rejects.toThrow('缺少租户 ID')
    expect(post).not.toHaveBeenCalled()
  })

  it('真实诊断修改只发送诊断类型', async () => {
    put.mockResolvedValue({ data: 'diag-real-1' })
    const result = await realUpdateInpatientDiagnosis(
      'diag-real-1',
      {
        catalogId: 'catalog-1',
        code: 'I10',
        name: '高血压',
        typeName: '出院诊断',
        primary: true,
        orderNo: 1,
        version: 0
      },
      {
        id: 'diag-real-1',
        backendId: 'diag-real-1',
        source: 'real',
        encounterId: 'enc-1',
        patientId: 'patient-1',
        catalogId: 'catalog-1',
        code: 'I10',
        name: '高血压',
        typeName: '西医诊断',
        primary: true,
        orderNo: 1,
        diagnosedAt: '',
        updatedAt: '',
        version: 0
      }
    )

    expect(put).toHaveBeenCalledWith(
      '/diagnosis/modify',
      { id: 'diag-real-1', diag_type_code: '出院诊断' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.typeName).toBe('出院诊断')
  })

  it('真实诊断删除使用 backendId', async () => {
    deleteRequest.mockResolvedValue({ data: true })

    await realDeleteInpatientDiagnosis('local-id', 'diag-real-1')

    expect(deleteRequest).toHaveBeenCalledWith(
      '/diagnosis/diag-real-1',
      undefined,
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })
})
