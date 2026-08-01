import {
  mockFindNurseStationPatient,
  mockListNurseStationPatients,
  mockUpdateNurseStationPatient
} from './mock'
import type {
  NurseStationBedBoardItemVO,
  NurseStationBedBoardQuery,
  NurseStationBedBoardVO,
  NurseStationBedOperationResultVO,
  NurseStationDischargePendingExecutionVO,
  NurseStationDischargePendingMedicationVO,
  NurseStationEncounterTransitionDTO,
  NurseStationEncounterTransitionResultVO,
  NurseStationExecuteBedOperationDTO
} from './atd-type'

interface MockBedDefinition {
  bedId: string
  bedName: string
  wardId: string
  wardName: string
  houseId: string
  houseName: string
  displayOrder: number
  disabled?: boolean
  version: number
}

const beds: MockBedDefinition[] = [
  ['bed-301-01', '01床', 'ward-internal', '内科疗区', 'house-301', '301病房', 1],
  ['bed-301-02', '02床', 'ward-internal', '内科疗区', 'house-301', '301病房', 2],
  ['bed-301-03', '03床', 'ward-internal', '内科疗区', 'house-301', '301病房', 3],
  ['bed-302-01', '01床', 'ward-internal', '内科疗区', 'house-302', '302病房', 1],
  ['bed-302-02', '02床', 'ward-internal', '内科疗区', 'house-302', '302病房', 2],
  ['bed-302-03', '03床', 'ward-internal', '内科疗区', 'house-302', '302病房', 3],
  ['bed-401-01', '01床', 'ward-surgery', '外科疗区', 'house-401', '401病房', 1],
  ['bed-401-02', '02床', 'ward-surgery', '外科疗区', 'house-401', '401病房', 2],
  ['bed-401-05', '05床', 'ward-surgery', '外科疗区', 'house-401', '401病房', 5],
  ['bed-402-01', '01床', 'ward-surgery', '外科疗区', 'house-402', '402病房', 1],
  ['bed-402-02', '02床', 'ward-surgery', '外科疗区', 'house-402', '402病房', 2],
  ['bed-402-03', '03床', 'ward-surgery', '外科疗区', 'house-402', '402病房', 3],
  ['bed-501-01', '01床', 'ward-pediatrics', '儿科疗区', 'house-501', '501病房', 1],
  ['bed-501-02', '02床', 'ward-pediatrics', '儿科疗区', 'house-501', '501病房', 2],
  ['bed-502-01', '01床', 'ward-pediatrics', '儿科疗区', 'house-502', '502病房', 1],
  ['bed-502-02', '02床', 'ward-pediatrics', '儿科疗区', 'house-502', '502病房', 2]
].map(([bedId, bedName, wardId, wardName, houseId, houseName, displayOrder]) => ({
  bedId: String(bedId),
  bedName: String(bedName),
  wardId: String(wardId),
  wardName: String(wardName),
  houseId: String(houseId),
  houseName: String(houseName),
  displayOrder: Number(displayOrder),
  version: 1
}))

const pendingExecutions: Record<string, NurseStationDischargePendingExecutionVO[]> = {
  'encounter-003': [
    {
      requestId: 'request-discharge-001',
      therapyEnum: 'LONG_TERM',
      reqAuthoredTime: '2026-07-30 08:00:00',
      displayName: '头孢曲松钠静脉滴注',
      doseText: '2g',
      rateCode: 'QD',
      blockingReason: '仍有计划执行时点未处理'
    }
  ]
}

const pendingMedications: Record<string, NurseStationDischargePendingMedicationVO[]> = {
  'encounter-003': [
    {
      dispenseId: 'dispense-discharge-001',
      medicationName: '头孢曲松钠',
      statusEnum: 'WAIT_DISPENSE',
      statusEnumText: '待发药',
      quantity: 2,
      unitCode: '支',
      performLocationName: '住院药房',
      plannedDispenseTime: '2026-07-31 08:00:00'
    }
  ]
}

const idempotentResults = new Map<string, NurseStationBedOperationResultVO>()

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

