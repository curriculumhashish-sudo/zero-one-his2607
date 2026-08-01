/** 字典 / 枚举类型记录（左侧列表项） */
export interface CodeTypeRecord {
  id: string
  name: string
  title: string
  version: string
  description: string
  createdAt: string
  updatedAt: string
}

/** 字典 / 枚举编码项记录（右侧表格行） */
export interface CodeItemRecord {
  id: string
  typeId: string
  code: string
  name: string
  description: string
  sort: number
  createdAt: string
  updatedAt: string
}

/** 字典 / 枚举类型查询参数 */
export interface CodeTypeQuery {
  keyword?: string
  system: 'dict' | 'enum'
}

/** 字典 / 枚举编码项查询参数 */
export interface CodeItemQuery {
  typeId: string
  keyword?: string
  name?: string
  system: 'dict' | 'enum'
}

/** 字典 / 枚举类型保存参数 */
export interface CodeTypeSavePayload {
  name: string
  title: string
  version: string
  description: string
}

/** 字典 / 枚举编码项保存参数 */
export interface CodeItemSavePayload {
  code: string
  name: string
  description: string
  sort: number
}
