// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { describe, expect, it } from 'vitest'

import PaginationBar from '@/components/common/PaginationBar.vue'
import RecordSidebar from '@/components/common/RecordSidebar.vue'

import MedicalRecordHistoryPanel from './MedicalRecordHistoryPanel.vue'

const rows = [
  {
    encounterId: 'encounter-history-1',
    visitDate: '2026-06-20 09:30',
    deptName: '心血管内科',
    doctorName: '李医生',
    diagnosis: '高血压'
  }
]

describe('MedicalRecordHistoryPanel', () => {
  it('将病历摘要转换为公共侧栏记录，并显示窄栏分页器', () => {
    const wrapper = mount(MedicalRecordHistoryPanel, {
      props: {
        patientSelected: true,
        rows,
        page: 1,
        pageSize: 10,
        total: 12
      }
    })

    const sidebar = wrapper.getComponent(RecordSidebar)
    const sidebarItems = sidebar.props('items') ?? []
    expect(sidebarItems).toHaveLength(1)
    expect(sidebarItems[0]!).toMatchObject({
      key: 'encounter-history-1',
      title: '2026-06-20 09:30',
      subtitle: '心血管内科 · 李医生',
      description: '主诊断：高血压'
    })
    expect(wrapper.getComponent(PaginationBar).props('total')).toBe(12)
  })

  it('转发搜索、分页和历史病历选择事件', async () => {
    const wrapper = mount(MedicalRecordHistoryPanel, {
      props: {
        patientSelected: true,
        rows
      }
    })

    wrapper.getComponent(RecordSidebar).vm.$emit('search', '高血压')
    wrapper.getComponent(PaginationBar).vm.$emit('change', { page: 2, pageSize: 10 })
    wrapper.getComponent(RecordSidebar).vm.$emit('select', {
      key: 'encounter-history-1'
    })
    await wrapper.vm.$nextTick()

    expect(wrapper.emitted('search')).toEqual([['高血压']])
    expect(wrapper.emitted('pageChange')).toEqual([[{ page: 2, pageSize: 10 }]])
    expect(wrapper.emitted('select')).toEqual([[rows[0]]])
  })
})
