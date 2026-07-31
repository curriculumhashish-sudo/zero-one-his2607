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
  RequestContext,
  ScheduleInitResult,
  ScheduleImpactPreview,
  ScheduleRangeQuery,
  ScheduleRecord,
  UpdateScheduleDraftRequest,
  PublishScheduleRequest
} from './type'
import type {
  DailyTemplate,
  TimePeriod,
  WeeklyTemplate
} from '../schedule-template/type'

interface ScheduleGenerationTemplateData {
  dailyTemplates: DailyTemplate[]
  weeklyTemplates: WeeklyTemplate[]
  timePeriods: TimePeriod[]
}

interface InternalScheduleSavePayload {
  departmentId: string
  doctorId: string
  roomId: string
  date: string
  startTime: string
  endTime: string
  capacity: number
  publish: boolean
}

const departments: BookingOption[] = [
  { value: 'dept-internal', label: '心血管内科' },
  { value: 'dept-respiratory', label: '呼吸内科' },
  { value: 'dept-surgery', label: '普通外科' }
]

const doctors: BookingOption[] = [
  { value: 'doctor-wang', label: '王建国', departmentId: 'dept-internal', color: '#409eff' },
  { value: 'doctor-li', label: '李文静', departmentId: 'dept-respiratory', color: '#67c23a' },
  { value: 'doctor-zhao', label: '赵海峰', departmentId: 'dept-surgery', color: '#e6a23c' }
]

const rooms: BookingOption[] = [
  { value: 'room-101', label: '门诊 101 诊室' },
  { value: 'room-203', label: '门诊 203 诊室' },
  { value: 'room-305', label: '外科 305 诊室' }
]

const releasedSlots = new Map<string, ReleasedSlotRecord[]>()

function formatDate(date: Date) {
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')
  return `${year}-${month}-${day}`
}

function relativeDate(offset: number) {
  const date = new Date()
  date.setHours(12, 0, 0, 0)
  date.setDate(date.getDate() + offset)
  return formatDate(date)
}

function nowText() {
  return new Date().toISOString()
}

let sequence = 7
const schedules: ScheduleRecord[] = [
  {
    id: 'schedule-1',
    departmentId: 'dept-internal',
    departmentName: '心血管内科',
    doctorId: 'doctor-wang',
    doctorName: '王建国',
    roomId: 'room-101',
    roomName: '门诊 101 诊室',
    date: relativeDate(0),
    startTime: '08:00',
    endTime: '12:00',
    capacity: 24,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-2',
    departmentId: 'dept-respiratory',
    departmentName: '呼吸内科',
    doctorId: 'doctor-li',
    doctorName: '李文静',
    roomId: 'room-203',
    roomName: '门诊 203 诊室',
    date: relativeDate(0),
    startTime: '14:00',
    endTime: '17:00',
    capacity: 18,
    status: 'stopped',
    stopReason: '医生临时参加院内会诊',
    updatedAt: nowText()
  },
  {
    id: 'schedule-3',
    departmentId: 'dept-surgery',
    departmentName: '普通外科',
    doctorId: 'doctor-zhao',
    doctorName: '赵海峰',
    roomId: 'room-305',
    roomName: '外科 305 诊室',
    date: relativeDate(1),
    startTime: '08:30',
    endTime: '11:30',
    capacity: 16,
    status: 'draft',
    updatedAt: nowText()
  },
  {
    id: 'schedule-4',
    departmentId: 'dept-internal',
    departmentName: '心血管内科',
    doctorId: 'doctor-wang',
    doctorName: '王建国',
    roomId: 'room-101',
    roomName: '门诊 101 诊室',
    date: relativeDate(2),
    startTime: '14:00',
    endTime: '17:30',
    capacity: 20,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-5',
    departmentId: 'dept-respiratory',
    departmentName: '呼吸内科',
    doctorId: 'doctor-li',
    doctorName: '李文静',
    roomId: 'room-203',
    roomName: '门诊 203 诊室',
    date: relativeDate(4),
    startTime: '18:00',
    endTime: '20:00',
    capacity: 12,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-6',
    departmentId: 'dept-surgery',
    departmentName: '普通外科',
    doctorId: 'doctor-zhao',
    doctorName: '赵海峰',
    roomId: 'room-305',
    roomName: '外科 305 诊室',
    date: relativeDate(-1),
    startTime: '09:00',
    endTime: '11:00',
    capacity: 10,
    status: 'published',
    updatedAt: nowText()
  }
]

