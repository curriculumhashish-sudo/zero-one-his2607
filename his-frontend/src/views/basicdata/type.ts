import type { MedicationCatalogSavePayload } from '@/apis/basicdata'

export interface MedicationCatalogFormModel extends MedicationCatalogSavePayload {
  id: string
}

export type MedicationEnabledFilter = '' | 'enabled' | 'disabled'

export interface MedicationCatalogDialogOpenOptions {
  id?: string
}