function movePatientToBed(encounterId: string, bed: MockBedDefinition) {
  mockUpdateNurseStationPatient(encounterId, {
    bedId: bed.bedId,
    bedName: bed.bedName,
    wardId: bed.wardId,
    wardName: bed.wardName,
    houseId: bed.houseId,
    houseName: bed.houseName,
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    recallEnabled: false
  })
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

/** Mock：原子执行首次分床、换空床或两患者互换。 */
export async function mockExecuteNurseStationBedOperation(
  data: NurseStationExecuteBedOperationDTO
): Promise<NurseStationBedOperationResultVO> {
  await Promise.resolve()
  const cached = idempotentResults.get(data.idempotencyKey)
  if (cached) return { ...cached }

  const patient = mockFindNurseStationPatient(data.encounterId)
  const targetBed = findBed(data.targetBedId)
  if (!patient || !targetBed) throw new Error('患者或目标床位不存在')
  if (targetBed.houseId !== data.targetHouseId) throw new Error('目标床位与病房不匹配')
  if (data.expectedBedVersion !== undefined && data.expectedBedVersion !== targetBed.version) {
    throw new Error('床位状态已变化，请刷新床位图后重试')
  }

  const targetOccupant = findOccupant(targetBed.bedId)
  const sourceBed = patient.bedId ? findBed(patient.bedId) : undefined
  if (data.operationType === 'ASSIGN') {
    if (patient.bedId) throw new Error('患者已有床位，不能重复分床')
    if (targetOccupant) throw new Error('目标床位已被占用')
    movePatientToBed(patient.encounterId, targetBed)
  } else if (data.operationType === 'TRANSFER') {
    if (!sourceBed) throw new Error('患者尚未分床，请使用首次分床')
    if (targetOccupant) throw new Error('换床目标必须为空床')
    movePatientToBed(patient.encounterId, targetBed)
    sourceBed.version += 1
  } else {
    if (!sourceBed || !targetOccupant) throw new Error('床位互换要求两名患者均已占床')
    if (targetOccupant.encounterId !== data.swapEncounterId) throw new Error('互换患者已变化')
    movePatientToBed(patient.encounterId, targetBed)
    movePatientToBed(targetOccupant.encounterId, sourceBed)
    sourceBed.version += 1
  }
  targetBed.version += 1

  const result: NurseStationBedOperationResultVO = {
    encounterId: patient.encounterId,
    operationType: data.operationType,
    succeeded: true
  }
  idempotentResults.set(data.idempotencyKey, result)
  return { ...result }
}

export async function mockQueryDischargePendingExecutions(encounterId: string) {
  await Promise.resolve()
  return (pendingExecutions[encounterId] ?? []).map((item) => ({ ...item }))
}

export async function mockQueryDischargePendingMedications(encounterId: string) {
  await Promise.resolve()
  return (pendingMedications[encounterId] ?? []).map((item) => ({ ...item }))
}

/** Mock：办理出院，服务端仍会再次校验出院待办。 */
export async function mockExecuteInpatientDischarge(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  if (!patient) throw new Error('患者不存在')
  if ((pendingExecutions[data.encounterId]?.length ?? 0) > 0) throw new Error('仍有待执行医嘱')
  if ((pendingMedications[data.encounterId]?.length ?? 0) > 0) throw new Error('仍有待发或待退药')
  const previousStatusEnum = patient.statusEnum
  mockUpdateNurseStationPatient(data.encounterId, {
    statusEnum: 'PENDING',
    statusEnumText: '待结算'
  })
  return {
    encounterId: data.encounterId,
    succeeded: true,
    previousStatusEnum,
    targetStatusEnum: 'PENDING',
    bedReleased: false
  }
}

/** Mock：清床并将患者移入已出院结算范围。 */
export async function mockClearDischargedBed(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  if (!patient) throw new Error('患者不存在')
  if (patient.statusEnum !== 'PENDING' && patient.statusEnum !== 'COMPLETED') {
    throw new Error('患者尚未完成出院结算，不能清床')
  }
  const previousStatusEnum = patient.statusEnum
  const sourceBed = patient.bedId ? findBed(patient.bedId) : undefined
  mockUpdateNurseStationPatient(data.encounterId, {
    bedId: undefined,
    bedName: undefined,
    houseId: undefined,
    houseName: undefined,
    statusEnum: 'DISCHARGED',
    statusEnumText: '已离院',
    recallEnabled: true
  })
  if (sourceBed) sourceBed.version += 1
  return {
    encounterId: data.encounterId,
    succeeded: true,
    previousStatusEnum,
    targetStatusEnum: 'DISCHARGED',
    bedReleased: true
  }
}

/** Mock：出院召回后恢复待入科，不自动占回原床。 */
export async function mockRecallInpatientDischarge(
  data: NurseStationEncounterTransitionDTO
): Promise<NurseStationEncounterTransitionResultVO> {
  await Promise.resolve()
  const patient = mockFindNurseStationPatient(data.encounterId)
  if (!patient) throw new Error('患者不存在')
  const previousStatusEnum = patient.statusEnum
  mockUpdateNurseStationPatient(data.encounterId, {
    bedId: undefined,
    bedName: undefined,
    houseId: undefined,
    houseName: undefined,
    statusEnum: 'REGISTERED',
    statusEnumText: '待入科',
    recallEnabled: false
  })
  return {
    encounterId: data.encounterId,
    succeeded: true,
    previousStatusEnum,
    targetStatusEnum: 'REGISTERED',
    bedReleased: true,
    requiresBedAssignment: true
  }
}
