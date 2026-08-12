import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  SaveTcmPrescriptionDTO,
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
import { outpatientDoctorStationApiBaseURL } from '@/apis/outpatient/doctor-station/mode'

interface BackendEnvelope<T> {
  code?: number
  message?: string
  data?: T
}

interface BackendCurrentSummaryDTO {
  id?: string
  prescriptionNo?: string
  encounterId?: string
  authoredAt?: string
  summary?: string
  herbNames?: string[]
}

interface BackendLegacyCurrentPrescriptionDTO {
  id?: string
  prescriptionGroupKey?: string
  prescriptionNo?: string
  statusEnum?: string
  days?: number
  chineseHerbsDoseQuantity?: number
  sufferingFlag?: boolean | number
  description?: string
  lines?: Array<{
    medicationRequestId?: string
    medicationDefinitionId?: string
    medicationName?: string
    dose?: number
    unitName?: string
  }>
}

interface BackendCurrentListDTO {
  records?: BackendCurrentSummaryDTO[]
  prescriptions?: BackendLegacyCurrentPrescriptionDTO[]
}

interface BackendCurrentDetailLineDTO {
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

interface BackendCurrentDetailDTO {
  id?: string
  prescriptionId?: string
  encounterId?: string
  patientId?: string
  status?: string
  statusName?: string
  usage?: string
  frequency?: string
  days?: number
  copies?: number
  decoction?: boolean | number
  diagnosis?: string
  remark?: string
  version?: number
  createdAt?: string
  details?: BackendCurrentDetailLineDTO[]
}

interface BackendHistoryRowDTO {
  visitId?: string
  visitTime?: string
  visitType?: string
  mainDiagnosis?: string
  doctorName?: string
  drugCount?: number
  examCount?: number
  materialCount?: number
}

interface BackendHistoryPageDTO {
  pageIndex?: number
  pageSize?: number
  total?: number
  rows?: BackendHistoryRowDTO[]
}

interface BackendHistoryDetailRowDTO {
  orderId?: string
  orderType?: string
  orderName?: string
  quantity?: number
  executedCount?: number
  sampleType?: string
  executeHospital?: string
  createTime?: string
  diagnosis?: string
  doctorName?: string
  isSkinTest?: number
}

interface BackendHistoryDetailPageDTO {
  rows?: BackendHistoryDetailRowDTO[]
}

interface BackendSaveResponseDTO {
  id?: string
  prescriptionNo?: string
  status?: string
  version?: number
  createdAt?: string
  updatedAt?: string
  herbCount?: number
  singleDoseAmount?: number
  totalAmount?: number
}

interface BackendSignResponseDTO {
  prescriptionNos?: string[]
  prescriptionNoByGroupKey?: Record<string, string>
  statusEnum?: string
  issuedAt?: string
}

interface BackendRevokeResponseDTO {
  revokedPrescriptionNos?: string[]
  revokedAt?: string
}

interface BackendCatalogRowDTO {
  id?: string
  code?: string
  name?: string
  specification?: string
  defaultDose?: number
  doseUnit?: string
  retailPrice?: number
  categoryCode?: string
  category?: string
  inventoryQuantity?: number
  inventoryUnit?: string
}

interface BackendCatalogPageDTO {
  pageIndex?: number
  pageSize?: number
  total?: number
  rows?: BackendCatalogRowDTO[]
}

const statusNames: Record<TcmPrescriptionStatus, string> = {
  DRAFT: '待签发',
  UNSIGNED: '待签发',
  ACTIVE: '已激活',
  SIGNED: '已签发',
  COMPLETED: '已完成',
  STOPPED: '已停用'
}

function apiBaseURL(scope: TcmPrescriptionStationScope) {
  if (scope === 'outpatient') return outpatientDoctorStationApiBaseURL
  return import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'
}

function unwrapPayload<T>(response: BackendEnvelope<T> | T): T {
  if (response && typeof response === 'object') {
    const body = response as BackendEnvelope<T>
    if (typeof body.code === 'number' && body.code !== 0 && body.code !== 10000) {
      throw new Error(body.message || '中医处方接口请求失败')
    }
    if (body.data !== undefined && body.data !== null) return body.data
  }
  return response as T
}

function toStatus(value?: string): TcmPrescriptionStatus {
  const status = value?.trim().toUpperCase()
  if (
    status === 'DRAFT' ||
    status === 'UNSIGNED' ||
    status === 'ACTIVE' ||
    status === 'SIGNED' ||
    status === 'COMPLETED' ||
    status === 'STOPPED'
  ) {
    return status
  }
  return 'UNSIGNED'
}

function toBoolean(value: boolean | number | undefined): boolean {
  return value === true || value === 1
}

function mapLine(line: BackendCurrentDetailLineDTO): TcmPrescriptionLineVO {
  const medicationDefinitionId = line.medicineId || ''
  return {
    medicationDefinitionId,
    catalogId: medicationDefinitionId,
    code: line.medicineCode,
    name: line.name || '',
    dose: Number(line.dose ?? line.quantity ?? 0),
    doseUnit: line.doseUnit || line.unit || '',
    retailUnitPrice: Number(line.unitPrice ?? 0),
    retailPrice: Number(line.unitPrice ?? 0),
    orderNo: Number(line.sortNumber ?? 0)
  }
}

function mapCurrentDetail(
  detail: BackendCurrentDetailDTO,
  summary: BackendCurrentSummaryDTO,
  query: TcmPrescriptionQuery
): TcmPrescriptionVO {
  const prescriptionNo = (detail.prescriptionId || summary.prescriptionNo || '').trim()
  const id = (detail.id || summary.id || '').trim()
  const status = toStatus(detail.status)
  return {
    id: id || undefined,
    scope: query.scope,
    encounterId: detail.encounterId || query.encounterId,
    patientId: detail.patientId || query.patientId,
    prescriptionGroupKey: prescriptionNo || id || undefined,
    prescriptionNo: prescriptionNo || undefined,
    encounterDiagnosisId: detail.diagnosis,
    methodCode: detail.usage,
    methodName: detail.usage,
    rateCode: detail.frequency,
    rateName: detail.frequency,
    syndromeName: detail.diagnosis || '',
    doseCount: Number(detail.copies ?? 0),
    days: Number(detail.days ?? 0),
    decoctionMethod: toBoolean(detail.decoction) ? '代煎' : '自煎',
    remark: detail.remark || undefined,
    status,
    statusName: detail.statusName || statusNames[status],
    version: Number(detail.version ?? 0),
    authoredAt: detail.createdAt,
    updatedAt: detail.createdAt,
    lines: (detail.details || []).map(mapLine)
  }
}

function mapLegacyCurrent(
  item: BackendLegacyCurrentPrescriptionDTO,
  query: TcmPrescriptionQuery
): TcmPrescriptionVO {
  const status = toStatus(item.statusEnum)
  const id = item.id || item.prescriptionNo || item.prescriptionGroupKey
  return {
    id,
    scope: query.scope,
    encounterId: query.encounterId,
    patientId: query.patientId,
    prescriptionGroupKey: item.prescriptionGroupKey || item.prescriptionNo || id,
    prescriptionNo: item.prescriptionNo,
    doseCount: Number(item.chineseHerbsDoseQuantity ?? 0),
    days: Number(item.days ?? 0),
    decoctionMethod: toBoolean(item.sufferingFlag) ? '代煎' : '自煎',
    syndromeName: item.description || '',
    status,
    statusName: statusNames[status],
    lines: (item.lines || []).map((line) => ({
      id: line.medicationRequestId,
      medicationRequestId: line.medicationRequestId,
      medicationDefinitionId: line.medicationDefinitionId,
      catalogId: line.medicationDefinitionId,
      name: line.medicationName || '',
      dose: Number(line.dose ?? 0),
      doseUnit: line.unitName || ''
    }))
  }
}

export async function realListTcmPrescriptions(
  query: TcmPrescriptionQuery
): Promise<TcmPrescriptionVO[]> {
  const response = await useHttp().get<BackendCurrentListDTO>(
    '/c2-opdoctor/chinapres',
    { encounterId: query.encounterId, patientId: query.patientId },
    { baseURL: apiBaseURL(query.scope) }
  )
  const payload = unwrapPayload(response)
  if (!payload.records?.length && payload.prescriptions?.length) {
    return payload.prescriptions.map((item) => mapLegacyCurrent(item, query))
  }
  const summaries = payload.records || []
  return Promise.all(
    summaries.map(async (summary) => {
      if (!summary.prescriptionNo) {
        return mapCurrentDetail(summary, summary, query)
      }
      const detailResponse = await useHttp().get<BackendCurrentDetailDTO>(
        '/c2-opdoctor/chinapres-detail',
        {
          encounterId: query.encounterId,
          patientId: query.patientId,
          presId: summary.prescriptionNo
        },
        { baseURL: apiBaseURL(query.scope) }
      )
      return mapCurrentDetail(unwrapPayload(detailResponse), summary, query)
    })
  )
}

export async function realListTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  const response = await useHttp().get<BackendHistoryPageDTO>(
    '/c2-opdoctor/historyOrder/list',
    {
      patientId: query.patientId,
      keyword: query.keyword || undefined,
      pageIndex: 1,
      pageSize: 20
    },
    { baseURL: apiBaseURL(query.scope) }
  )
  const payload = unwrapPayload(response)
  return (payload.rows || []).map((item, index) => ({
    id: item.visitId || `visit-${index + 1}`,
    visitId: item.visitId,
    visitTime: item.visitTime,
    visitType: item.visitType,
    mainDiagnosis: item.mainDiagnosis,
    doctorName: item.doctorName,
    drugCount: Number(item.drugCount ?? 0),
    examCount: Number(item.examCount ?? 0),
    materialCount: Number(item.materialCount ?? 0),
    patientId: query.patientId,
    encounterId: item.visitId || '',
    prescriptionNo: '',
    summary: item.mainDiagnosis || '',
    herbNames: []
  }))
}

