import dayjs from 'dayjs'

import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationDeleteNursingRecordDTO,
  NurseStationDeleteNursingRecordResultVO,
  NurseStationNursingRecordQueryDTO,
  NurseStationNursingRecordRowVO,
  NurseStationNursingRecordSaveItemDTO,
  NurseStationSaveNursingRecordsDTO,
  NurseStationSaveNursingRecordsResultVO
} from './nursing-record-type'

interface MockNursingRecord extends NurseStationNursingRecordRowVO {
  /** Mock 内部保存记录所属就诊，真实数据库中也必须有同样的归属关系。 */
  encounterId: string
  /** Mock 内部保存患者 ID，用于防止跨患者读取或写入。 */
  patientId: string
}

let recordSeed = 100

const nursingRecords: MockNursingRecord[] = [
  {
    vitalSignId: 'nursing-record-001',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-08-01',
    recordTime: '08:30',
    temperature: '36.5',
    pulse: '80',
    heartRate: '80',
    respiratory: '18',
    bloodPressure: '120/80',
    consciousness: '清醒',
    pupilLeft: '3mm',
    pupilRight: '3mm',
    intakeName: '静脉输液',
    intakeAmount: '500',
    outputName: '尿液',
    outputAmount: '350',
    outputColorCharacter: '淡黄清亮',
    conditionMeasure: '患者精神可，输液通畅，无不良反应。',
    nurseSignature: '周护士',
    version: 1,
    createdAt: '2026-08-01 08:31:00',
    updatedAt: '2026-08-01 08:31:00'
  },
  {
    vitalSignId: 'nursing-record-002',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-08-01',
    recordTime: '14:00',
    temperature: '36.7',
    pulse: '78',
    heartRate: '78',
    respiratory: '17',
    bloodPressure: '118/76',
    consciousness: '清醒',
    intakeName: '饮水',
    intakeAmount: '300',
    outputName: '尿液',
    outputAmount: '280',
    outputColorCharacter: '淡黄',
    conditionMeasure: '协助翻身拍背，指导有效咳嗽。',
    nurseSignature: '周护士',
    version: 2,
    createdAt: '2026-08-01 14:02:00',
    updatedAt: '2026-08-01 14:05:00'
  },
  {
    vitalSignId: 'nursing-record-004',
    encounterId: 'encounter-004',
    patientId: 'patient-004',
    recordDate: '2026-08-01',
    recordTime: '09:10',
    temperature: '37.2',
    pulse: '88',
    heartRate: '88',
    respiratory: '20',
    bloodPressure: '126/82',
    consciousness: '清醒',
    outputName: '引流液',
    outputAmount: '45',
    outputColorCharacter: '淡红',
    conditionMeasure: '切口敷料干燥，引流管固定通畅。',
    nurseSignature: '陈护士',
    version: 1,
    createdAt: '2026-08-01 09:12:00',
    updatedAt: '2026-08-01 09:12:00'
  }
]

const saveIdempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationSaveNursingRecordsResultVO }
>()
const deleteIdempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationDeleteNursingRecordResultVO }
>()

function cloneRow(row: NurseStationNursingRecordRowVO): NurseStationNursingRecordRowVO {
  return { ...row }
}

function cloneSaveResult(
  result: NurseStationSaveNursingRecordsResultVO
): NurseStationSaveNursingRecordsResultVO {
  return {
    ...result,
    results: result.results.map((item) => ({
      ...item,
      row: item.row ? cloneRow(item.row) : undefined
    }))
  }
}

function assertPatientContext(encounterId: string, patientId: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient || patient.patientId !== patientId) throw new Error('患者与住院记录不匹配')
  return patient
}

/** Mock：查询患者指定时间范围内的护理记录与纸质单抬头。 */
export async function mockQueryNurseStationNursingRecords(
  query: NurseStationNursingRecordQueryDTO
) {
  await Promise.resolve()
  const patient = assertPatientContext(query.encounterId, query.patientId)
  const from = query.recordedAtFrom ? dayjs(query.recordedAtFrom) : undefined
  const to = query.recordedAtTo ? dayjs(query.recordedAtTo) : undefined
  if (from && !from.isValid()) throw new Error('护理记录开始时间格式不正确')
  if (to && !to.isValid()) throw new Error('护理记录结束时间格式不正确')

  const rows = nursingRecords
    .filter((row) => {
      if (row.encounterId !== query.encounterId || row.patientId !== query.patientId) return false
      const recordedAt = dayjs(`${row.recordDate} ${row.recordTime}`)
      if (from && recordedAt.isBefore(from)) return false
      if (to && recordedAt.isAfter(to)) return false
      return true
    })
    .sort((left, right) => {
      const compared = `${left.recordDate} ${left.recordTime}`.localeCompare(
        `${right.recordDate} ${right.recordTime}`
      )
      return query.sortAsc === false ? -compared : compared
    })
    .map(cloneRow)

  return {
    paperHeader: {
      patientName: patient.patientName,
      genderText: patient.genderEnumText,
      ageText: patient.ageText,
      wardName: patient.wardName,
      bedName: patient.bedName,
      encounterBusNo: patient.encounterBusNo,
      admissionDate: patient.admissionTime?.slice(0, 10),
      dischargeDate: patient.endTime?.slice(0, 10)
    },
    rows
  }
}

const contentFields: Array<keyof NurseStationNursingRecordSaveItemDTO> = [
  'temperature',
  'pulse',
  'heartRate',
  'respiratory',
  'bloodPressure',
  'oxygenSaturation',
  'oxygenFlow',
  'oxygenMode',
  'oxygenRate',
  'consciousness',
  'pupilLeft',
  'pupilRight',
  'skinStatus',
  'tubeCare',
  'conditionMeasure',
  'intakeName',
  'intakeAmount',
  'intakeRoute',
  'outputName',
  'outputAmount',
  'outputColorCharacter',
  'nurseSignature'
]

