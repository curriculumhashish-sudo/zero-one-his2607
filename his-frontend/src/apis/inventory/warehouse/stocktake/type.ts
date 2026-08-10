/** 后端列表查询请求参数 */
export interface StocktakeQuery {
  stocktakeNo?: string
  startDate?: string
  endDate?: string
  pharmacy?: string
  pageIndex: number
  pageSize: number
}

/** 后端列表项 PharmacyInventoryVO */
export interface RawStocktakeItem {
  busNo: string
  statusEnum: string
  statusEnumText: string
  purposeLocationId: string
  purposeLocationName: string
  itemCount: number
  profitLossQuantitySum: number
  actualAmountSum: number
  applicantId: string
  applicantName: string
  applyTime: string
  approverId: string
  approverName: string
  approvalTime: string
  occurrenceTime: string
}

/** 后端明细行 PharmacyInventoryItemVO（/stocktake/detail 响应 lines） */
export interface RawStocktakeLine {
  lineId: string
  itemTable: string
  itemId: string
  productName: string
  specification: string
  manufacturer: string
  lotNumber: string
  expirationDate: string
  unitCode: string
  unitName?: string
  bookQuantity: number
  profitLossQuantity: number
  actualQuantity: number
  actualAmount: number
  profitLossAmount: number
  price: number
  inventoryItemId: string
  supplierId: string
  traceNo: string
  remake: string
  occurrenceTime: string
}

/** 后端详情响应 StocktakeDetailVO */
export interface RawStocktakeDetail {
  busNo: string
  statusEnum: string
  purposeLocationId: string
  purposeLocationName: string
  practitionerId: string
  practitionerName: string
  creatorId: string
  creatorName: string
  applicantId: string
  applicantName: string
  applyTime: string
  approverId: string
  approverName: string
  approvalTime: string
  categoryEnum: string
  totalAmount: number
  lines: RawStocktakeLine[]
}

/** 新增请求 DTO POST /stocktake/create */
export interface StocktakeAddDTO {
  purposeLocationId: string
  practitionerId?: string
  categoryEnum: string
}

/** 保存明细行 DTO PUT /stocktake/{busNo}/save */
export interface StocktakeSaveLineDTO {
  inventoryItemId: string
  actualQuantity: number
  profitLossQuantity: number
  unitCode: string
  lotNumber?: string
  traceNo?: string
  remake?: string
}

/** 保存请求 DTO PUT /stocktake/{busNo}/save */
export interface StocktakeSaveDTO {
  busNo: string
  statusEnum?: string
  purposeLocationId: string
  purposeLocationName?: string
  practitionerId?: string
  practitionerName?: string
  creatorId?: string
  creatorName?: string
  applicantId?: string
  applicantName?: string
  approverId?: string
  approverName?: string
  applyTime?: string
  approvalTime?: string
  totalAmount?: number
  categoryEnum?: string
  lines: StocktakeSaveLineDTO[]
}

/** 审批结果响应 */
export interface StocktakeApprovalResult {
  success: boolean
  statusEnum: string
}

/** 提交审批 DTO POST /stocktake/submit-approval */
export interface StocktakeSubmitDTO {
  busNo: string
  approverId: string
}

/** 撤回审批 DTO POST /stocktake/withdraw-approval */
export interface StocktakeWithdrawDTO {
  busNo: string
}

/** 审批通过 DTO POST /stocktake/approve */
export interface StocktakeApproveDTO {
  busNo: string
  approverId?: string
  approvalClock?: string
  contractBusNo?: string
}

/** 驳回审批 DTO POST /stocktake/reject */
export interface StocktakeRejectDTO {
  busNo: string
  approverId?: string
  approvalClock?: string
  changeReason: string
  changeReasonCode?: string
}
