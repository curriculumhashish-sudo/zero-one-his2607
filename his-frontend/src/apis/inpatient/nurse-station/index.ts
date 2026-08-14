import type { PageDTO } from '../../type'
import { mockGetNurseStationPatientStatistics, mockPageNurseStationPatientQueue } from './mock'
import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO,
  NurseStationPatientQueueQuery,
  NurseStationPatientStatisticsVO
} from './type'
import {
  mockAssignNurseStationBed,
  mockClearDischargedBed,
  mockExecuteInpatientDischarge,
  mockQueryNurseStationBedBoard
} from './atd-mock'
import {
  realAssignNurseStationBed,
  realClearDischargedBed,
  realExecuteInpatientDischarge,
  realQueryNurseStationBedBoard
} from './atd-real'
import type {
  NurseStationAssignBedDTO,
  NurseStationBedBoardQuery,
  NurseStationEncounterTransitionDTO
} from './atd-type'
import {
  mockQueryNurseStationOrderAudits,
  mockRejectNurseStationOrders,
  mockVerifyNurseStationOrders
} from './order-audit-mock'
import {
  realQueryNurseStationOrderAudits,
  realRejectNurseStationOrders,
  realVerifyNurseStationOrders
} from './order-audit-real'
import type {
  NurseStationOrderAuditQueryDTO,
  NurseStationRejectOrdersDTO,
  NurseStationVerifyOrdersDTO
} from './order-audit-type'
import {
  mockExecuteNurseStationOrders,
  mockQueryNurseStationOrderExecutions
} from './order-execute-mock'
import {
  realExecuteNurseStationOrders,
  realQueryNurseStationOrderExecutions
} from './order-execute-real'
import type {
  NurseStationExecuteOrdersDTO,
  NurseStationOrderExecuteQueryDTO
} from './order-execute-type'
import {
  mockDeleteNurseStationNursingRecord,
  mockQueryNurseStationNursingRecords,
  mockSaveNurseStationNursingRecords
} from './nursing-record-mock'
import {
  realDeleteNurseStationNursingRecord,
  realQueryNurseStationNursingRecords,
  realSaveNurseStationNursingRecords
} from './nursing-record-real'
import type {
  NurseStationDeleteNursingRecordDTO,
  NurseStationNursingRecordQueryDTO,
  NurseStationSaveNursingRecordsDTO
} from './nursing-record-type'
import {
  mockQueryNurseStationTemperatureSheet,
  mockSaveNurseStationTemperatureSheet
} from './temperature-sheet-mock'
import {
  realQueryNurseStationTemperatureSheet,
  realSaveNurseStationTemperatureSheet
} from './temperature-sheet-real'
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
  mockSaveNurseStationAutoBillingRule
} from './inpatient-billing-mock'
import {
  realCancelNurseStationBilling,
  realConfirmNurseStationBilling,
  realDeleteNurseStationAutoBillingRule,
  realQueryNurseStationAutoBillingRules,
  realQueryNurseStationBillingCatalog,
  realQueryNurseStationPricedOrders,
  realQueryNurseStationSupplementOrders,
  realSaveNurseStationAutoBillingRule
} from './inpatient-billing-real'
import type {
  NurseStationAutoBillingQueryDTO,
  NurseStationBillingCatalogQueryDTO,
  NurseStationCancelBillingDTO,
  NurseStationConfirmBillingDTO,
  NurseStationDeleteAutoBillingRuleDTO,
  NurseStationPricedOrderQueryDTO,
  NurseStationSaveAutoBillingRuleDTO,
  NurseStationSupplementOrderQueryDTO
} from './inpatient-billing-type'
import {
  mockQueryNurseStationCostDetail,
  mockQueryNurseStationCostSummary
} from './cost-detail-mock'
import {
  realQueryNurseStationCostDetail,
  realQueryNurseStationCostSummary
} from './cost-detail-real'
import type {
  NurseStationCostDetailQueryDTO,
  NurseStationCostSummaryQueryDTO
} from './cost-detail-type'
import {
  isNurseStationRealApiEnabled,
  realGetNurseStationPatientStatistics,
  realPageNurseStationPatientQueue
} from './patient-real'

import {
  mockCancelNurseStationDispenseSummaries,
  mockCreateNurseStationDispenseSummary,
  mockGetNurseStationDispenseSummaryDetail,
  mockQueryNurseStationDispensePending,
  mockQueryNurseStationDispenseSummaries,
  mockSubmitNurseStationDispenseSummaries
} from './dispense-summary-mock'
import {
  realCancelNurseStationDispenseSummaries,
  realCreateNurseStationDispenseSummary,
  realGetNurseStationDispenseSummaryDetail,
  realQueryNurseStationDispensePending,
  realQueryNurseStationDispenseSummaries,
  realSubmitNurseStationDispenseSummaries
} from './dispense-summary-real'
import type {
  NurseStationCancelDispenseSummariesDTO,
  NurseStationCreateDispenseSummaryDTO,
  NurseStationDispensePendingQueryDTO,
  NurseStationDispenseSummaryQueryDTO,
  NurseStationSubmitDispenseSummariesDTO
} from './dispense-summary-type'

