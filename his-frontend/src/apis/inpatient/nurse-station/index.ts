import type { PageDTO } from '../../type'
import {
  mockGetNurseStationPatientStatistics,
  mockLoadNurseStationWardTree,
  mockPageNurseStationPatientQueue
} from './mock'
import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO,
  NurseStationPatientQueueQuery,
  NurseStationPatientStatisticsVO,
  NurseStationWardTreeVO
} from './type'
import {
  mockClearDischargedBed,
  mockExecuteInpatientDischarge,
  mockExecuteNurseStationBedOperation,
  mockQueryDischargePendingExecutions,
  mockQueryDischargePendingMedications,
  mockQueryNurseStationBedBoard,
  mockRecallInpatientDischarge
} from './atd-mock'
import type {
  NurseStationBedBoardQuery,
  NurseStationEncounterTransitionDTO,
  NurseStationExecuteBedOperationDTO
} from './atd-type'
import {
  mockQueryNurseStationOrderAudits,
  mockRejectNurseStationOrders,
  mockVerifyNurseStationOrders
} from './order-audit-mock'
import type {
  NurseStationOrderAuditQueryDTO,
  NurseStationRejectOrdersDTO,
  NurseStationVerifyOrdersDTO
} from './order-audit-type'
import {
  mockCancelNurseStationExecutions,
  mockExecuteNurseStationOrders,
  mockQueryNurseStationOrderExecutions,
  mockSkipNurseStationOrders
} from './order-execute-mock'
import type {
  NurseStationCancelExecutionsDTO,
  NurseStationExecuteOrdersDTO,
  NurseStationOrderExecuteQueryDTO,
  NurseStationSkipOrdersDTO
} from './order-execute-type'
import {
  mockDeleteNurseStationNursingRecord,
  mockQueryNurseStationNursingRecords,
  mockSaveNurseStationNursingRecords
} from './nursing-record-mock'
import type {
  NurseStationDeleteNursingRecordDTO,
  NurseStationNursingRecordQueryDTO,
  NurseStationSaveNursingRecordsDTO
} from './nursing-record-type'
import {
  mockQueryNurseStationTemperatureSheet,
  mockSaveNurseStationTemperatureSheet
} from './temperature-sheet-mock'
import type {
  NurseStationSaveTemperatureSheetDTO,
  NurseStationTemperatureSheetQueryDTO
} from './temperature-sheet-type'
import {
  mockCancelNurseStationBilling,
  mockConfirmNurseStationBilling,
  mockDeleteNurseStationAutoBillingRule,
  mockQueryNurseStationAutoBillingRules,
  mockQueryNurseStationBillingCatalog,
  mockQueryNurseStationPricedOrders,
  mockQueryNurseStationSupplementOrders,
  mockSaveNurseStationAutoBillingRule,
  mockSetNurseStationAutoBillingRuleStatus
} from './inpatient-billing-mock'
import type {
  NurseStationAutoBillingQueryDTO,
  NurseStationBillingCatalogQueryDTO,
  NurseStationCancelBillingDTO,
  NurseStationConfirmBillingDTO,
  NurseStationDeleteAutoBillingRuleDTO,
  NurseStationPricedOrderQueryDTO,
  NurseStationSaveAutoBillingRuleDTO,
  NurseStationSetAutoBillingRuleStatusDTO,
  NurseStationSupplementOrderQueryDTO
} from './inpatient-billing-type'
import {
  mockLoadNurseStationCostDetailInit,
  mockQueryNurseStationCostDetail
} from './cost-detail-mock'
import type { NurseStationCostDetailQueryDTO } from './cost-detail-type'
import {
  mockCancelNurseStationDispenseSummaries,
  mockCreateNurseStationDispenseSummary,
  mockGetNurseStationDispenseSummaryDetail,
  mockQueryNurseStationDispensePending,
  mockQueryNurseStationDispenseSummaries,
  mockSubmitNurseStationDispenseSummaries
} from './dispense-summary-mock'
import type {
  NurseStationCancelDispenseSummariesDTO,
  NurseStationCreateDispenseSummaryDTO,
  NurseStationDispensePendingQueryDTO,
  NurseStationDispenseSummaryQueryDTO,
  NurseStationSubmitDispenseSummariesDTO
} from './dispense-summary-type'

/**
 * 加载当前护士站管理范围内的病区病房树。
 * 后端完成后只替换本文件实现，页面和 adapter 无需修改。
 */
