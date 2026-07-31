import type { HolidayExceptionType, ScheduleMode, TemplateStatus } from '@/apis/booking/schedule-template'

export interface ScheduleTemplatePageQuery {
  keyword: string
  status: TemplateStatus | ''
  page: number
  pageSize: number
}

export interface TimePeriodForm {
  name: string
  segments: Array<{ start: string; end: string; capacity: number }>
  status: TemplateStatus
}

export interface DailyTemplateForm {
  name: string
  scheduleMode: ScheduleMode
  serviceType: string
  periodIds: string[]
  doctorId: string
  roomId: string
  releasePolicyId: string
  effectiveStart: string
  effectiveEnd: string
  priority: number
  defaultAppointment: boolean
  status: TemplateStatus
}

export interface WeeklyTemplateForm {
  name: string
  isDefault: boolean
  appointmentOpen: boolean
  status: TemplateStatus
  items: Array<{ weekday: number; dailyTemplateId: string; enabled: boolean }>
}

export interface HolidayExceptionForm {
  date: string
  rangeEnd?: string
  type: HolidayExceptionType
  replacementDailyTemplateId?: string
  reason: string
  autoApplyOnGenerate: boolean
}
