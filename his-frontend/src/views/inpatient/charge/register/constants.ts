import type { StatusDisplayOption } from '@/components/common/type'

export const REGISTRATION_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'WAIT_ADMISSION', label: '待登记', type: 'warning' },
  { value: 'ADMITTED', label: '已登记', type: 'success' },
  { value: 'CANCELLED', label: '已取消', type: 'info' }
]

/** 参考原型 `PATIENT_CONDITION_OPTIONS`，值直接提交到 `priorityEnum`。 */
export const PATIENT_CONDITION_OPTIONS = [
  { label: '一般', value: 'GENERAL' },
  { label: '急', value: 'URGENT' },
  { label: '危', value: 'CRITICAL' }
] as const
