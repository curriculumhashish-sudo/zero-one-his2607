import {
  mockGetSlotDetail,
  mockLoadSlotDepartmentOptions,
  mockLoadSlotDoctorOptions,
  mockLoadSlotInit,
  mockPageSlotsByDate
} from './mock'

export const loadSlotInit = mockLoadSlotInit
export const pageSlotsByDate = mockPageSlotsByDate
export const getSlotDetail = mockGetSlotDetail
export const loadDepartmentOptions = mockLoadSlotDepartmentOptions
export const loadDoctorOptions = mockLoadSlotDoctorOptions

export type * from './type'
