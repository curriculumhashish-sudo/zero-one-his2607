import { type JsonVO } from '@/apis/http'
import { createJ2HttpClient } from '@/apis/j2-http'
import { mockSearchAvailableUsers } from './mock'
import type {
  TenantAddUsersPayload,
  TenantOption,
  TenantPageResult,
  TenantQuery,
  TenantRecord,
  TenantRemoveUsersPayload,
  TenantSavePayload,
  TenantUserPageResult,
  TenantUserQuery,
  TenantUserRecord
} from './type'

/**
 * J2 系统管理 - 租户/租户用户服务客户端
 * 后端接口约定（context-path: /j2-sysmgr）：
 *  租户管理：
 *   - GET    /rent/query-all          获取租户列表（条件+分页）
 *   - GET    /rent/query-name         获取租户名称（关键字搜索）
 *   - POST   /rent/addRent            新增租户
 *   - PUT    /rent/updateRent/{id}    修改租户
 *   - DELETE /rent/deleteRent/{id}    删除租户
 *  租户用户管理：
 *   - GET    /sysmgr/tenant-user/list-tenant-users    获取租户用户列表
 *   - POST   /sysmgr/tenant-user/add-tenant-users    添加租户用户
 *   - POST   /sysmgr/tenant-user/remove-tenant-users 移除租户用户
 */
const tenantHttp = createJ2HttpClient('/j2-sysmgr')

/** 后端 RentDTO2 字段（继承自 RentNameDTO） */
interface BackendRentDTO {
  id?: string
  name?: string
  code?: string
  description?: string
  is_active?: number // 后端 Integer：1-启用 0-禁用
}

/** 后端 RentNameDTO 字段 */
interface BackendRentNameDTO {
  id?: string
  name?: string
  code?: string
}

/** 后端 RentAddDTO 字段 */
interface BackendRentAddDTO {
  tenantCode: string
  tenantName: string
  tenantStatus: string
  tenantDescription?: string
}

/** 后端 RentDTO（修改）字段 */
interface BackendRentUpdateDTO {
  id?: string
  tenantName: string
  tenantStatus: string
  tenantDescription?: string
}

/** 后端 TenantUserDTO 字段 */
interface BackendTenantUserDTO {
  id?: string | number
  tenantId?: string
  username?: string
  name?: string
  email?: string
  phone?: string
  roleNames?: string[]
}

/** 后端 AddTenantUserDTO.users 元素 */
interface BackendTenantUserItem {
  username: string
  name: string
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
      throw new Error(res.message || '租户服务请求失败')
    }
    return res.data
  })
}

/** 后端 is_active (Integer 1/0) → 前端 isActive (string 'true'/'false') */
function toIsActiveString(value: number | string | undefined): string {
  if (value === undefined || value === null) return 'true'
  return String(value) === '1' || String(value).toLowerCase() === 'true' ? 'true' : 'false'
}

/** 前端 isActive (string 'true'/'false') → 后端 tenantStatus (string '1'/'0') */
function toTenantStatus(isActive: string): string {
  return isActive === 'true' || isActive === '1' ? '1' : '0'
}

function toTenantRecord(item: BackendRentDTO): TenantRecord {
  return {
    id: String(item.id ?? ''),
    code: item.code ?? '',
    name: item.name ?? '',
    description: item.description ?? '',
    extraProperties: '',
    isActive: toIsActiveString(item.is_active),
    // 后端暂未返回以下字段
    createdAt: '',
    updatedAt: '',
    createdBy: '',
    updatedBy: ''
  }
}

function toTenantOption(item: BackendRentNameDTO): TenantOption {
  return {
    id: String(item.id ?? ''),
    name: item.name ?? '',
    code: item.code ?? ''
  }
}

/**
 * 将后端 TenantUserDTO 转换为前端 TenantUserRecord
 */
function toTenantUserRecord(item: BackendTenantUserDTO): TenantUserRecord {
  const username = item.username ?? ''
  return {
    id: item.id != null ? String(item.id) : username,
    userName: username,
    name: item.name ?? '',
    email: item.email ?? '',
    phone: item.phone ?? '',
    roleNames: item.roleNames ?? []
  }
}

/**
 * 获取租户列表（条件+分页）
 */
