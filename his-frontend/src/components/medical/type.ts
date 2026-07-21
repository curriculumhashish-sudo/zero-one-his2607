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
  /** 药房/发药点，用于题名括注 */
  pharmacy?: string
  remark?: string
  status?: string
  group?: string
  /**
   * 成组子药列表：存在且非空时，C55 以「成组括线」版式展示，
   * 共用用法/频次/天数取首项；缺省或空数组时按单条医嘱展示。
   */
  groupItems?: AdviceOrder[]
  raw?: unknown
}
export interface AdviceColumn { prop: string; label: string; width?: string | number; minWidth?: string | number; formatter?: (row: AdviceOrder) => string; align?: 'left' | 'center' | 'right' }
