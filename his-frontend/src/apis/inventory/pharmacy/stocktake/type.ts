import type { PurchaseInboundStatus } from '@/apis/inventory/common/type'

// ==================== 状态映射 ====================

/** 盘点单 statusEnum → 前端 PurchaseInboundStatus（兼容两套枚举值） */
export const STOCKTAKE_STATUS_MAP: Record<string, PurchaseInboundStatus> = {
  DRAFT: 0,
  PENDING_APPROVAL: 1,
  WITHDRAWN: 3,
  WITHDRAW: 3,
  APPROVED: 4,
  AGREE: 4,
  REJECTED: 5,
  REJECT: 5
}

/** 状态展示文本（卡片徽章渲染，兼容两套枚举值） */
export const STOCKTAKE_STATUS_TEXT: Record<string, string> = {
  DRAFT: '草稿',
  PENDING_APPROVAL: '待审批',
  WITHDRAWN: '已撤回',
  WITHDRAW: '已撤回',
  APPROVED: '已审批',
  AGREE: '已审批',
  REJECTED: '已驳回',
  REJECT: '已驳回'
}

/** 前端 status 数字 → 后端 statusEnum 字符串（保存/提交时用） */
export const STOCKTAKE_STATUS_ENUM_MAP: Record<number, string> = {
  0: 'DRAFT',
  1: 'PENDING_APPROVAL',
  2: 'PENDING_APPROVAL',
  3: 'WITHDRAWN',
  4: 'APPROVED',
  5: 'REJECTED'
}

// ==================== 后端响应类型 ====================

/** 盘点单列表项（后端返回） */
export interface StocktakeRecord {
  id: string
  busNo: string
  statusEnum: string
  createdBy: string
  createdByName: string
  createdAt: string
  warehouseId: string
  warehouseName: string
  stocktakePharmacy: string
  totalPrice: number
  profitLossPrice: number
  applicantId: string
  applicantName: string
  applyTime: string
  approverId: string
  approverName: string
  approvalTime: string
}

/** 盘点单详情响应（后端 StocktakeDetailVO，GET /api/inventory-check/detail/{id}） */
export interface StocktakeDetail {
  id: string
  busNo: string
  statusEnum: string
  statusDesc: string
  warehouseId: string
  warehouseName: string
  stocktakePharmacy: string
  stocktakeType: string
  totalPrice: number
  totalProfitPrice: number
  totalLossPrice: number
  profitLossPrice: number
  itemCount: number
  createdBy: string
  createdByName: string
  applicantId: string
  applicantName: string
  applyTime: string
  approverId: string
  approverName: string
  approvalTime: string
  approvalRemark: string
  remark: string
  createdAt: string
  items: StocktakeDetailItem[]
}

/** 盘点单明细行（后端 StocktakeItemVO） */
export interface StocktakeDetailItem {
  id: string
  itemId: string
  itemName: string
  specification: string
  manufacturer: string
  lotNumber: string
  bookQuantity: number
  totalQuantity: number
  itemQuantity: number
  unitCode: string
  unitName: string
  price: number
  totalPrice: number
  profitLossPrice: number
  remark: string
}

/** 新增盘点单响应（后端 StocktakeSaveVO，GET /api/inventory-check/new） */
export interface StocktakeCreateResponse {
  id: string
  busNo: string
  statusEnum: string
  createdBy: string
  createdByName: string
  createdAt: string
  approverId: string
  approverName: string
  approvalTime: string
  warehouseId: string
  warehouseName: string
  stocktakePharmacy: string
  itemCount: number
  totalPrice: number
  operateTime: string
  items: StocktakeDetailItem[]
}
