import { computed, onBeforeUnmount, reactive, ref, type ComputedRef, type Ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  createAdviceOrder,
  deleteAdviceOrders,
  groupAdviceOrders,
  pageAdviceOrderCatalog,
  pageAdviceOrders,
  signAdviceOrders,
  ungroupAdviceOrders,
  updateAdviceOrder,
  withdrawAdviceOrders,
  type DoctorAdviceOrderCategory,
  type DoctorAdviceOrderListVO,
  type DoctorAdviceOrderStatus,
  type DoctorPatientContextVO,
  type SaveDoctorAdviceOrderItemDTO
} from '@/apis/outpatient/doctor-station'
import {
  buildClinicalOrderDraftChanges,
  type ClinicalOrderCatalogLoader,
  type ClinicalOrderContext,
  type ClinicalOrderDraft
} from '@/components/clinical/order'
import type { PatientSummary } from '@/components/patient/type'

import {
  outpatientAdviceAdapter,
  toClinicalOrder,
  toClinicalOrderCandidate
} from '../advice/outpatientAdviceAdapter'

interface DoctorAdviceOrderOptions {
  selectedPatientId: Ref<string | number | null>
  selectedPatient: ComputedRef<PatientSummary | null>
  patientContext: Ref<DoctorPatientContextVO | null>
  patientAccepted: ComputedRef<boolean>
}

interface AdviceOrderQueryState {
  pageIndex: number
  pageSize: number
  keyword: string
  category: '' | DoctorAdviceOrderCategory
  status: '' | DoctorAdviceOrderStatus
}

/** 生成不包含临时 key 的医嘱业务快照。 */
function serializeAdviceOrders(items: ClinicalOrderDraft[]) {
  return JSON.stringify(
    items.map((item) => ({
      recordId: item.recordId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      category: item.category,
      categoryName: item.categoryName,
      specification: item.specification,
      dose: item.dose,
      unit: item.unit,
      frequency: item.frequency,
      route: item.route,
      days: item.days,
      quantity: item.quantity,
      quantityUnit: item.quantityUnit,
      dosePerQuantityUnit: item.dosePerQuantityUnit,
      remark: item.remark,
      status: item.status,
      groupId: item.groupId
    }))
  )
}

function toSaveItem(order: ClinicalOrderDraft, orderNo: number): SaveDoctorAdviceOrderItemDTO {
  return {
    id: order.recordId,
    catalogId: order.catalogId,
    code: order.code,
    name: order.name,
    category: order.category,
    categoryName: order.categoryName,
    specification: order.specification,
    dose: order.dose === undefined ? undefined : Number(order.dose),
    unit: order.unit,
    frequency: order.frequency,
    route: order.route,
    days: order.days === undefined ? undefined : Number(order.days),
    quantity: order.quantity ?? 0,
    quantityUnit: order.quantityUnit ?? '项',
    dosePerQuantityUnit: order.dosePerQuantityUnit,
    remark: order.remark,
    groupId: order.groupId,
    orderNo
  }
}

