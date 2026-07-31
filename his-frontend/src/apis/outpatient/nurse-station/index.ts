import type { PageDTO } from '../../type'
import {
  mockCancelNurseExecution,
  mockGetNurseConsumables,
  mockGetNurseExecutionItems,
  mockPageNursePatientQueue,
  mockPagePatientExecutionHistory,
  mockPerformNurseExecutions,
  mockPrintNurseDocuments
} from './mock'
import type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionCommandResultVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  PerformNurseExecutionsDTO,
  NursePrintRequestDTO,
  NursePrintResultVO
} from './type'

/**
 * 分页查询护士站患者队列。
 *
 * 当前由 Mock 提供数据；后端契约确定后只替换此文件内部实现。
 */
export function pageNursePatientQueue(
  query: NursePatientQueueQuery
): Promise<PageDTO<NursePatientQueueItemVO>> {
  return mockPageNursePatientQueue(query)
}

/** 查询当前就诊的执行项目。 */
export function getNurseExecutionItems(encounterId: string): Promise<NurseExecutionItemVO[]> {
  return mockGetNurseExecutionItems(encounterId)
}

/** 查询当前就诊的耗材项目。 */
export function getNurseConsumables(encounterId: string): Promise<NurseConsumableItemVO[]> {
  return mockGetNurseConsumables(encounterId)
}

/** 批量执行当前选择的护士项目。 */
export function performNurseExecutions(
  request: PerformNurseExecutionsDTO
): Promise<NurseExecutionCommandResultVO> {
  return mockPerformNurseExecutions(request)
}

/** 取消项目最近一次执行。 */
export function cancelNurseExecution(
  request: CancelNurseExecutionDTO
): Promise<NurseExecutionCommandResultVO> {
  return mockCancelNurseExecution(request)
}

/** 提交当前护士站的打印任务。 */
export function printNurseDocuments(request: NursePrintRequestDTO): Promise<NursePrintResultVO> {
  return mockPrintNurseDocuments(request)
}

/**
 * 分页查询患者历史执行记录（跨就诊）。
 *
 * 当前由 Mock 提供数据；后端契约确定后只替换此文件内部实现。
 */
export function pagePatientExecutionHistory(
  patientId: string,
  query: NurseExecutionHistoryQuery
): Promise<PageDTO<NurseExecutionRecordVO>> {
  return mockPagePatientExecutionHistory(patientId, query)
}

export type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseConsumableStatus,
  NurseExecutionCommandItemDTO,
  NurseExecutionCommandResultVO,
  NurseExecutionFailureCode,
  NurseExecutionFailureVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemType,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NurseExecutionStatus,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  NursePatientStatus,
  NursePrintDocumentType,
  PerformNurseExecutionsDTO,
  NursePrintRequestDTO,
  NursePrintResultVO
} from './type'
