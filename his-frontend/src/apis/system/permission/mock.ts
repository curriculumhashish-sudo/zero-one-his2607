import { createCrudStore } from '@/apis/basic/crud-mock'
import type { PermissionRecord, PermissionSavePayload } from './type'

const store = createCrudStore<PermissionRecord>(
  'permission',
  [
    {
      id: 'permission-001',
      name: 'System.User',
      displayName: '用户管理',
      parentName: 'System',
      description: '查看和维护系统用户',
      providerName: 'R',
      grantable: true
    },
    {
      id: 'permission-002',
      name: 'System.Role',
      displayName: '角色管理',
      parentName: 'System',
      description: '查看和维护系统角色',
      providerName: 'R',
      grantable: true
    },
    {
      id: 'permission-003',
      name: 'Outpatient.Order',
      displayName: '门诊开单',
      parentName: 'Outpatient',
      description: '创建和维护门诊医嘱',
      providerName: 'R',
      grantable: true
    }
  ],
  ['name', 'displayName', 'parentName', 'description']
)

function normalize(payload: PermissionSavePayload): PermissionSavePayload {
  if (!payload.name.trim() || !payload.displayName.trim()) {
    throw new Error('权限标识和权限名称不能为空')
  }
  return {
    ...payload,
    name: payload.name.trim(),
    displayName: payload.displayName.trim(),
    parentName: payload.parentName.trim(),
    description: payload.description.trim()
  }
}

export async function mockCreatePermission(payload: PermissionSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (rows.some((item) => item.name.toLowerCase() === normalized.name.toLowerCase())) {
    throw new Error('权限标识已存在')
  }
  return store.create({ ...normalized, providerName: 'R' })
}

export async function mockUpdatePermission(id: string, payload: PermissionSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (
    rows.some((item) => item.id !== id && item.name.toLowerCase() === normalized.name.toLowerCase())
  ) {
    throw new Error('权限标识已存在')
  }
  return store.update(id, { ...normalized, providerName: 'R' })
}

export async function mockLoadPermissionNameTree() {
  return (await store.list()).map((item) => ({ label: item.displayName, value: item.name }))
}

export { store as permissionStore }
