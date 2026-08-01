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
import {
  createPurchaseInbound,
  updatePurchaseInbound,
  removePurchaseInbound,
  submitPurchaseInbound,
  withdrawPurchaseInbound,
  approvePurchaseInbound,
  rejectPurchaseInbound,
} from '@/apis/inventory/pharmacy/stocktake'
import type { PurchaseInboundDetail, PurchaseInboundStatus } from '@/apis/warehouse/purchase-inbound'

export function useApprovalFlow(options: {
  orders: Ref<Record<string, unknown>[]>
  selectedOrder: Ref<Record<string, unknown> | null>
  /** 获取当前右侧表单的明细行数据（MainPane 的 details） */
  getDetails: () => Record<string, unknown>[]
  /** 状态变更后刷新 MainPane */
  refreshMainPane: () => void
}) {
  const { orders, selectedOrder, getDetails, refreshMainPane } = options

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

  /** 操作后同步卡片列表和选中状态 */
  function syncOrder(updated: Record<string, any>, oldOrder?: Record<string, any> | null) {
    // 草稿保存后 id 会变，先移除旧草稿
    if (oldOrder?._draft) {
      const draftIdx = orders.value.findIndex((o) => o._draft && o._clientDraftId === oldOrder._clientDraftId)
      if (draftIdx !== -1) orders.value.splice(draftIdx, 1)
    }
    const idx = orders.value.findIndex((o) => o.id === updated.id)
    if (idx !== -1) {
      orders.value.splice(idx, 1, updated)
    } else {
      orders.value.unshift(updated)
    }
    selectedOrder.value = updated
    // 通知 MainPane 刷新（当 orderId 未变时也需要同步状态到右侧）
    refreshMainPane()
  }

  /** 从 selectedOrder 中提取保存参数 */
  function buildSavePayload(targetStatus: PurchaseInboundStatus): { pharmacy: string; itemType: string; targetStatus: PurchaseInboundStatus; details: PurchaseInboundDetail[] } {
    const o = currentOrder()
    return {
      pharmacy: (o?.sourceLocationId_dictText as string) ?? '',
      itemType: (o?.category as string) ?? '',
      targetStatus,
      details: getDetails() as unknown as PurchaseInboundDetail[],
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
        ? await createPurchaseInbound(payload as any)
        : await updatePurchaseInbound(o.id as string, payload as any)
      syncOrder(result.order as unknown as Record<string, unknown>, o)
      ElMessage.success('保存成功')
    } catch {
      ElMessage.error('保存失败')
    }
  }

  /** 保存并提交审批：编辑中(0) → 审核中(2) */
  async function handleSaveAndSubmit() {
    const o = currentOrder()
    if (!o) return
    const payload = buildSavePayload(2)
    if (!validateBeforeSave(payload as any)) return
    try {
      const result = isDraft(o)
        ? await createPurchaseInbound(payload as any)
        : await updatePurchaseInbound(o.id as string, payload as any)
      const submitted = await submitPurchaseInbound(result.order.id as string)
      syncOrder(submitted as unknown as Record<string, unknown>, o)
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
      await removePurchaseInbound(o.id as string)
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
      const result = await submitPurchaseInbound(o.id as string)
      syncOrder(result as unknown as Record<string, unknown>)
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
      const result = await withdrawPurchaseInbound(o.id as string)
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
      const result = await approvePurchaseInbound(o.id as string)
      syncOrder(result as unknown as Record<string, unknown>)
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
      const result = await rejectPurchaseInbound(o.id as string, reason)
      syncOrder(result as unknown as Record<string, unknown>)
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
