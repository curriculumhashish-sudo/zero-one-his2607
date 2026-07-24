import type { UserRecord } from '@/apis/system/user'
import type { UserFormModel } from './type'

export function createEmptyUserForm(): UserFormModel {
  return {
    id: '',
    username: '',
    realName: '',
    phone: '',
    email: '',
    roleIds: [],
    enabled: true
  }
}

export function toUserForm(user: UserRecord): UserFormModel {
  return {
    id: user.id,
    username: user.username,
    realName: user.realName,
    phone: user.phone,
    email: user.email,
    roleIds: [...user.roleIds],
    enabled: user.enabled
  }
}
