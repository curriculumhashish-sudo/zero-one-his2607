// @vitest-environment jsdom

import { describe, expect, it } from 'vitest'

import {
  type ScheduleRecord,
  createSchedule,
  generateFormalSchedules,
  getScheduleDetail,
  listReleasedSlots,
  pageSchedulesByRange,
  partialStopSchedule,
  provisionScheduleSlots,
  publishSchedule,
  resumeSchedule,
  stopSchedule,
  updateSchedule,
  voidSchedule
} from '@/apis/booking/outpatient-schedule'
import {
  batchBlockSlots,
  batchUnblockSlots,
  markSlotOverbooked,
  pageSlotsByDate
} from '@/apis/booking/slot-calendar'
import { toScheduleEvents } from './outpatient-schedule/adapter'
import { toTimeSlots } from './slot-calendar/adapter'
import { isSlotExpired } from '@/apis/booking/slot-calendar/mock'

function todayText() {
  const date = new Date()
  return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

function slotQuery(date: string) {
  return {
    organizationId: 'dept-internal',
    date,
    PageIndex: 1,
    PageSize: 100
  }
}

describe('B组门诊排班和号源业务', () => {
  it('当天号源按结束时间判断是否过期', () => {
    const now = new Date(2026, 6, 31, 10, 0, 0)
    expect(isSlotExpired('2026-07-31', '09:30', now)).toBe(true)
    expect(isSlotExpired('2026-07-31', '10:30', now)).toBe(false)
    expect(isSlotExpired('2026-07-30', '23:59', now)).toBe(true)
    expect(isSlotExpired('2026-08-01', '00:01', now)).toBe(false)
  })
  it('将接口排班转换为公共日历事件', async () => {
    const date = todayText()
    const rows = (
      await pageSchedulesByRange({
        organizationId: 'org-main',
        rangeStart: date,
        rangeEnd: date
      })
    ).records
    const events = toScheduleEvents(rows)

    expect(events.length).toBeGreaterThan(0)
    expect(events[0]).toMatchObject({ date, key: rows[0]?.id, resourceKey: rows[0]?.doctorId })
    expect(rows[0]).toMatchObject({
      scheduleId: rows[0]?.id,
      practitionerId: rows[0]?.doctorId,
      locationId: rows[0]?.roomId,
      statusEnum: rows[0]?.status
    })
    expect(rows[0]?.planningHorizonStart).toContain(`${date}T`)
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
      organizationId: 'org-main',
      actors: [
        { actorType: 10, actorId: 'doctor-wang' },
        { actorType: 20, actorId: 'room-101' },
        { actorType: 30, actorId: 'dept-internal' }
      ],
      planningHorizonStart: `${date}T18:00:00`,
      planningHorizonEnd: `${date}T20:00:00`
    })
    expect(Object.keys(created)).toEqual(['scheduleId'])
    const scheduleId = created.scheduleId!
    expect(await getScheduleDetail(scheduleId)).toMatchObject({ status: 'draft' })

    await updateSchedule({
      scheduleId,
      planningHorizonStart: `${date}T18:30:00`,
      planningHorizonEnd: `${date}T20:30:00`
    })
    await publishSchedule({
      scheduleId,
      planningHorizonStart: `${date}T18:30:00`,
      planningHorizonEnd: `${date}T20:30:00`
    })
    expect(await getScheduleDetail(scheduleId)).toMatchObject({ status: 'published' })
    await expect(stopSchedule({ scheduleId, reason: '' })).rejects.toThrow('停诊原因')

    await stopSchedule({ scheduleId, reason: '医生临时调班' })
    expect(await getScheduleDetail(scheduleId)).toMatchObject({
      status: 'stopped',
      stopReason: '医生临时调班'
    })
  })

  it('按原型状态门禁拒绝非法排班操作', async () => {
    const date = todayText()
    const created = await createSchedule({
      organizationId: 'org-main',
      actors: [
        { actorType: 10, actorId: 'doctor-wang' },
        { actorType: 20, actorId: 'room-101' },
        { actorType: 30, actorId: 'dept-internal' }
      ],
      planningHorizonStart: `${date}T21:00:00`,
      planningHorizonEnd: `${date}T22:00:00`
    })
    const scheduleId = created.scheduleId!

    await expect(stopSchedule({ scheduleId, reason: '非法停诊' })).rejects.toThrow(
      '只有已发布排班可以停诊'
    )
    await publishSchedule({
      scheduleId,
      planningHorizonStart: `${date}T21:00:00`,
      planningHorizonEnd: `${date}T22:00:00`
    })
    await expect(updateSchedule({ scheduleId, name: '非法编辑' })).rejects.toThrow(
      '只有草稿排班可以编辑'
    )
    await expect(
      publishSchedule({
        scheduleId,
        planningHorizonStart: `${date}T21:00:00`,
        planningHorizonEnd: `${date}T22:00:00`
      })
    ).rejects.toThrow('只有草稿排班可以发布')
    await expect(voidSchedule(scheduleId, '非法作废')).rejects.toThrow('只有草稿或停诊排班可以作废')

    await stopSchedule({ scheduleId, reason: '测试停诊' })
    await resumeSchedule(scheduleId)
    expect(await getScheduleDetail(scheduleId)).toMatchObject({ status: 'published' })
    await stopSchedule({ scheduleId, reason: '再次停诊' })
    await voidSchedule(scheduleId, '测试作废')
    await expect(resumeSchedule(scheduleId)).rejects.toThrow('只有停诊排班可以恢复开诊')
  })

  it('由排班状态生成分组号源并映射公共号源模型', async () => {
    const result = await pageSlotsByDate(slotQuery(todayText()))
    const rows = result.content
    const slots = toTimeSlots(rows)

    expect(slots.length).toBeGreaterThan(0)
    expect(slots.every((slot) => ['上午', '下午', '晚间'].includes(slot.group ?? ''))).toBe(true)
    expect(slots.some((slot) => ['full', 'expired'].includes(slot.status ?? ''))).toBe(true)
    expect(slots.every((slot) => slot.capacity !== undefined && slot.remaining !== undefined)).toBe(
      true
    )
    expect(rows[0]).toMatchObject({
      slotId: rows[0]?.id,
      organizationId: 'dept-internal',
      scheduleId: expect.any(String),
      practitionerId: rows[0]?.doctorId,
      practitionerName: expect.any(String),
      organizationName: expect.any(String),
      healthcareServiceName: expect.stringMatching(/普通号|专家号|专科号/),
      bookedCount: rows[0]?.used,
      remainingCount: rows[0]?.remaining,
      statusCode: rows[0]?.statusCode,
      displayStatus: expect.any(String),
      overbookedFlag: expect.any(Boolean)
    })

    const yesterday = new Date()
    yesterday.setDate(yesterday.getDate() - 1)
    const yesterdayText = `${yesterday.getFullYear()}-${String(yesterday.getMonth() + 1).padStart(2, '0')}-${String(yesterday.getDate()).padStart(2, '0')}`
    const expiredSchedule = await createSchedule({
      organizationId: 'org-main',
      actors: [
        { actorType: 10, actorId: 'doctor-wang' },
        { actorType: 20, actorId: 'room-101' },
        { actorType: 30, actorId: 'dept-internal' }
      ],
      planningHorizonStart: `${yesterdayText}T20:00:00`,
      planningHorizonEnd: `${yesterdayText}T21:00:00`
    })
    await publishSchedule({
      scheduleId: expiredSchedule.scheduleId!,
      planningHorizonStart: `${yesterdayText}T20:00:00`,
      planningHorizonEnd: `${yesterdayText}T21:00:00`
    })
    const expiredSlots = toTimeSlots((await pageSlotsByDate(slotQuery(yesterdayText))).content)
    expect(expiredSlots.some((slot) => slot.status === 'expired')).toBe(true)
  })

  it('取消旧请求时不返回过期查询结果', async () => {
    const controller = new AbortController()
    const request = pageSchedulesByRange(
      { organizationId: 'org-main', rangeStart: todayText(), rangeEnd: todayText() },
      { signal: controller.signal }
    )
    controller.abort()
    await expect(request).rejects.toMatchObject({ name: 'AbortError' })
  })

  it('支持正式排班生成、放号和号源状态维护', async () => {
    const date = '2026-10-05'
    const generated = await generateFormalSchedules({
      organizationId: 'org-main',
      weekScheduleTemplateId: 'weekly-1',
      templateKind: 'WEEK',
      rangeStart: date,
      rangeEnd: date,
      autoPublish: true,
      autoProvision: false
    })
    expect(generated.generatedCount + generated.skippedCount).toBe(1)

    const schedule = (
      await pageSchedulesByRange({ organizationId: 'org-main', rangeStart: date, rangeEnd: date })
    ).records.find((item) => item.departmentId === 'dept-internal' && item.status === 'published')
    expect(schedule).toBeTruthy()
    expect(schedule).toMatchObject({
      weekScheduleTemplateId: 'weekly-1',
      dayScheduleTemplateId: 'daily-1'
    })
    const provisioned = await provisionScheduleSlots({
      scheduleId: schedule!.id,
      organizationId: schedule!.organizationId!,
      slotTemplates: [{ startTime: '08:00', endTime: '08:30', capacity: 5 }]
    })
    expect(provisioned.generatedCount).toBe(1)
    expect(
      (await listReleasedSlots({ scheduleId: schedule!.id, PageIndex: 1, PageSize: 100 })).content
    ).toHaveLength(1)

    const slotPage = await pageSlotsByDate(slotQuery(date))
    const slot = slotPage.content.find((item) => item.status === 'available')
    expect(slot).toBeTruthy()
    await expect(
      batchBlockSlots({ slotIds: [slot!.slotId], reason: '接口联调测试' })
    ).resolves.toBeUndefined()
    expect(
      (await pageSlotsByDate(slotQuery(date))).content.find((item) => item.slotId === slot!.slotId)
    ).toMatchObject({ status: 'blocked', statusCode: 'BLOCKED', displayStatus: '已封锁' })
    await expect(batchUnblockSlots({ slotIds: [slot!.slotId] })).resolves.toBeUndefined()
    await expect(
      markSlotOverbooked({ slotId: slot!.slotId, overbookedFlag: true })
    ).resolves.toBeUndefined()
    expect(
      (await pageSlotsByDate(slotQuery(date))).content.find((item) => item.slotId === slot!.slotId)
        ?.overbookedFlag
    ).toBe(true)
  })

  it('按所选日排班模板生成医生、诊室、时段和号数', async () => {
    const date = '2026-09-29'
    const generated = await generateFormalSchedules({
      organizationId: 'org-main',
      scheduleTemplateId: 'daily-1',
      templateKind: 'DAY',
      rangeStart: date,
      rangeEnd: date,
      autoPublish: false,
      autoProvision: false
    })

    expect(generated).toMatchObject({ generatedCount: 1, skippedCount: 0 })
    const schedule = (
      await pageSchedulesByRange({ organizationId: 'org-main', rangeStart: date, rangeEnd: date })
    ).records[0]
    expect(schedule).toMatchObject({
      doctorId: 'doctor-wang',
      roomId: 'room-101',
      startTime: '08:00',
      endTime: '17:30',
      capacity: 70,
      status: 'draft',
      dayScheduleTemplateId: 'daily-1'
    })
  })

  it('部分停诊转移预约时要求原型 transferItems 字段', async () => {
    const date = todayText()
    const schedule = (
      await pageSchedulesByRange({ organizationId: 'org-main', rangeStart: date, rangeEnd: date })
    ).records.find((item) => item.status === 'published')!
    const slot = (await listReleasedSlots({ scheduleId: schedule.id, PageIndex: 1, PageSize: 100 }))
      .content[0]
    expect(slot).toBeTruthy()
    await expect(
      partialStopSchedule({
        organizationId: schedule.organizationId!,
        scheduleId: schedule.id,
        slotIds: [slot!.slotId],
        dispositionMode: 'TRANSFER'
      })
    ).rejects.toThrow('逐条预约与目标号源')
  })

  it('整日停诊复用原型预约处置契约并更新排班状态', async () => {
    const date = todayText()
    const created = await createSchedule({
      organizationId: 'org-main',
      actors: [
        { actorType: 10, actorId: 'doctor-wang' },
        { actorType: 20, actorId: 'room-101' },
        { actorType: 30, actorId: 'dept-internal' }
      ],
      planningHorizonStart: `${date}T20:00:00`,
      planningHorizonEnd: `${date}T21:00:00`
    })
    const schedule = await getScheduleDetail(created.scheduleId!)
    await publishSchedule({
      scheduleId: schedule.id,
      planningHorizonStart: `${date}T20:00:00`,
      planningHorizonEnd: `${date}T21:00:00`
    })
    const result = await partialStopSchedule({
      organizationId: schedule.organizationId!,
      scheduleId: schedule.id,
      slotIds: [],
      dispositionMode: 'CANCEL',
      reason: '整日停诊测试'
    })

    expect(result.scheduleDeactivated).toBe(true)
    expect(result.blockedSlotCount).toBeGreaterThan(0)
    expect(await getScheduleDetail(schedule.id)).toMatchObject({
      status: 'stopped',
      stopReason: '整日停诊测试'
    })
  })
})
