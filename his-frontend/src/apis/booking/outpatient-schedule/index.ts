import HttpClient, { DataUpType } from '@/apis/http'
import * as mock from './mock'
import type {
  BookingOption,
  CreateScheduleRequest,
  DeactivateScheduleRequest,
  GenerateFormalSchedulesPayload,
  GenerateFormalSchedulesResult,
  PartialStopSchedulePayload,
  PartialStopScheduleResult,
  ProvisionSlotsPayload,
  ProvisionSlotsResult,
  ReleasedSlotPage,
  ReleasedSlotRecord,
  ListReleasedSlotsQuery,
  RequestContext,
  ScheduleActorRequest,
  ScheduleCalendarResult,
  ScheduleGenerationOptions,
  ScheduleImpactPreview,
  ScheduleInitResult,
  ScheduleRangeQuery,
  ScheduleRecord,
  ScheduleSubstitutionResult,
  UpdateScheduleDraftRequest,
  PublishScheduleRequest,
  ResumeScheduleResult
} from './type'
import {
  loadScheduleTemplateInit,
  pageDailyTemplates,
  pageTimePeriods,
  pageWeeklyTemplates
} from '../schedule-template'
import { loadDoctorOptions as loadCalendarDoctorOptions } from '../slot-calendar'

type LooseRecord = Record<string, any>
type Envelope<T> = { code?: number; message?: string; data?: T }
const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_J8_CLINICS_MOCK === 'true'
const clinicsHttp = new HttpClient(
  { baseURL: import.meta.env.VITE_J8_CLINICS_SERVER_URL || '/j8-clinics' },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token')
      const tokenHead = typeof localStorage === 'undefined' ? 'Bearer' : localStorage.getItem('tokenHead') || 'Bearer'
      if (token) config.headers.Authorization = `${tokenHead} ${token}`
      if (config.upType === DataUpType.json) config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

async function unwrap<T>(request: Promise<Envelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) throw new Error(response.message || '门诊排班接口请求失败')
  if (response.data === undefined || response.data === null) throw new Error(response.message || '门诊排班接口未返回数据')
  return response.data
}

async function unwrapVoid(request: Promise<Envelope<unknown>>) {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) throw new Error(response.message || '门诊排班操作失败')
}

function requestConfig(context: RequestContext = {}) {
  return context.signal ? { signal: context.signal } : undefined
}

function actorId(actors: any[], type: number) {
  return String(actors?.find((actor) => Number(actor.actorType) === type)?.actorId ?? '')
}

function statusOf(value: unknown): ScheduleRecord['status'] {
  const status = String(value ?? '').toLowerCase()
  if (status === 'draft') return 'draft'
  if (status === 'stopped' || status === 'inactive' || status === 'retired') return 'stopped'
  return 'published'
}

