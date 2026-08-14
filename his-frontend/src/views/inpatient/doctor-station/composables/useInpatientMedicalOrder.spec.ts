import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  createInpatientMedicalOrder,
  deleteInpatientMedicalOrders,
  pageInpatientMedicalOrderCatalog,
  pageInpatientMedicalOrders,
  stopInpatientMedicalOrders,
  updateInpatientMedicalOrder,
  type InpatientMedicalOrderRecordVO
} from '@/apis/inpatient/doctor-station'
import type { ClinicalOrderContext, ClinicalOrderDraft } from '@/components/clinical/order'
import { useInpatientMedicalOrder } from './useInpatientMedicalOrder'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  cancelStopInpatientMedicalOrders: vi.fn(),
  createInpatientMedicalOrder: vi.fn(),
  deleteInpatientMedicalOrders: vi.fn(),
  groupInpatientMedicalOrders: vi.fn(),
  pageInpatientMedicalOrderCatalog: vi.fn(),
  pageInpatientMedicalOrders: vi.fn(),
  signInpatientMedicalOrders: vi.fn(),
  stopInpatientMedicalOrders: vi.fn(),
  ungroupInpatientMedicalOrders: vi.fn(),
  updateInpatientMedicalOrder: vi.fn(),
  withdrawInpatientMedicalOrders: vi.fn()
}))

const context: ClinicalOrderContext = {
  scope: 'inpatient',
  encounterKey: 'enc-1',
  patientId: 'patient-1',
  departmentName: '心血管内科'
}

function record(
  id: string,
  status: InpatientMedicalOrderRecordVO['status'] = 'DRAFT',
  therapyType: InpatientMedicalOrderRecordVO['therapyType'] = 'LONG_TERM',
  orderNo = 1
): InpatientMedicalOrderRecordVO {
  return {
    id,
    encounterId: context.encounterKey,
    patientId: context.patientId,
    catalogId: `catalog-${id}`,
    code: `CODE-${id}`,
    name: `治疗项目-${id}`,
    category: 'TREATMENT',
    categoryName: '治疗',
    quantity: 1,
    quantityUnit: '次',
    therapyType,
    therapyTypeName: therapyType === 'LONG_TERM' ? '长期' : '临时',
    status,
    statusName:
      status === 'DRAFT'
        ? '待签发'
        : status === 'SIGNED'
          ? '已签发'
          : status === 'STOPPED'
            ? '已停嘱'
            : '已撤回',
    orderNo,
    authoredAt: '2026-07-27 09:00:00',
    updatedAt: '2026-07-27 09:00:00',
    version: 1
  }
}

function page(rows: InpatientMedicalOrderRecordVO[]) {
  return {
    pageIndex: 1,
    pageSize: 200,
    total: rows.length,
    pages: 1,
    rows
  }
}

function batchResult(rows: InpatientMedicalOrderRecordVO[]) {
  return { updatedCount: rows.length, rows }
}

