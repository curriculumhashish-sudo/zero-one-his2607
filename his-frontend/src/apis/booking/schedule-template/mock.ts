import type {
  BookingOption,
  DailyTemplate,
  DailyTemplateQuery,
  DailyTemplateSavePayload,
  HolidayException,
  HolidayExceptionSavePayload,
  PageResult,
  ReleasePolicy,
  RequestContext,
  ScheduleTemplateInit,
  TemplateStatus,
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
let policy: ReleasePolicy = {
  id: 'policy-default',
  name: '门诊默认放号策略',
  enabled: true,
  openDays: 14,
  openTime: '08:00',
  closeTime: '20:00',
  maxAdvanceDays: 30,
  maxPerPatient: 3,
  slotMinutes: 30,
  capacityPerSlot: 10
}
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
  return { rows: clone(rows.slice(start, start + pageSize)), total: rows.length }
}

export async function mockLoadScheduleTemplateInit(
  context: RequestContext = {}
): Promise<ScheduleTemplateInit> {
  await delay(context)
  return clone({
    department: departments[0]!,
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
    query.page,
    query.pageSize
  )
}
export async function mockSaveTimePeriod(
  payload: TimePeriodSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (!payload.name.trim() || !payload.segments.length) throw new Error('时段名称和时间段不能为空')
  if (payload.segments.some((item) => item.end <= item.start))
    throw new Error('结束时间必须晚于开始时间')
  const segments = payload.segments.map((item, index) => ({
    id: `${id ?? 'segment'}-${index + 1}`,
    ...item,
    capacity: Number(item.capacity ?? policy.capacityPerSlot)
  }))
  if (id) {
    const row = findOrThrow(periods, id, '放号时段不存在')
    Object.assign(row, { name: payload.name.trim(), segments, status: payload.status })
    return clone(row)
  }
  const row: TimePeriod = {
    id: `period-${sequence++}`,
    name: payload.name.trim(),
    segments,
    status: payload.status,
    usageCount: 0
  }
  periods.push(row)
  return clone(row)
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
    query.page,
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
    !payload.name.trim() ||
    !payload.periodIds.length ||
    !payload.serviceType ||
    !payload.doctorId ||
    !payload.roomId
  )
    throw new Error('请完整填写日排班模板')
  if (payload.effectiveEnd < payload.effectiveStart) throw new Error('生效结束日期不能早于开始日期')
  payload.periodIds.forEach((periodId) =>
    option(
      periods.map((item) => ({ value: item.id, label: item.name })),
      periodId,
      '放号时段'
    )
  )
  const doctor = option(doctors, payload.doctorId, '医生')
  const room = option(rooms, payload.roomId, '诊室')
  if (payload.releasePolicyId !== policy.id) throw new Error('放号策略不存在或已停用')
  if (id) {
    const row = findOrThrow(dailyTemplates, id, '日排班模板不存在')
    row.periodIds.forEach((periodId) => {
      const period = periods.find((item) => item.id === periodId)
      if (period) period.usageCount = Math.max(0, period.usageCount - 1)
    })
    Object.assign(row, {
      ...payload,
      name: payload.name.trim(),
      doctorName: doctor.label,
      roomName: room.label
    })
    payload.periodIds.forEach((periodId) => {
      const period = periods.find((item) => item.id === periodId)
      if (period) period.usageCount += 1
    })
    return clone(row)
  }
  const row: DailyTemplate = {
    ...payload,
    id: `daily-${sequence++}`,
    name: payload.name.trim(),
    doctorName: doctor.label,
    roomName: room.label
  }
  dailyTemplates.push(row)
  payload.periodIds.forEach((periodId) => {
    const period = periods.find((item) => item.id === periodId)
    if (period) period.usageCount += 1
  })
  return clone(row)
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
    query.page,
    query.pageSize
  )
}
export async function mockSaveWeeklyTemplate(
  payload: WeeklyTemplateSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (!payload.name.trim() || !payload.items.length) throw new Error('周排班名称和周明细不能为空')
  if (new Set(payload.items.map((item) => item.weekday)).size !== payload.items.length)
    throw new Error('同一周排班中不能重复配置星期')
  const items = payload.items.map((item, index) => {
    const daily = option(
      dailyTemplates.map((entry) => ({ value: entry.id, label: entry.name })),
      item.dailyTemplateId,
      '日排班模板'
    )
    return {
      ...item,
      id: `${id ?? `weekly-${sequence}`}-item-${index + 1}`,
      weekdayLabel: weekdayLabel(item.weekday),
      dailyTemplateName: daily.label
    }
  })
  const value = {
    ...payload,
    name: payload.name.trim(),
    items
  }
  if (id) {
    const row = findOrThrow(weeklyTemplates, id, '周排班模板不存在')
    Object.assign(row, value)
    return clone(row)
  }
  const row: WeeklyTemplate = { ...value, id: `weekly-${sequence++}` }
  weeklyTemplates.push(row)
  return clone(row)
}
export async function mockRemoveWeeklyTemplate(id: string, context: RequestContext = {}) {
  await delay(context)
  findOrThrow(weeklyTemplates, id, '周排班模板不存在')
  weeklyTemplates = weeklyTemplates.filter((item) => item.id !== id)
}
export async function mockGetReleasePolicy(context: RequestContext = {}) {
  await delay(context)
  return clone(policy)
}
export async function mockSaveReleasePolicy(payload: ReleasePolicy, context: RequestContext = {}) {
  await delay(context)
  if (!payload.name.trim()) throw new Error('策略名称不能为空')
  if (payload.openTime >= payload.closeTime) throw new Error('放号开始时间必须早于结束时间')
  if (payload.slotMinutes < 5 || payload.capacityPerSlot < 1)
    throw new Error('时段粒度和单段容量必须大于零')
  policy = clone(payload)
  return clone(policy)
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
export async function mockSaveHolidayException(
  payload: HolidayExceptionSavePayload,
  context: RequestContext = {}
) {
  const id = payload.id
  await delay(context)
  if (!payload.date || !payload.reason.trim()) throw new Error('日期和例外原因不能为空')
  if (payload.rangeEnd && payload.rangeEnd < payload.date)
    throw new Error('结束日期不能早于开始日期')
  let replacementDailyTemplateName: string | undefined
  if (payload.type !== 'closed') {
    replacementDailyTemplateName = option(
      dailyTemplates.map((item) => ({ value: item.id, label: item.name })),
      payload.replacementDailyTemplateId ?? '',
      '替代日排班'
    ).label
  }
  const value = { ...payload, reason: payload.reason.trim(), replacementDailyTemplateName }
  if (id) {
    const row = findOrThrow(holidays, id, '节假日例外不存在')
    Object.assign(row, value)
    return clone(row)
  }
  const row: HolidayException = { ...value, id: `holiday-${sequence++}` }
  holidays.push(row)
  return clone(row)
}
export async function mockRemoveHolidayException(id: string, context: RequestContext = {}) {
  await delay(context)
  findOrThrow(holidays, id, '节假日例外不存在')
  holidays = holidays.filter((item) => item.id !== id)
}

export function getScheduleTemplateSnapshot() {
  return clone({ periods, dailyTemplates, weeklyTemplates, policy, holidays })
}
