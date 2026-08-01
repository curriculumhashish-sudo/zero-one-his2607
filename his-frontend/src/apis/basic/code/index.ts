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
import type { CodeItemQuery, CodeItemSavePayload, CodeTypeQuery, CodeTypeSavePayload } from './type'

// ---- 字典类型 ----

export const pageDictionaryTypes = (query: Omit<CodeTypeQuery, 'system'>) =>
  mockPageCodeTypes({ ...query, system: 'dict' })

export const createDictionaryType = (data: CodeTypeSavePayload) =>
  mockCreateCodeType('dict', data)

export const updateDictionaryType = (id: string, data: CodeTypeSavePayload) =>
  mockUpdateCodeType('dict', id, data)

export const removeDictionaryType = (id: string) => mockRemoveCodeType('dict', id)

// ---- 字典选项 ----

export const loadDictionaryOptions = (keyword?: string) => mockLoadCodeOptions('dict', keyword)

// ---- 字典项 ----

export const pageDictionaryItems = (query: Omit<CodeItemQuery, 'system'>) =>
  mockPageCodeItems({ ...query, system: 'dict' })

export const createDictionaryItem = (typeId: string, data: CodeItemSavePayload) =>
  mockCreateCodeItem('dict', typeId, data)

export const updateDictionaryItem = (id: string, data: CodeItemSavePayload) =>
  mockUpdateCodeItem('dict', id, data)

export const removeDictionaryItem = (id: string) => mockRemoveCodeItem('dict', id)

// ---- 枚举类型 ----

export const pageEnumTypes = (query: Omit<CodeTypeQuery, 'system'>) =>
  mockPageCodeTypes({ ...query, system: 'enum' })

export const createEnumType = (data: CodeTypeSavePayload) => mockCreateCodeType('enum', data)

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
