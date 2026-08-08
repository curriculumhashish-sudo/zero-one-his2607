import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface ExecuteLocationRecord {
  id: string
  organizationId: string
  itemTable: string
  distributionCategoryCode: string
  defLocationId: string
  priority: number
  itemDefinitionId?: string
  methodCode?: string
  startTime?: string
  endTime?: string
  displayOrder?: number
  remark?: string
  defLocationName?: string
}

export interface ExecuteLocationQuery extends BasicPageQuery {
  itemTable?: string
}

export type ExecuteLocationSavePayload = Omit<ExecuteLocationRecord, 'id' | 'defLocationName'>

