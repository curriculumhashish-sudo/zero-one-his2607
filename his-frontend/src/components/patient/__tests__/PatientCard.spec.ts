import { describe, expect, it } from 'vitest'
import { mount } from '@vue/test-utils'

import PatientCard from '../PatientCard.vue'
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
  queueNumber: 'A012',
  visitTime: '2026-07-20 09:30',
  departmentName: '心内科',
  status: '候诊中',
  statusTone: 'warning',
  paymentType: '医保',
  riskHint: '青霉素过敏'
}

describe('PatientCard', () => {
  it('展示患者摘要', () => {
    const wrapper = mount(PatientCard, {
      props: { patient }
    })

    expect(wrapper.text()).toContain('张三')
    expect(wrapper.text()).toContain('男')
    expect(wrapper.text()).toContain('38岁')
    expect(wrapper.text()).toContain('普通门诊')
    expect(wrapper.text()).toContain('2026-07-20 09:30')
    expect(wrapper.text()).toContain('心内科')
    expect(wrapper.text()).toContain('青霉素过敏')
    expect(wrapper.text()).toContain('排队号 A012')
    expect(wrapper.text()).toContain('医保')
    expect(wrapper.text()).toContain('候诊中')
  })

  it('点击卡片发出 select 并返回当前患者', async () => {
    const wrapper = mount(PatientCard, {
      props: { patient }
    })

    await wrapper.get('.patient-card').trigger('click')

    expect(wrapper.emitted('select')).toEqual([[patient]])
  })

  it('按 Enter 或 Space 发出 select', async () => {
    const wrapper = mount(PatientCard, {
      props: { patient }
    })
    const card = wrapper.get('.patient-card')

    await card.trigger('keydown', { key: 'Enter' })
    await card.trigger('keydown', { key: ' ' })
    await card.trigger('keydown', { key: 'Escape' })

    expect(wrapper.emitted('select')).toEqual([[patient], [patient]])
  })

  it('点击查看只发出 view，不触发 select', async () => {
    const wrapper = mount(PatientCard, {
      props: { patient }
    })

    await wrapper.get('.patient-card__view').trigger('click')

    expect(wrapper.emitted('view')).toEqual([[patient]])
    expect(wrapper.emitted('select')).toBeUndefined()
  })

  it('selected 为 true 时展示选中状态', () => {
    const wrapper = mount(PatientCard, {
      props: {
        patient,
        selected: true
      }
    })

    const card = wrapper.get('.patient-card')
    expect(card.classes()).toContain('patient-card--selected')
    expect(card.attributes('aria-pressed')).toBe('true')
  })

  it('缺少可选信息时使用降级文案并隐藏空区域', () => {
    const wrapper = mount(PatientCard, {
      props: {
        patient: {
          id: 'empty-patient',
          name: null,
          age: null
        }
      }
    })

    expect(wrapper.text()).toContain('无档案登记')
    expect(wrapper.text()).toContain('- · - · -')
    expect(wrapper.find('.patient-card__row').exists()).toBe(false)
    expect(wrapper.find('.patient-card__risk').exists()).toBe(false)
    expect(wrapper.find('.el-tag').exists()).toBe(false)
  })

  it('无排队号时按就诊号、患者号顺序降级展示', async () => {
    const wrapper = mount(PatientCard, {
      props: {
        patient: {
          id: 'number-fallback',
          visitNo: 'OP-FALLBACK',
          patientNo: 'MR-FALLBACK'
        }
      }
    })

    expect(wrapper.get('.patient-card__number').text()).toBe('OP-FALLBACK')

    await wrapper.setProps({
      patient: {
        id: 'number-fallback',
        patientNo: 'MR-FALLBACK'
      }
    })

    expect(wrapper.get('.patient-card__number').text()).toBe('MR-FALLBACK')
  })
})
