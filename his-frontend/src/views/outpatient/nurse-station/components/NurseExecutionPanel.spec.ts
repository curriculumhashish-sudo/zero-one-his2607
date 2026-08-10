// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { defineComponent, h } from 'vue'
import { beforeEach, describe, expect, it, vi } from 'vitest'

import type { NurseExecutionItemVO } from '@/apis/outpatient/nurse-station'
import type { AdviceOrder } from '@/components/medical/type'

import NurseExecutionPanel from './NurseExecutionPanel.vue'

const clearSelectionSpy = vi.fn()

const AdviceOrderTableStub = defineComponent({
  name: 'AdviceOrderTable',
  props: {
    rows: { type: Array, default: () => [] },
    disabledRow: { type: Function, default: () => false }
  },
  emits: ['selectionChange'],
  setup(props, { emit, expose, slots }) {
    expose({ clearSelection: clearSelectionSpy })
    return () => {
      const rows = props.rows as AdviceOrder[]
      return h('div', { class: 'advice-table-stub' }, [
        h(
          'button',
          {
            class: 'select-all',
            onClick: () => emit('selectionChange', rows)
          },
          '模拟全选'
        ),
        ...rows.flatMap((row) => slots.actions?.({ row }) ?? [])
      ])
    }
  }
})

const global = {
  stubs: {
    AdviceOrderTable: AdviceOrderTableStub,
    EmptyState: { template: '<div class="empty-state" />' },
    SectionTitle: { template: '<div class="section-title" />' },
    ElButton: {
      props: ['disabled', 'type'],
      emits: ['click'],
      template: '<button :disabled="disabled" @click="$emit(\'click\')"><slot /></button>'
    },
    ElTag: { template: '<span><slot /></span>' }
  }
}

const items: NurseExecutionItemVO[] = [
  {
    itemId: 'pending',
    encounterId: 'enc-001',
    categoryCode: 'SERVICE',
    categoryName: '处置',
    itemName: '静脉输液',
    quantity: 250,
    unitName: 'ml',
    status: 'PENDING',
    statusName: '待执行',
    executionCount: 0
  },
  {
    itemId: 'completed',
    encounterId: 'enc-001',
    categoryCode: 'SERVICE',
    categoryName: '治疗',
    itemName: '雾化治疗',
    status: 'COMPLETED',
    statusName: '已完成',
    executionCount: 1,
    executorName: '模拟护士',
    executedAt: '2026-07-29 09:00:00'
  }
]

describe('NurseExecutionPanel', () => {
  beforeEach(() => {
    clearSelectionSpy.mockClear()
  })

  it('将执行项目映射为公共医嘱表格行', () => {
    const wrapper = mount(NurseExecutionPanel, {
      props: { items, hasPatient: true },
      global
    })
    const rows = wrapper.findComponent(AdviceOrderTableStub).props('rows') as AdviceOrder[]

    expect(rows).toHaveLength(2)
    expect(rows[0]).toMatchObject({
      key: 'pending',
      name: '静脉输液',
      specification: '250 ml',
      status: 'PENDING'
    })
  })

  it('仅上报仍可执行的勾选项目', async () => {
    const wrapper = mount(NurseExecutionPanel, {
      props: { items, hasPatient: true },
      global
    })

    await wrapper.get('.select-all').trigger('click')

    expect(wrapper.emitted('selectionChange')?.[0]).toEqual([['pending']])
  })

  it('已执行项目提供取消入口并上报原始项目', async () => {
    const wrapper = mount(NurseExecutionPanel, {
      props: { items, hasPatient: true },
      global
    })
    const cancelButton = wrapper
      .findAll('button')
      .find((button) => button.text().includes('取消执行'))

    expect(cancelButton).toBeTruthy()
    await cancelButton?.trigger('click')
    expect(wrapper.emitted('cancelExecute')?.[0]).toEqual([items[1]])
  })

  it('向公共表格转发清空勾选', () => {
    const wrapper = mount(NurseExecutionPanel, {
      props: { items, hasPatient: true },
      global
    })

    ;(wrapper.vm as unknown as { clearSelection: () => void }).clearSelection()

    expect(clearSelectionSpy).toHaveBeenCalledOnce()
  })
})
