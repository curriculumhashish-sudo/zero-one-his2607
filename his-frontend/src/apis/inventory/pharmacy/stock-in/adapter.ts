import type {
  PurchaseInboundOrder,
  PurchaseInboundDetail,
  PurchaseInboundDetailResult,
  PurchaseInboundSavePayload,
  PurchaseInboundStatus,
  SupplierItem
} from '@/apis/inventory/common/type'

/**
 * 后端入库单状态字符串 → 前端状态数字
 *
 * 后端状态枚举：
 *   DRAFT             草稿（编辑中）
 *   PENDING_APPROVAL  待审批（编辑中可提交）
 *   WITHDRAW          已撤回
 *   AGREE             已入库（同意）
 *   REJECT            已驳回
 */
const STATUS_MAP: Record<string, PurchaseInboundStatus> = {
  DRAFT: 0,
  PENDING_APPROVAL: 1,
  WITHDRAW: 3,
  AGREE: 2,
  REJECT: 3
}

/** 前端状态数字 → 显示文本 */
export const STATUS_TEXT: Record<number, string> = {
  0: '编辑中',
  1: '待审批',
  2: '已入库',
  3: '已驳回'
}

/** 后端入库单列表项原始结构 */
export interface BackendStockInListItem {
  orderNo?: string
  supplierName?: string
  stockInDate?: string
  drugCount?: number
  totalAmount?: number
  status?: string
  creator?: string
  approverId?: string
  approverName?: string
  approvalTime?: string
  sourceLocationId?: string
  sourceLocationName?: string
  purposeLocationId?: string
  purposeLocationName?: string
}

/** 后端入库单详情原始结构 */
export interface BackendStockInDetail {
  orderNo?: string
  supplierName?: string
  stockInType?: string
  stockInDate?: string
  status?: string
  rejectReason?: string
  remark?: string
  id?: string
  creator?: string
  creatorName?: string
  approverId?: string
  approverName?: string
  approvalTime?: string
  sourceLocationId?: string
  sourceLocationName?: string
  purposeLocationId?: string
  purposeLocationName?: string
  totalAmount?: number
  drugList?: BackendStockInDrugItem[]
}

/** 后端入库药品明细项原始结构 */
export interface BackendStockInDrugItem {
  detailId?: string
  drugName?: string
  specification?: string
  manufacturer?: string
  batchNo?: string
  expiryDate?: string
  unit?: string
  quantity?: number
  unitPrice?: number
  totalAmount?: number
  traceCode?: string
  remark?: string
}

/** 后端保存请求 DTO */
export interface BackendStockInSaveDTO {
  orderNo?: string
  id?: string
  sourceLocationId?: string
  purposeLocationId?: string
  supplierName?: string
  stockInType?: string
  remark?: string
  drugList: BackendStockInDrugItemDTO[]
}

export interface BackendStockInDrugItemDTO {
  detailId?: string
  drugName?: string
  specification?: string
  manufacturer?: string
  batchNo?: string
  expiryDate?: string
  unit?: string
  quantity?: number
  unitPrice?: number
  totalAmount?: number
  traceCode?: string
  remark?: string
}

/** 适配后端列表项 → 前端单据对象 */
export function adaptBackendListItem(item: BackendStockInListItem): PurchaseInboundOrder {
  const statusNum = STATUS_MAP[item.status ?? 'DRAFT'] ?? 0
  return {
    id: item.orderNo ?? '',
    docNo: item.orderNo ?? '',
    date: item.stockInDate ?? '',
    handler: item.creator ?? '',
    counterparty: item.supplierName ?? '',
    status: statusNum,
    type: item.purposeLocationName ?? '',
    category: item.sourceLocationName ?? '',
    rejectReason: undefined
  }
}

/** 适配后端详情 → 前端详情结果 */
export function adaptBackendDetail(detail: BackendStockInDetail): PurchaseInboundDetailResult {
  const statusNum = STATUS_MAP[detail.status ?? 'DRAFT'] ?? 0
  const order: PurchaseInboundOrder = {
    id: detail.orderNo ?? '',
    docNo: detail.orderNo ?? '',
    date: detail.stockInDate ?? '',
    handler: detail.creatorName ?? detail.creator ?? '',
    counterparty: detail.supplierName ?? '',
    status: statusNum,
    type: detail.purposeLocationName ?? '',
    category: detail.sourceLocationName ?? '',
    rejectReason: detail.rejectReason
  }
  const details: PurchaseInboundDetail[] = (detail.drugList ?? []).map(adaptBackendDrugItem)
  return { order, details }
}

/** 适配后端药品明细 → 前端明细 */
export function adaptBackendDrugItem(item: BackendStockInDrugItem): PurchaseInboundDetail {
  return {
    id: item.detailId ?? '',
    itemCode: '',
    itemName: item.drugName ?? '',
    specification: item.specification ?? '',
    manufacturer: item.manufacturer ?? '',
    batchNo: item.batchNo ?? '',
    bookQuantity: 0,
    actualQuantity: item.quantity ?? 0,
    actualUnit: item.unit ?? '',
    profitLoss: 0,
    unitPrice: item.unitPrice ?? 0,
    actualAmount: item.totalAmount ?? 0,
    profitLossAmount: 0,
    remark: item.remark ?? ''
  }
}

/** 适配前端保存载荷 → 后端保存 DTO */
export function adaptSavePayloadToBackend(
  payload: PurchaseInboundSavePayload,
  orderNo?: string
): BackendStockInSaveDTO {
  return {
    orderNo: orderNo || undefined,
    sourceLocationId: payload.pharmacy || undefined,
    purposeLocationId: payload.pharmacy || undefined,
    supplierName: payload.pharmacy || undefined,
    stockInType: payload.itemType || undefined,
    drugList: payload.details.map(d => ({
      detailId: d.id || undefined,
      drugName: d.itemName || undefined,
      specification: d.specification || undefined,
      manufacturer: d.manufacturer || undefined,
      batchNo: d.batchNo || undefined,
      unit: d.actualUnit || undefined,
      quantity: d.actualQuantity,
      unitPrice: d.unitPrice,
      totalAmount: d.actualAmount,
      traceCode: '',
      remark: d.remark || undefined
    }))
  }
}

/** 适配前端药品选项 → 后端药品查询参数（保留兼容） */
export function adaptSupplierItem(item: Record<string, any>): SupplierItem {
  return {
    itemCode: item.itemCode ?? item.code ?? '',
    itemName: item.itemName ?? item.name ?? '',
    manufacturer: item.manufacturer ?? '',
    purchasePrice: Number(item.purchasePrice ?? 0),
    retailPrice: Number(item.retailPrice ?? 0),
    packageUnit: item.packageUnit ?? '',
    specification: item.specification ?? '',
    stockQuantity: Number(item.stockQuantity ?? 0),
    approvalNo: item.approvalNo ?? ''
  }
}