export async function realListTcmPrescriptionHistoryDetails(
  query: { scope: TcmPrescriptionStationScope; visitId: string; keyword?: string }
) {
  const response = await useHttp().get<BackendHistoryDetailPageDTO>(
    '/c2-opdoctor/historyOrder/detail',
    { visitId: query.visitId, orderType: 'DRUG', keyword: query.keyword || undefined, pageIndex: 1, pageSize: 100 },
    { baseURL: apiBaseURL(query.scope) }
  )
  const payload = unwrapPayload(response)
  return (payload.rows || [])
    .filter((item) => !item.orderType || item.orderType === 'DRUG')
    .map((item) => ({
      id: item.orderId || '',
      name: item.orderName || '',
      quantity: Number(item.quantity ?? 0),
      executedCount: Number(item.executedCount ?? 0),
      createTime: item.createTime,
      diagnosis: item.diagnosis,
      doctorName: item.doctorName
    }))
}

export async function realPageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  const response = await useHttp().post<BackendCatalogPageDTO>(
    '/tcm-prescription/page-herb-catalog',
    { scope: query.scope, keyword: query.keyword || undefined, pageIndex: query.pageIndex, pageSize: query.pageSize },
    { baseURL: apiBaseURL(query.scope) }
  )
  const page = unwrapPayload(response)
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? 0,
    rows: (page.rows || []).map((item) => ({
      id: item.id || '',
      code: item.code,
      name: item.name || '',
      specification: item.specification,
      defaultDose: Number(item.defaultDose ?? 0),
      doseUnit: item.doseUnit || '',
      doseUnitCode: item.doseUnit,
      retailPrice: Number(item.retailPrice ?? 0),
      retailUnitPrice: Number(item.retailPrice ?? 0),
      categoryCode: item.categoryCode,
      category: item.category,
      inventoryQuantity: item.inventoryQuantity,
      inventoryUnit: item.inventoryUnit
    }))
  }
}

