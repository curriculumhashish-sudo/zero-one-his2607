import { useHttp, DataUpType } from '@/plugins/http'
import {
  mockCreateCodeItem,
  mockCreateCodeType,
  mockLoadCodeOptions,
  mockPageCodeItems,
  mockPageCodeTypes,
  mockRemoveCodeItem,
  mockRemoveCodeType,
  mockUpdateCodeItem,
  mockUpdateCodeType
} from './mock'
import type {
  CodeTypeRecord,
  CodeItemRecord,
  RawDictTypeItem,
  RawDictTypeAllItem,
  RawEnumTypeItem,
  CodeItemQuery,
  CodeItemSavePayload,
  CodeTypeQuery,
  CodeTypeSavePayload
} from './type'

/** 基础数据模块 baseURL */
const baseURL = '/j2-basedata'

/**
 * 获取字典类型列表（条件 + 分页）—— 左侧面板
 * 对接后端 GET /j2-basedata/dictionarytype/queryall
 */
export const pageDictionaryTypes = async (
  query: Omit<CodeTypeQuery, 'system'>
): Promise<{ rows: CodeTypeRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.get<{
      rows?: RawDictTypeAllItem[]
      total?: number
    }>(
      '/dictionarytype/queryall',
      {
        dictKeyword: query.keyword,
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 10
      },
      { baseURL }
    )
    const payload = res.data
    const rows: CodeTypeRecord[] = (payload?.rows ?? []).map((item) => ({
      id: item.dictTypeId,
      name: item.dictTypeCode,
      title: item.dictTypeName,
      description: '',
      codeSystemId: undefined,
      createdAt: '',
      updatedAt: ''
    }))
    return {
      rows,
      total: payload?.total ?? rows.length
    }
  } catch {
    const rows = await mockPageCodeTypes({ ...query, system: 'dict' })
    return { rows, total: rows.length }
  }
}

/**
 * 新增字典类型（左侧面板新增）
 * 对接后端 POST /j2-basedata/dictionarytype/addConcept
 */
export const createDictionaryType = async (data: CodeTypeSavePayload) => {
  try {
    const http = useHttp()
    await http.post(
      '/dictionarytype/addConcept',
      {
        name: data.name,
        title: data.title,
        description: data.description
      },
      { baseURL, upType: DataUpType.json }
    )
  } catch {
    return mockCreateCodeType('dict', data)
  }
}

export const updateDictionaryType = (id: string, data: CodeTypeSavePayload) =>
  mockUpdateCodeType('dict', id, data)

export const removeDictionaryType = (id: string) => mockRemoveCodeType('dict', id)

// ---- 字典选项 ----

export const loadDictionaryOptions = (keyword?: string) => mockLoadCodeOptions('dict', keyword)

// ---- 字典项 ----

/**
 * 获取字典编码项列表（右侧表格数据）
 * 对接后端 GET /j2-basedata/dictionary/queryDictItem
 */
export const pageDictionaryItems = async (
  query: Omit<CodeItemQuery, 'system'>
): Promise<CodeItemRecord[]> => {
  try {
    const http = useHttp()
    const res = await http.get<{
      rows?: RawDictTypeItem[]
      total?: number
    }>(
      '/dictionary/queryDictItem',
      {
        dictCode: query.dictCode ?? query.typeId,
        dicitemKeyword: query.keyword,
        pageIndex: 1,
        pageSize: 100
      },
      { baseURL }
    )
    const payload = res.data
    return (payload?.rows ?? []).map((item) => ({
      id: item.dicId,
      typeId: query.typeId,
      code: item.dicCode,
      name: item.dicName,
      description: item.dicDescription ?? '',
      sort: item.dicSort ?? 0,
      createdAt: '',
      updatedAt: ''
    }))
  } catch {
    return mockPageCodeItems({ ...query, system: 'dict' })
  }
}

/**
 * 新增字典编码项（右侧表格新增）
 * 对接后端 POST /j2-basedata/dictionary/addDictItem
 */
