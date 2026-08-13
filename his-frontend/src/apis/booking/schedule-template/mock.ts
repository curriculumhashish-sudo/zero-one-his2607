import type {
  BookingOption,
  DailyTemplate,
  DailyTemplateQuery,
  DailyTemplateSavePayload,
  HolidayException,
  HolidayExceptionSavePayload,
  HolidayExceptionQuery,
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

const departments: BookingOption[] = [
  { value: 'dept-internal', label: '心血管内科' },
  { value: 'dept-respiratory', label: '呼吸内科' }
]
const doctors: BookingOption[] = [
  { value: 'doctor-wang', label: '王建国' },
  { value: 'doctor-li', label: '李文静' },
  { value: 'doctor-zhao', label: '赵海峰' }
]
const rooms: BookingOption[] = [
  { value: 'room-101', label: '门诊 101 诊室' },
  { value: 'room-203', label: '门诊 203 诊室' },
  { value: 'room-305', label: '外科 305 诊室' }
]
const serviceTypes: BookingOption[] = [
  { value: '普通门诊', label: '普通门诊' },
  { value: '专家门诊', label: '专家门诊' },
  { value: '专科门诊', label: '专科门诊' }
]

let sequence = 10
let periods: TimePeriod[] = [
  {
    id: 'period-1',
    name: '上午门诊',
    segments: [{ id: 'segment-1', start: '08:00', end: '12:00', capacity: 40 }],
    status: 'enabled',
    usageCount: 2
  },
  {
    id: 'period-2',
    name: '下午门诊',
    segments: [{ id: 'segment-2', start: '14:00', end: '17:30', capacity: 30 }],
    status: 'enabled',
    usageCount: 1
  },
  {
    id: 'period-3',
    name: '晚间门诊',
    segments: [{ id: 'segment-3', start: '18:00', end: '20:00', capacity: 15 }],
    status: 'disabled',
    usageCount: 0
  }
]
let dailyTemplates: DailyTemplate[] = [
  {
    id: 'daily-1',
    name: '心内科工作日',
    scheduleMode: 'MODE-A',
    serviceType: '普通门诊',
    periodIds: ['period-1', 'period-2'],
    doctorId: 'doctor-wang',
    doctorName: '王建国',
    roomId: 'room-101',
    roomName: '门诊 101 诊室',
    releasePolicyId: 'policy-default',
    effectiveStart: '2026-07-01',
    effectiveEnd: '2026-12-31',
    priority: 10,
    defaultAppointment: true,
    status: 'enabled'
  },
  {
    id: 'daily-2',
    name: '呼吸专家日',
    scheduleMode: 'MODE-A',
    serviceType: '专家门诊',
    periodIds: ['period-1'],
    doctorId: 'doctor-li',
    doctorName: '李文静',
    roomId: 'room-203',
    roomName: '门诊 203 诊室',
    releasePolicyId: 'policy-default',
    effectiveStart: '2026-07-01',
    effectiveEnd: '2026-12-31',
    priority: 20,
    defaultAppointment: false,
    status: 'enabled'
  }
]
let weeklyTemplates: WeeklyTemplate[] = [
  {
    id: 'weekly-1',
    name: '内科默认周排班',
    isDefault: true,
    appointmentOpen: true,
    status: 'enabled',
    items: [
      {
        id: 'weekly-item-1',
        weekday: 1,
        weekdayLabel: '星期一',
        dailyTemplateId: 'daily-1',
        dailyTemplateName: '心内科工作日',
        enabled: true
      },
      {
        id: 'weekly-item-2',
        weekday: 3,
        weekdayLabel: '星期三',
        dailyTemplateId: 'daily-2',
        dailyTemplateName: '呼吸专家日',
        enabled: true
      }
    ]
  }
]
let policies: ReleasePolicy[] = [
  {
    id: 'policy-default',
    policyCode: 'DEFAULT',
    name: '门诊默认放号策略',
    enabled: true,
    openDays: 14,
    openTime: '08:00',
    closeTime: '20:00',
    maxAdvanceDays: 30,
    maxPerPatient: 3,
    slotMinutes: 30,
    capacityPerSlot: 10,
    channelQuotaJsonText: '{"WINDOW":50,"MINIAPP":50}'
  }
]
let holidays: HolidayException[] = [
  {
    id: 'holiday-1',
    date: '2026-10-01',
    rangeEnd: '2026-10-02',
    type: 'closed',
    reason: '国庆节停诊',
    autoApplyOnGenerate: true
  },
  {
    id: 'holiday-2',
    date: '2026-10-03',
    type: 'replace',
    replacementDailyTemplateId: 'daily-1',
    replacementDailyTemplateName: '心内科工作日',
    reason: '调休补班',
    autoApplyOnGenerate: true
  }
]

function clone<T>(value: T): T {
  return structuredClone(value)
}
function delay(context: RequestContext = {}, ms = 90) {
  return new Promise<void>((resolve, reject) => {
    if (context.signal?.aborted) {
      reject(new DOMException('请求已取消', 'AbortError'))
      return
    }
    const timer = setTimeout(resolve, ms)
    context.signal?.addEventListener(
      'abort',
      () => {
        clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      },
      { once: true }
    )
  })
}
function findOrThrow<T extends { id: string }>(rows: T[], id: string, message: string) {
  const row = rows.find((item) => item.id === id)
  if (!row) throw new Error(message)
  return row
}
function option(source: BookingOption[], value: string, label: string) {
  const item = source.find((entry) => entry.value === value)
  if (!item) throw new Error(`${label}不存在或已停用`)
  return item
}
function weekdayLabel(value: number) {
  return ['星期日', '星期一', '星期二', '星期三', '星期四', '星期五', '星期六'][value] ?? '未知'
}
function toPage<T>(rows: T[], page = 1, pageSize = 10): PageResult<T> {
  const start = (page - 1) * pageSize
  const pageRows = clone(rows.slice(start, start + pageSize))
  return {
    content: clone(pageRows),
    totalCount: rows.length,
    pageIndex: page,
    pageSize
  }
}

function syncPeriodFields(row: TimePeriod) {
  row.organizationId = departments[0]!.value
  row.templateCode ||= row.id
  row.templateName = row.name
  row.segments.forEach((segment) => {
    segment.startTime = segment.start
    segment.endTime = segment.end
  })
  row.slotTemplates = row.segments
  row.enabled = row.status === 'enabled'
  return row
}

function syncDailyFields(row: DailyTemplate) {
  row.organizationId = departments[0]!.value
  row.templateCode = row.id
  row.templateName = row.name
  row.healthcareServiceId = row.serviceType
  row.slotProvisionTemplateId = row.periodIds[0]
  row.practitionerId = row.doctorId
  row.locationId = row.roomId
  row.defaultReleasePolicyId = row.releasePolicyId
  row.enabled = row.status === 'enabled'
  return row
}

function syncWeeklyFields(row: WeeklyTemplate) {
  row.organizationId = departments[0]!.value
  row.templateCode ||= row.id
  row.templateName = row.name
  row.fallbackBookable = row.appointmentOpen
  row.enabled = row.status === 'enabled'
  row.items.forEach((item, index) => {
    item.dayScheduleTemplateId = item.dailyTemplateId
    item.sortNo = index + 1
  })
  return row
}

function syncPolicyFields(row: ReleasePolicy) {
  row.organizationId = departments[0]!.value
  row.policyName = row.policyName ?? row.name
  row.name = row.policyName
  row.policyCode = row.policyCode ?? row.id
  row.advanceDays = row.openDays
  if (row.channelQuotaJsonText) {
    row.channelQuotas = JSON.parse(row.channelQuotaJsonText) as Record<string, number>
  } else if (row.channelQuotas) {
    row.channelQuotaJsonText = JSON.stringify(row.channelQuotas)
  }
  return row
}

function parseChannelQuotas(value?: string) {
  if (!value?.trim()) return undefined
  let parsed: unknown
  try {
    parsed = JSON.parse(value)
  } catch {
    throw new Error('渠道配额必须是有效 JSON')
  }
  if (
    !parsed ||
    Array.isArray(parsed) ||
    typeof parsed !== 'object' ||
    Object.values(parsed).some((item) => typeof item !== 'number' || !Number.isFinite(item))
  ) {
    throw new Error('渠道配额必须是“渠道编码: 数字”的 JSON 对象')
  }
  return parsed as Record<string, number>
}

function syncHolidayFields(row: HolidayException) {
  row.organizationId = departments[0]!.value
  row.exceptionDate = row.date
  row.exceptionType = row.type
  row.overrideScheduleTemplateId = row.replacementDailyTemplateId
  return row
}

periods.forEach(syncPeriodFields)
dailyTemplates.forEach(syncDailyFields)
weeklyTemplates.forEach(syncWeeklyFields)
policies.forEach(syncPolicyFields)
holidays.forEach(syncHolidayFields)

export async function mockLoadScheduleTemplateInit(
  context: RequestContext = {}
): Promise<ScheduleTemplateInit> {
  await delay(context)
  return clone({
    department: departments[0]!,
    organizationId: departments[0]!.value,
    departments,
    doctors,
    rooms,
    serviceTypes,
    dailyTemplates: dailyTemplates.map((item) => ({ value: item.id, label: item.name }))
  })
}
export async function mockPageTimePeriods(query: TimePeriodQuery, context: RequestContext = {}) {
  await delay(context)
  const keyword = query.keyword?.trim().toLowerCase()
  return toPage(
    periods.filter(
      (item) =>
        (!keyword || item.name.toLowerCase().includes(keyword)) &&
        (!query.status || item.status === query.status)
    ),
    query.pageIndex,
    query.pageSize
  )
}
export async function mockSaveTimePeriod(
  payload: TimePeriodSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (!payload.templateCode.trim() || !payload.templateName.trim() || !payload.slotTemplates.length)
    throw new Error('模板编码、时段名称和时间段不能为空')
  if (payload.slotTemplates.some((item) => item.endTime <= item.startTime))
    throw new Error('结束时间必须晚于开始时间')
  const segments = payload.slotTemplates.map((item, index) => ({
    id: `${id ?? 'segment'}-${index + 1}`,
    start: item.startTime,
    end: item.endTime,
    capacity: Number(item.capacity ?? policies[0]!.capacityPerSlot)
  }))
  if (id) {
    const row = findOrThrow(periods, id, '放号时段不存在')
    Object.assign(row, {
      name: payload.templateName.trim(),
      templateCode: payload.templateCode ?? row.templateCode,
      organizationId: payload.organizationId ?? row.organizationId,
      segments,
      status: payload.enabled ? 'enabled' : 'disabled'
    })
    syncPeriodFields(row)
    return { id: row.id }
  }
  const row: TimePeriod = {
    id: `period-${sequence++}`,
    templateCode: payload.templateCode.trim(),
    name: payload.templateName.trim(),
    segments,
    status: payload.enabled ? 'enabled' : 'disabled',
    usageCount: 0
  }
  periods.push(row)
  syncPeriodFields(row)
  return { id: row.id }
}
export async function mockRemoveTimePeriod(id: string, context: RequestContext = {}) {
  await delay(context)
  const row = findOrThrow(periods, id, '放号时段不存在')
  if (row.usageCount > 0 || dailyTemplates.some((item) => item.periodIds.includes(id)))
    throw new Error('该时段已被日排班引用，不能直接删除')
  periods = periods.filter((item) => item.id !== id)
}
export async function mockPageDailyTemplates(
  query: DailyTemplateQuery,
  context: RequestContext = {}
) {
  await delay(context)
  const keyword = query.keyword?.trim().toLowerCase()
  return toPage(
    dailyTemplates.filter(
      (item) =>
        (!keyword ||
          `${item.name}${item.doctorName}${item.roomName}`.toLowerCase().includes(keyword)) &&
        (!query.status || item.status === query.status)
    ),
    query.pageIndex,
    query.pageSize
  )
}
export async function mockSaveDailyTemplate(
  payload: DailyTemplateSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (
    !payload.templateName.trim() ||
    !payload.slotProvisionTemplateId ||
    !payload.healthcareServiceId ||
    !payload.practitionerId
  )
    throw new Error('请完整填写日排班模板')
  if (
    payload.effectiveStart &&
    payload.effectiveEnd &&
    payload.effectiveEnd < payload.effectiveStart
  )
    throw new Error('生效结束日期不能早于开始日期')
  const periodIds = [payload.slotProvisionTemplateId]
  periodIds.forEach((periodId) =>
    option(
      periods.map((item) => ({ value: item.id, label: item.name })),
      periodId,
      '放号时段'
    )
  )
  const doctor = option(doctors, payload.practitionerId, '医生')
  const room = payload.locationId ? option(rooms, payload.locationId, '诊室') : undefined
  if (
    payload.defaultReleasePolicyId &&
    !policies.some((item) => item.id === payload.defaultReleasePolicyId)
  )
    throw new Error('放号策略不存在或已停用')
  if (id) {
    const row = findOrThrow(dailyTemplates, id, '日排班模板不存在')
    row.periodIds.forEach((periodId) => {
      const period = periods.find((item) => item.id === periodId)
      if (period) period.usageCount = Math.max(0, period.usageCount - 1)
    })
    Object.assign(row, {
      name: payload.templateName.trim(),
      templateCode: payload.templateCode ?? row.templateCode,
      organizationId: payload.organizationId,
      scheduleMode: payload.scheduleMode,
      serviceType: payload.healthcareServiceId,
      periodIds,
      doctorId: payload.practitionerId,
      roomId: payload.locationId ?? '',
      releasePolicyId: payload.defaultReleasePolicyId ?? '',
      effectiveStart: payload.effectiveStart ?? '',
      effectiveEnd: payload.effectiveEnd ?? '',
      priority: payload.priority ?? 0,
      status: payload.enabled ? 'enabled' : 'disabled',
      doctorName: doctor.label,
      roomName: room?.label ?? ''
    })
    periodIds.forEach((periodId) => {
      const period = periods.find((item) => item.id === periodId)
      if (period) period.usageCount += 1
    })
    syncDailyFields(row)
    return { id: row.id }
  }
  const row: DailyTemplate = {
    id: `daily-${sequence++}`,
    name: payload.templateName.trim(),
    scheduleMode: payload.scheduleMode,
    serviceType: payload.healthcareServiceId,
    periodIds,
    doctorId: payload.practitionerId,
    roomId: payload.locationId ?? '',
    releasePolicyId: payload.defaultReleasePolicyId ?? '',
    effectiveStart: payload.effectiveStart ?? '',
    effectiveEnd: payload.effectiveEnd ?? '',
    priority: payload.priority ?? 0,
    defaultAppointment: false,
    status: payload.enabled ? 'enabled' : 'disabled',
    doctorName: doctor.label,
    roomName: room?.label ?? ''
  }
  dailyTemplates.push(row)
  periodIds.forEach((periodId) => {
    const period = periods.find((item) => item.id === periodId)
    if (period) period.usageCount += 1
  })
  syncDailyFields(row)
  return { id: row.id }
}
export async function mockRemoveDailyTemplate(id: string, context: RequestContext = {}) {
  await delay(context)
  const row = findOrThrow(dailyTemplates, id, '日排班模板不存在')
  if (weeklyTemplates.some((item) => item.items.some((entry) => entry.dailyTemplateId === id)))
    throw new Error('该日排班已被周排班引用，不能直接删除')
  row.periodIds.forEach((periodId) => {
    const period = periods.find((item) => item.id === periodId)
    if (period) period.usageCount = Math.max(0, period.usageCount - 1)
  })
  dailyTemplates = dailyTemplates.filter((item) => item.id !== id)
}
export async function mockPageWeeklyTemplates(
  query: WeeklyTemplateQuery,
  context: RequestContext = {}
) {
  await delay(context)
  const keyword = query.keyword?.trim().toLowerCase()
  return toPage(
    weeklyTemplates.filter(
      (item) =>
        (!keyword || item.name.toLowerCase().includes(keyword)) &&
        (!query.status || item.status === query.status)
    ),
    query.pageIndex,
    query.pageSize
  )
}
export async function mockSaveWeeklyTemplate(
  payload: WeeklyTemplateSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  const payloadItems = payload.items ?? []
  if (!payload.templateCode.trim() || !payload.templateName.trim() || !payloadItems.length)
    throw new Error('周排班编码、名称和周明细不能为空')
  if (new Set(payloadItems.map((item) => item.weekday)).size !== payloadItems.length)
    throw new Error('同一周排班中不能重复配置星期')
  const items = payloadItems.map((item, index) => {
    const daily = option(
      dailyTemplates.map((entry) => ({ value: entry.id, label: entry.name })),
      item.dayScheduleTemplateId,
      '日排班模板'
    )
    return {
      id: `${id ?? `weekly-${sequence}`}-item-${index + 1}`,
      weekday: item.weekday,
      weekdayLabel: weekdayLabel(item.weekday),
      dailyTemplateId: item.dayScheduleTemplateId,
      enabled: item.enabled ?? true,
      dailyTemplateName: daily.label
    }
  })
  const value: Omit<WeeklyTemplate, 'id'> = {
    organizationId: payload.organizationId,
    templateCode: payload.templateCode.trim(),
    name: payload.templateName.trim(),
    isDefault: payload.isDefault ?? false,
    appointmentOpen: payload.fallbackBookable ?? false,
    status: payload.enabled ? 'enabled' : 'disabled',
    items
  }
  if (id) {
    const row = findOrThrow(weeklyTemplates, id, '周排班模板不存在')
    Object.assign(row, value)
    syncWeeklyFields(row)
    return { id: row.id }
  }
  const row: WeeklyTemplate = { ...value, id: `weekly-${sequence++}` }
  weeklyTemplates.push(row)
  syncWeeklyFields(row)
  return { id: row.id }
}
export async function mockRemoveWeeklyTemplate(id: string, context: RequestContext = {}) {
  await delay(context)
  findOrThrow(weeklyTemplates, id, '周排班模板不存在')
  weeklyTemplates = weeklyTemplates.filter((item) => item.id !== id)
}
export async function mockGetReleasePolicy(context: RequestContext = {}) {
  await delay(context)
  if (!policies[0]) throw new Error('尚未配置放号策略')
  return clone(policies[0])
}
export async function mockPageReleasePolicies(
  query: ReleasePolicyQuery,
  context: RequestContext = {}
) {
  await delay(context)
  const keyword = query.keyword?.trim().toLowerCase()
  return toPage(
    policies.filter(
      (item) =>
        !keyword ||
        `${item.policyCode ?? ''}${item.policyName ?? ''}`.toLowerCase().includes(keyword)
    ),
    query.pageIndex,
    query.pageSize
  )
}
export async function mockSaveReleasePolicy(
  payload: ReleasePolicySavePayload,
  context: RequestContext = {}
) {
  await delay(context)
  if (!payload.policyName.trim()) throw new Error('策略名称不能为空')
  if (payload.slotMinutes < 5 || payload.capacityPerSlot < 1)
    throw new Error('时段粒度和单段容量必须大于零')
  const channelQuotaJsonText = payload.channelQuotaJson?.trim() || undefined
  const channelQuotas = parseChannelQuotas(channelQuotaJsonText)
  if (payload.id) {
    const row = findOrThrow(policies, payload.id, '放号策略不存在')
    Object.assign(row, {
      policyCode: payload.policyCode?.trim() || row.policyCode,
      policyName: payload.policyName.trim(),
      name: payload.policyName.trim(),
      advanceDays: payload.advanceDays,
      openDays: payload.advanceDays,
      slotMinutes: payload.slotMinutes,
      capacityPerSlot: payload.capacityPerSlot,
      channelQuotaJsonText,
      channelQuotas
    })
    syncPolicyFields(row)
    return { id: row.id }
  }
  const row: ReleasePolicy = {
    id: `policy-${sequence++}`,
    policyCode: payload.policyCode?.trim() || `RP-${sequence}`,
    policyName: payload.policyName.trim(),
    name: payload.policyName.trim(),
    enabled: true,
    openDays: payload.advanceDays,
    advanceDays: payload.advanceDays,
    openTime: '00:00',
    closeTime: '23:59',
    maxAdvanceDays: payload.advanceDays,
    maxPerPatient: 1,
    slotMinutes: payload.slotMinutes,
    capacityPerSlot: payload.capacityPerSlot,
    channelQuotaJsonText,
    channelQuotas
  }
  policies.push(syncPolicyFields(row))
  return { id: row.id }
}
export async function mockRemoveReleasePolicy(id: string, context: RequestContext = {}) {
  await delay(context)
  findOrThrow(policies, id, '放号策略不存在')
  if (dailyTemplates.some((item) => item.releasePolicyId === id))
    throw new Error('该放号策略已被日排班引用，不能删除')
  policies = policies.filter((item) => item.id !== id)
}
export async function mockListHolidayExceptions(
  range: { start: string; end: string },
  context: RequestContext = {}
) {
  await delay(context)
  return clone(
    holidays.filter((item) => item.date <= range.end && (item.rangeEnd ?? item.date) >= range.start)
  )
}
export async function mockPageHolidayExceptions(
  query: HolidayExceptionQuery,
  context: RequestContext = {}
) {
  await delay(context)
  const typeMap = {
    CLOSED: 'closed',
    REDUCED: 'reduced',
    OVERRIDE: 'replace',
    EXTRA: 'extra'
  } as const
  const type = query.exceptionType ? typeMap[query.exceptionType] : undefined
  const keyword = query.keyword?.trim().toLowerCase()
  return toPage(
    holidays.filter(
      (item) =>
        (!query.rangeStart || (item.rangeEnd ?? item.date) >= query.rangeStart) &&
        (!query.rangeEnd || item.date <= query.rangeEnd) &&
        (!type || item.type === type) &&
        (!keyword ||
          `${item.reason}${item.replacementDailyTemplateName ?? ''}`
            .toLowerCase()
            .includes(keyword))
    ),
    query.pageIndex,
    query.pageSize
  )
}
export async function mockSaveHolidayException(
  payload: HolidayExceptionSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (!payload.exceptionDate || !payload.exceptionType) throw new Error('起始日和例外类型不能为空')
  if (payload.rangeEnd && payload.rangeEnd < payload.exceptionDate)
    throw new Error('结束日期不能早于起始日')
  let replacementDailyTemplateName: string | undefined
  const typeMap = {
    CLOSED: 'closed',
    REDUCED: 'reduced',
    OVERRIDE: 'replace',
    EXTRA: 'extra'
  } as const
  const type = typeMap[payload.exceptionType]
  if (type !== 'closed') {
    replacementDailyTemplateName = option(
      dailyTemplates.map((item) => ({ value: item.id, label: item.name })),
      payload.overrideScheduleTemplateId ?? '',
      '替代日排班'
    ).label
  }
  const value = {
    date: payload.exceptionDate,
    rangeEnd: payload.rangeEnd,
    type,
    replacementDailyTemplateId: payload.overrideScheduleTemplateId,
    reason: payload.reason?.trim() ?? '',
    autoApplyOnGenerate: payload.autoApplyOnGenerate ?? false,
    replacementDailyTemplateName
  }
  if (id) {
    const row = findOrThrow(holidays, id, '节假日例外不存在')
    Object.assign(row, value)
    syncHolidayFields(row)
    return { id: row.id }
  }
  const row: HolidayException = { ...value, id: `holiday-${sequence++}` }
  holidays.push(row)
  syncHolidayFields(row)
  return { id: row.id }
}
export async function mockRemoveHolidayException(id: string, context: RequestContext = {}) {
  await delay(context)
  findOrThrow(holidays, id, '节假日例外不存在')
  holidays = holidays.filter((item) => item.id !== id)
}

export function getScheduleTemplateSnapshot() {
  return clone({ periods, dailyTemplates, weeklyTemplates, policies, holidays })
}
