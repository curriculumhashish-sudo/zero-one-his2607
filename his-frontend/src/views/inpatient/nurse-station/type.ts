/** 住院护士站当前展示的业务页签。 */
export type NurseStationOperationTab =
  | '入出转管理'
  | '医嘱校对'
  | '医嘱执行'
  | '汇总领药'
  | '护理记录'
  | '体温单'
  | '住院记账'
  | '费用明细'

/** 页签对左侧患者列表的选择要求。 */
export type NurseStationSelectionPolicy = 'single' | 'multiple'

/** 左侧患者队列的任务状态筛选。 */
export type NurseStationTaskFilter = 'all' | 'pending-audit' | 'pending-execute'

/**
 * 工作台使用的患者摘要模型。
 *
 * 该模型只描述页面展示需要的字段。正式对接时由 adapter 将后端患者队列 DTO
 * 转换为此模型，避免页面直接依赖接口字段。
 */
export interface NurseStationPatientSummary {
  /**
   * 当前住院账户 ID。
   * 执行医嘱可能同时产生收费，因此写操作必须携带账户 ID，不能只使用患者 ID。
   */
  accountId?: string
  /** 当前住院就诊 ID，也是工作台选择患者的唯一键。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 患者姓名。 */
  patientName: string
  /** 住院号。 */
  encounterBusNo?: string
  /** 入院时间，分床时用于生成后端要求的预期入院日期。 */
  admissionTime?: string
  /** 性别显示文本。 */
  genderText?: string
  /** 年龄显示文本。 */
  ageText?: string
  /** 当前在院状态文本。 */
  statusText?: string
  /** 当前在院状态编码。 */
  statusCode?: string
  /** 主要诊断摘要。 */
  diagnosisSummary?: string
  /** 病区名称。 */
  wardName?: string
  /** 病区 ID。 */
  wardId?: string
  /** 病房名称。 */
  houseName?: string
  /** 病房 ID。 */
  houseId?: string
  /** 床位显示名称。 */
  bedName?: string
  /** 床位 ID。 */
  bedId?: string
  /** 待校对医嘱数。 */
  pendingAuditCount: number
  /** 待执行医嘱数。 */
  pendingExecuteCount: number
  /** 可用余额。 */
  availableBalance?: number
  /** 是否触发余额预警。 */
  lowBalanceWarning?: boolean
}

/** 工作台顶部的患者聚合统计。 */
export interface NurseStationPatientStatistics {
  totalCount: number
  pendingAuditCount: number
  pendingExecuteCount: number
}

/** 业务页签静态元数据。 */
export interface NurseStationOperationConfig {
  id: NurseStationOperationTab
  description: string
  selectionPolicy: NurseStationSelectionPolicy
}
