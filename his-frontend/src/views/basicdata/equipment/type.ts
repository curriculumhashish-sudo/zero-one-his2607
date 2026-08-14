import type { EquipmentSavePayload } from '@/apis/basicdata/equipment'

export interface EquipmentFormModel extends EquipmentSavePayload {
  id: string
}

export type EquipmentEnabledFilter = '' | 'enabled' | 'disabled'

export interface EquipmentDialogOpenOptions {
  id?: string
}
