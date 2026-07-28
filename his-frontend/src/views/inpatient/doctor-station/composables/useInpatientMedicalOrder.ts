import { computed, ref } from 'vue'

import {
  cancelStopInpatientMedicalOrders,
  createInpatientMedicalOrder,
  deleteInpatientMedicalOrders,
  groupInpatientMedicalOrders,
  pageInpatientMedicalOrderCatalog,
  pageInpatientMedicalOrders,
  signInpatientMedicalOrders,
  stopInpatientMedicalOrders,
  ungroupInpatientMedicalOrders,
  updateInpatientMedicalOrder,
  withdrawInpatientMedicalOrders,
  type InpatientMedicalOrderBatchActionDTO,
  type InpatientMedicalOrderRecordVO
} from '@/apis/inpatient/doctor-station'
import type {
  ClinicalOrderCatalogLoader,
  ClinicalOrderContext,
  ClinicalOrderDraft
} from '@/components/clinical/order'
import {
  inpatientAdviceAdapter,
  toCreateInpatientMedicalOrderDTO,
  toInpatientClinicalOrder,
  toInpatientOrderCandidate,
  toUpdateInpatientMedicalOrderDTO
} from '../advice/inpatientAdviceAdapter'

export type InpatientMedicalOrderAction =
  | 'delete'
  | 'group'
  | 'ungroup'
  | 'sign'
  | 'withdraw'
  | 'stop'
  | 'cancelStop'

function serializeOrders(orders: ClinicalOrderDraft[]) {
  return JSON.stringify(
    orders.map((order, index) => ({
      recordId: order.recordId,
      catalogId: order.catalogId,
      code: order.code,
      name: order.name,
      category: order.category,
      specification: order.specification,
      dose: order.dose,
      unit: order.unit,
      frequency: order.frequency,
      route: order.route,
      days: order.days,
      quantity: order.quantity,
      quantityUnit: order.quantityUnit,
      remark: order.remark,
      therapyType: order.therapyType,
      group: order.group,
      statusCode: order.statusCode,
      orderNo: index + 1
    }))
  )
}

function orderChanged(
  saved: InpatientMedicalOrderRecordVO,
  current: ClinicalOrderDraft,
  index: number
) {
  return (
    saved.catalogId !== current.catalogId ||
    saved.code !== current.code ||
    saved.name !== current.name ||
    saved.category !== current.category ||
    saved.specification !== current.specification ||
    saved.dose !== current.dose ||
    saved.unit !== current.unit ||
    saved.frequency !== current.frequency ||
    saved.route !== current.route ||
    saved.days !== current.days ||
    saved.quantity !== current.quantity ||
    saved.quantityUnit !== current.quantityUnit ||
    saved.remark !== current.remark ||
    saved.therapyType !== current.therapyType ||
    saved.orderNo !== index + 1
  )
}

