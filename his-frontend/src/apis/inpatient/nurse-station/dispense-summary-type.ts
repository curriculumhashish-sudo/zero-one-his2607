import type { PageQuery } from '../../type'

/** 汇总领药页面的中西药分类。 */
export type NurseStationDispenseMedicineCategory = 'TCM' | 'WEST'

/** 医嘱长期、临时筛选。 */
export type NurseStationDispenseTherapyFilter = 'ALL' | 'LONG' | 'TEMP'

/**
 * 汇总单状态。
 *
 * 页面只依赖这一套状态，避免后端原文中 WAIT_REVIEW、PENDING_APPROVAL 等多套枚举
 * 同时出现后，前端无法准确判断按钮是否可用。
 */
export type NurseStationDispenseSummaryStatus =
  | 'CANCELLED'
  | 'DISPENSED'
  | 'PENDING_SUBMIT'
  | 'SUBMITTED'

/** 多患者待汇总明细查询条件。 */
export interface NurseStationDispensePendingQueryDTO extends PageQuery {
  /** 当前左侧勾选的住院就诊 ID。 */
  encounterIds: string[]
  /** 西药或中药。 */
  medicineCategory: NurseStationDispenseMedicineCategory
  /** 全部、长期或临时医嘱。 */
  therapyFilter: NurseStationDispenseTherapyFilter
  /** 查询截止时间，包含该时刻。 */
  queryDeadline: string
  /** 可选的药品名称、医嘱号关键字。 */
  keyword?: string
}

/** 一次可单独勾选的计划发药时点。 */
export interface NurseStationDispenseTimePointVO {
  /** 发药计划 ID，是创建汇总单时使用的稳定主键。 */
  dispenseId: string
  /** 对应的医嘱执行记录 ID。 */
  procedureId: string
  /** 计划发药时间。 */
  plannedDispenseTime: string
  /** 此时点需要领取的数量。 */
  quantity: number
  /** 数量单位编码。 */
  unitCode: string
  /** 数量单位显示名称。 */
  unitCodeText: string
  /** 拆零单位的计费单价。 */
  chargeUnitPrice: number
}

/** 待汇总页面的一行药品医嘱。 */
export interface NurseStationDispenseOrderRowVO {
  /** 药品医嘱 ID。 */
  medicationRequestId: string
  /** 药品定义 ID。 */
  medicationId: string
  /** 药品名称。 */
  medicationName: string
  /** 药品规格。 */
  specificationText: string
  /** 长期或临时编码。 */
  therapyType: 'LONG' | 'TEMP'
  /** 长期或临时显示名称。 */
  therapyTypeText: string
  /** 单次剂量说明。 */
  dosageInstruction: string
  /** 给药途径。 */
  methodText: string
  /** 执行频次。 */
  frequencyText: string
  /** 目标药房 ID，Mock 按此字段拆分汇总单。 */
  pharmacyId: string
  /** 目标药房名称。 */
  pharmacyName: string
  /** 当前过滤条件下仍可汇总的发药时点。 */
  timePoints: NurseStationDispenseTimePointVO[]
}

/** 待汇总明细按患者分组，方便页面直接渲染患者折叠卡。 */
export interface NurseStationDispensePatientGroupVO {
  encounterId: string
  patientId: string
  patientName: string
  bedDisplay: string
  orderRows: NurseStationDispenseOrderRowVO[]
}

/** 待汇总查询结果，total 明确表示可勾选发药时点总数。 */
export interface NurseStationDispensePendingResultVO {
  pageIndex: number
  pageSize: number
  /** 当前条件下全部可勾选发药时点数量。 */
  total: number
  patientGroups: NurseStationDispensePatientGroupVO[]
}

/** 创建汇总单时提交的一条发药时点。 */
export interface NurseStationCreateDispenseItemDTO {
  dispenseId: string
  encounterId: string
}

/** 创建汇总单请求。 */
export interface NurseStationCreateDispenseSummaryDTO {
  /** 防止按钮重复点击产生两批汇总单的幂等键。 */
  idempotencyKey: string
  /** 单批最多 500 个发药时点。 */
  dispenseItems: NurseStationCreateDispenseItemDTO[]
}

/** 批量写操作中每个目标的处理结果。 */
export interface NurseStationDispenseWriteItemResultVO {
  /** 发药时点 ID 或汇总单号。 */
  targetId: string
  success: boolean
  message: string
}

/** 创建汇总单响应。 */
export interface NurseStationCreateDispenseSummaryResultVO {
  summaryNos: string[]
  processedCount: number
  results: NurseStationDispenseWriteItemResultVO[]
  message: string
}

/** 已生成汇总单的分页查询条件。 */
export interface NurseStationDispenseSummaryQueryDTO extends PageQuery {
  pharmacyId?: string
  status?: NurseStationDispenseSummaryStatus
  dateFrom?: string
  dateTo?: string
  keyword?: string
}

/** 汇总单头，也是“汇总单”页签的表格行。 */
export interface NurseStationDispenseSummaryHeaderVO {
  summaryNo: string
  pharmacyId: string
  pharmacyName: string
  applicantId: string
  applicantName: string
  applyTime: string
  receiverId?: string
  receiverName?: string
  dispenseTime?: string
  status: NurseStationDispenseSummaryStatus
  statusText: string
  itemCount: number
  /** 乐观锁版本；发送和撤销时必须回传当前值。 */
  version: number
}

/** 汇总单中按药品合并的一行明细。 */
export interface NurseStationDispenseSummaryLineVO {
  lineId: string
  medicationId: string
  medicationName: string
  specificationText: string
  itemQuantity: number
  unitCode: string
  unitCodeText: string
  /** 例如“1盒1片”，优先用于护士核对整包装和拆零数量。 */
  displayQuantityText: string
  unitPrice: number
  totalPrice: number
}

/** 汇总单详情包含单头和药品行，打开弹窗时一次返回。 */
export interface NurseStationDispenseSummaryDetailVO {
  header: NurseStationDispenseSummaryHeaderVO
  lines: NurseStationDispenseSummaryLineVO[]
}

/** 发送或撤销时的单据版本。 */
export interface NurseStationDispenseSummaryVersionDTO {
  summaryNo: string
  version: number
}

/** 发送汇总单请求。 */
export interface NurseStationSubmitDispenseSummariesDTO {
  idempotencyKey: string
  summaries: NurseStationDispenseSummaryVersionDTO[]
}

/** 撤销汇总单请求。 */
export interface NurseStationCancelDispenseSummariesDTO {
  idempotencyKey: string
  summaries: NurseStationDispenseSummaryVersionDTO[]
}

/** 发送或撤销的统一响应。 */
export interface NurseStationDispenseSummaryWriteResultVO {
  processedCount: number
  results: NurseStationDispenseWriteItemResultVO[]
  message: string
}
