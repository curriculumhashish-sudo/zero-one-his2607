import type { PageDTO } from '../../type'
import {
  mockCancelStopTcmPrescription,
  mockDeleteTcmPrescription,
  mockListTcmPrescriptionHistory,
  mockListTcmPrescriptions,
  mockPageTcmHerbCatalog,
  mockSaveTcmPrescriptionDraft,
  mockSignTcmPrescription,
  mockStopTcmPrescription,
  mockWithdrawTcmPrescription
} from './mock'
import type {
  SaveTcmPrescriptionDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionQuery,
  TcmPrescriptionVO
} from './type'

/** 查询当前就诊中医处方。 */
export function listTcmPrescriptions(query: TcmPrescriptionQuery): Promise<TcmPrescriptionVO[]> {
  return mockListTcmPrescriptions(query)
}

/** 查询患者历史中医处方。 */
export function listTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  return mockListTcmPrescriptionHistory(query)
}

/** 分页查询中药饮片目录。 */
export function pageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  return mockPageTcmHerbCatalog(query)
}

/** 新增或修改中医处方草稿。 */
export function saveTcmPrescriptionDraft(data: SaveTcmPrescriptionDTO): Promise<TcmPrescriptionVO> {
  return mockSaveTcmPrescriptionDraft(data)
}

/** 删除待签发中医处方。 */
export function deleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  return mockDeleteTcmPrescription(data)
}

/** 签发中医处方。 */
export function signTcmPrescription(data: TcmPrescriptionActionDTO): Promise<TcmPrescriptionVO> {
  return mockSignTcmPrescription(data)
}

/** 撤回已签发中医处方。 */
export function withdrawTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  return mockWithdrawTcmPrescription(data)
}

/** 停止住院长期中医处方。 */
export function stopTcmPrescription(data: TcmPrescriptionActionDTO): Promise<TcmPrescriptionVO> {
  return mockStopTcmPrescription(data)
}

/** 取消停止住院长期中医处方。 */
export function cancelStopTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  return mockCancelStopTcmPrescription(data)
}

export type {
  SaveTcmPrescriptionDTO,
  SaveTcmPrescriptionLineDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionLineVO,
  TcmPrescriptionQuery,
  TcmPrescriptionStationScope,
  TcmPrescriptionStatus,
  TcmPrescriptionTherapyType,
  TcmPrescriptionVO
} from './type'
