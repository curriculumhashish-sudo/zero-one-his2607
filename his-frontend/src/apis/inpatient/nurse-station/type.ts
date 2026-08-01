import type { PageQuery } from '../../type'

/** 护士站患者任务筛选编码。 */
export type NurseStationTaskFilterCode = 'pending-audit' | 'pending-execute' | 'processed'

/** 护士站患者列表范围编码。 */
export type NurseStationStatusFilterCode = 'SETTLED_DISCHARGED'

/** 住院就诊状态编码。 */
export type InpatientEncounterStatusCode =
  | 'REGISTERED'
  | 'IN_PROGRESS'
  | 'ON_HOLD'
  | 'PENDING'
  | 'COMPLETED'
  | 'DISCHARGED'

/** 病房节点。 */
export interface NurseStationHouseNodeVO {
  /** 病房唯一标识。 */
  houseId: string
  /** 病房名称。 */
  houseName: string
  /** 展示顺序。 */
  displayOrder?: number
}

/** 病区节点。 */
export interface NurseStationWardNodeVO {
  /** 病区唯一标识。 */
  wardId: string
  /** 病区名称。 */
  wardName: string
  /** 展示顺序。 */
  displayOrder?: number
  /** 病区下的病房。 */
  houseNodes: NurseStationHouseNodeVO[]
}

/** 当前护士站管理范围内的病区病房树。 */
export interface NurseStationWardTreeVO {
  wardNodes: NurseStationWardNodeVO[]
}

/** 患者统计和患者队列共用的筛选条件。 */
export interface NurseStationPatientFilterQuery {
  /** 姓名、病区、病房、床号或检索码。 */
  keyword?: string
  /** 兼容后端旧契约的单病区 ID。 */
  wardId?: string
  /** 兼容后端旧契约的单病房 ID。 */
  houseId?: string
  /** 多个病区 ID，以英文逗号分隔。 */
  wardIds?: string
  /** 多个病房 ID，以英文逗号分隔。 */
  houseIds?: string
  /** 护士站任务执行日期。 */
  nurseStationExecutionDate?: string
  /** 患者任务筛选。 */
  nurseStationTaskFilter?: NurseStationTaskFilterCode
  /** 列表范围筛选；空值表示在院患者。 */
  nurseStationStatusFilter?: NurseStationStatusFilterCode
}

/** 患者队列分页查询。 */
export interface NurseStationPatientQueueQuery extends PageQuery, NurseStationPatientFilterQuery {
  /** 时间范围开始。 */
  startDate?: string
  /** 时间范围结束。 */
  endDate?: string
  /** 服务端白名单排序字段。 */
  sortBy?: string
  /** 排序方向。 */
  sortDirection?: 'asc' | 'desc'
  /** 服务端查询字段白名单。 */
  where?: string
}

/** 患者队列返回项。 */
export interface NurseStationPatientQueueItemVO {
  /** 住院账户 ID。 */
  accountId?: string
  /** 账户余额。 */
  accountBalance?: number
  /** 余额显示模式。 */
  accountBalanceDisplayMode?: string
  /** 预交金净值。 */
  prepayNetBalance?: number
  /** 余额辅助说明。 */
  accountBalanceSubText?: string
  /** 余额汇总说明。 */
  accountBalanceSummaryText?: string
  /** 可用余额。 */
  availableBalance?: number
  /** 入院时间。 */
  admissionTime?: string
  /** 年龄显示文本。 */
  ageText?: string
  /** 欠费金额。 */
  arrearsAmount?: number
  /** 主治医生姓名。 */
  attendingDoctorName?: string
  /** 床位 ID。 */
  bedId?: string
  /** 床位名称。 */
  bedName?: string
  /** 诊断摘要。 */
  diagnosisSummary?: string
  /** 诊断展示列表。 */
  diagnosisList?: string[]
  /** 住院号。 */
  encounterBusNo?: string
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 就诊结束时间。 */
  endTime?: string
  /** 性别编码。 */
  genderEnum?: string
  /** 性别名称。 */
  genderEnumText?: string
  /** 病房 ID。 */
  houseId?: string
  /** 病房名称。 */
  houseName?: string
  /** 是否低余额预警。 */
  lowBalanceWarning?: boolean
  /** 当前住院科室 ID。 */
  organizationId?: string
  /** 当前住院科室名称。 */
  organizationName?: string
  /** 已缴金额。 */
  paidAmount?: number
  /** 患者主档 ID。 */
  patientId: string
  /** 患者姓名。 */
  patientName: string
  /** 待校对医嘱条数。 */
  pendingAuditCount?: number
  /** 待执行医嘱条数。 */
  pendingExecuteCount?: number
  /** 待结算金额。 */
  pendingSettlementTotal?: number
  /** 拼音码。 */
  pinyinCode?: string
  /** 责任护士姓名。 */
  primaryNurseName?: string
  /** 优先级编码。 */
  priorityEnum?: string
  /** 已出院结算列表中是否允许召回。 */
  recallEnabled?: boolean
  /** 就诊开始时间。 */
  startTime?: string
  /** 住院就诊状态。 */
  statusEnum?: InpatientEncounterStatusCode
  /** 住院就诊状态名称。 */
  statusEnumText?: string
  /** 费用总额。 */
  totalAmount?: number
  /** 病区 ID。 */
  wardId?: string
  /** 病区名称。 */
  wardName?: string
  /** 五笔码。 */
  wubiCode?: string
}

/** 工作台顶部患者统计。 */
export interface NurseStationPatientStatisticsVO {
  /** 当前筛选范围内的患者人数。 */
  totalCount: number
  /** 当前筛选患者的待校对医嘱总数。 */
  pendingAuditCount: number
  /** 当前筛选患者的待执行医嘱总数。 */
  pendingExecuteCount: number
}
