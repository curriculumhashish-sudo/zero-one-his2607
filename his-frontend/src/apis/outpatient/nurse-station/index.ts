import type { PageDTO } from '../../type'
import {
  mockCancelNurseExecution,
  mockGetNurseConsumables,
  mockGetNurseExecutionItems,
  mockPageNursePatientQueue,
  mockPagePatientExecutionHistory,
  mockPerformNurseExecutions
} from './mock'
import {
  isOutpatientNurseStationRealApiEnabled,
  realCancelNurseExecution,
  realGetNurseConsumables,
  realGetNurseExecutionItems,
  realPageNursePatientQueue,
  realPagePatientExecutionHistory,
  realPerformNurseExecutions
} from './real'
import type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NurseBatchResultVO,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  PerformNurseExecutionsDTO
} from './type'

/** 分页查询护士站患者队列。 */
export function pageNursePatientQueue(
  query: NursePatientQueueQuery
): Promise<PageDTO<NursePatientQueueItemVO>> {
  return isOutpatientNurseStationRealApiEnabled
    ? realPageNursePatientQueue(query)
    : mockPageNursePatientQueue(query)
}

/** 查询当前就诊的执行项目。 */
export function getNurseExecutionItems(
  patientId: string,
  encounterId?: string
): Promise<NurseExecutionItemVO[]> {
  return isOutpatientNurseStationRealApiEnabled
    ? realGetNurseExecutionItems(patientId, encounterId)
    : mockGetNurseExecutionItems(patientId, encounterId)
}

/** 查询当前就诊的耗材项目。 */
export function getNurseConsumables(
  patientId: string,
  encounterId?: string
): Promise<NurseConsumableItemVO[]> {
  return isOutpatientNurseStationRealApiEnabled
    ? realGetNurseConsumables(patientId, encounterId)
    : mockGetNurseConsumables(patientId, encounterId)
}

/** 批量执行当前选择的护士项目。 */
export function performNurseExecutions(
  request: PerformNurseExecutionsDTO
): Promise<NurseBatchResultVO> {
  return isOutpatientNurseStationRealApiEnabled
    ? realPerformNurseExecutions(request)
    : mockPerformNurseExecutions(request)
}

/** 取消项目最近一次执行。 */
export function cancelNurseExecution(
  request: CancelNurseExecutionDTO
): Promise<NurseBatchResultVO> {
  return isOutpatientNurseStationRealApiEnabled
    ? realCancelNurseExecution(request)
    : mockCancelNurseExecution(request)
}

/** 分页查询患者历史执行记录（跨就诊）。 */
export function pagePatientExecutionHistory(
  patientId: string,
  query: NurseExecutionHistoryQuery
): Promise<PageDTO<NurseExecutionRecordVO>> {
  return isOutpatientNurseStationRealApiEnabled
    ? realPagePatientExecutionHistory(patientId, query)
    : mockPagePatientExecutionHistory(patientId, query)
}

export { isOutpatientNurseStationRealApiEnabled }

export type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionCommandItemDTO,
  NurseBatchFailedItemVO,
  NurseBatchResultVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NurseExecutionStatus,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  NursePatientStatus,
  PerformNurseExecutionsDTO
} from './type'
