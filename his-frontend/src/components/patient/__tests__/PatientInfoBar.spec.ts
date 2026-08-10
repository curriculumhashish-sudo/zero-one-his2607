import { h } from 'vue'
import { describe, expect, it } from 'vitest'
import { mount } from '@vue/test-utils'

import PatientInfoBar from '../PatientInfoBar.vue'
import type { PatientSummary } from '../type'

const patient: PatientSummary = {
  id: 'visit-1001',
  name: '张三',
  gender: '男',
  age: 38,
  patientType: 'outpatient',
  serviceType: '普通门诊',
  patientNo: 'MR20260001',
  visitNo: 'OP20260001',
  visitTime: '2026-07-20 09:30',
  departmentName: '心内科',
  status: '已接诊',
  statusTone: 'success',
  paymentType: '医保',
  priority: '重点关注'
}

describe('PatientInfoBar', () => {
  it('展示患者主信息和标签', () => {
    const wrapper = mount(PatientInfoBar, {
      props: { patient }
    })

    expect(wrapper.text()).toContain('张三')
    expect(wrapper.text()).toContain('男 · 38岁')
    expect(wrapper.text()).toContain('普通门诊')
    expect(wrapper.text()).toContain('MR20260001')
    expect(wrapper.text()).toContain('OP20260001')
    expect(wrapper.text()).toContain('2026-07-20 09:30')
    expect(wrapper.text()).toContain('心内科')
    expect(wrapper.text()).toContain('已接诊')
    expect(wrapper.text()).toContain('医保')
  })

  it('门诊挂号摘要模式只展示参考界面要求的字段', () => {
    const wrapper = mount(PatientInfoBar, {
      props: {
        patient,
        displayMode: 'outpatient-registration',
        phone: '138****0000',
        doctorName: 'admin'
      }
    })

    expect(wrapper.text()).toContain('患者：')
    expect(wrapper.text()).toContain('张三')
    expect(wrapper.text()).toContain('男')
    expect(wrapper.text()).toContain('38岁')
    expect(wrapper.text()).toContain('电话：')
    expect(wrapper.text()).toContain('138****0000')
    expect(wrapper.text()).toContain('病历号：')
    expect(wrapper.text()).toContain('MR20260001')
    expect(wrapper.text()).toContain('普通门诊')
    expect(wrapper.text()).toContain('挂号时间：')
    expect(wrapper.text()).toContain('07-20 09:30')
    expect(wrapper.text()).toContain('医生：')
    expect(wrapper.text()).toContain('admin')

    expect(wrapper.text()).not.toContain('OP20260001')
    expect(wrapper.text()).not.toContain('心内科')
    expect(wrapper.text()).not.toContain('已接诊')
    expect(wrapper.text()).not.toContain('医保')
    expect(wrapper.text()).not.toContain('重点关注')
  })

  it('住院摘要模式只展示参考界面要求的字段', () => {
    const wrapper = mount(PatientInfoBar, {
      props: {
        patient: {
          ...patient,
          patientType: 'inpatient',
          serviceType: '心内一病区 · 08床',
          patientNo: 'P10001',
          visitNo: 'ZY20260701001',
          visitTime: '2026-07-21 09:20',
          departmentName: '心血管内科',
          status: '在院',
          paymentType: '职工医保'
        },
        displayMode: 'inpatient-registration',
        doctorName: '李尔王'
      }
    })

    expect(wrapper.text()).toContain('患者：')
    expect(wrapper.text()).toContain('张三')
    expect(wrapper.text()).toContain('男')
    expect(wrapper.text()).toContain('38岁')
    expect(wrapper.text()).toContain('病历号：')
    expect(wrapper.text()).toContain('ZY20260701001')
    expect(wrapper.text()).toContain('心内一病区 · 08床')
    expect(wrapper.text()).toContain('挂号时间：')
    expect(wrapper.text()).toContain('07-21 09:20')
    expect(wrapper.text()).toContain('医生：')
    expect(wrapper.text()).toContain('李尔王')

    expect(wrapper.text()).not.toContain('电话：')
    expect(wrapper.text()).not.toContain('P10001')
    expect(wrapper.text()).not.toContain('心血管内科')
    expect(wrapper.text()).not.toContain('在院')
    expect(wrapper.text()).not.toContain('职工医保')
    expect(wrapper.text()).not.toContain('重点关注')
  })

  it('展示优先级标签', () => {
    const wrapper = mount(PatientInfoBar, {
      props: { patient }
    })

    const risks = wrapper.get('.patient-info-bar__risks')
    expect(risks.text()).toContain('重点关注')
  })

  it('未传患者时显示明确占位', () => {
    const wrapper = mount(PatientInfoBar)

    expect(wrapper.find('.patient-info-bar__empty').exists()).toBe(true)
    expect(wrapper.text()).toContain('当前未选择患者')
    expect(wrapper.text()).toContain('请先从患者列表中选择一位患者')
    expect(wrapper.find('.patient-info-bar__main').exists()).toBe(false)
  })

  it('patient 为 null 时同样显示占位', () => {
    const wrapper = mount(PatientInfoBar, {
      props: { patient: null }
    })

    expect(wrapper.find('.patient-info-bar__empty').exists()).toBe(true)
  })

  it('actions 插槽获得当前患者', () => {
    const wrapper = mount(PatientInfoBar, {
      props: { patient },
      slots: {
        actions: ({ patient: slotPatient }: { patient: PatientSummary }) =>
          h(
            'button',
            {
              class: 'test-action',
              'data-patient-id': slotPatient.id
            },
            `操作${slotPatient.name}`
          )
      }
    })

    const action = wrapper.get('.test-action')
    expect(action.text()).toBe('操作张三')
    expect(action.attributes('data-patient-id')).toBe('visit-1001')
  })

  it('未提供 actions 插槽时不渲染操作区', () => {
    const wrapper = mount(PatientInfoBar, {
      props: { patient }
    })

    expect(wrapper.find('.patient-info-bar__actions').exists()).toBe(false)
  })

  it('缺少可选详情和优先级时不渲染对应区域', () => {
    const wrapper = mount(PatientInfoBar, {
      props: {
        patient: {
          id: 'minimal-patient',
          name: null,
          age: null
        }
      }
    })

    expect(wrapper.text()).toContain('无档案登记')
    expect(wrapper.find('.patient-info-bar__details').exists()).toBe(false)
    expect(wrapper.find('.patient-info-bar__risks').exists()).toBe(false)
    expect(wrapper.find('.patient-info-bar__empty').exists()).toBe(false)
  })
})
