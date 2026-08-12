import type { PurchaseInboundStatus } from '@/apis/inventory/common/type'
// ==================== 状态映射 ====================

/** 药房入库单 statusName/statusEnum → 前端 PurchaseInboundStatus */
export const STATUS_NAME_MAP: Record<string, PurchaseInboundStatus> = {
  // 列表接口 statusName 为中文
  编辑中: 0,
  待审批: 1,
  审批中: 2,
  已撤回: 3,
  已通过: 4,
  已驳回: 5,
  // 详情/创建接口 statusEnum 为英文
  DRAFT: 0,
  PENDING_APPROVAL: 1,
  APPROVAL: 2,
  WITHDRAW: 3,
  AGREE: 4,
  REJECT: 5
}

/** 状态展示文本（卡片徽章渲染） */
export const STATUS_TEXT: Record<string, string> = {
  编辑中: '编辑中',
  待审批: '待审核',
  审批中: '审核中',
  已撤回: '已撤回',
  已通过: '同意',
  已驳回: '驳回'
}

/** 前端 status 数字 → 后端 statusEnum 字符串 */
export const STATUS_ENUM_MAP: Record<number, string> = {
  0: 'DRAFT',
  1: 'PENDING_APPROVAL',
  2: 'APPROVAL',
  3: 'WITHDRAW',
  4: 'AGREE',
  5: 'REJECT'
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

/** 入库单详情响应（后端 PurchaseInboundDetailVO，GET /j7/inbound-approval/detail/{id}） */
export interface PharmacyInboundDetail {
  id: string
  busNo: string
  supplierId: string
  supplierName: string
  purposeLocationId: string
  purposeLocationName: string
  totalPrice: number
  statusEnum: string
  applyTime: string
  remake: string
  createByName: string
  approverName: string
  approveTime: string
  items: PharmacyInboundDetailItem[]
}

/** 入库单明细行（后端 PurchaseInboundItemVO） */
export interface PharmacyInboundDetailItem {
  id: string
  itemId: string
  itemName: string
  spec: string
  manufacturer: string
  lotNumber: string
  quantity: number
  unit: string
  price: number
  subTotal: number
  traceCode: string
  remark: string
  productionDate: string
  expirationDate: string
}

/** 新增入库单响应（POST /j7/inbound-approval/addInStockOrder 返回 data） */
export interface PharmacyInboundCreateResponse {
  id: string
  orderNo: string
  sourceLocationId: string
  warehouseId: string
  projectType: string
  supplierId: number
  inDate: string
  remark: string
  statusEnum: string
  createdBy: string
  createdAt: string
  approverId: string
  approvalTime: string
  totalPrice: number
  items: PharmacyInboundCreateItem[]
}

/** 新增入库单响应明细行 */
export interface PharmacyInboundCreateItem {
  id: number
  seqNo: number
  itemId: string
  drugName: string
  spec: string
  manufacturer: string
  quantity: number
  unitPrice: number
  totalAmount: number
  batchNo: string
  expireDate: string
  traceNo: string
  itemRemark: string
}