function savePayload(data: SaveTcmPrescriptionDTO) {
  return {
    encounterId: data.encounterId,
    patientId: data.patientId,
    encounterDiagnosisId: data.encounterDiagnosisId || '',
    usage: data.usage || '',
    frequency: data.frequency || '',
    days: data.days,
    copies: data.copies ?? data.chineseHerbsDoseQuantity ?? 1,
    decoction: data.decoction ?? (data.sufferingFlag ? 1 : 0),
    remark: data.remark || '',
    details: data.details.map((line, index) => ({
      medicineId: line.medicineId,
      quantity: line.quantity,
      unit: line.unit,
      dose: line.dose,
      doseUnit: line.doseUnit || line.doseUnitCode,
      sortNo: line.sortNo ?? index + 1
    }))
  }
}

export async function realSaveTcmPrescriptionDraft(
  data: SaveTcmPrescriptionDTO
): Promise<TcmPrescriptionVO> {
  if (data.id) throw new Error('已保存的中医处方本轮不可修改，请删除后重新开立')
  const response = await useHttp().post<BackendSaveResponseDTO>(
    '/c2-opdoctor/chinapres2/add',
    savePayload(data),
    { baseURL: apiBaseURL(data.scope) }
  )
  const result = unwrapPayload(response)
  const status = toStatus(result.status)
  return {
    id: result.id,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: data.patientId,
    prescriptionNo: result.prescriptionNo,
    prescriptionGroupKey: result.prescriptionNo || result.id,
    encounterDiagnosisId: data.encounterDiagnosisId,
    methodCode: data.usage,
    methodName: data.usage,
    rateCode: data.frequency,
    rateName: data.frequency,
    syndromeName: data.diagnosisName,
    doseCount: data.copies ?? data.chineseHerbsDoseQuantity ?? 0,
    days: data.days,
    decoctionMethod: (data.decoction ?? (data.sufferingFlag ? 1 : 0)) ? '代煎' : '自煎',
    remark: data.remark,
    status,
    statusName: statusNames[status],
    version: Number(result.version ?? 0),
    authoredAt: result.createdAt,
    updatedAt: result.updatedAt,
    lines: []
  }
}

