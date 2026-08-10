// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import ClinicalOrderEditor from './ClinicalOrderEditor.vue'
import type { AdviceScenarioAdapter } from './adapter'
import type { ClinicalOrderDraft } from './type'

const draftRows: ClinicalOrderDraft[] = [
  {
    key: 'draft-1',
    recordId: 'record-1',
    catalogId: 'drug-1',
    code: 'YP0001',
    name: '测试药品',
    category: 'DRUG',
    categoryName: '药品',
    dose: 5,
    unit: 'mg',
    frequency: '每日一次',
    route: '口服',
    days: 7,
    quantity: 7,
    quantityUnit: '片',
    status: 'DRAFT'
  },
  {
    key: 'draft-2',
    recordId: 'record-2',
    catalogId: 'lab-1',
    code: 'JY0001',
    name: '血常规',
    category: 'LAB',
    categoryName: '检验',
    quantity: 1,
    quantityUnit: '次',
    status: 'DRAFT'
  },
  {
    key: 'signed-1',
    recordId: 'record-3',
    catalogId: 'exam-1',
    code: 'JC0001',
    name: '胸部 CT',
    category: 'EXAM',
    categoryName: '检查',
    quantity: 1,
    quantityUnit: '次',
    status: 'SIGNED',
    serviceRequestId: 'service-request-3'
  }
]

const adapter: AdviceScenarioAdapter = {
  scope: 'outpatient',
  createDraft: () => draftRows[0]!,
  validate: () => ({ valid: true, errors: [] }),
  mapSaveRequest: () => ({})
}

const buttonStub = {
  props: ['disabled', 'loading'],
  emits: ['click'],
  template: '<button :disabled="disabled" @click="$emit(\'click\')"><slot /></button>'
}

function mountEditor() {
  return mount(ClinicalOrderEditor, {
    props: {
      modelValue: draftRows,
      context: {
        scope: 'outpatient',
        patientId: 'patient-1',
        encounterKey: 'encounter-1'
      },
      adapter,
      patientSelected: true,
      accepted: true,
      showFilters: false,
      total: draftRows.length
    },
    global: {
      stubs: {
        ElButton: buttonStub,
        ElTag: { template: '<span><slot /></span>' },
        ElInput: true,
        ElSelect: true,
        ElOption: true,
        ElAlert: true,
        EmptyState: true,
        PaginationBar: true,
        OrderSelectorDialog: true,
        AdviceOrderTable: {
          name: 'AdviceOrderTable',
          props: ['rows'],
          emits: ['selectionChange'],
          template: '<div class="table-stub" />'
        }
      }
    }
  })
}

function buttonByText(wrapper: ReturnType<typeof mountEditor>, text: string) {
  return wrapper.findAll('button').find((button) => button.text().trim() === text)!
}

describe('ClinicalOrderEditor batch permissions', () => {
  it('在顶部操作栏根据选中行状态启用删除、分组、签发和撤回', async () => {
    const wrapper = mountEditor()
    const table = wrapper.getComponent({ name: 'AdviceOrderTable' })
    const toolbarActions = wrapper.get('.clinical-order-editor__actions')

    expect(toolbarActions.text()).toContain('历史医嘱')
    expect(toolbarActions.text()).toContain('新增医嘱')
    expect(wrapper.text()).not.toContain('查询')
    expect(wrapper.text()).not.toContain('重置')
    expect(wrapper.text()).not.toContain('最近保存')
    expect(buttonByText(wrapper, '删除').attributes('disabled')).toBeDefined()
    expect(buttonByText(wrapper, '签发').attributes('disabled')).toBeDefined()
    expect(buttonByText(wrapper, '撤回').attributes('disabled')).toBeDefined()

    table.vm.$emit('selectionChange', [draftRows[0]])
    await wrapper.vm.$nextTick()
    expect(buttonByText(wrapper, '删除').attributes('disabled')).toBeUndefined()
    expect(buttonByText(wrapper, '签发').attributes('disabled')).toBeUndefined()
    expect(buttonByText(wrapper, '并组').attributes('disabled')).toBeDefined()

    table.vm.$emit('selectionChange', draftRows.slice(0, 2))
    await wrapper.vm.$nextTick()
    expect(buttonByText(wrapper, '并组').attributes('disabled')).toBeUndefined()
    await buttonByText(wrapper, '并组').trigger('click')
    const groupedRows = wrapper.emitted('update:modelValue')?.at(-1)?.[0] as
      | ClinicalOrderDraft[]
      | undefined
    expect(groupedRows?.[0]?.groupId).toBeTruthy()
    expect(groupedRows?.[0]?.groupId).toBe(groupedRows?.[1]?.groupId)

    table.vm.$emit('selectionChange', [draftRows[2]])
    await wrapper.vm.$nextTick()
    expect(buttonByText(wrapper, '签发').attributes('disabled')).toBeDefined()
    expect(buttonByText(wrapper, '撤回').attributes('disabled')).toBeDefined()

    wrapper.unmount()
  })
})
