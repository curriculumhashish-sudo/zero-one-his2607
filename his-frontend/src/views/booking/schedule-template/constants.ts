import type { StatusDisplayOption } from '@/components/common/type'
export const TEMPLATE_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'enabled', label: '启用', type: 'success' },
  { value: 'disabled', label: '停用', type: 'info' }
]
export const WEEKDAYS = [
  { value: 1, label: '星期一' },
  { value: 2, label: '星期二' },
  { value: 3, label: '星期三' },
  { value: 4, label: '星期四' },
  { value: 5, label: '星期五' },
  { value: 6, label: '星期六' },
  { value: 0, label: '星期日' }
]
