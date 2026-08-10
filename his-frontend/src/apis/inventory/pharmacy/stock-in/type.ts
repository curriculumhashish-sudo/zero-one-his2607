import type { PurchaseInboundStatus } from '@/apis/inventory/common/type'
// ==================== 状态映射 ====================

/** 药房入库单 statusName → 前端 PurchaseInboundStatus */
export const STATUS_NAME_MAP: Record<string, PurchaseInboundStatus> = {
  DRAFT: 0,
  PENDING: 1,
  AUDITING: 2,
  REVOKED: 3,
  APPROVED: 4,
  REJECTED: 5
}

/** 状态展示文本（卡片徽章渲染） */
export const STATUS_TEXT: Record<string, string> = {
  DRAFT: '编辑中',
  PENDING: '待审核',
  AUDITING: '审核中',
  REVOKED: '已撤回',
  APPROVED: '同意',
  REJECTED: '驳回'
}

// ==================== 后端响应类型 ====================

/** 药房入库单列表项（后端返回） */
export interface PharmacyInboundRecord {
  id: string
  busNo: string
  statusName: string
  createByName: string
  applyTime: string
  approverName: string
  approveTime: string
  sourceLocationName: string
  purposeLocationName: string
  totalPrice: number
}
