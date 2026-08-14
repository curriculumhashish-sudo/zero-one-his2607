import { describe, expect, it } from 'vitest'
import {
  createDepartment,
  getDepartmentDetail,
  listDepartments,
  removeDepartment,
  setDepartmentEnabled,
  updateDepartment
} from './index'

describe('科室管理模拟接口', () => {
  it('支持树形创建、修改、启停和删除', async () => {
    const created = await createDepartment({
      name: '接口测试科室',
      parentId: 'dept-002',
      type: '临床科室',
      category: '门诊',
      leaderId: 'leader-1',
      sort: 99,
      enabled: true
    })
    expect(created.parentName).toBe('门诊部')

    const updated = await updateDepartment(created.id, { ...created, name: '已修改测试科室' })
    expect(updated.name).toBe('已修改测试科室')
    expect((await setDepartmentEnabled(created.id, false)).enabled).toBe(false)
    expect((await getDepartmentDetail(created.id)).enabled).toBe(false)

    const tree = await listDepartments({ keyword: '已修改测试科室' })
    expect(tree[0]?.name).toBe('零壹医院')
    await removeDepartment(created.id)
    await expect(getDepartmentDetail(created.id)).rejects.toThrow('科室不存在')
  })
})
