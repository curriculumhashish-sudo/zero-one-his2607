// @vitest-environment jsdom

import { defineComponent, nextTick } from 'vue'
import { flushPromises, shallowMount } from '@vue/test-utils'
import { beforeEach, describe, expect, it, vi } from 'vitest'
import { ElMessage } from 'element-plus'

import OutpatientSchedulePage from './outpatient-schedule/Index.vue'
import SlotCalendarPage from './slot-calendar/Index.vue'
import SlotDetailDialogComponent from './slot-calendar/components/SlotDetailDialog.vue'

const scheduleApi = vi.hoisted(() => ({
  loadScheduleInit: vi.fn<(...args: unknown[]) => unknown>(),
  pageSchedulesByRange: vi.fn<(...args: unknown[]) => unknown>(),
  getScheduleDetail: vi.fn<(...args: unknown[]) => unknown>(),
  createSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  updateSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  publishSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  stopSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  loadDepartmentOptions: vi.fn<(...args: unknown[]) => unknown>(),
  loadDoctorOptions: vi.fn<(...args: unknown[]) => unknown>(),
  loadScheduleGenerationOptions: vi.fn<(...args: unknown[]) => unknown>(),
  generateFormalSchedules: vi.fn<(...args: unknown[]) => unknown>(),
  clearScheduleSubstitution: vi.fn<(...args: unknown[]) => unknown>(),
  deleteReleasedSlots: vi.fn<(...args: unknown[]) => unknown>(),
  listReleasedSlots: vi.fn<(...args: unknown[]) => unknown>(),
  partialStopSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  previewScheduleImpact: vi.fn<(...args: unknown[]) => unknown>(),
  provisionScheduleSlots: vi.fn<(...args: unknown[]) => unknown>(),
  resumeSchedule: vi.fn<(...args: unknown[]) => unknown>(),
  setScheduleSubstitution: vi.fn<(...args: unknown[]) => unknown>(),
  unlockPartialStop: vi.fn<(...args: unknown[]) => unknown>(),
  voidSchedule: vi.fn<(...args: unknown[]) => unknown>()
}))

const slotApi = vi.hoisted(() => ({
  loadSlotInit: vi.fn<(...args: unknown[]) => unknown>(),
  pageSlotsByDate: vi.fn<(...args: unknown[]) => unknown>(),
  getSlotDetail: vi.fn<(...args: unknown[]) => unknown>(),
  loadDepartmentOptions: vi.fn<(...args: unknown[]) => unknown>(),
  loadDoctorOptions: vi.fn<(...args: unknown[]) => unknown>(),
  batchBlockSlots: vi.fn<(...args: unknown[]) => unknown>(),
  batchUnblockSlots: vi.fn<(...args: unknown[]) => unknown>(),
  markSlotOverbooked: vi.fn<(...args: unknown[]) => unknown>()
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

const DescriptionListStub = defineComponent({
  name: 'DescriptionList',
  props: ['data', 'items', 'column'],
  template: '<div class="description-list-stub"><slot name="value-status" /></div>'
})

const DrawerStub = defineComponent({
  name: 'ElDrawer',
  props: ['modelValue', 'title'],
  emits: ['update:modelValue'],
  template: '<section><slot /><slot name="footer" /></section>'
})

const ButtonStub = defineComponent({
  emits: ['click'],
  template: '<button type="button" @click="$emit(\'click\')"><slot /></button>'
})

const ActionToolbarStub = defineComponent({
  name: 'ActionToolbar',
  props: ['actions'],
  emits: ['action'],
  template:
    '<div class="toolbar-stub"><button v-for="action in actions" :key="action.key" :class="`action-${action.key}`" @click="$emit(\'action\', action)">{{ action.label }}</button></div>'
})

const editOpen = vi.fn<(...args: unknown[]) => unknown>()
const generateOpen = vi.fn<(...args: unknown[]) => unknown>()
const generateClose = vi.fn<(...args: unknown[]) => unknown>()
const slotDetailOpen = vi.fn<(...args: unknown[]) => unknown>()
const slotDetailClose = vi.fn<(...args: unknown[]) => unknown>()

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
    expose({ open: editOpen, close: vi.fn<() => void>() })
    return () => null
  }
})

