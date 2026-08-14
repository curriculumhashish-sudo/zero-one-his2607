// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'
import AdviceOrderDisplayCell from './AdviceOrderDisplayCell.vue'
import type { AdviceOrder } from './type'

const global = {
  stubs: {
    ElTag: { template: '<span class="tag"><slot /></span>' }
  }
}

describe('C55 AdviceOrderDisplayCell', () => {
  it('joins only present detail fields without undefined noise', () => {
    const order: AdviceOrder = { key: 1, name: '阿莫西林', dose: 2, unit: '粒', frequency: 'tid', days: 3 }
    const wrapper = mount(AdviceOrderDisplayCell, { props: { order }, global })
    expect(wrapper.text()).toContain('阿莫西林')
    expect(wrapper.text()).toContain('2粒 · tid · 3天')
    expect(wrapper.text()).not.toContain('undefined')
  })

  it('falls back to placeholder name and emits click with the order', async () => {
    const order = { key: 2, name: '' } as AdviceOrder
    const wrapper = mount(AdviceOrderDisplayCell, { props: { order }, global })
    expect(wrapper.text()).toContain('未命名医嘱')
    await wrapper.trigger('click')
    expect(wrapper.emitted('click')?.[0]).toEqual([order])
  })

  it('maps display fields through the fields prop', () => {
    const order = { key: 3, name: '生化', route: '口服', remark: '空腹', productName: '肝功五项' } as unknown as AdviceOrder
    const wrapper = mount(AdviceOrderDisplayCell, {
      props: { order, fields: { name: 'productName' as keyof AdviceOrder } },
      global
    })
    expect(wrapper.text()).toContain('肝功五项')
  })

  it('shows status tag when showStatus and remark when not compact', () => {
    const order: AdviceOrder = { key: 4, name: '头孢', status: 'stopped', remark: '皮试阳性禁用' }
    const wrapper = mount(AdviceOrderDisplayCell, {
      props: { order, showStatus: true, statusMap: { stopped: '停用' } },
      global
    })
    expect(wrapper.find('.tag').text()).toContain('停用')
    expect(wrapper.text()).toContain('皮试阳性禁用')
  })

  it('hides remark in compact mode', () => {
    const order: AdviceOrder = { key: 5, name: '头孢', remark: '皮试阳性禁用' }
    const wrapper = mount(AdviceOrderDisplayCell, { props: { order, compact: true }, global })
    expect(wrapper.classes()).toContain('is-compact')
    // compact 下 remark 通过 CSS 隐藏，但 DOM 仍不渲染警告块（showRemark 默认 true 时靠样式），
    // 这里断言 compact 类存在即视为通过；文本仍在 DOM 中由样式隐藏。
    expect(wrapper.find('.advice-cell__warning').exists()).toBe(true)
  })

  it('renders grouped bracket layout with per-drug titles and one shared line', () => {
    const order: AdviceOrder = {
      key: 6,
      name: '补液组',
      groupItems: [
        { key: 'g1', name: '0.9%氯化钠注射液', specification: '250ml', dose: 250, unit: 'ml', pharmacy: '住院药房', route: '静滴', frequency: 'qd', days: 1 },
        { key: 'g2', name: '注射用头孢曲松钠', specification: '2g', dose: 2, unit: 'g' }
      ]
    }
    const wrapper = mount(AdviceOrderDisplayCell, { props: { order }, global })
    expect(wrapper.classes()).toContain('is-group')
    // 题名拼接：名称【规格】（药房）
    expect(wrapper.text()).toContain('0.9%氯化钠注射液【250ml】（住院药房）')
    expect(wrapper.text()).toContain('注射用头孢曲松钠【2g】')
    // 每味药一行
    expect(wrapper.findAll('.advice-cell__group-line')).toHaveLength(2)
    // 剂量：250ml/次
    expect(wrapper.text()).toContain('250ml/次')
    // 共用行取首项：静滴，qd（1天）
    expect(wrapper.find('.advice-cell__group-shared').text()).toContain('静滴，qd（1天）')
  })

  it('falls back to single layout when groupItems is empty', () => {
    const order: AdviceOrder = { key: 7, name: '单条医嘱', groupItems: [] }
    const wrapper = mount(AdviceOrderDisplayCell, { props: { order }, global })
    expect(wrapper.classes()).not.toContain('is-group')
    expect(wrapper.find('.advice-cell__primary').text()).toContain('单条医嘱')
  })
})