describe('useInpatientMedicalOrder', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('加载住院医嘱后转换为公共医嘱模型并建立快照', async () => {
    vi.mocked(pageInpatientMedicalOrders).mockResolvedValue(
      page([record('order-2', 'SIGNED', 'TEMPORARY', 2), record('order-1')])
    )
    const state = useInpatientMedicalOrder()

    await state.loadOrders(context.encounterKey)

    expect(state.orders.value.map((order) => order.recordId)).toEqual(['order-1', 'order-2'])
    expect(state.orders.value[1]).toEqual(
      expect.objectContaining({
        statusCode: 'SIGNED',
        therapyType: 'TEMPORARY',
        therapyTypeName: '临时'
      })
    )
    expect(state.ordersDirty.value).toBe(false)
  })

  it('保存时按列表差异调用修改、新增和删除接口', async () => {
    const deleted = record('order-deleted', 'DRAFT', 'LONG_TERM', 1)
    const updated = record('order-updated', 'DRAFT', 'LONG_TERM', 2)
    const reloadedUpdated = { ...updated, quantity: 2, orderNo: 1, version: 2 }
    const created = record('order-created', 'DRAFT', 'TEMPORARY', 2)

    vi.mocked(pageInpatientMedicalOrders)
      .mockResolvedValueOnce(page([deleted, updated]))
      .mockResolvedValueOnce(page([reloadedUpdated, created]))
    vi.mocked(deleteInpatientMedicalOrders).mockResolvedValue(batchResult([deleted]))
    vi.mocked(updateInpatientMedicalOrder).mockResolvedValue(reloadedUpdated)
    vi.mocked(createInpatientMedicalOrder).mockResolvedValue(created)
    const state = useInpatientMedicalOrder()
    await state.loadOrders(context.encounterKey)

    const existing = state.orders.value[1]!
    const newDraft: ClinicalOrderDraft = {
      key: 'draft-new',
      catalogId: 'catalog-new',
      code: 'NEW-001',
      name: '新治疗项目',
      category: 'TREATMENT',
      categoryName: '治疗',
      quantity: 1,
      quantityUnit: '次',
      therapyType: 'TEMPORARY',
      therapyTypeName: '临时',
      status: 'DRAFT',
      statusCode: 'DRAFT'
    }
    state.orders.value = [{ ...existing, quantity: 2 }, newDraft]

    const changedCount = await state.saveOrders(context)

    expect(changedCount).toBe(3)
    expect(deleteInpatientMedicalOrders).toHaveBeenCalledWith(
      expect.objectContaining({
        encounterId: context.encounterKey,
        items: [{ id: deleted.id, version: deleted.version }]
      })
    )
    expect(updateInpatientMedicalOrder).toHaveBeenCalledWith(
      updated.id,
      expect.objectContaining({ quantity: 2, orderNo: 1, version: updated.version })
    )
    expect(createInpatientMedicalOrder).toHaveBeenCalledWith(
      expect.objectContaining({
        encounterId: context.encounterKey,
        patientId: context.patientId,
        catalogId: newDraft.catalogId,
        orderNo: 2
      })
    )
    expect(state.ordersDirty.value).toBe(false)
  })

  it('历史住院医嘱可展示但不能编辑或参与选择', async () => {
    const current = record('order-current')
    const history = { ...record('order-history'), encounterId: 'enc-old' }
    vi.mocked(pageInpatientMedicalOrders).mockResolvedValue(page([current, history]))

    const state = useInpatientMedicalOrder()
    await state.loadOrders(context.encounterKey, context.patientId)

    expect(state.orders.value).toHaveLength(2)
    expect(state.canEditOrder(state.orders.value[0]!)).toBe(true)
    expect(state.canEditOrder(state.orders.value[1]!)).toBe(false)
    expect(state.selectionDisabledRow(state.orders.value[1]!)).toBe(true)
  })

  it('仅对符合状态和长临规则的选中医嘱开放生命周期操作', async () => {
    const signedLongTerm = record('order-signed', 'SIGNED', 'LONG_TERM')
    const stopped = { ...signedLongTerm, status: 'STOPPED' as const, statusName: '已停嘱' }
    vi.mocked(pageInpatientMedicalOrders)
      .mockResolvedValueOnce(page([signedLongTerm]))
      .mockResolvedValueOnce(page([stopped]))
    vi.mocked(stopInpatientMedicalOrders).mockResolvedValue(batchResult([stopped]))
    const state = useInpatientMedicalOrder()
    await state.loadOrders(context.encounterKey)
    state.selectOrders(state.orders.value)

    expect(state.actionAvailability.value).toEqual(
      expect.objectContaining({
        stop: true,
        withdraw: true,
        sign: false,
        cancelStop: false
      })
    )

    const changedCount = await state.executeOrderAction('stop', context)

    expect(changedCount).toBe(1)
    expect(stopInpatientMedicalOrders).toHaveBeenCalledWith({
      encounterId: context.encounterKey,
      patientId: context.patientId,
      items: [{ id: signedLongTerm.id, version: signedLongTerm.version }]
    })
    expect(state.orders.value[0]?.statusCode).toBe('STOPPED')
  })

  it('医嘱项目目录加载器会适配公共选择弹窗模型', async () => {
    vi.mocked(pageInpatientMedicalOrderCatalog).mockResolvedValue({
      pageIndex: 1,
      pageSize: 10,
      total: 1,
      pages: 1,
      rows: [
        {
          id: 'catalog-1',
          code: 'LAB-001',
          name: '血常规',
          category: 'LAB',
          categoryName: '检验',
          defaultQuantity: 1,
          quantityUnit: '次',
          defaultTherapyType: 'TEMPORARY'
        }
      ]
    })
    const state = useInpatientMedicalOrder()
    const result = await state.loadOrderCatalog(
      { pageIndex: 1, pageSize: 10, keyword: '血常规' },
      { signal: new AbortController().signal }
    )

    expect(result.rows[0]).toEqual(
      expect.objectContaining({
        id: 'catalog-1',
        category: 'LAB',
        defaultTherapyType: 'TEMPORARY'
      })
    )
  })
})