export function loadNurseStationWardTree(): Promise<NurseStationWardTreeVO> {
  return mockLoadNurseStationWardTree()
}

/** 获取患者统计信息。 */
export function getNurseStationPatientStatistics(
  query: NurseStationPatientFilterQuery
): Promise<NurseStationPatientStatisticsVO> {
  return mockGetNurseStationPatientStatistics(query)
}

/** 分页查询患者队列。 */
export function pageNurseStationPatientQueue(
  query: NurseStationPatientQueueQuery
): Promise<PageDTO<NurseStationPatientQueueItemVO>> {
  return mockPageNurseStationPatientQueue(query)
}

/** 查询护士站完整床位看板。 */
export function queryNurseStationBedBoard(query: NurseStationBedBoardQuery) {
  return mockQueryNurseStationBedBoard(query)
}

/** 执行首次分床、换空床或床位互换。 */
export function executeNurseStationBedOperation(data: NurseStationExecuteBedOperationDTO) {
  return mockExecuteNurseStationBedOperation(data)
}

/** 查询出院前待执行医嘱。 */
export function queryDischargePendingExecutions(encounterId: string) {
  return mockQueryDischargePendingExecutions(encounterId)
}

/** 查询出院前待发或待退药。 */
export function queryDischargePendingMedications(encounterId: string) {
  return mockQueryDischargePendingMedications(encounterId)
}

/** 办理出院。 */
export function executeInpatientDischarge(data: NurseStationEncounterTransitionDTO) {
  return mockExecuteInpatientDischarge(data)
}

/** 清床离院。 */
export function clearDischargedBed(data: NurseStationEncounterTransitionDTO) {
  return mockClearDischargedBed(data)
}

/** 出院召回。 */
export function recallInpatientDischarge(data: NurseStationEncounterTransitionDTO) {
  return mockRecallInpatientDischarge(data)
}

/** 查询所选患者的医嘱校对列表。 */
export function queryNurseStationOrderAudits(data: NurseStationOrderAuditQueryDTO) {
  return mockQueryNurseStationOrderAudits(data)
}

/** 批量校对通过，当前阶段仅写入共享 Mock 状态。 */
export function verifyNurseStationOrders(data: NurseStationVerifyOrdersDTO) {
  return mockVerifyNurseStationOrders(data)
}

/** 批量退回医嘱，当前阶段仅写入共享 Mock 状态。 */
export function rejectNurseStationOrders(data: NurseStationRejectOrdersDTO) {
  return mockRejectNurseStationOrders(data)
}

/**
 * 查询所选患者的医嘱执行计划。
 * 当前函数只调用内存 Mock；真实后端完成后在这里替换请求实现，页面无需改动。
 */
export function queryNurseStationOrderExecutions(data: NurseStationOrderExecuteQueryDTO) {
  return mockQueryNurseStationOrderExecutions(data)
}

/** 批量执行计划点，并模拟生成执行记录与同步患者待执行数量。 */
export function executeNurseStationOrders(data: NurseStationExecuteOrdersDTO) {
  return mockExecuteNurseStationOrders(data)
}

/** 批量把待执行计划点登记为不执行。 */
export function skipNurseStationOrders(data: NurseStationSkipOrdersDTO) {
  return mockSkipNurseStationOrders(data)
}

/** 批量取消已经完成的执行记录。 */
export function cancelNurseStationExecutions(data: NurseStationCancelExecutionsDTO) {
  return mockCancelNurseStationExecutions(data)
}

/**
 * 查询单个患者的护理记录纸质单。
 * 当前由内存 Mock 提供；后端完成后只替换本函数实现，页面结构和字段映射保持不变。
 */
export function queryNurseStationNursingRecords(data: NurseStationNursingRecordQueryDTO) {
  return mockQueryNurseStationNursingRecords(data)
}

/** 批量新增或更新护理记录。 */
export function saveNurseStationNursingRecords(data: NurseStationSaveNursingRecordsDTO) {
  return mockSaveNurseStationNursingRecords(data)
}

/** 删除一条已经保存的额外护理记录。 */
export function deleteNurseStationNursingRecord(data: NurseStationDeleteNursingRecordDTO) {
  return mockDeleteNurseStationNursingRecord(data)
}

/**
 * 查询单个患者的体温单窗口。
 * 当前阶段使用内存 Mock；后端接口稳定后只需要替换这里，页面不必感知数据来源变化。
 */
export function queryNurseStationTemperatureSheet(data: NurseStationTemperatureSheetQueryDTO) {
  return mockQueryNurseStationTemperatureSheet(data)
}

