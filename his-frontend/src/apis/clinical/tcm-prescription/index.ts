import type { PageDTO } from '../../type'
import {
  mockDeleteTcmPrescription,
  mockListTcmPrescriptionHistory,
  mockListTcmPrescriptions,
  mockPageTcmHerbCatalog,
  mockSaveTcmPrescriptionDraft,
  mockSignTcmPrescription,
  mockWithdrawTcmPrescription
} from './mock'
import {
  isTcmPrescriptionRealApiEnabled,
  realDeleteTcmPrescription,
  realListTcmPrescriptionHistory,
  realListTcmPrescriptions,
  realPageTcmHerbCatalog,
  realSaveTcmPrescriptionDraft,
  realSignTcmPrescription,
  realWithdrawTcmPrescription
} from './real'
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

/**
 * 中医处方版本缓存（处方级）。
 *
 * 后端中医处方保存/删除/状态变更接口要求携带处方版本，取最近一次查询或
 * 操作响应的值。这里按处方 ID 保存最近一次 version，操作请求由 API 层自动
 * 携带，页面无需感知。
 */
const tcmPrescriptionVersionCache = new Map<string, number>()

function rememberTcmVersion(id: string, version?: number) {
  if (version !== undefined) tcmPrescriptionVersionCache.set(id, version)
}

/** 取缓存中的处方版本；未查询过则从 0 开始，由 mock/后端决定是否拒绝。 */
function resolveTcmVersion(id: string): number {
  return tcmPrescriptionVersionCache.get(id) ?? 0
}

/** 查询当前就诊中医处方。 */
export function listTcmPrescriptions(query: TcmPrescriptionQuery): Promise<TcmPrescriptionVO[]> {
  const request = isTcmPrescriptionRealApiEnabled(query.scope)
    ? realListTcmPrescriptions(query)
    : mockListTcmPrescriptions(query)
  return request.then((result) => {
    for (const record of result) rememberTcmVersion(record.id, record.version)
    return result
  })
}

/** 查询患者历史中医处方。 */
export function listTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  return isTcmPrescriptionRealApiEnabled(query.scope)
    ? realListTcmPrescriptionHistory(query)
    : mockListTcmPrescriptionHistory(query)
}

/** 分页查询中药材目录。 */
export function pageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  return isTcmPrescriptionRealApiEnabled(query.scope)
    ? realPageTcmHerbCatalog(query)
    : mockPageTcmHerbCatalog(query)
}

/** 新增或修改中医处方草稿。 */
export function saveTcmPrescriptionDraft(data: SaveTcmPrescriptionDTO): Promise<TcmPrescriptionVO> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realSaveTcmPrescriptionDraft(data)
  const withVersion: SaveTcmPrescriptionDTO =
    data.id !== undefined ? { ...data, version: data.version ?? resolveTcmVersion(data.id) } : data
  return mockSaveTcmPrescriptionDraft(withVersion).then((result) => {
    rememberTcmVersion(result.id, result.version)
    return result
  })
}

/** 删除待签发中医处方。 */
export function deleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realDeleteTcmPrescription(data)
  return mockDeleteTcmPrescription({
    ...data,
    version: data.version ?? resolveTcmVersion(data.id)
  })
}

/** 签发中医处方。 */
export function signTcmPrescription(data: TcmPrescriptionActionDTO): Promise<TcmPrescriptionVO> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realSignTcmPrescription(data)
  return mockSignTcmPrescription({
    ...data,
    version: data.version ?? resolveTcmVersion(data.id)
  }).then((result) => {
    rememberTcmVersion(result.id, result.version)
    return result
  })
}

/** 撤回已签发中医处方。 */
export function withdrawTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  if (isTcmPrescriptionRealApiEnabled(data.scope)) return realWithdrawTcmPrescription(data)
  return mockWithdrawTcmPrescription({
    ...data,
    version: data.version ?? resolveTcmVersion(data.id)
  }).then((result) => {
    rememberTcmVersion(result.id, result.version)
    return result
  })
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
  TcmPrescriptionVO
} from './type'
