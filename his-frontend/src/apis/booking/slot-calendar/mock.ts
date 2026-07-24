import {
  getMockScheduleSnapshot,
  mockLoadDepartmentOptions,
  mockLoadDoctorOptions
} from '../outpatient-schedule/mock'
import type { RequestContext, SlotQuery, SlotRecord, SlotStatus } from './type'

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
        const provisionalRemaining = index % 4 === 1 ? 0 : Math.max(1, capacity - (index % 3))
        const status = statusOf(schedule.status, schedule.date, index, provisionalRemaining)
        const remaining = status === 'full' ? 0 : provisionalRemaining
        return {
          id: `${schedule.id}-slot-${index + 1}`,
          scheduleId: schedule.id,
          departmentId: schedule.departmentId,
          departmentName: schedule.departmentName,
          doctorId: schedule.doctorId,
          doctorName: schedule.doctorName,
          roomName: schedule.roomName,
          date: schedule.date,
          startTime: minutesToTime(slotStart),
          endTime: minutesToTime(slotEnd),
          group: groupOf(minutesToTime(slotStart)),
          capacity,
          used: capacity - remaining,
          remaining,
          status,
          unavailableReason:
            status === 'stopped'
              ? schedule.stopReason || '该排班已停诊'
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
  const keyword = query.keyword?.trim().toLocaleLowerCase()
  return structuredClone(
    buildSlots().filter((item) => {
      if (item.date !== query.date) return false
      if (query.departmentId && item.departmentId !== query.departmentId) return false
      if (query.doctorId && item.doctorId !== query.doctorId) return false
      if (
        keyword &&
        !`${item.doctorName}${item.departmentName}${item.roomName}${item.startTime}`
          .toLocaleLowerCase()
          .includes(keyword)
      )
        return false
      return true
    })
  )
}

export async function mockGetSlotDetail(id: string, context: RequestContext = {}) {
  await delay(context, 180)
  const slot = buildSlots().find((item) => item.id === id)
  if (!slot) throw new Error('号源不存在或已发生变化')
  return structuredClone(slot)
}

export async function mockLoadSlotInit(context: RequestContext = {}) {
  const [departments, doctors] = await Promise.all([
    mockLoadDepartmentOptions('', context),
    mockLoadDoctorOptions('', null, context)
  ])
  return { departments, doctors }
}

export const mockLoadSlotDepartmentOptions = mockLoadDepartmentOptions
export const mockLoadSlotDoctorOptions = mockLoadDoctorOptions
