import type { MedicationDetail } from '@/apis/basicdata/medication'
import type { MedicationFormModel } from './type'

export function createEmptyMedicationForm(): MedicationFormModel {
  return {
    id: '',
    code: '',
    name: '',
    genericName: '',
    category: '',
    specification: '',
    dosageForm: '',
    unit: '',
    manufacturer: '',
    retailPrice: 0,
    enabled: true,
    sort: 0,
    remark: ''
  }
}

export function toMedicationForm(detail: MedicationDetail): MedicationFormModel {
  return {
    ...createEmptyMedicationForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
