/** 医嘱执行页面的四种计划点状态。 */
export type NurseStationOrderExecuteStatus = 'CANCELLED' | 'EXECUTED' | 'PENDING' | 'SKIPPED'

/** 医嘱长期、临时筛选。 */
export type NurseStationOrderExecuteTherapyFilter = 'ALL' | 'LONG_TERM' | 'TEMPORARY'

/** 药品分类筛选。诊疗项目统一放在西药页签中展示。 */
export type NurseStationOrderExecuteMedicineCategory = 'TCM' | 'WEST'

/** 能够产生执行计划的两类医嘱请求。 */
export type NurseStationOrderExecuteOrderType = 'MED_REQUEST' | 'SERVICE_REQUEST'

/** 查询多位患者医嘱执行计划的条件。 */
export interface NurseStationOrderExecuteQueryDTO {
  /** 左侧已勾选的住院就诊 ID，服务端应去重并限制为 1～50 个。 */
  encounterIds: string[]
  /** 当前状态页签。 */
  executeStatus: NurseStationOrderExecuteStatus
  /** 长期或临时筛选。 */
  therapyType: NurseStationOrderExecuteTherapyFilter
  /** 西药或中药筛选。 */
  medicineCategory: NurseStationOrderExecuteMedicineCategory
  /** 只查询此时间以前的计划点，格式为 yyyy-MM-dd HH:mm:ss。 */
  executionDeadline: string
}

/** 医嘱的单个计划执行时点。 */
export interface NurseStationOrderExecuteSlotVO {
  /** 医生医嘱生成的计划执行时间。 */
  planTime: string
  /** 已执行后生成的执行记录 ID；取消执行时必须回传该字段。 */
  procedureId?: string
  /** 当前计划点状态。 */
  status: NurseStationOrderExecuteStatus
  /** 实际执行、不执行或取消的记录时间。 */
  recordedTime?: string
}

/** 医嘱执行时可能一起计费的绑定项目。 */
export interface NurseStationOrderExecuteBindItemVO {
  /** 绑定关系 ID，用于服务端校验配置是否仍然有效。 */
  bindingId: string
  /** 绑定类型，例如材料或诊疗活动。 */
  bindType: string
  /** 收费项目 ID。 */
  chargeItemDefinitionId: string
  /** 页面显示名称。 */
  displayName: string
  /** 规格。 */
  specificationText?: string
  /** 单价。 */
  unitPrice?: number
  /** 本次执行数量；待执行状态下允许护士修改。 */
  quantity: number
  /** 数量单位。 */
  unitText?: string
  /** 多个绑定项目的显示顺序。 */
  priority?: number
}

/** 医嘱执行工作台中的一行医嘱。 */
export interface NurseStationOrderExecuteRowVO {
  /** 表格稳定行键，不承担业务含义。 */
  listRowKey: string
  /** 医嘱请求 ID。 */
  orderId: string
  /** 药品申请或诊疗申请。 */
  orderType: NurseStationOrderExecuteOrderType
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 医嘱当前状态编码。 */
  statusEnum: string
  /** 医嘱当前状态名称。 */
  statusDisplay: string
  /** 长期、临时编码。 */
  therapyEnum: string
  /** 长期、临时名称。 */
  therapyDisplay: string
  /** 医嘱名称。 */
  adviceDisplayName: string
  /** 规格。 */
  specificationText?: string
  /** 剂量或每次用量。 */
  dosageDisplay?: string
  /** 用法。 */
  usageDisplay?: string
  /** 频次。 */
  frequencyDisplay?: string
  /** 医嘱开始时间。 */
  authoredTime?: string
  /** 医嘱停止时间。 */
  stopTime?: string
  /** 开立医生。 */
  requesterName?: string
  /** 执行科室。 */
  performerLocationDisplay?: string
  /** WEST 或 TCM，用于中西药页签筛选。 */
  medicineCategory: NurseStationOrderExecuteMedicineCategory
  /** 当前筛选状态下需要展示的计划时点。 */
  slots: NurseStationOrderExecuteSlotVO[]
  /** 执行时一起确认数量并计费的项目。 */
  bindItems: NurseStationOrderExecuteBindItemVO[]
  /** 是否是中药饮片整方折叠行。 */
  tcmFlag?: boolean
  /** 中药处方号。 */
  prescriptionNo?: string
  /** 中药整方包含的申请 ID。 */
  lineRequestIds?: string[]
  /** 中药付数。 */
  chineseHerbsDoseQuantity?: number
}

