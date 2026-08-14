import { describe, expect, it } from 'vitest'
import {
  createPermission,
  loadPermissionNameTree,
  pagePermissions,
  removePermission,
  updatePermission
} from './permission'
import {
  createMenu,
  getMenuDetail,
  pageMenus,
  removeMenu,
  setMenuEnabled,
  updateMenu
} from './menu'
import {
  createRole,
  getRoleDetail,
  loadRoleNameOptions,
  pageRoles,
  removeRole,
  updateRole
} from './role'

describe('系统管理权限、菜单和角色接口', () => {
  it('权限接口具备名称树、分页、新增、修改和删除能力', async () => {
    expect((await loadPermissionNameTree()).length).toBeGreaterThan(0)
    const created = await createPermission({
      name: 'Test.Permission',
      displayName: '测试权限',
      parentName: 'Test',
      description: '测试使用',
      grantable: true
    })
    await updatePermission(created.id, { ...created, displayName: '修改后的测试权限' })
    expect((await pagePermissions({ pageIndex: 1, pageSize: 20, keyword: '修改后' })).total).toBe(1)
    await removePermission(created.id)
  })

  it('菜单接口具备名称树、分页、详情、增改删和启停能力', async () => {
    const created = await createMenu({
      name: '测试菜单',
      parentName: '系统管理',
      path: '/system/test-menu',
      component: 'system/test/Index',
      icon: 'IconMenu',
      sortNo: 99,
      visible: true,
      enabled: true,
      description: '测试使用'
    })
    await updateMenu(created.id, { ...created, name: '修改后的测试菜单' })
    expect((await getMenuDetail(created.id)).name).toBe('修改后的测试菜单')
    expect((await setMenuEnabled(created.id, false)).enabled).toBe(false)
    expect((await pageMenus({ pageIndex: 1, pageSize: 20, keyword: created.path })).total).toBe(1)
    await removeMenu(created.id)
  })

  it('角色接口具备名称、分页、详情、新增、修改和删除能力', async () => {
    expect((await loadRoleNameOptions()).length).toBeGreaterThan(0)
    const created = await createRole({
      name: 'test-role',
      displayName: '测试角色',
      description: '测试使用',
      permissionIds: ['System.User'],
      isDefault: false
    })
    await updateRole(created.id, { ...created, displayName: '修改后的测试角色' })
    expect((await getRoleDetail(created.id)).permissionNames).toEqual(['用户管理'])
    expect((await pageRoles({ pageIndex: 1, pageSize: 20, keyword: created.name })).total).toBe(1)
    await removeRole(created.id)
  })
})
