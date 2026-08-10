import { useHttp } from '@/plugins/http'
import type {
  CaptchaImage,
  LoginOrganizationOption,
  LoginParams,
  OrganizationInfo,
  Oauth2TokenDTO,
  RouteMenuRecord,
  TenantInfo,
  UserInfo
} from './type'

/** J1 认证服务基础地址（通过 Vite 代理 /j1-api 访问） */
const J1_BASE_URL = import.meta.env.VITE_J1_API_URL

/**
 * 生成验证码
 * POST /auth/captcha/generate
 * 返回 { uuid, imageBase64 }
 */
export async function j1GenerateCaptcha(): Promise<CaptchaImage> {
  const http = useHttp()
  const vo = await http.post<CaptchaImage>('/auth/captcha/generate', undefined, {
    baseURL: J1_BASE_URL
  })
  return vo.data!
}

/**
 * 登录
 * POST /login，body: { username, password, captchaUuid, code }
 * 返回 Oauth2TokenDTO: { token, refreshToken, tokenHead }
 */
export async function j1Login(data: LoginParams): Promise<Oauth2TokenDTO> {
  const http = useHttp()
  const vo = await http.post<Oauth2TokenDTO>(
    '/login',
    {
      username: data.username,
      password: data.password,
      captchaUuid: data.captchaUuid,
      code: data.code
    },
    { baseURL: J1_BASE_URL }
  )
  return vo.data!
}

/**
 * 获取租户列表
 * GET /auth/tenants
 * 返回数组: [{ id, name, code, description, isActive, ... }]
 * 注意：isActive 在 J1 中是 integer (1/0)，需要转换为 boolean
 */
export async function j1GetTenants(): Promise<TenantInfo[]> {
  const http = useHttp()
  const vo = await http.get<TenantInfo[]>('/auth/tenants', undefined, {
    baseURL: J1_BASE_URL
  })
  const list = vo.data ?? []
  // J1 可能返回重复的租户数据，按 id 去重
  const seen = new Set<string | number>()
  return list
    .filter((item) => {
      const key = item.id
      if (seen.has(key)) return false
      seen.add(key)
      return true
    })
    .map((item) => ({
      ...item,
      isActive: Boolean(item.isActive)
    }))
}

/**
 * 选择租户
 * POST /auth/select，body: { tenantId: number }
 * 注意：此接口有后端 SQL bug，调用时需要 catch 错误并返回 undefined
 */
export async function j1SelectTenant(tenantId: number | string): Promise<void> {
  const http = useHttp()
  try {
    await http.post('/auth/select', { tenantId: Number(tenantId) }, { baseURL: J1_BASE_URL })
  } catch (error) {
    console.warn('j1SelectTenant failed (expected backend SQL bug):', error)
  }
}

/**
 * 获取医院列表
 * GET /auth/hospitals?keyword=xxx
 * 返回数组: [{ id, code, name, displayName }]
 * 注意：J1 的 hospital id 是 integer 类型，前端期望 string，需要转换
 */
export async function j1GetHospitals(): Promise<LoginOrganizationOption[]> {
  const http = useHttp()
  try {
    const vo = await http.get<LoginOrganizationOption[]>('/auth/hospitals', undefined, {
      baseURL: J1_BASE_URL
    })
    const list = vo.data ?? []
    const mapped = list.map((item) => ({
      ...item,
      id: String(item.id)
    }))
    // J1 可能返回空数组（当前租户未配置医院），返回默认医院以保证登录流程继续
    if (!mapped.length) {
      return [{ id: '1', displayName: '默认医院', name: '默认医院' }]
    }
    return mapped
  } catch {
    // J1 租户选择有后端 SQL bug，医院列表可能无法获取
    // 返回默认医院以保证登录流程继续（C6 后端不需要医院上下文）
    return [{ id: '1', displayName: '默认医院', name: '默认医院' }]
  }
}

/**
 * 选择医院
 * POST /auth/hospital/select，body: { hospitalId: string }
 */
export async function j1SelectHospital(hospitalId: number | string): Promise<void> {
  const http = useHttp()
  try {
    await http.post(
      '/auth/hospital/select',
      { hospitalId: String(hospitalId) },
      { baseURL: J1_BASE_URL }
    )
  } catch {
    // J1 后端可能有 bug，容错处理
  }
}

/**
 * 获取科室列表
 * GET /auth/dept/list?displayName=xxx&code=xxx
 * 返回数组: [{ id, code, displayName, isEnabled, ... }]
 */
export async function j1GetDepartments(): Promise<OrganizationInfo[]> {
  const http = useHttp()
  try {
    const vo = await http.get<OrganizationInfo[]>('/auth/dept/list', undefined, {
      baseURL: J1_BASE_URL
    })
    return vo.data ?? []
  } catch {
    return []
  }
}

/**
 * 选择科室
 * POST /auth/dept/select
 */
export async function j1SelectDepartment(departmentId: string): Promise<void> {
  const http = useHttp()
  try {
    await http.post('/auth/dept/select', { departmentId }, { baseURL: J1_BASE_URL })
  } catch {
    // J1 后端可能有 bug，容错处理
  }
}

/**
 * 获取当前用户信息
 * GET /user/info
 * 返回 LoginVO: { userId, id, username, realName, name, avatar, desc, homePath, token, roles, ... }
 * UserInfo 的 roles 字段从 LoginVO 的 roles 数组获取
 */
export async function j1GetCurrentUser(): Promise<UserInfo> {
  const http = useHttp()
  const vo = await http.get<any>('/user/info', undefined, { baseURL: J1_BASE_URL })
  const data = vo.data!
  return {
    avatar: data.avatar ?? '',
    realName: data.realName ?? data.name ?? '',
    roles: Array.isArray(data.roles) ? data.roles : [],
    userId: String(data.userId ?? data.id ?? ''),
    username: data.username ?? '',
    desc: data.desc ?? '',
    homePath: data.homePath ?? '/home',
    token: data.token ?? ''
  }
}

/**
 * 获取菜单
 * GET /auth/menus
 * 可能返回失败(code 9999)，需要 catch 并返回空数组
 */
export async function j1GetMenus(): Promise<RouteMenuRecord[]> {
  const http = useHttp()
  try {
    const vo = await http.get<RouteMenuRecord[]>('/auth/menus', undefined, {
      baseURL: J1_BASE_URL
    })
    return vo.data ?? []
  } catch {
    return []
  }
}

/**
 * 退出登录
 * POST /logout
 * 即使失败也不 throw
 */
export async function j1Logout(): Promise<void> {
  const http = useHttp()
  try {
    await http.post('/logout', undefined, { baseURL: J1_BASE_URL })
  } catch {
    // 即使失败也不 throw
  }
}