export const createDictionaryItem = async (typeId: string, data: CodeItemSavePayload) => {
  try {
    const http = useHttp()
    await http.post(
      '/dictionary/addDictItem',
      {
        codeSystemId: 1,
        dicCode: data.code,
        dicName: data.name,
        dicDescription: data.description,
        dicSort: data.sort
      },
      { baseURL, upType: DataUpType.json }
    )
  } catch {
    return mockCreateCodeItem('dict', typeId, data)
  }
}

/**
 * 修改字典编码项（右侧表格编辑）
 * 对接后端 PUT /j2-basedata/dictionary/dictItem
 */
export const updateDictionaryItem = async (id: string, data: CodeItemSavePayload) => {
  try {
    const http = useHttp()
    await http.put(
      '/dictionary/dictItem',
      {
        id: Number(id),
        code: data.code,
        dictName: data.name,
        description: data.description,
        sort: data.sort
      },
      { baseURL, upType: DataUpType.json }
    )
  } catch {
    return mockUpdateCodeItem('dict', id, data)
  }
}

/**
 * 删除字典编码项（右侧表格删除）
 * 对接后端 DELETE /j2-basedata/dictionary/dictItem/{id}
 */
export const removeDictionaryItem = async (id: string) => {
  try {
    const http = useHttp()
    await http.delete(`/dictionary/dictItem/${id}`, undefined, { baseURL })
  } catch {
    return mockRemoveCodeItem('dict', id)
  }
}

// ---- 枚举类型 ----

/**
 * 获取枚举类型列表（条件 + 分页）—— 左侧面板
 * 对接后端 GET /j2-basedata/sys/query-all-enum-type
 */
export const pageEnumTypes = async (
  query: Omit<CodeTypeQuery, 'system'>
): Promise<{ rows: CodeTypeRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.get<{
      rows?: RawEnumTypeItem[]
      total?: number
    }>(
      '/sys/query-all-enum-type',
      {
        enumTypeKeyword: query.keyword,
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 10
      },
      { baseURL }
    )
    const payload = res.data
    const rows: CodeTypeRecord[] = (payload?.rows ?? []).map((item) => ({
      id: item.id,
      name: item.enumTypeKey,
      title: item.displayName,
      description: item.description ?? '',
      codeSystemId: undefined,
      createdAt: item.createdAt ?? '',
      updatedAt: item.updatedAt ?? ''
    }))
    return { rows, total: payload?.total ?? rows.length }
  } catch {
    const rows = await mockPageCodeTypes({ ...query, system: 'enum' })
    return { rows, total: rows.length }
  }
}

/**
 * 创建枚举类型（左侧面板新增）
 * 对接后端 POST /j2-basedata/sys/add-enum-type
 */
export const createEnumType = async (data: CodeTypeSavePayload) => {
  try {
    const http = useHttp()
    await http.post(
      '/sys/add-enum-type',
      {
        enumTypeKey: data.title,
        displayName: data.name,
        description: data.description,
        isSystem: false,
        status: 0
      },
      { baseURL, upType: DataUpType.json }
    )
  } catch {
    return mockCreateCodeType('enum', data)
  }
}

export const updateEnumType = (id: string, data: CodeTypeSavePayload) =>
  mockUpdateCodeType('enum', id, data)

export const removeEnumType = (id: string) => mockRemoveCodeType('enum', id)

// ---- 枚举选项 ----

export const loadEnumOptions = (keyword?: string) => mockLoadCodeOptions('enum', keyword)

// ---- 枚举项 ----

export const pageEnumItems = (query: Omit<CodeItemQuery, 'system'>) =>
  mockPageCodeItems({ ...query, system: 'enum' })

export const createEnumItem = (typeId: string, data: CodeItemSavePayload) =>
  mockCreateCodeItem('enum', typeId, data)

export const updateEnumItem = (id: string, data: CodeItemSavePayload) =>
  mockUpdateCodeItem('enum', id, data)

export const removeEnumItem = (id: string) => mockRemoveCodeItem('enum', id)

export type * from './type'
