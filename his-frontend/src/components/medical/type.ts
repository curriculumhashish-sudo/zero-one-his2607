export type MedicalKey = string | number
export interface AdviceOrder {
  key: MedicalKey
  name: string
  specification?: string
  dose?: string | number
  unit?: string
  frequency?: string
  route?: string
  days?: string | number
  remark?: string
  status?: string
  group?: string
  raw?: unknown
}
export interface AdviceColumn { prop: string; label: string; width?: string | number; minWidth?: string | number; formatter?: (row: AdviceOrder) => string; align?: 'left' | 'center' | 'right' }
