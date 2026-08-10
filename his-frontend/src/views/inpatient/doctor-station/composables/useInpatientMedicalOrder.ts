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
  type InpatientMedicalOrderBatchResultVO,
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
  const lastOrderActionResult = ref<InpatientMedicalOrderBatchResultVO | undefined>()

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
  const realLifecycleFieldsReady = computed(() =>
    selectedOrders.value.every((order) => {
      const record = order.raw as InpatientMedicalOrderRecordVO | undefined
      return record?.source !== 'real' || Boolean(record.id && record.productTableName)
    })
  )
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
    ungroup:
      allSelectedDraft.value &&
      selectedOrders.value.every((order) => Boolean(order.group)),
    sign: allSelectedDraft.value && realLifecycleFieldsReady.value,
    withdraw: allSelectedSigned.value && realLifecycleFieldsReady.value,
    stop:
      allSelectedSigned.value &&
      realLifecycleFieldsReady.value &&
      selectedOrders.value.every((order) => order.therapyType === 'LONG_TERM'),
    cancelStop:
      allSelectedStopped.value &&
      realLifecycleFieldsReady.value &&
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
    lastOrderActionResult.value = undefined
    ordersError.value = ''
  }

  async function loadOrders(encounterId: string, patientId = '') {
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
        encounterId,
        patientId: patientId || undefined
      })
      if (requestId !== latestOrderRequestId || loadedEncounterId.value !== encounterId) return

      const records = (result.rows ?? [])
        .map((record) => ({
          ...record,
          source: record.source ?? ('mock' as const),
          encounterMismatch: record.encounterId !== encounterId,
          readonly: Boolean(record.readonly || record.encounterId !== encounterId)
        }))
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
    const record = order.raw as InpatientMedicalOrderRecordVO | undefined
    return (
      !record?.encounterMismatch &&
      !record?.readonly &&
      (!order.recordId || order.statusCode === 'DRAFT')
    )
  }

  function selectionDisabledRow(order: ClinicalOrderDraft) {
    const record = order.raw as InpatientMedicalOrderRecordVO | undefined
    return (
      Boolean(record?.encounterMismatch || record?.readonly) ||
      order.statusCode === 'WITHDRAWN'
    )
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
      (record) =>
        record.encounterId === context.encounterKey &&
        !record.readonly &&
        record.status === 'DRAFT' &&
        !currentIds.has(record.id)
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
        if (
          !saved ||
          saved.encounterId !== context.encounterKey ||
          saved.readonly ||
          saved.status !== 'DRAFT' ||
          !orderChanged(saved, order, index)
        ) continue
        const updateData = toUpdateInpatientMedicalOrderDTO(order, index, saved.version)
        if (saved.source === 'real') {
          await updateInpatientMedicalOrder(saved.id, updateData, {
            source: saved.source,
            current: saved
          })
        } else {
          await updateInpatientMedicalOrder(saved.id, updateData)
        }
        changedCount += 1
      }

      if (loadedEncounterId.value === context.encounterKey) {
        await loadOrders(context.encounterKey, context.patientId)
      }
      return changedCount
    } catch (error) {
      if (loadedEncounterId.value === context.encounterKey) {
        await loadOrders(context.encounterKey, context.patientId)
      }
      throw error
    } finally {
      orderMutationLoading.value = false
    }
  }

  function selectedBatchData(
    context: ClinicalOrderContext,
    options: { stopReason?: number } = {}
  ): InpatientMedicalOrderBatchActionDTO {
    const recordsById = new Map(orderRecords.value.map((record) => [record.id, record]))
    const result: InpatientMedicalOrderBatchActionDTO = {
      encounterId: context.encounterKey,
      patientId: context.patientId,
      items: selectedOrders.value.map((order) => {
        const record = order.recordId ? recordsById.get(order.recordId) : undefined
        if (!record) throw new Error('所选医嘱尚未保存或已经失效，请刷新后重试')
        if (record.encounterId !== context.encounterKey || record.readonly) {
          throw new Error('历史住院医嘱不能执行当前操作')
        }
        const item: InpatientMedicalOrderBatchActionDTO['items'][number] = {
          id: record.id,
          version: record.version
        }
        if (record.source === 'real') {
          item.productTableName = record.productTableName
          item.groupId = record.groupId
        }
        return item
      })
    }
    if (options.stopReason !== undefined) result.stopReason = options.stopReason
    return result
  }

  async function executeOrderAction(
    action: InpatientMedicalOrderAction,
    context: ClinicalOrderContext,
    options: { stopReason?: number } = {}
  ) {
    if (ordersDirty.value) throw new Error('请先保存当前医嘱修改')
    if (!actionAvailability.value[action]) throw new Error('所选医嘱状态不支持当前操作')
    if (
      action === 'stop' &&
      options.stopReason === undefined &&
      selectedOrders.value.some(
        (order) => (order.raw as InpatientMedicalOrderRecordVO | undefined)?.source === 'real'
      )
    ) {
      throw new Error('请填写有效的停嘱原因码')
    }
    /*
    if (
      action === 'stop' &&
      options.stopReason === undefined &&
      selectedOrders.value.some(
        (order) => (order.raw as InpatientMedicalOrderRecordVO | undefined)?.source === 'real'
      )
    ) {
    if (ordersDirty.value) throw new Error('请先保存当前医嘱修改')
    if (!actionAvailability.value[action]) throw new Error('所选医嘱状态不支持当前操作')
      throw new Error('当前后端尚未提供完整医嘱生命周期字段，暂不能执行该操作')
    }
    */
    const data = selectedBatchData(context, options)
    orderMutationLoading.value = true
    lastOrderActionResult.value = undefined

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
        await loadOrders(context.encounterKey, context.patientId)
      }
      lastOrderActionResult.value = result
      return result.updatedCount
    } finally {
      orderMutationLoading.value = false
    }
  }

  return {
    actionAvailability,
    canEditOrder,
    executeOrderAction,
    lastOrderActionResult,
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
