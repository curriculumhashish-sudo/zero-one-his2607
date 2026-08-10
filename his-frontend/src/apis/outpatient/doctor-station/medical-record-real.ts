import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordHistoryQuery,
  DoctorMedicalRecordVO,
  SaveDoctorMedicalRecordDTO
} from './type'

const realApiBaseURL = import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_URL || undefined

export const isDoctorMedicalRecordRealApiEnabled =
  import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_MODE === 'real'

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
  data?: T[]
}

interface BackendRecord {
  id?: string
  encounterId?: string
  content?: string
  updatedAt?: string
}

interface BackendHistoryItem {
  id?: string
  encounterId?: string
  visitDate?: string
  patientName?: string
  diagnosis?: string
  doctorName?: string
  deptName?: string
}

function text(value: unknown): string | undefined {
  return typeof value === 'string' && value.trim() ? value : undefined
}

function unwrap<T>(response: BackendJsonVO<T> | T, fallback: string): T {
  const body = response as BackendJsonVO<T>
  if (typeof body.code === 'number' && body.code !== 10000) {
    throw new Error(body.message || fallback)
  }
  if (body.data !== undefined) {
    if (body.data === null) throw new Error(body.message || fallback)
    return body.data
  }
  return response as T
}

function pageResult<T>(page: BackendPage<unknown>, rows: T[]): PageDTO<T> {
  return {
    pageIndex: page.pageIndex ?? 1,
    pageSize: page.pageSize ?? (rows.length || 10),
    total: page.total ?? rows.length,
    pages: page.pages,
    rows
  }
}

function mapRecord(
  row: BackendRecord,
  encounterId: string,
  fallbackContent = ''
): DoctorMedicalRecordVO {
  return {
    id: text(row.id),
    encounterId: text(row.encounterId) || encounterId,
    content: row.content ?? fallbackContent,
    updatedAt: text(row.updatedAt)
  }
}

function mapHistoryItem(row: BackendHistoryItem): DoctorMedicalRecordHistoryItemVO {
  return {
    encounterId: text(row.encounterId) || text(row.id) || '',
    visitDate: text(row.visitDate) || '',
    diagnosis: text(row.diagnosis),
    doctorName: text(row.doctorName),
    deptName: text(row.deptName)
  }
}

export async function realGetMedicalRecord(encounterId: string): Promise<DoctorMedicalRecordVO> {
  const response = await useHttp().get<BackendRecord>(
    `/outpatientrecord2/detail/${encodeURIComponent(encounterId)}`,
    undefined,
    { baseURL: realApiBaseURL }
  )
  return mapRecord(unwrap(response, '门诊病历加载失败'), encounterId)
}

export async function realSaveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  const response = await useHttp().post<BackendRecord>(
    '/outpatientrecord3/save',
    { encounterId, content: data.content },
    { baseURL: realApiBaseURL }
  )
  const result = unwrap(response, '门诊病历保存失败')
  return mapRecord(result, encounterId, data.content)
}

export async function realPagePatientMedicalRecordHistory(
  query: DoctorMedicalRecordHistoryQuery
): Promise<PageDTO<DoctorMedicalRecordHistoryItemVO>> {
  const response = await useHttp().get<BackendPage<BackendHistoryItem>>(
    '/outpatientrecord2',
    {
      patientId: query.patientId,
      keyword: query.keyword,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrap(response, '历史病历加载失败')
  const rows = (data.rows ?? data.data ?? []).map(mapHistoryItem)
  return pageResult(data, rows)
}
