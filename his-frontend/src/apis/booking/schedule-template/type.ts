export type TemplateStatus = 'enabled' | 'disabled'

export interface BookingOption {
  value: string
  label: string
}

export interface PageResult<T> {
  content: T[]
  totalCount: number
  pageIndex: number
  pageSize: number
}

export interface TimeSegment {
  id: string
  startTime?: string
  endTime?: string
  start: string
  end: string
  capacity: number
}

export interface TimePeriod {
  id: string
  organizationId?: string
  templateCode?: string
  templateName?: string
  slotTemplates?: TimeSegment[]
  enabled?: boolean
  name: string
  segments: TimeSegment[]
  status: TemplateStatus
  usageCount: number
}

export interface DailyTemplate {
  id: string
  organizationId?: string
  templateCode?: string
  templateName?: string
  name: string
  scheduleMode: ScheduleMode
  serviceType: string
  healthcareServiceId?: string
  healthcareServiceName?: string
  periodIds: string[]
  slotProvisionTemplateId?: string
  practitionerId?: string
  doctorId: string
  doctorName: string
  roomId: string
  locationId?: string
  roomName: string
  releasePolicyId: string
  defaultReleasePolicyId?: string
  effectiveStart: string
  effectiveEnd: string
  priority: number
  defaultAppointment: boolean
  enabled?: boolean
  sourcePractitionerRoleId?: string
  status: TemplateStatus
}

export type ScheduleMode = 'MODE-A' | 'MODE-B' | 'MODE-C'

export interface WeeklyTemplateItem {
  id: string
  weekday: number
  weekdayLabel: string
  dailyTemplateId: string
  dayScheduleTemplateId?: string
  dailyTemplateName: string
  enabled: boolean
  sortNo?: number
}

export interface WeeklyTemplate {
  id: string
  organizationId?: string
  templateCode?: string
  templateName?: string
  name: string
  isDefault: boolean
  appointmentOpen: boolean
  fallbackBookable?: boolean
  enabled?: boolean
  effectiveStart?: string
  effectiveEnd?: string
  status: TemplateStatus
  items: WeeklyTemplateItem[]
}

export interface ReleasePolicy {
  id: string
  organizationId?: string
  policyCode?: string
  name: string
  policyName?: string
  enabled: boolean
  openDays: number
  advanceDays?: number
  openTime: string
  closeTime: string
  maxAdvanceDays: number
  maxPerPatient: number
  slotMinutes: number
  capacityPerSlot: number
  channelQuotaJsonText?: string
  channelQuotas?: Record<string, number>
}

export interface ReleasePolicyQuery {
  organizationId?: string
  keyword?: string
  pageIndex: number
  pageSize: number
}

export interface SaveReleasePolicyRequest {
  id?: string
  policyCode?: string
  policyName: string
  advanceDays: number
  slotMinutes: number
  capacityPerSlot: number
  channelQuotaJson?: string
}

export type HolidayExceptionType = 'closed' | 'replace' | 'reduced' | 'extra'

export interface HolidayException {
  id: string
  organizationId?: string
  date: string
  exceptionDate?: string
  rangeEnd?: string
  type: HolidayExceptionType
  exceptionType?: HolidayExceptionType
  replacementDailyTemplateId?: string
  overrideScheduleTemplateId?: string
  replacementDailyTemplateName?: string
  reason: string
  autoApplyOnGenerate: boolean
}

export interface ScheduleTemplateInit {
  department: BookingOption
  organizationId?: string
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
  serviceTypes: BookingOption[]
  dailyTemplates: BookingOption[]
}

export interface TimePeriodQuery {
  organizationId?: string
  keyword?: string
  status?: TemplateStatus | null
  pageIndex?: number
  pageSize?: number
}

export interface PageTemplatesRequest {
  organizationId?: string
  keyword?: string
  pageIndex: number
  pageSize: number
}

export interface DailyTemplateQuery {
  organizationId?: string
  keyword?: string
  status?: TemplateStatus | null
  pageIndex?: number
  pageSize?: number
}

export interface WeeklyTemplateQuery {
  organizationId?: string
  keyword?: string
  status?: TemplateStatus | null
  pageIndex?: number
  pageSize?: number
}

export interface SaveSlotProvisionTemplateRequest {
  id?: string
  organizationId?: string
  templateCode: string
  templateName: string
  slotTemplates: Array<{ startTime: string; endTime: string; capacity: number }>
  enabled: boolean
}

export interface SaveScheduleTemplateRequest {
  id?: string
  organizationId: string
  templateCode?: string
  templateName: string
  scheduleMode: ScheduleMode
  practitionerId?: string
  locationId?: string
  healthcareServiceId: string
  slotProvisionTemplateId?: string
  defaultReleasePolicyId?: string
  effectiveStart?: string
  effectiveEnd?: string
  priority?: number
  enabled: boolean
}

export interface SaveWeekScheduleTemplateRequest {
  id?: string
  organizationId: string
  templateCode: string
  templateName: string
  fallbackBookable?: boolean
  isDefault?: boolean
  enabled: boolean
  effectiveStart?: string
  effectiveEnd?: string
  items: Array<{
    id?: string
    weekday: number
    dayScheduleTemplateId: string
    sortNo?: number
    enabled: boolean
  }>
}

export interface SaveCalendarExceptionRequest {
  id?: string
  organizationId?: string
  exceptionDate: string
  rangeEnd?: string
  exceptionType: 'CLOSED' | 'REDUCED' | 'OVERRIDE' | 'EXTRA'
  overrideScheduleTemplateId?: string
  reason?: string
  autoApplyOnGenerate?: boolean
}

export interface SaveEntityResult {
  id?: string
}

export type TimePeriodSavePayload = SaveSlotProvisionTemplateRequest
export type DailyTemplateSavePayload = SaveScheduleTemplateRequest
export type WeeklyTemplateSavePayload = SaveWeekScheduleTemplateRequest
export type HolidayExceptionSavePayload = SaveCalendarExceptionRequest
export type ReleasePolicySavePayload = SaveReleasePolicyRequest

export interface HolidayExceptionQuery {
  organizationId?: string
  exceptionType?: 'CLOSED' | 'REDUCED' | 'OVERRIDE' | 'EXTRA'
  rangeStart?: string
  rangeEnd?: string
  keyword?: string
  pageIndex: number
  pageSize: number
}

export interface RequestContext {
  signal?: AbortSignal
}
