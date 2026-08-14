// @vitest-environment jsdom

import { flushPromises, mount } from '@vue/test-utils'
import dayjs from 'dayjs'
import { createPinia, setActivePinia } from 'pinia'
import { defineComponent } from 'vue'
import { beforeEach, describe, expect, it, vi } from 'vitest'

import type {
  NurseConsumableItemVO,
  NurseExecutionItemVO,
  NursePatientQueueItemVO
} from '@/apis/outpatient/nurse-station'

import NurseStation from './Index.vue'

const api = vi.hoisted(() => ({
  cancelNurseExecution: vi.fn(),
  getNurseConsumables: vi.fn(),
  getNurseExecutionItems: vi.fn(),
  pageNursePatientQueue: vi.fn(),
  printNurseExecutions: vi.fn(),
  performNurseExecutions: vi.fn()
}))

vi.mock('@/apis/outpatient/nurse-station', () => api)

interface Deferred<T> {
  promise: Promise<T>
  resolve: (value: T) => void
}

function deferred<T>(): Deferred<T> {
  let resolve!: (value: T) => void
  const promise = new Promise<T>((done) => {
    resolve = done
  })
  return { promise, resolve }
}

const patientRows: NursePatientQueueItemVO[] = [
  {
    encounterId: 'enc-a',
    patientId: 'patient-a',
    patientName: '患者甲',
    genderName: '女',
    ageText: '30岁',
    bookingDate: '2026-07-29',
    departmentName: '内科',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    priorityName: '优先'
  },
  {
    encounterId: 'enc-b',
    patientId: 'patient-b',
    patientName: '患者乙',
    genderName: '男',
    ageText: '40岁',
    bookingDate: '2026-07-29',
    departmentName: '外科',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    priorityName: '优先'
  }
]

function executionItem(itemId: string): NurseExecutionItemVO {
  return {
    itemId,
    encounterId: 'enc-a',
    categoryCode: 'SERVICE',
    categoryName: '处置',
    itemName: `项目-${itemId}`,
    status: 'PENDING',
    statusName: '待执行',
    executionCount: 0
  }
}

const PatientQueuePanelStub = defineComponent({
  name: 'PatientQueuePanel',
  props: {
    patients: { type: Array, default: () => [] },
    date: { type: String, default: '' },
    pagination: { type: Object, default: undefined },
    selectedId: { type: [String, Number], default: '' },
    showDepartment: { type: Boolean, default: true },
    showKeyword: { type: Boolean, default: true },
    showPrimaryNumber: { type: Boolean, default: true },
    primaryNumberMode: { type: String, default: 'auto' }
  },
  emits: ['select', 'view', 'search', 'singleDateChange', 'pageChange', 'refresh'],
  template: '<div class="patient-queue-stub" />'
})

const NurseExecutionPanelStub = defineComponent({
  name: 'NurseExecutionPanel',
  props: {
    items: { type: Array, default: () => [] },
    hasPatient: Boolean,
    loading: Boolean,
    printingItemId: String
  },
  emits: ['selectionChange', 'refresh', 'cancelExecute', 'print'],
  setup(_, { expose }) {
    expose({ clearSelection: vi.fn() })
    return {}
  },
  template: '<div class="execution-panel-stub" />'
})

const global = {
  stubs: {
    StationTriPaneLayout: {
      template: '<div><slot name="left" /><slot name="main" /><slot name="right" /></div>'
    },
    PatientQueuePanel: PatientQueuePanelStub,
    PatientInfoBar: { name: 'PatientInfoBar', template: '<div />' },
    NurseExecutionPanel: NurseExecutionPanelStub,
    NurseFooterBar: { name: 'NurseFooterBar', template: '<div />' },
    NurseRightSidebar: { name: 'NurseRightSidebar', template: '<div />' }
  }
}

