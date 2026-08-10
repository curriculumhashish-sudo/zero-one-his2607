import HttpClient, { DataUpType } from '@/apis/http'
import * as mock from './mock'
import type {
  BookingOption,
  DailyTemplate,
  DailyTemplateQuery,
  DailyTemplateSavePayload,
  HolidayException,
  HolidayExceptionQuery,
  HolidayExceptionSavePayload,
  PageResult,
  ReleasePolicy,
  ReleasePolicyQuery,
  ReleasePolicySavePayload,
  RequestContext,
  ScheduleTemplateInit,
  TimePeriod,
  TimePeriodQuery,
  TimePeriodSavePayload,
  WeeklyTemplate,
  WeeklyTemplateQuery,
  WeeklyTemplateSavePayload
} from './type'

type LooseRecord = Record<string, any>
type Envelope<T> = { code?: number; message?: string; data?: T }

const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_J8_SETROSTER_MOCK === 'true'
const rosterHttp = new HttpClient(
  { baseURL: import.meta.env.VITE_J8_SETROSTER_SERVER_URL || '/j8-setroster' },
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
  if (response.code !== undefined && response.code !== 10000) throw new Error(response.message || '排班设置接口请求失败')
  if (response.data === undefined || response.data === null) throw new Error(response.message || '排班设置接口未返回数据')
  return response.data
}

async function unwrapVoid(request: Promise<Envelope<unknown>>) {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) throw new Error(response.message || '排班设置操作失败')
}

function requestConfig(context: RequestContext = {}) {
  return context.signal ? { signal: context.signal } : undefined
}

function page<T>(data: LooseRecord, map: (row: LooseRecord) => T, fallbackSize = 0): PageResult<T> {
  const rows = Array.isArray(data?.rows) ? data.rows : Array.isArray(data?.content) ? data.content : []
  return {
    content: rows.map(map),
    totalCount: Number(data?.totalCount ?? data?.total ?? rows.length),
    pageIndex: Number(data?.pageIndex ?? data?.pageNo ?? 1),
    pageSize: Number(data?.pageSize ?? fallbackSize)
  }
}

function toTimePeriod(row: LooseRecord): TimePeriod {
  const id = String(row.id ?? '')
  const segments = (Array.isArray(row.slotTemplates) ? row.slotTemplates : []).map((item: LooseRecord, index: number) => ({
    id: `${id}-${index + 1}`,
    start: String(item.startTime ?? item.start ?? ''),
    end: String(item.endTime ?? item.end ?? ''),
    startTime: String(item.startTime ?? item.start ?? ''),
    endTime: String(item.endTime ?? item.end ?? ''),
    capacity: Number(item.capacity ?? 0)
  }))
  return {
    id,
    organizationId: row.organizationId,
    templateCode: row.templateCode,
    templateName: row.templateName,
    name: String(row.templateName ?? row.templateCode ?? id),
    segments,
    slotTemplates: segments,
    status: row.enabled === false ? 'disabled' : 'enabled',
    enabled: row.enabled !== false,
    usageCount: 0
  }
}

function toDailyTemplate(row: LooseRecord): DailyTemplate {
  const id = String(row.id ?? '')
  return {
    id,
    organizationId: row.organizationId,
    templateCode: row.templateCode,
    templateName: row.templateName,
    name: String(row.templateName ?? row.templateCode ?? id),
    scheduleMode: row.scheduleMode ?? 'MODE-A',
    serviceType: String(row.healthcareServiceId ?? ''),
    healthcareServiceId: row.healthcareServiceId,
    periodIds: row.slotProvisionTemplateId ? [String(row.slotProvisionTemplateId)] : [],
    slotProvisionTemplateId: row.slotProvisionTemplateId,
    practitionerId: row.practitionerId,
    doctorId: String(row.practitionerId ?? ''),
    doctorName: String(row.practitionerName ?? row.practitionerId ?? ''),
    roomId: String(row.locationId ?? ''),
    locationId: row.locationId,
    roomName: String(row.locationName ?? row.locationId ?? ''),
    releasePolicyId: String(row.defaultReleasePolicyId ?? ''),
    defaultReleasePolicyId: row.defaultReleasePolicyId,
    effectiveStart: String(row.effectiveStart ?? ''),
    effectiveEnd: String(row.effectiveEnd ?? ''),
    priority: Number(row.priority ?? 0),
    defaultAppointment: false,
    enabled: row.enabled !== false,
    status: row.enabled === false ? 'disabled' : 'enabled'
  }
}