export function useInpatientMedicalOrder() {
  const orderRecords = ref<InpatientMedicalOrderRecordVO[]>([])
  const orders = ref<ClinicalOrderDraft[]>([])
  const selectedOrders = ref<ClinicalOrderDraft[]>([])
  const savedOrderSnapshot = ref('[]')
  const loadedEncounterId = ref('')
  const ordersLoading = ref(false)
  const orderMutationLoading = ref(false)
  const ordersError = ref('')

  const ordersDirty = computed(
    () =>
      Boolean(loadedEncounterId.value) &&
      !ordersLoading.value &&
      serializeOrders(orders.value) !== savedOrderSnapshot.value
  )
  const ordersUpdatedAt = computed(
    () =>
      orderRecords.value
        .map((record) => record.updatedAt)
        .sort((left, right) => right.localeCompare(left))[0] ?? ''
  )
  const selectedOrderCount = computed(() => selectedOrders.value.length)
  const allSelectedDraft = computed(
    () =>
      selectedOrders.value.length > 0 &&
      selectedOrders.value.every((order) => order.statusCode === 'DRAFT')
  )
  const allSelectedSigned = computed(
    () =>
      selectedOrders.value.length > 0 &&
      selectedOrders.value.every((order) => order.statusCode === 'SIGNED')
  )
  const allSelectedStopped = computed(
    () =>
      selectedOrders.value.length > 0 &&
      selectedOrders.value.every((order) => order.statusCode === 'STOPPED')
  )
  const actionAvailability = computed(() => ({
    delete: allSelectedDraft.value,
    group:
      selectedOrders.value.length >= 2 &&
      allSelectedDraft.value &&
      selectedOrders.value.every((order) => order.category === 'DRUG' && !order.group),
    ungroup: allSelectedDraft.value && selectedOrders.value.every((order) => Boolean(order.group)),
    sign: allSelectedDraft.value,
    withdraw: allSelectedSigned.value,
    stop:
      allSelectedSigned.value &&
      selectedOrders.value.every((order) => order.therapyType === 'LONG_TERM'),
    cancelStop:
      allSelectedStopped.value &&
      selectedOrders.value.every((order) => order.therapyType === 'LONG_TERM')
  }))

  let latestOrderRequestId = 0

  function resetOrders() {
    latestOrderRequestId += 1
    loadedEncounterId.value = ''
    orderRecords.value = []
    orders.value = []
    selectedOrders.value = []
    savedOrderSnapshot.value = '[]'
    ordersLoading.value = false
    orderMutationLoading.value = false
    ordersError.value = ''
  }

  async function loadOrders(encounterId: string) {
    const requestId = ++latestOrderRequestId
    loadedEncounterId.value = encounterId
    orderRecords.value = []
    orders.value = []
    selectedOrders.value = []
    savedOrderSnapshot.value = '[]'
    ordersLoading.value = true
    ordersError.value = ''

    try {
      const result = await pageInpatientMedicalOrders({
        pageIndex: 1,
        pageSize: 200,
        encounterId
      })
      if (requestId !== latestOrderRequestId || loadedEncounterId.value !== encounterId) return

      const records = (result.rows ?? [])
        .slice()
        .sort((left, right) => left.orderNo - right.orderNo)
      const clinicalRows = records.map(toInpatientClinicalOrder)
      orderRecords.value = records
      orders.value = clinicalRows
      savedOrderSnapshot.value = serializeOrders(clinicalRows)
    } catch (error) {
      if (requestId !== latestOrderRequestId || loadedEncounterId.value !== encounterId) return
      ordersError.value = error instanceof Error ? error.message : '住院医嘱加载失败'
    } finally {
      if (requestId === latestOrderRequestId) ordersLoading.value = false
    }
  }

  const loadOrderCatalog: ClinicalOrderCatalogLoader = async (query, context) => {
    if (context.signal.aborted) return { rows: [], total: 0 }
    const result = await pageInpatientMedicalOrderCatalog({
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword || undefined
    })
    if (context.signal.aborted) return { rows: [], total: 0 }
    return {
      rows: (result.rows ?? []).map(toInpatientOrderCandidate),
      total: result.total
    }
  }

  function canEditOrder(order: ClinicalOrderDraft) {
    return !order.recordId || order.statusCode === 'DRAFT'
  }

  function selectionDisabledRow(order: ClinicalOrderDraft) {
    return !order.recordId || order.statusCode === 'WITHDRAWN'
  }

  function selectOrders(rows: ClinicalOrderDraft[]) {
    selectedOrders.value = rows
  }

  async function saveOrders(context: ClinicalOrderContext) {
    if (loadedEncounterId.value !== context.encounterKey || orderMutationLoading.value) return false

    const validation = inpatientAdviceAdapter.validate(orders.value, context)
    if (!validation.valid) {
      throw new Error(validation.errors[0]?.message ?? '住院医嘱校验失败')
    }

    const savedById = new Map(orderRecords.value.map((record) => [record.id, record]))
    const currentIds = new Set(
      orders.value.map((order) => order.recordId).filter((id): id is string => Boolean(id))
    )
    const deletedDrafts = orderRecords.value.filter(
      (record) => record.status === 'DRAFT' && !currentIds.has(record.id)
    )
    let changedCount = 0
    orderMutationLoading.value = true

    try {
      if (deletedDrafts.length > 0) {
        await deleteInpatientMedicalOrders({
          encounterId: context.encounterKey,
          patientId: context.patientId,
          items: deletedDrafts.map((record) => ({ id: record.id, version: record.version }))
        })
        changedCount += deletedDrafts.length
      }

      for (const [index, order] of orders.value.entries()) {
        if (!order.recordId) {
          await createInpatientMedicalOrder(toCreateInpatientMedicalOrderDTO(order, index, context))
          changedCount += 1
          continue
        }

        const saved = savedById.get(order.recordId)
        if (!saved || saved.status !== 'DRAFT' || !orderChanged(saved, order, index)) continue
        await updateInpatientMedicalOrder(
          saved.id,
          toUpdateInpatientMedicalOrderDTO(order, index, saved.version)
        )
        changedCount += 1
      }

      if (loadedEncounterId.value === context.encounterKey) {
        await loadOrders(context.encounterKey)
      }
      return changedCount
    } catch (error) {
      if (loadedEncounterId.value === context.encounterKey) {
        await loadOrders(context.encounterKey)
      }
      throw error
    } finally {
      orderMutationLoading.value = false
    }
  }

  function selectedBatchData(context: ClinicalOrderContext): InpatientMedicalOrderBatchActionDTO {
    const recordsById = new Map(orderRecords.value.map((record) => [record.id, record]))
    return {
      encounterId: context.encounterKey,
      patientId: context.patientId,
      items: selectedOrders.value.map((order) => {
        const record = order.recordId ? recordsById.get(order.recordId) : undefined
        if (!record) throw new Error('所选医嘱尚未保存或已经失效，请刷新后重试')
        return { id: record.id, version: record.version }
      })
    }
  }

  async function executeOrderAction(
    action: InpatientMedicalOrderAction,
    context: ClinicalOrderContext
  ) {
    if (ordersDirty.value) throw new Error('请先保存当前医嘱修改')
    if (!actionAvailability.value[action]) throw new Error('所选医嘱状态不支持当前操作')
    const data = selectedBatchData(context)
    orderMutationLoading.value = true

    try {
      const result =
        action === 'delete'
          ? await deleteInpatientMedicalOrders(data)
          : action === 'group'
            ? await groupInpatientMedicalOrders(data)
            : action === 'ungroup'
              ? await ungroupInpatientMedicalOrders(data)
              : action === 'sign'
                ? await signInpatientMedicalOrders(data)
                : action === 'withdraw'
                  ? await withdrawInpatientMedicalOrders(data)
                  : action === 'stop'
                    ? await stopInpatientMedicalOrders(data)
                    : await cancelStopInpatientMedicalOrders(data)

      if (loadedEncounterId.value === context.encounterKey) {
        await loadOrders(context.encounterKey)
      }
      return result.updatedCount
    } finally {
      orderMutationLoading.value = false
    }
  }

  return {
    actionAvailability,
    canEditOrder,
    executeOrderAction,
    loadOrderCatalog,
    loadOrders,
    orderMutationLoading,
    orders,
    ordersDirty,
    ordersError,
    ordersLoading,
    ordersUpdatedAt,
    resetOrders,
    saveOrders,
    selectOrders,
    selectedOrderCount,
    selectionDisabledRow
  }
}
