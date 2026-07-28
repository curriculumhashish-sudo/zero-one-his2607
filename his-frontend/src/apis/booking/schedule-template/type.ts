export type TemplateStatus = 'enabled' | 'disabled'

export interface BookingOption {
  value: string
  label: string
}

export interface PageResult<T> {
  rows: T[]
  total: number
}

export interface TimeSegment {
  id: string
  start: string
  end: string
  capacity: number
}

export interface TimePeriod {
  id: string
  name: string
  segments: TimeSegment[]
  status: TemplateStatus
  usageCount: number
}

export interface DailyTemplate {
  id: string
  name: string
  scheduleMode: ScheduleMode
  serviceType: string
  periodIds: string[]
  doctorId: string
  doctorName: string
  roomId: string
  roomName: string
  releasePolicyId: string
  effectiveStart: string
  effectiveEnd: string
  priority: number
  defaultAppointment: boolean
  status: TemplateStatus
}

export type ScheduleMode = 'MODE-A' | 'MODE-B' | 'MODE-C'

export interface WeeklyTemplateItem {
  id: string
  weekday: number
  weekdayLabel: string
  dailyTemplateId: string
  dailyTemplateName: string
  enabled: boolean
}

export interface WeeklyTemplate {
  id: string
  name: string
  isDefault: boolean
  appointmentOpen: boolean
  status: TemplateStatus
  items: WeeklyTemplateItem[]
}

export interface ReleasePolicy {
  id: string
  name: string
  enabled: boolean
  openDays: number
  openTime: string
  closeTime: string
  maxAdvanceDays: number
  maxPerPatient: number
  slotMinutes: number
  capacityPerSlot: number
}

export type HolidayExceptionType = 'closed' | 'replace' | 'reduced' | 'extra'

export interface HolidayException {
  id: string
  date: string
  rangeEnd?: string
  type: HolidayExceptionType
  replacementDailyTemplateId?: string
  replacementDailyTemplateName?: string
  reason: string
  autoApplyOnGenerate: boolean
}

export interface ScheduleTemplateInit {
  department: BookingOption
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
  serviceTypes: BookingOption[]
  dailyTemplates: BookingOption[]
}

export interface TimePeriodQuery {
  keyword?: string
  status?: TemplateStatus | null
  page?: number
  pageSize?: number
}

export interface DailyTemplateQuery {
  keyword?: string
  status?: TemplateStatus | null
  page?: number
  pageSize?: number
}

export interface WeeklyTemplateQuery {
  keyword?: string
  status?: TemplateStatus | null
  page?: number
  pageSize?: number
}

export interface TimePeriodSavePayload {
  id?: string
  name: string
  segments: Array<{ start: string; end: string; capacity: number }>
  status: TemplateStatus
}

export interface DailyTemplateSavePayload {
  id?: string
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

export interface WeeklyTemplateSavePayload {
  id?: string
  name: string
  isDefault: boolean
  appointmentOpen: boolean
  status: TemplateStatus
  items: Array<{
    weekday: number
    dailyTemplateId: string
    enabled: boolean
  }>
}

export interface HolidayExceptionSavePayload {
  id?: string
  date: string
  rangeEnd?: string
  type: HolidayExceptionType
  replacementDailyTemplateId?: string
  reason: string
  autoApplyOnGenerate: boolean
}

export interface RequestContext {
  signal?: AbortSignal
}
