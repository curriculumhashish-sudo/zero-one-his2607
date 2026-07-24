// @vitest-environment jsdom

import { defineComponent } from 'vue'
import { flushPromises, shallowMount } from '@vue/test-utils'
import { beforeEach, describe, expect, it, vi } from 'vitest'
import UserPage from './system/user/Index.vue'
import DepartmentPage from './basic/department/Index.vue'

const userApi = vi.hoisted(() => ({
  pageUsers: vi.fn(),
  setUserEnabled: vi.fn(),
  unlockUser: vi.fn()
}))
const departmentApi = vi.hoisted(() => ({
  listDepartments: vi.fn(),
  removeDepartment: vi.fn(),
  setDepartmentEnabled: vi.fn()
}))

vi.mock('@/apis/system/user', () => userApi)
vi.mock('@/apis/basic/department', () => departmentApi)

const userDialogOpen = vi.fn()
const departmentDialogOpen = vi.fn()

const SearchPanelStub = defineComponent({
  name: 'SearchPanel',
  emits: ['search', 'reset'],
  template: '<div class="search-panel-stub"><slot name="header" /></div>'
})
const ActionToolbarStub = defineComponent({
  name: 'ActionToolbar',
  emits: ['action'],
  template: '<div class="action-toolbar-stub" />'
})
const DataTablePanelStub = defineComponent({
  name: 'DataTablePanel',
  props: ['data', 'loading', 'total', 'page', 'pageSize'],
  emits: ['update:page', 'update:pageSize'],
  template: '<div class="data-table-stub"><slot /></div>'
})
const UserEditDialogStub = defineComponent({
  name: 'UserEditDialog',
  emits: ['saved'],
  setup(_, { expose }) {
    expose({ open: userDialogOpen })
    return () => null
  }
})
const DepartmentEditDialogStub = defineComponent({
  name: 'DepartmentEditDialog',
  emits: ['saved'],
  setup(_, { expose }) {
    expose({ open: departmentDialogOpen })
    return () => null
  }
})

const commonStubs = {
  SearchPanel: SearchPanelStub,
  ActionToolbar: ActionToolbarStub,
  DataTablePanel: DataTablePanelStub,
  StatusDisplay: true,
  RowActionGroup: true,
  ElAlert: true,
  ElButton: true,
  ElSelect: true,
  ElOption: true,
  ElTableColumn: true
}

beforeEach(() => {
  vi.clearAllMocks()
  userApi.pageUsers.mockResolvedValue({ rows: [], total: 0 })
  departmentApi.listDepartments.mockResolvedValue([])
})

describe('A组页面事件连接', () => {
  it('用户页面加载列表并能打开新增对话框', async () => {
    const wrapper = shallowMount(UserPage, {
      global: { stubs: { ...commonStubs, UserEditDialog: UserEditDialogStub } }
    })
    await flushPromises()

    expect(userApi.pageUsers).toHaveBeenCalledWith(
      expect.objectContaining({ page: 1, pageSize: 10, enabled: null, locked: null })
    )
    wrapper.findComponent(ActionToolbarStub).vm.$emit('action', { key: 'create' })
    await flushPromises()
    expect(userDialogOpen).toHaveBeenCalledOnce()

    wrapper.findComponent(SearchPanelStub).vm.$emit('reset')
    await flushPromises()
    expect(userApi.pageUsers).toHaveBeenCalledTimes(2)
  })

  it('科室页面加载树并能打开新增顶级科室对话框', async () => {
    const wrapper = shallowMount(DepartmentPage, {
      global: {
        stubs: { ...commonStubs, DepartmentEditDialog: DepartmentEditDialogStub }
      }
    })
    await flushPromises()

    expect(departmentApi.listDepartments).toHaveBeenCalledWith(
      expect.objectContaining({ keyword: '', type: '', category: '', enabled: null })
    )
    wrapper.findComponent(ActionToolbarStub).vm.$emit('action', { key: 'create' })
    await flushPromises()
    expect(departmentDialogOpen).toHaveBeenCalledOnce()

    wrapper.findComponent(SearchPanelStub).vm.$emit('reset')
    await flushPromises()
    expect(departmentApi.listDepartments).toHaveBeenCalledTimes(2)
  })
})
