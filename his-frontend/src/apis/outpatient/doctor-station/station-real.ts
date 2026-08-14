import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorPatientContextVO
} from './type'
import type {
  CheckInpatientAdmissionVO,
  InpatientAdmissionFormOptionsVO,
  InpatientAdmissionWardVO,
  RequestInpatientAdmissionDTO,
  RequestInpatientAdmissionVO
} from './inpatient-admission/type'
import { outpatientDoctorStationApiBaseURL } from './mode'

const realApiBaseURL = outpatientDoctorStationApiBaseURL

interface JsonVO<T> {
  code?: number
  message?: string
  data?: T
}

interface BackendPage<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
}

function unwrap<T>(response: JsonVO<T> | T, fallback = 'Request failed'): T {
  if (response && typeof response === 'object' && 'data' in response) {
    const body = response as JsonVO<T>
    if (typeof body.code === 'number' && body.code !== 10000) {
      throw new Error(body.message || fallback)
    }
    if (body.data === undefined || body.data === null) {
      throw new Error(body.message || fallback)
    }
    return body.data as T
  }
  return response as T
}

interface BackendPatientContext {
  patientId?: string
  encounterId?: string
  patientName?: string
  visitStatus?: string
  gender?: string
  age?: number | string
  registerType?: string
  visitNo?: string
  payType?: string
  visitTime?: string
  registerDept?: string
  phoneNum?: string
  attendingDoctorName?: string
}

interface BackendTransitionResult {
  encounterId?: string
  visitStatus?: string
  acceptedAt?: string
  completedAt?: string
  attendingDoctorName?: string
}

function normalizeStatus(value: unknown): DoctorPatientContextVO['visitStatus'] {
  const status = String(value ?? '').trim().toLowerCase()
  if (status === 'in-progress' || status.includes('\u5728\u8bca')) return 'in-progress'
  if (status === 'finished' || status.includes('\u5b8c\u8bca')) return 'finished'
  return 'arrived'
}

function mapContext(row: BackendPatientContext, fallbackEncounterId = ''): DoctorPatientContextVO {
  return {
    encounterId: row.encounterId || fallbackEncounterId,
    patientId: row.patientId || '',
    visitStatus: normalizeStatus(row.visitStatus),
    visitStatusName:
      normalizeStatus(row.visitStatus) === 'in-progress'
        ? '\u5728\u8bca'
        : normalizeStatus(row.visitStatus) === 'finished'
          ? '\u5b8c\u8bca'
          : '\u5f85\u8bca',
    attendingDoctorName: row.attendingDoctorName,
    acceptedAt: row.visitTime,
    phone: row.phoneNum || ''
  }
}

export async function realAcceptPatient(encounterId: string): Promise<DoctorPatientContextVO> {
  const response = await useHttp().post<BackendPatientContext>(
    '/c2-opdoctor/accept',
    { encounterId },
    { baseURL: realApiBaseURL }
  )
  return mapContext(unwrap(response, 'Patient acceptance failed'), encounterId)
}

async function realPatientTransition(
  path: '/c2-opdoctor/leave' | '/c2-opdoctor/finish',
  encounterId: string,
  fallback: string
): Promise<DoctorPatientContextVO> {
  const response = await useHttp().put<BackendTransitionResult>(
    path,
    { encounterId },
    { baseURL: realApiBaseURL }
  )
  const result = unwrap(response, fallback)
  return {
    encounterId: result.encounterId || encounterId,
    patientId: '',
    visitStatus: normalizeStatus(result.visitStatus),
    visitStatusName:
      normalizeStatus(result.visitStatus) === 'finished' ? '\u5b8c\u8bca' : '\u5f85\u8bca',
    attendingDoctorName: result.attendingDoctorName,
    acceptedAt: result.acceptedAt,
    phone: ''
  }
}

export function realLeavePatient(encounterId: string) {
  return realPatientTransition('/c2-opdoctor/leave', encounterId, 'Patient leave failed')
}

export function realCompletePatient(encounterId: string) {
  return realPatientTransition('/c2-opdoctor/finish', encounterId, 'Patient completion failed')
}

function pageResult<T>(page: BackendPage<T>, query: { pageIndex: number; pageSize: number }): PageDTO<T> {
  const rows = page.rows || []
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? rows.length,
    pages: page.pages ?? Math.ceil((page.total ?? rows.length) / Math.max(page.pageSize ?? query.pageSize, 1)),
    rows
  }
}

export async function realPageDiagnosisDictionary(
  query: DoctorDiagnosisDictionaryQuery
): Promise<PageDTO<DoctorDiagnosisDictionaryItemVO>> {
  const response = await useHttp().get<BackendPage<DoctorDiagnosisDictionaryItemVO>>(
    '/c2-opdoctor/page-diagnosis-dictionary',
    {
      ...query,
      keyword: query.keyword?.trim() ?? ''
    },
    { baseURL: realApiBaseURL }
  )
  const page = unwrap(response)
  const rows = page.rows ?? []
  const total = page.total ?? rows.length
  const start = Math.max(query.pageIndex - 1, 0) * query.pageSize
  const pagedRows = rows.length > query.pageSize ? rows.slice(start, start + query.pageSize) : rows
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total,
    pages: page.pages ?? Math.ceil(total / Math.max(query.pageSize, 1)),
    rows: pagedRows
  }
}

export async function realCheckInpatientAdmission(encounterId: string): Promise<CheckInpatientAdmissionVO> {
  const response = await useHttp().post<CheckInpatientAdmissionVO>(
    '/c2-opdoctor/inpatient-admission/check',
    { encounterId },
    { baseURL: realApiBaseURL }
  )
  return unwrap(response)
}

export async function realLoadInpatientAdmissionOptions(): Promise<InpatientAdmissionFormOptionsVO> {
  const response = await useHttp().post<InpatientAdmissionFormOptionsVO>(
    '/doctor-station/inpatient-admission/options',
    undefined,
    { baseURL: realApiBaseURL }
  )
  return unwrap(response)
}

export async function realListInpatientAdmissionWards(
  departmentId: string
): Promise<InpatientAdmissionWardVO[]> {
  const response = await useHttp().post<InpatientAdmissionWardVO[]>(
    '/c2-opdoctor/inpatient-admission/wards',
    { departmentId },
    { baseURL: realApiBaseURL }
  )
  const data = unwrap(response)
  return Array.isArray(data) ? data : ((data as unknown as { rows?: InpatientAdmissionWardVO[] }).rows || [])
}

export async function realRequestInpatientAdmission(
  data: RequestInpatientAdmissionDTO
): Promise<RequestInpatientAdmissionVO> {
  const response = await useHttp().post<RequestInpatientAdmissionVO>(
    '/c2-opdoctor/admission_apply',
    data,
    { baseURL: realApiBaseURL }
  )
  return unwrap(response)
}