export async function pageTenants(query: TenantQuery): Promise<TenantPageResult> {
  const params: Record<string, string | number | undefined> = {
    pageIndex: query.page,
    pageSize: query.pageSize
  }
  if (query.code) params.code = query.code
  if (query.keyword) params.rentKeywords = query.keyword
  if (query.isActive) params.isActive = query.isActive === 'true' ? 1 : 0

  const page = await unwrap(
    tenantHttp.get<BackendPageDTO<BackendRentDTO>>('/rent/query-all', params)
  )
  return {
    list: (page.rows ?? []).map(toTenantRecord),
    total: Number(page.total ?? 0)
  }
}

/**
 * 获取启用的租户选项列表（用于下拉选择）
 */
export async function loadTenantOptions(): Promise<TenantOption[]> {
  const page = await unwrap(
    tenantHttp.get<BackendPageDTO<BackendRentNameDTO>>('/rent/query-name', {
      keyword: ''
    })
  )
  return (page.rows ?? []).map(toTenantOption)
}

/**
 * 新增租户
 */
export async function createTenant(data: TenantSavePayload): Promise<TenantRecord> {
  const payload: BackendRentAddDTO = {
    tenantCode: data.code || '',
    tenantName: data.name,
    tenantStatus: toTenantStatus(data.isActive),
    tenantDescription: data.description
  }
  await unwrap(tenantHttp.post<string>('/rent/addRent', payload))
  return {
    id: '',
    code: data.code || '',
    name: data.name,
    description: data.description ?? '',
    extraProperties: data.extraProperties,
    isActive: data.isActive,
    createdAt: '',
    updatedAt: '',
    createdBy: '',
    updatedBy: ''
  }
}

/**
 * 修改租户
 * 注意：后端 RentDTO 不包含 tenantCode，无法修改租户编码
 */
export async function updateTenant(
  id: string,
  data: TenantSavePayload
): Promise<TenantRecord> {
  const payload: BackendRentUpdateDTO = {
    id,
    tenantName: data.name,
    tenantStatus: toTenantStatus(data.isActive),
    tenantDescription: data.description
  }
  await unwrap(tenantHttp.put<string>(`/rent/updateRent/${id}`, payload))
  return {
    id,
    code: data.code || '',
    name: data.name,
    description: data.description ?? '',
    extraProperties: data.extraProperties,
    isActive: data.isActive,
    createdAt: '',
    updatedAt: '',
    createdBy: '',
    updatedBy: ''
  }
}

/**
 * 删除租户
 */
export async function removeTenant(id: string): Promise<void> {
  await unwrap(tenantHttp.delete<string>(`/rent/deleteRent/${id}`))
}

/**
 * 获取租户用户列表
 */
export async function pageTenantUsers(
  tenantId: string,
  query: TenantUserQuery
): Promise<TenantUserPageResult> {
  const params: Record<string, string | number | undefined> = {
    pageIndex: query.page,
    pageSize: query.pageSize,
    newTenant: tenantId
  }
  if (query.keyword) {
    // 后端支持 userName / name 两个字段过滤，前端统一用 keyword 模糊匹配
    params.userName = query.keyword
    params.name = query.keyword
  }

  const page = await unwrap(
    tenantHttp.get<BackendPageDTO<BackendTenantUserDTO>>(
      '/sysmgr/tenant-user/list-tenant-users',
      params
    )
  )
  return {
    list: (page.rows ?? []).map(toTenantUserRecord),
    total: Number(page.total ?? 0)
  }
}

/**
 * 添加租户用户
 */
export async function addTenantUsers(payload: TenantAddUsersPayload): Promise<void> {
  const body = {
    tenantId: payload.tenantId,
    users: payload.users.map<BackendTenantUserItem>((u) => ({
      username: u.username,
      name: u.name
    }))
  }
  await unwrap(tenantHttp.post<string>('/sysmgr/tenant-user/add-tenant-users', body))
}

/**
 * 移除租户用户
 */
export async function removeTenantUsers(payload: TenantRemoveUsersPayload): Promise<void> {
  const body = {
    tenantId: payload.tenantId,
    usernames: payload.usernames
  }
  await unwrap(tenantHttp.post<string>('/sysmgr/tenant-user/remove-tenant-users', body))
}

/**
 * 搜索可添加的用户
 * 后端暂未提供统一的用户搜索接口，沿用前端 mock
 */
export const searchAvailableUsers = (keyword: string): Promise<TenantUserRecord[]> =>
  mockSearchAvailableUsers(keyword)

export type * from './type'
