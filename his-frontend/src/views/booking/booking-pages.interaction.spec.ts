// @vitest-environment jsdom

import { defineComponent, nextTick } from 'vue'
import { flushPromises, shallowMount } from '@vue/test-utils'
import { beforeEach, describe, expect, it, vi } from 'vitest'
import { ElMessage } from 'element-plus'

import OutpatientSchedulePage from './outpatient-schedule/Index.vue'
import SlotCalendarPage from './slot-calendar/Index.vue'

const scheduleApi = vi.hoisted(() => ({
  loadScheduleInit: vi.fn(),
  pageSchedulesByRange: vi.fn(),
  getScheduleDetail: vi.fn(),
  createSchedule: vi.fn(),
  updateSchedule: vi.fn(),
  publishSchedule: vi.fn(),
  stopSchedule: vi.fn(),
  loadDepartmentOptions: vi.fn(),
  loadDoctorOptions: vi.fn()
}))

const slotApi = vi.hoisted(() => ({
  loadSlotInit: vi.fn(),
  pageSlotsByDate: vi.fn(),
  getSlotDetail: vi.fn(),
  loadDepartmentOptions: vi.fn(),
  loadDoctorOptions: vi.fn()
}))

vi.mock('@/apis/booking/outpatient-schedule', () => scheduleApi)
vi.mock('@/apis/booking/slot-calendar', () => slotApi)

const CalendarStub = defineComponent({
  name: 'CalendarSchedule',
  props: ['modelValue', 'view', 'events'],
  emits: ['rangeChange', 'dateClick', 'slotClick', 'eventClick'],
  template: '<div class="calendar-stub" />'
})

const TimeSlotGridStub = defineComponent({
  name: 'TimeSlotGrid',
  props: ['modelValue', 'slots'],
  emits: ['update:modelValue', 'select', 'disabledClick'],
  template: '<div class="slot-grid-stub" />'
})

const SearchPanelStub = defineComponent({
  name: 'SearchPanel',
  emits: ['search', 'reset'],
  template: '<div class="search-stub"><slot name="header" /></div>'
})

const AlertStub = defineComponent({
  props: ['title'],
  template: '<div class="alert-stub"><span>{{ title }}</span><slot /></div>'
})

const ButtonStub = defineComponent({
  emits: ['click'],
  template: '<button type="button" @click="$emit(\'click\')"><slot /></button>'
})

const editOpen = vi.fn()
const slotDetailOpen = vi.fn()
const slotDetailClose = vi.fn()

function createDeferred<T>() {
  let resolve!: (value: T) => void
  const promise = new Promise<T>((done) => {
    resolve = done
  })
  return { promise, resolve }
}

const ScheduleEditDialogStub = defineComponent({
  name: 'ScheduleEditDialog',
  setup(_, { expose }) {
    expose({ open: editOpen, close: vi.fn() })
    return () => null
  }
})

const SlotDetailDialogStub = defineComponent({
  name: 'SlotDetailDialog',
  props: ['data', 'loading'],
  setup(_, { expose }) {
    expose({ open: slotDetailOpen, close: slotDetailClose })
    return () => null
  }
})

const commonStubs = {
  SearchPanel: SearchPanelStub,
  RemoteSelectField: true,
  ActionToolbar: true,
  ScheduleDetailPanel: true,
  ScheduleEditDialog: ScheduleEditDialogStub,
  SlotDetailDialog: SlotDetailDialogStub,
  ElAlert: AlertStub,
  ElButton: ButtonStub,
  ElSelect: true,
  ElOption: true,
  ElDatePicker: true,
  ElTag: true
}

function mountSchedulePage() {
  return shallowMount(OutpatientSchedulePage, {
    global: {
      stubs: { ...commonStubs, CalendarSchedule: CalendarStub },
      directives: { loading: () => undefined }
    }
  })
}

function mountSlotPage() {
  return shallowMount(SlotCalendarPage, {
    global: {
      stubs: { ...commonStubs, TimeSlotGrid: TimeSlotGridStub },
      directives: { loading: () => undefined }
    }
  })
}

beforeEach(() => {
  vi.clearAllMocks()
  editOpen.mockClear()
  slotDetailOpen.mockClear()
  slotDetailClose.mockClear()
  scheduleApi.loadScheduleInit.mockResolvedValue({ departments: [], doctors: [], rooms: [] })
  scheduleApi.pageSchedulesByRange.mockResolvedValue([])
  scheduleApi.getScheduleDetail.mockResolvedValue({ id: 'schedule-1' })
  scheduleApi.loadDepartmentOptions.mockResolvedValue([])
  scheduleApi.loadDoctorOptions.mockResolvedValue([])
  slotApi.loadSlotInit.mockResolvedValue({ departments: [], doctors: [] })
  slotApi.pageSlotsByDate.mockResolvedValue([])
  slotApi.getSlotDetail.mockResolvedValue({ id: 'slot-1' })
  slotApi.loadDepartmentOptions.mockResolvedValue([])
  slotApi.loadDoctorOptions.mockResolvedValue([])
})

