import type {
  NurseStationNursingRecordRowVO,
  NurseStationNursingRecordSaveItemDTO,
  NurseStationNursingRecordSaveResultVO
} from '@/apis/inpatient/nurse-station'

/** 纸质单中的预制空行和护士手动添加行。 */
export type NursingRecordRowType = 'extra' | 'skeleton'

/**
 * 页面可编辑行。
 * 接口字段在编辑态统一转成字符串，避免 input 在 undefined 和字符串之间来回切换。
 */
export interface NursingRecordViewRow {
  clientRowId: string
  vitalSignId?: string
  version?: number
  rowType: NursingRecordRowType
  isDirty: boolean
  recordDate: string
  recordTime: string
  temperature: string
  pulse: string
  heartRate: string
  respiration: string
  bloodPressure: string
  consciousness: string
  pupilLeft: string
  pupilRight: string
  observation: string
  intakeName: string
  intakeVolume: string
  outputName: string
  outputVolume: string
  outputColor: string
  nurseSignature: string
}

let clientRowSeed = 0

function nextClientRowId() {
  clientRowSeed += 1
  return `nursing-row-${clientRowSeed}`
}

/** 创建一条不落库的纸质单空行。 */
export function createEmptyNursingRecordRow(
  partial: Partial<NursingRecordViewRow> = {}
): NursingRecordViewRow {
  return {
    clientRowId: nextClientRowId(),
    rowType: 'skeleton',
    isDirty: false,
    recordDate: '',
    recordTime: '',
    temperature: '',
    pulse: '',
    heartRate: '',
    respiration: '',
    bloodPressure: '',
    consciousness: '',
    pupilLeft: '',
    pupilRight: '',
    observation: '',
    intakeName: '',
    intakeVolume: '',
    outputName: '',
    outputVolume: '',
    outputColor: '',
    nurseSignature: '',
    ...partial
  }
}

/** 每周预制 7 天 × 每天 2 个时点，共 14 行纸质单骨架。 */
export function buildWeekSkeletonRows() {
  return Array.from({ length: 14 }, () => createEmptyNursingRecordRow())
}

/** 接口记录转换为页面可编辑行，空字段统一变成空字符串。 */
export function mapNursingRecordToViewRow(
  row: NurseStationNursingRecordRowVO,
  rowType: NursingRecordRowType
): NursingRecordViewRow {
  return {
    clientRowId: nextClientRowId(),
    vitalSignId: row.vitalSignId,
    version: row.version,
    rowType,
    isDirty: false,
    recordDate: row.recordDate,
    recordTime: row.recordTime,
    temperature: row.temperature ?? '',
    pulse: row.pulse ?? '',
    heartRate: row.heartRate ?? '',
    respiration: row.respiration ?? '',
    bloodPressure: row.bloodPressure ?? '',
    consciousness: row.consciousness ?? '',
    pupilLeft: row.pupilLeft ?? '',
    pupilRight: row.pupilRight ?? '',
    observation: row.observation ?? '',
    intakeName: row.intakeName ?? '',
    intakeVolume: row.intakeVolume ?? '',
    outputName: row.outputName ?? '',
    outputVolume: row.outputVolume ?? '',
    outputColor: row.outputColor ?? '',
    nurseSignature: row.nurseSignature ?? ''
  }
}

/**
 * 把已保存记录放入纸质单前部，再补足 14 行骨架。
 * 超过 14 条的记录按额外行展示，只有额外行显示删除按钮。
 */
export function mergeRecordsWithWeekSkeleton(rows: NurseStationNursingRecordRowVO[]) {
  const sorted = [...rows].sort((left, right) =>
    `${left.recordDate} ${left.recordTime}`.localeCompare(`${right.recordDate} ${right.recordTime}`)
  )
  const skeletonRows = sorted.slice(0, 14).map((row) => mapNursingRecordToViewRow(row, 'skeleton'))
  while (skeletonRows.length < 14) skeletonRows.push(createEmptyNursingRecordRow())
  const extraRows = sorted.slice(14).map((row) => mapNursingRecordToViewRow(row, 'extra'))
  return [...skeletonRows, ...extraRows]
}

const contentKeys: Array<keyof NursingRecordViewRow> = [
  'temperature',
  'pulse',
  'heartRate',
  'respiration',
  'bloodPressure',
  'consciousness',
  'pupilLeft',
  'pupilRight',
  'observation',
  'intakeName',
  'intakeVolume',
  'outputName',
  'outputVolume',
  'outputColor',
  'nurseSignature'
]

/** 判断是否至少填写了一项真正的护理业务内容。 */
export function hasNursingRecordContent(row: NursingRecordViewRow) {
  return contentKeys.some((key) => String(row[key] ?? '').trim().length > 0)
}

/**
 * 只把已修改且含业务内容的行送入保存接口。
 * 空白骨架行会留在页面，不创建无意义的数据库记录。
 */
export function mapViewRowToSaveItem(
  row: NursingRecordViewRow
): NurseStationNursingRecordSaveItemDTO | null {
  if (!row.isDirty || !hasNursingRecordContent(row)) return null
  return {
    clientRowId: row.clientRowId,
    vitalSignId: row.vitalSignId,
    expectedVersion: row.version,
    recordDate: row.recordDate,
    recordTime: row.recordTime,
    temperature: row.temperature || undefined,
    pulse: row.pulse || undefined,
    heartRate: row.heartRate || undefined,
    respiration: row.respiration || undefined,
    bloodPressure: row.bloodPressure || undefined,
    consciousness: row.consciousness || undefined,
    pupilLeft: row.pupilLeft || undefined,
    pupilRight: row.pupilRight || undefined,
    observation: row.observation || undefined,
    intakeName: row.intakeName || undefined,
    intakeVolume: row.intakeVolume || undefined,
    outputName: row.outputName || undefined,
    outputVolume: row.outputVolume || undefined,
    outputColor: row.outputColor || undefined,
    nurseSignature: row.nurseSignature || undefined
  }
}

/** 把批量保存成功结果合并回对应的页面行，并保留原来的骨架/额外行身份。 */
export function applySaveResults(
  rows: NursingRecordViewRow[],
  results: NurseStationNursingRecordSaveResultVO[]
) {
  const resultMap = new Map(results.map((item) => [item.clientRowId, item]))
  return rows.map((current) => {
    const result = resultMap.get(current.clientRowId)
    if (!result?.success || !result.row) return current
    return {
      ...mapNursingRecordToViewRow(result.row, current.rowType),
      clientRowId: current.clientRowId
    }
  })
}
