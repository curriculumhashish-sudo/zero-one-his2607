import { useHttp } from '@/plugins/http'

import { realApiBaseURL } from './patient-real'
import type {
  NurseStationDeleteNursingRecordDTO,
  NurseStationDeleteNursingRecordResultVO,
  NurseStationNursingRecordListVO,
  NurseStationNursingRecordPaperHeaderVO,
  NurseStationNursingRecordQueryDTO,
  NurseStationNursingRecordRowVO,
  NurseStationNursingRecordSaveItemDTO,
  NurseStationSaveNursingRecordsDTO,
  NurseStationSaveNursingRecordsResultVO
} from './nursing-record-type'

interface BackendNursingPaperHeader {
  inpatientNo?: string
  name?: string
  gender?: string
  age?: string | number
  admitAt?: string
  dischargeAt?: string
  wardName?: string
  bedNo?: string
}

interface BackendNursingListResponse {
  paperHeader?: BackendNursingPaperHeader
  rows?: Array<Partial<NurseStationNursingRecordRowVO>>
}

interface BackendNursingSaveResponse {
  rows?: Array<Partial<NurseStationNursingRecordRowVO>>
  savedCount?: number
}

interface BackendNursingDeleteResponse {
  success?: boolean
  vitalSignId?: string
  statusEnum?: string
}

function getRealHttp() {
  return useHttp()
}

function readString(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function mapHeader(
  header: BackendNursingPaperHeader | undefined,
  fallback?: NurseStationNursingRecordPaperHeaderVO
) {
  const normalized = {
    patientName: readString(header?.name) || fallback?.patientName || '未知患者',
    genderText: readString(header?.gender) || fallback?.genderText,
    ageText: header?.age === undefined ? fallback?.ageText : String(header.age),
    wardName: readString(header?.wardName) || fallback?.wardName,
    bedName: readString(header?.bedNo) || fallback?.bedName,
    encounterBusNo: readString(header?.inpatientNo) || fallback?.encounterBusNo,
    admissionDate: readString(header?.admitAt)?.slice(0, 10) || fallback?.admissionDate,
    dischargeDate: readString(header?.dischargeAt)?.slice(0, 10) || fallback?.dischargeDate
  }
  return normalized
}

function mapRow(row: Partial<NurseStationNursingRecordRowVO>): NurseStationNursingRecordRowVO {
  return {
    vitalSignId: readString(row.vitalSignId) || '',
    recordDate: readString(row.recordDate) || '',
    recordTime: readString(row.recordTime) || '',
    temperature: readString(row.temperature),
    pulse: readString(row.pulse),
    heartRate: readString(row.heartRate),
    respiration: readString(row.respiration),
    bloodPressure: readString(row.bloodPressure),
    consciousness: readString(row.consciousness),
    pupilLeft: readString(row.pupilLeft),
    pupilRight: readString(row.pupilRight),
    observation: readString(row.observation),
    intakeName: readString(row.intakeName),
    intakeVolume: readString(row.intakeVolume),
    outputName: readString(row.outputName),
    outputVolume: readString(row.outputVolume),
    outputColor: readString(row.outputColor),
    nurseSignature: readString(row.nurseSignature),
    version: typeof row.version === 'number' ? row.version : 0,
    createdAt: readString(row.createdAt),
    updatedAt: readString(row.updatedAt)
  }
}

/** 查询后端已有的护理记录字段，并用页面上下文补齐旧接口缺失的纸质单抬头。 */
export async function realQueryNurseStationNursingRecords(
  query: NurseStationNursingRecordQueryDTO
): Promise<NurseStationNursingRecordListVO> {
  const response = await getRealHttp().get<BackendNursingListResponse>(
    '/nursing/list-records',
    {
      encounterId: query.encounterId,
      patientId: query.patientId,
      recordedAtFrom: query.recordedAtFrom,
      recordedAtTo: query.recordedAtTo,
      sortAsc: query.sortAsc
    },
    { baseURL: realApiBaseURL }
  )
  return {
    paperHeader: mapHeader(response.data?.paperHeader, query.patientContext),
    rows: (response.data?.rows ?? []).map(mapRow)
  }
}

function toBackendSaveItem(item: NurseStationNursingRecordSaveItemDTO) {
  return {
    clientRowId: item.clientRowId,
    vitalSignId: item.vitalSignId,
    expectedVersion: item.expectedVersion,
    recordDate: item.recordDate,
    recordTime: item.recordTime,
    temperature: item.temperature,
    pulse: item.pulse,
    heartRate: item.heartRate,
    respiration: item.respiration,
    bloodPressure: item.bloodPressure,
    consciousness: item.consciousness,
    pupilLeft: item.pupilLeft,
    pupilRight: item.pupilRight,
    observation: item.observation,
    intakeName: item.intakeName,
    intakeVolume: item.intakeVolume,
    outputName: item.outputName,
    outputVolume: item.outputVolume,
    outputColor: item.outputColor,
    nurseSignature: item.nurseSignature
  }
}

function matchSavedRow(
  rows: Array<Partial<NurseStationNursingRecordRowVO>>,
  item: NurseStationNursingRecordSaveItemDTO
) {
  return rows.find(
    (row) =>
      row.vitalSignId === item.vitalSignId ||
      (row.recordDate === item.recordDate && row.recordTime === item.recordTime)
  )
}

/** 把旧接口的整体保存结果转换为页面需要的逐条结果。 */
export async function realSaveNurseStationNursingRecords(
  data: NurseStationSaveNursingRecordsDTO
): Promise<NurseStationSaveNursingRecordsResultVO> {
  const response = await getRealHttp().post<BackendNursingSaveResponse>(
    '/nursing/save-records',
    {
      idempotencyKey: data.idempotencyKey,
      encounterId: data.encounterId,
      patientId: data.patientId,
      records: data.records.map(toBackendSaveItem)
    },
    { baseURL: realApiBaseURL }
  )
  const backendRows = response.data?.rows ?? []
  const savedCount = response.data?.savedCount ?? data.records.length
  const allItemsSaved = savedCount >= data.records.length
  const results = data.records.map((item) => {
    const row = matchSavedRow(backendRows, item)
    const success = allItemsSaved || Boolean(row)
    const fallbackRow = {
      ...toBackendSaveItem(item),
      vitalSignId: item.vitalSignId || '',
      version: (item.expectedVersion ?? 0) + 1
    }
    return {
      clientRowId: item.clientRowId,
      success,
      message: success ? '保存成功' : '后端未返回该条护理记录的保存结果',
      row: success ? mapRow(row ?? fallbackRow) : undefined
    }
  })
  return {
    success: savedCount === data.records.length,
    savedCount,
    results
  }
}

export async function realDeleteNurseStationNursingRecord(
  data: NurseStationDeleteNursingRecordDTO
): Promise<NurseStationDeleteNursingRecordResultVO> {
  const response = await getRealHttp().post<BackendNursingDeleteResponse>(
    '/nursing/delete-record',
    data,
    { baseURL: realApiBaseURL }
  )
  const result = response.data ?? {}
  const success = result.success === true && result.statusEnum === 'DELETED'
  return {
    success,
    vitalSignId: result.vitalSignId || data.vitalSignId,
    message: success ? '护理记录已删除' : '护理记录删除失败'
  }
}
