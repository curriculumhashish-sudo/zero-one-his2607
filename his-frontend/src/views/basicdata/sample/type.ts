import type { SampleSavePayload } from '@/apis/basicdata/sample'

export interface SampleFormModel extends SampleSavePayload {
  id: string
}

export type SampleEnabledFilter = '' | 'enabled' | 'disabled'

export interface SampleDialogOpenOptions {
  id?: string
}
