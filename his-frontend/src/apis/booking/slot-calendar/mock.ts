import {
  getMockScheduleSnapshot,
  mockLoadDepartmentOptions,
  mockLoadDoctorOptions
} from '../outpatient-schedule/mock'
import type {
  BatchBlockSlotsPayload,
  BatchUnblockSlotsPayload,
  MarkSlotOverbookedPayload,
  RequestContext,
  SlotPageResult,
  SlotQuery,
  SlotRecord,
  SlotStatus,
  SlotStatusCode
} from './type'

const blockedSlots = new Map<string, string>()
const overbookedSlots = new Set<string>()
const serviceTypeByDoctorId: Record<string, string> = {
  'doctor-wang': '普通号',
  'doctor-li': '专家号',
  'doctor-zhao': '专科号'
}

function delay(context: RequestContext = {}, milliseconds = 280) {
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

function timeToMinutes(value: string) {
  const [hour = '0', minute = '0'] = value.split(':')
  return Number(hour) * 60 + Number(minute)
}

function minutesToTime(value: number) {
  return `${String(Math.floor(value / 60)).padStart(2, '0')}:${String(value % 60).padStart(2, '0')}`
}

function groupOf(startTime: string): SlotRecord['group'] {
  const hour = Number(startTime.slice(0, 2))
  if (hour < 12) return '上午'
  if (hour < 18) return '下午'
  return '晚间'
}

function statusOf(
  scheduleStatus: string,
  date: string,
  index: number,
  remaining: number
): SlotStatus {
  if (scheduleStatus === 'stopped') return 'stopped'
  const today = new Date()
  const localToday = `${today.getFullYear()}-${String(today.getMonth() + 1).padStart(2, '0')}-${String(today.getDate()).padStart(2, '0')}`
  if (date < localToday) return 'expired'
  if (remaining <= 0 || index % 4 === 1) return 'full'
  return 'available'
}

function statusCodeOf(status: SlotStatus): SlotStatusCode {
  if (status === 'available') return 'FREE'
  if (status === 'full') return 'FULL'
  if (status === 'expired') return 'EXPIRED'
  return 'BLOCKED'
}

function buildSlots(): SlotRecord[] {
  return getMockScheduleSnapshot()
    .filter((schedule) => schedule.status !== 'draft')
    .flatMap((schedule) => {
      const start = timeToMinutes(schedule.startTime)
      const end = timeToMinutes(schedule.endTime)
      const count = Math.max(1, Math.min(6, Math.ceil((end - start) / 30)))
      const capacity = Math.max(1, Math.ceil(schedule.capacity / count))
      return Array.from({ length: count }, (_, index) => {
        const slotStart = start + index * 30
        const slotEnd = Math.min(slotStart + 30, end)
        const id = `${schedule.id}-slot-${index + 1}`
        const provisionalRemaining = index % 4 === 1 ? 0 : Math.max(1, capacity - (index % 3))
        const blockedReason = blockedSlots.get(id)
        const status = blockedReason
          ? 'stopped'
          : statusOf(schedule.status, schedule.date, index, provisionalRemaining)
        const remaining = status === 'full' ? 0 : provisionalRemaining
        const used = capacity - remaining
        const statusText =
          status === 'available'
            ? '可约'
            : status === 'full'
              ? '已满'
              : status === 'stopped'
                ? '停诊'
                : '过期'
        return {
          slotId: id,
          id,
          scheduleId: schedule.id,
          organizationId: schedule.departmentId,
          departmentId: schedule.departmentId,
          departmentName: schedule.departmentName,
          practitionerId: schedule.practitionerId ?? schedule.doctorId,
          practitionerName: schedule.practitionerName ?? schedule.doctorName,
          doctorId: schedule.doctorId,
          doctorName: schedule.doctorName,
          healthcareServiceId: schedule.healthcareServiceId ?? schedule.departmentId,
          healthcareServiceName: serviceTypeByDoctorId[schedule.doctorId] ?? '普通号',
          organizationName: schedule.departmentName,
          roomName: schedule.roomName,
          date: schedule.date,
          startTime: `${schedule.date} ${minutesToTime(slotStart)}:00`,
          endTime: `${schedule.date} ${minutesToTime(slotEnd)}:00`,
          group: groupOf(minutesToTime(slotStart)),
          capacity,
          bookedCount: used,
          used,
          remainingCount: remaining,
          remaining,
          statusCode: statusCodeOf(status),
          status,
          displayStatus: statusText,
          overbookedFlag: overbookedSlots.has(id),
          blockedFlag: Boolean(blockedReason),
          commentText: blockedReason ?? schedule.stopReason,
          unavailableReason:
            status === 'stopped'
              ? blockedReason || schedule.stopReason || '该排班已停诊'
              : status === 'full'
                ? '该时段号源已满'
                : status === 'expired'
                  ? '该时段已经过期'
                  : undefined
        } satisfies SlotRecord
      })
    })
}

export async function mockPageSlotsByDate(query: SlotQuery, context: RequestContext = {}) {
  await delay(context, 360)
  const filtered = buildSlots().filter((item) => {
    if (item.date !== query.date) return false
    const organizationId = query.organizationId
    const practitionerId = query.practitionerId
    if (organizationId && item.organizationId !== organizationId) return false
    if (practitionerId && item.practitionerId !== practitionerId) return false
    if (query.scheduleId && item.scheduleId !== query.scheduleId) return false
    if (
      query.statusCode &&
      item.status !== query.statusCode &&
      item.statusCode !== query.statusCode
    )
      return false
    return true
  })
  const pageNo = query.pageNo
  const pageSize = query.pageSize
  const start = (pageNo - 1) * pageSize
  const result: SlotPageResult = {
    content: structuredClone(filtered.slice(start, start + pageSize)),
    pageNo,
    pageSize,
    totalCount: filtered.length
  }
  return result
}

export async function mockGetSlotDetail(
  request: { slotId: string },
  context: RequestContext = {}
) {
  await delay(context, 180)
  const slot = buildSlots().find((item) => item.id === request.slotId)
  if (!slot) throw new Error('号源不存在或已发生变化')
  return structuredClone(slot)
}

export async function mockLoadSlotInit(context: RequestContext = {}) {
  const [departments, doctors] = await Promise.all([
    mockLoadDepartmentOptions('', context),
    mockLoadDoctorOptions('', null, context)
  ])
  return { organizationId: departments[0]?.value ?? '', departments, doctors }
}

export const mockLoadSlotDepartmentOptions = mockLoadDepartmentOptions
export const mockLoadSlotDoctorOptions = mockLoadDoctorOptions

export async function mockBatchBlockSlots(payload: BatchBlockSlotsPayload) {
  await delay()
  if (!payload.slotIds.length) throw new Error('请至少选择一个号源')
  const reason = payload.reason?.trim() || '人工封锁'
  const existing = new Set(buildSlots().map((slot) => slot.slotId))
  if (payload.slotIds.some((id) => !existing.has(id)))
    throw new Error('部分号源已发生变化，请刷新后重试')
  payload.slotIds.forEach((id) => blockedSlots.set(id, reason))
  return undefined
}

export async function mockBatchUnblockSlots(payload: BatchUnblockSlotsPayload) {
  await delay()
  if (!payload.slotIds.length) throw new Error('请至少选择一个号源')
  payload.slotIds.forEach((id) => blockedSlots.delete(id))
  return undefined
}

export async function mockMarkSlotOverbooked(payload: MarkSlotOverbookedPayload) {
  await delay()
  if (!buildSlots().some((slot) => slot.slotId === payload.slotId)) throw new Error('号源不存在')
  if (payload.overbookedFlag) overbookedSlots.add(payload.slotId)
  else overbookedSlots.delete(payload.slotId)
  return undefined
}
