// @vitest-environment jsdom

import { defineComponent, h } from 'vue'
import { flushPromises, mount } from '@vue/test-utils'
import { ElMessageBox } from 'element-plus'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import type { PatientSummary } from '@/components/patient/type'

import RefundApplicationDialog from './RefundApplicationDialog.vue'

const apiMocks = vi.hoisted(() => ({
  applyDoctorRefund: vi.fn(),
  queryDoctorRefundItems: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<Record<string, unknown>>()),
  ...apiMocks
}))

const patient: PatientSummary = {
  id: 'enc-refund-test',
  name: '王志强',
  gender: '男',
  age: 67,
  patientNo: 'MR20260003',
  visitNo: 'OP202607230003',
  departmentName: '心血管内科',
  status: '在诊'
}

const refundRows = [
  {
    patientName: '王志强',
    paymentNo: 'PAY-001',
    itemName: '阿司匹林肠溶片',
    quantity: '20 片',
    amount: '18.60',
    chargeStatus: '已收费',
    refundStatus: '未申请'
  },
  {
    patientName: '王志强',
    paymentNo: 'PAY-001',
    itemName: '血常规检查',
    quantity: '1 次',
    amount: '25.00',
    chargeStatus: '已收费',
    refundStatus: '未申请'
  },
  {
    patientName: '王志强',
    paymentNo: 'PAY-002',
    itemName: '十二导联心电图',
    quantity: '1 次',
    amount: '30.00',
    chargeStatus: '已收费',
    refundStatus: '已退费'
  }
]

/**
 * 轻量表格桩保留 Element Plus 选择行为：组件调用默认全选方法时，
 * 桩会发出 selection-change，从而能测试实际提交数据而不是内部实现。
 */
const tableStub = defineComponent({
  name: 'ElTable',
  props: {
    data: {
      type: Array,
      default: () => []
    }
  },
  emits: ['selection-change'],
  setup(_props, { emit, expose }) {
    const selectedRows: unknown[] = []

    function clearSelection() {
      selectedRows.splice(0)
      emit('selection-change', [])
    }

    function toggleRowSelection(row: unknown, selected = true) {
      const index = selectedRows.indexOf(row)
      if (selected && index < 0) selectedRows.push(row)
      if (!selected && index >= 0) selectedRows.splice(index, 1)
      emit('selection-change', [...selectedRows])
    }

    expose({ clearSelection, toggleRowSelection })
    return () => h('div', { class: 'el-table-stub' })
  }
})

const dialogStub = defineComponent({
  name: 'ElDialog',
  props: {
    modelValue: Boolean
  },
  emits: ['open', 'closed', 'update:modelValue'],
  setup(_props, { slots }) {
    return () => h('section', { class: 'el-dialog-stub' }, [slots.default?.(), slots.footer?.()])
  }
})

const alertStub = defineComponent({
  name: 'ElAlert',
  props: {
    title: String
  },
  setup(props) {
    return () => h('div', { class: 'el-alert-stub' }, props.title)
  }
})

function mountDialog() {
  return mount(RefundApplicationDialog, {
    props: {
      modelValue: true,
      patient
    },
    global: {
      stubs: {
        ElDialog: dialogStub,
        ElTable: tableStub,
        ElTableColumn: true,
        ElSkeleton: true,
        ElAlert: alertStub,
        ElTag: true,
        DetailSummary: true
      }
    }
  })
}

describe('RefundApplicationDialog', () => {
  beforeEach(() => {
    apiMocks.queryDoctorRefundItems.mockResolvedValue(refundRows)
    apiMocks.applyDoctorRefund.mockResolvedValue('RF202608010001')
  })

  afterEach(() => {
    vi.restoreAllMocks()
    vi.clearAllMocks()
  })

  it('打开时按病历号查询，并默认勾选所有可退项目', async () => {
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    expect(apiMocks.queryDoctorRefundItems).toHaveBeenCalledWith({ num: 'MR20260003' })
    expect(wrapper.text()).toContain('可退 2 项')
    expect(wrapper.text()).toContain('已选 2 项')
    expect(wrapper.text()).toContain('¥43.60')
    wrapper.unmount()
  })

  it('确认后按后端契约提交勾选数和金额，并返回申请号', async () => {
    vi.spyOn(ElMessageBox, 'confirm').mockResolvedValue('confirm' as never)
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    const submitButton = wrapper
      .findAll('button')
      .find((button) => button.text().includes('提交申请'))
    expect(submitButton).toBeTruthy()
    await submitButton!.trigger('click')
    await flushPromises()

    expect(apiMocks.applyDoctorRefund).toHaveBeenCalledWith({
      patientNo: 'MR20260003',
      apply: refundRows.slice(0, 2),
      applyNo: 2,
      applySum: 43.6
    })
    expect(wrapper.emitted('submitted')).toEqual([['RF202608010001']])
    expect(wrapper.emitted('update:modelValue')).toContainEqual([false])
    wrapper.unmount()
  })

  it('加载失败时展示错误并允许重新加载', async () => {
    apiMocks.queryDoctorRefundItems
      .mockRejectedValueOnce(new Error('收费系统暂不可用'))
      .mockResolvedValueOnce(refundRows)
    const wrapper = mountDialog()
    wrapper.getComponent({ name: 'ElDialog' }).vm.$emit('open')
    await flushPromises()

    expect(wrapper.text()).toContain('收费系统暂不可用')
    const retryButton = wrapper
      .findAll('button')
      .find((button) => button.text().includes('重新加载'))
    await retryButton!.trigger('click')
    await flushPromises()

    expect(apiMocks.queryDoctorRefundItems).toHaveBeenCalledTimes(2)
    expect(wrapper.text()).toContain('可退 2 项')
    wrapper.unmount()
  })
})