/** 单位患者的医嘱执行分组。 */
export interface NurseStationOrderExecutePatientGroupVO {
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 当前组合筛选下的医嘱。 */
  orders: NurseStationOrderExecuteRowVO[]
}

/** 医嘱执行工作台查询结果。 */
export interface NurseStationOrderExecuteListVO {
  /** 分组顺序与请求中的患者顺序保持一致。 */
  groups: NurseStationOrderExecutePatientGroupVO[]
}

/** 执行或不执行时用于定位一个计划点的数据。 */
export interface NurseStationOrderExecuteWriteItemDTO {
  /** 医嘱请求 ID。 */
  orderId: string
  /** 医嘱请求类型。 */
  orderType: NurseStationOrderExecuteOrderType
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 计划执行时间，与 orderId 和 orderType 一起唯一定位计划点。 */
  planTime: string
  /** 执行时必填的住院账户 ID。 */
  accountId?: string
  /** 实际执行时间；登记不执行时不传。 */
  actualTime?: string
  /** 护士最终确认数量后的绑定收费项。 */
  bindItems?: NurseStationOrderExecuteBindItemVO[]
  /** 中药处方号。 */
  prescriptionNo?: string
  /** 中药整方的全部申请 ID。 */
  lineRequestIds?: string[]
}

/** 批量执行计划点请求。 */
export interface NurseStationExecuteOrdersDTO {
  /** 防止网络重试造成重复执行或重复计费的幂等键。 */
  idempotencyKey: string
  /** 待执行计划点，单批 1～50 个。 */
  items: NurseStationOrderExecuteWriteItemDTO[]
}

/** 批量登记不执行请求。 */
export interface NurseStationSkipOrdersDTO {
  /** 防止重复登记的幂等键。 */
  idempotencyKey: string
  /** 可选的统一不执行原因。 */
  reason?: string
  /** 待登记计划点，单批 1～50 个。 */
  items: NurseStationOrderExecuteWriteItemDTO[]
}

/** 取消一个已执行计划点所需的数据。 */
export interface NurseStationCancelExecutionItemDTO {
  /** 当前住院就诊 ID，用于权限与患者范围校验。 */
  encounterId: string
  /** 已执行时生成的执行记录 ID。 */
  procedureId: string
}

/** 批量取消执行请求。 */
export interface NurseStationCancelExecutionsDTO {
  /** 防止重复取消的幂等键。 */
  idempotencyKey: string
  /** 取消原因，不能为空。 */
  reason: string
  /** 待取消执行记录，单批 1～50 个。 */
  items: NurseStationCancelExecutionItemDTO[]
}

/** 单个计划点的写入结果。 */
export interface NurseStationOrderExecuteWriteResultVO {
  /** 计划点或执行记录 ID。 */
  itemKey: string
  /** 当前条目是否成功。 */
  success: boolean
  /** 处理结果说明。 */
  message: string
}

/** 执行、不执行、取消执行共用的批量结果。 */
export interface NurseStationOrderExecuteBatchResultVO {
  /** 是否全部成功。 */
  success: boolean
  /** 成功处理数量。 */
  processedCount: number
  /** 取消执行影响并重建的领药汇总单号。 */
  rebuiltSummaryNos?: string[]
  /** 每个计划点的处理结果。 */
  results: NurseStationOrderExecuteWriteResultVO[]
}
