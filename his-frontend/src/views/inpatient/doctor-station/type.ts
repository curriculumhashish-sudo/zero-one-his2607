/** 住院医生站首期预留的工作区页签。 */
export type InpatientDoctorStationTab = 'medicalRecord' | 'diagnosis' | 'orders' | 'tcmPrescription'

export interface InpatientDiagnosisItem {
  key: string
  id?: string
  conditionId?: string
  definitionCode: string
  description: string
  kindName: string
  diagnosisTypeCode: string
  diagnosisTypeName: string
  primaryFlag: number
  rank: number
  tcmFlag: number
}

/** 住院医生站首期支持的顶部操作。 */
export type InpatientDoctorStationAction =
  | 'refresh'
  | 'discharge'
  | 'newMedicalRecord'
  | 'saveMedicalRecord'
  | 'saveDiagnosis'
  | 'saveOrder'
  | 'saveTcmPrescription'
