import type { BasicPageQuery, BasicPageResult } from '@/apis/basic/crud-mock'

export interface MedicationLocationRuleRecord {
  id: string
  departmentName: string
  itemType: 'DRUG' | 'HERBAL' | 'DEVICE'
  itemName: string
  pharmacyName: string
  routeName: string
  priority: number
  enabled: boolean
}

export interface MedicationLocationRuleQuery extends BasicPageQuery {}
export type MedicationLocationRuleSavePayload = MedicationLocationRuleRecord[]
export type MedicationLocationRulePageResult = BasicPageResult<MedicationLocationRuleRecord>
