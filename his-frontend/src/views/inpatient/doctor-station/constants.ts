import type { DoctorStationTabItem } from '@/components/clinical/type'
import type { InpatientDoctorStationTab } from './type'

/** 住院医生站工作区页签。 */
export const INPATIENT_DOCTOR_STATION_TABS: Array<
  DoctorStationTabItem & { key: InpatientDoctorStationTab }
> = [
  { key: 'medicalRecord', label: '住院病历' },
  { key: 'diagnosis', label: '诊断录入' },
  { key: 'orders', label: '开立医嘱' },
  { key: 'tcmPrescription', label: '中医处方' }
]
