// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import PatientContextPanel from './PatientContextPanel.vue'

describe('PatientContextPanel', () => {
  it('未选择患者时显示选择提示', () => {
    const wrapper = mount(PatientContextPanel)

    expect(wrapper.text()).toContain('请选择患者')
  })

  it('选择待诊患者后提示先接诊，不提前展示患者信息', () => {
    const wrapper = mount(PatientContextPanel, {
      props: {
        patientSelected: true,
        patient: {
          id: 'encounter-waiting',
          name: '待诊患者',
          patientNo: 'MZ-WAITING'
        }
      }
    })

    expect(wrapper.text()).toContain('请先接诊患者')
    expect(wrapper.text()).not.toContain('待诊患者')
    expect(wrapper.text()).not.toContain('MZ-WAITING')
  })

  it('按参考卡片展示精简后的门诊就诊信息', () => {
    const wrapper = mount(PatientContextPanel, {
      props: {
        patientSelected: true,
        patient: {
          id: 'encounter-1',
          name: '全功能演示患者',
          gender: '男',
          age: 52,
          serviceType: '专家门诊',
          patientNo: 'MZ20260802000',
          paymentType: '自费',
          visitTime: '2026-08-02 22:50:43',
          departmentName: '心血管内科门诊'
        },
        context: {
          encounterId: 'encounter-1',
          patientId: 'patient-1',
          visitStatus: 'in-progress',
          visitStatusName: '在诊',
          attendingDoctorName: '李尔王',
          acceptedAt: '2026-08-02 22:50:43',
          phone: '138****0000'
        }
      }
    })

    for (const text of [
      '全功能演示患者',
      '在诊',
      '男 · 52岁 · 专家门诊',
      '病历号',
      'MZ20260802000',
      '性别',
      '费用性质',
      '自费',
      '就诊时间',
      '2026-08-02 22:50:43',
      '挂号科室',
      '心血管内科门诊',
      '电话号码',
      '138****0000'
    ]) {
      expect(wrapper.text()).toContain(text)
    }

    expect(wrapper.text()).not.toContain('接诊医生')
    expect(wrapper.text()).not.toContain('分诊主诉')
    expect(wrapper.text()).not.toContain('既往病史')
    expect(wrapper.text()).not.toContain('紧急联系人')
    expect(wrapper.text()).not.toContain('分诊备注')
  })
})