function syncPrototypeFields(record: ScheduleRecord): ScheduleRecord {
  record.scheduleId = record.id
  record.organizationId = 'org-main'
  record.name = `${record.doctorName} ${record.date} 门诊`
  record.practitionerId = record.doctorId
  record.locationId = record.roomId
  record.healthcareServiceId = record.departmentId
  record.healthcareServiceName = record.departmentName
  record.practitionerName = record.doctorName
  record.locationName = record.roomName
  record.planningHorizonStart = `${record.date}T${record.startTime}:00`
  record.planningHorizonEnd = `${record.date}T${record.endTime}:00`
  record.statusEnum = record.status
  record.bookable = record.status === 'published'
  record.actors = [
    { actorType: 10, actorId: record.doctorId, sortNo: 1 },
    { actorType: 20, actorId: record.roomId, sortNo: 2 },
    { actorType: 30, actorId: record.departmentId, sortNo: 3 }
  ]
  record.fromTemplate = record.fromTemplate ?? false
  return record
}

schedules.forEach(syncPrototypeFields)

function clone<T>(value: T): T {
  return structuredClone(value)
}

function delay(context: RequestContext = {}, milliseconds = 260) {
  return new Promise<void>((resolve, reject) => {
    if (context.signal?.aborted) {
      reject(new DOMException('请求已取消', 'AbortError'))
      return
    }
    const timer = setTimeout(resolve, milliseconds)
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

function findOption(source: BookingOption[], value: string, field: string) {
  const option = source.find((item) => item.value === value)
  if (!option) throw new Error(`${field}不存在或已停用`)
  return option
}

function toRecord(payload: InternalScheduleSavePayload, id: string): ScheduleRecord {
  if (payload.endTime <= payload.startTime) throw new Error('结束时间必须晚于开始时间')
  const department = findOption(departments, payload.departmentId, '科室')
  const doctor = findOption(doctors, payload.doctorId, '医生')
  const room = findOption(rooms, payload.roomId, '诊室')
  if (doctor.departmentId !== department.value) throw new Error('医生不属于所选科室')
  return syncPrototypeFields({
    id,
    departmentId: department.value,
    departmentName: department.label,
    doctorId: doctor.value,
    doctorName: doctor.label,
    roomId: room.value,
    roomName: room.label,
    date: payload.date,
    startTime: payload.startTime,
    endTime: payload.endTime,
    capacity: payload.capacity,
    status: payload.publish ? 'published' : 'draft',
    updatedAt: nowText()
  })
}

export async function mockLoadScheduleInit(
  context: RequestContext = {}
): Promise<ScheduleInitResult> {
  await delay(context)
  return clone({
    organizationId: 'org-main',
    departments,
    practitioners: doctors,
    doctors,
    clinicLocations: rooms,
    rooms,
    healthcareServices: departments
  })
}

export async function mockPageSchedulesByRange(
  query: ScheduleRangeQuery,
  context: RequestContext = {}
) {
  await delay(context, 360)
  const rangeStart = query.rangeStart
  const rangeEnd = query.rangeEnd
  const filtered = schedules.filter((item) => {
    const organizationId = query.organizationId
    const practitionerId = query.practitionerId
    const locationId = query.locationId
    if (item.date < rangeStart || item.date > rangeEnd) return false
    if (organizationId && item.organizationId !== organizationId) return false
    if (practitionerId && item.practitionerId !== practitionerId) return false
    if (locationId && item.locationId !== locationId) return false
    if (query.healthcareServiceId && item.healthcareServiceId !== query.healthcareServiceId)
      return false
    return true
  })
  const days = []
  const cursor = new Date(`${rangeStart}T00:00:00`)
  const end = new Date(`${rangeEnd}T00:00:00`)
  while (cursor <= end) {
    const date = formatDate(cursor)
    const rows = filtered.filter((item) => item.date === date)
    days.push({
      date,
      inMonth: true,
      activeCount: rows.filter((item) => item.status === 'published').length,
      draftCount: rows.filter((item) => item.status === 'draft').length,
      inactiveCount: rows.filter((item) => item.status === 'stopped').length,
      schedules: clone(rows)
    })
    cursor.setDate(cursor.getDate() + 1)
  }
  const result = {
    days,
    records: clone(filtered),
    yearMonth: rangeStart.slice(0, 7),
    monthStart: rangeStart,
    monthEnd: rangeEnd,
    gridStart: rangeStart,
    gridEnd: rangeEnd,
    summary: {
      publishedDayCount: days.filter((day) => day.activeCount > 0).length,
      draftDayCount: days.filter((day) => day.draftCount > 0).length,
      unscheduledDayCount: days.filter((day) => day.schedules.length === 0).length,
      inactiveDayCount: days.filter((day) => day.inactiveCount > 0).length,
      bookedAppointmentCount: 0,
      totalScheduleCount: filtered.length
    }
  }
  return clone(result)
}

export async function mockGetScheduleDetail(id: string, context: RequestContext = {}) {
  await delay(context, 180)
  const record = schedules.find((item) => item.id === id)
  if (!record) throw new Error('排班不存在或已被删除')
  return clone(record)
}

function actorId(actors: Array<{ actorType: number; actorId: string }>, actorType: number) {
  return actors.find((actor) => actor.actorType === actorType)?.actorId ?? ''
}

function internalPayload(
  request: CreateScheduleRequest | UpdateScheduleDraftRequest,
  existing?: ScheduleRecord
): InternalScheduleSavePayload {
  const actors = request.actors ?? existing?.actors ?? []
  const start = request.planningHorizonStart ?? existing?.planningHorizonStart ?? ''
  const end = request.planningHorizonEnd ?? existing?.planningHorizonEnd ?? ''
  return {
    departmentId: actorId(actors, 30) || existing?.departmentId || '',
    doctorId: actorId(actors, 10) || existing?.doctorId || '',
    roomId: actorId(actors, 20) || existing?.roomId || '',
    date: start.slice(0, 10),
    startTime: start.slice(11, 16),
    endTime: end.slice(11, 16),
    capacity: existing?.capacity ?? 20,
    publish: existing?.status === 'published'
  }
}

export async function mockCreateSchedule(payload: CreateScheduleRequest) {
  await delay()
  const record = toRecord(internalPayload(payload), `schedule-${sequence++}`)
  record.name = payload.name
  record.slotProvisionTemplateId = payload.slotProvisionTemplateId
  schedules.push(record)
  return { scheduleId: record.id }
}

export async function mockUpdateSchedule(payload: UpdateScheduleDraftRequest) {
  await delay()
  const index = schedules.findIndex((item) => item.id === payload.scheduleId)
  if (index < 0) throw new Error('排班不存在或已被删除')
  const previous = schedules[index]!
  if (previous.status !== 'draft' || previous.voidedFlag) throw new Error('只有草稿排班可以编辑')
  const record = toRecord(internalPayload(payload, previous), payload.scheduleId)
  record.status = previous.status
  record.slotProvisionTemplateId = payload.slotProvisionTemplateId ?? previous.slotProvisionTemplateId
  syncPrototypeFields(record)
  record.name = payload.name ?? previous.name
  schedules[index] = record
}

export async function mockPublishSchedule(payload: PublishScheduleRequest) {
  await delay()
  const record = schedules.find((item) => item.id === payload.scheduleId)
  if (!record) throw new Error('排班不存在或已被删除')
  if (record.status !== 'draft' || record.voidedFlag) throw new Error('只有草稿排班可以发布')
  record.planningHorizonStart = payload.planningHorizonStart
  record.planningHorizonEnd = payload.planningHorizonEnd
  record.status = 'published'
  syncPrototypeFields(record)
  record.updatedAt = nowText()
}

export async function mockStopSchedule(payload: DeactivateScheduleRequest) {
  await delay()
  if (!payload.reason?.trim()) throw new Error('请输入停诊原因')
  const record = schedules.find((item) => item.id === payload.scheduleId)
  if (!record) throw new Error('排班不存在或已被删除')
  if (record.status !== 'published' || record.voidedFlag) throw new Error('只有已发布排班可以停诊')
  record.status = 'stopped'
  record.stopReason = payload.reason.trim()
  syncPrototypeFields(record)
  record.updatedAt = nowText()
}

function minutes(value: string) {
  const [hour = '0', minute = '0'] = value.split(':')
  return Number(hour) * 60 + Number(minute)
}

function timeText(value: number) {
  return `${String(Math.floor(value / 60)).padStart(2, '0')}:${String(value % 60).padStart(2, '0')}`
}

function defaultSlots(record: ScheduleRecord): ReleasedSlotRecord[] {
  const start = minutes(record.startTime)
  const end = minutes(record.endTime)
  const count = Math.max(1, Math.ceil((end - start) / 30))
  const capacity = Math.max(1, Math.ceil(record.capacity / count))
  return Array.from({ length: count }, (_, index) => {
    const slotStart = start + index * 30
    const slotEnd = Math.min(end, slotStart + 30)
    return {
      slotId: `${record.id}-released-${index + 1}`,
      scheduleId: record.id,
      organizationId: record.organizationId ?? 'org-main',
      practitionerId: record.practitionerId ?? record.doctorId,
      healthcareServiceId: record.healthcareServiceId ?? record.departmentId,
      practitionerName: record.doctorName,
      healthcareServiceName: record.healthcareServiceName ?? record.departmentName,
      organizationName: record.departmentName,
      startTime: `${record.date} ${timeText(slotStart)}:00`,
      endTime: `${record.date} ${timeText(slotEnd)}:00`,
      statusCode: record.partialStoppedSlotIds?.includes(`${record.id}-released-${index + 1}`)
        ? 'BLOCKED'
        : 'FREE',
      capacity,
      bookedCount: 0,
      remainingCount: capacity,
      displayStatus: record.partialStoppedSlotIds?.includes(`${record.id}-released-${index + 1}`)
        ? '已封锁'
        : '可约',
      overbookedFlag: false
    }
  })
}

function slotsOf(record: ScheduleRecord) {
  const rows = releasedSlots.get(record.id) ?? defaultSlots(record)
  releasedSlots.set(record.id, rows)
  return rows
}

export async function mockGenerateFormalSchedules(
  payload: GenerateFormalSchedulesPayload,
  templates: ScheduleGenerationTemplateData,
  context: RequestContext = {}
): Promise<GenerateFormalSchedulesResult> {
  await delay(context)
  if (!payload.scheduleTemplateId && !payload.weekScheduleTemplateId)
    throw new Error('请选择排班模板')
  if (payload.autoProvision && !payload.autoPublish)
    throw new Error('自动放号前必须启用自动发布')
  const selectedDaily = payload.scheduleTemplateId
    ? templates.dailyTemplates.find((item) => item.id === payload.scheduleTemplateId)
    : undefined
  const selectedWeekly = payload.weekScheduleTemplateId
    ? templates.weeklyTemplates.find((item) => item.id === payload.weekScheduleTemplateId)
    : undefined
  if (payload.templateKind === 'DAY' && !selectedDaily) throw new Error('所选日排班不存在或已停用')
  if (payload.templateKind === 'WEEK' && !selectedWeekly)
    throw new Error('所选周排班不存在或已停用')
  const scheduleIds: string[] = []
  let skippedCount = 0
  let provisionedSlotCount = 0
  const cursor = new Date(`${payload.rangeStart}T00:00:00`)
  const end = new Date(`${payload.rangeEnd}T00:00:00`)
  while (cursor <= end) {
    const date = formatDate(cursor)
    const weekday = cursor.getDay() || 7
    const weeklyItem = selectedWeekly?.items.find(
      (item) => item.weekday === weekday && item.enabled
    )
    const dailyTemplate =
      payload.templateKind === 'DAY'
        ? selectedDaily
        : templates.dailyTemplates.find(
            (item) => item.id === (weeklyItem?.dayScheduleTemplateId ?? weeklyItem?.dailyTemplateId)
          )
    if (
      !dailyTemplate ||
      dailyTemplate.status !== 'enabled' ||
      (dailyTemplate.effectiveStart && date < dailyTemplate.effectiveStart) ||
      (dailyTemplate.effectiveEnd && date > dailyTemplate.effectiveEnd)
    ) {
      skippedCount += 1
      cursor.setDate(cursor.getDate() + 1)
      continue
    }
    const doctor = findOption(doctors, dailyTemplate.practitionerId ?? dailyTemplate.doctorId, '医生')
    const department = findOption(departments, doctor.departmentId ?? '', '科室')
    const room = findOption(rooms, dailyTemplate.locationId ?? dailyTemplate.roomId, '诊室')
    const segments = dailyTemplate.periodIds
      .flatMap(
        (periodId) =>
          templates.timePeriods.find((period) => period.id === periodId)?.segments ?? []
      )
      .sort((left, right) => left.start.localeCompare(right.start))
    const startTime = segments[0]?.start ?? '08:00'
    const endTime = segments[segments.length - 1]?.end ?? '12:00'
    const capacity = segments.reduce((sum, segment) => sum + segment.capacity, 0) || 20
    if (
      schedules.some(
        (item) =>
          item.date === date &&
          item.departmentId === department.value &&
          item.doctorId === doctor.value
      )
    ) {
      skippedCount += 1
    } else {
      const record = toRecord(
        {
          departmentId: department.value,
          doctorId: doctor.value,
          roomId: room.value,
          date,
          startTime,
          endTime,
          capacity,
          publish: payload.autoPublish ?? false
        },
        `schedule-${sequence++}`
      )
      record.fromTemplate = true
      record.sourceScheduleTemplateId = payload.scheduleTemplateId ?? payload.weekScheduleTemplateId
      record.dayScheduleTemplateId = dailyTemplate.id
      record.weekScheduleTemplateId =
        payload.templateKind === 'WEEK' ? payload.weekScheduleTemplateId : undefined
      record.slotProvisionTemplateId = dailyTemplate.slotProvisionTemplateId
      schedules.push(record)
      scheduleIds.push(record.id)
      if (payload.autoProvision) provisionedSlotCount += slotsOf(record).length
    }
    cursor.setDate(cursor.getDate() + 1)
  }
  return { scheduleIds, generatedCount: scheduleIds.length, skippedCount, provisionedSlotCount }
}

export async function mockSetScheduleSubstitution(
  scheduleId: string,
  substitutePractitionerId: string,
  reason = ''
) {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'published' || record.voidedFlag) throw new Error('只有已发布排班可以替诊')
  const substitute = findOption(doctors, substitutePractitionerId, '替诊医生')
  record.originalPractitionerId = record.originalPractitionerId ?? record.doctorId
  record.substitutePractitionerId = substitute.value
  record.substituteReason = reason.trim()
  record.substituteDisplay = `${record.doctorName} → ${substitute.label}`
  return {
    scheduleId,
    originalPractitionerId: record.originalPractitionerId,
    substitutePractitionerId: substitute.value,
    substituteReason: record.substituteReason,
    cleared: false
  }
}

export async function mockClearScheduleSubstitution(scheduleId: string) {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'published' || record.voidedFlag)
    throw new Error('只有已发布排班可以撤销替诊')
  if (!record.substitutePractitionerId) throw new Error('当前排班没有替诊信息')
  const original = record.originalPractitionerId
  record.substitutePractitionerId = undefined
  record.substituteDisplay = undefined
  record.substituteReason = undefined
  return {
    scheduleId,
    originalPractitionerId: original,
    substitutePractitionerId: null,
    substituteReason: '',
    cleared: true
  }
}

export async function mockPreviewScheduleImpact(
  scheduleId: string
): Promise<ScheduleImpactPreview> {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  const slotCount = slotsOf(record).length
  return {
    scheduleId,
    scheduleCount: 1,
    slotCount,
    appointmentCount: 0,
    registeredCount: 0,
    checkedInCount: 0,
    channelCount: 0,
    blockDeactivateIfUnhandled: true,
    defaultDispositionMode: 'TRANSFER',
    transferMatchRule: 'SAME_SERVICE',
    sourceSlotsFrozen: true,
    frozenSlotCount: slotCount,
    freezeExpiresAt: new Date(Date.now() + 30 * 60 * 1000).toISOString(),
    items: []
  }
}

export async function mockPartialStopSchedule(
  payload: PartialStopSchedulePayload
): Promise<PartialStopScheduleResult> {
  await delay()
  if (payload.dispositionMode === 'TRANSFER' && !payload.transferItems?.length)
    throw new Error('转移预约时必须提交逐条预约与目标号源')
  const record = schedules.find((item) => item.id === payload.scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'published' || record.voidedFlag)
    throw new Error('只有已发布排班可以部分停诊')
  const currentSlots = slotsOf(record)
  const valid = new Set(currentSlots.map((slot) => slot.slotId))
  if (payload.slotIds.some((id) => !valid.has(id))) throw new Error('部分号源已变化，请刷新后重试')
  const fullStop = payload.slotIds.length === 0
  const stoppedSlotIds = fullStop ? currentSlots.map((slot) => slot.slotId) : payload.slotIds
  record.partialStoppedSlotIds = [
    ...new Set([...(record.partialStoppedSlotIds ?? []), ...stoppedSlotIds])
  ]
  if (fullStop) {
    if (!payload.reason?.trim()) throw new Error('请输入停诊原因')
    record.status = 'stopped'
    record.stopReason = payload.reason.trim()
    record.updatedAt = nowText()
    syncPrototypeFields(record)
  }
  releasedSlots.set(
    record.id,
    slotsOf(record).map((slot) =>
      record.partialStoppedSlotIds?.includes(slot.slotId)
        ? { ...slot, statusCode: 'BLOCKED', displayStatus: '已封锁', remainingCount: 0 }
        : slot
    )
  )
  return {
    successCount: payload.transferItems?.length ?? 0,
    failedCount: 0,
    scheduleDeactivated: fullStop,
    blockedSlotCount: stoppedSlotIds.length,
    successItems: [],
    failedItems: []
  }
}

export async function mockUnlockPartialStop(
  _organizationId: string,
  scheduleId: string,
  slotIds: string[] = []
) {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'published' || record.voidedFlag)
    throw new Error('只有已发布排班可以解锁停诊号源')
  if (!record.partialStoppedSlotIds?.length) throw new Error('当前排班没有可解锁的停诊号源')
  const targets = new Set(slotIds.length ? slotIds : (record.partialStoppedSlotIds ?? []))
  record.partialStoppedSlotIds = (record.partialStoppedSlotIds ?? []).filter(
    (id) => !targets.has(id)
  )
  releasedSlots.delete(record.id)
  return null
}

