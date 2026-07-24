// @vitest-environment jsdom

import { describe, expect, it } from 'vitest'

import {
  type ScheduleRecord,
  createSchedule,
  getScheduleDetail,
  pageSchedulesByRange,
  publishSchedule,
  stopSchedule,
  updateSchedule
} from '@/apis/booking/outpatient-schedule'
import { pageSlotsByDate } from '@/apis/booking/slot-calendar'
import { toScheduleEvents } from './outpatient-schedule/adapter'
import { toTimeSlots } from './slot-calendar/adapter'

function todayText() {
  const date = new Date()
  return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

describe('B组门诊排班和号源业务', () => {
  it('将接口排班转换为公共日历事件', async () => {
    const date = todayText()
    const rows = await pageSchedulesByRange({ start: date, end: date })
    const events = toScheduleEvents(rows)

    expect(events.length).toBeGreaterThan(0)
    expect(events[0]).toMatchObject({ date, key: rows[0]?.id, resourceKey: rows[0]?.doctorId })
    expect(events[0]?.raw).toEqual(rows[0])
  })

  it('将周视图和月视图范围记录完整转换为日历事件', () => {
    const base: ScheduleRecord = {
      id: 'week-record',
      departmentId: 'dept-internal',
      departmentName: '心血管内科',
      doctorId: 'doctor-wang',
      doctorName: '王建国',
      roomId: 'room-101',
      roomName: '门诊 101 诊室',
      date: '2026-07-20',
      startTime: '08:00',
      endTime: '12:00',
      capacity: 20,
      fee: 20,
      status: 'published',
      updatedAt: '2026-07-20T00:00:00.000Z'
    }
    const rows: ScheduleRecord[] = [
      base,
      { ...base, id: 'month-record', date: '2026-08-23', status: 'draft' }
    ]

    expect(toScheduleEvents(rows)).toMatchObject([
      { key: 'week-record', date: '2026-07-20', status: 'published' },
      { key: 'month-record', date: '2026-08-23', status: 'draft' }
    ])
  })

  it('完成新增、编辑、发布和停诊状态流转', async () => {
    const date = todayText()
    const created = await createSchedule({
      departmentId: 'dept-internal',
      doctorId: 'doctor-wang',
      roomId: 'room-101',
      date,
      startTime: '18:00',
      endTime: '20:00',
      capacity: 8,
      fee: 20,
      publish: false
    })
    expect(created.status).toBe('draft')

    const updated = await updateSchedule(created.id, { ...created, capacity: 10, publish: false })
    expect(updated.capacity).toBe(10)
    expect((await publishSchedule(created.id)).status).toBe('published')
    await expect(stopSchedule(created.id, '')).rejects.toThrow('停诊原因')

    const stopped = await stopSchedule(created.id, '医生临时调班')
    expect(stopped).toMatchObject({ status: 'stopped', stopReason: '医生临时调班' })
    expect(await getScheduleDetail(created.id)).toMatchObject({ status: 'stopped' })
  })

  it('由排班状态生成分组号源并映射公共号源模型', async () => {
    const rows = await pageSlotsByDate({ date: todayText() })
    const slots = toTimeSlots(rows)

    expect(slots.length).toBeGreaterThan(0)
    expect(slots.every((slot) => ['上午', '下午', '晚间'].includes(slot.group ?? ''))).toBe(true)
    expect(slots.some((slot) => slot.status === 'full')).toBe(true)
    expect(slots.some((slot) => slot.status === 'stopped')).toBe(true)
    expect(slots.every((slot) => slot.capacity !== undefined && slot.remaining !== undefined)).toBe(
      true
    )

    const yesterday = new Date()
    yesterday.setDate(yesterday.getDate() - 1)
    const yesterdayText = `${yesterday.getFullYear()}-${String(yesterday.getMonth() + 1).padStart(2, '0')}-${String(yesterday.getDate()).padStart(2, '0')}`
    const expiredSlots = toTimeSlots(await pageSlotsByDate({ date: yesterdayText }))
    expect(expiredSlots.some((slot) => slot.status === 'expired')).toBe(true)
  })

  it('取消旧请求时不返回过期查询结果', async () => {
    const controller = new AbortController()
    const request = pageSchedulesByRange(
      { start: todayText(), end: todayText() },
      { signal: controller.signal }
    )
    controller.abort()
    await expect(request).rejects.toMatchObject({ name: 'AbortError' })
  })
})
