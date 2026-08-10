/** 后端接口 StockOutListVO 原始响应字段 */
export interface RawStockOutItem {
  busNo: string
  typeEnum: string
  statusEnum: string
  statusEnumText: string
  sourceLocationId: string
  sourceLocationName: string
  purposeLocationId: string
  purposeLocationName: string
  applyTime: string
  approvalTime: string
  occurrenceTime: string
  applicantId: string
  createdBy: string
  approverId: string
  approverName: string
  totalAmount: number
  originalBusNo: string
}

/** 后端详情接口 StockOutDetailVO.lines 行数据 */
export interface RawStockOutLine {
  id: string
  itemTable: string
  itemId: string
  productName: string
  specification: string
  manufacturer: string
  itemQuantity: number
  unitCode: string
  unitCodeDictText: string
  lotNumber: string
  traceNo: string
  price: number
  totalPrice: number
  occurrenceTime: string
  startTime: string
  endTime: string
  remake: string
}

/** 后端详情接口 StockOutDetailVO 响应体 */
export interface RawStockOutDetail {
  busNo: string
  originalBusNo: string
  typeEnum: string
  statusEnum: string
  categoryEnum: string
  sourceLocationId: string
  sourceLocationName: string
  purposeLocationId: string
  purposeLocationName: string
  sourceTypeEnum: string
  purposeTypeEnum: string
  version: number
  applyTime: string
  approvalTime: string
  practitionerId: string
  practitionerName: string
  creatorId: string
  creatorName: string
  applicantId: string
  applicantName: string
  approverId: string
  approverName: string
  remark: string
  totalAmount: number
  lines: RawStockOutLine[]
}

/** 前端 列表查询 请求参数（对接后端 /getPharmacy） */
export interface StockOutQuery {
  busNokey?: string
  startDate?: string
  endDate?: string
  drugDatabase?: string
  pharmacyWarehouse?: string
  pageIndex: number
  pageSize: number
}

/** 新增出库单请求体 POST /stockout/add */
export interface StockOutAddDTO {
  sourceLocationId: string
  purposeLocationId: string
  practitionerId?: string
  relatedReqNo?: string
  remark?: string
}

/** 保存出库单明细行 */
export interface StockOutSaveLineDTO {
  itemTable: string
  itemId: string
  itemQuantity: number
  unitCode: string
  productName?: string
  specification?: string
  manufacturer?: string
  lotNumber?: string
  traceNo?: string
  price?: number
  totalPrice?: number
  occurrenceTime?: string
  startTime?: string
  endTime?: string
  remake?: string
}

/** 审批类接口通用响应体 */
export interface StockOutApprovalResult {
  success: boolean
  statusEnum: string
}

/** 提交审批请求体 POST /stockout/submit-approval */
export interface StockOutSubmitDTO {
  busNo: string
  approverId: string
}

/** 撤回审批请求体 POST /stockout/withdraw-approval */
export interface StockOutWithdrawDTO {
  busNo: string
}

/** 审批通过请求体 POST /stockout/agree */
export interface StockOutApproveDTO {
  busNo: string
  approverId?: string
  approvalClock?: string
  contractBusNo?: string
}

/** 驳回审批请求体 POST /stockout/reject */
export interface StockOutRejectDTO {
  busNo: string
  approverId?: string
  approvalClock?: string
  rejectReason: string
}

/** 保存出库单完整请求体 POST /stockout/save */
export interface StockOutSaveDTO {
  busNo: string
  originalBusNo?: string
  typeEnum: string
  categoryEnum: string
  statusEnum?: string
  sourceTypeEnum: string
  sourceLocationId: string
  sourceLocationName?: string
  purposeTypeEnum: string
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
  remark?: string
  version: number
  lines: StockOutSaveLineDTO[]
}
