import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface OrderTemplateLine {
  id?: string
  orderTemplateId?: string
  lineKindEnum: string
  medicationDefinitionId?: string
  activityDefinitionId?: string
  deviceDefinitionId?: string
  groupKey?: string
  sortOrder: number
  defaultQuantity?: number
  defaultUnitCode?: string
  defaultDays?: number
  frequencyId?: string
  routeCode?: string
  dosageText?: string
  defaultPerformOrgId?: string
  lineRemark?: string
  chineseHerbsDoseQuantity?: number
  sufferingFlag?: boolean
}

export interface OrderTemplateRecord {
  id: string
  objectId?: string
  name: string
  bindingEnum: string
  sceneEnum: string
  templateKindEnum: string
  templateCode?: string
  description?: string
  sortWeight?: number
  remark?: string
  defaultChineseHerbsDoseQuantity?: number
  defaultSufferingFlag?: boolean
  statusEnum?: string
  templateSourceEnum?: string
  createBy?: string
  createTime?: string
  lines?: OrderTemplateLine[]
}

export interface OrderTemplateQuery extends BasicPageQuery {
  name?: string
  bindingEnum?: string
  statusEnum?: string
  templateKindEnum?: string
}

export interface OrderTemplateSavePayload extends Omit<OrderTemplateRecord, 'id' | 'statusEnum' | 'templateSourceEnum' | 'createBy' | 'createTime'> {
  lines: OrderTemplateLine[]
}

export interface OrderTemplateEnvelope {
  header: Omit<OrderTemplateSavePayload, 'lines'> & { id?: string }
  lines: OrderTemplateLine[]
}

