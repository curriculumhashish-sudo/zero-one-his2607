import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO,
  NurseStationWardTreeVO
} from '@/apis/inpatient/nurse-station'

import type {
  NurseStationHouseOption,
  NurseStationPatientSummary,
  NurseStationQueueScope,
  NurseStationTaskFilter,
  NurseStationWardOption
} from './type'

export function mapWardTreeToOptions(tree: NurseStationWardTreeVO): {
  wardOptions: NurseStationWardOption[]
  houseOptions: NurseStationHouseOption[]
} {
  const wardOptions = tree.wardNodes.map((ward) => ({
    id: ward.wardId,
    name: ward.wardName
  }))
  const houseOptions = tree.wardNodes.flatMap((ward) =>
    ward.houseNodes.map((house) => ({
      id: house.houseId,
      name: house.houseName,
      wardId: ward.wardId
    }))
  )
  return { wardOptions, houseOptions }
}

export function mapPatientQueueItemToSummary(
  row: NurseStationPatientQueueItemVO
): NurseStationPatientSummary {
  return {
    accountId: row.accountId,
    encounterId: row.encounterId,
    patientId: row.patientId,
    patientName: row.patientName,
    encounterBusNo: row.encounterBusNo,
    genderText: row.genderEnumText,
    ageText: row.ageText,
    paymentTypeText: row.organizationName,
    statusCode: row.statusEnum,
    statusText: row.statusEnumText,
    diagnosisSummary: row.diagnosisSummary,
    wardId: row.wardId,
    wardName: row.wardName,
    houseId: row.houseId,
    houseName: row.houseName,
    bedId: row.bedId,
    bedName: row.bedName,
    pendingAuditCount: row.pendingAuditCount ?? 0,
    pendingExecuteCount: row.pendingExecuteCount ?? 0,
    availableBalance: row.availableBalance ?? row.accountBalance,
    lowBalanceWarning: row.lowBalanceWarning,
    recallEnabled: row.recallEnabled
  }
}

export function buildPatientFilterQuery(input: {
  keyword: string
  queueScope: NurseStationQueueScope
  taskFilter: NurseStationTaskFilter
  selectedWardIds: string[]
  selectedHouseIds: string[]
}): NurseStationPatientFilterQuery {
  return {
    keyword: input.keyword.trim() || undefined,
    wardIds: input.selectedWardIds.length ? input.selectedWardIds.join(',') : undefined,
    houseIds: input.selectedHouseIds.length ? input.selectedHouseIds.join(',') : undefined,
    nurseStationTaskFilter: input.taskFilter === 'all' ? undefined : input.taskFilter,
    nurseStationStatusFilter:
      input.queueScope === 'settled-discharged' ? 'SETTLED_DISCHARGED' : undefined
  }
}
