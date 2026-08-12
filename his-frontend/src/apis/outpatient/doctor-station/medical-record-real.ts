import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordHistoryQuery,
  DoctorMedicalRecordVO,
  SaveDoctorMedicalRecordDTO
} from './type'
import { isOutpatientDoctorStationRealApiEnabled, outpatientDoctorStationApiBaseURL } from './mode'

const realApiBaseURL = outpatientDoctorStationApiBaseURL

/** Backward-compatible alias; the mode itself is owned by ./mode. */
export const isDoctorMedicalRecordRealApiEnabled = isOutpatientDoctorStationRealApiEnabled

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
  if (typeof body.code === 'number' && body.code !== 0 && body.code !== 10000) {
    throw new Error(body.message || fallback)
  }
  if (body.data !== undefined) {
    if (body.data === null) throw new Error(body.message || fallback)
    return body.data
  }
  return response as T
}

export function isMedicalRecordNotFoundError(error: unknown): boolean {
  const message = error instanceof Error ? error.message.toLowerCase() : String(error).toLowerCase()
  return (
    message.includes('medical record content not found') || message.includes('record not found')
  )
}

export const MEDICAL_RECORD_CONTENT_FORMAT_ERROR =
  '当前后端仅支持 JSON 病历格式，WangEditor HTML 内容暂不可保存'

function isSupportedMedicalRecordContent(content: string): boolean {
  if (!content.trim()) return false

  try {
    const parsed: unknown = JSON.parse(content)
    return Boolean(parsed && typeof parsed === 'object')
  } catch {
    return false
  }
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
  try {
    const response = await useHttp().get<BackendRecord>(
      `/outpatientrecord2/detail/${encodeURIComponent(encounterId)}`,
      undefined,
      { baseURL: realApiBaseURL }
    )
    return mapRecord(unwrap(response, '门诊病历加载失败'), encounterId)
  } catch (error) {
    if (isMedicalRecordNotFoundError(error)) {
      const notFoundError = new Error('Medical record content not found')
      notFoundError.name = 'MedicalRecordNotFoundError'
      throw notFoundError
    }
    throw error
  }
}

export async function realSaveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  if (!isSupportedMedicalRecordContent(data.content)) {
    throw new Error(MEDICAL_RECORD_CONTENT_FORMAT_ERROR)
  }

  const response = await useHttp().post<BackendRecord>(
    '/c2-opdoctor/outpatientrecord3/save',
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
  const allRows = (data.rows ?? data.data ?? []).map(mapHistoryItem)
  const pageSize = data.pageSize ?? query.pageSize
  const pageIndex = data.pageIndex ?? query.pageIndex
  const rows =
    allRows.length > pageSize
      ? allRows.slice((pageIndex - 1) * pageSize, pageIndex * pageSize)
      : allRows
  return pageResult({ ...data, pageIndex, pageSize }, rows)
}
