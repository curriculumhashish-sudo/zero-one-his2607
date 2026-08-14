import { createCrudStore } from '@/apis/basic/crud-mock'
import type { RoleRecord, RoleSavePayload } from './type'

const permissionMap = new Map([
  ['System.User', '用户管理'],
  ['System.Role', '角色管理'],
  ['System.Menu', '菜单管理'],
  ['Outpatient.Order', '门诊开单'],
  ['Pharmacy.Dispense', '药房发药']
])

const store = createCrudStore<RoleRecord>(
  'role',
  [
    seed(
      'role-admin',
      'admin',
      '系统管理员',
      '拥有系统管理权限',
      ['System.User', 'System.Role', 'System.Menu'],
      true
    ),
    seed('role-doctor', 'doctor', '医生', '负责门诊和住院诊疗', ['Outpatient.Order'], false),
    seed('role-pharmacist', 'pharmacist', '药师', '负责药房发退药', ['Pharmacy.Dispense'], false)
  ],
  ['name', 'displayName', 'description', 'permissionNames']
)

function seed(
  id: string,
  name: string,
  displayName: string,
  description: string,
  permissionIds: string[],
  isDefault: boolean
): RoleRecord {
  return {
    id,
    name,
    displayName,
    description,
    permissionIds,
    permissionNames: resolvePermissionNames(permissionIds),
    isDefault
  }
}

function resolvePermissionNames(ids: string[]) {
  return ids.map((id) => permissionMap.get(id) ?? id)
}

function normalize(payload: RoleSavePayload): RoleSavePayload {
  if (!payload.name.trim() || !payload.displayName.trim()) {
    throw new Error('角色编码和角色名称不能为空')
  }
  return {
    ...payload,
    name: payload.name.trim(),
    displayName: payload.displayName.trim(),
    description: payload.description.trim(),
    permissionIds: [...new Set(payload.permissionIds)]
  }
}

export async function mockCreateRole(payload: RoleSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (rows.some((item) => item.name.toLowerCase() === normalized.name.toLowerCase())) {
    throw new Error('角色编码已存在')
  }
  return store.create({
    ...normalized,
    permissionNames: resolvePermissionNames(normalized.permissionIds)
  })
}

export async function mockUpdateRole(id: string, payload: RoleSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (
    rows.some((item) => item.id !== id && item.name.toLowerCase() === normalized.name.toLowerCase())
  ) {
    throw new Error('角色编码已存在')
  }
  return store.update(id, {
    ...normalized,
    permissionNames: resolvePermissionNames(normalized.permissionIds)
  })
}

export async function mockLoadRoleNameOptions() {
  return (await store.list()).map((item) => ({ id: item.id, name: item.displayName }))
}

export { store as roleStore }
