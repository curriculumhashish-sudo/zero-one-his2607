import type { TreatmentSavePayload } from '@/apis/basicdata/treatment'

export interface TreatmentFormModel extends TreatmentSavePayload {
  id: string
}

export type TreatmentEnabledFilter = '' | 'enabled' | 'disabled'

export interface TreatmentDialogOpenOptions {
  id?: string
}