const ScheduleGenerateDialogStub = defineComponent({
  name: 'ScheduleGenerateDialog',
  props: ['dailyTemplates', 'weeklyTemplates', 'submitting'],
  emits: ['submit'],
  setup(_, { expose }) {
    expose({ open: generateOpen, close: generateClose })
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
  ActionToolbar: ActionToolbarStub,
  ScheduleDetailPanel: true,
  ScheduleEditDialog: ScheduleEditDialogStub,
  ScheduleGenerateDialog: ScheduleGenerateDialogStub,
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
  generateOpen.mockClear()
  generateClose.mockClear()
  slotDetailOpen.mockClear()
  slotDetailClose.mockClear()
  scheduleApi.loadScheduleInit.mockResolvedValue({
    organizationId: 'org-main',
    departments: [],
    doctors: [],
    rooms: []
  })
  scheduleApi.pageSchedulesByRange.mockResolvedValue({ records: [], days: [], summary: {} })
  scheduleApi.getScheduleDetail.mockResolvedValue({ id: 'schedule-1' })
  scheduleApi.loadDepartmentOptions.mockResolvedValue([])
  scheduleApi.loadDoctorOptions.mockResolvedValue([])
  scheduleApi.loadScheduleGenerationOptions.mockResolvedValue({
    dailyTemplates: [{ value: 'daily-1', label: '心内科工作日' }],
    weeklyTemplates: [{ value: 'weekly-1', label: '内科默认周排班' }]
  })
  scheduleApi.generateFormalSchedules.mockResolvedValue({
    scheduleIds: ['generated-1'],
    generatedCount: 1,
    skippedCount: 0,
    provisionedSlotCount: 0
  })
  slotApi.loadSlotInit.mockResolvedValue({
    organizationId: 'org-main',
    departments: [],
    doctors: []
  })
  slotApi.pageSlotsByDate.mockResolvedValue({
    content: [],
    totalCount: 0,
    pageNo: 1,
    pageSize: 100
  })
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
      expect.objectContaining({
        organizationId: 'org-main',
        rangeStart: '2026-07-20',
        rangeEnd: '2026-07-26'
      }),
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

  it('生成正式排班从模板列表选择并按原型字段提交，不输入裸模板 ID', async () => {
    const wrapper = mountSchedulePage()
    await flushPromises()
    const calendar = wrapper.findComponent(CalendarStub)
    calendar.vm.$emit('rangeChange', { start: '2026-07-20', end: '2026-07-26' })
    await flushPromises()

    await wrapper.get('.action-generate').trigger('click')
    await flushPromises()
    expect(scheduleApi.loadScheduleGenerationOptions).toHaveBeenCalledWith('org-main')
    expect(generateOpen).toHaveBeenCalledWith({ start: '2026-07-20', end: '2026-07-26' })
    expect(wrapper.findComponent(ScheduleGenerateDialogStub).props()).toMatchObject({
      dailyTemplates: [{ value: 'daily-1', label: '心内科工作日' }],
      weeklyTemplates: [{ value: 'weekly-1', label: '内科默认周排班' }]
    })

    wrapper.findComponent(ScheduleGenerateDialogStub).vm.$emit('submit', {
      templateKind: 'DAY',
      scheduleTemplateId: 'daily-1',
      weekScheduleTemplateId: undefined,
      rangeStart: '2026-07-20',
      rangeEnd: '2026-07-26',
      autoPublish: true,
      autoProvision: false
    })
    await flushPromises()

    expect(scheduleApi.generateFormalSchedules).toHaveBeenCalledWith({
      organizationId: 'org-main',
      templateKind: 'DAY',
      scheduleTemplateId: 'daily-1',
      weekScheduleTemplateId: undefined,
      rangeStart: '2026-07-20',
      rangeEnd: '2026-07-26',
      autoPublish: true,
      autoProvision: false
    })
    expect(generateClose).toHaveBeenCalled()
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

  it('号源详情按原型 SlotDetailDto 字段和单列顺序展示', () => {
    const wrapper = shallowMount(SlotDetailDialogComponent, {
      props: {
        data: {
          slotId: 'slot-1',
          id: 'slot-1',
          scheduleId: 'schedule-1',
          organizationId: 'dept-internal',
          organizationName: '心血管内科',
          departmentId: 'dept-internal',
          departmentName: '心血管内科',
          practitionerId: 'doctor-wang',
          practitionerName: '王建国',
          doctorId: 'doctor-wang',
          doctorName: '王建国',
          healthcareServiceId: 'normal',
          healthcareServiceName: '普通号',
          roomName: '门诊 101 诊室',
          date: '2026-07-30',
          startTime: '2026-07-30 08:00:00',
          endTime: '2026-07-30 08:30:00',
          group: '上午',
          capacity: 10,
          bookedCount: 2,
          used: 2,
          remainingCount: 8,
          remaining: 8,
          statusCode: 'FREE',
          status: 'available',
          displayStatus: '可约',
          overbookedFlag: false,
          commentText: '演示号源'
        }
      },
      global: {
        stubs: {
          ElDrawer: DrawerStub,
          DescriptionList: DescriptionListStub,
          StatusDisplay: true,
          ElEmpty: true,
          ElButton: true
        },
        directives: { loading: () => undefined }
      }
    })
    const descriptions = wrapper.findComponent(DescriptionListStub)

    expect(descriptions.props('column')).toBe(1)
    expect(descriptions.props('items').map((item: { label: string }) => item.label)).toEqual([
      '科室',
      '医师',
      '号源类型',
      '状态',
      '时段',
      '容量',
      '已约',
      '剩余',
      '超售',
      '备注',
      '号源 ID',
      '日程 ID'
    ])
    expect(descriptions.props('data')).toMatchObject({
      organizationName: '心血管内科',
      practitionerName: '王建国',
      healthcareServiceName: '普通号',
      timeRange: '08:00 — 08:30',
      overbookedText: '否',
      commentText: '演示号源'
    })
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
