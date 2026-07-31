import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export type OrderTemplateStatus = 'ENABLED' | 'DISABLED'

export interface OrderTemplateRecord {
  id: string
  code: string
  name: string
  templateType: 'GENERAL' | 'TCM' | 'PRICING'
  departmentName: string
  itemSummary: string
  status: OrderTemplateStatus
  statusText: string
  description: string
}

export interface OrderTemplateQuery extends BasicPageQuery {}
export type OrderTemplateSavePayload = Omit<OrderTemplateRecord, 'id' | 'statusText'>
export interface OrderTemplateNameOption {
  label: string
  value: string
}
