export interface LoginParams {
  password: string
  username: string
  captchaUuid?: string
  code?: string
}

export interface LoginUser {
  id: number
  userName: string
  name: null | string
  surname: null | string
  email: string
  phoneNumber: null | string
  isActive: boolean
  isLockedOut: boolean
  lockoutEnd: null | string
  accessFailedCount: number
  tenantId: null | number
  extraProperties: null | Record<string, unknown>
  createdAt: string
  updatedAt: string
  practitionerId?: null | number | string
  organizationId?: null | number | string
}

export interface GrantedPolicies {
  [policy: string]: boolean
}

export interface CurrentUser {
  isAuthenticated: boolean
  id: number
  tenantId: null | number
  userName: string
  name: null | string
  surName: null | string
  email: string
  emailVerified: boolean | null
  phoneNumber: null | string
  phoneNumberVerified: boolean | null
  roles: string[]
  sessionId: null | string
  impersonatorUserId: null | number
  impersonatorTenantId: null | number
  impersonatorUserName: null | string
  impersonatorTenantName: null | string
  practitionerId?: null | number | string
  organizationId?: null | number | string
  canFinanceApprove?: boolean
}

export interface LoginConfiguration {
  auth: { grantedPolicies: GrantedPolicies }
  currentUser: CurrentUser
  currentTenant: { id: null | number; name: null | string; isAvailable: boolean }
  multiTenancy: { isEnabled: boolean }
  setting: { values: Record<string, unknown> }
  extraProperties: Record<string, unknown>
}

export interface LoginResult {
  accessToken: string
  refreshToken: string
  tokenHead?: string
  expiresIn: number
  user: LoginUser
  configuration: LoginConfiguration
}

export interface TenantInfo {
  /** 后端为 int64；前端保留字符串，避免超过 JS 安全整数范围后丢失精度。 */
  id: number | string
  name: string
  code: string
  description: string
  isActive: boolean
  extraProperties: null
  createdAt: string
  updatedAt: string
  createdBy: string
  updatedBy: string
}

export interface LoginOrganizationOption {
  id: number | string
  code?: string
  displayName?: string
  name?: string
}

export interface OrganizationInfo {
  children: OrganizationInfo[] | null
  code: string
  createdAt: string
  displayName: string
  id: string
  isEnabled: boolean | number
  parentDisplayName: null | string
  parentId: null | string
  remark: null | string
  sortOrder: number
  tenantId: string
  updatedAt: null | string
}

export interface UserInfo {
  avatar: string
  realName: string
  roles?: string[]
  userId: string
  /** 当前登录账号绑定的医护人员 ID。 */
  practitionerId?: string
  username: string
  desc: string
  homePath: string
  token: string
}

export interface RouteMenuRecord {
  path: string
  name: string
  component: string
  meta?: { title: string; icon?: string }
  children?: RouteMenuRecord[]
}

export interface Menu {
  id: string
  text: string
  icon: string
  href?: string
  pid?: string
  children?: Menu[]
}

export interface CaptchaResult {
  id: string
  uuid?: string
  imageBase64?: string
}

export interface CaptchaImage {
  uuid: string
  imageBase64: string
}

export interface CaptchaVerifyData {
  id?: string
  uuid?: string
  code?: string
}

export interface Oauth2TokenDTO {
  token: string
  refreshToken: string
  tokenHead?: string
  user: LoginUser
  configuration?: LoginConfiguration
}

export type LoginDTO = LoginParams
