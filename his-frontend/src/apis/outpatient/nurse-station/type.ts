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
  /** 队列业务号（后端 bus_no）。 */
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
  /** 优先级提示。 */
  priorityName?: string
}

/** 门诊护士站患者队列查询条件。 */
export interface NursePatientQueueQuery extends PageQuery {
  /** 预约日期，格式 YYYY-MM-DD，对应后端 booking_date。 */
  bookingDate?: string
}

/** 护士执行项目状态。 */
export type NurseExecutionStatus = 'PENDING' | 'COMPLETED' | 'CANCELLED'

/** 当前就诊的执行项目。 */
export interface NurseExecutionItemVO {
  /** 执行项目唯一标识。 */
  itemId: string
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 项目分类编码。 */
  categoryCode?: string
  /** 项目分类名称。 */
  categoryName?: string
  /** 项目名称。 */
  itemName: string
  /** 项目数量。 */
  quantity?: number
  /** 项目单位。 */
  unitName?: string
  /** 执行状态。 */
  status: NurseExecutionStatus
  /** 执行状态名称。 */
  statusName: string
  /** 执行次数。 */
  executionCount: number
  /** 执行人姓名。 */
  executorName?: string
  /** 最近执行时间。 */
  executedAt?: string
  /** 执行备注。 */
  remark?: string
}

/** 批量执行中的单个项目参数。 */
export interface NurseExecutionCommandItemDTO {
  /** 执行项目唯一标识。 */
  itemId: string
}

/** 批量执行请求。 */
export interface PerformNurseExecutionsDTO {
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 本次选择的执行项目。 */
  items: NurseExecutionCommandItemDTO[]
  /** 防止重复提交的幂等键。 */
  idempotencyKey: string
  /** 当前执行者 ID；真实后端必填。 */
  executorId: string
  /** 当前执行者姓名。 */
  executorName?: string
  /** 指定执行时间；为空时由后端使用当前时间。 */
  executeTime?: string
  /** 执行类型。 */
  executeType?: string
  /** 执行备注。 */
  remark?: string
}

/** 取消执行请求。 */
export interface CancelNurseExecutionDTO {
  /** 当前就诊唯一标识。 */
  encounterId: string
  /** 需要取消的执行项目。 */
  item: NurseExecutionCommandItemDTO
  /** 取消原因。 */
  reason: string
  /** 取消原因编码。 */
  reasonCode?: string
  /** 当前操作人 ID。 */
  operatorId?: string
  /** 当前操作人姓名。 */
  operatorName?: string
  /** 防止重复取消的幂等键。 */
  idempotencyKey?: string
}

/** 后端批量执行/取消失败项目。 */
export interface NurseBatchFailedItemVO {
  /** 后端记录唯一标识。 */
  recordId: string
  /** 后端错误编码。 */
  errorCode: string
  /** 后端失败原因。 */
  reason: string
}

/** 后端批量执行/取消结果。 */
export interface NurseBatchResultVO {
  /** 请求项目总数。 */
  totalCount: number
  /** 成功项目数。 */
  successCount: number
  /** 失败项目数。 */
  failedCount: number
  /** 成功记录 ID。 */
  successIds: string[]
  /** 失败项目明细。 */
  failedItems: NurseBatchFailedItemVO[]
}

/** 当前就诊的耗材项目。 */
export interface NurseConsumableItemVO {
  /** 器械定义 ID。 */
  deviceDefId: string
  /** 数量。 */
  quantity?: number
  /** 单位编码。 */
  unitCode?: string
  /** 用法说明。 */
  usageInstruction?: string
  /** 批号。 */
  lotNumber?: string
  /** 后端状态枚举。 */
  statusEnum?: string
  /** 发放时间。 */
  dispenseTime?: string
  /** 后端支持信息，用于展示真实业务描述。 */
  supportInfo?: string
}

/** 患者历史执行记录查询条件。 */
export interface NurseExecutionHistoryQuery extends PageQuery {
  /** 查询开始日期，格式 YYYY-MM-DD。 */
  startDate?: string
  /** 查询结束日期，格式 YYYY-MM-DD。 */
  endDate?: string
}

/** 患者历史执行记录行（跨就诊）。 */
export interface NurseExecutionRecordVO extends NurseExecutionItemVO {
  /** 就诊号。 */
  visitNo: string
}
