import { mockChangePassword, mockGetProfile, mockUpdateProfile } from './mock'
import type { PasswordChangePayload, ProfileInfo, ProfileUpdatePayload } from './type'

export const getProfile = (): Promise<ProfileInfo> => mockGetProfile()
export const updateProfile = (data: ProfileUpdatePayload): Promise<ProfileInfo> =>
  mockUpdateProfile(data)
export const changePassword = (data: PasswordChangePayload): Promise<void> =>
  mockChangePassword(data)

export type * from './type'
