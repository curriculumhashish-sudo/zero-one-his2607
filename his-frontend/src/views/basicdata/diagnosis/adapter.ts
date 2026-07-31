import type { DiagnosisDetail } from '@/apis/basicdata/diagnosis'
import type { DiagnosisFormModel } from './type'

export function createEmptyDiagnosisForm(): DiagnosisFormModel {
  return {
    id: '',
    code: '',
    name: '',
    pyCode: '',
    type: 'WESTERN',
    category: 'OUTPATIENT',
    enabled: true,
    sort: 0,
    remark: ''
  }
}

export function toDiagnosisForm(detail: DiagnosisDetail): DiagnosisFormModel {
  return {
    ...createEmptyDiagnosisForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
