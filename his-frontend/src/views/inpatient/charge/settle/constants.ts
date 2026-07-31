import type { StatusDisplayOption } from '@/components/common/type'

export const SETTLE_LINE_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'PENDING', label: '待结算', type: 'warning' },
  { value: 'SETTLED', label: '已结算', type: 'success' }
]

export const SETTLEMENT_RECORD_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'SUCCESS', label: '成功', type: 'success' },
  { value: 'FAILED', label: '失败', type: 'danger' }
]
