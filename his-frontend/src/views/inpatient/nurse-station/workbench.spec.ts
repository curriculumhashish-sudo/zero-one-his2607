import { createPinia, setActivePinia } from 'pinia'
import { beforeEach, describe, expect, it } from 'vitest'
import { flushPromises, mount } from '@vue/test-utils'

import Index from './Index.vue'
import NurseStationPatientPanel from './components/NurseStationPatientPanel.vue'
import {
  DEFAULT_NURSE_STATION_TAB,
  getNurseStationOperationConfig,
  NURSE_STATION_OPERATION_CONFIGS,
  NURSE_STATION_TASK_FILTER_OPTIONS
} from './constants'

describe('住院护士站工作台骨架', () => {
  beforeEach(() => setActivePinia(createPinia()))

  it('展示本项目启用的八个业务页签及默认页签', () => {
    expect(NURSE_STATION_OPERATION_CONFIGS.map((item) => item.id)).toEqual([
      '入出转管理',
      '医嘱校对',
      '医嘱执行',
      '汇总领药',
      '护理记录',
      '体温单',
      '住院记账',
      '费用明细'
    ])
    expect(DEFAULT_NURSE_STATION_TAB).toBe('医嘱执行')
  })

  it('为批量业务和单患者业务配置正确选择策略', () => {
    expect(getNurseStationOperationConfig('医嘱校对')?.selectionPolicy).toBe('multiple')
    expect(getNurseStationOperationConfig('医嘱执行')?.selectionPolicy).toBe('multiple')
    expect(getNurseStationOperationConfig('汇总领药')?.selectionPolicy).toBe('multiple')
    expect(getNurseStationOperationConfig('体温单')?.selectionPolicy).toBe('single')
  })

  it('入出转管理展示可滚动的病区、病房和床位看板', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const atdTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '入出转管理')

    await atdTab?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.atd-module').exists()).toBe(true)
    expect(wrapper.find('.bed-board-scroll').exists()).toBe(true)
    expect(wrapper.find('.bed-ward').text()).toContain('内科疗区')
    expect(wrapper.find('.bed-house').text()).toContain('301病房')
    expect(wrapper.findAll('.bed-card').length).toBeGreaterThan(0)
  })

  it('可以切换到汇总领药页签并展示患者选择空状态', async () => {
    const wrapper = mount(Index)

    expect(wrapper.find('.operation-tabs__item.is-active').text()).toBe('医嘱执行')

    const dispenseTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '汇总领药')
    expect(dispenseTab).toBeDefined()

    await dispenseTab?.trigger('click')

    expect(wrapper.find('.operation-tabs__item.is-active').text()).toBe('汇总领药')
    expect(wrapper.find('.dispense-summary-module').exists()).toBe(true)
    expect(wrapper.find('.dispense-summary-module .el-empty__description').text()).toContain(
      '请先从左侧选择患者'
    )
  })

  it('医嘱校对根据左侧多选患者展示折叠医嘱卡片', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const auditTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '医嘱校对')

    await auditTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.order-audit-module').exists()).toBe(true)
    expect(wrapper.findAll('.patient-order-card')).toHaveLength(1)
    expect(wrapper.find('.patient-order-card__header').text()).toContain('孙晓梅')
    expect(wrapper.findAll('.order-audit-table .el-table__row')).toHaveLength(2)
  })

  it('医嘱执行根据左侧患者展示可开始执行的诊疗医嘱', async () => {
    const wrapper = mount(Index)
    await flushPromises()

    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.order-execute-module').exists()).toBe(true)
    expect(wrapper.findAll('.patient-execute-card')).toHaveLength(1)
    expect(wrapper.find('.patient-execute-card__header').text()).toContain('孙晓梅')
    expect(wrapper.findAll('.order-execute-table .el-table__row')).toHaveLength(2)
    expect(wrapper.find('.order-execute-toolbar').text()).toContain('开始执行')
    expect(wrapper.findAll('.schedule-slot')).toHaveLength(0)
    expect(wrapper.text()).not.toContain('取消执行')
  })

  it('汇总领药按选中患者展示药品医嘱和可勾选领药时点', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const dispenseTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '汇总领药')

    await dispenseTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.dispense-summary-module').exists()).toBe(true)
    expect(wrapper.find('.dispense-patient-card__header').text()).toContain('孙晓梅')
    expect(wrapper.findAll('.dispense-order-table .el-table__row')).toHaveLength(2)
    expect(wrapper.findAll('.dispense-slot')).toHaveLength(3)
    expect(wrapper.find('.record-count').text()).toContain('3个时点')

    const summaryMode = wrapper.find('.dispense-toolbar input[type="radio"][value="SUMMARY"]')
    expect(summaryMode.exists()).toBe(true)
    await summaryMode.setValue(true)
    await flushPromises()

    expect(wrapper.find('[data-testid="dispense-summary-table"]').exists()).toBe(true)
    expect(
      wrapper.findAll('[data-testid="dispense-summary-table"] .el-table__row').length
    ).toBeGreaterThan(0)
  })

  it('护理记录按单患者展示当周纸质护理记录单', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const nursingTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '护理记录')

    await nursingTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.nursing-record-module').exists()).toBe(true)
    expect(wrapper.find('.paper-record__doc-head h2').text()).toContain('护 理 记 录 单')
    expect(wrapper.find('.patient-info-line').text()).toContain('孙晓梅')
    expect(wrapper.findAll('.paper-record__row')).toHaveLength(14)
    expect(
      (
        wrapper.findAll('.paper-record__row').at(0)?.findAll('input').at(2)?.element as
          | HTMLInputElement
          | undefined
      )?.value
    ).toBe('36.5')
  })

  it('体温单按单患者展示七天六时次曲线和表尾', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const temperatureTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '体温单')

    await temperatureTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.temperature-sheet-module').exists()).toBe(true)
    expect(wrapper.find('.paper-heading h2').text()).toContain('体 温 单')
    expect(wrapper.find('.patient-header').text()).toContain('孙晓梅')
    expect(wrapper.findAll('.day-cell--clickable')).toHaveLength(7)
    expect(wrapper.findAll('.time-group span')).toHaveLength(42)
    expect(wrapper.find('[data-testid="temperature-chart"]').exists()).toBe(true)
    expect(wrapper.find('[data-testid="temperature-footer"]').text()).not.toContain('皮试')
  })

  it('住院记账按单患者只展示后端当前支持的自动计费规则', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const billingTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '住院记账')

    await billingTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.inpatient-billing-module').exists()).toBe(true)
    expect(wrapper.find('.billing-patient-header').text()).toContain('孙晓梅')
    expect(wrapper.find('.billing-patient-header').text()).toContain('account-001')
    expect(wrapper.findAll('[data-testid="auto-billing-table"] .el-table__row')).toHaveLength(1)
    expect(wrapper.text()).not.toContain('医嘱计费')
    expect(wrapper.text()).not.toContain('非医嘱计费')
    expect(wrapper.text()).not.toContain('取消划价')
    expect(wrapper.text()).not.toContain('护理级别自动计费')
    expect(wrapper.text()).not.toContain('停用')
  })

  it('费用明细按单患者展示后端字段和分类日期汇总', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const costTab = wrapper
      .findAll('.operation-tabs__item')
      .find((item) => item.text() === '费用明细')

    await costTab?.trigger('click')
    await wrapper.findAll('.patient-card').at(0)?.trigger('click')
    await flushPromises()

    expect(wrapper.find('.cost-detail-module').exists()).toBe(true)
    expect(wrapper.find('.cost-patient-header').text()).toContain('孙晓梅')
    expect(wrapper.findAll('[data-testid="cost-detail-table"] .el-table__row')).toHaveLength(3)
    expect(wrapper.find('[data-testid="cost-detail-table"]').text()).toContain('费用分类')
    expect(wrapper.text()).not.toContain('来源')
    expect(wrapper.text()).not.toContain('药品合计')

    const summaryTab = wrapper
      .findAll('.presentation-tabs button')
      .find((item) => item.text() === '汇总')
    await summaryTab?.trigger('click')
    await flushPromises()

    expect(wrapper.findAll('[data-testid="cost-summary-table"] .el-table__row')).toHaveLength(3)
    expect(wrapper.find('[data-testid="cost-summary-table"]').text()).toContain('汇总日期')
    expect(wrapper.find('.cost-item-count').text()).toContain('汇总项数：3 项')
  })

  it('挂载后加载患者队列和一致的顶部统计', async () => {
    const wrapper = mount(Index)
    await flushPromises()

    expect(wrapper.findAll('.patient-card')).toHaveLength(8)
    expect(wrapper.findAll('.statistic__value').map((item) => item.text())).toEqual([
      '8',
      '11',
      '20'
    ])
    expect(wrapper.find('.patient-panel__spatial').exists()).toBe(false)
  })

  it('任务筛选会同时刷新患者队列和统计', async () => {
    const wrapper = mount(Index)
    await flushPromises()
    const panel = wrapper.findComponent(NurseStationPatientPanel)

    panel.vm.$emit('update:taskFilter', 'pending-audit')
    await flushPromises()

    expect(wrapper.findAll('.patient-card')).toHaveLength(5)
    expect(wrapper.findAll('.statistic__value').map((item) => item.text())).toEqual([
      '5',
      '11',
      '17'
    ])
  })

  it('不再暴露后端未实现的已处理和召回筛选', async () => {
    const wrapper = mount(Index)
    await flushPromises()

    expect(NURSE_STATION_TASK_FILTER_OPTIONS.map((item) => item.value)).toEqual([
      'all',
      'pending-audit',
      'pending-execute'
    ])
    expect(wrapper.text()).not.toContain('已出院结算')
    expect(wrapper.text()).not.toContain('召回')
  })
})
