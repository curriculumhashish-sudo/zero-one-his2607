import type { PageDTO } from '../../type'
import {
  mockDeleteTcmPrescription,
  mockListTcmPrescriptionHistory,
  mockListTcmPrescriptions,
  mockPageTcmHerbCatalog,
  mockSaveTcmPrescriptionDraft,
  mockSignTcmPrescription,
  mockWithdrawTcmPrescription,
  mockStopTcmPrescription,
  mockCancelStopTcmPrescription
} from './mock'
import {
  realDeleteTcmPrescription,
  realListTcmPrescriptionHistory,
  realListTcmPrescriptionHistoryDetails,
  realListTcmPrescriptions,
  realPageTcmHerbCatalog,
  realSaveTcmPrescriptionDraft,
  realSignTcmPrescription,
  realWithdrawTcmPrescription,
  realStopTcmPrescription,
  realCancelStopTcmPrescription
} from './real'
import { isTcmPrescriptionRealApiEnabled } from './mode'
import type {
  SaveTcmPrescriptionDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryDetailVO,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionQuery,
  TcmPrescriptionStationScope,
  TcmPrescriptionVO
} from './type'

/** 查询当前就诊中医处方。 */
export function listTcmPrescriptions(query: TcmPrescriptionQuery): Promise<TcmPrescriptionVO[]> {
  return isTcmPrescriptionRealApiEnabled(query.scope)
    ? realListTcmPrescriptions(query)
    : mockListTcmPrescriptions(query)
}

/** 查询患者历史中医处方。 */
export function listTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  return isTcmPrescriptionRealApiEnabled(query.scope)
    ? realListTcmPrescriptionHistory(query)
    : mockListTcmPrescriptionHistory(query)
}

export function listTcmPrescriptionHistoryDetails(query: {
  scope: TcmPrescriptionStationScope
  visitId: string
  keyword?: string
}) {
  if (isTcmPrescriptionRealApiEnabled(query.scope)) {
    return realListTcmPrescriptionHistoryDetails(query)
  }
  return Promise.resolve([] as TcmPrescriptionHistoryDetailVO[])
}

/** 分页查询中药材目录。 */
export function pageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  return isTcmPrescriptionRealApiEnabled(query.scope)
    ? realPageTcmHerbCatalog(query)
    : mockPageTcmHerbCatalog(query)
}

/** 新增中医处方草稿。已保存处方暂不支持修改。 */
export function saveTcmPrescriptionDraft(data: SaveTcmPrescriptionDTO): Promise<TcmPrescriptionVO> {
  return isTcmPrescriptionRealApiEnabled(data.scope)
    ? realSaveTcmPrescriptionDraft(data)
    : mockSaveTcmPrescriptionDraft(data)
}

/** 删除待签发中医处方。 */
export function deleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realDeleteTcmPrescription(data)
  return mockDeleteTcmPrescription(data)
}

/** 签发中医处方。 */
export function signTcmPrescription(data: TcmPrescriptionActionDTO): Promise<TcmPrescriptionVO> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realSignTcmPrescription(data)
  return mockSignTcmPrescription(data)
}

/** 撤回已签发中医处方。 */
export function withdrawTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realWithdrawTcmPrescription(data)
  return mockWithdrawTcmPrescription(data)
}

export function stopTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  return isTcmPrescriptionRealApiEnabled(data.scope)
    ? realStopTcmPrescription(data)
    : mockStopTcmPrescription(data)
}

export function cancelStopTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  return isTcmPrescriptionRealApiEnabled(data.scope)
    ? realCancelStopTcmPrescription(data)
    : mockCancelStopTcmPrescription(data)
}

export type {
  SaveTcmPrescriptionDTO,
  SaveTcmPrescriptionLineDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryDetailVO,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionLineVO,
  TcmPrescriptionQuery,
  TcmPrescriptionStationScope,
  TcmPrescriptionStatus,
  TcmPrescriptionVO
} from './type'
