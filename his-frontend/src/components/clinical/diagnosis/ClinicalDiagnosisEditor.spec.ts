// @vitest-environment jsdom

import { flushPromises, mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import ElementPlus from 'element-plus'

import ClinicalDiagnosisEditor from './ClinicalDiagnosisEditor.vue'
import type { ClinicalDiagnosisItem } from './type'

// Element Plus 表格在 jsdom 中需要 ResizeObserver。
class ResizeObserverStub {
  observe() {}
  unobserve() {}
  disconnect() {}
}
globalThis.ResizeObserver = ResizeObserverStub as unknown as typeof ResizeObserver

function mountEditor(modelValue: ClinicalDiagnosisItem[] = []) {
  return mount(ClinicalDiagnosisEditor, {
    props: {
      modelValue,
      patientSelected: true,
      accepted: true,
      loadDictionary: async () => ({ rows: [], total: 0 })
    },
    global: {
      plugins: [ElementPlus],
      stubs: { EmptyState: true, DiagnosisSelectorDialog: true }
    }
  })
}

describe('ClinicalDiagnosisEditor', () => {
  it('不展示快捷诊断，并可打开历史诊断', async () => {
    const wrapper = mountEditor()
    const buttons = wrapper.findAll('button')
    expect(wrapper.text()).not.toContain('快捷诊断')

    await buttons.find((button) => button.text().trim() === '历史诊断')!.trigger('click')
    expect(wrapper.emitted('openHistory')).toHaveLength(1)
    wrapper.unmount()
  })

  it('allowSetPrimary 为 false 时只读展示主诊断标签，不渲染“设为主诊断”按钮', async () => {
    const wrapper = mount(ClinicalDiagnosisEditor, {
      props: {
        modelValue: [
          {
            key: 'saved-1',
            recordId: 'diag-1',
            catalogId: 'catalog-1',
            code: 'I10',
            name: '原发性高血压',
            typeName: '西医诊断',
            primary: true
          },
          {
            key: 'saved-2',
            recordId: 'diag-2',
            catalogId: 'catalog-2',
            code: 'R00.2',
            name: '心悸',
            typeName: '症状诊断',
            primary: false
          }
        ],
        patientSelected: true,
        accepted: true,
        allowSetPrimary: false,
        loadDictionary: async () => ({ rows: [], total: 0 })
      },
      global: {
        plugins: [ElementPlus],
        stubs: { EmptyState: true, DiagnosisSelectorDialog: true }
      }
    })

    await flushPromises()
    expect(wrapper.text()).toContain('主诊断')
    expect(wrapper.text()).toContain('原发性高血压')
    expect(wrapper.text()).not.toContain('设为主诊断')
    wrapper.unmount()
  })

  it('allowSetPrimary 默认为 true 时可编辑行渲染“设为主诊断”按钮', async () => {
    const wrapper = mountEditor([
      {
        key: 'saved-1',
        recordId: 'diag-1',
        catalogId: 'catalog-1',
        code: 'I10',
        name: '原发性高血压',
        typeName: '西医诊断',
        primary: false
      }
    ])

    await flushPromises()
    expect(wrapper.text()).toContain('设为主诊断')
    wrapper.unmount()
  })
})
