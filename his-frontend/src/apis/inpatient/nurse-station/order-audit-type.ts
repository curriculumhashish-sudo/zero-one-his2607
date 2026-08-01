/** 医嘱校对状态页签。 */
export type NurseStationOrderAuditStatus = 'PASSED' | 'PENDING' | 'STOPPED' | 'SUSPENDED'

/** 医嘱长期、临时筛选。 */
export type NurseStationOrderAuditTherapyFilter = 'ALL' | 'LONG_TERM' | 'TEMPORARY'

/** 医嘱药品分类筛选。 */
export type NurseStationOrderAuditMedicineCategory = 'TCM' | 'WEST'

/** 可被护士站校对的医嘱请求类型。 */
export type NurseStationOrderAuditOrderType = 'MED_REQUEST' | 'SERVICE_REQUEST'

/** 医嘱校对列表查询参数。 */
export interface NurseStationOrderAuditQueryDTO {
  /** 左侧已选住院就诊 ID。 */
  encounterIds: string[]
  /** 当前状态页签。 */
  auditStatus: NurseStationOrderAuditStatus
  /** 长期或临时筛选。 */
  therapyType: NurseStationOrderAuditTherapyFilter
  /** 西药或中药筛选。 */
  medicineCategory: NurseStationOrderAuditMedicineCategory
}

/** 医嘱校对列表行。 */
export interface NurseStationOrderAuditRowVO {
  /** 表格稳定行键。 */
  listRowKey: string
  /** 医嘱请求 ID。 */
  requestId: string
  /** 医嘱请求类型。 */
  orderType: NurseStationOrderAuditOrderType
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 当前状态。 */
  statusEnum: NurseStationOrderAuditStatus | 'REJECTED'
  /** 当前状态名称。 */
  statusDisplay: string
  /** 长期、临时或出院带药编码。 */
  therapyEnum: string
  /** 长期、临时或出院带药名称。 */
  therapyDisplay: string
  /** 医嘱名称。 */
  adviceDisplayName: string
  /** 规格。 */
  specificationText?: string
  /** 剂量或数量。 */
  dosageDisplay?: string
  /** 用法。 */
  usageDisplay?: string
  /** 频次。 */
  frequencyDisplay?: string
  /** 签发时间。 */
  authoredTime?: string
  /** 停嘱时间。 */
  stopTime?: string
  /** 开立医生。 */
  requesterName?: string
  /** 执行科室。 */
  performerLocationDisplay?: string
  /** 医嘱分类。 */
  categoryEnum?: string
  /** 医嘱组号。 */
  groupNo?: number
  /** 组内顺序。 */
  sortNumber?: number
  /** 是否为中药饮片整方折叠行。 */
  tcmFlag?: boolean
  /** 中药处方号。 */
  prescriptionNo?: string
  /** 中药整方包含的药品申请 ID。 */
  lineRequestIds?: string[]
  /** 是否自备药。 */
  sufferingFlag?: boolean
  /** 中药付数。 */
  chineseHerbsDoseQuantity?: number
  /** 乐观锁版本。 */
  version: number
}

/** 单个患者的医嘱分组。 */
export interface NurseStationOrderAuditPatientGroupVO {
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 当前筛选下的医嘱列表。 */
  orders: NurseStationOrderAuditRowVO[]
}

/** 医嘱校对列表响应。 */
export interface NurseStationOrderAuditListVO {
  groups: NurseStationOrderAuditPatientGroupVO[]
}

/** 批量校对或退回的单条锚点。 */
export interface NurseStationOrderAuditWriteItemDTO {
  /** 医嘱请求类型。 */
  orderType: NurseStationOrderAuditOrderType
  /** 医嘱请求 ID。 */
  requestId: string
  /** 查询时取得的乐观锁版本。 */
  expectedVersion: number
}

/** 批量校对通过请求。 */
export interface NurseStationVerifyOrdersDTO {
  /** 本次写操作幂等键。 */
  idempotencyKey: string
  /** 待校对医嘱，单批最多 50 条。 */
  items: NurseStationOrderAuditWriteItemDTO[]
}

/** 批量退回请求。 */
export interface NurseStationRejectOrdersDTO extends NurseStationVerifyOrdersDTO {
  /** 统一退回原因，最多 500 字。 */
  reason?: string
}

/** 批量写操作的逐条结果。 */
export interface NurseStationOrderAuditWriteResultVO {
  /** 医嘱请求类型。 */
  orderType: NurseStationOrderAuditOrderType
  /** 医嘱请求 ID。 */
  requestId: string
  /** 当前条目是否成功。 */
  success: boolean
  /** 成功后的目标状态。 */
  targetStatusEnum?: string
  /** 处理结果说明。 */
  message: string
}

/** 批量校对或退回响应。 */
export interface NurseStationOrderAuditBatchResultVO {
  /** 是否全部处理成功。 */
  success: boolean
  /** 成功处理条数。 */
  processedCount: number
  /** 逐条处理结果。 */
  results: NurseStationOrderAuditWriteResultVO[]
}
