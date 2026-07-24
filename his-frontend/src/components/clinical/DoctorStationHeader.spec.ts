// @vitest-environment jsdom

import { defineComponent } from 'vue'
import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import DoctorStationHeader from './DoctorStationHeader.vue'
import type { ActionToolbarItem } from '@/components/common/type'

/**
 * 用轻量替身隔离 Element Plus 和 ActionToolbar。
 * 这里测试的是公共 Header 的复用契约，而不是重复测试子组件的内部实现。
 */
const ElTabsStub = defineComponent({
  name: 'ElTabs',
  props: ['modelValue'],
  emits: ['update:modelValue'],
  template: '<div class="tabs-stub"><slot /></div>'
})

const ElTabPaneStub = defineComponent({
  name: 'ElTabPane',
  props: ['name', 'label', 'disabled'],
  template: '<span class="tab-pane-stub">{{ label }}</span>'
})

const ActionToolbarStub = defineComponent({
  name: 'ActionToolbar',
  props: ['actions', 'maxVisible', 'compact', 'loading', 'disabled'],
  emits: ['action'],
  template: '<div class="toolbar-stub" />'
})

const global = {
  stubs: {
    ElTabs: ElTabsStub,
    ElTabPane: ElTabPaneStub,
    ActionToolbar: ActionToolbarStub
  }
}

describe('DoctorStationHeader', () => {
  it('根据场景配置渲染页签，并转发新的页签 key', () => {
    const wrapper = mount(DoctorStationHeader, {
      props: {
        activeTab: 'record',
        tabs: [
          { key: 'record', label: '病历' },
          { key: 'long-term-orders', label: '长期医嘱' }
        ]
      },
      global
    })

    expect(wrapper.findAll('.tab-pane-stub').map((item) => item.text())).toEqual([
      '病历',
      '长期医嘱'
    ])

    wrapper.getComponent(ElTabsStub).vm.$emit('update:modelValue', 'long-term-orders')
    expect(wrapper.emitted('tabChange')?.[0]).toEqual(['long-term-orders'])
  })

  it('把场景按钮交给工具栏，并原样转发被点击的按钮对象', () => {
    const actions: ActionToolbarItem[] = [
      { key: 'submit', label: '提交', type: 'primary' },
      { key: 'stop', label: '停嘱', disabled: true }
    ]
    const wrapper = mount(DoctorStationHeader, {
      props: {
        activeTab: 'orders',
        tabs: [{ key: 'orders', label: '医嘱' }],
        actions,
        maxVisible: 2
      },
      global
    })

    const toolbar = wrapper.getComponent(ActionToolbarStub)
    expect(toolbar.props('actions')).toEqual(actions)
    expect(toolbar.props('maxVisible')).toBe(2)

    toolbar.vm.$emit('action', actions[0])
    expect(wrapper.emitted('action')?.[0]).toEqual([actions[0]])
  })
})
