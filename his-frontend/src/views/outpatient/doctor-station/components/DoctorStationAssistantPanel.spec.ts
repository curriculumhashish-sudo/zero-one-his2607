// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import DoctorStationAssistantPanel from './DoctorStationAssistantPanel.vue'

describe('DoctorStationAssistantPanel', () => {
  it('只保留历史病历和就诊信息，不再展示病历模板入口', () => {
    const wrapper = mount(DoctorStationAssistantPanel, {
      global: {
        stubs: {
          ElTabs: { template: '<div><slot /></div>' },
          ElTabPane: {
            props: ['label', 'name'],
            template: '<section :data-pane="name">{{ label }}<slot /></section>'
          },
          MedicalRecordHistoryPanel: true,
          PatientContextPanel: true
        }
      }
    })

    expect(wrapper.text()).toContain('历史病历')
    expect(wrapper.text()).toContain('就诊信息')
    expect(wrapper.text()).not.toContain('病历模板')
    wrapper.unmount()
  })
})