function hasBusinessContent(item: NurseStationNursingRecordSaveItemDTO) {
  return contentFields.some((key) => String(item[key] ?? '').trim().length > 0)
}

function buildStoredRow(
  item: NurseStationNursingRecordSaveItemDTO,
  current?: MockNursingRecord
): NurseStationNursingRecordRowVO {
  const now = dayjs().format('YYYY-MM-DD HH:mm:ss')
  return {
    vitalSignId: current?.vitalSignId ?? `nursing-record-${++recordSeed}`,
    recordDate: item.recordDate,
    recordTime: item.recordTime,
    temperature: item.temperature,
    pulse: item.pulse,
    heartRate: item.heartRate,
    respiratory: item.respiratory,
    bloodPressure: item.bloodPressure,
    oxygenSaturation: item.oxygenSaturation,
    oxygenFlow: item.oxygenFlow,
    oxygenMode: item.oxygenMode,
    oxygenRate: item.oxygenRate,
    consciousness: item.consciousness,
    pupilLeft: item.pupilLeft,
    pupilRight: item.pupilRight,
    skinStatus: item.skinStatus,
    tubeCare: item.tubeCare,
    conditionMeasure: item.conditionMeasure,
    intakeName: item.intakeName,
    intakeAmount: item.intakeAmount,
    intakeRoute: item.intakeRoute,
    outputName: item.outputName,
    outputAmount: item.outputAmount,
    outputColorCharacter: item.outputColorCharacter,
    nurseSignature: item.nurseSignature,
    version: (current?.version ?? 0) + 1,
    createdAt: current?.createdAt ?? now,
    updatedAt: now
  }
}

/**
 * Mock：批量新增或更新护理记录。
 * 同一幂等键重复提交相同内容会返回第一次结果，不会重复生成 vitalSignId。
 */
export async function mockSaveNurseStationNursingRecords(data: NurseStationSaveNursingRecordsDTO) {
  await Promise.resolve()
  assertPatientContext(data.encounterId, data.patientId)
  if (!data.idempotencyKey.trim()) throw new Error('幂等键不能为空')
  if (data.records.length < 1 || data.records.length > 50) {
    throw new Error('单次只能保存 1～50 条护理记录')
  }

  const fingerprint = JSON.stringify(data)
  const cached = saveIdempotentResults.get(data.idempotencyKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return cloneSaveResult(cached.result)
  }

  const results = data.records.map((item) => {
    if (
      !dayjs(item.recordDate, 'YYYY-MM-DD', true).isValid() ||
      !/^\d{2}:\d{2}$/.test(item.recordTime)
    ) {
      return { clientRowId: item.clientRowId, success: false, message: '记录日期或时间格式错误' }
    }
    if (!hasBusinessContent(item)) {
      return { clientRowId: item.clientRowId, success: false, message: '空白骨架行无需保存' }
    }

    const current = item.vitalSignId
      ? nursingRecords.find(
          (row) =>
            row.vitalSignId === item.vitalSignId &&
            row.encounterId === data.encounterId &&
            row.patientId === data.patientId
        )
      : undefined
    if (item.vitalSignId && !current) {
      return { clientRowId: item.clientRowId, success: false, message: '护理记录不存在' }
    }
    if (current && current.version !== item.expectedVersion) {
      return { clientRowId: item.clientRowId, success: false, message: '记录已被他人修改' }
    }

    const saved = buildStoredRow(item, current)
    const stored: MockNursingRecord = {
      ...saved,
      encounterId: data.encounterId,
      patientId: data.patientId
    }
    if (current) Object.assign(current, stored)
    else nursingRecords.push(stored)
    return {
      clientRowId: item.clientRowId,
      success: true,
      message: '保存成功',
      row: cloneRow(stored)
    }
  })

  const savedCount = results.filter((item) => item.success).length
  const result: NurseStationSaveNursingRecordsResultVO = {
    success: savedCount === results.length,
    savedCount,
    results
  }
  saveIdempotentResults.set(data.idempotencyKey, {
    fingerprint,
    result: cloneSaveResult(result)
  })
  return cloneSaveResult(result)
}

/** Mock：删除一条已保存护理记录，空白骨架行不会调用此函数。 */
export async function mockDeleteNurseStationNursingRecord(
  data: NurseStationDeleteNursingRecordDTO
) {
  await Promise.resolve()
  assertPatientContext(data.encounterId, data.patientId)
  if (!data.idempotencyKey.trim()) throw new Error('幂等键不能为空')

  const fingerprint = JSON.stringify(data)
  const cached = deleteIdempotentResults.get(data.idempotencyKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return { ...cached.result }
  }

  const index = nursingRecords.findIndex(
    (row) =>
      row.vitalSignId === data.vitalSignId &&
      row.encounterId === data.encounterId &&
      row.patientId === data.patientId
  )
  let result: NurseStationDeleteNursingRecordResultVO
  if (index < 0) {
    result = { success: false, vitalSignId: data.vitalSignId, message: '护理记录不存在' }
  } else if (nursingRecords[index]?.version !== data.expectedVersion) {
    result = { success: false, vitalSignId: data.vitalSignId, message: '记录已被他人修改' }
  } else {
    nursingRecords.splice(index, 1)
    result = { success: true, vitalSignId: data.vitalSignId, message: '删除成功' }
  }
  deleteIdempotentResults.set(data.idempotencyKey, { fingerprint, result: { ...result } })
  return { ...result }
}
