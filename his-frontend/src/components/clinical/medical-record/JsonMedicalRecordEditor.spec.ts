// @vitest-environment jsdom

import { nextTick } from 'vue'
import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import JsonMedicalRecordEditor from './JsonMedicalRecordEditor.vue'
import { createEmptyMedicalRecordDocument, isMedicalRecordJsonDocument } from './type'

const global = {
  stubs: {
    ElButton: {
      props: ['disabled'],
      template: '<button :disabled="disabled" @click="$emit(\'click\')"><slot /></button>'
    },
    ElButtonGroup: {
      template: '<div><slot /></div>'
    }
  }
}

describe('JsonMedicalRecordEditor', () => {
  it('使用 JSON 文档对象初始化编辑器', async () => {
    const document = {
      type: 'doc' as const,
      content: [
        {
          type: 'paragraph',
          content: [{ type: 'text', text: '患者主诉头痛两天' }]
        }
      ]
    }

    const wrapper = mount(JsonMedicalRecordEditor, {
      props: { modelValue: document },
      global
    })

    await nextTick()
    const component = wrapper.vm as unknown as {
      getJSON: () => unknown
    }

    expect(JSON.stringify(component.getJSON())).toContain('患者主诉头痛两天')
    expect(wrapper.text()).toContain('8 字')
    expect(wrapper.text()).toContain('JSON 文档')
    expect(wrapper.text()).not.toContain('<p>')
  })

  it('编辑后发送 JSON 对象而不是 HTML 字符串', async () => {
    const wrapper = mount(JsonMedicalRecordEditor, { global })
    const component = wrapper.vm as unknown as {
      editor: { commands: { insertContent: (value: string) => void } }
    }

    component.editor.commands.insertContent('现病史内容')
    await nextTick()

    const value = wrapper.emitted('update:modelValue')?.at(-1)?.[0]
    expect(isMedicalRecordJsonDocument(value)).toBe(true)
    expect(typeof value).toBe('object')
    expect(JSON.stringify(value)).toContain('现病史内容')
  })

  it('接收父组件回填的 JSON 且不产生额外编辑事件', async () => {
    const wrapper = mount(JsonMedicalRecordEditor, { global })
    const nextDocument = {
      type: 'doc' as const,
      content: [
        {
          type: 'paragraph',
          content: [{ type: 'text', text: '复诊病历' }]
        }
      ]
    }

    await wrapper.setProps({ modelValue: nextDocument })
    const component = wrapper.vm as unknown as { getJSON: () => unknown }

    expect(JSON.stringify(component.getJSON())).toContain('复诊病历')
    expect(wrapper.emitted('update:modelValue')).toBeUndefined()
  })

  it('只读模式隐藏工具栏并禁用编辑', () => {
    const wrapper = mount(JsonMedicalRecordEditor, {
      props: {
        readonly: true,
        modelValue: createEmptyMedicalRecordDocument()
      },
      global
    })

    expect(wrapper.find('[role="toolbar"]').exists()).toBe(false)
    expect(wrapper.classes()).toContain('json-medical-record-editor--readonly')
  })
})

describe('JSON 病历文档工具', () => {
  it('创建有效的空 JSON 文档并拒绝 HTML 字符串', () => {
    expect(createEmptyMedicalRecordDocument()).toEqual({
      type: 'doc',
      content: [{ type: 'paragraph' }]
    })
    expect(isMedicalRecordJsonDocument('<p>病历</p>')).toBe(false)
  })
})
