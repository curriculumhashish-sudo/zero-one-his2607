import { type JsonVO } from '@/apis/http'
import { createJ2HttpClient } from '@/apis/j2-http'
import { useUserStore } from '@/stores/user'
import type {
  PasswordChangePayload,
  ProfileInfo,
  ProfileUpdatePayload
} from './type'

/**
 * J2 用户个人中心服务客户端
 * 后端接口约定：
 *  - GET  /j2-personcenter/user/get-info?userId=xxx      获取用户信息
 *  - PUT  /j2-personcenter/user/update-user/{id}         更新用户信息
 *  - PUT  /j2-personcenter/user/modify-password/{id}     修改密码
 */
const profileHttp = createJ2HttpClient('/j2-personcenter')

/** 后端 UserInfoDTO 字段（来自 J2 personcenter 服务） */
interface BackendUserInfo {
  userId?: string | number
  userName?: string
  realName?: string
  avatar?: string
  email?: string
  phoneNumber?: string
  isActive?: boolean
  isLockedOut?: boolean
  tenantId?: string | number
  organizationId?: string | number
  practitionerId?: string | number
  createdAt?: string
  updatedAt?: string
  roles?: string[]
  gender?: string
  departmentName?: string
}

/** 后端 UpdateUserDTO 字段 */
interface BackendUpdateUserDTO {
  id: string
  name?: string
  surname?: string
  email?: string
  phoneNumber?: string
}

/** 后端 ChangePasswordDTO 字段 */
interface BackendChangePasswordDTO {
  id: string
  oldPassword: string
  newPassword: string
  confirmPassword: string
}

/** 格式化后端返回的日期时间（Java LocalDateTime 格式: 2026-08-09T23:08:38.247502） */
function formatDateTime(raw: string | undefined): string {
  if (!raw) return ''
  try {
    const normalized = raw.includes('T') ? raw.replace('T', ' ') : raw
    const d = new Date(normalized.replace(' ', 'T'))
    if (!isNaN(d.getTime())) {
      const pad = (n: number) => String(n).padStart(2, '0')
      return `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}:${pad(d.getSeconds())}`
    }
  } catch { /* ignore */ }
  return raw.split('.')[0].replace('T', ' ')
}

/** 将后端 UserInfoDTO 转换为前端 ProfileInfo */
function toProfile(info: BackendUserInfo): ProfileInfo {
  return {
    id: String(info.userId ?? ''),
    username: info.userName ?? '',
    name: info.realName ?? info.userName ?? '',
    avatar: info.avatar ?? '',
    email: info.email ?? '',
    phone: info.phoneNumber ?? '',
    gender: info.gender ?? '',
    department: info.departmentName ?? '',
    roles: info.roles ?? [],
    createdAt: formatDateTime(info.createdAt)
  }
}

/**
 * 从 JWT token 载荷中解析用户 ID
 */
function getUserIdFromToken(): string {
  try {
    const token = localStorage.getItem('token') || ''
    if (!token) return ''
    const parts = token.split('.')
    if (parts.length !== 3) return ''
    const payload = JSON.parse(atob(parts[1].replace(/-/g, '+').replace(/_/g, '/')))
    return String(payload.id ?? payload.userId ?? payload.sub ?? '')
  } catch {
    return ''
  }
}

/**
 * 读取当前登录用户 ID；优先使用 Pinia user store，其次从 JWT token 解析
 * J2 接口要求 userId 必传
 */
function getCurrentUserId(): string {
  try {
    const store = useUserStore()
    if (store.user?.id) return String(store.user.id)
  } catch {
    /* store 未初始化时忽略 */
  }
  return getUserIdFromToken()
}

function unwrap<T>(promise: Promise<JsonVO<T>>): Promise<T> {
  return promise.then((res) => {
    if (res.code !== 10000 || res.data === undefined) {
      throw new Error(res.message || '用户信息请求失败')
    }
    return res.data
  })
}

export async function getProfile(): Promise<ProfileInfo> {
  const userId = getCurrentUserId()
  if (!userId) {
    throw new Error('未获取到当前用户 ID，请重新登录')
  }
  const data = await unwrap(profileHttp.get<BackendUserInfo>('/user/get-info', { userId }))
  return toProfile(data)
}

export async function updateProfile(data: ProfileUpdatePayload): Promise<ProfileInfo> {
  const userId = getCurrentUserId()
  if (!userId) {
    throw new Error('未获取到当前用户 ID，请重新登录')
  }
  // 后端 UpdateUserDTO 只接受 name（名）/ surname（姓）/ email / phoneNumber
  // 前端目前只有一个 name 字段，这里作为 name（名）提交；gender 等字段后端暂未支持
  const payload: BackendUpdateUserDTO = {
    id: userId,
    name: data.name,
    email: data.email,
    phoneNumber: data.phone
  }
  const result = await unwrap(
    profileHttp.put<BackendUserInfo>(`/user/update-user/${userId}`, payload)
  )
  return toProfile(result)
}

export async function changePassword(data: PasswordChangePayload): Promise<void> {
  const userId = getCurrentUserId()
  if (!userId) {
    throw new Error('未获取到当前用户 ID，请重新登录')
  }
  const payload: BackendChangePasswordDTO = {
    id: userId,
    oldPassword: data.oldPassword,
    newPassword: data.newPassword,
    confirmPassword: data.confirmPassword
  }
  await unwrap(profileHttp.put<string>(`/user/modify-password/${userId}`, payload))
}

export type * from './type'
