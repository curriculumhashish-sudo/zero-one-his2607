import HttpClient, { DataUpType } from '@/apis/http'
import {
  mockGetCaptcha,
  mockLoadAvailableDepartments,
  mockLoadAvailableHospitals,
  mockLoadAvailableTenants,
  mockLoadCurrentMenus,
  mockLoadCurrentUser,
  mockLoginAccount,
  mockLogout,
  mockSelectDepartment,
  mockSelectHospital,
  mockSelectTenant,
  mockVerifyCaptcha
} from './mock'
import type {
  CaptchaResult,
  CaptchaVerifyData,
  LoginOrganizationOption,
  LoginParams,
  LoginResult,
  OrganizationInfo,
  RouteMenuRecord,
  TenantInfo,
  UserInfo
} from './type'

const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_LOGIN_MOCK === 'true' || import.meta.env.VITE_USE_MOCK === 'true'
let captchaStore: { id: string; imageBase64?: string } | null = null

export const getCaptchaImage = () => captchaStore?.imageBase64 ?? ''
export const getCaptchaUuid = () => captchaStore?.id ?? ''

const authHttp = new HttpClient(
  // 登录服务单独走自己的代理前缀，避免被通用 /api 网关代理改写。
  { baseURL: import.meta.env.VITE_AUTH_SERVER_URL || import.meta.env.VITE_LOGIN_API_URL || import.meta.env.VITE_API_URL || '/api' },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token')
      const tokenHead = (typeof localStorage === 'undefined' ? 'Bearer' : (localStorage.getItem('tokenHead') || 'Bearer')).trim()
      const requestPath = String(config.url ?? '')
      const isPublicAuthRequest = ['/login', '/auth/captcha/generate', '/auth/captcha/verify'].some((path) =>
        requestPath.endsWith(path) || requestPath.includes(`${path}?`)
      )
      if (token && !isPublicAuthRequest) config.headers.Authorization = tokenHead ? `${tokenHead} ${token}` : token
      config.headers.Accept = 'application/json'
      if (config.upType === DataUpType.json) {
        config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      }
      return config
    }
  }
)

type BackendEnvelope<T> = { code?: number; message?: string; data?: T }

async function unwrap<T>(request: Promise<BackendEnvelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '登录服务请求失败')
  }
  const data = response.data === undefined ? response as T : response.data
  if (data === undefined || data === null) {
    throw new Error(response.message || '登录服务未返回数据')
  }
  return data
}

function toCaptchaResult(data: { uuid?: string; id?: string; imageBase64?: string }): CaptchaResult {
  const id = data.id ?? data.uuid
  if (!id) throw new Error('登录服务未返回验证码编号')
  return { id, imageBase64: data.imageBase64 }
}