/** 管理门诊医嘱分页、草稿差异、自动保存以及正式状态动作。 */
export function useDoctorAdviceOrders(options: DoctorAdviceOrderOptions) {
  const { selectedPatientId, selectedPatient, patientContext, patientAccepted } = options

  const adviceOrderList = ref<DoctorAdviceOrderListVO | null>(null)
  const adviceOrders = ref<ClinicalOrderDraft[]>([])
  const savedAdviceOrders = ref<ClinicalOrderDraft[]>([])
  const savedAdviceOrderSnapshot = ref('[]')
  const adviceOrdersLoading = ref(false)
  const adviceOrdersSaving = ref(false)
  const adviceOrdersActionLoading = ref(false)
  const adviceOrdersError = ref('')
  const adviceOrdersTotal = ref(0)
  const adviceOrderQuery = reactive<AdviceOrderQueryState>({
    pageIndex: 1,
    pageSize: 10,
    keyword: '',
    category: '',
    status: ''
  })

  const adviceOrderContext = computed<ClinicalOrderContext | null>(() => {
    if (!patientContext.value || selectedPatientId.value === null) return null
    return {
      scope: 'outpatient',
      patientId: patientContext.value.patientId,
      encounterKey: String(selectedPatientId.value),
      departmentName: selectedPatient.value?.departmentName ?? undefined
    }
  })

  const ordersDirty = computed(
    () =>
      adviceOrderList.value !== null &&
      !adviceOrdersLoading.value &&
      serializeAdviceOrders(adviceOrders.value) !== savedAdviceOrderSnapshot.value
  )

  let latestAdviceOrdersRequestId = 0
  let autoSaveTimer: ReturnType<typeof setTimeout> | undefined

  function cancelAdviceOrderAutoSave() {
    if (autoSaveTimer) clearTimeout(autoSaveTimer)
    autoSaveTimer = undefined
  }

  function resetQuery() {
    adviceOrderQuery.pageIndex = 1
    adviceOrderQuery.pageSize = 10
    adviceOrderQuery.keyword = ''
    adviceOrderQuery.category = ''
    adviceOrderQuery.status = ''
  }

  async function loadCurrentAdviceOrders(encounterId: string | number, shouldResetQuery = true) {
    cancelAdviceOrderAutoSave()
    if (shouldResetQuery) resetQuery()
    const requestId = ++latestAdviceOrdersRequestId
    adviceOrdersLoading.value = true
    adviceOrdersError.value = ''

    try {
      const result = await pageAdviceOrders({
        encounterId: String(encounterId),
        pageIndex: adviceOrderQuery.pageIndex,
        pageSize: adviceOrderQuery.pageSize,
        keyword: adviceOrderQuery.keyword.trim() || undefined,
        category: adviceOrderQuery.category || undefined,
        status: adviceOrderQuery.status || undefined
      })
      if (requestId !== latestAdviceOrdersRequestId) return
      const maxPage = Math.max(1, Math.ceil(result.total / adviceOrderQuery.pageSize))
      if (adviceOrderQuery.pageIndex > maxPage) {
        adviceOrderQuery.pageIndex = maxPage
        await loadCurrentAdviceOrders(encounterId, false)
        return
      }

      const pageRows = result.rows ?? []
      const clinicalRows = pageRows
        .slice()
        .sort((left, right) => left.orderNo - right.orderNo)
        .map(toClinicalOrder)
      adviceOrderList.value = {
        encounterId: result.encounterId,
        rows: pageRows,
        updatedAt: result.updatedAt
      }
      adviceOrders.value = clinicalRows
      savedAdviceOrders.value = clinicalRows.map((item) => ({ ...item }))
      savedAdviceOrderSnapshot.value = serializeAdviceOrders(clinicalRows)
      adviceOrdersTotal.value = result.total
    } catch (error) {
      if (requestId !== latestAdviceOrdersRequestId) return
      adviceOrdersError.value = error instanceof Error ? error.message : '门诊医嘱加载失败'
    } finally {
      if (requestId === latestAdviceOrdersRequestId) adviceOrdersLoading.value = false
    }
  }

  function resetAdviceOrders() {
    cancelAdviceOrderAutoSave()
    latestAdviceOrdersRequestId++
    resetQuery()
    adviceOrderList.value = null
    adviceOrders.value = []
    savedAdviceOrders.value = []
    savedAdviceOrderSnapshot.value = serializeAdviceOrders([])
    adviceOrdersTotal.value = 0
    adviceOrdersError.value = ''
    adviceOrdersLoading.value = false
  }

  const loadAdviceOrderCatalog: ClinicalOrderCatalogLoader = async (query, context) => {
    if (context.signal.aborted) return { rows: [], total: 0 }

    const result = await pageAdviceOrderCatalog({
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword || undefined
    })
    if (context.signal.aborted) return { rows: [], total: 0 }

    return {
      rows: (result.rows ?? []).map(toClinicalOrderCandidate),
      total: result.total
    }
  }

  function validateCurrentAdviceOrders() {
    const context = adviceOrderContext.value
    if (!context) {
      return { valid: false, message: '患者医嘱上下文尚未加载完成，请稍后重试' }
    }

    const result = outpatientAdviceAdapter.validate(adviceOrders.value, context)
    return {
      valid: result.valid,
      message: result.errors[0]?.message ?? '请完善医嘱内容'
    }
  }

  async function applySavedGrouping(
    encounterId: string,
    initialResult: DoctorAdviceOrderListVO,
    desiredRows: ClinicalOrderDraft[]
  ) {
    let result = initialResult
    const resolvedRows = desiredRows.map((order) => {
      if (order.recordId) return order
      const created = result.rows.find(
        (row) => row.catalogId === order.catalogId && row.status === 'DRAFT'
      )
      return created ? { ...order, recordId: created.id } : order
    })
    const desiredGroupById = new Map(
      resolvedRows
        .filter(
          (item): item is ClinicalOrderDraft & { recordId: string } =>
            item.status === 'DRAFT' && Boolean(item.recordId)
        )
        .map((item) => [item.recordId, item.groupId])
    )
    const changedRows = result.rows.filter(
      (row) =>
        row.status === 'DRAFT' &&
        desiredGroupById.has(row.id) &&
        desiredGroupById.get(row.id) !== row.groupId
    )
    const idsToUngroup = changedRows.filter((row) => row.groupId).map((row) => row.id)
    if (idsToUngroup.length > 0) {
      result = await ungroupAdviceOrders({
        encounterId,
        adviceIds: idsToUngroup
      })
    }

    const targetGroups = new Map<string, string[]>()
    for (const [recordId, groupId] of desiredGroupById) {
      if (!groupId) continue
      const members = targetGroups.get(groupId) ?? []
      members.push(recordId)
      targetGroups.set(groupId, members)
    }
    for (const [groupId, adviceIds] of targetGroups) {
      if (adviceIds.length < 2) continue
      const needsUpdate = adviceIds.some(
        (id) => result.rows.find((row) => row.id === id)?.groupId !== groupId
      )
      if (!needsUpdate) continue
      result = await groupAdviceOrders({
        encounterId,
        adviceIds,
        groupId
      })
    }
    return result
  }

  async function saveCurrentAdviceOrders(notifySuccess = true): Promise<boolean> {
    const encounterId = selectedPatientId.value
    const context = adviceOrderContext.value
    if (
      !encounterId ||
      !context ||
      !patientAccepted.value ||
      !adviceOrderList.value ||
      adviceOrdersSaving.value
    ) {
      return false
    }
    if (!ordersDirty.value) return true

    const validation = outpatientAdviceAdapter.validate(adviceOrders.value, context)
    if (!validation.valid) {
      ElMessage.warning(validation.errors[0]?.message ?? '请完善医嘱内容')
      return false
    }

    const encounterIdText = String(encounterId)
    const changes = buildClinicalOrderDraftChanges(adviceOrders.value, savedAdviceOrders.value)
    const desiredRows = adviceOrders.value.map((item) => ({ ...item }))
    const pageOffset = (adviceOrderQuery.pageIndex - 1) * adviceOrderQuery.pageSize
    adviceOrdersSaving.value = true

    try {
      if (changes.deletedRecordIds.length > 0) {
        await deleteAdviceOrders({
          encounterId: encounterIdText,
          adviceIds: changes.deletedRecordIds
        })
      }

      for (const order of changes.updated) {
        const index = adviceOrders.value.findIndex((item) => item.key === order.key)
        await updateAdviceOrder({
          encounterId: encounterIdText,
          adviceId: order.recordId!,
          item: toSaveItem(order, pageOffset + index + 1)
        })
      }

      for (const order of changes.created) {
        const index = adviceOrders.value.findIndex((item) => item.key === order.key)
        await createAdviceOrder({
          encounterId: encounterIdText,
          item: toSaveItem(order, pageOffset + index + 1)
        })
      }

      // Mutation endpoints return a single row or an operation summary rather
      // than the complete list. Refresh before resolving local group IDs.
      await loadCurrentAdviceOrders(encounterIdText, false)
      if (!adviceOrderList.value) return false
      await applySavedGrouping(encounterIdText, adviceOrderList.value, desiredRows)
      if (String(selectedPatientId.value) !== encounterIdText) return false

      await loadCurrentAdviceOrders(encounterIdText, false)
      if (notifySuccess) ElMessage.success('门诊医嘱保存成功')
      return true
    } catch (error) {
      const message = error instanceof Error ? error.message : '门诊医嘱保存失败'
      ElMessage.error(message)
      if (String(selectedPatientId.value) === encounterIdText) {
        await loadCurrentAdviceOrders(encounterIdText, false)
      }
      return false
    } finally {
      adviceOrdersSaving.value = false
    }
  }

  /** 校验通过后防抖提交医嘱草稿，患者切换会取消旧就诊的待提交任务。 */
  function scheduleAdviceOrderAutoSave(delay = 800) {
    cancelAdviceOrderAutoSave()
    const encounterId = selectedPatientId.value
    if (!encounterId || !patientAccepted.value) return
    const encounterIdText = String(encounterId)
    autoSaveTimer = setTimeout(() => {
      autoSaveTimer = undefined
      if (
        String(selectedPatientId.value) !== encounterIdText ||
        !ordersDirty.value ||
        adviceOrdersActionLoading.value
      ) {
        return
      }
      if (!validateCurrentAdviceOrders().valid) return
      void saveCurrentAdviceOrders(false)
    }, delay)
  }

  onBeforeUnmount(cancelAdviceOrderAutoSave)

  async function confirmDiscardQueryChanges() {
    if (!ordersDirty.value) return true
    try {
      await ElMessageBox.confirm(
        '当前页存在未保存的医嘱修改，切换查询将放弃这些修改，是否继续？',
        '未保存提示',
        {
          type: 'warning',
          confirmButtonText: '放弃并继续',
          cancelButtonText: '返回保存'
        }
      )
      return true
    } catch {
      return false
    }
  }

  async function updateAdviceOrderQuery(patch: Partial<AdviceOrderQueryState>, resetPage = false) {
    const encounterId = selectedPatientId.value
    if (!encounterId || !(await confirmDiscardQueryChanges())) return false
    Object.assign(adviceOrderQuery, patch)
    if (resetPage) adviceOrderQuery.pageIndex = 1
    await loadCurrentAdviceOrders(encounterId, false)
    return true
  }

  async function performAdviceStatusAction(
    action: 'sign' | 'withdraw',
    selectedRows: ClinicalOrderDraft[]
  ) {
    const encounterId = selectedPatientId.value
    if (
      !encounterId ||
      !adviceOrderList.value ||
      adviceOrdersActionLoading.value ||
      selectedRows.length === 0
    ) {
      return false
    }

    const requiredStatus = action === 'sign' ? 'DRAFT' : 'SIGNED'
    if (
      selectedRows.some((row) => row.status !== requiredStatus) ||
      selectedRows.some((row) => row.category !== 'DRUG')
    ) {
      ElMessage.warning(action === 'sign' ? '签发只能选择未签发医嘱' : '撤回只能选择已签发医嘱')
      return false
    }

    if (ordersDirty.value && !(await saveCurrentAdviceOrders(false))) return false
    const refreshedRows = selectedRows
      .map((selected) =>
        adviceOrders.value.find(
          (row) =>
            (selected.recordId && row.recordId === selected.recordId) ||
            row.catalogId === selected.catalogId
        )
      )
      .filter((row): row is ClinicalOrderDraft => Boolean(row))
    if (
      refreshedRows.length !== selectedRows.length ||
      refreshedRows.some((row) => row.status !== requiredStatus || !row.recordId)
    ) {
      ElMessage.warning(action === 'sign' ? '签发只能选择未签发医嘱' : '撤回只能选择已签发医嘱')
      return false
    }

    try {
      await ElMessageBox.confirm(
        action === 'sign'
          ? `确认签发选中的 ${refreshedRows.length} 条医嘱吗？`
          : `确认撤回选中的 ${refreshedRows.length} 条医嘱吗？撤回后不能恢复为未签发状态。`,
        action === 'sign' ? '签发医嘱' : '撤回医嘱',
        { type: 'warning', confirmButtonText: '确认', cancelButtonText: '取消' }
      )
    } catch {
      return false
    }

    adviceOrdersActionLoading.value = true
    try {
      const request = {
        encounterId: String(encounterId),
        adviceIds: refreshedRows.map((row) => row.recordId!)
      }
      if (action === 'sign') await signAdviceOrders(request)
      else await withdrawAdviceOrders(request)
      await loadCurrentAdviceOrders(encounterId, false)
      ElMessage.success(action === 'sign' ? '医嘱签发成功' : '医嘱撤回成功')
      return true
    } catch (error) {
      ElMessage.error(error instanceof Error ? error.message : '医嘱状态操作失败')
      return false
    } finally {
      adviceOrdersActionLoading.value = false
    }
  }

  return {
    adviceOrderList,
    adviceOrders,
    adviceOrdersLoading,
    adviceOrdersSaving,
    adviceOrdersActionLoading,
    adviceOrdersError,
    adviceOrdersTotal,
    adviceOrderQuery,
    adviceOrderContext,
    ordersDirty,
    adviceAdapter: outpatientAdviceAdapter,
    loadCurrentAdviceOrders,
    resetAdviceOrders,
    loadAdviceOrderCatalog,
    validateCurrentAdviceOrders,
    saveCurrentAdviceOrders,
    scheduleAdviceOrderAutoSave,
    cancelAdviceOrderAutoSave,
    updateAdviceOrderQuery,
    signSelectedAdviceOrders: (rows: ClinicalOrderDraft[]) =>
      performAdviceStatusAction('sign', rows),
    withdrawSelectedAdviceOrders: (rows: ClinicalOrderDraft[]) =>
      performAdviceStatusAction('withdraw', rows)
  }
}
