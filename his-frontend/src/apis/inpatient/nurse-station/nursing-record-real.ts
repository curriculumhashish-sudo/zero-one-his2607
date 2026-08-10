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

interface BackendNursingListResponse {
  paperHeader?: NurseStationNursingRecordPaperHeaderVO
  rows?: Array<Partial<NurseStationNursingRecordRowVO>>
}

interface BackendNursingSaveResponse {
  rows?: Array<Partial<NurseStationNursingRecordRowVO>>
  savedCount?: number
}

function getRealHttp() {
  return useHttp()
}

function readString(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function mapHeader(
  header: NurseStationNursingRecordPaperHeaderVO | undefined,
  fallback?: NurseStationNursingRecordPaperHeaderVO
) {
  const merged = {
    ...(fallback ?? {}),
    ...(header ?? {})
  }
  return { ...merged, patientName: merged.patientName || '未知患者' }
}

function mapRow(row: Partial<NurseStationNursingRecordRowVO>): NurseStationNursingRecordRowVO {
  return {
    vitalSignId: readString(row.vitalSignId) || '',
    recordDate: readString(row.recordDate) || '',
    recordTime: readString(row.recordTime) || '',
    temperature: readString(row.temperature),
    pulse: readString(row.pulse),
    heartRate: readString(row.heartRate),
    respiratory: readString(row.respiratory),
    bloodPressure: readString(row.bloodPressure),
    oxygenSaturation: readString(row.oxygenSaturation),
    oxygenFlow: readString(row.oxygenFlow),
    oxygenMode: readString(row.oxygenMode),
    oxygenRate: readString(row.oxygenRate),
    consciousness: readString(row.consciousness),
    pupilLeft: readString(row.pupilLeft),
    pupilRight: readString(row.pupilRight),
    skinStatus: readString(row.skinStatus),
    tubeCare: readString(row.tubeCare),
    conditionMeasure: readString(row.conditionMeasure),
    intakeName: readString(row.intakeName),
    intakeAmount: readString(row.intakeAmount),
    intakeRoute: readString(row.intakeRoute),
    outputName: readString(row.outputName),
    outputAmount: readString(row.outputAmount),
    outputColorCharacter: readString(row.outputColorCharacter),
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
  // 当前 Swagger 不接收幂等键、版本、clientRowId 和护士签名，避免发送未知字段导致 400。
  return {
    vitalSignId: item.vitalSignId,
    recordDate: item.recordDate,
    recordTime: item.recordTime,
    temperature: item.temperature,
    pulse: item.pulse,
    heartRate: item.heartRate,
    respiratory: item.respiratory,
    bloodPressure: item.bloodPressure,
    oxygenSaturation: item.oxygenSaturation,
    consciousness: item.consciousness,
    pupilLeft: item.pupilLeft,
    pupilRight: item.pupilRight,
    skinStatus: item.skinStatus,
    conditionMeasure: item.conditionMeasure,
    tubeCare: item.tubeCare,
    intakeName: item.intakeName,
    intakeAmount: item.intakeAmount,
    intakeRoute: item.intakeRoute,
    outputName: item.outputName,
    outputAmount: item.outputAmount,
    outputColorCharacter: item.outputColorCharacter,
    oxygenFlow: item.oxygenFlow,
    oxygenMode: item.oxygenMode,
    oxygenRate: item.oxygenRate,
    rowType: item.rowType
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

/** 当前 Swagger 没有删除护理记录接口，真实模式禁止伪造删除请求。 */
export async function realDeleteNurseStationNursingRecord(
  _data: NurseStationDeleteNursingRecordDTO
): Promise<NurseStationDeleteNursingRecordResultVO> {
  throw new Error('当前后端暂未提供护理记录删除接口，已保存记录暂不可删除')
}