function weekdayLabel(value: number) {
  return ['星期日', '星期一', '星期二', '星期三', '星期四', '星期五', '星期六'][value] ?? '未知'
}

function toWeeklyTemplate(row: LooseRecord): WeeklyTemplate {
  const id = String(row.id ?? '')
  const items = (Array.isArray(row.items) ? row.items : []).map((item: LooseRecord, index: number) => ({
    id: String(item.id ?? `${id}-${index + 1}`),
    weekday: Number(item.weekday ?? 0),
    weekdayLabel: weekdayLabel(Number(item.weekday ?? 0)),
    dailyTemplateId: String(item.dayScheduleTemplateId ?? ''),
    dayScheduleTemplateId: item.dayScheduleTemplateId,
    dailyTemplateName: String(item.dayScheduleTemplateName ?? item.dayScheduleTemplateId ?? ''),
    enabled: item.enabled !== false,
    sortNo: item.sortNo
  }))
  return {
    id,
    organizationId: row.organizationId,
    templateCode: row.templateCode,
    templateName: row.templateName,
    name: String(row.templateName ?? row.templateCode ?? id),
    isDefault: Boolean(row.isDefault),
    appointmentOpen: row.fallbackBookable !== false,
    fallbackBookable: row.fallbackBookable,
    enabled: row.enabled !== false,
    effectiveStart: row.effectiveStart,
    effectiveEnd: row.effectiveEnd,
    status: row.enabled === false ? 'disabled' : 'enabled',
    items
  }
}

function toPolicy(row: LooseRecord): ReleasePolicy {
  const quotas = row.channelQuotas && typeof row.channelQuotas === 'object' ? row.channelQuotas : undefined
  return {
    id: String(row.id ?? ''),
    organizationId: row.organizationId,
    policyCode: row.policyCode,
    name: String(row.policyName ?? row.policyCode ?? row.id ?? ''),
    policyName: row.policyName,
    enabled: row.enabled !== false,
    openDays: Number(row.advanceDays ?? 0),
    advanceDays: Number(row.advanceDays ?? 0),
    openTime: '00:00',
    closeTime: '23:59',
    maxAdvanceDays: Number(row.advanceDays ?? 0),
    maxPerPatient: 0,
    slotMinutes: Number(row.slotMinutes ?? 0),
    capacityPerSlot: Number(row.capacityPerSlot ?? 0),
    channelQuotas: quotas,
    channelQuotaJsonText: quotas ? JSON.stringify(quotas) : undefined
  }
}

function toHoliday(row: LooseRecord): HolidayException {
  const typeMap: Record<string, HolidayException['type']> = { CLOSED: 'closed', REDUCED: 'reduced', OVERRIDE: 'replace', EXTRA: 'extra' }
  const type = typeMap[String(row.exceptionType ?? '').toUpperCase()] ?? 'closed'
  return {
    id: String(row.id ?? ''),
    organizationId: row.organizationId,
    date: String(row.exceptionDate ?? ''),
    exceptionDate: row.exceptionDate,
    rangeEnd: row.rangeEnd,
    type,
    exceptionType: type,
    replacementDailyTemplateId: row.overrideScheduleTemplateId,
    overrideScheduleTemplateId: row.overrideScheduleTemplateId,
    reason: String(row.reason ?? ''),
    autoApplyOnGenerate: Boolean(row.autoApplyOnGenerate)
  }
}

function parseQuota(value?: string) {
  if (!value?.trim()) return {}
  try { return JSON.parse(value) as Record<string, number> } catch { throw new Error('渠道配额必须是有效 JSON') }
}

