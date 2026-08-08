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
  request(authHttp.post<Oauth2TokenDTO>('/auth-login', data))

export const loadAvailableTenants = () =>
  request(authHttp.get<TenantInfo[]>('/auth/tenants'))

export const selectTenant = (tenantId: number | string) =>
  request(authHttp.post<TenantInfo>('/auth/select', { tenantId: Number(tenantId) }))

export const loadAvailableHospitals = () =>
  request(authHttp.get<LoginOrganizationOption[]>('/auth/hospitals'))

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
  request<RouteMenuRecord[]>(authHttp.get('/get-menus'))

export async function loadCurrentUser(): Promise<UserInfo> {
  const user = await request<BackendLoginUser>(authHttp.get('/current-user'))
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