function toRecord(row: LooseRecord, dateHint = ''): ScheduleRecord {
  const actors = Array.isArray(row.actors) ? row.actors : []
  const practitionerId = String(row.practitionerId ?? row.originalPractitionerId ?? actorId(actors, 10))
  const locationId = String(row.locationId ?? actorId(actors, 20))
  const healthcareServiceId = String(row.healthcareServiceId ?? actorId(actors, 30))
  // 后端 organizationId 在本项目约定中就是科室 ID；healthcareServiceId 仍保留为号类/医疗服务 ID。
  const organizationId = String(row.organizationId ?? '')
  const start = String(row.planningHorizonStart ?? row.startTime ?? '')
  const end = String(row.planningHorizonEnd ?? row.endTime ?? '')
  const status = statusOf(row.statusEnum)
  return {
    scheduleId: String(row.scheduleId ?? row.id ?? ''),
    id: String(row.scheduleId ?? row.id ?? ''),
    organizationId: organizationId || undefined,
    departmentId: organizationId || healthcareServiceId,
    departmentName: String(row.organizationName ?? row.healthcareServiceName ?? (organizationId || healthcareServiceId)),
    name: row.name,
    practitionerId,
    doctorId: practitionerId,
    doctorName: String(row.practitionerName ?? practitionerId),
    practitionerName: row.practitionerName,
    locationId,
    roomId: locationId,
    roomName: String(row.locationName ?? locationId),
    locationName: row.locationName,
    healthcareServiceId,
    healthcareServiceName: row.healthcareServiceName,
    // 列表响应的 day.date 才是日历当天；planningHorizonStart 只是排班有效期起点。
    date: String(row.date || dateHint || start.slice(0, 10) || ''),
    planningHorizonStart: start,
    planningHorizonEnd: end,
    startTime: start.includes('T') ? start.slice(11, 16) : start.slice(0, 5),
    endTime: end.includes('T') ? end.slice(11, 16) : end.slice(0, 5),
    capacity: Number(row.capacity ?? 0),
    statusEnum: status,
    status,
    bookable: row.bookable ?? status === 'published',
    actors: actors.map((actor: any) => ({ actorType: Number(actor.actorType), actorId: String(actor.actorId ?? ''), sortNo: Number(actor.sortNo ?? 0) })),
    fromTemplate: row.fromTemplate,
    weekScheduleTemplateId: row.weekScheduleTemplateId,
    dayScheduleTemplateId: row.dayScheduleTemplateId,
    sourceScheduleTemplateId: row.sourceScheduleTemplateId,
    slotProvisionTemplateId: row.slotProvisionTemplateId,
    originalPractitionerId: row.originalPractitionerId,
    substitutePractitionerId: row.substitutePractitionerId,
    substituteDisplay: row.substituteDisplay,
    updatedAt: String(row.updatedAt ?? '')
  }
}

function toCalendar(data: LooseRecord): ScheduleCalendarResult {
  const days = (Array.isArray(data?.days) ? data.days : []).map((day: LooseRecord) => ({
    date: String(day.date ?? ''),
    inMonth: Boolean(day.inMonth),
    activeCount: Number(day.activeCount ?? 0),
    draftCount: Number(day.draftCount ?? 0),
    inactiveCount: Number(day.inactiveCount ?? 0),
    schedules: (Array.isArray(day.schedules) ? day.schedules : []).map((row: LooseRecord) => toRecord(row, String(day.date ?? '')))
  }))
  return {
    days,
    records: days.flatMap((day) => day.schedules),
    yearMonth: data?.yearMonth,
    monthStart: data?.monthStart,
    monthEnd: data?.monthEnd,
    gridStart: String(data?.gridStart ?? data?.monthStart ?? ''),
    gridEnd: String(data?.gridEnd ?? data?.monthEnd ?? ''),
    summary: {
      publishedDayCount: Number(data?.summary?.publishedDayCount ?? 0),
      draftDayCount: Number(data?.summary?.draftDayCount ?? 0),
      unscheduledDayCount: Number(data?.summary?.unscheduledDayCount ?? 0),
      inactiveDayCount: Number(data?.summary?.inactiveDayCount ?? 0),
      bookedAppointmentCount: Number(data?.summary?.bookedAppointmentCount ?? 0),
      totalScheduleCount: Number(data?.summary?.totalScheduleCount ?? 0)
    }
  }
}

function organizationIdOf(query?: { organizationId?: string }) {
  if (query?.organizationId) return query.organizationId
  try {
    const context = JSON.parse(localStorage.getItem('loginContext') ?? '{}') as { organizationId?: string; departmentId?: string }
    return context.organizationId || context.departmentId || String(import.meta.env.VITE_J8_DEFAULT_ORGANIZATION_ID ?? '').trim()
  } catch {
    return String(import.meta.env.VITE_J8_DEFAULT_ORGANIZATION_ID ?? '').trim()
  }
}

