/** 后端 字典列表查询的返回原始字段 */
export interface RawDictTypeItem {
  codeSystemId: number // 代码体系id
  dicCode: string // 字典编码
  dicName: string // 字典名称
  dicDescription: string // 字典描述
  dicSort: number // 字典排序
  dicId: string // 字典序号
}

/** 后端 /dictionarytype/queryall 返回的字典类型项 */
export interface RawDictTypeAllItem {
  dictTypeId: string // 字典类型ID
  dictTypeName: string // 字典类型名称
  dictTypeCode: string // 字典类型编码
}

/** 后端 /sys/query-all-enum-type 返回的枚举类型项 */
export interface RawEnumTypeItem {
  enumTypeKey: string
  displayName: string
  description: string
  isSystem: boolean
  status: number
  id: string
  tenantId: string
  createdAt: string
  updatedAt: string
  createdBy: string
  updatedBy: string
}

/** 后端 /sys/query-all-enum 返回的枚举值项 */
export interface RawEnumItem {
  enumTypeId: string
  enumKey: string
  enumCode: number
  display: string
  description: string
  order: number
  status: number
  id: string
}

/** 字典 / 枚举类型记录（左侧列表项） */
export interface CodeTypeRecord {
  id: string
  name: string
  title: string
  description: string
  codeSystemId?: string
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
  pageIndex?: number
  pageSize?: number
}

/** 字典 / 枚举编码项查询参数 */
export interface CodeItemQuery {
  typeId: string
  keyword?: string
  name?: string
  system: 'dict' | 'enum'
  /** 字典类型编码，用于后端接口过滤 */
  dictCode?: string
  /** 字典体系ID（/dictionarytype/queryall 返回的 dictTypeId） */
  codeSystemId?: string
  pageIndex?: number
  pageSize?: number
}

/** 字典 / 枚举类型保存参数 */
export interface CodeTypeSavePayload {
  name: string
  title: string
  description: string
  codeSystemId?: string | number
}

/** 字典 / 枚举编码项保存参数 */
export interface CodeItemSavePayload {
  code: string
  name: string
  description: string
  sort: number
}