/** 批量保存同一天六个固定时次的生命体征和日汇总数据。 */
export function saveNurseStationTemperatureSheet(data: NurseStationSaveTemperatureSheetDTO) {
  return mockSaveNurseStationTemperatureSheet(data)
}

/** 查询住院记账的诊疗项目或耗材目录。 */
export function queryNurseStationBillingCatalog(data: NurseStationBillingCatalogQueryDTO) {
  return mockQueryNurseStationBillingCatalog(data)
}

/** 查询可补费父医嘱及其已有费用子项。 */
export function queryNurseStationSupplementOrders(data: NurseStationSupplementOrderQueryDTO) {
  return mockQueryNurseStationSupplementOrders(data)
}

/** 查询直接划价和医嘱补费产生的统一明细。 */
export function queryNurseStationPricedOrders(data: NurseStationPricedOrderQueryDTO) {
  return mockQueryNurseStationPricedOrders(data)
}

/** 批量确认直接划价或医嘱补费。 */
export function confirmNurseStationBilling(data: NurseStationConfirmBillingDTO) {
  return mockConfirmNurseStationBilling(data)
}

/** 批量取消尚未收费或发放的划价项目。 */
export function cancelNurseStationBilling(data: NurseStationCancelBillingDTO) {
  return mockCancelNurseStationBilling(data)
}

/** 查询护理级别只读项和患者自定义自动计费规则。 */
export function queryNurseStationAutoBillingRules(data: NurseStationAutoBillingQueryDTO) {
  return mockQueryNurseStationAutoBillingRules(data)
}

/** 新增或修改自动计费规则。 */
export function saveNurseStationAutoBillingRule(data: NurseStationSaveAutoBillingRuleDTO) {
  return mockSaveNurseStationAutoBillingRule(data)
}

/** 删除自动计费规则。 */
export function deleteNurseStationAutoBillingRule(data: NurseStationDeleteAutoBillingRuleDTO) {
  return mockDeleteNurseStationAutoBillingRule(data)
}

/** 将自动计费规则设置为明确的启用或停用状态。 */
export function setNurseStationAutoBillingRuleStatus(
  data: NurseStationSetAutoBillingRuleStatusDTO
) {
  return mockSetNurseStationAutoBillingRuleStatus(data)
}

/** 加载费用明细页面默认日期、展示模式和费用大类顺序。 */
export function loadNurseStationCostDetailInit() {
  return mockLoadNurseStationCostDetailInit()
}

/** 按单患者和日期范围查询费用明细、汇总及完整合计。 */
export function queryNurseStationCostDetail(data: NurseStationCostDetailQueryDTO) {
  return mockQueryNurseStationCostDetail(data)
}

/**
 * 查询选中患者的待汇总发药时点。
 * 当前只调用内存 Mock；后端完成后仅替换本函数内部实现，页面不需要改字段映射。
 */
export function queryNurseStationDispensePending(data: NurseStationDispensePendingQueryDTO) {
  return mockQueryNurseStationDispensePending(data)
}

/** 按药房拆分并创建汇总领药单。 */
export function createNurseStationDispenseSummary(data: NurseStationCreateDispenseSummaryDTO) {
  return mockCreateNurseStationDispenseSummary(data)
}

/** 分页查询当前护士站已经生成的汇总单。 */
export function queryNurseStationDispenseSummaries(data: NurseStationDispenseSummaryQueryDTO) {
  return mockQueryNurseStationDispenseSummaries(data)
}

/** 查询一张汇总单的单头和药品合并明细。 */
export function getNurseStationDispenseSummaryDetail(summaryNo: string) {
  return mockGetNurseStationDispenseSummaryDetail(summaryNo)
}

/** 批量发送待发送汇总单。 */
export function submitNurseStationDispenseSummaries(
  data: NurseStationSubmitDispenseSummariesDTO
) {
  return mockSubmitNurseStationDispenseSummaries(data)
}

/** 批量撤销待发送汇总单，并恢复被占用的发药时点。 */
export function cancelNurseStationDispenseSummaries(
  data: NurseStationCancelDispenseSummariesDTO
) {
  return mockCancelNurseStationDispenseSummaries(data)
}

export type * from './type'
export type * from './atd-type'
export type * from './order-audit-type'
export type * from './order-execute-type'
export type * from './nursing-record-type'
export type * from './temperature-sheet-type'
export type * from './inpatient-billing-type'
export type * from './cost-detail-type'
export type * from './dispense-summary-type'
