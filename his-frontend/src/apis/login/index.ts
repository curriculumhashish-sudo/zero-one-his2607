import HttpClient, { type JsonVO } from '@/apis/http'
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
import {
  j1GenerateCaptcha,
  j1GetCurrentUser,
  j1GetDepartments,
  j1GetHospitals,
  j1GetMenus,
  j1GetTenants,
  j1Login,
  j1Logout,
  j1SelectDepartment,
  j1SelectHospital,
  j1SelectTenant
} from './j1'
import type {
  CaptchaImage,
  CaptchaResult,
  CaptchaVerifyData,
  LoginOrganizationOption,
  LoginParams,
  Oauth2TokenDTO,
  OrganizationInfo,
  RouteMenuRecord,
  TenantInfo,
  UserInfo
} from './type'

const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_USE_MOCK === 'true'
// J1 适配按显式配置启用，避免覆盖当前 /login-api 的完整本地登录链路。
const useJ1Adapter = String(import.meta.env.VITE_LOGIN_PROVIDER ?? '').toLowerCase() === 'j1'
let captchaStore: CaptchaImage | undefined

const authHttp = new HttpClient(
  { baseURL: import.meta.env.VITE_LOGIN_API_URL || '/login-api' },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token') || ''
      if (token) config.headers.Authorization = `Bearer ${token}`
      config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

async function request<T>(promise: Promise<JsonVO<T>>): Promise<T> {
  const response = await promise
  if (response.code !== 10000 || response.data === undefined) {
    throw new Error(response.message || '登录服务请求失败')
  }
  return response.data
}

export function getCaptchaImage() {
  return captchaStore?.imageBase64 ?? ''
}

export const getCaptcha = async (): Promise<CaptchaResult> => {
  if (useMock) return mockGetCaptcha()
  if (!useJ1Adapter) return request(authHttp.post<CaptchaResult>('/auth/captcha/generate'))
  captchaStore = await j1GenerateCaptcha()
  return { id: captchaStore.uuid, uuid: captchaStore.uuid, imageBase64: captchaStore.imageBase64 }
}

export const verifyCaptcha = (data: CaptchaVerifyData): Promise<void> => {
  if (useMock) return mockVerifyCaptcha(data)
  if (useJ1Adapter) return Promise.resolve()
  return request(
    authHttp.post<boolean>('/auth/captcha/verify', {
      uuid: data.id ?? data.uuid,
      code: data.code
    })
  ).then((valid) => {
    if (!valid) throw new Error('验证码错误，请重试')
  })
}

export const loginAccount = (data: LoginParams): Promise<Oauth2TokenDTO> => {
  if (useMock) return mockLoginAccount(data) as Promise<Oauth2TokenDTO>
  return useJ1Adapter ? j1Login(data) : request(authHttp.post<Oauth2TokenDTO>('/login', data))
}

export const loadAvailableTenants = (): Promise<TenantInfo[]> => {
  if (useMock) return mockLoadAvailableTenants()
  if (useJ1Adapter) return j1GetTenants()
  return request(
    authHttp.get<TenantInfo[]>('/auth/tenants', undefined, {
      transformResponse: [(data: unknown) => parseLoginContextResponse(data)]
    })
  )
}

export const selectTenant = (tenantId: number | string) => {
  if (useMock) return mockSelectTenant(tenantId)
  if (useJ1Adapter) return j1SelectTenant(tenantId)
  return request(authHttp.post<TenantInfo>('/auth/select', { tenantId: String(tenantId) }))
}

export const loadAvailableHospitals = (): Promise<LoginOrganizationOption[]> => {
  if (useMock) return mockLoadAvailableHospitals()
  if (useJ1Adapter) return j1GetHospitals()
  return request(
    authHttp.get<LoginOrganizationOption[]>('/auth/hospitals', undefined, {
      transformResponse: [(data: unknown) => parseLoginContextResponse(data)]
    })
  )
}

export const selectHospital = (hospitalId: number | string) => {
  if (useMock) return mockSelectHospital(hospitalId)
  if (useJ1Adapter) return j1SelectHospital(hospitalId)
  return request(
    authHttp.post<LoginOrganizationOption>('/auth/hospital/select', { hospitalId: String(hospitalId) })
  )
}

export const loadAvailableDepartments = (): Promise<OrganizationInfo[]> => {
  if (useMock) return mockLoadAvailableDepartments()
  if (useJ1Adapter) return j1GetDepartments()
  return request(
    authHttp.get<OrganizationInfo[]>('/auth/dept/list', undefined, {
      transformResponse: [(data: unknown) => parseLoginContextResponse(data)]
    })
  )
}

export const selectDepartment = (departmentId: string) => {
  if (useMock) return mockSelectDepartment(departmentId)
  if (useJ1Adapter) return j1SelectDepartment(departmentId)
  return request(authHttp.post<{ organizationUnitId: string }>('/auth/dept/select', { organizationUnitId: departmentId }))
}

export const loadCurrentMenus = (): Promise<RouteMenuRecord[]> => {
  if (useMock) return mockLoadCurrentMenus()
  if (useJ1Adapter) return j1GetMenus()
  return request<unknown>(authHttp.get('/auth/menus')).then(normalizeMenus)
}

function normalizeMenus(data: unknown): RouteMenuRecord[] {
  if (!Array.isArray(data)) return []
  const normalize = (item: Record<string, unknown>): RouteMenuRecord | null => {
    const path = String(item.path ?? item.href ?? '').trim()
    const id = String(item.name ?? item.id ?? path).trim()
    const meta = item.meta && typeof item.meta === 'object' ? item.meta as Record<string, unknown> : {}
    const children = Array.isArray(item.children)
      ? item.children
          .filter((child): child is Record<string, unknown> => Boolean(child && typeof child === 'object'))
          .map(normalize)
          .filter((child): child is RouteMenuRecord => child !== null)
      : undefined
    if (!path && !children?.length) return null
    return {
      path,
      name: id || path,
      component: String(item.component ?? 'Layout'),
      meta: {
        title: String((meta.title ?? item.text ?? item.title ?? id) || path),
        icon: String(meta.icon ?? item.icon ?? 'IconMenu')
      },
      children: children?.length ? children : undefined
    }
  }
  return data
    .filter((item): item is Record<string, unknown> => Boolean(item && typeof item === 'object'))
    .map(normalize)
    .filter((item): item is RouteMenuRecord => item !== null)
}

export const loadCurrentUser = (): Promise<UserInfo> => {
  if (useMock) return mockLoadCurrentUser()
  if (useJ1Adapter) return j1GetCurrentUser()
  return request<BackendLoginUser>(authHttp.get('/user/info')).then((user) => ({
    avatar: user.avatar ?? '',
    realName: user.realName ?? user.name ?? user.username ?? '',
    roles: user.roles ?? [],
    userId: String(user.userId ?? user.id ?? ''),
    username: user.username ?? '',
    desc: user.desc ?? '',
    homePath: user.homePath ?? '/home',
    token: typeof localStorage === 'undefined' ? '' : localStorage.getItem('token') ?? ''
  }))
}

/** 登录上下文的 Java long 主键必须在 JSON.parse 前保留为字符串。 */
export function parseLoginContextResponse(data: unknown) {
  if (typeof data !== 'string') return data
  const safeJson = data.replace(
    /("(?:id|tenantId|hospitalId|organizationId|organizationUnitId)"\s*:\s*)(-?\d{16,})(?=\s*[,}])/gi,
    '$1"$2"'
  )
  return JSON.parse(safeJson)
}

interface BackendLoginUser {
  id?: string | number
  userId?: string | number
  username?: string
  userName?: string
  name?: string
  realName?: string
  avatar?: string
  desc?: string
  homePath?: string
  roles?: string[]
}

export const logout = (): Promise<void> => {
  if (useMock) return mockLogout()
  if (useJ1Adapter) return j1Logout()
  return request(authHttp.post<string>('/logout')).then(() => undefined)
}

export type * from './type'
