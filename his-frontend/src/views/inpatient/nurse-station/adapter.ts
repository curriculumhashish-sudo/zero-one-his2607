import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary, NurseStationTaskFilter } from './type'

export function mapPatientQueueItemToSummary(
  row: NurseStationPatientQueueItemVO
): NurseStationPatientSummary {
  return {
    accountId: row.accountId,
    encounterId: row.encounterId,
    patientId: row.patientId,
    patientName: row.patientName,
    encounterBusNo: row.encounterBusNo,
    admissionTime: row.admissionTime,
    genderText: row.genderEnumText,
    ageText: row.ageText,
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
    lowBalanceWarning: row.lowBalanceWarning
  }
}

export function buildPatientFilterQuery(input: {
  keyword: string
  taskFilter: NurseStationTaskFilter
}): NurseStationPatientFilterQuery {
  return {
    keyword: input.keyword.trim() || undefined,
    nurseStationTaskFilter: input.taskFilter === 'all' ? undefined : input.taskFilter
  }
}