export async function loadScheduleTemplateInit(context: RequestContext = {}): Promise<ScheduleTemplateInit> {
  if (useMock) return mock.mockLoadScheduleTemplateInit(context)
  const organizationId = (() => { try { return JSON.parse(localStorage.getItem('loginContext') ?? '{}').organizationId ?? '' } catch { return '' } })()
  let dailyTemplates: BookingOption[] = []
  try {
    const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/listDailyScheduleOptions', { organizationId, pageIndex: 1, pageSize: 200 }, requestConfig(context)))
    const rows = Array.isArray(data) ? data : Array.isArray(data?.content) ? data.content : Array.isArray(data?.rows) ? data.rows : []
    dailyTemplates = rows.map((item: LooseRecord) => ({ value: String(item.value ?? item.id ?? ''), label: String(item.label ?? item.templateName ?? '') }))
  } catch { /* 页面仍可打开，选项接口失败由具体列表提示 */ }
  const department = { value: organizationId, label: organizationId ? '当前机构' : '未选择机构' }
  return { department, organizationId, departments: organizationId ? [department] : [], doctors: [], rooms: [], serviceTypes: [], dailyTemplates }
}

export async function pageTimePeriods(query: TimePeriodQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageTimePeriods(query, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/querySlotProvisionTemplates', { keyword: query.keyword, organizationId: query.organizationId, enabled: query.status ? query.status === 'enabled' : undefined, pageIndex: query.pageIndex, pageSize: query.pageSize }, requestConfig(context)))
  return page(data, toTimePeriod, query.pageSize)
}

export async function saveTimePeriod(payload: TimePeriodSavePayload, context: RequestContext = {}) {
  if (useMock) return mock.mockSaveTimePeriod(payload, context)
  const body = { ...payload, slotTemplates: payload.slotTemplates.map((item) => ({ startTime: item.startTime, endTime: item.endTime, capacity: item.capacity })) }
  const path = payload.id ? '/j8/setroster/modifySlotProvisionTemplate' : '/j8/setroster/addSlotProvisionTemplate'
  const data = await unwrap<LooseRecord>(payload.id ? rosterHttp.put(path, body, requestConfig(context)) : rosterHttp.post(path, body, requestConfig(context)))
  return { id: String(data?.id ?? payload.id ?? '') }
}

export async function removeTimePeriod(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockRemoveTimePeriod(id, context)
  return unwrapVoid(rosterHttp.delete(`/j8/setroster/deleteSlotProvisionTemplate/${encodeURIComponent(id)}`, undefined, requestConfig(context)))
}

export async function pageDailyTemplates(query: DailyTemplateQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageDailyTemplates(query, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/pageDailySchedule', { keyword: query.keyword, organizationId: query.organizationId, pageIndex: query.pageIndex, pageSize: query.pageSize }, requestConfig(context)))
  return page(data, toDailyTemplate, query.pageSize ?? 0)
}

export async function saveDailyTemplate(payload: DailyTemplateSavePayload, context: RequestContext = {}) {
  if (useMock) return mock.mockSaveDailyTemplate(payload, context)
  const path = payload.id ? '/j8/setroster/modifyDailySchedule' : '/j8/setroster/addDailySchedule'
  const data = await unwrap<LooseRecord>(payload.id ? rosterHttp.put(path, payload, requestConfig(context)) : rosterHttp.post(path, payload, requestConfig(context)))
  return { id: String(data?.id ?? payload.id ?? '') }
}

export async function removeDailyTemplate(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockRemoveDailyTemplate(id, context)
  return unwrapVoid(rosterHttp.delete(`/j8/setroster/deleteDailySchedule/${encodeURIComponent(id)}`, undefined, requestConfig(context)))
}

export async function pageWeeklyTemplates(query: WeeklyTemplateQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageWeeklyTemplates(query, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/pageWeekScheduleTemplate', { keyword: query.keyword, organizationId: query.organizationId, enabled: query.status ? query.status === 'enabled' : undefined, pageIndex: query.pageIndex, pageSize: query.pageSize }, requestConfig(context)))
  return page(data, toWeeklyTemplate, query.pageSize ?? 0)
}

export async function saveWeeklyTemplate(payload: WeeklyTemplateSavePayload, context: RequestContext = {}) {
  if (useMock) return mock.mockSaveWeeklyTemplate(payload, context)
  const path = payload.id ? '/j8/setroster/modifyWeekScheduleTemplate' : '/j8/setroster/addWeekScheduleTemplate'
  const data = await unwrap<LooseRecord>(payload.id ? rosterHttp.put(path, payload, requestConfig(context)) : rosterHttp.post(path, payload, requestConfig(context)))
  return { id: String(data?.id ?? payload.id ?? '') }
}

export async function removeWeeklyTemplate(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockRemoveWeeklyTemplate(id, context)
  return unwrapVoid(rosterHttp.delete(`/j8/setroster/deleteWeekScheduleTemplate/${encodeURIComponent(id)}`, undefined, requestConfig(context)))
}

export async function getReleasePolicy(context: RequestContext = {}) {
  if (useMock) return mock.mockGetReleasePolicy(context)
  const result = await pageReleasePolicies({ pageIndex: 1, pageSize: 1 }, context)
  if (!result.content[0]) throw new Error('尚未配置放号策略')
  return result.content[0]
}

export async function pageReleasePolicies(query: ReleasePolicyQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageReleasePolicies(query, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/queryAllocationStrategies', { keyword: query.keyword, organizationId: query.organizationId, pageIndex: query.pageIndex, pageSize: query.pageSize }, requestConfig(context)))
  return page(data, toPolicy, query.pageSize)
}

export async function saveReleasePolicy(payload: ReleasePolicySavePayload, context: RequestContext = {}) {
  if (useMock) return mock.mockSaveReleasePolicy(payload, context)
  const body = { ...payload, channelQuotaJson: parseQuota(payload.channelQuotaJson) }
  const path = payload.id ? '/j8/setroster/modifyAllocationStrategy' : '/j8/setroster/addAllocationStrategy'
  const data = await unwrap<LooseRecord>(payload.id ? rosterHttp.put(path, body, requestConfig(context)) : rosterHttp.post(path, body, requestConfig(context)))
  return { id: String(data?.id ?? payload.id ?? '') }
}

export async function removeReleasePolicy(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockRemoveReleasePolicy(id, context)
  return unwrapVoid(rosterHttp.delete(`/j8/setroster/deleteAllocationStrategy/${encodeURIComponent(id)}`, undefined, requestConfig(context)))
}

export async function listHolidayExceptions(range: { start: string; end: string }, context: RequestContext = {}) {
  if (useMock) return mock.mockListHolidayExceptions(range, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/queryCalendarExceptions', { rangeStart: range.start, rangeEnd: range.end, pageIndex: 1, pageSize: 500 }, requestConfig(context)))
  const rows = Array.isArray(data) ? data : Array.isArray(data?.content) ? data.content : Array.isArray(data?.rows) ? data.rows : []
  return rows.map((row: LooseRecord) => toHoliday(row))
}

export async function pageHolidayExceptions(query: HolidayExceptionQuery, context: RequestContext = {}) {
  if (useMock) return mock.mockPageHolidayExceptions(query, context)
  const data = await unwrap<LooseRecord>(rosterHttp.get('/j8/setroster/queryCalendarExceptions', query, requestConfig(context)))
  if (Array.isArray(data)) {
    const start = (query.pageIndex - 1) * query.pageSize
    return {
      content: data.slice(start, start + query.pageSize).map((row: LooseRecord) => toHoliday(row)),
      totalCount: data.length,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    }
  }
  return page(data, toHoliday, query.pageSize)
}

export async function saveHolidayException(payload: HolidayExceptionSavePayload, context: RequestContext = {}) {
  if (useMock) return mock.mockSaveHolidayException(payload, context)
  const path = payload.id ? '/j8/setroster/modifyCalendarException' : '/j8/setroster/addCalendarException'
  const data = await unwrap<LooseRecord>(payload.id ? rosterHttp.put(path, payload, requestConfig(context)) : rosterHttp.post(path, payload, requestConfig(context)))
  return { id: String(data?.id ?? payload.id ?? '') }
}

export async function removeHolidayException(id: string, context: RequestContext = {}) {
  if (useMock) return mock.mockRemoveHolidayException(id, context)
  return unwrapVoid(rosterHttp.delete(`/j8/setroster/deleteCalendarException/${encodeURIComponent(id)}`, undefined, requestConfig(context)))
}

export type * from './type'