describe('B组页面事件连接', () => {
  it('范围变化重新查询，点击空日期区域打开新增排班', async () => {
    const wrapper = mountSchedulePage()
    await flushPromises()
    const calendar = wrapper.findComponent(CalendarStub)

    calendar.vm.$emit('rangeChange', { start: '2026-07-20', end: '2026-07-26' })
    await flushPromises()
    expect(scheduleApi.pageSchedulesByRange).toHaveBeenCalledWith(
      expect.objectContaining({ start: '2026-07-20', end: '2026-07-26' }),
      expect.objectContaining({ signal: expect.any(AbortSignal) })
    )

    calendar.vm.$emit('dateClick', '2026-07-24', { events: [] })
    await nextTick()
    expect(editOpen).toHaveBeenCalledWith(undefined, '2026-07-24')

    calendar.vm.$emit('dateClick', '2026-07-25', { events: [{ key: 'existing' }] })
    await nextTick()
    expect(editOpen).toHaveBeenCalledTimes(1)

    calendar.vm.$emit('slotClick', '2026-07-26', { events: [] })
    await nextTick()
    expect(editOpen).toHaveBeenLastCalledWith(undefined, '2026-07-26')

    calendar.vm.$emit('eventClick', { key: 'schedule-1', raw: { id: 'schedule-1' } })
    await flushPromises()
    expect(scheduleApi.getScheduleDetail).toHaveBeenCalledWith('schedule-1')
  })

  it('排班初始化失败后重新加载筛选选项', async () => {
    scheduleApi.loadScheduleInit
      .mockRejectedValueOnce(new Error('初始化失败'))
      .mockResolvedValueOnce({ departments: [], doctors: [], rooms: [] })
    const wrapper = mountSchedulePage()
    await flushPromises()

    expect(wrapper.text()).toContain('初始化失败')
    await wrapper.get('.alert-stub button').trigger('click')
    await flushPromises()
    expect(scheduleApi.loadScheduleInit).toHaveBeenCalledTimes(2)
  })

  it('号源初始化可以重试，重新查询会清空旧选择', async () => {
    slotApi.loadSlotInit
      .mockRejectedValueOnce(new Error('号源初始化失败'))
      .mockResolvedValueOnce({ departments: [], doctors: [] })
    const wrapper = mountSlotPage()
    await flushPromises()

    await wrapper.get('.alert-stub button').trigger('click')
    await flushPromises()
    expect(slotApi.loadSlotInit).toHaveBeenCalledTimes(2)

    const grid = wrapper.findComponent(TimeSlotGridStub)
    grid.vm.$emit('update:modelValue', 'slot-1')
    await nextTick()
    expect(grid.props('modelValue')).toBe('slot-1')

    wrapper.findComponent(SearchPanelStub).vm.$emit('search')
    await flushPromises()
    expect(grid.props('modelValue')).toBeNull()
    expect(slotDetailClose).toHaveBeenCalled()
  })

  it('已满、停诊和过期号源点击后显示对应不可用原因', async () => {
    const warning = vi.spyOn(ElMessage, 'warning').mockImplementation(() => undefined as never)
    const wrapper = mountSlotPage()
    await flushPromises()
    const grid = wrapper.findComponent(TimeSlotGridStub)

    grid.vm.$emit('disabledClick', {
      key: 'full',
      raw: { status: 'full', unavailableReason: '该时段号源已满' }
    })
    grid.vm.$emit('disabledClick', {
      key: 'stopped',
      raw: { status: 'stopped', unavailableReason: '医生临时停诊' }
    })
    grid.vm.$emit('disabledClick', {
      key: 'expired',
      raw: { status: 'expired', unavailableReason: '该时段已经过期' }
    })

    expect(warning).toHaveBeenNthCalledWith(1, '该时段号源已满')
    expect(warning).toHaveBeenNthCalledWith(2, '医生临时停诊')
    expect(warning).toHaveBeenNthCalledWith(3, '该时段已经过期')
    warning.mockRestore()
  })

  it('快速切换号源详情时旧请求不能覆盖新选择', async () => {
    const first = createDeferred<{ id: string }>()
    const second = createDeferred<{ id: string }>()
    slotApi.getSlotDetail.mockReturnValueOnce(first.promise).mockReturnValueOnce(second.promise)
    const wrapper = mountSlotPage()
    await flushPromises()
    const grid = wrapper.findComponent(TimeSlotGridStub)

    grid.vm.$emit('select', { key: 'slot-1', raw: { id: 'slot-1' } })
    await nextTick()
    grid.vm.$emit('select', { key: 'slot-2', raw: { id: 'slot-2' } })
    await nextTick()

    second.resolve({ id: 'slot-2' })
    await flushPromises()
    const dialog = wrapper.findComponent(SlotDetailDialogStub)
    expect(dialog.props('data')).toEqual({ id: 'slot-2' })

    first.resolve({ id: 'slot-1' })
    await flushPromises()
    expect(dialog.props('data')).toEqual({ id: 'slot-2' })
  })
})
