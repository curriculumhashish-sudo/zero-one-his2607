import type { UserSavePayload } from '@/apis/system/user'

export interface UserFormModel extends UserSavePayload {
  id: string
}

export type UserEnabledFilter = '' | 'enabled' | 'disabled'
export type UserLockedFilter = '' | 'locked' | 'unlocked'