export async function loadScheduleInit(context: RequestContext = {}): Promise<ScheduleInitResult> {
  if (useMock) return mock.mockLoadScheduleInit(context)
  const templateInit = await loadScheduleTemplateInit(context)
  const organizationId = templateInit.organizationId ?? organizationIdOf()
  const toOption = (option: { value: string; label: string }): BookingOption => ({
    value: option.value,
    label: option.label
  })
  const toDoctorOption = (option: { value: string; label: string; departmentId?: string }): BookingOption => ({
    value: option.value,
    label: option.label,
    departmentId: option.departmentId ?? organizationId
  })
  return {
    organizationId,
    departments: templateInit.departments.map(toOption),
    practitioners: templateInit.doctors.map(toDoctorOption),
    doctors: templateInit.doctors.map(toDoctorOption),
    clinicLocations: templateInit.rooms.map(toOption),
    rooms: templateInit.rooms.map(toOption),
    healthcareServices: templateInit.serviceTypes.map(toOption)
  }
}

export async function pageSchedulesByRange(query: ScheduleRangeQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageSchedulesByRange(query, context)
  const data = await unwrap<LooseRecord>(clinicsHttp.get('/query-schedule', query, requestConfig(context)))
  return toCalendar(data)
}

export async function getScheduleDetail(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockGetScheduleDetail(id, context)
  return toRecord(await unwrap<LooseRecord>(clinicsHttp.get(`/query-schedule-detail/${encodeURIComponent(id)}`, undefined, requestConfig(context))))
}

export async function createSchedule(payload: CreateScheduleRequest, context: RequestContext = {}) {
  if (useMock) return mock.mockCreateSchedule(payload)
  const body = { ...payload, actors: payload.actors.map((actor) => ({ ...actor, actorType: String(actor.actorType) })) }
  const data = await unwrap<LooseRecord>(clinicsHttp.post('/add-schedule', body, requestConfig(context)))
  return { scheduleId: String(data?.scheduleId ?? '') }
}

export async function updateSchedule(payload: UpdateScheduleDraftRequest, context: RequestContext = {}) {
  if (useMock) return mock.mockUpdateSchedule(payload)
  throw new Error('J8 门诊排班接口未提供草稿修改接口')
}

export async function publishSchedule(payload: PublishScheduleRequest, context: RequestContext = {}) {
  if (useMock) return mock.mockPublishSchedule(payload)
  throw new Error('J8 门诊排班接口未提供单独发布接口')
}

export async function stopSchedule(payload: DeactivateScheduleRequest, context: RequestContext = {}) {
  if (useMock) return mock.mockStopSchedule(payload)
  const organizationId = organizationIdOf(payload)
  await unwrap(clinicsHttp.post('/j8/clinics/execute-stop-all-day', { scheduleId: payload.scheduleId, organizationId, reason: payload.reason, sendNotification: false }, requestConfig(context)))
}

export async function loadDepartmentOptions(keyword = '', context: RequestContext = {}) {
  if (useMock) return mock.mockLoadDepartmentOptions(keyword, context)
  const init = await loadScheduleInit(context)
  return init.departments.filter((item) => !keyword || item.label.toLowerCase().includes(keyword.toLowerCase()))
}

export async function loadDoctorOptions(keyword = '', departmentId?: string | null, context: RequestContext = {}) {
  if (useMock) return mock.mockLoadDoctorOptions(keyword, departmentId, context)
  const requestedDepartmentId = String(departmentId ?? '').trim()
  const loadFromDepartment = async (sourceDepartmentId: string) => {
    try {
      const options = await loadCalendarDoctorOptions(keyword, sourceDepartmentId, context)
      if (options.length) return options
    } catch (error) {
      if (context.signal?.aborted) throw error
    }
    // 日历服务人员接口为空时，用已维护的日排班模板补充真实人员，不制造前端假数据。
    try {
      const fallback = await pageDailyTemplates(
        { organizationId: sourceDepartmentId || undefined, pageIndex: 1, pageSize: 200 },
        context
      )
      const normalizedKeyword = keyword.trim().toLowerCase()
      return fallback.content
        .map((row) => ({ value: row.doctorId, label: row.doctorName || row.doctorId }))
        .filter((item) => !normalizedKeyword || item.label.toLowerCase().includes(normalizedKeyword))
    } catch (error) {
      if (context.signal?.aborted) throw error
      return []
    }
  }
  let options = await loadFromDepartment(requestedDepartmentId)
  const defaultDepartmentId = String(import.meta.env.VITE_J8_DEFAULT_ORGANIZATION_ID ?? '').trim()
  if (!options.length && defaultDepartmentId && defaultDepartmentId !== requestedDepartmentId) {
    options = await loadFromDepartment(defaultDepartmentId)
  }
  return options.map((option) => ({
    ...option,
    departmentId: requestedDepartmentId || defaultDepartmentId || undefined
  }))
}

