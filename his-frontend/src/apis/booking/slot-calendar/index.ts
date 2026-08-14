import HttpClient, { DataUpType } from '@/apis/http'
import {
  mockBatchBlockSlots,
  mockBatchUnblockSlots,
  mockGetSlotDetail,
  mockLoadSlotDepartmentOptions,
  mockLoadSlotDoctorOptions,
  mockLoadSlotInit,
  mockMarkSlotOverbooked,
  mockPageSlotsByDate
} from './mock'
import type {
  BatchBlockSlotsPayload,
  BatchUnblockSlotsPayload,
  BookingOption,
  LoadSlotDetailRequest,
  MarkSlotOverbookedPayload,
  RequestContext,
  SlotDetailDto,
  SlotInitResult,
  SlotListItemDto,
  SlotPageResponse,
  SlotPageResult,
  SlotQuery,
  SlotRecord,
  SlotStatus
} from './type'

const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_J8_CALENDAR_MOCK === 'true'

const calendarHttp = new HttpClient(
  { baseURL: import.meta.env.VITE_J8_CALENDAR_SERVER_URL || '/j8-calendar' },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token')
      const tokenHead = typeof localStorage === 'undefined' ? 'Bearer' : (localStorage.getItem('tokenHead') || 'Bearer')
      if (token) config.headers.Authorization = tokenHead + ' ' + token
      if (config.upType === DataUpType.json) config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

type BackendEnvelope<T> = { code?: number; message?: string; data?: T }

async function unwrap<T>(request: Promise<BackendEnvelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '号源日历接口请求失败')
  }
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '号源日历接口未返回数据')
  }
  return response.data
}

async function unwrapVoid(request: Promise<BackendEnvelope<unknown>>): Promise<void> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '号源状态操作失败')
  }
}

function requestConfig(context: RequestContext) {
  return context.signal ? { signal: context.signal } : undefined
}

// 后端该接口沿用 organizationId 字段承载科室 ID；页面内部仍使用 departmentId，
// 字段转换只放在接口适配层，避免影响其他接口的 organizationId 语义。
const practitionerOptionsPath =
  import.meta.env.VITE_J8_CALENDAR_PRACTITIONER_OPTIONS_PATH || '/j8/calendar/listPractitioners'

function toDoctorOptions(data: unknown, keyword = ''): BookingOption[] {
  const raw = data as { list?: unknown; content?: unknown } | unknown[] | null | undefined
  const rows = Array.isArray(raw)
    ? raw
    : Array.isArray(raw?.list)
      ? raw.list
      : Array.isArray(raw?.content)
        ? raw.content
        : []
  const normalizedKeyword = keyword.trim().toLowerCase()
  return rows
    .map((row) => {
      const item = row as Record<string, unknown>
      const value = String(item.practitionerId ?? item.doctorId ?? item.id ?? '')
      const label = String(item.practitionerName ?? item.doctorName ?? item.name ?? value)
      return { value, label, practitionerId: value, practitionerName: label }
    })
    .filter((item) => item.value && (!normalizedKeyword || item.label.toLowerCase().includes(normalizedKeyword)))
}

function localLoginContext() {
  try {
    return JSON.parse(localStorage.getItem('loginContext') ?? '{}') as { departmentId?: string | null }
  } catch {
    return {}
  }
}

function defaultOrganizationId() {
  return String(import.meta.env.VITE_J8_DEFAULT_ORGANIZATION_ID ?? '').trim()
}

function groupOf(startTime: string): SlotRecord['group'] {
  const hour = Number(startTime.slice(11, 13) || startTime.slice(0, 2))
  if (hour < 12) return '上午'
  if (hour < 18) return '下午'
  return '晚间'
}

function isExpired(startTime: string, endTime: string) {
  const value = endTime || startTime
  if (!value) return false
  const normalized = value.includes('T') ? value : value.replace(' ', 'T')
  const timestamp = Date.parse(normalized)
  return Number.isFinite(timestamp) && timestamp <= Date.now()
}

export function normalizeSlotStatus(item: SlotListItemDto): SlotStatus {
  const code = String(item.statusCode ?? '').toLowerCase()
  const display = String(item.displayStatus ?? '')
  if (display.includes('封锁') || display.includes('锁定')) return 'blocked'
  if (display.includes('停诊')) return 'stopped'
  if (code.includes('block') || code.includes('封锁') || code === 'busy-unavailable') return 'blocked'
  if (code.includes('stop')) return 'stopped'
  if (isExpired(String(item.startTime ?? ''), String(item.endTime ?? ''))) return 'expired'
  if (code === 'busy' || code === 'full' || Number(item.remainingCount ?? 0) <= 0 || display.includes('已满')) return 'full'
  return 'available'
}

