import type { SampleDetail } from '@/apis/basicdata/sample'
import type { SampleFormModel } from './type'

export function createEmptySampleForm(): SampleFormModel {
  return {
    id: '',
    code: '',
    name: '',
    pyCode: '',
    type: '',
    collectionMethod: '',
    storageCondition: '',
    validHours: 24,
    enabled: true,
    sort: 0,
    remark: ''
  }
}

export function toSampleForm(detail: SampleDetail): SampleFormModel {
  return {
    ...createEmptySampleForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
