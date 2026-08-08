import { toPageResult, unwrap, type BackendPage } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { MenuQuery, MenuRecord, MenuSavePayload } from './type'

const baseUrl = '/sysmgr/menu'

/** 获取菜单名称树。 */
export async function loadMenuNameTree() {
  const tree = unwrap(await useHttp().post<MenuRecord[]>(`${baseUrl}/getTree`))
  return flattenMenuTree(tree).map((item) => ({ label: item.name, value: item.id }))
}
/** 获取菜单列表（条件 + 分页）。 */
export async function pageMenus(query: MenuQuery) {
  const page = unwrap(
    await useHttp().post<BackendPage<MenuRecord>>(`${baseUrl}/getList`, {
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      keyword: query.keyword
    })
  )
  const result = toPageResult(page)
  result.rows = result.rows.map(normalizeMenu)
  return result
}
/** 获取指定菜单详情。 */
export async function getMenuDetail(id: string) {
  const row = unwrap(await useHttp().post<MenuRecord>(`${baseUrl}/getById`, { id }))
  return normalizeMenu(row)
}
/** 新增菜单。 */
export async function createMenu(data: MenuSavePayload): Promise<any> {
  return unwrap(await useHttp().post<boolean>(`${baseUrl}/create`, toMenuPayload(data)))
}
/** 修改菜单。 */
export async function updateMenu(id: string, data: MenuSavePayload): Promise<any> {
  return unwrap(await useHttp().put<boolean>(`${baseUrl}/update`, { id, ...toMenuPayload(data) }))
}
/** 删除菜单。 */
export async function removeMenu(id: string) {
  return unwrap(await useHttp().delete<boolean>(`${baseUrl}/delete`, { id }))
}
/** 停用或启用菜单。 */
export async function setMenuEnabled(id: string, enabled: boolean): Promise<any> {
  return unwrap(await useHttp().post<boolean>(`${baseUrl}/enable`, { id, status: enabled ? 1 : 0 }))
}

function toMenuPayload(data: MenuSavePayload) {
  return {
    parentId: data.parentId || data.parentName || undefined,
    code: data.code || data.name,
    displayName: data.displayName || data.name,
    showName: data.displayName || data.name,
    menuName: data.menuName || data.name,
    name: data.name,
    path: data.path,
    component: data.component,
    icon: data.icon,
    permissionName: data.permissionName,
    remark: data.remark || data.description,
    menuType: data.menuType || 'menu',
    sortOrder: data.sortNo,
    orderNum: data.sortNo,
    enabled: data.enabled,
    isEnabled: data.enabled,
    hidden: !data.visible,
    isHidden: !data.visible,
    external: false,
    isExternal: false,
    externalUrl: ''
  }
}

function normalizeMenu(row: MenuRecord): MenuRecord {
  return {
    ...row,
    id: row.id || row.menuId || '',
    name: row.name || row.menuName || row.showName || '',
    displayName: row.displayName || row.showName || row.menuName || '',
    parentName: row.parentName || row.parentId || '',
    sortNo: row.sortNo ?? row.sortNum ?? 0,
    visible: row.visible ?? !row.hidden,
    enabled: row.enabled ?? (row.status === 1 || row.status === '1'),
    description: row.description || row.remark || ''
  }
}

function flattenMenuTree(rows: MenuRecord[]): MenuRecord[] {
  return rows.flatMap((row) => [normalizeMenu(row), ...flattenMenuTree(row.children ?? [])])
}

export type * from './type'
