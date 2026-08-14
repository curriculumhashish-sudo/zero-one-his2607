import { createCrudStore } from '@/apis/basic/crud-mock'
import type { MenuRecord, MenuSavePayload } from './type'

const store = createCrudStore<MenuRecord>(
  'menu',
  [
    seed(
      'menu-001',
      '系统管理',
      '',
      '/system',
      '',
      'IconSetting',
      10,
      true,
      true,
      '系统管理菜单组'
    ),
    seed(
      'menu-002',
      '用户管理',
      '系统管理',
      '/system/user',
      'system/user/Index',
      'IconUser',
      11,
      true,
      true,
      '维护用户账号'
    ),
    seed(
      'menu-003',
      '角色管理',
      '系统管理',
      '/system/role',
      'system/role/Index',
      'IconUserFilled',
      12,
      true,
      true,
      '维护角色权限'
    ),
    seed('menu-004', '基础数据', '', '/basic', '', 'IconMenu', 20, true, true, '基础数据菜单组')
  ],
  ['name', 'parentName', 'path', 'component', 'description']
)

function seed(
  id: string,
  name: string,
  parentName: string,
  path: string,
  component: string,
  icon: string,
  sortNo: number,
  visible: boolean,
  enabled: boolean,
  description: string
): MenuRecord {
  return { id, name, parentName, path, component, icon, sortNo, visible, enabled, description }
}

function normalize(payload: MenuSavePayload): MenuSavePayload {
  if (!payload.name.trim()) throw new Error('菜单名称不能为空')
  if (payload.path && !payload.path.startsWith('/')) throw new Error('菜单路径必须以 / 开头')
  return {
    ...payload,
    name: payload.name.trim(),
    parentName: payload.parentName.trim(),
    path: payload.path.trim(),
    component: payload.component.trim(),
    icon: payload.icon.trim(),
    description: payload.description.trim(),
    sortNo: Number(payload.sortNo) || 0
  }
}

export async function mockCreateMenu(payload: MenuSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (rows.some((item) => item.path && item.path === normalized.path)) {
    throw new Error('菜单路径已存在')
  }
  return store.create(normalized)
}

export async function mockUpdateMenu(id: string, payload: MenuSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (normalized.path && rows.some((item) => item.id !== id && item.path === normalized.path)) {
    throw new Error('菜单路径已存在')
  }
  return store.update(id, normalized)
}

export async function mockLoadMenuNameTree() {
  return (await store.list()).map((item) => ({ label: item.name, value: item.name }))
}

export { store as menuStore }
