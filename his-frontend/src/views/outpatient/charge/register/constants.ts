import type { StatusDisplayOption } from '@/components/common/type'

export const REGISTER_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'PLANNED', label: '已挂号', type: 'success' },
  { value: 'IN_PROGRESS', label: '诊疗中', type: 'primary' },
  { value: 'CANCELLED', label: '已退号', type: 'info' }
]