export async function generateFormalSchedules(payload: GenerateFormalSchedulesPayload, context: RequestContext = {}): Promise<GenerateFormalSchedulesResult> {
  if (useMock) {
    const [dailyTemplates, weeklyTemplates, timePeriods] = await Promise.all([
      pageDailyTemplates({ organizationId: payload.organizationId, pageIndex: 1, pageSize: 200 }, context),
      pageWeeklyTemplates({ organizationId: payload.organizationId, pageIndex: 1, pageSize: 200 }, context),
      pageTimePeriods({ organizationId: payload.organizationId, pageIndex: 1, pageSize: 200 }, context)
    ])
    return mock.mockGenerateFormalSchedules(payload, { dailyTemplates: dailyTemplates.content, weeklyTemplates: weeklyTemplates.content, timePeriods: timePeriods.content }, context)
  }
  return unwrap<GenerateFormalSchedulesResult>(clinicsHttp.post('/generate-formal-roster', payload, requestConfig(context)))
}

export async function setScheduleSubstitution(scheduleId: string, substitutePractitionerId: string, reason = '', context: RequestContext = {}): Promise<ScheduleSubstitutionResult> {
  if (useMock) return mock.mockSetScheduleSubstitution(scheduleId, substitutePractitionerId, reason)
  return unwrap<ScheduleSubstitutionResult>(clinicsHttp.post('/j8/clinics/set-substitute', { scheduleId, substitutePractitionerId, reason }, requestConfig(context)))
}

export async function clearScheduleSubstitution(scheduleId: string, context: RequestContext = {}) {
  if (useMock) return mock.mockClearScheduleSubstitution(scheduleId)
  return unwrap<ScheduleSubstitutionResult>(clinicsHttp.post('/j8/clinics/cancel-substitute', undefined, { ...requestConfig(context), params: { scheduleId } }))
}

export async function previewScheduleImpact(scheduleId: string, context: RequestContext = {}): Promise<ScheduleImpactPreview> {
  if (useMock) return mock.mockPreviewScheduleImpact(scheduleId)
  throw new Error(`J8 门诊排班接口未提供停诊影响预览接口（排班：${scheduleId}）`)
}

export async function partialStopSchedule(payload: PartialStopSchedulePayload, context: RequestContext = {}): Promise<PartialStopScheduleResult> {
  if (useMock) return mock.mockPartialStopSchedule(payload)
  return unwrap<PartialStopScheduleResult>(clinicsHttp.post('/j8/clinics/execute-stop-partial', { ...payload, sendNotification: false }, requestConfig(context)))
}

export async function unlockPartialStop(organizationId: string, scheduleId: string, slotIds: string[] = [], context: RequestContext = {}) {
  if (useMock) return mock.mockUnlockPartialStop(organizationId, scheduleId, slotIds)
  await unwrap(clinicsHttp.post('/j8/clinics/unlock-stop-clinic', { scheduleId, organizationId, slotIds }, requestConfig(context)))
}

