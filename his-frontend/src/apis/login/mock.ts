import type {
  CaptchaResult,
  CaptchaVerifyData,
  LoginOrganizationOption,
  LoginParams,
  LoginResult,
  OrganizationInfo,
  RouteMenuRecord,
  TenantInfo,
  UserInfo
} from './type'

const tenants: TenantInfo[] = [
  { id: 1001, name: '零壹医疗集团', code: 'ZERO_ONE', description: '默认医疗租户', isActive: true, extraProperties: null, createdAt: '2026-01-01 08:00:00', updatedAt: '2026-07-30 08:00:00', createdBy: 'system', updatedBy: 'system' },
  { id: 1002, name: '零壹第二医院', code: 'ZERO_ONE_2', description: '演示医疗租户', isActive: true, extraProperties: null, createdAt: '2026-01-01 08:00:00', updatedAt: '2026-07-30 08:00:00', createdBy: 'system', updatedBy: 'system' }
]
const hospitals: LoginOrganizationOption[] = [
  { id: 'hospital-1', code: 'H001', displayName: '零壹医院', name: '零壹医院' },
  { id: 'hospital-2', code: 'H002', displayName: '零壹第二医院', name: '零壹第二医院' }
]
const departments: OrganizationInfo[] = [
  { children: null, code: 'DEPT-001', createdAt: '2026-01-01 08:00:00', displayName: '心血管内科', id: 'dept-internal', isEnabled: true, parentDisplayName: null, parentId: null, remark: null, sortOrder: 1, tenantId: '1001', updatedAt: null },
  { children: null, code: 'DEPT-002', createdAt: '2026-01-01 08:00:00', displayName: '普通外科', id: 'dept-surgery', isEnabled: true, parentDisplayName: null, parentId: null, remark: null, sortOrder: 2, tenantId: '1001', updatedAt: null }
]

let loggedIn = false
let currentUsername = 'admin'
let selectedTenantId: number | string | null = null
let selectedHospitalId: number | string | null = null
let selectedDepartmentId: string | null = null

function delay(ms = 60) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function requireLogin() {
  if (!loggedIn && typeof localStorage !== 'undefined' && localStorage.getItem('token')) loggedIn = true
  if (!loggedIn) throw new Error('登录状态已失效，请重新登录')
}

function currentProfile() {
  if (currentUsername === 'reporter') {
    return {
      id: 2,
      realName: '报表专员',
      name: '报表',
      surname: '专员',
      email: 'reporter@zero-one.test',
      roles: ['reporter'],
      practitionerId: null,
      canFinanceApprove: false
    }
  }
  return {
    id: 1,
    realName: '系统管理员',
    name: '系统',
    surname: '管理员',
    email: 'admin@zero-one.test',
    roles: ['doctor', 'administrator'],
    practitionerId: 'doctor-1',
    canFinanceApprove: true
  }
}

function configuration(): LoginResult['configuration'] {
  const profile = currentProfile()
  return {
    auth: { grantedPolicies: { 'Report.OutpatientFeeDetail': true, 'Report.DataStatistics': currentUsername !== 'reporter' } },
    currentUser: { isAuthenticated: true, id: profile.id, tenantId: selectedTenantId ? Number(selectedTenantId) : null, userName: currentUsername, name: profile.name, surName: profile.surname, email: profile.email, emailVerified: true, phoneNumber: '13800000000', phoneNumberVerified: true, roles: profile.roles, sessionId: `session-${currentUsername}`, impersonatorUserId: null, impersonatorTenantId: null, impersonatorUserName: null, impersonatorTenantName: null, practitionerId: profile.practitionerId, organizationId: selectedDepartmentId, canFinanceApprove: profile.canFinanceApprove },
    currentTenant: { id: selectedTenantId ? Number(selectedTenantId) : null, name: tenants.find((item) => item.id === Number(selectedTenantId))?.name ?? null, isAvailable: Boolean(selectedTenantId) },
    multiTenancy: { isEnabled: true },
    setting: { values: {} },
    extraProperties: {}
  }
}

export async function mockGetCaptcha(): Promise<CaptchaResult> {
  await delay(30)
  return { id: `captcha-${Date.now()}` }
}

export async function mockVerifyCaptcha(data: CaptchaVerifyData): Promise<void> {
  await delay(30)
  if (!(data.id ?? data.uuid ?? '').trim()) throw new Error('验证码验证失败，请重试')
}

