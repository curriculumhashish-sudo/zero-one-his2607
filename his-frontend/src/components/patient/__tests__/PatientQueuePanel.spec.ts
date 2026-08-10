import { describe, expect, it } from 'vitest'
import { mount } from '@vue/test-utils'

import EmptyState from '@/components/common/EmptyState.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import type { DateRangeValue } from '@/components/common/type'

import PatientCard from '../PatientCard.vue'
import PatientQueuePanel from '../PatientQueuePanel.vue'
import type { PatientSummary } from '../type'

const patients: PatientSummary[] = [
  {
    id: 'visit-1001',
    name: '张三',
    gender: '男',
    age: 38,
    patientNo: 'MR001',
    visitNo: 'OP001',
    status: '候诊中',
    statusTone: 'warning'
  },
  {
    id: 'visit-1002',
    name: '李四',
    gender: '女',
    age: 42,
    patientNo: 'MR002',
    visitNo: 'OP002',
    status: '已接诊',
    statusTone: 'success'
  }
]

describe('PatientQueuePanel', () => {
  it('组合查询、单日日期和患者卡片组件', () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        selectedId: 'visit-1001',
        date: '2026-07-26'
      }
    })

    expect(wrapper.findComponent(SearchPanel).exists()).toBe(true)
    expect(wrapper.findComponent({ name: 'ElDatePicker' }).exists()).toBe(true)
    expect(wrapper.find('.patient-queue-panel__pagination').exists()).toBe(false)
    expect(wrapper.findAllComponents(PatientCard)).toHaveLength(2)
    expect(wrapper.find('.patient-info-bar').exists()).toBe(false)
    expect(wrapper.findComponent(EmptyState).exists()).toBe(false)
    expect(wrapper.text()).toContain('张三')
    expect(wrapper.text()).toContain('2')
  })

  it('支持隐藏后端暂不支持的关键词查询框', () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        date: '2026-07-26',
        showKeyword: false
      }
    })

    const searchPanel = wrapper.findComponent(SearchPanel)
    expect(searchPanel.props('showKeyword')).toBe(false)
  })

  it('把门诊患者卡片的精简展示配置传给每张卡片', () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        showDepartment: false,
        showPrimaryNumber: false,
        primaryNumberMode: 'patient-no'
      }
    })

    for (const card of wrapper.findAllComponents(PatientCard)) {
      expect(card.props('showDepartment')).toBe(false)
      expect(card.props('showPrimaryNumber')).toBe(false)
      expect(card.props('primaryNumberMode')).toBe('patient-no')
    }
  })

  it('在底部显示分页器并转发页码变化', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        pagination: {
          pageIndex: 2,
          pageSize: 10,
          total: 35
        }
      }
    })

    const pagination = wrapper.findComponent({ name: 'ElPagination' })
    expect(pagination.exists()).toBe(true)
    expect(pagination.props()).toMatchObject({
      currentPage: 2,
      pageSize: 10,
      total: 35
    })
    expect(wrapper.get('.patient-queue-panel__title').text()).toContain('35')

    pagination.vm.$emit('currentChange', 3)
    await wrapper.vm.$nextTick()

    expect(wrapper.emitted('pageChange')).toEqual([[3]])
  })

  it('根据 selectedId 展示卡片选中状态', () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        selectedId: 'visit-1002'
      }
    })

    expect(wrapper.findAll('.patient-card--selected')).toHaveLength(1)
    expect(wrapper.findAllComponents(PatientCard)[1]?.props('selected')).toBe(true)
  })

  it('查询时发出清理首尾空格后的 search', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: { patients }
    })

    const keywordInput = wrapper.get('.patient-queue-panel__search .el-input__inner')
    await keywordInput.setValue('  张三  ')
    await wrapper.get('.query-shell__btn--search').trigger('click')

    expect(wrapper.emitted('search')).toEqual([['张三']])
  })

  it('日期变化时转发 dateChange', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: { patients, date: '2026-07-19' }
    })
    const value = '2026-07-20'

    wrapper.findComponent({ name: 'ElDatePicker' }).vm.$emit('change', value)
    await wrapper.vm.$nextTick()

    expect(wrapper.emitted('singleDateChange')).toEqual([[value]])
  })

  it('将页面提供的默认日期显示在查询条件中', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        date: '2026-07-26'
      }
    })

    expect(wrapper.findComponent({ name: 'ElDatePicker' }).props('modelValue')).toBe('2026-07-26')

    await wrapper.setProps({
      date: '2026-07-20'
    })

    expect(wrapper.findComponent({ name: 'ElDatePicker' }).props('modelValue')).toBe('2026-07-20')
  })

  it('重置时清空关键词和日期条件', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: { patients, date: '2026-07-20' }
    })

    await wrapper.get('.patient-queue-panel__search .el-input__inner').setValue('张三')
    await wrapper.get('.query-shell__btn--reset').trigger('click')

    expect(wrapper.emitted('search')).toEqual([['']])
    expect(wrapper.emitted('singleDateChange')).toEqual([['']])
  })

  it('未传单日日期时兼容受控日期范围', async () => {
    const initial: DateRangeValue = ['2026-07-29', '2026-07-29']
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        dateRange: initial
      }
    })

    expect(wrapper.findComponent(DateRangeField).props('modelValue')).toEqual(initial)

    const changed: DateRangeValue = ['2026-07-30', '2026-07-30']
    await wrapper.setProps({ dateRange: changed })
    expect(wrapper.findComponent(DateRangeField).props('modelValue')).toEqual(changed)
  })

  it('点击患者卡片发出 select，但不自行修改 selectedId', async () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        selectedId: 'visit-1001'
      }
    })

    await wrapper.findAll('.patient-card')[1]?.trigger('click')

    expect(wrapper.emitted('select')).toEqual([[patients[1]]])
    expect(wrapper.props('selectedId')).toBe('visit-1001')
    expect(wrapper.findAllComponents(PatientCard)[0]?.props('selected')).toBe(true)
  })

  it('空列表使用 EmptyState，并转发刷新事件', async () => {
    const wrapper = mount(PatientQueuePanel)

    expect(wrapper.findComponent(EmptyState).exists()).toBe(true)
    expect(wrapper.text()).toContain('暂无患者')

    await wrapper.get('.empty-state__btn').trigger('click')

    expect(wrapper.emitted('refresh')).toEqual([[]])
  })

  it('loading 状态应用到列表和操作按钮', () => {
    const wrapper = mount(PatientQueuePanel, {
      props: {
        patients,
        loading: true
      }
    })

    expect(wrapper.get('.patient-queue-panel__list').attributes('aria-busy')).toBe('true')
    expect(wrapper.findComponent(EmptyState).exists()).toBe(false)
    expect(wrapper.find('.el-loading-mask').exists()).toBe(true)
  })
})
