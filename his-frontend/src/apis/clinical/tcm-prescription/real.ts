import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  SaveTcmPrescriptionDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionQuery,
  TcmPrescriptionStationScope,
  TcmPrescriptionStatus,
  TcmPrescriptionVO
} from './type'

interface BackendPrescriptionSummaryDTO {
  prescriptionNo?: string
  summary?: string
  herbNames?: string[]
  authoredAt?: string
}

interface BackendPrescriptionListDTO {
  records?: BackendPrescriptionSummaryDTO[]
}

interface BackendPrescriptionHerbDTO {
  medicineId?: string
  medicineCode?: string
  name?: string
  quantity?: number
  unit?: string
  dose?: number
  doseUnit?: string
  sortNumber?: number
  unitPrice?: number
  subtotal?: number
}

interface BackendPrescriptionDetailDTO {
  prescriptionId?: string
  encounterId?: string
  patientId?: string
  encounterDiagnosisId?: string
  diagnosis?: string
  diagnosisName?: string
  status?: string
  statusName?: string
  usage?: string
  frequency?: string
  days?: number
  copies?: number
  decoction?: boolean
  remark?: string
  version?: number
  createdAt?: string
  statusChangedAt?: string
  details?: BackendPrescriptionHerbDTO[]
}

interface BackendSaveResponseDTO {
  id?: string
  prescriptionNo?: string
  status?: string
  version?: number
  createdAt?: string
  updatedAt?: string
}

interface BackendIssueResponseDTO {
  prescriptionNos?: string[]
  statusEnum?: string
  issuedAt?: string
}

interface BackendRevokeResponseDTO {
  revokedPrescriptionNos?: string[]
  revokedAt?: string
}

const statusNames: Record<TcmPrescriptionStatus, string> = {
  DRAFT: '待签发',
  SIGNED: '已签发'
}

function apiBaseURL(scope: TcmPrescriptionStationScope) {
  if (scope === 'outpatient') {
    return import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_URL || '/api'
  }
  return import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'
}

export function isTcmPrescriptionRealApiEnabled(scope: TcmPrescriptionStationScope) {
  return scope === 'outpatient'
    ? import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_MODE === 'real'
    : import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_MODE === 'real'
}

function unwrapPayload<T>(response: { data?: T } | T): T {
  if (response && typeof response === 'object' && 'data' in response) {
    const data = (response as { data?: T }).data
    if (data !== undefined && data !== null) return data
  }
  return response as T
}

function toStatus(value?: string): TcmPrescriptionStatus {
  const status = value?.trim().toUpperCase()
  return status === 'ACTIVE' || status === 'SIGNED' ? 'SIGNED' : 'DRAFT'
}

function mapDetail(
  detail: BackendPrescriptionDetailDTO,
  query: Pick<TcmPrescriptionQuery, 'scope' | 'encounterId' | 'patientId'>
): TcmPrescriptionVO {
  const prescriptionId = detail.prescriptionId?.trim() || ''
  const status = toStatus(detail.status)
  const lines = (detail.details ?? [])
    .slice()
    .sort((left, right) => Number(left.sortNumber ?? 0) - Number(right.sortNumber ?? 0))
    .map((line, index) => ({
      id: `${prescriptionId || 'tcm'}-${index + 1}`,
      catalogId: line.medicineId || '',
      code: line.medicineCode || '',
      name: line.name || '',
      specification: '',
      dose: Number(line.quantity ?? line.dose ?? 0),
      doseUnit: line.unit || line.doseUnit || '',
      retailPrice: Number(line.unitPrice ?? 0),
      orderNo: Number(line.sortNumber ?? index + 1)
    }))

  return {
    id: prescriptionId,
    scope: query.scope,
    encounterId: detail.encounterId || query.encounterId,
    patientId: detail.patientId || query.patientId,
    prescriptionNo: prescriptionId || undefined,
    encounterDiagnosisId: detail.encounterDiagnosisId || detail.diagnosis || '',
    syndromeName: detail.diagnosisName || detail.diagnosis || '',
    doseCount: Number(detail.copies ?? 0),
    method: detail.usage || '',
    frequency: detail.frequency || '',
    days: Number(detail.days ?? 0),
    decoctionMethod: detail.decoction ? '代煎' : '自煎',
    remark: detail.remark || undefined,
    status,
    statusName: detail.statusName || statusNames[status],
    lines,
    authoredAt: detail.createdAt || '',
    updatedAt: detail.statusChangedAt || detail.createdAt || '',
    version: detail.version
  }
}

async function listSummaries(
  scope: TcmPrescriptionStationScope,
  patientId: string,
  encounterId?: string,
  keyword?: string
) {
  const response = await useHttp().get<BackendPrescriptionListDTO>(
    '/chinapres',
    {
      patientId,
      ...(encounterId ? { encounterId } : {}),
      ...(keyword?.trim() ? { keyword: keyword.trim() } : {})
    },
    { baseURL: apiBaseURL(scope) }
  )
  return unwrapPayload(response).records ?? []
}

async function loadDetail(
  scope: TcmPrescriptionStationScope,
  patientId: string,
  prescriptionNo: string,
  encounterId?: string
) {
  const response = await useHttp().get<BackendPrescriptionDetailDTO>(
    '/chinapres-detail',
    {
      patientId,
      presId: prescriptionNo,
      ...(encounterId ? { encounterId } : {})
    },
    { baseURL: apiBaseURL(scope) }
  )
  return unwrapPayload(response)
}

