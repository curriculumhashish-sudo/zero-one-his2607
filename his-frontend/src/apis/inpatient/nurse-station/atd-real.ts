import dayjs from 'dayjs'

import { useHttp } from '@/plugins/http'

import type {
  NurseStationAssignBedDTO,
  NurseStationAssignBedResultVO,
  NurseStationBedBoardItemVO,
  NurseStationBedBoardQuery,
  NurseStationBedBoardVO,
  NurseStationBedLocationVO,
  NurseStationEncounterTransitionDTO,
  NurseStationEncounterTransitionResultVO
} from './atd-type'
import type { NurseStationPatientFilterQuery, NurseStationPatientQueueItemVO } from './type'
import { realApiBaseURL, realLoadAllNurseStationPatientRows } from './patient-real'

/** Swagger `/bed` 返回的空闲床位字段。 */
interface BackendBedVO {
  id: string
  name: string
  parent_id: string
  form_enum: string
  status_enum: string
  organization_id: string
}

/** Swagger `/bed` 的分页包装。 */
interface BackendBedPageVO {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendBedVO[]
}

/** 真实接口返回的统一包装；写接口的 data 允许为空。 */
interface BackendJsonVO<T> {
  code?: number
  message?: string
  data?: T
}

function parseIds(value?: string) {
  return new Set(
    (value ?? '')
      .split(',')
      .map((item) => item.trim())
      .filter(Boolean)
  )
}

function unwrapData<T>(response: BackendJsonVO<T>): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '床位接口未返回有效数据')
  }
  return response.data
}

function getRealHttp() {
  return useHttp()
}

function unknownHouseId(parentId: string) {
  return `__unknown-house__${parentId || 'root'}`
}

function isSelectedHouseUnknown(query: NurseStationBedBoardQuery) {
  return parseIds(query.houseIds).size > 0
}

function mapFreeBed(bed: BackendBedVO): NurseStationBedLocationVO {
  const parentId = bed.parent_id || 'unknown-ward'
  return {
    bedId: bed.id,
    bedName: bed.name,
    wardId: parentId,
    wardName: parentId,
    houseId: unknownHouseId(parentId),
    houseName: '未提供病房',
    operationalEnum: bed.status_enum === 'ENABLE' ? 'AVAILABLE' : 'DISABLED'
  }
}

function mapOccupiedBed(patient: NurseStationPatientQueueItemVO): NurseStationBedBoardItemVO {
  return {
    bed: {
      bedId: patient.bedId!,
      bedName: patient.bedName || patient.bedId!,
      wardId: patient.wardId || 'unknown-ward',
      wardName: patient.wardName || patient.wardId || '未提供病区',
      houseId: patient.houseId || unknownHouseId(patient.wardId || 'unknown-ward'),
      houseName: patient.houseName || '未提供病房',
      operationalEnum: 'OCCUPIED'
    },
    occupant: {
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      patientName: patient.patientName,
      genderEnumText: patient.genderEnumText,
      ageText: patient.ageText,
      diagnosisSummary: patient.diagnosisSummary,
      statusEnum: patient.statusEnum,
      statusEnumText: patient.statusEnumText,
      priorityEnum: patient.priorityEnum
    }
  }
}

async function queryFreeBedPage(parentId: string | undefined, pageIndex: number) {
  const response = await getRealHttp().get<BackendBedPageVO>(
    '/bed',
    {
      pageIndex,
      pageSize: 100,
      status_enum: 'ENABLE',
      intended_admission_date: dayjs().format('YYYY-MM-DD'),
      parent_id: parentId
    },
    { baseURL: realApiBaseURL }
  )
  return unwrapData(response)
}

async function queryAllFreeBeds(parentIds: string[]) {
  const targets = parentIds.length ? parentIds : [undefined]
  const result: BackendBedVO[] = []

  for (const parentId of targets) {
    let parentRows = 0
    let pageIndex = 1
    let total = 0
    do {
      const page = await queryFreeBedPage(parentId, pageIndex)
      const rows = page.rows ?? []
      result.push(...rows)
      parentRows += rows.length
      total = page.total ?? rows.length
      pageIndex += 1
      if (!rows.length) break
    } while (parentRows < total)
  }

  return result
}

