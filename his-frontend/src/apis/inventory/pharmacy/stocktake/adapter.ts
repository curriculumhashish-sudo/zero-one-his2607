import type {
  PurchaseInboundOrder,
  PurchaseInboundDetail,
  PurchaseInboundDetailResult,
  PurchaseInboundSavePayload,
  PurchaseInboundStatus,
  SupplierItem
} from '@/apis/inventory/common/type'

/** 后端状态字符串 → 前端状态数字 */
const STATUS_MAP: Record<string, PurchaseInboundStatus> = {
  DRAFT: 0,
  PENDING_APPROVAL: 2,
  WITHDRAW: 3,
  AGREE: 4,
  REJECT: 5
}

/** 前端状态数字 → 后端状态文本 */
export const STATUS_TEXT: Record<number, string> = {
  0: '编辑中',
  2: '审核中',
  3: '已撤回',
  4: '同意',
  5: '驳回'
}

/** 后端盘点单列表项原始结构 */
export interface BackendInventoryCheckListItem {
  checkNo?: string
  checkType?: string
  checkDate?: string
  drugCount?: number
  status?: string
  checker?: string
  creatorName?: string
  approverId?: string
  approverName?: string
  approvalTime?: string
  purposeLocationId?: string
  purposeLocationName?: string
  totalAmount?: number
}

/** 后端盘点单详情原始结构 */
export interface BackendInventoryCheckDetail {
  checkNo?: string
  checkType?: string
  checkDate?: string
  status?: string
  rejectReason?: string
  remark?: string
  id?: string
  creatorName?: string
  approverId?: string
  approverName?: string
  approvalTime?: string
  purposeLocationId?: string
  purposeLocationName?: string
  totalAmount?: number
  drugList?: BackendCheckDrugItem[]
}

/** 后端盘点药品明细项原始结构 */
export interface BackendCheckDrugItem {
  detailId?: string
  drugName?: string
  specification?: string
  batchNo?: string
  systemQty?: number
  actualQty?: number
  diffQty?: number
  diffReason?: string
  manufacturer?: string
  unitPrice?: number
  actualAmount?: number
  profitLossAmount?: number
  remark?: string
}

/** 后端保存请求 DTO */
export interface BackendInventoryCheckSaveDTO {
  busNo?: string
  purposeLocationId?: string
  intentEnum?: string
  reason?: string
  remark?: string
  drugItems: BackendInventoryCheckDrugItemDTO[]
}

export interface BackendInventoryCheckDrugItemDTO {
  id?: string
  itemId?: string
  itemTable?: string
  totalQuantity?: number
  itemQuantity?: number
  unitCode?: string
  lotNumber?: string
  price?: number
  totalPrice?: number
  profitLossPrice?: number
  reason?: string
  remark?: string
}

/** 适配后端列表项 → 前端单据对象 */
export function adaptBackendListItem(item: BackendInventoryCheckListItem): PurchaseInboundOrder {
  const statusNum = STATUS_MAP[item.status ?? 'DRAFT'] ?? 0
  return {
    id: item.checkNo ?? '',
    status: statusNum,
    category: item.checkType ?? '',
    busNo: item.checkNo ?? '',
    statusEnum: statusNum,
    statusEnum_enumText: STATUS_TEXT[statusNum] ?? '编辑中',
    applicantId_dictText: item.creatorName ?? '',
    applyTime: item.checkDate ?? '',
    approverId_dictText: item.approverName ?? '',
    approvalTime: item.approvalTime ?? '',
    purposeLocationId_dictText: item.purposeLocationName ?? '',
    totalAmount: item.totalAmount ?? 0
  }
}

/** 适配后端详情 → 前端详情结果 */
export function adaptBackendDetail(detail: BackendInventoryCheckDetail): PurchaseInboundDetailResult {
  const statusNum = STATUS_MAP[detail.status ?? 'DRAFT'] ?? 0
  const order: PurchaseInboundOrder = {
    id: detail.checkNo ?? '',
    status: statusNum,
    category: detail.checkType ?? '',
    busNo: detail.checkNo ?? '',
    statusEnum: statusNum,
    statusEnum_enumText: STATUS_TEXT[statusNum] ?? '编辑中',
    applicantId_dictText: detail.creatorName ?? '',
    applyTime: detail.checkDate ?? '',
    approverId_dictText: detail.approverName ?? '',
    approvalTime: detail.approvalTime ?? '',
    purposeLocationId_dictText: detail.purposeLocationName ?? '',
    totalAmount: detail.totalAmount ?? 0,
    rejectReason: detail.rejectReason
  }
  const details: PurchaseInboundDetail[] = (detail.drugList ?? []).map(adaptBackendDrugItem)
  return { order, details }
}

/** 适配后端药品明细 → 前端明细 */
export function adaptBackendDrugItem(item: BackendCheckDrugItem): PurchaseInboundDetail {
  return {
    id: item.detailId ?? '',
    itemCode: '',
    itemName: item.drugName ?? '',
    specification: item.specification ?? '',
    manufacturer: item.manufacturer ?? '',
    batchNo: item.batchNo ?? '',
    bookQuantity: item.systemQty ?? 0,
    actualQuantity: item.actualQty ?? 0,
    actualUnit: '',
    profitLoss: item.diffQty ?? 0,
    unitPrice: item.unitPrice ?? 0,
    actualAmount: item.actualAmount ?? 0,
    profitLossAmount: item.profitLossAmount ?? 0,
    remark: item.remark ?? ''
  }
}

/** 适配前端保存载荷 → 后端保存 DTO */
export function adaptSavePayloadToBackend(
  payload: PurchaseInboundSavePayload,
  busNo?: string
): BackendInventoryCheckSaveDTO {
  return {
    busNo: busNo || undefined,
    purposeLocationId: payload.pharmacy || undefined,
    intentEnum: payload.itemType || undefined,
    drugItems: payload.details.map(d => ({
      id: d.id || undefined,
      itemId: d.itemCode || undefined,
      itemTable: 'med_medication_definition',
      totalQuantity: d.actualQuantity,
      itemQuantity: d.profitLoss,
      unitCode: d.actualUnit || undefined,
      lotNumber: d.batchNo || undefined,
      price: d.unitPrice,
      totalPrice: d.actualAmount,
      profitLossPrice: d.profitLossAmount,
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
