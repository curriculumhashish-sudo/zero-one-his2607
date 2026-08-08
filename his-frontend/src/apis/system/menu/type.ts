import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface MenuRecord {
  id: string
  name: string
  parentName: string
  parentId?: string
  menuId?: string
  menuName?: string
  displayName?: string
  showName?: string
  code?: string
  path: string
  component: string
  icon: string
  sortNo: number
  visible: boolean
  enabled: boolean
  description: string
  remark?: string
  permissionName?: string
  menuType?: string
  sortNum?: number
  status?: number | string
  hidden?: boolean
  external?: boolean
  externalUrl?: string
  children?: MenuRecord[]
}

export interface MenuQuery extends BasicPageQuery {}
export type MenuSavePayload = Omit<MenuRecord, 'id'>
export interface MenuNameOption {
  label: string
  value: string
}
