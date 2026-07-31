import type { MenuQuery, MenuSavePayload } from './type'
import { menuStore, mockCreateMenu, mockLoadMenuNameTree, mockUpdateMenu } from './mock'

/** 获取菜单名称树。 */
export const loadMenuNameTree = () => mockLoadMenuNameTree()
/** 获取菜单列表（条件 + 分页）。 */
export const pageMenus = (query: MenuQuery) => menuStore.page(query)
/** 获取指定菜单详情。 */
export const getMenuDetail = (id: string) => menuStore.detail(id)
/** 新增菜单。 */
export const createMenu = (data: MenuSavePayload) => mockCreateMenu(data)
/** 修改菜单。 */
export const updateMenu = (id: string, data: MenuSavePayload) => mockUpdateMenu(id, data)
/** 删除菜单。 */
export const removeMenu = (id: string) => menuStore.remove(id)
/** 停用或启用菜单。 */
export const setMenuEnabled = (id: string, enabled: boolean) => menuStore.update(id, { enabled })

export type * from './type'
