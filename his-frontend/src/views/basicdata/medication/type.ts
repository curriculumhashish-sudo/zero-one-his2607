import type { MedicationSavePayload } from '@/apis/basicdata/medication'

export interface MedicationFormModel extends MedicationSavePayload {
  id: string
}

export type MedicationEnabledFilter = '' | 'enabled' | 'disabled'

export interface MedicationDialogOpenOptions {
  id?: string
}
