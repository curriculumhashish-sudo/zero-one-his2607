import {
  mockGetSlotDetail,
  mockBatchBlockSlots,
  mockBatchUnblockSlots,
  mockLoadSlotDepartmentOptions,
  mockLoadSlotDoctorOptions,
  mockLoadSlotInit,
  mockPageSlotsByDate,
  mockMarkSlotOverbooked
} from './mock'

export const loadSlotInit = mockLoadSlotInit
export const pageSlotsByDate = mockPageSlotsByDate
export const getSlotDetail = mockGetSlotDetail
export const loadDepartmentOptions = mockLoadSlotDepartmentOptions
export const loadDoctorOptions = mockLoadSlotDoctorOptions
export const batchBlockSlots = mockBatchBlockSlots
export const batchUnblockSlots = mockBatchUnblockSlots
export const markSlotOverbooked = mockMarkSlotOverbooked

export type * from './type'
