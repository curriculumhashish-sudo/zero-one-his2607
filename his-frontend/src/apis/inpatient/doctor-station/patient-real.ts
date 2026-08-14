import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import {
  mapDischargeOrderDetail,
  mapInpatientPatientQueuePage,
  type DischargeOrderDetailDTO,
  type InpatientPatientQueuePageDTO
} from './adapter'
import type {
  CreateInpatientDischargeAdviceDTO,
  InpatientDischargeAdviceVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientDoctorPatientQueueQuery,
  InpatientMedicalRecordDocumentTypeCode,
  InpatientMedicalRecordHistoryDetailVO,
  InpatientMedicalRecordHistoryItemVO,
  InpatientMedicalRecordHistoryQuery,
  InpatientMedicalRecordVO,
  SaveInpatientMedicalRecordDTO
} from './type'

/** 住院医生站真实接口代理前缀；生产环境可替换为网关前缀。 */
const realApiBaseURL =
  import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'

interface BackendMedicalRecordBriefDTO {
  id?: string
  document_type_code?: string
  document_type_name?: string
  title?: string
  status?: string
}

interface BackendMedicalRecordPageDTO {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendMedicalRecordBriefDTO[]
}

interface BackendMedicalRecordDetailDTO {
  id?: string
  content?: string
}

interface BackendMedicalRecordAddDTO {
  patient_id: string
  encounter_id: string
  document_type_code: string
  title: string
  content: string
}

const medicalRecordMetadataCache = new Map<string, InpatientMedicalRecordHistoryItemVO>()

const documentTypeNames: Record<InpatientMedicalRecordDocumentTypeCode, string> = {
  ADMISSION_RECORD: '入院记录',
  FIRST_COURSE_RECORD: '首次病程记录',
  DAILY_COURSE_RECORD: '日常病程记录',
  DISCHARGE_RECORD: '出院记录',
  PROGRESS_NOTE: '病程记录'
}

const statusNames: Record<string, string> = {
  DRAFT: '草稿',
  SIGNED: '已签名',
  ARCHIVED: '已归档'
}

function normalizeDocumentType(value?: string): InpatientMedicalRecordDocumentTypeCode {
  const text = value?.trim() ?? ''
  if (
    text === 'ADMISSION_RECORD' ||
    text === 'FIRST_COURSE_RECORD' ||
    text === 'DAILY_COURSE_RECORD' ||
    text === 'DISCHARGE_RECORD' ||
    text === 'PROGRESS_NOTE'
  ) {
    return text
  }
  if (text.includes('入院')) return 'ADMISSION_RECORD'
  if (text.includes('首次')) return 'FIRST_COURSE_RECORD'
  if (text.includes('日常')) return 'DAILY_COURSE_RECORD'
  if (text.includes('出院')) return 'DISCHARGE_RECORD'
  if (text.includes('病程')) return 'PROGRESS_NOTE'
  return 'ADMISSION_RECORD'
}

function normalizeRecordStatus(value?: string) {
  const status = value?.trim().toUpperCase()
  if (status === 'SIGNED' || status === 'ARCHIVED') return status
  return 'DRAFT' as const
}

function mapMedicalRecordBrief(
  row: BackendMedicalRecordBriefDTO,
  _query: InpatientMedicalRecordHistoryQuery
): InpatientMedicalRecordHistoryItemVO {
  const id = row.id ?? ''
  const documentTypeCode = normalizeDocumentType(row.document_type_code || row.document_type_name)
  const status = normalizeRecordStatus(row.status)
  const item: InpatientMedicalRecordHistoryItemVO = {
    id,
    patientId: _query.patientId,
    encounterId: _query.currentEncounterId ?? '',
    inpatientNo: '',
    documentTypeCode,
    documentTypeName: row.document_type_name || documentTypeNames[documentTypeCode],
    title: row.title ?? '',
    status,
    statusName: statusNames[status] ?? status,
    departmentName: '',
    authoredByName: '',
    authoredAt: '',
    updatedAt: ''
  }
  if (id) medicalRecordMetadataCache.set(id, item)
  return item
}

function mapMedicalRecordDetail(
  detail: BackendMedicalRecordDetailDTO,
  recordId: string,
  readonlyOverride = true
): InpatientMedicalRecordVO {
  const metadata = medicalRecordMetadataCache.get(recordId)
  const status = metadata?.status ?? 'DRAFT'
  return {
    ...(metadata ?? {
      id: recordId,
      patientId: '',
      encounterId: '',
      inpatientNo: '',
      documentTypeCode: 'ADMISSION_RECORD',
      documentTypeName: documentTypeNames.ADMISSION_RECORD,
      title: '',
      status,
      statusName: statusNames[status] ?? status,
      departmentName: '',
      authoredByName: '',
      authoredAt: '',
      updatedAt: ''
    }),
    id: detail.id ?? recordId,
    content: detail.content ?? '',
    version: 0,
    readonly: readonlyOverride
  }
}

