import {
  mockGetCaptcha,
  mockLoadAvailableDepartments,
  mockLoadAvailableHospitals,
  mockLoadAvailableTenants,
  mockLoadCurrentMenus,
  mockLoadCurrentUser,
  mockLoginAccount,
  mockLogout,
  mockSelectDepartment,
  mockSelectHospital,
  mockSelectTenant,
  mockVerifyCaptcha
} from './mock'
import type { CaptchaVerifyData, LoginParams } from './type'

export const getCaptcha = () => mockGetCaptcha()
export const verifyCaptcha = (data: CaptchaVerifyData) => mockVerifyCaptcha(data)
export const loginAccount = (data: LoginParams) => mockLoginAccount(data)
export const loadAvailableTenants = () => mockLoadAvailableTenants()
export const selectTenant = (tenantId: number | string) => mockSelectTenant(tenantId)
export const loadAvailableHospitals = () => mockLoadAvailableHospitals()
export const selectHospital = (hospitalId: number | string) => mockSelectHospital(hospitalId)
export const loadAvailableDepartments = () => mockLoadAvailableDepartments()
export const selectDepartment = (departmentId: string) => mockSelectDepartment(departmentId)
export const loadCurrentMenus = () => mockLoadCurrentMenus()
export const loadCurrentUser = () => mockLoadCurrentUser()
export const logout = () => mockLogout()

export type * from './type'
