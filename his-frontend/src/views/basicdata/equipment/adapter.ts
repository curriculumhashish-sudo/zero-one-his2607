import type { EquipmentDetail } from '@/apis/basicdata/equipment'
import type { EquipmentFormModel } from './type'

export function createEmptyEquipmentForm(): EquipmentFormModel {
  return {
    id: '',
    code: '',
    name: '',
    pyCode: '',
    category: 'CONSUMABLE',
    specification: '',
    model: '',
    unit: '',
    price: 0,
    enabled: true,
    sort: 0,
    remark: ''
  }
}

export function toEquipmentForm(detail: EquipmentDetail): EquipmentFormModel {
  return {
    ...createEmptyEquipmentForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
