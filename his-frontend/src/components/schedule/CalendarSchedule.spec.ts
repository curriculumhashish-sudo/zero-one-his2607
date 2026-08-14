// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import CalendarSchedule from './CalendarSchedule.vue'
import type { ScheduleEvent, ScheduleStatusMap } from './type'

const global = {
  stubs: {
    ElButton: { template: '<button><slot /></button>' },
    ElButtonGroup: { template: '<div><slot /></div>' },
    ElRadioGroup: {
      props: ['modelValue'],
      emits: ['update:modelValue'],
      template: '<div><slot /></div>'
    },
    ElRadioButton: { props: ['value'], template: '<label><slot /></label>' },
    ElSkeleton: { template: '<div class="skeleton" />' }
  }
}

const events: ScheduleEvent[] = [
  { key: 'a', date: '2026-07-20', start: '08:00', end: '12:00', title: '张医生门诊', status: 'open' },
  { key: 'b', date: '2026-07-22', start: '13:00', end: '17:00', title: '李医生门诊', status: 'closed' }
]

const statusMap: ScheduleStatusMap = {
  open: { text: '可约', type: 'success' },
  closed: { text: '停诊', type: 'danger' }
}

describe('C28 CalendarSchedule', () => {
  it('renders a controlled week and reports its inclusive range once', () => {
    const wrapper = mount(CalendarSchedule, {
      props: { modelValue: '2026-07-20', events, statusMap },
      global
    })
    expect(wrapper.text()).toContain('张医生门诊')
    expect(wrapper.text()).toContain('可约')
    const emitted = wrapper.emitted('rangeChange')
    expect(emitted).toHaveLength(1)
    expect(emitted?.[0]).toEqual([{ start: '2026-07-20', end: '2026-07-26' }])
  })

  it('moves by a whole week without mutating the controlled value', async () => {
    const wrapper = mount(CalendarSchedule, {
      props: { modelValue: '2026-07-20', events },
      global
    })
    const buttons = wrapper.findAll('button')
    await buttons[0]!.trigger('click') // 上一周
    expect(wrapper.emitted('update:modelValue')?.[0]).toEqual(['2026-07-13'])
    expect(wrapper.props('modelValue')).toBe('2026-07-20')
  })

  it('re-emits range when switching from week to month view', async () => {
    const wrapper = mount(CalendarSchedule, {
      props: { modelValue: '2026-07-20', view: 'week', events },
      global
    })
    expect(wrapper.emitted('rangeChange')?.length).toBe(1)
    await wrapper.setProps({ view: 'month' })
    const emitted = wrapper.emitted('rangeChange')
    expect(emitted?.length).toBe(2)
    // 2026-07 月视图，周一为首日：网格起点为 2026-06-29
    expect(emitted?.[1]).toEqual([{ start: '2026-06-29', end: '2026-08-09' }])
  })

  it('collapses dense events and surfaces the day via dateClick on more', async () => {
    const dense: ScheduleEvent[] = Array.from({ length: 5 }, (_, index) => ({
      key: index,
      date: '2026-07-20',
      start: '08:00',
      end: '09:00',
      title: `排班${index}`
    }))
    const wrapper = mount(CalendarSchedule, {
      props: { modelValue: '2026-07-20', events: dense, maxEventsPerDay: 2 },
      global
    })
    expect(wrapper.text()).toContain('+3 更多')
    await wrapper.get('.calendar-schedule__more').trigger('click')
    const clicked = wrapper.emitted('dateClick')?.[0]
    expect(clicked?.[0]).toBe('2026-07-20')
    expect((clicked?.[1] as { events: ScheduleEvent[] }).events).toHaveLength(5)
  })

  it('suppresses interaction emits when readonly', async () => {
    const wrapper = mount(CalendarSchedule, {
      props: { modelValue: '2026-07-20', events, readonly: true },
      global
    })
    await wrapper.get('.calendar-schedule__day').trigger('click')
    expect(wrapper.emitted('dateClick')).toBeUndefined()
  })
})
