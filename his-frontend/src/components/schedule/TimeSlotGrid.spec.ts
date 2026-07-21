// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import TimeSlotGrid from './TimeSlotGrid.vue'
import type { TimeSlot, TimeSlotStatusMap } from './type'

const global = {
  stubs: {
    ElSkeleton: { template: '<div class="skeleton" />' }
  }
}

const slots: TimeSlot[] = [
  { key: 'a', start: '08:00', end: '08:30', remaining: 2, capacity: 5, status: 'available', group: '上午' },
  { key: 'b', start: '08:30', end: '09:00', remaining: 0, capacity: 5, status: 'full', group: '上午' },
  { key: 'c', start: '14:00', end: '14:30', remaining: 3, status: 'available', group: '下午' }
]

describe('C29 TimeSlotGrid', () => {
  it('groups slots by field and emits a controlled selection', async () => {
    const wrapper = mount(TimeSlotGrid, { props: { slots, groupBy: 'group' }, global })
    expect(wrapper.text()).toContain('上午')
    expect(wrapper.text()).toContain('下午')
    await wrapper.findAll('.time-slot-grid__item')[0]!.trigger('click')
    expect(wrapper.emitted('update:modelValue')?.[0]).toEqual(['a'])
    expect(wrapper.emitted('select')?.[0]).toEqual([slots[0]])
  })

  it('treats zero-remaining slot as unavailable and emits disabledClick', async () => {
    const wrapper = mount(TimeSlotGrid, { props: { slots }, global })
    const full = wrapper.findAll('.time-slot-grid__item')[1]!
    expect(full.classes()).toContain('is-disabled')
    await full.trigger('click')
    expect(wrapper.emitted('update:modelValue')).toBeUndefined()
    expect(wrapper.emitted('disabledClick')?.[0]).toEqual([slots[1]])
  })

  it('honors statusMap.selectable override over the default rule', async () => {
    const statusMap: TimeSlotStatusMap = {
      locked: { text: '锁定', type: 'warning', selectable: false },
      preferred: { text: '推荐', type: 'success', selectable: true }
    }
    const custom: TimeSlot[] = [
      { key: 'x', start: '10:00', end: '10:30', status: 'preferred' },
      { key: 'y', start: '10:30', end: '11:00', status: 'locked' }
    ]
    const wrapper = mount(TimeSlotGrid, { props: { slots: custom, statusMap }, global })
    const items = wrapper.findAll('.time-slot-grid__item')
    await items[0]!.trigger('click')
    expect(wrapper.emitted('update:modelValue')?.[0]).toEqual(['x'])
    await items[1]!.trigger('click')
    expect(wrapper.emitted('disabledClick')?.[0]).toEqual([custom[1]])
    expect(items[1]!.classes()).toContain('is-warning')
  })

  it('blocks selection entirely in readonly mode', async () => {
    const wrapper = mount(TimeSlotGrid, { props: { slots, readonly: true }, global })
    await wrapper.findAll('.time-slot-grid__item')[0]!.trigger('click')
    expect(wrapper.emitted('update:modelValue')).toBeUndefined()
    expect(wrapper.emitted('disabledClick')?.[0]).toEqual([slots[0]])
  })

  it('renders capacity text and falls back to label when times are absent', () => {
    const custom: TimeSlot[] = [{ key: 'z', label: '专家号', capacity: 8, status: 'available' }]
    const wrapper = mount(TimeSlotGrid, { props: { slots: custom }, global })
    expect(wrapper.text()).toContain('专家号')
    expect(wrapper.text()).toContain('容量 8')
  })
})
