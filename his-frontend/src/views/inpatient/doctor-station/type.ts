/** 住院医生站首期预留的工作区页签。 */
export type InpatientDoctorStationTab = 'medicalRecord' | 'diagnosis' | 'orders' | 'tcmPrescription'

/** 住院医生站首期支持的顶部操作。 */
export type InpatientDoctorStationAction =
  | 'refresh'
  | 'discharge'
  | 'newMedicalRecord'
  | 'saveMedicalRecord'
  | 'signMedicalRecord'
  | 'saveDiagnosis'
  | 'saveOrder'
  | 'saveTcmPrescription'