describe('OutpatientNurseStation', () => {
  beforeEach(() => {
    setActivePinia(createPinia())
    vi.clearAllMocks()
    api.pageNursePatientQueue.mockImplementation((query: { pageIndex: number }) =>
      Promise.resolve({
        pageIndex: query.pageIndex,
        pageSize: 10,
        total: 22,
        pages: 3,
        rows: patientRows
      })
    )
  })

  it('分页查询患者并切换预约日期', async () => {
    const wrapper = mount(NurseStation, { global })
    await flushPromises()
    const queue = wrapper.findComponent(PatientQueuePanelStub)

    expect(queue.props('pagination')).toEqual({
      pageIndex: 1,
      pageSize: 10,
      total: 22
    })
    expect(queue.props('showDepartment')).toBe(false)
    expect(queue.props('showPrimaryNumber')).toBe(true)
    expect(queue.props('primaryNumberMode')).toBe('patient-no')
    expect(queue.props('date')).toBe(dayjs().format('YYYY-MM-DD'))
    expect(api.pageNursePatientQueue).toHaveBeenCalledWith(
      expect.objectContaining({
        pageIndex: 1,
        pageSize: 10,
        bookingDate: dayjs().format('YYYY-MM-DD')
      })
    )

    api.pageNursePatientQueue.mockClear()
    queue.vm.$emit('singleDateChange', '2026-07-30')
    await flushPromises()

    expect(api.pageNursePatientQueue).toHaveBeenCalledWith(
      expect.objectContaining({
        pageIndex: 1,
        pageSize: 10,
        bookingDate: '2026-07-30'
      })
    )

    api.pageNursePatientQueue.mockClear()
    queue.vm.$emit('pageChange', 2)
    await flushPromises()

    expect(api.pageNursePatientQueue).toHaveBeenCalledWith(
      expect.objectContaining({ pageIndex: 2, pageSize: 10 })
    )

    expect(queue.props('showKeyword')).toBe(false)
  })

  it('快速切换患者时忽略较晚返回的旧患者数据', async () => {
    const executionA = deferred<NurseExecutionItemVO[]>()
    const executionB = deferred<NurseExecutionItemVO[]>()
    const suppliesA = deferred<NurseConsumableItemVO[]>()
    const suppliesB = deferred<NurseConsumableItemVO[]>()

    api.getNurseExecutionItems.mockImplementation((_patientId: string, encounterId: string) =>
      encounterId === 'enc-a' ? executionA.promise : executionB.promise
    )
    api.getNurseConsumables.mockImplementation((_patientId: string, encounterId: string) =>
      encounterId === 'enc-a' ? suppliesA.promise : suppliesB.promise
    )

    const wrapper = mount(NurseStation, { global })
    await flushPromises()
    const queue = wrapper.findComponent(PatientQueuePanelStub)

    queue.vm.$emit('select', { id: 'enc-a' })
    await wrapper.vm.$nextTick()
    queue.vm.$emit('select', { id: 'enc-b' })
    await wrapper.vm.$nextTick()

    executionB.resolve([executionItem('item-b')])
    suppliesB.resolve([])
    await flushPromises()

    const executionPanel = wrapper.findComponent(NurseExecutionPanelStub)
    expect(executionPanel.props('items')).toEqual([expect.objectContaining({ itemId: 'item-b' })])

    executionA.resolve([executionItem('item-a')])
    suppliesA.resolve([])
    await flushPromises()

    expect(executionPanel.props('items')).toEqual([expect.objectContaining({ itemId: 'item-b' })])
  })

  it('请求打印文本并交给新窗口打印', async () => {
    const printDocument = document.implementation.createHTMLDocument('')
    const print = vi.fn()
    const printWindow = {
      document: printDocument,
      opener: window,
      closed: false,
      focus: vi.fn(),
      print,
      close: vi.fn()
    } as unknown as Window
    const openSpy = vi.spyOn(window, 'open').mockReturnValue(printWindow)
    api.printNurseExecutions.mockResolvedValue('服务请求打印单\n1. 记录ID：item-print')

    const wrapper = mount(NurseStation, { global })
    await flushPromises()
    wrapper.findComponent(NurseExecutionPanelStub).vm.$emit('print', executionItem('item-print'))
    await flushPromises()

    expect(api.printNurseExecutions).toHaveBeenCalledWith({
      recordIds: ['item-print'],
      printType: 'EXECUTION_RECORD'
    })
    expect(printDocument.body.textContent).toContain('服务请求打印单')
    expect(print).toHaveBeenCalledOnce()
    openSpy.mockRestore()
  })
})