function parseJsonWithSafeIds(data: unknown) {
  if (typeof data !== 'string') return data

  // JavaScript 无法精确表示超过安全整数范围的 ID，先将 JSON 中的 ID 数字转成字符串。
  const safeJson = data.replace(/("[^"]*(?:id|Id|ID)"\s*:\s*)(-?\d{16,})(?=\s*[,}])/g, '$1"$2"')
  return JSON.parse(safeJson)
}

function toTenant(data: any): TenantInfo {
  return {
    id: data.id == null ? '' : data.id,
    name: String(data.name ?? data.displayName ?? ''),
    code: String(data.code ?? ''),
    description: String(data.description ?? ''),
    isActive: Boolean(data.isActive ?? data.enabled ?? true),
    extraProperties: null,
    createdAt: String(data.createdAt ?? ''),
    updatedAt: String(data.updatedAt ?? ''),
    createdBy: String(data.createdBy ?? ''),
    updatedBy: String(data.updatedBy ?? '')
  }
}

function toHospital(data: any): LoginOrganizationOption {
  return {
    id: String(data.id ?? data.hospitalId ?? data.organizationId ?? ''),
    code: data.code ?? data.hospitalCode ?? data.organizationCode,
    displayName: data.displayName ?? data.name ?? data.hospitalName ?? data.organizationName,
    name: data.name ?? data.displayName ?? data.hospitalName ?? data.organizationName
  }
}

function toDepartment(data: any): OrganizationInfo {
  return {
    children: null,
    code: String(data.code ?? ''),
    createdAt: String(data.createdAt ?? ''),
    displayName: String(data.displayName ?? data.name ?? ''),
    id: String(data.id ?? ''),
    isEnabled: data.isEnabled === 0 || data.isEnabled === '0' || data.enabled === 0 || data.enabled === '0' ? false : Boolean(data.isEnabled ?? data.enabled ?? true),
    parentDisplayName: null,
    parentId: data.parentId == null ? null : String(data.parentId),
    remark: data.remark == null ? null : String(data.remark),
    sortOrder: Number(data.sortOrder ?? 0),
    tenantId: String(data.tenantId ?? ''),
    updatedAt: data.updatedAt == null ? null : String(data.updatedAt)
  }
}

function toUserInfo(data: any): UserInfo {
  const userId = String(data.userId ?? data.id ?? '')
  const realName = String(data.realName ?? data.name ?? data.username ?? '')
  return {
    avatar: String(data.avatar ?? ''),
    realName,
    roles: Array.isArray(data.roles) ? data.roles.map(String) : [],
    userId,
    username: String(data.username ?? data.userName ?? ''),
    desc: String(data.desc ?? ''),
    homePath: String(data.homePath ?? '/home'),
    token: typeof localStorage === 'undefined' ? '' : (localStorage.getItem('token') ?? '')
  }
}

function toRouteMenus(data: any): RouteMenuRecord[] {
  const list = Array.isArray(data) ? data : data?.menus ?? data?.items ?? data?.children ?? []
  if (!Array.isArray(list)) return []
  return list.map((item: any) => ({
    path: String(item.path ?? item.url ?? ''),
    name: String(item.name ?? item.code ?? item.id ?? item.path ?? ''),
    component: String(item.component ?? item.componentName ?? 'Layout'),
    meta: {
      title: String(item.meta?.title ?? item.title ?? item.name ?? ''),
      icon: item.meta?.icon ?? item.icon
    },
    children: toRouteMenus(item.children)
  })).filter((item) => item.path || item.children?.length)
}

function toLoginResult(data: Partial<LoginResult> & { token?: string; accessToken?: string; refreshToken?: string; tokenHead?: string }): LoginResult {
  const accessToken = String(data.accessToken ?? data.token ?? '')
  if (!accessToken) throw new Error('登录服务未返回访问令牌')
  return {
    accessToken,
    refreshToken: String(data.refreshToken ?? ''),
    tokenHead: data.tokenHead,
    expiresIn: 0,
    user: data.user ?? {
      id: 0,
      userName: '',
      name: null,
      surname: null,
      email: '',
      phoneNumber: null,
      isActive: true,
      isLockedOut: false,
      lockoutEnd: null,
      accessFailedCount: 0,
      tenantId: null,
      extraProperties: null,
      createdAt: '',
      updatedAt: ''
    },
    configuration: data.configuration ?? {
      auth: { grantedPolicies: {} },
      currentUser: {
        isAuthenticated: true,
        id: 0,
        tenantId: null,
        userName: '',
        name: null,
        surName: null,
        email: '',
        emailVerified: null,
        phoneNumber: null,
        phoneNumberVerified: null,
        roles: [],
        sessionId: null,
        impersonatorUserId: null,
        impersonatorTenantId: null,
        impersonatorUserName: null,
        impersonatorTenantName: null
      },
      currentTenant: { id: null, name: null, isAvailable: false },
      multiTenancy: { isEnabled: true },
      setting: { values: {} },
      extraProperties: {}
    }
  }
}

export const getCaptcha = async (): Promise<CaptchaResult> => {
  if (useMock) return mockGetCaptcha()
  const result = toCaptchaResult(await unwrap<{ uuid?: string; id?: string; imageBase64?: string }>(authHttp.post('/auth/captcha/generate')))
  captchaStore = result
  return result
}

export const verifyCaptcha = (data: CaptchaVerifyData): Promise<void> => {
  if (useMock) return mockVerifyCaptcha(data)
  return unwrap(authHttp.post<boolean>('/auth/captcha/verify', { uuid: data.id, code: data.code ?? '' })).then((valid) => {
    if (valid !== true) throw new Error('验证码错误，请重新输入')
  })
}

export const loginAccount = (data: LoginParams): Promise<LoginResult> => {
  if (useMock) return mockLoginAccount(data)
  const payload = data.captchaUuid || data.code
    ? { ...data, captchaUuid: data.captchaUuid, code: data.code }
    : { username: data.username, password: data.password }
  return unwrap<Partial<LoginResult> & { token?: string; accessToken?: string; refreshToken?: string; tokenHead?: string }>(authHttp.post('/login', payload)).then(toLoginResult)
}

export const loadAvailableTenants = (): Promise<TenantInfo[]> => {
  if (useMock) return mockLoadAvailableTenants()
  return unwrap(authHttp.get<any[]>('/auth/tenants')).then((items) => {
    const unique = new Map<string, TenantInfo>()
    for (const item of items) {
      const tenant = toTenant(item)
      const key = `${tenant.id}:${tenant.code || tenant.name}`
      if (!unique.has(key)) unique.set(key, tenant)
    }
    return [...unique.values()]
  })
}

export const selectTenant = (tenantId: number | string): Promise<TenantInfo> => {
  if (useMock) return mockSelectTenant(tenantId)
  return unwrap(authHttp.post<any>('/auth/select', { tenantId })).then(toTenant)
}

export const loadAvailableHospitals = (): Promise<LoginOrganizationOption[]> => {
  if (useMock) return mockLoadAvailableHospitals()
  return unwrap(authHttp.get<any[]>('/auth/hospitals', undefined, {
    transformResponse: [(data: unknown) => parseJsonWithSafeIds(data)]
  })).then((items) => {
    const unique = new Map<string, LoginOrganizationOption>()
    for (const item of items) {
      const hospital = toHospital(item)
      const key = `${hospital.id}:${hospital.code || hospital.name || hospital.displayName || ''}`
      if (!unique.has(key)) unique.set(key, hospital)
    }
    return [...unique.values()]
  })
}

export const selectHospital = (hospitalId: number | string): Promise<LoginOrganizationOption> => {
  if (useMock) return mockSelectHospital(hospitalId)
  return unwrap(authHttp.post<any>('/auth/hospital/select', { hospitalId: String(hospitalId) })).then(toHospital)
}

export const loadAvailableDepartments = (): Promise<OrganizationInfo[]> => {
  if (useMock) return mockLoadAvailableDepartments()
  return unwrap(authHttp.get<any[]>('/auth/dept/list')).then((items) => {
    const unique = new Map<string, OrganizationInfo>()
    for (const item of items) {
      const department = toDepartment(item)
      const key = `${department.id}:${department.code || department.displayName}`
      if (!unique.has(key)) unique.set(key, department)
    }
    return [...unique.values()]
  })
}

export const selectDepartment = (departmentId: string): Promise<OrganizationInfo> => {
  if (useMock) return mockSelectDepartment(departmentId)
  return unwrap(authHttp.post<any>('/auth/dept/select', { organizationUnitId: departmentId })).then(toDepartment)
}

export const loadCurrentMenus = (): Promise<RouteMenuRecord[]> => {
  if (useMock) return mockLoadCurrentMenus()
  return unwrap(authHttp.get<any>('/auth/menus')).then(toRouteMenus)
}

export const loadCurrentUser = (): Promise<UserInfo> => {
  if (useMock) return mockLoadCurrentUser()
  return unwrap(authHttp.get<any>('/user/info')).then(toUserInfo)
}

export const logout = (): Promise<void> => {
  if (useMock) return mockLogout()
  return unwrap(authHttp.post<string>('/logout', {})).then(() => undefined)
}

export type * from './type'
