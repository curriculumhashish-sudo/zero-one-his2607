// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { defineComponent, h } from 'vue'
import { describe, expect, it, vi } from 'vitest'
import AdviceOrderTable from './AdviceOrderTable.vue'
import type { AdviceOrder } from './type'

/**
 * ElTable 及其列在真实实现里通过 provide/inject 协作，jsdom 下难以直接挂载。
 * 这里用轻量 stub 复刻「按列的 #default 作用域插槽逐行渲染」的行为，
 * 从而覆盖 AdviceOrderTable 自身的列渲染、当前行高亮与行操作转发逻辑。
 */
const ColumnStub = defineComponent({
  name: 'ElTableColumn',
  props: {
    prop: { type: String, default: '' },
    label: { type: String, default: '' }
  },
  setup(props, { slots }) {
    const rows =
      (globalThis as unknown as { __ADVICE_ROWS__?: AdviceOrder[] }).__ADVICE_ROWS__ ?? []
    return () =>
      h(
        'div',
        { class: 'col', 'data-prop': props.prop },
        rows.map((row) =>
          slots.default
            ? slots.default({ row })
            : h('span', String(row[props.prop as keyof AdviceOrder] ?? ''))
        )
      )
  }
})

const clearSelectionSpy = vi.fn()

const TableStub = defineComponent({
  name: 'ElTable',
  props: {
    data: { type: Array, default: () => [] },
    rowClassName: { type: Function, default: undefined }
  },
  setup(props, { expose, slots }) {
    ;(globalThis as unknown as { __ADVICE_ROWS__?: AdviceOrder[] }).__ADVICE_ROWS__ =
      props.data as AdviceOrder[]
    expose({ clearSelection: clearSelectionSpy })
    return () =>
      h('div', { class: 'table' }, [
        ...(props.data as AdviceOrder[]).map((row) =>
          h('div', {
            class: ['row', props.rowClassName ? props.rowClassName({ row }) : ''].join(' ')
          })
        ),
        slots.default?.()
      ])
  }
})

const global = {
  stubs: {
    ElTable: TableStub,
    ElTableColumn: ColumnStub,
    ElTag: { template: '<span class="tag"><slot /></span>' }
  },
  directives: { loading: () => undefined }
}

const rows: AdviceOrder[] = [
  { key: 'o1', name: '阿莫西林', dose: 1, unit: '粒', status: 'pending', group: '处方A' },
  { key: 'o2', name: '血常规', status: 'done', group: '长期' }
]

describe('C54 AdviceOrderTable', () => {
  it('renders generic orders through the display cell', () => {
    const wrapper = mount(AdviceOrderTable, { props: { rows }, global })
    expect(wrapper.text()).toContain('阿莫西林')
    expect(wrapper.text()).toContain('血常规')
  })

  it('marks the controlled current row', () => {
    const wrapper = mount(AdviceOrderTable, { props: { rows, currentRowKey: 'o2' }, global })
    const current = wrapper.findAll('.row').filter((r) => r.classes().includes('is-current'))
    expect(current).toHaveLength(1)
  })

  it('forwards row actions with row and action name', async () => {
    const wrapper = mount(AdviceOrderTable, {
      props: { rows },
      slots: { actions: '<button class="stop" @click="action(\'stop\')">停止</button>' },
      global
    })
    await wrapper.get('.stop').trigger('click')
    expect(wrapper.emitted('rowAction')?.[0]?.[0]).toMatchObject({ action: 'stop', row: rows[0] })
  })

  it('formats empty cell values as dash via custom column', () => {
    const wrapper = mount(AdviceOrderTable, {
      props: {
        rows,
        columns: [
          { prop: 'name', label: '名称' },
          { prop: 'remark', label: '备注' }
        ]
      },
      global
    })
    // 两行均无 remark，应渲染占位符
    expect(wrapper.text()).toContain('-')
  })

  it('blocks row selection when readonly or disabledRow matches', () => {
    // 高难：勾选可用性由 readonly 与 disabledRow 共同决定，组件不写死业务规则。
    const readonly = mount(AdviceOrderTable, {
      props: { rows, selectable: true, readonly: true },
      global
    })
    // 只读时任何行都不可勾选
    expect(
      (readonly.vm as unknown as { canSelectRow: (r: AdviceOrder) => boolean }).canSelectRow(
        rows[0]!
      )
    ).toBe(false)

    const perRow = mount(AdviceOrderTable, {
      props: {
        rows,
        selectable: true,
        // 已完成的行禁止勾选，规则由页面注入
        disabledRow: (row: AdviceOrder) => row.status === 'done'
      },
      global
    })
    const canSelect = (perRow.vm as unknown as { canSelectRow: (r: AdviceOrder) => boolean })
      .canSelectRow
    expect(canSelect(rows[0]!)).toBe(true) // pending 可选
    expect(canSelect(rows[1]!)).toBe(false) // done 不可选
  })

  it('exposes clearSelection from the underlying Element Plus table', () => {
    clearSelectionSpy.mockClear()
    const wrapper = mount(AdviceOrderTable, { props: { rows, selectable: true }, global })

    ;(wrapper.vm as unknown as { clearSelection: () => void }).clearSelection()

    expect(clearSelectionSpy).toHaveBeenCalledOnce()
  })

  it('keeps currentRowKey controlled under rapid repeated current changes', async () => {
    // 高难 / 竞态：连续快速切换当前行，组件不得内部改写受控值，
    // 只能逐次上报 currentChange，最终值由页面决定。
    const wrapper = mount(AdviceOrderTable, {
      props: { rows, currentRowKey: 'o1' },
      global
    })
    const vm = wrapper.vm as unknown as { onCurrentChange: (row: AdviceOrder | null) => void }
    // 模拟表格在同一 tick 内连续触发（用户快速点选或数据刷新）
    vm.onCurrentChange(rows[1]!)
    vm.onCurrentChange(rows[0]!)
    vm.onCurrentChange(null)
    await wrapper.vm.$nextTick()

    const emitted = wrapper.emitted('currentChange')
    expect(emitted).toHaveLength(3)
    expect(emitted?.[0]).toEqual([rows[1]])
    expect(emitted?.[1]).toEqual([rows[0]])
    // 组件把 null 归一为 undefined 上报，而非自行清空受控值
    expect(emitted?.[2]).toEqual([undefined])
    // 受控值始终保持页面传入的原值，未被组件内部篡改
    expect(wrapper.props('currentRowKey')).toBe('o1')
  })
})
