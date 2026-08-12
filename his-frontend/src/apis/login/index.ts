import HttpClient, { type JsonVO } from '@/apis/http'
import type {
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

export const getCaptcha = () => request(authHttp.post<CaptchaResult>('/auth/captcha/generate'))

export const verifyCaptcha = (data: CaptchaVerifyData) =>
  request(authHttp.post<boolean>('/auth/captcha/verify', data)).then((valid) => {
    if (!valid) throw new Error('验证码错误，请重试')
  })

export const loginAccount = (data: LoginParams) =>
  request(authHttp.post<Oauth2TokenDTO>('/login', data))

export const loadAvailableTenants = () =>
  request(authHttp.get<TenantInfo[]>('/auth/tenants'))

export const selectTenant = (tenantId: number | string) =>
  request(authHttp.post<TenantInfo>('/auth/select', { tenantId: String(tenantId) }))

export const loadAvailableHospitals = () =>
  request(
    authHttp.get<LoginOrganizationOption[]>('/auth/hospitals', undefined, {
      transformResponse: [(data: unknown) => parseHospitalResponse(data)]
    })
  )

export const selectHospital = (hospitalId: number | string) =>
  request(authHttp.post<LoginOrganizationOption>('/auth/hospital/select', {
    hospitalId: String(hospitalId)
  }))

export const loadAvailableDepartments = () =>
  request(authHttp.get<OrganizationInfo[]>('/auth/dept/list'))

export const selectDepartment = (departmentId: string) =>
  request(authHttp.post<{ organizationUnitId: string }>('/auth/dept/select', {
    organizationUnitId: departmentId
  }))

export const loadCurrentMenus = () =>
  request<unknown>(authHttp.get('/auth/menus')).then((data) => normalizeMenus(data))

function normalizeMenus(data: unknown): RouteMenuRecord[] {
  if (!Array.isArray(data)) return []

  const normalize = (item: Record<string, unknown>): RouteMenuRecord | null => {
    const path = String(item.path ?? item.href ?? '').trim()
    const id = String(item.name ?? item.id ?? path).trim()
    const meta = item.meta && typeof item.meta === 'object'
      ? item.meta as Record<string, unknown>
      : {}
    const title = String(meta.title ?? item.text ?? item.title ?? '').trim()
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
        title: title || id || path,
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

export async function loadCurrentUser(): Promise<UserInfo> {
  const user = await request<BackendLoginUser>(authHttp.get('/user/info'))
  return {
    avatar: user.avatar ?? '',
    realName: user.realName ?? user.name ?? user.username ?? '',
    roles: user.roles ?? [],
    userId: String(user.userId ?? user.id ?? ''),
    username: user.username ?? '',
    desc: user.desc ?? '',
    homePath: user.homePath ?? '/home',
    token: typeof localStorage === 'undefined' ? '' : localStorage.getItem('token') ?? ''
  }
}

function parseHospitalResponse(data: unknown) {
  if (typeof data !== 'string') return data

  // 医院 ID 超过 JavaScript 安全整数范围，先转成字符串再交给业务层。
  const safeJson = data.replace(/("id"\s*:\s*)(\d{16,})(?=\s*[,}])/g, '$1"$2"')
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

export const logout = () => request(authHttp.post<string>('/logout'))

export type * from './type'