function requestIds(data: TcmPrescriptionActionDTO): string[] {
  if (data.id) return [data.id]
  const ids = data.medicationRequestIds?.filter(Boolean) || []
  if (ids.length) return ids
  throw new Error('缺少处方请求 ID')
}

export async function realDeleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  await useHttp().post<BackendEnvelope<unknown>>(
    '/c2-opdoctor/chinapres2/remove',
    { id: requestIds(data)[0], encounterId: data.encounterId, version: data.version ?? 0 },
    { baseURL: apiBaseURL(data.scope) }
  )
}

export async function realSignTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  const response = await useHttp().post<BackendSignResponseDTO>(
    '/c2-opdoctor/chinapres3/issue',
    { encounterId: data.encounterId, signTargets: requestIds(data).map((id) => ({ medicationRequestId: id })) },
    { baseURL: apiBaseURL(data.scope) }
  )
  const result = unwrapPayload(response)
  const status = toStatus(result.statusEnum)
  const prescriptionNo =
    result.prescriptionNos?.[0] ||
    result.prescriptionNoByGroupKey?.[data.prescriptionGroupKey || ''] ||
    data.prescriptionNo
  return {
    id: data.id,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: data.patientId || '',
    prescriptionGroupKey: data.prescriptionGroupKey,
    prescriptionNo,
    doseCount: 0,
    days: 0,
    status,
    statusName: statusNames[status],
    lines: []
  }
}

export async function realWithdrawTcmPrescription(
  data: TcmPrescriptionActionDTO
): Promise<TcmPrescriptionVO> {
  const response = await useHttp().post<BackendRevokeResponseDTO>(
    '/c2-opdoctor/chinapres3/revoke',
    { medicationRequestIds: requestIds(data) },
    { baseURL: apiBaseURL(data.scope) }
  )
  const result = unwrapPayload(response)
  return {
    id: data.id,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: data.patientId || '',
    prescriptionGroupKey: data.prescriptionGroupKey,
    prescriptionNo: result.revokedPrescriptionNos?.[0] || data.prescriptionNo,
    doseCount: 0,
    days: 0,
    status: 'DRAFT',
    statusName: statusNames.DRAFT,
    lines: []
  }
}

export async function realStopTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  void data
  throw new Error('后端暂未提供中医处方停用接口')
}

export async function realCancelStopTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  void data
  throw new Error('后端暂未提供中医处方取消停用接口')
}
