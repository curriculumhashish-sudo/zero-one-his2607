import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface ExecuteLocationRecord {
  id: string
  departmentName: string
  itemCode: string
  itemName: string
  serviceCategory: string
  startDate: string
  endDate: string
  description: string
}

export interface ExecuteLocationQuery extends BasicPageQuery {}
export type ExecuteLocationSavePayload = Omit<ExecuteLocationRecord, 'id'>
export interface ExecuteDepartmentOption {
  label: string
  value: string
}
