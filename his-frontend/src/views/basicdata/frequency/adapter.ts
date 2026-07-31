import type { FrequencyDetail } from '@/apis/basicdata/frequency'
import type { FrequencyFormModel } from './type'

export function createEmptyFrequencyForm(): FrequencyFormModel {
  return {
    id: '',
    code: '',
    name: '',
    type: 'DAILY',
    timesPerDay: 1,
    intervalHours: 24,
    timePoints: '',
    enabled: true,
    sort: 1,
    remark: ''
  }
}

export function toFrequencyForm(detail: FrequencyDetail): FrequencyFormModel {
  return {
    ...createEmptyFrequencyForm(),
    ...detail,
    id: detail.id ?? ''
  }
}
