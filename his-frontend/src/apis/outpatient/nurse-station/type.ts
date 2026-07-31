import type { PageQuery } from '../../type'

/** 门诊护士站患者状态。 */
export type NursePatientStatus = 'WAITING' | 'READY' | 'COMPLETED'

/** 门诊护士站患者队列行。 */
export interface NursePatientQueueItemVO {
  /** 本次就诊唯一标识。 */
  encounterId: string
  /** 患者档案唯一标识。 */
  patientId: string
  /** 患者姓名。 */
  patientName: string
  /** 性别名称。 */
  genderName: string
  /** 年龄展示文本。 */
  ageText: string
  /** 病历号。 */
  medicalRecordNo: string
  /** 就诊号。 */
  visitNo: string
  /** 当前排队号。 */
  queueNumber: number
  /** 挂号时间。 */
  registeredAt: string
  /** 接诊科室名称。 */
  departmentName: string
  /** 门诊类别。 */
  visitTypeName: string
  /** 当前业务状态。 */
  status: NursePatientStatus
  /** 当前业务状态名称。 */
  statusName: string
  /** 支付类型名称。 */
  paymentTypeName: string
  /** 待执行项目数量。 */
  pendingCount: number
  /** 优先级提示。 */
  priorityName?: string
  /** 风险或过敏提示。 */
  riskHint?: string
}

/** 门诊护士站患者队列查询条件。 */
export interface NursePatientQueueQuery extends PageQuery {
  /** 姓名、病历号或就诊号关键词。 */
  keyword?: string
  /** 查询开始日期，格式 YYYY-MM-DD。 */
  startDate?: string
  /** 查询结束日期，格式 YYYY-MM-DD。 */
  endDate?: string
}

/** 护士执行项目类型。 */
export type NurseExecutionItemType = 'SERVICE' | 'MEDICATION'

/** 护士执行项目状态。 */
export type NurseExecutionStatus = 'PENDING' | 'COMPLETED' | 'CANCELLED'

/** 与原型护士站一致的打印单据类型。 */
export type NursePrintDocumentType =
  | 'BOTTLE_LABEL'
  | 'TREATMENT_SHEET'
  | 'SPECIMEN_BARCODE'
  | 'PRESCRIPTION'
  | 'SKIN_TEST_LABEL'
  | 'NEBULIZATION_LABEL'
  | 'PROCEDURE_APPLICATION'

/** 当前就诊的执行项目。 */
export interface NurseExecutionItemVO {
  /** 执行项目唯一标识。 */
  itemId: string
  /** 来源医嘱或申请唯一标识。 */
  requestId: string
  /** 项目类型。 */
  requestType: NurseExecutionItemType
  /** 项目类型名称。 */
  requestTypeName: string
  /** 医嘱或处置内容。 */
  orderContent: string
  /** 规格或剂量说明。 */
  specification?: string
  /** 执行状态。 */
  status: NurseExecutionStatus
  /** 执行状态名称。 */
  statusName: string
  /** 计划执行次数。 */
  plannedCount: number
  /** 已执行次数。 */
  executedCount: number
  /** 执行人姓名。 */
  executorName?: string
  /** 最近执行时间。 */
  executedAt?: string
  /** 数据版本，用于防止并发覆盖。 */
  version: number
  /** 当前项目可生成的打印单据类型。 */
  printTypes: NursePrintDocumentType[]
}

/** 批量执行中的单个项目参数。 */
export interface NurseExecutionCommandItemDTO {
  /** 执行项目唯一标识。 */
  itemId: string
  /** 页面读取项目时的版本。 */
  version: number
}

/** 批量执行请求。 */
export interface PerformNurseExecutionsDTO {
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 本次选择的执行项目。 */
  items: NurseExecutionCommandItemDTO[]
  /** 防止重复提交的幂等键。 */
  idempotencyKey: string
}

/** 取消执行请求。 */
export interface CancelNurseExecutionDTO {
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 需要取消的执行项目。 */
  item: NurseExecutionCommandItemDTO
  /** 取消原因。 */
  reason: string
}

/** 执行失败原因。 */
export type NurseExecutionFailureCode = 'ITEM_NOT_FOUND' | 'INVALID_STATUS' | 'VERSION_CONFLICT'

/** 批量操作失败项目。 */
export interface NurseExecutionFailureVO {
  /** 执行项目唯一标识。 */
  itemId: string
  /** 失败原因代码。 */
  code: NurseExecutionFailureCode
  /** 失败原因说明。 */
  message: string
}

/** 批量执行或取消结果。 */
export interface NurseExecutionCommandResultVO {
  /** 操作成功后的最新项目。 */
  successItems: NurseExecutionItemVO[]
  /** 操作失败项目。 */
  failedItems: NurseExecutionFailureVO[]
}

/** 耗材发放状态。 */
export type NurseConsumableStatus = 'PENDING' | 'DISPENSED'

/** 当前就诊的耗材项目。 */
export interface NurseConsumableItemVO {
  /** 发放项目唯一标识。 */
  dispenseItemId: string
  /** 耗材目录唯一标识。 */
  materialId: string
  /** 耗材名称。 */
  materialName: string
  /** 型号规格。 */
  specification: string
  /** 数量。 */
  quantity: number
  /** 单位名称。 */
  unitName: string
  /** 耗材室名称。 */
  roomName: string
  /** 发放状态。 */
  status: NurseConsumableStatus
  /** 发放状态名称。 */
  statusName: string
}

/** 打印请求。 */
export interface NursePrintRequestDTO {
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 需要打印的执行项目 ID。 */
  documentIds: string[]
  /** 打印单据类型。 */
  documentType: NursePrintDocumentType
  /** 是否补打。 */
  reprint: boolean
}

/** 打印结果。 */
export interface NursePrintResultVO {
  /** 打印是否成功。 */
  success: boolean
  /** 打印机名称。 */
  printer: string
  /** 打印结果说明。 */
  message: string
  /** 实际打印单据类型。 */
  documentType: NursePrintDocumentType
  /** 生成的打印任务数量。 */
  count: number
}

/** 患者历史执行记录查询条件。 */
export interface NurseExecutionHistoryQuery extends PageQuery {
  /** 查询开始日期，格式 YYYY-MM-DD。 */
  startDate?: string
  /** 查询结束日期，格式 YYYY-MM-DD。 */
  endDate?: string
  /** 执行状态过滤。 */
  status?: NurseExecutionStatus
}

/** 患者历史执行记录行（跨就诊）。 */
export interface NurseExecutionRecordVO extends NurseExecutionItemVO {
  /** 所属就诊唯一标识。 */
  encounterId: string
  /** 就诊号。 */
  visitNo: string
}