/** 获取患者统计信息。 */
export function getNurseStationPatientStatistics(
  query: NurseStationPatientFilterQuery
): Promise<NurseStationPatientStatisticsVO> {
  return isNurseStationRealApiEnabled
    ? realGetNurseStationPatientStatistics(query)
    : mockGetNurseStationPatientStatistics(query)
}

/** 分页查询患者队列。 */
export function pageNurseStationPatientQueue(
  query: NurseStationPatientQueueQuery
): Promise<PageDTO<NurseStationPatientQueueItemVO>> {
  return isNurseStationRealApiEnabled
    ? realPageNurseStationPatientQueue(query)
    : mockPageNurseStationPatientQueue(query)
}

/** 查询护士站完整床位看板。 */
export function queryNurseStationBedBoard(query: NurseStationBedBoardQuery) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationBedBoard(query)
    : mockQueryNurseStationBedBoard(query)
}

/** 预约空闲床位。 */
export function assignNurseStationBed(data: NurseStationAssignBedDTO) {
  return isNurseStationRealApiEnabled
    ? realAssignNurseStationBed(data)
    : mockAssignNurseStationBed(data)
}

/** 办理出院。 */
export function executeInpatientDischarge(data: NurseStationEncounterTransitionDTO) {
  return isNurseStationRealApiEnabled
    ? realExecuteInpatientDischarge(data)
    : mockExecuteInpatientDischarge(data)
}

/** 清床离院。 */
export function clearDischargedBed(data: NurseStationEncounterTransitionDTO) {
  return isNurseStationRealApiEnabled ? realClearDischargedBed(data) : mockClearDischargedBed(data)
}

/** 查询所选患者的医嘱校对列表。 */
export function queryNurseStationOrderAudits(data: NurseStationOrderAuditQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationOrderAudits(data)
    : mockQueryNurseStationOrderAudits(data)
}

/** 批量校对通过。 */
export function verifyNurseStationOrders(data: NurseStationVerifyOrdersDTO) {
  return isNurseStationRealApiEnabled
    ? realVerifyNurseStationOrders(data)
    : mockVerifyNurseStationOrders(data)
}

/** 批量退回医嘱。 */
export function rejectNurseStationOrders(data: NurseStationRejectOrdersDTO) {
  return isNurseStationRealApiEnabled
    ? realRejectNurseStationOrders(data)
    : mockRejectNurseStationOrders(data)
}

/** Query verified service requests that can be started for the selected patients. */
export function queryNurseStationOrderExecutions(data: NurseStationOrderExecuteQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationOrderExecutions(data)
    : mockQueryNurseStationOrderExecutions(data)
}

/** Adapt the page batch selection to the backend single-order start endpoint. */
export function executeNurseStationOrders(data: NurseStationExecuteOrdersDTO) {
  return isNurseStationRealApiEnabled
    ? realExecuteNurseStationOrders(data)
    : mockExecuteNurseStationOrders(data)
}

/**
 * 查询单个患者的护理记录纸质单。
 * 根据环境在真实接口和内存 Mock 之间切换，页面结构和字段映射保持不变。
 */
export function queryNurseStationNursingRecords(data: NurseStationNursingRecordQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationNursingRecords(data)
    : mockQueryNurseStationNursingRecords(data)
}

/** 批量新增或更新护理记录。 */
export function saveNurseStationNursingRecords(data: NurseStationSaveNursingRecordsDTO) {
  return isNurseStationRealApiEnabled
    ? realSaveNurseStationNursingRecords(data)
    : mockSaveNurseStationNursingRecords(data)
}

/** 删除一条已经保存的额外护理记录。 */
export function deleteNurseStationNursingRecord(data: NurseStationDeleteNursingRecordDTO) {
  return isNurseStationRealApiEnabled
    ? realDeleteNurseStationNursingRecord(data)
    : mockDeleteNurseStationNursingRecord(data)
}

/**
 * 查询单个患者的体温单窗口。
 * 根据环境在真实接口和内存 Mock 之间切换；旧接口字段由适配层转换为页面网格。
 */
export function queryNurseStationTemperatureSheet(data: NurseStationTemperatureSheetQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationTemperatureSheet(data)
    : mockQueryNurseStationTemperatureSheet(data)
}

/** 批量保存同一天六个固定时次的生命体征和日汇总数据。 */
export function saveNurseStationTemperatureSheet(data: NurseStationSaveTemperatureSheetDTO) {
  return isNurseStationRealApiEnabled
    ? realSaveNurseStationTemperatureSheet(data)
    : mockSaveNurseStationTemperatureSheet(data)
}

