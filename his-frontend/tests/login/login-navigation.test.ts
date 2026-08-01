// @vitest-environment jsdom

import { createPinia, setActivePinia } from 'pinia'
import { beforeEach, describe, expect, it, vi } from 'vitest'

vi.mock('@/views/HomeView.vue', () => ({ default: { name: 'MockHome', template: '<router-view />' } }))
vi.mock('@/views/dashboard/Dashboard.vue', () => ({ default: { name: 'MockDashboard', template: '<div />' } }))

import {
  loadCurrentMenus,
  loadCurrentUser,
  loginAccount,
  selectDepartment,
  selectHospital,
  selectTenant
} from '@/apis/login'
import router from '@/router'
import { useUserStore } from '@/stores/user'

describe('登录后主页面导航', () => {
  beforeEach(async () => {
    localStorage.clear()
    setActivePinia(createPinia())
    await router.replace({ name: 'Login' })
  })

  it('完成登录上下文后进入仪表盘主框架', async () => {
    const result = await loginAccount({ username: 'admin', password: '123456' })
    await selectTenant(1001)
    await selectHospital('hospital-1')
    await selectDepartment('dept-internal')

    const store = useUserStore()
    store.setToken(result)
    const [menus, user] = await Promise.all([loadCurrentMenus(), loadCurrentUser()])
    expect(menus).not.toHaveLength(0)
    expect(user.username).toBe('admin')

    await Promise.all([store.loadMenus(), store.loadUser()])
    store.setLoaded(true)
    await router.replace({ name: 'Dashboard' })

    expect(router.currentRoute.value.path).toBe('/dashboard')
    expect(router.currentRoute.value.matched.map((record) => record.name)).toEqual(['Home', 'Dashboard'])
  })
})
