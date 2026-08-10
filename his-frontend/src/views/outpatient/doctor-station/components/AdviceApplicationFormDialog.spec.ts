// @vitest-environment jsdom

import { defineComponent, h } from 'vue'
import { flushPromises, mount } from '@vue/test-utils'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import AdviceApplicationFormDialog from './AdviceApplicationFormDialog.vue'

const apiMocks = vi.hoisted(() => ({
  printAdviceApplicationForm: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<Record<string, unknown>>()),
  ...apiMocks
}))

const dialogStub = defineComponent({
  name: 'ElDialog',
  props: { modelValue: Boolean },
  emits: ['open', 'closed', 'update:modelValue'],
  setup(_props, { slots }) {
    return () => h('section', [slots.default?.(), slots.footer?.()])
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

function mountDialog() {
  return mount(AdviceApplicationFormDialog, {
    props: {
      modelValue: true,
      serviceRequestIds: ['SR-001', 'SR-002']
    },
    global: {
      stubs: {
        ElDialog: dialogStub,
        ElButton: buttonStub,
        ElSkeleton: true,
        EmptyState: true,
        AdviceApplicationFormView: true
      }
    }
  })
}

describe('AdviceApplicationFormDialog', () => {
  beforeEach(() => {
    apiMocks.printAdviceApplicationForm.mockImplementation(
      async ({ serviceRequestId }: { serviceRequestId: string }) => ({
        serviceRequestId,
        applicationNo: `AF-${serviceRequestId}`
      })
    )
    vi.stubGlobal('print', vi.fn())
  })

  afterEach(() => {
    vi.clearAllMocks()
    vi.unstubAllGlobals()
  })

  it('按选择顺序加载多张申请单并调用浏览器打印', async () => {
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    expect(apiMocks.printAdviceApplicationForm).toHaveBeenNthCalledWith(1, {
      serviceRequestId: 'SR-001'
    })
    expect(apiMocks.printAdviceApplicationForm).toHaveBeenNthCalledWith(2, {
      serviceRequestId: 'SR-002'
    })
    expect(wrapper.findAllComponents({ name: 'AdviceApplicationFormView' })).toHaveLength(2)

    const printButton = wrapper.findAll('button').find((button) => button.text() === '打印')
    await printButton!.trigger('click')
    expect(window.print).toHaveBeenCalledOnce()
    wrapper.unmount()
  })
})