export async function realListTcmPrescriptions(
  query: TcmPrescriptionQuery
): Promise<TcmPrescriptionVO[]> {
  const summaries = await listSummaries(query.scope, query.patientId, query.encounterId)
  const details = await Promise.all(
    summaries
      .map((item) => item.prescriptionNo?.trim() || '')
      .filter(Boolean)
      .map((prescriptionNo) =>
        loadDetail(query.scope, query.patientId, prescriptionNo, query.encounterId)
      )
  )
  return details.map((detail) => mapDetail(detail, query))
}

export async function realListTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  const summaries = await listSummaries(query.scope, query.patientId, undefined, query.keyword)
  const details = await Promise.all(
    summaries
      .map((item) => item.prescriptionNo?.trim() || '')
      .filter(Boolean)
      .map((prescriptionNo) => loadDetail(query.scope, query.patientId, prescriptionNo))
  )

  return details
    .map((detail) =>
      mapDetail(detail, {
        scope: query.scope,
        encounterId: detail.encounterId || '',
        patientId: query.patientId
      })
    )
    .filter((record) => record.encounterId !== query.currentEncounterId)
    .map((record) => ({
      id: record.id,
      prescriptionNo: record.prescriptionNo || record.id,
      encounterId: record.encounterId,
      patientId: record.patientId,
      authoredAt: record.authoredAt,
      summary: `${record.syndromeName || '未关联诊断'} · ${record.doseCount}付`,
      herbNames: record.lines.map((line) => line.name)
    }))
}

export async function realPageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  const response = await useHttp().post<PageDTO<TcmHerbCatalogItemVO>>(
    '/tcm-prescription/page-herb-catalog',
    query,
    { baseURL: apiBaseURL(query.scope) }
  )
  return unwrapPayload(response)
}

function savePayload(data: SaveTcmPrescriptionDTO) {
  return {
    ...(data.id ? { id: data.id } : {}),
    encounterId: data.encounterId,
    patientId: data.patientId,
    encounterDiagnosisId: data.encounterDiagnosisId,
    usage: data.usage,
    frequency: data.frequency,
    days: data.days,
    copies: data.copies,
    decoction: data.decoction,
    ...(data.remark ? { remark: data.remark } : {}),
    ...(data.version !== undefined ? { version: data.version } : {}),
    details: data.details
  }
}

function savedRecord(data: SaveTcmPrescriptionDTO, result: BackendSaveResponseDTO) {
  const status = toStatus(result.status)
  return {
    id: result.id || data.id || '',
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: data.patientId,
    prescriptionNo: result.prescriptionNo,
    encounterDiagnosisId: data.encounterDiagnosisId,
    syndromeName: data.diagnosisName,
    doseCount: data.copies,
    method: data.usage,
    frequency: data.frequency,
    days: data.days,
    decoctionMethod: data.decoction ? '代煎' : '自煎',
    remark: data.remark,
    status,
    statusName: statusNames[status],
    lines: [],
    authoredAt: result.createdAt || '',
    updatedAt: result.updatedAt || result.createdAt || '',
    version: result.version
  } satisfies TcmPrescriptionVO
}

export async function realSaveTcmPrescriptionDraft(
  data: SaveTcmPrescriptionDTO
): Promise<TcmPrescriptionVO> {
  const response = data.id
    ? await useHttp().put<BackendSaveResponseDTO>('/chinapres2/update', savePayload(data), {
        baseURL: apiBaseURL(data.scope)
      })
    : await useHttp().post<BackendSaveResponseDTO>('/chinapres2/add', savePayload(data), {
        baseURL: apiBaseURL(data.scope)
      })
  return savedRecord(data, unwrapPayload(response))
}

export async function realDeleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  await useHttp().post(
    '/chinapres2/remove',
    { id: data.id, encounterId: data.encounterId, version: data.version ?? 0 },
    { baseURL: apiBaseURL(data.scope) }
  )
}

export async function realSignTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  const response = await useHttp().post<BackendIssueResponseDTO>(
    '/chinapres3/issue',
    {
      encounterId: data.encounterId,
      signTargets: [{ medicationRequestId: data.id }]
    },
    { baseURL: apiBaseURL(data.scope) }
  )
  unwrapPayload(response)
  return {
    id: data.id,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: '',
    encounterDiagnosisId: '',
    syndromeName: '',
    doseCount: 0,
    method: '',
    frequency: '',
    days: 0,
    status: 'SIGNED',
    statusName: statusNames.SIGNED,
    lines: [],
    authoredAt: '',
    updatedAt: ''
  }
}

export async function realWithdrawTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  const response = await useHttp().post<BackendRevokeResponseDTO>(
    '/chinapres3/revoke',
    { medicationRequestIds: [data.id] },
    { baseURL: apiBaseURL(data.scope) }
  )
  unwrapPayload(response)
  return {
    id: data.id,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: '',
    encounterDiagnosisId: '',
    syndromeName: '',
    doseCount: 0,
    method: '',
    frequency: '',
    days: 0,
    status: 'DRAFT',
    statusName: statusNames.DRAFT,
    lines: [],
    authoredAt: '',
    updatedAt: ''
  }
}
