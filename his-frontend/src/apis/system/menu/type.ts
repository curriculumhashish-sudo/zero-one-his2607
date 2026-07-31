import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface MenuRecord {
  id: string
  name: string
  parentName: string
  path: string
  component: string
  icon: string
  sortNo: number
  visible: boolean
  enabled: boolean
  description: string
}

export interface MenuQuery extends BasicPageQuery {}
export type MenuSavePayload = Omit<MenuRecord, 'id'>
export interface MenuNameOption {
  label: string
  value: string
}
