export interface ProfileInfo {
  id: string
  username: string
  name: string
  avatar?: string
  email: string
  phone: string
  gender: string
  department: string
  roles: string[]
  createdAt: string
}

export interface ProfileUpdatePayload {
  name?: string
  email?: string
  phone?: string
  gender?: string
  avatar?: string
}

export interface PasswordChangePayload {
  oldPassword: string
  newPassword: string
  confirmPassword: string
}
