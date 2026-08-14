// @vitest-environment jsdom

import { createPinia, setActivePinia } from 'pinia'
import { beforeEach, describe, expect, it } from 'vitest'

import { useUserStore } from '@/stores/user'

describe('登录会话恢复', () => {
  beforeEach(() => {
    localStorage.clear()
    setActivePinia(createPinia())
  })

  it('刷新后从持久化凭据恢复用户和登录上下文', async () => {
    localStorage.setItem('token', 'persisted-access-token')
    localStorage.setItem('refreshToken', 'persisted-refresh-token')
    localStorage.setItem(
      'loginContext',
      JSON.stringify({ tenantId: 1001, hospitalId: 'hospital-1', departmentId: 'dept-internal' })
    )

    const store = useUserStore()
    expect(store.getToken).toBe('persisted-access-token')

    await store.loadUser()

    expect(store.token).toBe('persisted-access-token')
    expect(store.refreshToken).toBe('persisted-refresh-token')
    expect(store.user?.username).toBe('admin')
    expect(store.tenantId).toBe(1001)
    expect(store.hospitalId).toBe('hospital-1')
    expect(store.departmentId).toBe('dept-internal')
  })

  it('退出清理凭据、上下文和用户状态', () => {
    localStorage.setItem('token', 'persisted-access-token')
    localStorage.setItem('refreshToken', 'persisted-refresh-token')
    localStorage.setItem('loginContext', JSON.stringify({ tenantId: 1001 }))
    const store = useUserStore()
    store.token = 'persisted-access-token'
    store.refreshToken = 'persisted-refresh-token'
    store.setLoginContext({ tenantId: 1001, hospitalId: 'hospital-1', departmentId: 'dept-internal' })

    store.resetSaveData()

    expect(store.getToken).toBe('')
    expect(store.user).toBeNull()
    expect(store.tenantId).toBeNull()
    expect(store.hospitalId).toBeNull()
    expect(store.departmentId).toBeNull()
    expect(localStorage.getItem('token')).toBeNull()
    expect(localStorage.getItem('refreshToken')).toBeNull()
    expect(localStorage.getItem('loginContext')).toBeNull()
  })
})