function toSlotRecord(item: SlotListItemDto, fallbackDate: string): SlotRecord {
  const slotId = String(item.slotId ?? '')
  const organizationId = String(item.organizationId ?? '')
  const practitionerId = String(item.practitionerId ?? '')
  const startTime = String(item.startTime ?? '')
  const endTime = String(item.endTime ?? '')
  const status = normalizeSlotStatus(item)
  const bookedCount = Number(item.bookedCount ?? 0)
  const remainingCount = Number(item.remainingCount ?? 0)
  return {
    slotId,
    id: slotId,
    scheduleId: String(item.scheduleId ?? ''),
    organizationId,
    departmentId: organizationId,
    departmentName: String(item.organizationName ?? ''),
    practitionerId,
    practitionerName: String(item.practitionerName ?? ''),
    doctorId: practitionerId,
    doctorName: String(item.practitionerName ?? ''),
    healthcareServiceId: String(item.healthcareServiceId ?? ''),
    healthcareServiceName: String(item.healthcareServiceName ?? ''),
    organizationName: String(item.organizationName ?? ''),
    roomName: '',
    date: startTime.slice(0, 10) || fallbackDate,
    group: groupOf(startTime),
    used: bookedCount,
    bookedCount,
    remainingCount,
    remaining: remainingCount,
    capacity: Number(item.capacity ?? 0),
    startTime,
    endTime,
    statusCode: String(item.statusCode ?? ''),
    status,
    displayStatus: String(item.displayStatus ?? ''),
    overbookedFlag: Boolean(item.overbookedFlag),
    blockedFlag: status === 'blocked',
    commentText: undefined,
    unavailableReason: status === 'stopped' || status === 'blocked' || status === 'full' || status === 'expired'
      ? String(item.displayStatus ?? '')
      : undefined
  }
}

function toPageResult(data: SlotPageResponse, fallbackDate: string): SlotPageResult {
  const pageIndex = Number(data.pageIndex ?? 1)
  const pageSize = Number(data.pageSize ?? 0)
  return {
    content: (data.content ?? []).map((item) => toSlotRecord(item, fallbackDate)),
    pageIndex,
    pageSize,
    totalCount: Number(data.totalCount ?? 0),
    pages: data.pages == null ? undefined : Number(data.pages),
    stats: data.stats
  }
}

export const loadSlotInit = async (context: RequestContext = {}): Promise<SlotInitResult> => {
  if (useMock) return mockLoadSlotInit(context)
  const departmentId = localLoginContext().departmentId || defaultOrganizationId()
  return {
    organizationId: departmentId,
    departments: departmentId ? [{ value: departmentId, label: '当前登录科室' }] : [],
    doctors: []
  }
}

export const pageSlotsByDate = async (query: SlotQuery, context: RequestContext = {}): Promise<SlotPageResult> => {
  if (useMock) return mockPageSlotsByDate(query, context)
  const data = await unwrap(calendarHttp.get<SlotPageResponse>('/j8/calendar/pageSlots', {
    organizationId: query.organizationId,
    date: query.date,
    ...(query.scheduleId ? { scheduleId: query.scheduleId } : {}),
    ...(query.practitionerId ? { practitionerId: query.practitionerId } : {}),
    ...(query.statusCode ? { statusCode: query.statusCode } : {}),
    pageIndex: query.pageIndex,
    pageSize: query.pageSize
  }, requestConfig(context)))
  return toPageResult(data, query.date)
}

export const getSlotDetail = async (request: LoadSlotDetailRequest, context: RequestContext = {}): Promise<SlotRecord> => {
  if (useMock) return mockGetSlotDetail(request, context)
  const data = await unwrap(calendarHttp.get<SlotDetailDto>('/j8/calendar/getSlotDetail', { slotId: request.slotId }, requestConfig(context)))
  return toSlotRecord(data, '')
}

export const loadDepartmentOptions = async (keyword = '', context: RequestContext = {}): Promise<BookingOption[]> => {
  if (useMock) return mockLoadSlotDepartmentOptions(keyword, context)
  const result = await loadSlotInit(context)
  return result.departments.filter((item) => !keyword || item.label.toLowerCase().includes(keyword.toLowerCase()))
}

export const loadDoctorOptions = async (keyword = '', departmentId?: string | null, context: RequestContext = {}): Promise<BookingOption[]> => {
  if (useMock) return mockLoadSlotDoctorOptions(keyword, departmentId, context)
  const department = String(departmentId ?? '').trim()
  // 未选择科室时不查询全院医生，避免下拉列表出现无关数据。
  if (!department) return []
  const data = await unwrap<unknown>(calendarHttp.get(practitionerOptionsPath, {
    // 后端接口的 organizationId 在此处表示科室 ID。
    organizationId: department,
    ...(keyword.trim() ? { keyword: keyword.trim() } : {})
  }, requestConfig(context)))
  return toDoctorOptions(data, keyword)
}

export const batchBlockSlots = async (payload: BatchBlockSlotsPayload, context: RequestContext = {}) => {
  if (useMock) return mockBatchBlockSlots(payload)
  return unwrapVoid(calendarHttp.put('/j8/calendar/blockSlots', payload, requestConfig(context)))
}

export const batchUnblockSlots = async (payload: BatchUnblockSlotsPayload, context: RequestContext = {}) => {
  if (useMock) return mockBatchUnblockSlots(payload)
  return unwrapVoid(calendarHttp.put('/j8/calendar/unblockSlots', payload, requestConfig(context)))
}

export const markSlotOverbooked = async (payload: MarkSlotOverbookedPayload, context: RequestContext = {}) => {
  if (useMock) return mockMarkSlotOverbooked(payload)
  return unwrapVoid(calendarHttp.put('/j8/calendar/setOverbooked', payload, requestConfig(context)))
}

export type * from './type'
