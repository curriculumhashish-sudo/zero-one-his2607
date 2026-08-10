// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import PrescriptionSheetView from './PrescriptionSheetView.vue'

describe('PrescriptionSheetView', () => {
  it('展示患者摘要、Rp 正文、金额和签名信息', () => {
    const wrapper = mount(PrescriptionSheetView, {
      props: {
        data: {
          patientName: '王志强',
          age: 67,
          gender: '男',
          patientId: 'OP-001',
          prescriptionNo: 'RX-001',
          department: '心血管内科',
          doctor: '李尔王',
          diagnosis: '高血压',
          totalPrice: 53.8,
          pharmacist: '赵药师',
          dispenser: '钱药师',
          priceChecker: '系统划价',
          items: '1. 苯磺酸氨氯地平片\n用法：每日一次'
        }
      }
    })

    expect(wrapper.text()).toContain('门诊处方笺')
    expect(wrapper.text()).toContain('王志强')
    expect(wrapper.text()).toContain('RX-001')
    expect(wrapper.text()).toContain('苯磺酸氨氯地平片')
    expect(wrapper.text()).toContain('¥53.80')
    expect(wrapper.text()).toContain('调配：赵药师')
  })
})
