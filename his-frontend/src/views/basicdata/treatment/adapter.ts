import type { TreatmentDetail } from '@/apis/basicdata/treatment'
import type { TreatmentFormModel } from './type'

export function createEmptyTreatmentForm(): TreatmentFormModel {
  return {
    id: '',
    code: '',
    name: '',
    pyCode: '',
    category: '',
    unit: '次',
    price: 0,
    enabled: true,
    sort: 0,
    remark: ''
  }
}

export function toTreatmentForm(detail: TreatmentDetail): TreatmentFormModel {
  return {
    ...createEmptyTreatmentForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
