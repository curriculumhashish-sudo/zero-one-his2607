// @vitest-environment jsdom

import { describe, expect, it } from 'vitest'
import { mount } from '@vue/test-utils'
import { defineComponent, h } from 'vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import RecordSidebar from '@/components/common/RecordSidebar.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import SideNavigator from '@/components/common/SideNavigator.vue'
import SidePanelShell from '@/components/common/SidePanelShell.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'

const TreeStub = defineComponent({
  name: 'ElTree',
  props: {
    data: { type: Array, default: () => [] }
  },
  emits: ['node-click'],
  setup(props, { emit, expose }) {
    expose({ filter: () => undefined, setCurrentKey: () => undefined })
    return () =>
      h(
        'div',
        props.data.map((node: any) =>
          h(
            'button',
            { class: 'tree-node-stub', onClick: () => emit('node-click', node) },
            node.label
          )
        )
      )
  }
})

const global = {
  stubs: {
    ElButton: { template: '<button><slot /></button>' },
    ElDescriptions: { template: '<div><slot /></div>' },
    ElDescriptionsItem: {
      props: ['label'],
      template: '<div><span>{{ label }}</span><slot /></div>'
    },
    ElEmpty: { template: '<div />' },
    ElDropdown: { template: '<div><slot /><slot name="dropdown" /></div>' },
    ElDropdownItem: {
      props: ['command'],
      emits: ['click'],
      template: '<button><slot /></button>'
    },
    ElDropdownMenu: { template: '<div><slot /></div>' },
    ElIcon: { template: '<span><slot /></span>' },
    ElInput: { template: '<input />' },
    ElTag: { template: '<span><slot /></span>' },
    ElTree: TreeStub
  },
  directives: {
    loading: () => undefined
  }
}

describe('C06-C08 公共组件', () => {
  it('ActionToolbar 点击按钮时返回动作配置', async () => {
    const actions = [
      { key: 'create', label: '新增', type: 'primary' as const },
      { key: 'refresh', label: '刷新' }
    ]
    const wrapper = mount(ActionToolbar, {
      props: { actions, selectionCount: 2 },
      global
    })

    await wrapper.findAll('button')[0]!.trigger('click')

    expect(wrapper.text()).toContain('已选 2 项')
    expect(wrapper.emitted('action')?.[0]).toEqual([actions[0]])
    expect(wrapper.emitted('click')?.[0]).toEqual(['create', actions[0]])
  })

  it('RowActionGroup 点击行操作时返回当前行', async () => {
    const row = { id: 1, name: '张明' }
    const actions = [
      { key: 'view', label: '查看', type: 'primary' as const },
      { key: 'edit', label: '编辑' }
    ]
    const wrapper = mount(RowActionGroup, {
      props: { actions, row },
      global
    })

    await wrapper.findAll('button')[1]!.trigger('click')

    expect(wrapper.emitted('action')?.[0]).toEqual([actions[1], row])
    expect(wrapper.emitted('click')?.[0]).toEqual(['edit', row, actions[1]])
  })

  it('StatusDisplay 根据状态值匹配文案和说明', () => {
    const wrapper = mount(StatusDisplay, {
      props: {
        value: 'done',
        showDescription: true,
        options: [{ value: 'done', label: '已完成', type: 'success', description: '诊疗闭环' }]
      },
      global
    })

    expect(wrapper.text()).toContain('已完成')
    expect(wrapper.text()).toContain('诊疗闭环')
  })
})

describe('C12-C14 公共组件', () => {
  it('SidePanelShell 折叠时返回受控事件并隐藏主体区域', async () => {
    const wrapper = mount(SidePanelShell, {
      props: { title: '侧栏', collapsible: true, collapsed: false },
      slots: {
        search: '<input />',
        default: '<div class="shell-body-content">主体内容</div>',
        footer: '<div>底部内容</div>'
      },
      global
    })

    await wrapper.find('button').trigger('click')

    expect(wrapper.text()).toContain('主体内容')
    expect(wrapper.emitted('update:collapsed')?.[0]).toEqual([true])
    await wrapper.setProps({ collapsed: true })
    expect(wrapper.text()).not.toContain('主体内容')
  })

  it('DetailSummary 展示摘要值和空值占位', () => {
    const wrapper = mount(DetailSummary, {
      props: {
        items: [
          { key: 'name', label: '姓名', value: '张明' },
          { key: 'allergy', label: '过敏史', value: null }
        ],
        column: 2
      },
      global
    })

    expect(wrapper.text()).toContain('张明')
    expect(wrapper.text()).toContain('过敏史')
    expect(wrapper.text()).toContain('—')
  })

  it('RecordSidebar 以受控事件返回选中记录', async () => {
    const records = [
      { key: 1, title: '记录一' },
      { key: 2, title: '记录二' }
    ]
    const wrapper = mount(RecordSidebar, {
      props: { items: records, modelValue: 1 },
      global
    })

    await wrapper.findAll('.record-sidebar__item')[1]!.trigger('click')

    expect(wrapper.emitted('update:modelValue')?.[0]).toEqual([2])
    expect(wrapper.emitted('select')?.[0]).toEqual([records[1]])
  })

  it('SideNavigator 点击节点时返回节点键和值', async () => {
    const nodes = [
      { key: 'outpatient', label: '门诊病历' },
      { key: 'inpatient', label: '住院记录' }
    ]
    const wrapper = mount(SideNavigator, {
      props: { items: nodes, modelValue: 'outpatient', searchable: false },
      global,
      attachTo: document.body
    })

    await wrapper.findAll('.tree-node-stub')[1]!.trigger('click')

    expect(wrapper.emitted('update:modelValue')?.[0]).toEqual(['inpatient'])
    expect(wrapper.emitted('select')?.[0]).toEqual([nodes[1]])
    wrapper.unmount()
  })
})
