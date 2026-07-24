import { describe, expect, it } from 'vitest'
import {
  createUser,
  getUserDetail,
  pageUsers,
  setUserEnabled,
  unlockUser,
  updateUser
} from './index'

describe('用户管理模拟接口', () => {
  it('支持创建、修改、启停和解锁', async () => {
    const created = await createUser({
      username: 'api-test-user',
      realName: '接口测试用户',
      phone: '13800138000',
      email: 'api-test@hospital.test',
      roleIds: ['role-doctor'],
      enabled: true
    })
    expect(created.roleNames).toEqual(['医生'])

    const updated = await updateUser(created.id, {
      username: created.username,
      realName: '修改后的用户',
      phone: created.phone,
      email: created.email,
      roleIds: ['role-nurse'],
      enabled: true
    })
    expect(updated.roleNames).toEqual(['护士'])
    expect((await setUserEnabled(created.id, false)).enabled).toBe(false)
    expect((await unlockUser(created.id)).locked).toBe(false)
    expect((await getUserDetail(created.id)).realName).toBe('修改后的用户')

    const result = await pageUsers({ keyword: created.username, page: 1, pageSize: 10 })
    expect(result.total).toBe(1)
  })
})
