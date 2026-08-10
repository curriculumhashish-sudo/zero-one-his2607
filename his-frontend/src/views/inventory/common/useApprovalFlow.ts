/**
 * 盘点单审批流 composable
 *
 * 管理 7 种操作的状态流转，多个页面复用。
 *
 * 使用方式：
 *   const approval = useApprovalFlow({ orders, selectedOrder, getDetails })
 *   <PurchaseInboundMainPane @action="approval.handleAction" />
 *   // 驳回弹窗：approval.rejectVisible / approval.rejectReason
 */

import { ref, type Ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import type { PurchaseInboundDetail, PurchaseInboundStatus } from '@/apis/warehouse/purchase-inbound'

/** 审批流依赖的 API 函数集合 */
export interface ApprovalFlowApis {
  createPurchaseInbound: (payload: any) => Promise<{ order: Record<string, unknown>; details: any[] }>
  updatePurchaseInbound: (id: string, payload: any) => Promise<{ order: Record<string, unknown>; details: any[] }>
  removePurchaseInbound: (id: string) => Promise<void>
  submitPurchaseInbound: (id: string) => Promise<Record<string, unknown>>
  withdrawPurchaseInbound: (id: string) => Promise<Record<string, unknown>>
  approvePurchaseInbound: (id: string) => Promise<Record<string, unknown>>
  rejectPurchaseInbound: (id: string, reason: string) => Promise<Record<string, unknown>>
}

export function useApprovalFlow(options: {
  orders: Ref<Record<string, unknown>[]>
  selectedOrder: Ref<Record<string, unknown> | null>
  /** 获取当前右侧表单的明细行数据（MainPane 的 details） */
  getDetails: () => Record<string, unknown>[]
  /** 状态变更后刷新 MainPane */
  refreshMainPane: () => void
  /** 获取检索区选中值 */
  getSearchPharmacy: () => string
  /** 获取检索区"目的药房"选中值（stock-in 用，其他为空字符串） */
  getSearchTargetPharmacy: () => string
  /** 获取检索区"供应商"选中值（purchase-inbound2 用，其他为空字符串） */
  getSearchSupplier: () => string
  /** API 函数集（各页面注入自己的 API 模块） */
  apis: ApprovalFlowApis
}) {
  const { orders, selectedOrder, getDetails, refreshMainPane, getSearchPharmacy, getSearchTargetPharmacy, getSearchSupplier, apis } = options

  // ==================== 驳回弹窗 ====================

  const rejectVisible = ref(false)
  const rejectReason = ref('')

  function openRejectDialog() {
    rejectReason.value = ''
    rejectVisible.value = true
  }

  function confirmReject() {
    if (!rejectReason.value.trim()) {
      ElMessage.warning('请填写驳回原因')
      return
    }
    rejectVisible.value = false
    doReject(rejectReason.value.trim())
  }

  function cancelReject() {
    rejectVisible.value = false
  }

  // ==================== 内部工具方法 ====================

  function currentOrder() {
    return selectedOrder.value as Record<string, any> | null
  }

  /** 当前时间字符串 YYYY-MM-DD HH:mm */
  function nowTime(): string {
    const d = new Date()
    const pad = (n: number) => String(n).padStart(2, '0')
    return `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}`
  }

  /** 记录被 patchCardFields 修改过的字段，syncOrder 时优先保留 */
  const patchedFields = new Set<string>()

  /** 操作后同步卡片列表和选中状态 */
  function syncOrder(updated: Record<string, any>, oldOrder?: Record<string, any> | null) {
    if (oldOrder?._draft) {
      const draftIdx = orders.value.findIndex((o) => o._draft && o._clientDraftId === oldOrder._clientDraftId)
      if (draftIdx !== -1) orders.value.splice(draftIdx, 1)
    }
    const idx = orders.value.findIndex((o) => o.id === updated.id)
    // merge: 保留被 patchCardFields 修改过的字段
    const existing = idx !== -1 ? orders.value[idx]! : {}
    const merged = { ...updated }
    for (const k of patchedFields) {
      if (existing[k]) merged[k] = existing[k]
    }
    if (idx !== -1) {
      orders.value.splice(idx, 1, merged)
    } else {
      orders.value.unshift(merged)
    }
    selectedOrder.value = merged
    refreshMainPane()
  }

  /** 对 selectedOrder / orders 中当前卡片打补丁字段 */
  function patchCardFields(patch: Record<string, unknown>) {
    const o = selectedOrder.value
    if (!o) return
    Object.assign(o, patch)
    Object.keys(patch).forEach((k) => patchedFields.add(k))
    const idx = orders.value.findIndex((item) => item.id === o.id)
    if (idx !== -1) orders.value.splice(idx, 1, { ...orders.value[idx], ...patch })
  }

  /** 从 selectedOrder 中提取保存参数 */
  function buildSavePayload(targetStatus: PurchaseInboundStatus) {
    const o = currentOrder()
    return {
      pharmacy: (o?.sourceLocationId_dictText as string) ?? '',
      itemType: (o?.category as string) ?? '',
      targetStatus,
      details: getDetails() as unknown as PurchaseInboundDetail[],
      applicantId_dictText: (o?.applicantId_dictText as string) ?? '',
      sourceLocationId: (o?.sourceLocationId as string) ?? '',
      sourceLocationId_dictText: (o?.sourceLocationId_dictText as string) ?? '',
      purposeLocationId: (o?.purposeLocationId as string) ?? '',
      purposeLocationId_dictText: (o?.purposeLocationId_dictText as string) ?? getSearchTargetPharmacy(),
      targetPharmacy: getSearchTargetPharmacy(),
      // purchase-inbound2 专用：进货单 addInbound 需要的库房和供应商
      // 优先从当前选中单据取值（handleQueueAdd 在 resetSearchValues 前已写入），再回退检索区
      warehouse: (o?.sourceLocationId_dictText as string) ?? getSearchPharmacy(),
      supplierId_dictText: (o?.supplierId_dictText as string) ?? '',
      supplier: (o?.supplierId_dictText as string) ?? getSearchSupplier(),
    }
  }

  // ==================== 7 种操作 ====================

  /** 是否草稿（新增单据在 mock 中不存在，需 create 而非 update） */
  function isDraft(o: Record<string, any>): boolean {
    return Boolean(o._draft)
  }

  /** 保存单据：编辑中(0) → 待审核(1) */
  async function handleSave() {
    const o = currentOrder()
    if (!o) return
    const payload = buildSavePayload(1)
    if (!validateBeforeSave(payload as any)) return
    try {
      const result = isDraft(o)
        ? await apis.createPurchaseInbound(payload as any)
        : await apis.updatePurchaseInbound(o.id as string, payload as any)
      syncOrder(result.order as unknown as Record<string, unknown>, o)
      patchCardFields({ applicantId_dictText: 'admin', sourceLocationId_dictText: (o?.sourceLocationId_dictText as string) || getSearchPharmacy(), purposeLocationId_dictText: getSearchTargetPharmacy(), supplierId_dictText: (o?.supplierId_dictText as string) || getSearchSupplier() })
      ElMessage.success('保存成功')
    } catch {
      ElMessage.error('保存失败')
    }
  }

  /** 保存并提交审批：编辑中(0) → 待审核(1) → 审核中(2) */
  async function handleSaveAndSubmit() {
    const o = currentOrder()
    if (!o) return
    const payload = buildSavePayload(1)
    if (!validateBeforeSave(payload as any)) return
    try {
      const result = isDraft(o)
        ? await apis.createPurchaseInbound(payload as any)
        : await apis.updatePurchaseInbound(o.id as string, payload as any)
      const submitted = await apis.submitPurchaseInbound(result.order.id as string)
      syncOrder(submitted as unknown as Record<string, unknown>, o)
      patchCardFields({ applicantId_dictText: 'admin', sourceLocationId_dictText: (o?.sourceLocationId_dictText as string) || getSearchPharmacy(), purposeLocationId_dictText: getSearchTargetPharmacy(), supplierId_dictText: (o?.supplierId_dictText as string) || getSearchSupplier(), applyTime: nowTime() })
      ElMessage.success('已保存并提交审批')
    } catch {
      ElMessage.error('操作失败')
    }
  }

  /** 删除单据：编辑中/待审核 → 删除卡片 */
  async function handleDelete() {
    const o = currentOrder()
    if (!o) return
    try {
      await ElMessageBox.confirm('确定要删除该单据吗？', '确认删除', {
        confirmButtonText: '删除',
        cancelButtonText: '取消',
        type: 'warning',
      })
      await apis.removePurchaseInbound(o.id as string)
      const idx = orders.value.findIndex((item) => item.id === o.id)
      if (idx !== -1) orders.value.splice(idx, 1)
      selectedOrder.value = orders.value.length > 0 ? orders.value[0]! : null
      ElMessage.success('删除成功')
    } catch (err) {
      if (err !== 'cancel') ElMessage.error('删除失败')
    }
  }

  /** 提交审批：待审核(1) / 已撤回(3) → 审核中(2) */
  async function handleSubmit() {
    const o = currentOrder()
    if (!o) return
    try {
      const result = await apis.submitPurchaseInbound(o.id as string)
      syncOrder(result as unknown as Record<string, unknown>)
      patchCardFields({ applyTime: nowTime() })
      ElMessage.success('已提交审批')
    } catch {
      ElMessage.error('提交失败')
    }
  }

  /** 撤回：审核中(2) → 已撤回(3) */
  async function handleWithdraw() {
    const o = currentOrder()
    if (!o) return
    try {
      const result = await apis.withdrawPurchaseInbound(o.id as string)
      syncOrder(result as unknown as Record<string, unknown>)
      ElMessage.success('已撤回')
    } catch {
      ElMessage.error('撤回失败')
    }
  }

  /** 审批通过：审核中(2) → 同意(4) */
  async function handleApprove() {
    const o = currentOrder()
    if (!o) return
    try {
      const result = await apis.approvePurchaseInbound(o.id as string)
      syncOrder(result as unknown as Record<string, unknown>)
      patchCardFields({ approverId_dictText: 'admin', approvalTime: nowTime() })
      ElMessage.success('审批通过')
    } catch {
      ElMessage.error('审批失败')
    }
  }

  /** 驳回：弹出原因输入框 → 审核中(2) → 驳回(5) */
  function handleReject() {
    const o = currentOrder()
    if (!o) return
    openRejectDialog()
  }

  async function doReject(reason: string) {
    const o = currentOrder()
    if (!o) return
    try {
      const result = await apis.rejectPurchaseInbound(o.id as string, reason)
      syncOrder(result as unknown as Record<string, unknown>)
      patchCardFields({ approverId_dictText: 'admin', approvalTime: nowTime() })
      ElMessage.success('已驳回')
    } catch {
      ElMessage.error('驳回失败')
    }
  }

  // ==================== 表单保存前验证 ====================

  function validateBeforeSave(payload: { details: Record<string, unknown>[] }): boolean {
    if (!payload.details || payload.details.length === 0) {
      ElMessage.warning('请至少添加一行盘点明细')
      return false
    }
    for (let i = 0; i < payload.details.length; i++) {
      const row = payload.details[i]!
      if (!row.itemName) {
        ElMessage.warning(`第${i + 1}行请选择项目`)
        return false
      }
    }
    return true
  }

  // ==================== 统一入口 ====================

  /** MainPane @action 的统一处理函数 */
  async function handleAction(action: string) {
    switch (action) {
      case 'save':          return handleSave()
      case 'saveAndSubmit': return handleSaveAndSubmit()
      case 'delete':        return handleDelete()
      case 'submit':        return handleSubmit()
      case 'withdraw':      return handleWithdraw()
      case 'approve':       return handleApprove()
      case 'reject':        return handleReject()
    }
  }

  return {
    handleAction,
    rejectVisible,
    rejectReason,
    confirmReject,
    cancelReject,
  }
}
