import {
  mockFindNurseStationPatient,
  mockListNurseStationPatients,
  mockUpdateNurseStationPatient
} from './mock'
import type {
  NurseStationAssignBedDTO,
  NurseStationAssignBedResultVO,
  NurseStationBedBoardItemVO,
  NurseStationBedBoardQuery,
  NurseStationBedBoardVO,
  NurseStationEncounterTransitionDTO,
  NurseStationEncounterTransitionResultVO
} from './atd-type'

interface MockBedDefinition {
  bedId: string
  bedName: string
  wardId: string
  wardName: string
  houseId: string
  houseName: string
  disabled?: boolean
}

const beds: MockBedDefinition[] = [
  ['bed-301-01', '01床', 'ward-internal', '内科疗区', 'house-301', '301病房'],
  ['bed-301-02', '02床', 'ward-internal', '内科疗区', 'house-301', '301病房'],
  ['bed-301-03', '03床', 'ward-internal', '内科疗区', 'house-301', '301病房'],
  ['bed-302-01', '01床', 'ward-internal', '内科疗区', 'house-302', '302病房'],
  ['bed-302-02', '02床', 'ward-internal', '内科疗区', 'house-302', '302病房'],
  ['bed-302-03', '03床', 'ward-internal', '内科疗区', 'house-302', '302病房'],
  ['bed-401-01', '01床', 'ward-surgery', '外科疗区', 'house-401', '401病房'],
  ['bed-401-02', '02床', 'ward-surgery', '外科疗区', 'house-401', '401病房'],
  ['bed-401-05', '05床', 'ward-surgery', '外科疗区', 'house-401', '401病房'],
  ['bed-402-01', '01床', 'ward-surgery', '外科疗区', 'house-402', '402病房'],
  ['bed-402-02', '02床', 'ward-surgery', '外科疗区', 'house-402', '402病房'],
  ['bed-402-03', '03床', 'ward-surgery', '外科疗区', 'house-402', '402病房'],
  ['bed-501-01', '01床', 'ward-pediatrics', '儿科疗区', 'house-501', '501病房'],
  ['bed-501-02', '02床', 'ward-pediatrics', '儿科疗区', 'house-501', '501病房'],
  ['bed-502-01', '01床', 'ward-pediatrics', '儿科疗区', 'house-502', '502病房'],
  ['bed-502-02', '02床', 'ward-pediatrics', '儿科疗区', 'house-502', '502病房']
].map(([bedId, bedName, wardId, wardName, houseId, houseName]) => ({
  bedId: String(bedId),
  bedName: String(bedName),
  wardId: String(wardId),
  wardName: String(wardName),
  houseId: String(houseId),
  houseName: String(houseName)
}))

function parseIds(value?: string) {
  return new Set(
    (value ?? '')
      .split(',')
      .map((item) => item.trim())
      .filter(Boolean)
  )
}

function findBed(bedId: string) {
  return beds.find((bed) => bed.bedId === bedId)
}

function findOccupant(bedId: string) {
  return mockListNurseStationPatients().find((patient) => patient.bedId === bedId)
}

function toBoardItem(bed: MockBedDefinition): NurseStationBedBoardItemVO {
  const occupant = findOccupant(bed.bedId)
  return {
    bed: {
      ...bed,
      operationalEnum: bed.disabled ? 'DISABLED' : occupant ? 'OCCUPIED' : 'AVAILABLE'
    },
    occupant: occupant
      ? {
          encounterId: occupant.encounterId,
          patientId: occupant.patientId,
          patientName: occupant.patientName,
          genderEnumText: occupant.genderEnumText,
          ageText: occupant.ageText,
          diagnosisSummary: occupant.diagnosisSummary,
          statusEnum: occupant.statusEnum,
          statusEnumText: occupant.statusEnumText,
          priorityEnum: occupant.priorityEnum
        }
      : null
  }
}

/** Mock：查询包含占床信息的完整床位看板。 */
export async function mockQueryNurseStationBedBoard(
  query: NurseStationBedBoardQuery
): Promise<NurseStationBedBoardVO> {
  await Promise.resolve()
  const wardIds = parseIds(query.wardIds)
  const houseIds = parseIds(query.houseIds)
  const items = beds
    .filter((bed) => houseIds.size === 0 || houseIds.has(bed.houseId))
    .filter((bed) => houseIds.size > 0 || wardIds.size === 0 || wardIds.has(bed.wardId))
    .map(toBoardItem)
    .filter((item) => {
      if (query.panel === 'ADMISSION') return true
      return ['ON_HOLD', 'PENDING', 'COMPLETED'].includes(item.occupant?.statusEnum ?? '')
    })
  return { items }
}

/** Mock：预约空闲床位。 */
export async function mockAssignNurseStationBed(
  data: NurseStationAssignBedDTO
): Promise<NurseStationAssignBedResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  const targetBed = findBed(data.targetBedId)
  if (!patient || !targetBed) throw new Error('患者或目标床位不存在')
  if (patient.bedId) throw new Error('患者已有床位，不能重复预约')
  if (findOccupant(targetBed.bedId)) throw new Error('目标床位已被占用')
  return {
    encounterId: patient.encounterId,
    reservationId: `reservation-${patient.encounterId}-${targetBed.bedId}`,
    succeeded: true
  }
}

/** Mock：办理出院。 */
export async function mockExecuteInpatientDischarge(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  if (!patient) throw new Error('患者不存在')
  if (patient.statusEnum !== 'ON_HOLD') throw new Error('当前状态不能办理出院')
  if ((patient.pendingAuditCount ?? 0) > 0 || (patient.pendingExecuteCount ?? 0) > 0) {
    throw new Error('仍有未完成医嘱')
  }
  mockUpdateNurseStationPatient(data.encounterId, {
    statusEnum: 'PENDING',
    statusEnumText: '待结算'
  })
  return { encounterId: data.encounterId, succeeded: true }
}

/** Mock：结算完成后清床离院。 */
export async function mockClearDischargedBed(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  if (!patient) throw new Error('患者不存在')
  if (patient.statusEnum !== 'COMPLETED') {
    throw new Error('患者尚未完成出院结算，不能清床')
  }
  mockUpdateNurseStationPatient(data.encounterId, {
    bedId: undefined,
    bedName: undefined,
    houseId: undefined,
    houseName: undefined,
    statusEnum: 'DISCHARGED',
    statusEnumText: '已离院'
  })
  return { encounterId: data.encounterId, succeeded: true }
}