export async function mockResumeSchedule(scheduleId: string) {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'stopped' || record.voidedFlag) throw new Error('只有停诊排班可以恢复开诊')
  record.status = 'published'
  record.stopReason = undefined
  record.voidedFlag = false
  syncPrototypeFields(record)
  return { scheduleId, needReprovision: !releasedSlots.has(scheduleId) }
}

export async function mockVoidSchedule(scheduleId: string, reason = '') {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.voidedFlag) throw new Error('排班已经作废')
  if (record.status !== 'draft' && record.status !== 'stopped')
    throw new Error('只有草稿或停诊排班可以作废')
  record.status = 'stopped'
  record.stopReason = reason.trim() || '日程已作废'
  record.voidedFlag = true
  syncPrototypeFields(record)
  return null
}

export async function mockListReleasedSlots(
  scheduleId: string,
  pageNo = 1,
  pageSize = 100
): Promise<ReleasedSlotPage> {
  await delay()
  const record = schedules.find((item) => item.id === scheduleId)
  if (!record) throw new Error('排班不存在')
  const rows = slotsOf(record)
  const start = (pageNo - 1) * pageSize
  return {
    content: clone(rows.slice(start, start + pageSize)),
    totalCount: rows.length,
    pageNo,
    pageSize
  }
}