async function queryOccupiedPatients(query: NurseStationBedBoardQuery) {
  const filter: NurseStationPatientFilterQuery = {
    wardIds: query.wardIds,
    houseIds: query.houseIds
  }
  const rows = await realLoadAllNurseStationPatientRows(filter)
  const dischargeStatuses = new Set(['ON_HOLD', 'PENDING', 'COMPLETED'])
  return query.panel === 'DISCHARGE'
    ? rows.filter((patient) => dischargeStatuses.has(patient.statusEnum ?? '') && patient.bedId)
    : rows.filter((patient) => patient.bedId)
}

/**
 * 组合真实患者队列和空闲床位接口，补足原页面需要的床位看板结构。
 * 空床接口没有病房字段，因此只在未选择具体病房时展示未知病房空床。
 */
export async function realQueryNurseStationBedBoard(
  query: NurseStationBedBoardQuery
): Promise<NurseStationBedBoardVO> {
  const occupiedPatients = await queryOccupiedPatients(query)
  const occupiedItems = occupiedPatients.map(mapOccupiedBed)
  if (query.panel === 'DISCHARGE') return { items: occupiedItems }

  const occupiedBedIds = new Set(occupiedItems.map((item) => item.bed.bedId))
  const wardIds = [...parseIds(query.wardIds)]
  const freeBeds = await queryAllFreeBeds(wardIds)
  const freeItems = freeBeds
    .filter((bed) => !occupiedBedIds.has(bed.id))
    .map((bed) => ({ bed: mapFreeBed(bed), occupant: null }))
    .filter(
      (item) => !isSelectedHouseUnknown(query) || !item.bed.houseId.startsWith('__unknown-house__')
    )

  return { items: [...occupiedItems, ...freeItems] }
}

/** 调用后端床位预约接口。 */
export async function realAssignNurseStationBed(
  data: NurseStationAssignBedDTO
): Promise<NurseStationAssignBedResultVO> {
  const response = await getRealHttp().post<string>(
    '/bed/assign',
    {
      patient_id: data.patientId,
      location_id: data.targetBedId,
      // 当前患者队列没有 appointmentId，按已确认方案兼容使用 encounterId。
      appointment_id: data.encounterId,
      intended_admission_date: data.intendedAdmissionDate || dayjs().format('YYYY-MM-DD')
    },
    { baseURL: realApiBaseURL }
  )
  const reservationId = unwrapData(response)
  if (!reservationId.trim()) throw new Error(response.message || '床位预约失败')

  return {
    encounterId: data.encounterId,
    reservationId,
    succeeded: true
  }
}

function mapTransitionResult(
  response: BackendJsonVO<NurseStationEncounterTransitionResultVO>,
  fallback: NurseStationEncounterTransitionDTO
): NurseStationEncounterTransitionResultVO {
  const data = unwrapData(response)
  if (data.succeeded !== true) throw new Error(response.message || '住院状态变更失败')
  return {
    encounterId: data.encounterId || fallback.encounterId,
    succeeded: true
  }
}

/** 办理出院由后端最终检查待办，前端不再伪造出院前明细。 */
export async function realExecuteInpatientDischarge(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  const response = await getRealHttp().post<NurseStationEncounterTransitionResultVO>(
    '/admissiontransfer/execute-inpatient-discharge',
    data,
    { baseURL: realApiBaseURL }
  )
  return mapTransitionResult(response, data)
}

/** 清床离院使用后端现有接口。 */
export async function realClearDischargedBed(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  const response = await getRealHttp().post<NurseStationEncounterTransitionResultVO>(
    '/admissiontransfer/clear-discharged-bed',
    data,
    { baseURL: realApiBaseURL }
  )
  return mapTransitionResult(response, data)
}
