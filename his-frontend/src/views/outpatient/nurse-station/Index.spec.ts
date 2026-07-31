// @vitest-environment jsdom

import { flushPromises, mount } from '@vue/test-utils'
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
  performNurseExecutions: vi.fn(),
  printNurseDocuments: vi.fn()
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
    medicalRecordNo: 'M-A',
    visitNo: 'V-A',
    queueNumber: 1,
    registeredAt: '2026-07-29 08:00',
    departmentName: '内科',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    paymentTypeName: '医保',
    pendingCount: 1
  },
  {
    encounterId: 'enc-b',
    patientId: 'patient-b',
    patientName: '患者乙',
    genderName: '男',
    ageText: '40岁',
    medicalRecordNo: 'M-B',
    visitNo: 'V-B',
    queueNumber: 2,
    registeredAt: '2026-07-29 08:10',
    departmentName: '外科',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    paymentTypeName: '自费',
    pendingCount: 1
  }
]

function executionItem(itemId: string): NurseExecutionItemVO {
  return {
    itemId,
    requestId: `request-${itemId}`,
    requestType: 'SERVICE',
    requestTypeName: '处置',
    orderContent: `项目-${itemId}`,
    status: 'PENDING',
    statusName: '待执行',
    plannedCount: 1,
    executedCount: 0,
    version: 0,
    printTypes: ['TREATMENT_SHEET']
  }
}

const PatientQueuePanelStub = defineComponent({
  name: 'PatientQueuePanel',
  props: {
    patients: { type: Array, default: () => [] },
    dateRange: { type: Array, default: null },
    selectedId: { type: [String, Number], default: '' }
  },
  emits: ['select', 'view', 'search', 'dateChange', 'refresh', 'update:dateRange'],
  template: '<div class="patient-queue-stub" />'
})

const NurseExecutionPanelStub = defineComponent({
  name: 'NurseExecutionPanel',
  props: {
    items: { type: Array, default: () => [] },
    hasPatient: Boolean,
    loading: Boolean
  },
  emits: ['selectionChange', 'refresh', 'cancelExecute'],
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
    vi.clearAllMocks()
    api.pageNursePatientQueue.mockResolvedValue({
      pageIndex: 1,
      pageSize: 100,
      total: patientRows.length,
      pages: 1,
      rows: patientRows
    })
  })

  it('快速切换患者时忽略较晚返回的旧患者数据', async () => {
    const executionA = deferred<NurseExecutionItemVO[]>()
    const executionB = deferred<NurseExecutionItemVO[]>()
    const suppliesA = deferred<NurseConsumableItemVO[]>()
    const suppliesB = deferred<NurseConsumableItemVO[]>()

    api.getNurseExecutionItems.mockImplementation((encounterId: string) =>
      encounterId === 'enc-a' ? executionA.promise : executionB.promise
    )
    api.getNurseConsumables.mockImplementation((encounterId: string) =>
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
})