export async function mockProvisionScheduleSlots(
  payload: ProvisionSlotsPayload
): Promise<ProvisionSlotsResult> {
  await delay()
  const record = schedules.find((item) => item.id === payload.scheduleId)
  if (!record) throw new Error('排班不存在')
  if (record.status !== 'published' || record.voidedFlag) throw new Error('只有已发布排班可以放号')
  if (!payload.slotTemplates.length) throw new Error('请配置放号时段')
  record.startTime = payload.slotTemplates[0]!.startTime.slice(0, 5)
  record.endTime = payload.slotTemplates[payload.slotTemplates.length - 1]!.endTime.slice(0, 5)
  record.capacity = payload.slotTemplates.reduce((sum, slot) => sum + Number(slot.capacity ?? 0), 0)
  syncPrototypeFields(record)
  const rows = payload.slotTemplates.map((slot, index) => ({
    slotId: `${record.id}-custom-${Date.now()}-${index + 1}`,
    scheduleId: record.id,
    organizationId: payload.organizationId,
    practitionerId: record.practitionerId ?? record.doctorId,
    healthcareServiceId: record.healthcareServiceId ?? record.departmentId,
    practitionerName: record.doctorName,
    healthcareServiceName: record.healthcareServiceName ?? record.departmentName,
    organizationName: record.departmentName,
    startTime: `${record.date} ${slot.startTime}:00`,
    endTime: `${record.date} ${slot.endTime}:00`,
    statusCode: 'FREE',
    capacity: Number(slot.capacity ?? 0),
    bookedCount: 0,
    remainingCount: Number(slot.capacity ?? 0),
    displayStatus: '可约',
    overbookedFlag: false
  }))
  releasedSlots.set(record.id, rows)
  return { slotIds: rows.map((slot) => slot.slotId), generatedCount: rows.length }
}

export async function mockDeleteReleasedSlots(slotIds: string[]) {
  await delay()
  if (!slotIds.length) throw new Error('请选择需要删除的号源')
  for (const [scheduleId, rows] of releasedSlots) {
    releasedSlots.set(
      scheduleId,
      rows.filter((slot) => !slotIds.includes(slot.slotId))
    )
  }
  return null
}

export async function mockLoadDepartmentOptions(keyword: string, context: RequestContext = {}) {
  await delay(context, 180)
  return clone(departments.filter((item) => item.label.includes(keyword.trim())))
}

export async function mockLoadDoctorOptions(
  keyword: string,
  departmentId?: string | null,
  context: RequestContext = {}
) {
  await delay(context, 180)
  return clone(
    doctors.filter(
      (item) =>
        (!departmentId || item.departmentId === departmentId) && item.label.includes(keyword.trim())
    )
  )
}

export function getMockScheduleSnapshot() {
  return clone(schedules)
}
