import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import {
  type DoctorDiagnosisListVO,
  type DoctorPatientDiagnosisHistoryItemVO,
  type DoctorPatientDiagnosisHistoryQuery,
  type DoctorPatientQueueItemVO,
  type DoctorPatientQueueQuery,
  type DoctorVisitStatus,
  type OpdoctorCreateDiagnosisDTO
} from './type'
import { outpatientDoctorStationApiBaseURL } from './mode'

const realApiBaseURL = outpatientDoctorStationApiBaseURL

interface BackendJsonVO<T> {
  code?: number
  message?: string
  data?: T | null
}

interface BackendPage<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
}

interface BackendQueueRow {
  patientId?: string
  encounterId?: string
  patientName?: string
  gender?: string
  age?: number | string
  registerType?: string
  visitTime?: string
  visitNo?: string
  payType?: string
  visitStatus?: string
  queueNumber?: number | string
  departmentName?: string
  priorityName?: string
}

interface BackendDiagnosisRow {
  id?: string
  encounterId?: string
  conditionId?: string
  description?: string
  diagTypeCode?: string
  diagCode?: number | string
  rank?: number | string
  primaryFlag?: number | boolean | string
  createdAt?: string
}

interface BackendDiagnosisMutation {
  id?: string
  encounterId?: string
}

const diagnosisEncounterIds = new Map<string, string>()

const diagnosisTypeNameMap: Record<string, string> = {
  '1': '西医诊断',
  '2': '症状诊断',
  '3': '中医诊断',
  WESTERN: '西医诊断',
  SYMPTOM: '症状诊断',
  TCM: '中医诊断',
  '西医诊断': '西医诊断',
  '症状诊断': '症状诊断',
  '中医诊断': '中医诊断'
}

const diagnosisTypeCodeMap: Record<string, string> = {
  '西医诊断': '1',
  '症状诊断': '2',
  '中医诊断': '3',
  WESTERN: '1',
  SYMPTOM: '2',
  TCM: '3',
  '1': '1',
  '2': '2',
  '3': '3'
}

function text(value: unknown): string {
  return typeof value === 'string'
    ? value.trim()
    : value === undefined || value === null
      ? ''
      : String(value)
}

function likeKeyword(value: string | undefined): string | undefined {
  const normalized = value?.trim()
  return normalized ? `%${normalized}%` : undefined
}

function number(value: unknown, fallback = 0): number {
  const result = typeof value === 'number' ? value : Number(value)
  return Number.isFinite(result) ? result : fallback
}

function flag(value: unknown): boolean {
  if (value === true || value === 1) return true
  return ['1', 'true', 'yes', 'primary'].includes(text(value).toLowerCase())
}

function unwrap<T>(response: BackendJsonVO<T> | T, fallback: string): T {
  const body = response as BackendJsonVO<T>
  if (typeof body.code === 'number' && body.code !== 10000) {
    throw new Error(body.message || fallback)
  }
  if (body.data === null) throw new Error(body.message || fallback)
  return body.data === undefined ? (response as T) : body.data
}

function toVisitStatus(value: unknown): DoctorVisitStatus {
  const normalized = text(value).toLowerCase()
  if (normalized === 'in-progress' || normalized.includes('在诊')) return 'in-progress'
  if (normalized === 'finished' || normalized.includes('完诊')) return 'finished'
  return 'arrived'
}

function toGender(value: unknown): string {
  const normalized = text(value).toLowerCase()
  if (normalized === '1' || normalized === 'm' || normalized === 'male') return '男'
  if (normalized === '2' || normalized === 'f' || normalized === 'female') return '女'
  return text(value)
}

function toPaymentType(value: unknown): string {
  const normalized = text(value).toUpperCase()
  if (normalized === 'SELF_PAY') return '自费'
  if (normalized === 'MEDICAL_INSURANCE') return '医保'
  return text(value)
}

function mapQueueRow(row: BackendQueueRow): DoctorPatientQueueItemVO {
  const encounterId = text(row.encounterId)
  const patientId = text(row.patientId) || encounterId
  return {
    encounterId,
    patientId,
    patientName: text(row.patientName),
    gender: toGender(row.gender),
    age: number(row.age),
    visitNo: text(row.visitNo) || encounterId,
    queueNumber: number(row.queueNumber),
    visitTime: text(row.visitTime),
    departmentName: text(row.departmentName),
    registerType: text(row.registerType),
    visitStatus: toVisitStatus(row.visitStatus),
    paymentTypeName: toPaymentType(row.payType),
    priorityName: text(row.priorityName) || undefined
  }
}

function diagnosisTypeName(value: unknown): string {
  const normalized = text(value)
  return diagnosisTypeNameMap[normalized] ?? normalized
}

export function diagnosisTypeCode(value: string): string {
  const normalized = value.trim()
  const code = diagnosisTypeCodeMap[normalized]
  if (!code || !['1', '2', '3'].includes(code)) {
    throw new Error('无法识别诊断分类，请刷新诊断字典后重试')
  }
  return code
}

function diagnosisCodeNumber(value: string): number {
  const normalized = value.trim()
  if (!/^\d+$/.test(normalized)) {
    throw new Error('后端暂不支持字符串诊断编码，请联系管理员处理 ICD 编码配置')
  }
  const result = Number(normalized)
  if (!Number.isSafeInteger(result)) {
    throw new Error('诊断编码超出后端支持范围')
  }
  return result
}