export async function resumeSchedule(scheduleId: string, context: RequestContext = {}): Promise<ResumeScheduleResult> {
  if (useMock) return mock.mockResumeSchedule(scheduleId)
  return unwrap<ResumeScheduleResult>(clinicsHttp.post(`/j8/clinics/reactivate-schedule/${encodeURIComponent(scheduleId)}`, undefined, requestConfig(context)))
}

export async function voidSchedule(scheduleId: string, reason = '', context: RequestContext = {}) {
  if (useMock) return mock.mockVoidSchedule(scheduleId, reason)
  return unwrapVoid(clinicsHttp.delete(`/j8/clinics/retire-schedule/${encodeURIComponent(scheduleId)}`, { scheduleId, reason }, requestConfig(context)))
}

function toReleasedSlot(row: LooseRecord): ReleasedSlotRecord {
  return {
    slotId: String(row.slotId ?? row.id ?? ''),
    scheduleId: String(row.scheduleId ?? ''),
    organizationId: String(row.organizationId ?? ''),
    practitionerId: String(row.practitionerId ?? ''),
    healthcareServiceId: String(row.healthcareServiceId ?? ''),
    practitionerName: String(row.practitionerName ?? ''),
    healthcareServiceName: String(row.healthcareServiceName ?? ''),
    organizationName: String(row.organizationName ?? ''),
    startTime: String(row.startTime ?? ''),
    endTime: String(row.endTime ?? ''),
    statusCode: String(row.statusCode ?? '').toUpperCase(),
    capacity: Number(row.capacity ?? 0),
    bookedCount: Number(row.bookedCount ?? 0),
    remainingCount: Number(row.remainingCount ?? 0),
    displayStatus: String(row.displayStatus ?? ''),
    overbookedFlag: Boolean(row.overbookedFlag)
  }
}

export async function listReleasedSlots(query: ListReleasedSlotsQuery, context: RequestContext = {}): Promise<ReleasedSlotPage> {
  if (useMock) return mock.mockListReleasedSlots(query)
  const data = await unwrap<LooseRecord>(clinicsHttp.post('/j8/clinics/get-rel-number-source', { ...query, organizationId: organizationIdOf(query) }, requestConfig(context)))
  const rows = Array.isArray(data?.content) ? data.content : Array.isArray(data?.rows) ? data.rows : Array.isArray(data) ? data : []
  return { content: rows.map(toReleasedSlot), totalCount: Number(data?.totalCount ?? data?.total ?? rows.length), pageIndex: Number(data?.pageIndex ?? data?.pageNo ?? query.pageIndex), pageSize: Number(data?.pageSize ?? query.pageSize) }
}

export async function provisionScheduleSlots(payload: ProvisionSlotsPayload, context: RequestContext = {}): Promise<ProvisionSlotsResult> {
  if (useMock) return mock.mockProvisionScheduleSlots(payload)
  return unwrap<ProvisionSlotsResult>(clinicsHttp.post('/j8/clinics/batch-release-numberSource', payload, requestConfig(context)))
}

export async function deleteReleasedSlots(slotIds: string[], context: RequestContext = {}) {
  if (useMock) return mock.mockDeleteReleasedSlots(slotIds)
  return unwrapVoid(clinicsHttp.delete('/j8/clinics/delete-number-source', { slotIds }, requestConfig(context)))
}

export async function loadScheduleGenerationOptions(organizationId: string, context: RequestContext = {}): Promise<ScheduleGenerationOptions> {
  const [dailyTemplates, weeklyTemplates] = await Promise.all([
    pageDailyTemplates({ organizationId, pageIndex: 1, pageSize: 200 }, context),
    pageWeeklyTemplates({ organizationId, pageIndex: 1, pageSize: 200 }, context)
  ])
  return {
    dailyTemplates: dailyTemplates.content.map((item) => ({ value: item.id, label: item.templateName ?? item.name })),
    weeklyTemplates: weeklyTemplates.content.map((item) => ({ value: item.id, label: item.templateName ?? item.name }))
  }
}

export type * from './type'
