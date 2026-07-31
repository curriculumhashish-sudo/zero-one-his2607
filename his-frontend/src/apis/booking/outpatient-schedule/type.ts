export type ScheduleStatus = 'draft' | 'published' | 'stopped'

export interface BookingOption extends Record<string, unknown> {
  value: string
  label: string
  departmentId?: string
  color?: string
}

export interface ScheduleRecord {
  /** 原型/后端排班主键。id 保留为页面兼容字段。 */
  scheduleId?: string
  id: string
  organizationId?: string
  departmentId: string
  departmentName: string
  name?: string
  practitionerId?: string
  doctorId: string
  doctorName: string
  locationId?: string
  roomId: string
  roomName: string
  healthcareServiceId?: string
  healthcareServiceName?: string
  practitionerName?: string
  locationName?: string
  date: string
  planningHorizonStart?: string
  planningHorizonEnd?: string
  startTime: string
  endTime: string
  capacity: number
  statusEnum?: ScheduleStatus
  status: ScheduleStatus
  bookable?: boolean
  actors?: Array<{ actorType: number; actorId: string; sortNo: number }>
  fromTemplate?: boolean
  weekScheduleTemplateId?: string
  dayScheduleTemplateId?: string
  sourceScheduleTemplateId?: string
  slotProvisionTemplateId?: string
  originalPractitionerId?: string
  substitutePractitionerId?: string
  substituteDisplay?: string
  stopReason?: string
  substituteReason?: string
  partialStoppedSlotIds?: string[]
  voidedFlag?: boolean
  updatedAt: string
}

export interface PageSchedulesByCalendarRequest {
  organizationId: string
  rangeStart: string
  rangeEnd: string
  practitionerId?: string | null
  locationId?: string | null
  healthcareServiceId?: string | null
}
export type ScheduleRangeQuery = PageSchedulesByCalendarRequest

export interface ScheduleCalendarDay {
  date: string
  inMonth: boolean
  activeCount: number
  draftCount: number
  inactiveCount: number
  schedules: ScheduleRecord[]
}

export interface ScheduleCalendarSummary {
  publishedDayCount: number
  draftDayCount: number
  unscheduledDayCount: number
  inactiveDayCount: number
  bookedAppointmentCount: number
  totalScheduleCount: number
}

export interface ScheduleCalendarResult {
  days: ScheduleCalendarDay[]
  records: ScheduleRecord[]
  yearMonth?: string
  monthStart?: string
  monthEnd?: string
  gridStart: string
  gridEnd: string
  summary: ScheduleCalendarSummary
}

export interface ScheduleActorRequest {
  actorType: number
  actorId: string
  sortNo?: number
}

export interface CreateScheduleRequest {
  organizationId: string
  name?: string
  actors: ScheduleActorRequest[]
  planningHorizonStart?: string
  planningHorizonEnd?: string
  slotProvisionTemplateId?: string
}

export interface CreateScheduleResponse { scheduleId?: string }

export interface UpdateScheduleDraftRequest {
  scheduleId: string
  name?: string
  actors?: ScheduleActorRequest[]
  planningHorizonStart?: string
  planningHorizonEnd?: string
  commentMarkdown?: string
  slotProvisionTemplateId?: string
}

export interface PublishScheduleRequest {
  scheduleId: string
  planningHorizonStart: string
  planningHorizonEnd: string
  acknowledgeConflictWarnings?: boolean
}

export interface DeactivateScheduleRequest { scheduleId: string; reason?: string }

export interface ScheduleInitResult {
  organizationId: string
  departments: BookingOption[]
  practitioners?: BookingOption[]
  doctors: BookingOption[]
  clinicLocations?: BookingOption[]
  rooms: BookingOption[]
  healthcareServices?: BookingOption[]
}

export interface RequestContext {
  signal?: AbortSignal
}

export interface GenerateFormalSchedulesPayload {
  organizationId: string
  scheduleTemplateId?: string
  weekScheduleTemplateId?: string
  templateKind: 'DAY' | 'WEEK'
  rangeStart: string
  rangeEnd: string
  autoPublish?: boolean
  autoProvision?: boolean
}

export interface GenerateFormalSchedulesResult {
  scheduleIds: string[]
  generatedCount: number
  skippedCount: number
  provisionedSlotCount: number
}

export interface ScheduleGenerationOptions {
  dailyTemplates: BookingOption[]
  weeklyTemplates: BookingOption[]
}

export interface ScheduleSubstitutionResult {
  scheduleId: string
  originalPractitionerId?: string
  substitutePractitionerId?: string | null
  substituteReason?: string
  cleared: boolean
}

export interface ReleasedSlotRecord {
  slotId: string
  scheduleId: string
  organizationId: string
  practitionerId: string
  healthcareServiceId: string
  practitionerName: string
  healthcareServiceName: string
  organizationName: string
  startTime: string
  endTime: string
  statusCode: string
  capacity: number
  bookedCount: number
  remainingCount: number
  displayStatus: string
  overbookedFlag: boolean
}

export interface ReleasedSlotPage {
  content: ReleasedSlotRecord[]
  totalCount: number
  pageNo: number
  pageSize: number
}

export interface ScheduleImpactItem {
  appointmentId: string
  busNo: string
  patientId: string
  patientName: string
  sourceSlotId: string
  sourceStartTime: string
  sourceEndTime: string
  targetSlotId?: string
  targetStartTime?: string
  targetEndTime?: string
  dispositionStatus: string
  dispositionMessage: string
}

export interface ScheduleImpactPreview {
  scheduleId: string
  scheduleCount: number
  slotCount: number
  appointmentCount: number
  registeredCount: number
  checkedInCount: number
  channelCount: number
  blockDeactivateIfUnhandled: boolean
  defaultDispositionMode: string
  transferMatchRule: string
  sourceSlotsFrozen: boolean
  frozenSlotCount: number
  freezeExpiresAt: string
  items: ScheduleImpactItem[]
}

export interface PartialStopSchedulePayload {
  organizationId: string
  scheduleId: string
  slotIds: string[]
  dispositionMode: 'TRANSFER' | 'CANCEL' | 'MANUAL'
  targetPractitionerId?: string
  transferItems?: Array<{ appointmentId: string; targetSlotId: string }>
  reason?: string
  deferManualHandling?: boolean
}

export interface PartialStopScheduleResult {
  successCount: number
  failedCount: number
  scheduleDeactivated: boolean
  blockedSlotCount: number
  successItems: Array<{ appointmentId: string; targetSlotId?: string; message: string }>
  failedItems: Array<{ appointmentId: string; targetSlotId?: string; message: string }>
}

export interface ResumeScheduleResult {
  scheduleId: string
  needReprovision: boolean
}

export interface ProvisionSlotsPayload {
  scheduleId: string
  organizationId: string
  slotTemplates: Array<{ startTime: string; endTime: string; capacity: number }>
}

export interface ProvisionSlotsResult {
  slotIds: string[]
  generatedCount: number
}
