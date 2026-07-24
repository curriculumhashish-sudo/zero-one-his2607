import {
  mockCreateSchedule,
  mockGetScheduleDetail,
  mockLoadDepartmentOptions,
  mockLoadDoctorOptions,
  mockLoadScheduleInit,
  mockPageSchedulesByRange,
  mockPublishSchedule,
  mockStopSchedule,
  mockUpdateSchedule
} from './mock'

export const loadScheduleInit = mockLoadScheduleInit
export const pageSchedulesByRange = mockPageSchedulesByRange
export const getScheduleDetail = mockGetScheduleDetail
export const createSchedule = mockCreateSchedule
export const updateSchedule = mockUpdateSchedule
export const publishSchedule = mockPublishSchedule
export const stopSchedule = mockStopSchedule
export const loadDepartmentOptions = mockLoadDepartmentOptions
export const loadDoctorOptions = mockLoadDoctorOptions

export type * from './type'
