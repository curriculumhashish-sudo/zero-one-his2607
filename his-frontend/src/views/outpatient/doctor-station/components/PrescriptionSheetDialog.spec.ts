// @vitest-environment jsdom

import { defineComponent, h } from 'vue'
import { flushPromises, mount } from '@vue/test-utils'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import type { PatientSummary } from '@/components/patient/type'

import PrescriptionSheetDialog from './PrescriptionSheetDialog.vue'

const apiMocks = vi.hoisted(() => ({
  queryDoctorPrescriptionSheets: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<Record<string, unknown>>()),
  ...apiMocks
}))

const patient: PatientSummary = {
  id: 'enc-prescription-test',
  name: '吴国平',
  patientNo: 'MR20260008',
  visitNo: 'OP202607230008',
  departmentName: '心血管内科'
}

const currentSheet = {
  patientName: '吴国平',
  patientId: 'OP202607230008',
  prescriptionNo: 'RX-CURRENT-001',
  items: '阿司匹林肠溶片'
}

const historySheet = {
  patientName: '吴国平',
  patientId: 'HOP-1008-001',
  prescriptionNo: 'RX-HISTORY-001',
  items: '苯磺酸氨氯地平片'
}

const dialogStub = defineComponent({
  name: 'ElDialog',
  props: { modelValue: Boolean },
  emits: ['open', 'closed', 'update:modelValue'],
  setup(_props, { slots }) {
    return () => h('section', [slots.default?.(), slots.footer?.()])
  }
})

const radioGroupStub = defineComponent({
  name: 'ElRadioGroup',
  props: { modelValue: String },
  emits: ['change', 'update:modelValue'],
  setup(_props, { slots }) {
    return () => h('div', { class: 'radio-group-stub' }, slots.default?.())
  }
})

const emptyStateStub = defineComponent({
  name: 'EmptyState',
  props: { title: String, description: String, actionText: String },
  emits: ['action'],
  setup(props, { emit }) {
    return () =>
      h('div', { class: 'empty-state-stub' }, [
        props.title,
        props.description,
        props.actionText ? h('button', { onClick: () => emit('action') }, props.actionText) : null
      ])
  }
})

function mountDialog() {
  return mount(PrescriptionSheetDialog, {
    props: { modelValue: true, patient },
    global: {
      stubs: {
        ElDialog: dialogStub,
        ElRadioGroup: radioGroupStub,
        ElRadioButton: true,
        ElSkeleton: true,
        EmptyState: emptyStateStub,
        PrescriptionSheetView: true
      }
    }
  })
}

describe('PrescriptionSheetDialog', () => {
  beforeEach(() => {
    apiMocks.queryDoctorPrescriptionSheets.mockImplementation(async ({ type }: { type: string }) =>
      type === 'HISTORY' ? [historySheet] : [currentSheet]
    )
  })

  afterEach(() => {
    vi.clearAllMocks()
  })

  it('打开时默认查询本次就诊处方单', async () => {
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    expect(apiMocks.queryDoctorPrescriptionSheets).toHaveBeenCalledWith({
      num: 'MR20260008',
      encounterId: 'enc-prescription-test',
      paymentTypeName: undefined,
      type: 'CURRENT_ENCOUNTER'
    })
    const sheet = wrapper.getComponent({ name: 'PrescriptionSheetView' })
    expect(sheet.props('data').prescriptionNo).toBe('RX-CURRENT-001')
    wrapper.unmount()
  })

  it('切换到历史处方后重新查询并替换展示内容', async () => {
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    wrapper.getComponent({ name: 'ElRadioGroup' }).vm.$emit('change', 'HISTORY')
    await flushPromises()

    expect(apiMocks.queryDoctorPrescriptionSheets).toHaveBeenLastCalledWith({
      num: 'MR20260008',
      encounterId: 'enc-prescription-test',
      paymentTypeName: undefined,
      type: 'HISTORY'
    })
    expect(
      wrapper.getComponent({ name: 'PrescriptionSheetView' }).props('data').prescriptionNo
    ).toBe('RX-HISTORY-001')
    wrapper.unmount()
  })

  it('加载失败时提供重试入口', async () => {
    apiMocks.queryDoctorPrescriptionSheets
      .mockRejectedValueOnce(new Error('处方服务暂不可用'))
      .mockResolvedValueOnce([currentSheet])
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    expect(wrapper.text()).toContain('处方服务暂不可用')
    const retryButton = wrapper.findAll('button').find((button) => button.text() === '重试')
    await retryButton!.trigger('click')
    await flushPromises()

    expect(apiMocks.queryDoctorPrescriptionSheets).toHaveBeenCalledTimes(2)
    expect(
      wrapper.getComponent({ name: 'PrescriptionSheetView' }).props('data').prescriptionNo
    ).toBe('RX-CURRENT-001')
    wrapper.unmount()
  })
})