/** 查询住院记账的诊疗项目或耗材目录。 */
export function queryNurseStationBillingCatalog(data: NurseStationBillingCatalogQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationBillingCatalog(data)
    : mockQueryNurseStationBillingCatalog(data)
}

/** 查询可补费父医嘱及其已有费用子项。 */
export function queryNurseStationSupplementOrders(data: NurseStationSupplementOrderQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationSupplementOrders(data)
    : mockQueryNurseStationSupplementOrders(data)
}

/** 查询直接划价和医嘱补费产生的统一明细。 */
export function queryNurseStationPricedOrders(data: NurseStationPricedOrderQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationPricedOrders(data)
    : mockQueryNurseStationPricedOrders(data)
}

/** 批量确认直接划价或医嘱补费。 */
export function confirmNurseStationBilling(data: NurseStationConfirmBillingDTO) {
  return isNurseStationRealApiEnabled
    ? realConfirmNurseStationBilling(data)
    : mockConfirmNurseStationBilling(data)
}

/** 批量取消尚未收费或发放的划价项目。 */
export function cancelNurseStationBilling(data: NurseStationCancelBillingDTO) {
  return isNurseStationRealApiEnabled
    ? realCancelNurseStationBilling(data)
    : mockCancelNurseStationBilling(data)
}

/** 查询护理级别只读项和患者自定义自动计费规则。 */
export function queryNurseStationAutoBillingRules(data: NurseStationAutoBillingQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationAutoBillingRules(data)
    : mockQueryNurseStationAutoBillingRules(data)
}

/** 新增或修改自动计费规则。 */
export function saveNurseStationAutoBillingRule(data: NurseStationSaveAutoBillingRuleDTO) {
  return isNurseStationRealApiEnabled
    ? realSaveNurseStationAutoBillingRule(data)
    : mockSaveNurseStationAutoBillingRule(data)
}

/** 删除自动计费规则。 */
export function deleteNurseStationAutoBillingRule(data: NurseStationDeleteAutoBillingRuleDTO) {
  return isNurseStationRealApiEnabled
    ? realDeleteNurseStationAutoBillingRule(data)
    : mockDeleteNurseStationAutoBillingRule(data)
}

/** Query paginated cost details for one inpatient encounter. */
export function queryNurseStationCostDetail(data: NurseStationCostDetailQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationCostDetail(data)
    : mockQueryNurseStationCostDetail(data)
}

/** Query the backend cost summary grouped by type and date. */
export function queryNurseStationCostSummary(data: NurseStationCostSummaryQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationCostSummary(data)
    : mockQueryNurseStationCostSummary(data)
}

/**
 * 查询选中患者的待汇总发药时点。
 * 真实接口适配后端字段名和筛选枚举，Mock 保持相同的页面契约。
 */
export function queryNurseStationDispensePending(data: NurseStationDispensePendingQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationDispensePending(data)
    : mockQueryNurseStationDispensePending(data)
}

/** 按药房拆分并创建汇总领药单。 */
export function createNurseStationDispenseSummary(data: NurseStationCreateDispenseSummaryDTO) {
  return isNurseStationRealApiEnabled
    ? realCreateNurseStationDispenseSummary(data)
    : mockCreateNurseStationDispenseSummary(data)
}

/** 分页查询当前护士站已经生成的汇总单。 */
export function queryNurseStationDispenseSummaries(data: NurseStationDispenseSummaryQueryDTO) {
  return isNurseStationRealApiEnabled
    ? realQueryNurseStationDispenseSummaries(data)
    : mockQueryNurseStationDispenseSummaries(data)
}

/** 查询一张汇总单的单头和药品合并明细。 */
export function getNurseStationDispenseSummaryDetail(summaryNo: string) {
  return isNurseStationRealApiEnabled
    ? realGetNurseStationDispenseSummaryDetail(summaryNo)
    : mockGetNurseStationDispenseSummaryDetail(summaryNo)
}

/** 批量发送待发送汇总单。 */
export function submitNurseStationDispenseSummaries(data: NurseStationSubmitDispenseSummariesDTO) {
  return isNurseStationRealApiEnabled
    ? realSubmitNurseStationDispenseSummaries(data)
    : mockSubmitNurseStationDispenseSummaries(data)
}

/** 批量撤销待发送汇总单，并恢复被占用的发药时点。 */
export function cancelNurseStationDispenseSummaries(data: NurseStationCancelDispenseSummariesDTO) {
  return isNurseStationRealApiEnabled
    ? realCancelNurseStationDispenseSummaries(data)
    : mockCancelNurseStationDispenseSummaries(data)
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

export { isNurseStationRealApiEnabled } from './patient-real'
