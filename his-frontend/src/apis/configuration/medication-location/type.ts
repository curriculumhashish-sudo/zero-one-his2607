import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface MedicationLocationRuleRecord {
  id: string
  organizationId: string
  itemTable: string
  distributionCategoryCode: string
  defLocationId: string
  itemDefinitionId?: string
  methodCode?: string
  priority: number
  displayOrder?: number
  startTime?: string
  endTime?: string
  remark?: string
  organizationName?: string
  defLocationName?: string
  itemDefinitionName?: string
}

export interface MedicationLocationRuleQuery extends BasicPageQuery {
  organizationId: string
}

export type MedicationLocationRuleSavePayload = {
  organizationId: string
  checkedItems: MedicationLocationRuleRecord[]
}