function buildSavedMedicalRecord(
  data: SaveInpatientMedicalRecordDTO,
  recordId: string
): InpatientMedicalRecordVO {
  const status = 'DRAFT' as const
  const item: InpatientMedicalRecordVO = {
    id: recordId,
    patientId: data.patientId,
    encounterId: data.encounterId,
    inpatientNo: '',
    documentTypeCode: data.documentTypeCode,
    documentTypeName: documentTypeNames[data.documentTypeCode],
    title: data.title,
    status,
    statusName: statusNames[status] ?? status,
    departmentName: '',
    authoredByName: '',
    authoredAt: '',
    updatedAt: '',
    content: data.content,
    version: 0,
    readonly: true
  }
  medicalRecordMetadataCache.set(recordId, item)
  return item
}

function unwrapData<T>(response: { data?: T; message?: string }): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '后端未返回有效数据')
  }
  return response.data
}

function toInpatientDoctorStationError(error: unknown, fallback: string): Error {
  if (error && typeof error === 'object') {
    const errorObject = error as {
      message?: unknown
      data?: { message?: unknown }
      response?: { data?: { message?: unknown } }
    }
    const candidates = [
      errorObject.message,
      errorObject.data?.message,
      errorObject.response?.data?.message
    ]
    const message = candidates.find(
      (candidate): candidate is string => typeof candidate === 'string' && Boolean(candidate.trim())
    )
    if (message) return new Error(message)
  }
  return new Error(fallback)
}

/** 查询真实住院患者队列。 */
export async function realPageInpatientDoctorPatientQueue(
  query: InpatientDoctorPatientQueueQuery
): Promise<PageDTO<InpatientDoctorPatientQueueItemVO>> {
  try {
    const response = await useHttp().get<InpatientPatientQueuePageDTO>(
      '/inpatientDoctorStation/cycle-control/patient-queue',
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        ...(query.keyword ? { keyword: query.keyword } : {})
      },
      { baseURL: realApiBaseURL }
    )
    return mapInpatientPatientQueuePage(unwrapData(response))
  } catch (error) {
    throw toInpatientDoctorStationError(error, '住院患者队列加载失败')
  }
}

/** 创建真实出院医嘱。 */
export async function realCreateInpatientDischargeAdvice(
  data: CreateInpatientDischargeAdviceDTO
): Promise<InpatientDischargeAdviceVO> {
  try {
    const response = await useHttp().post<DischargeOrderDetailDTO>(
      '/inpatientDoctorStation/cycle-control/discharge-order',
      {
        encounterId: data.encounterId,
        dischargeWay: data.outWayCode,
        dischargeTime: data.plannedDischargeTime
      },
      { baseURL: realApiBaseURL }
    )
    return mapDischargeOrderDetail(unwrapData(response), data)
  } catch (error) {
    throw toInpatientDoctorStationError(error, '开立出院医嘱失败')
  }
}

/** Query the patient's medical record history. */
export async function realPageInpatientMedicalRecordHistory(
  query: InpatientMedicalRecordHistoryQuery
): Promise<PageDTO<InpatientMedicalRecordHistoryItemVO>> {
  try {
    const response = await useHttp().get<BackendMedicalRecordPageDTO>(
      '/inpatient-record/brief',
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        patient_id: query.patientId,
        ...(query.keyword ? { title: query.keyword } : {})
      },
      { baseURL: realApiBaseURL }
    )
    const page = unwrapData(response)
    const rows = (page.rows ?? []).map((row) => mapMedicalRecordBrief(row, query))
    return {
      pageIndex: page.pageIndex ?? query.pageIndex,
      pageSize: page.pageSize ?? query.pageSize,
      total: page.total ?? rows.length,
      pages: page.pages ?? Math.ceil((page.total ?? rows.length) / Math.max(query.pageSize, 1)),
      rows
    }
  } catch (error) {
    throw toInpatientDoctorStationError(error, '历史病历列表加载失败')
  }
}

/** 获取真实病历详情；历史详情固定只读。 */
export async function realGetInpatientMedicalRecordHistoryDetail(
  recordId: string
): Promise<InpatientMedicalRecordHistoryDetailVO> {
  const record = await realGetInpatientMedicalRecord(recordId, true)
  return { ...record, readonly: true as const }
}

/** 获取真实病历详情。 */
export async function realGetInpatientMedicalRecord(
  recordId: string,
  readonlyOverride?: boolean
): Promise<InpatientMedicalRecordVO> {
  try {
    const response = await useHttp().get<BackendMedicalRecordDetailDTO>(
      '/inpatient-record/detail',
      { id: recordId },
      { baseURL: realApiBaseURL }
    )
    return mapMedicalRecordDetail(unwrapData(response), recordId, readonlyOverride)
  } catch (error) {
    throw toInpatientDoctorStationError(error, '住院病历加载失败')
  }
}

/** 保存真实病历草稿；后端保存接口当前没有独立签名语义。 */
export async function realSaveInpatientMedicalRecord(
  data: SaveInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  try {
    const payload: BackendMedicalRecordAddDTO = {
      patient_id: data.patientId,
      encounter_id: data.encounterId,
      document_type_code: data.documentTypeCode,
      title: data.title,
      content: data.content
    }
    const response = await useHttp().post<string>('/inpatient-record/add', payload, {
      baseURL: realApiBaseURL
    })
    const recordId = response.data
    if (!recordId) throw new Error(response.message || '保存病历后未返回病历 ID')
    return buildSavedMedicalRecord(data, recordId)
  } catch (error) {
    throw toInpatientDoctorStationError(error, '住院病历保存失败')
  }
}
