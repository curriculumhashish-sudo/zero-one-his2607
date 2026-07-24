export { default as ClinicalDiagnosisEditor } from './ClinicalDiagnosisEditor.vue'
export { default as DiagnosisSelectorDialog } from './DiagnosisSelectorDialog.vue'
export type {
  ClinicalDiagnosisCandidate,
  ClinicalDiagnosisDictionaryLoader,
  ClinicalDiagnosisItem,
  ClinicalDiagnosisKey
} from './type'
export {
  addClinicalDiagnoses,
  hasValidPrimaryDiagnosis,
  moveClinicalDiagnosis,
  removeClinicalDiagnosis,
  setPrimaryClinicalDiagnosis
} from './utils'
