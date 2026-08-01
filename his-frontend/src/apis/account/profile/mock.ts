import type { PasswordChangePayload, ProfileInfo, ProfileUpdatePayload } from './type'

let profile: ProfileInfo = {
  id: 'u1',
  username: 'admin',
  name: '管理员',
  avatar: '',
  email: 'admin@his.com',
  phone: '13800000000',
  gender: 'male',
  department: '信息科',
  roles: ['系统管理员'],
  createdAt: '2025-01-01'
}

function delay<T>(data: T, ms = 150): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(data), ms))
}

export function mockGetProfile(): Promise<ProfileInfo> {
  return delay({ ...profile })
}

export function mockUpdateProfile(data: ProfileUpdatePayload): Promise<ProfileInfo> {
  profile = {
    ...profile,
    ...data
  }
  return delay({ ...profile })
}

export function mockChangePassword(data: PasswordChangePayload): Promise<void> {
  if (!data.oldPassword) throw new Error('请输入原密码')
  if (!data.newPassword) throw new Error('请输入新密码')
  if (data.newPassword !== data.confirmPassword) throw new Error('两次密码不一致')
  if (data.newPassword.length < 6) throw new Error('新密码至少6位')
  return delay(undefined)
}
