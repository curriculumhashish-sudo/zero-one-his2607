import type { FrequencySavePayload } from '@/apis/basicdata/frequency'

export interface FrequencyFormModel extends FrequencySavePayload {
  id: string
}

export type FrequencyEnabledFilter = '' | 'enabled' | 'disabled'

export interface FrequencyDialogOpenOptions {
  id?: string
}