export async function mockLoginAccount(data: LoginParams): Promise<LoginResult> {
  await delay()
  if (!data.username?.trim() || !data.password) throw new Error('请输入账号和密码')
  if (data.username === 'locked') throw new Error('账号已锁定，请联系管理员')
  loggedIn = true
  currentUsername = data.username.trim()
  selectedTenantId = null
  selectedHospitalId = null
  selectedDepartmentId = null
  const accessToken = `mock-access-${Date.now()}`
  return {
    accessToken,
    refreshToken: `mock-refresh-${Date.now()}`,
    expiresIn: 7200,
    user: { id: currentProfile().id, userName: currentUsername, name: currentProfile().name, surname: currentProfile().surname, email: currentProfile().email, phoneNumber: '13800000000', isActive: true, isLockedOut: false, lockoutEnd: null, accessFailedCount: 0, tenantId: null, extraProperties: null, createdAt: '2026-01-01 08:00:00', updatedAt: '2026-07-30 08:00:00', practitionerId: currentProfile().practitionerId, organizationId: null },
    configuration: configuration()
  }
}

export async function mockLoadAvailableTenants() {
  await delay()
  requireLogin()
  return structuredClone(currentUsername === 'reporter' ? tenants.slice(1) : tenants)
}

export async function mockSelectTenant(tenantId: number | string) {
  await delay()
  requireLogin()
  const tenant = tenants.find((item) => item.id === Number(tenantId))
  if (!tenant) throw new Error('所选租户不存在或已停用')
  selectedTenantId = tenant.id
  selectedHospitalId = null
  selectedDepartmentId = null
  return structuredClone(tenant)
}

export async function mockLoadAvailableHospitals() {
  await delay()
  requireLogin()
  if (!selectedTenantId) throw new Error('请先选择租户')
  return structuredClone(selectedTenantId === 1002 ? hospitals.slice(1) : hospitals)
}

export async function mockSelectHospital(hospitalId: number | string) {
  await delay()
  requireLogin()
  const hospital = hospitals.find((item) => item.id === hospitalId)
  if (!hospital) throw new Error('所选医院不存在')
  selectedHospitalId = hospital.id
  selectedDepartmentId = null
  return structuredClone(hospital)
}

export async function mockLoadAvailableDepartments() {
  await delay()
  requireLogin()
  if (!selectedHospitalId) throw new Error('请先选择医院')
  return structuredClone(departments)
}

export async function mockSelectDepartment(departmentId: string) {
  await delay()
  requireLogin()
  const department = departments.find((item) => item.id === departmentId && item.isEnabled)
  if (!department) throw new Error('所选机构（科室）不存在或已停用')
  selectedDepartmentId = department.id
  return structuredClone(department)
}

export async function mockLoadCurrentMenus(): Promise<RouteMenuRecord[]> {
  await delay()
  requireLogin()
  const children: RouteMenuRecord[] = [{ path: '/report/outpatient-fee-detail', name: 'ReportOutpatientFeeDetail', component: '/report/outpatient-fee-detail/Index', meta: { title: '门诊费用明细', icon: 'IconTickets' } }]
  if (currentUsername !== 'reporter') {
    children.push({ path: '/report/data-statistics', name: 'ReportDataStatistics', component: '/report/data-statistics/Index', meta: { title: '数据统计', icon: 'IconTrendCharts' } })
  }
  return [{ path: '/report', name: 'ReportCenter', component: 'Layout', meta: { title: '报表中心', icon: 'IconTickets' }, children }]
}

export async function mockLoadCurrentUser(): Promise<UserInfo> {
  await delay()
  requireLogin()
  const profile = currentProfile()
  return { avatar: 'https://cube.elemecdn.com/3/7c/3ea6beec64369c2642b92c6726f1epng.png', realName: profile.realName, roles: profile.roles, userId: String(profile.id), practitionerId: profile.practitionerId ?? undefined, username: currentUsername, desc: `零壹医院${profile.realName}`, homePath: '/home', token: typeof localStorage === 'undefined' ? '' : (localStorage.getItem('token') ?? '') }
}

export async function mockLogout(): Promise<void> {
  await delay(30)
  loggedIn = false
  currentUsername = 'admin'
  selectedTenantId = null
  selectedHospitalId = null
  selectedDepartmentId = null
}
