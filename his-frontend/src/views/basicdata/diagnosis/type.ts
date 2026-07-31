import type { DiagnosisSavePayload } from '@/apis/basicdata/diagnosis'

export interface DiagnosisFormModel extends DiagnosisSavePayload {
  id: string
}

export type DiagnosisEnabledFilter = '' | 'enabled' | 'disabled'

export interface DiagnosisDialogOpenOptions {
  id?: string
}
