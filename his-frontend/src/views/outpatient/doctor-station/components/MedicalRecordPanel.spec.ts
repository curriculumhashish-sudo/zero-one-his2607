// @vitest-environment jsdom

import { defineComponent } from 'vue'
import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import MedicalRecordPanel from './MedicalRecordPanel.vue'

const ElButtonStub = defineComponent({
  name: 'ElButton',
  props: ['disabled', 'loading', 'type'],
  emits: ['click'],
  template: '<button :disabled="disabled" @click="$emit(\'click\')"><slot /></button>'
})

const global = {
  stubs: {
    EmptyState: true,
    RichTextEditorField: true,
    ElButton: ElButtonStub
  }
}

describe('MedicalRecordPanel', () => {
  it('在可编辑且病历已修改时，从病历区域发出保存事件', async () => {
    const wrapper = mount(MedicalRecordPanel, {
      props: {
        patientSelected: true,
        accepted: true,
        dirty: true
      },
      global
    })

    const saveButton = wrapper.get('button')
    expect(saveButton.text()).toBe('保存病历')
    expect(saveButton.attributes('disabled')).toBeUndefined()

    await saveButton.trigger('click')
    expect(wrapper.emitted('save')).toHaveLength(1)
  })

  it('内容未修改时禁用保存按钮，病历只读时不显示按钮', async () => {
    const wrapper = mount(MedicalRecordPanel, {
      props: {
        patientSelected: true,
        accepted: true,
        dirty: false
      },
      global
    })

    expect(wrapper.get('button').attributes('disabled')).toBeDefined()

    await wrapper.setProps({ accepted: false, readonly: true })
    expect(wrapper.find('button').exists()).toBe(false)
  })
})
