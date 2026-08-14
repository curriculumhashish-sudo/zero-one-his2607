import type { StatusDisplayOption } from '@/components/common/type'

export const DISPLAY_MODES = [
  { value: 'prescription', label: '按处方' },
  { value: 'encounter', label: '按就诊' },
  { value: 'flat', label: '平铺' }
] as const

export const RECORD_TYPE_OPTIONS: StatusDisplayOption[] = [
  { value: 'DISPENSE', label: '发药', type: 'success', effect: 'light' },
  { value: 'REFUND', label: '退药', type: 'warning', effect: 'light' }
]

export function getRecordTypeLabel(type: string): string {
  return RECORD_TYPE_OPTIONS.find((o) => o.value === type)?.label || type
}

export function getRecordTypeStatus(type: string): StatusDisplayOption {
  return (
    RECORD_TYPE_OPTIONS.find((o) => o.value === type) || {
      value: type,
      label: type,
      type: 'info'
    }
  )
}