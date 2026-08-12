// @vitest-environment jsdom

import { defineComponent, h } from 'vue'
import { flushPromises, mount } from '@vue/test-utils'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { ElMessage } from 'element-plus'

import type { InpatientDoctorPatientQueueItemVO } from '@/apis/inpatient/doctor-station'
import InpatientDischargeAdviceDialog from './InpatientDischargeAdviceDialog.vue'

const apiMocks = vi.hoisted(() => ({
  createInpatientDischargeAdvice: vi.fn()
}))

vi.mock('@/apis/inpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<Record<string, unknown>>()),
  ...apiMocks
}))

const slotStub = (name: string, tag = 'div') =>
  defineComponent({
    name,
    setup(_props, { slots }) {
      return () => h(tag, slots.default?.())
    }
  })

const dialogStub = defineComponent({
  name: 'ElDialog',
  props: { modelValue: Boolean },
  emits: ['update:modelValue'],
  setup(_props, { slots }) {
    return () => h('section', [slots.default?.(), slots.footer?.()])
  }
})

const formStub = defineComponent({
  name: 'ElForm',
  setup(_props, { expose, slots }) {
    expose({
      validate: async () => true,
      clearValidate: () => undefined
    })
    return () => h('form', slots.default?.())
  }
})

const selectStub = defineComponent({
  name: 'ElSelect',
  props: { modelValue: String },
  emits: ['update:modelValue'],
  setup(_props, { slots }) {
    return () => h('div', { class: 'select-stub' }, slots.default?.())
  }
})

const buttonStub = defineComponent({
  name: 'ElButton',
  props: { disabled: Boolean },
  emits: ['click'],
  setup(props, { emit, slots }) {
    return () =>
      h('button', { disabled: props.disabled, onClick: () => emit('click') }, slots.default?.())
  }
})

const patient: InpatientDoctorPatientQueueItemVO = {
  encounterId: 'enc-1',
  patientId: 'patient-1',
  patientName: '王建国',
  genderName: '男',
  ageText: '68岁',
  inpatientNo: 'ZY001',
  admissionAt: '2026-08-08 09:00:00',
  departmentName: '心血管内科',
  wardId: 'ward-1',
  wardName: '心内一病区',
  bedId: 'bed-1',
  bedName: '08床',
  statusEnum: 'IN_PROGRESS'
}

function mountDialog() {
  return mount(InpatientDischargeAdviceDialog, {
    props: {
      modelValue: true,
      patient
    },
    global: {
      stubs: {
        ElDialog: dialogStub,
        ElDescriptions: slotStub('ElDescriptions'),
        ElDescriptionsItem: slotStub('ElDescriptionsItem'),
        ElForm: formStub,
        ElFormItem: slotStub('ElFormItem'),
        ElSelect: selectStub,
        ElOption: slotStub('ElOption'),
        ElButton: buttonStub
      }
    }
  })
}

describe('InpatientDischargeAdviceDialog', () => {
  beforeEach(() => {
    vi.useFakeTimers()
    vi.setSystemTime(new Date('2026-08-08T10:30:00+08:00'))
    apiMocks.createInpatientDischargeAdvice.mockResolvedValue({
      id: 'order-1',
      encounterId: 'enc-1',
      statusEnum: 'ACTIVE',
      plannedDischargeTime: '2026-08-08 10:30:00',
      outWayCode: '1',
      outWayName: '医嘱离院',
      encounterNewStatus: 'ON_HOLD'
    })
  })

  afterEach(() => {
    vi.useRealTimers()
    vi.restoreAllMocks()
  })

  it('隐藏计划出院时间并在提交时自动使用当前时间', async () => {
    const successSpy = vi.spyOn(ElMessage, 'success').mockImplementation(() => undefined as never)
    const wrapper = mountDialog()

    expect(wrapper.findComponent({ name: 'ElDatePicker' }).exists()).toBe(false)
    wrapper.findComponent({ name: 'ElSelect' }).vm.$emit('update:modelValue', '1')
    await wrapper.vm.$nextTick()
    const submitButton = wrapper
      .findAll('button')
      .find((button) => button.text() === '提交出院医嘱')
    await submitButton!.trigger('click')
    await flushPromises()

    expect(apiMocks.createInpatientDischargeAdvice).toHaveBeenCalledWith({
      encounterId: 'enc-1',
      plannedDischargeTime: '2026-08-08 10:30:00',
      outWayCode: '1'
    })
    expect(wrapper.emitted('success')?.[0]?.[0]).toMatchObject({ id: 'order-1' })
    expect(successSpy).not.toHaveBeenCalled()
  })
})