function mapDiagnosisRow(
  row: BackendDiagnosisRow,
  index: number
): DoctorDiagnosisListVO['rows'][number] {
  const id = text(row.id)
  const encounterId = text(row.encounterId)
  if (id && encounterId) diagnosisEncounterIds.set(id, encounterId)
  return {
    id,
    encounterId,
    catalogId: text(row.conditionId),
    code: text(row.diagCode),
    name: text(row.description),
    typeName: diagnosisTypeName(row.diagTypeCode),
    primary: flag(row.primaryFlag),
    diagnosedAt: text(row.createdAt),
    orderNo: number(row.rank, index + 1)
  }
}

function pageResult<T>(
  page: BackendPage<unknown>,
  rows: T[],
  query: { pageIndex: number; pageSize: number }
): PageDTO<T> {
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? rows.length,
    pages:
      page.pages ??
      Math.ceil((page.total ?? rows.length) / Math.max(page.pageSize ?? query.pageSize, 1)),
    rows
  }
}

async function loadDiagnosesByPatient(
  encounterId: string,
  patientId: string
): Promise<DoctorDiagnosisListVO> {
  const resolvedPatientId = text(patientId)
  if (!resolvedPatientId) throw new Error('当前患者缺少患者 ID，无法加载诊断')
  const response = await useHttp().get<BackendPage<BackendDiagnosisRow>>(
    '/c2-opdoctor/diagnose/patient',
    {
      patientId: resolvedPatientId,
      pageIndex: 1,
      pageSize: 1000
    },
    { baseURL: realApiBaseURL }
  )
  const page = unwrap(response, 'Patient diagnosis loading failed')
  const rows = (page.rows ?? [])
    .filter((row) => !text(row.encounterId) || text(row.encounterId) === encounterId)
    .map(mapDiagnosisRow)
    .sort((left, right) => left.orderNo - right.orderNo)
  return {
    encounterId,
    rows,
    updatedAt: rows
      .map((row) => row.diagnosedAt)
      .filter(Boolean)
      .sort()
      .at(-1)
  }
}

export async function realListDoctorPatientQueue(
  query: DoctorPatientQueueQuery
): Promise<DoctorPatientQueueItemVO[]> {
  const response = await useHttp().get<BackendQueueRow[] | { rows?: BackendQueueRow[] }>(
    '/c2-opdoctor/queue',
    {
      date: query.visitDate,
      patientName: query.patientName?.trim() || undefined,
      visitStatus: query.visitStatus
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrap(response, 'Patient queue loading failed')
  return (Array.isArray(data) ? data : (data.rows ?? [])).map(mapQueueRow)
}

export async function realGetDiagnoses(
  encounterId: string,
  patientId: string
): Promise<DoctorDiagnosisListVO> {
  return loadDiagnosesByPatient(encounterId, patientId)
}

export async function realPagePatientDiagnosisHistory(
  query: DoctorPatientDiagnosisHistoryQuery
): Promise<PageDTO<DoctorPatientDiagnosisHistoryItemVO>> {
  const response = await useHttp().get<BackendPage<BackendDiagnosisRow>>(
    '/c2-opdoctor/diagnose/patient',
    {
      patientId: query.patientId,
      keyword: likeKeyword(query.keyword),
      startDate: query.startDate,
      endDate: query.endDate,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    },
    { baseURL: realApiBaseURL }
  )
  const page = unwrap(response, 'Patient diagnosis history loading failed')
  const rows = (page.rows ?? []).map(mapDiagnosisRow)
  return pageResult(page, rows, query)
}

export async function realCreateDiagnosis(
  data: OpdoctorCreateDiagnosisDTO,
  patientId: string
): Promise<DoctorDiagnosisListVO> {
  const resolvedPatientId = text(patientId)
  if (!resolvedPatientId) throw new Error('当前患者缺少患者 ID，无法新增诊断')
  const response = await useHttp().post<BackendDiagnosisMutation>(
    '/c2-opdoctor/diagnose/add',
    {
      ...data,
      diagCode: diagnosisCodeNumber(data.diagCode),
      diagTypeCode: diagnosisTypeCode(data.diagTypeCode)
    },
    { baseURL: realApiBaseURL }
  )
  const result = unwrap(response, 'Diagnosis creation failed')
  if (result.id) diagnosisEncounterIds.set(result.id, result.encounterId || data.encounterId)
  return loadDiagnosesByPatient(data.encounterId, resolvedPatientId)
}

export async function realDeleteDiagnosis(id: string, patientId: string): Promise<DoctorDiagnosisListVO> {
  const resolvedPatientId = text(patientId)
  if (!resolvedPatientId) throw new Error('当前患者缺少患者 ID，无法删除诊断')
  const response = await useHttp().delete<BackendDiagnosisMutation>(
    `/c2-opdoctor/diagnose/delete/${encodeURIComponent(id)}`,
    undefined,
    { baseURL: realApiBaseURL }
  )
  const result = unwrap(response, 'Diagnosis deletion failed')
  const encounterId = result.encounterId || diagnosisEncounterIds.get(id)
  if (!encounterId)
    throw new Error('The deleted diagnosis encounter was not returned by the backend')
  return loadDiagnosesByPatient(encounterId, resolvedPatientId)
}
