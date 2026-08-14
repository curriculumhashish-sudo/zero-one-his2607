// @vitest-environment jsdom

import { defineComponent } from 'vue'
import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import ClinicalMedicalRecordEditor from './ClinicalMedicalRecordEditor.vue'

const EmptyStateStub = defineComponent({
  name: 'EmptyState',
  props: ['title', 'description', 'actionText'],
  emits: ['action'],
  template:
    '<div class="empty-state-stub"><span>{{ title }}</span><button @click="$emit(\'action\')">action</button></div>'
})

const RichTextEditorFieldStub = defineComponent({
  name: 'RichTextEditorField',
  props: ['modelValue', 'readonly', 'minHeight', 'placeholder'],
  emits: ['update:modelValue', 'change'],
  template: '<div class="rich-text-stub" />'
})

const ElSelectStub = defineComponent({
  name: 'ElSelect',
  props: ['modelValue'],
  emits: ['update:modelValue'],
  template: '<div class="select-stub"><slot /></div>'
})

const ElInputStub = defineComponent({
  name: 'ElInput',
  props: ['modelValue', 'readonly', 'disabled'],
  emits: ['update:modelValue'],
  template: '<div class="input-stub" />'
})

const global = {
  stubs: {
    EmptyState: EmptyStateStub,
    RichTextEditorField: RichTextEditorFieldStub,
    ElSkeleton: true,
    ElTag: defineComponent({
      name: 'ElTag',
      template: '<span class="tag-stub"><slot /></span>'
    }),
    ElInput: ElInputStub,
    ElSelect: ElSelectStub,
    ElOption: true
  }
}

describe('ClinicalMedicalRecordEditor', () => {
  it('未选择患者时只展示调用方传入的空状态说明', () => {
    const wrapper = mount(ClinicalMedicalRecordEditor, {
      props: {
        patientSelected: false,
        patientEmptyDescription: '请选择住院患者'
      },
      global
    })

    const emptyState = wrapper.getComponent(EmptyStateStub)
    expect(emptyState.props('title')).toBe('请选择患者')
    expect(emptyState.props('description')).toBe('请选择住院患者')
    expect(wrapper.findComponent(RichTextEditorFieldStub).exists()).toBe(false)
  })

  it('统一转发文书元数据和正文编辑事件', () => {
    const wrapper = mount(ClinicalMedicalRecordEditor, {
      props: {
        patientSelected: true,
        initialized: true,
        showDocumentMeta: true,
        heading: '住院病历',
        statusText: '草稿已保存',
        documentTypes: [{ value: 'DAILY', label: '日常病程记录' }],
        documentTypeCode: 'DAILY',
        title: '日常病程记录',
        content: '<p>病情稳定</p>'
      },
      global
    })

    wrapper.getComponent(ElSelectStub).vm.$emit('update:modelValue', 'DAILY')
    wrapper.getComponent(ElInputStub).vm.$emit('update:modelValue', '新的标题')
    wrapper.getComponent(RichTextEditorFieldStub).vm.$emit('update:modelValue', '<p>病情改善</p>')

    expect(wrapper.emitted('update:documentTypeCode')?.[0]).toEqual(['DAILY'])
    expect(wrapper.emitted('documentTypeChange')?.[0]).toEqual(['DAILY'])
    expect(wrapper.emitted('update:title')?.[0]).toEqual(['新的标题'])
    expect(wrapper.emitted('update:content')?.[0]).toEqual(['<p>病情改善</p>'])
    expect(wrapper.text()).toContain('住院病历')
    expect(wrapper.text()).toContain('草稿已保存')
  })

  it('错误状态的重试动作由业务适配层继续处理', async () => {
    const wrapper = mount(ClinicalMedicalRecordEditor, {
      props: {
        patientSelected: true,
        error: '网络异常'
      },
      global
    })

    await wrapper.getComponent(EmptyStateStub).get('button').trigger('click')

    expect(wrapper.emitted('retry')).toHaveLength(1)
  })
})
