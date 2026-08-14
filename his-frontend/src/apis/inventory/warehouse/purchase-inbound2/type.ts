/** 后端列表项 InboundReceiptListVO */
export interface RawInboundItem {
  busNo: string
  supplierName: string
  statusEnum: string
  creatorName: string
  approverName: string
  totalAmount: number
  approvalTime: string
  applyTime: string
  purposeLocationName: string
}

/** 后端明细行 InboundReceiptDetailLineVO */
export interface RawInboundLine {
  id: string
  itemTable: string
  itemId: string
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
  productName: string
  specification: string
  manufacturer: string
}

/** 后端详情响应 InboundReceiptDetailVO */
export interface RawInboundDetail {
  busNo: string
  originalBusNo: string
  typeEnum: string
  statusEnum: string
  supplierId: string
  supplierName: string
  sourceLocationId: string
  purposeLocationId: string
  purposeLocationName: string
  sourceTypeEnum: string
  purposeTypeEnum: string
  categoryEnum: string
  rowVersion: number
  applyTime: string
  applicantId: string
  applicantName: string
  approverId: string
  approverName: string
  creatorId: string
  creatorName: string
  practitionerId: string
  approvalTime: string
  reason: string
  totalAmount: number
  lines: RawInboundLine[]
}

/** 新增请求 DTO POST /inbound/add */
export interface InboundAddDTO {
  purposeLocationId: string
  supplierId: string
  practitionerId?: string
  originalBusNo?: string
  reason?: string
}

/** 保存明细行 */
export interface InboundSaveLineDTO {
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

/** 保存请求 DTO PUT /inbound/save */
export interface InboundSaveDTO {
  busNo: string
  originalBusNo?: string
  typeEnum: string
  categoryEnum: string
  statusEnum?: string
  sourceTypeEnum?: string
  sourceLocationId?: string
  purposeTypeEnum: string
  purposeLocationId: string
  purposeLocationName?: string
  supplierId: string
  supplierName?: string
  practitionerId?: string
  creatorId?: string
  creatorName?: string
  applicantId?: string
  applicantName?: string
  approverId?: string
  approverName?: string
  applyTime?: string
  approvalTime?: string
  totalAmount?: number
  reason?: string
  version: number
  lines: InboundSaveLineDTO[]
}

/** 审批结果响应 */
export interface InboundApprovalResult {
  success: boolean
  statusEnum: string
}

/** 提交审批 DTO POST /inbound/submitInboundReceiptApproval */
export interface InboundSubmitDTO {
  busNo: string
  approverId: string
  applyClock?: string
}

/** 撤回审批 DTO */
export interface InboundWithdrawDTO {
  busNo: string
}

/** 审批通过 DTO */
export interface InboundApproveDTO {
  busNo: string
  approverId?: string
  approvalClock?: string
  contractBusNo?: string
}

/** 驳回审批 DTO */
export interface InboundRejectDTO {
  busNo: string
  rejectReason: string
  approverId?: string
  approvalClock?: string
}

/** 删除请求 DTO */
export interface InboundDeleteDTO {
  busNo: string
}
