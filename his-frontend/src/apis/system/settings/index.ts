import { type JsonVO } from '@/apis/http'
import { createJ2HttpClient } from '@/apis/j2-http'
import {
  mockGetProviderKeys,
  mockInitSetting
} from './mock'
import type {
  ProviderKeyOption,
  SettingDefinition,
  SettingInitResult,
  SettingRecord,
  SettingSavePayload,
  SettingUpdatePayload
} from './type'

/**
 * J2 系统管理 - 配置项服务客户端
 * 后端接口约定（context-path: /j2-sysmgr）：
 *  - GET    /settings/definitions        获取配置定义列表（分页）
 *  - GET    /settings                    获取配置项列表（分页）
 *  - POST   /settings                    新增配置项
 *  - PUT    /settings/setting-update     更新配置项
 *  - DELETE /settings/setting-delete     删除配置项
 *
 * 注意：后端 DTO 使用 snake_case（provider_name / provider_key），
 *      前端请求/响应需要做字段映射。
 */
const settingsHttp = createJ2HttpClient('/j2-sysmgr')

/** 后端 SettingDefinitionDTO */
interface BackendDefinitionDTO {
  id?: string
  description?: string // 后端语义：标题
  name?: string
}

/** 后端 SettingsDTO */
interface BackendSettingsDTO {
  id?: string
  name?: string
  description?: string
  provider_name?: string
  provider_key?: string
  value?: string
  createdAt?: string
  updatedAt?: string
}

/** 后端 SettingsAddDTO */
interface BackendSettingsAddDTO {
  name: string
  provider_name: string
  provider_key: string
  value: string
}

/** 后端 SettingUpdateDTO */
interface BackendSettingUpdateDTO {
  id: string
  value: string
}

/** 后端 SettingDeleteDTO */
interface BackendSettingDeleteDTO {
  id: string
}

/** 后端 PageDTO<T> */
interface BackendPageDTO<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
}

function unwrap<T>(promise: Promise<JsonVO<T>>): Promise<T> {
  return promise.then((res) => {
    if (res.code !== 10000 || res.data === undefined) {
      throw new Error(res.message || '配置项请求失败')
    }
    return res.data
  })
}

function toDefinition(item: BackendDefinitionDTO): SettingDefinition {
  return {
    id: String(item.id ?? ''),
    // 后端 description 字段语义为"标题"，前端用 title 展示
    title: item.description ?? '',
    name: item.name ?? '',
    description: '',
    remark: ''
  }
}

function toSettingRecord(item: BackendSettingsDTO): SettingRecord {
  return {
    id: Number(item.id) || 0,
    name: item.name ?? '',
    description: item.description ?? '',
    value: item.value ?? '',
    providerName: item.provider_name ?? '',
    providerKey: item.provider_key ?? '',
    createdAt: item.createdAt ?? '',
    updatedAt: item.updatedAt ?? ''
  }
}

/**
 * 获取配置定义列表
 * 后端为分页接口，前端目前一次性加载全部定义（取较大 pageSize）
 */
export async function initSettingDefinitions(): Promise<SettingDefinition[]> {
  const page = await unwrap(
    settingsHttp.get<BackendPageDTO<BackendDefinitionDTO>>('/settings/definitions', {
      pageIndex: 1,
      pageSize: 200,
      settingDefinitionKeywords: ''
    })
  )
  return (page.rows ?? []).map(toDefinition)
}

/**
 * 获取提供者选项
 * 后端暂未提供统一接口，沿用前端固定列表
 */
export const initSetting = (): Promise<SettingInitResult> => mockInitSetting()

/**
 * 获取提供者键选项（用户/角色/租户列表）
 * 后端暂未提供统一接口，沿用前端 mock
 */
export const getProviderKeys = (providerName: string): Promise<ProviderKeyOption[]> =>
  mockGetProviderKeys(providerName)

/**
 * 获取配置项列表
 * @param name 配置定义名称，用于按 name 精确过滤
 */
export async function pageSettings(name: string): Promise<SettingRecord[]> {
  const page = await unwrap(
    settingsHttp.get<BackendPageDTO<BackendSettingsDTO>>('/settings', {
      pageIndex: 1,
      pageSize: 200,
      name
    })
  )
  return (page.rows ?? []).map(toSettingRecord)
}

/**
 * 新增配置项
 */
export async function createSetting(data: SettingSavePayload): Promise<SettingRecord> {
  const payload: BackendSettingsAddDTO = {
    name: data.name,
    provider_name: data.providerName,
    provider_key: data.providerKey || '',
    value: data.value
  }
  await unwrap(settingsHttp.post<string>('/settings', payload))
  // 后端仅返回提示字符串，前端构造一个临时记录用于刷新列表
  return {
    id: 0,
    name: data.name,
    description: data.description,
    value: data.value,
    providerName: data.providerName,
    providerKey: data.providerKey,
    createdAt: '',
    updatedAt: ''
  }
}

/**
 * 更新配置项
 */
export async function updateSetting(
  id: number,
  data: SettingUpdatePayload
): Promise<SettingRecord> {
  const payload: BackendSettingUpdateDTO = {
    id: String(id),
    value: data.value ?? ''
  }
  await unwrap(settingsHttp.put<string>('/settings/setting-update', payload))
  return {
    id,
    name: '',
    description: data.description ?? '',
    value: data.value ?? '',
    providerName: '',
    providerKey: '',
    createdAt: '',
    updatedAt: ''
  }
}

/**
 * 删除配置项
 */
export async function deleteSetting(id: number): Promise<void> {
  const payload: BackendSettingDeleteDTO = { id: String(id) }
  await unwrap(settingsHttp.delete<string>('/settings/setting-delete', payload))
}

export type * from './type'
