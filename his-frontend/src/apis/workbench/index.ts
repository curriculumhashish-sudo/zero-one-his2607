import { type JsonVO } from '@/apis/http'
import { createJ2HttpClient } from '@/apis/j2-http'
import { useUserStore } from '@/stores/user'
import { createFrontendMenus } from '@/config/frontend-menus'
import router from '@/router'
import type { Menu } from '@/apis/login/type'
import type { WorkbenchAction, WorkbenchData } from './type'
import { mockLoadWorkbenchData } from './mock'

/**
 * J2 工作台服务客户端
 * 后端接口约定：
 *  - GET /j2-workbench/query-workbench?tenantId=xxx&userId=xxx&keyword=xxx
 *    返回 List<UserWorkbenchVO>，每个元素按角色分组，包含该角色可见的菜单卡片
 */
const workbenchHttp = createJ2HttpClient('/j2-workbench')
const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_USE_MOCK === 'true'

/** 后端 WorkbenchMenuVO 字段 */
interface BackendMenuVO {
  id?: string
  code?: string
  name?: string
  displayName?: string
  icon?: string
  path?: string
  component?: string
  permissionName?: string
  sortOrder?: number
}

/** 后端 UserWorkbenchVO 字段（按角色分组的工作台数据） */
interface BackendUserWorkbenchVO {
  roleId?: string
  roleCode?: string
  roleName?: string
  menus?: BackendMenuVO[]
}

/** 修复后端可能返回的 UTF-8 编码异常（如 "ä½é" → "住院"） */
function fixEncoding(raw: string): string {
  if (!raw) return ''
  try {
    if (/^[\x00-\x7F]+$/.test(raw)) return raw
    const bytes = new Uint8Array(raw.length)
    for (let i = 0; i < raw.length; i++) bytes[i] = raw.charCodeAt(i) & 0xFF
    const fixed = new TextDecoder('utf-8', { fatal: false }).decode(bytes)
    if (/[\u4e00-\u9fa5]/.test(fixed)) return fixed
  } catch { /* ignore */ }
  return raw
}

/** 检查路径是否对应已注册的前端路由（排除 404 兜底） */
function isValidFrontendPath(path: string): boolean {
  if (!path) return false
  try {
    const resolved = router.resolve(path)
    return resolved.name !== 'NotFound'
  } catch {
    return false
  }
}

/** 按路径将快捷方式归入业务大类 */
function resolveActionCategory(path: string): string {
  if (path.startsWith('/system/') || path.startsWith('/configuration/')) return '系统管理'
  if (path.startsWith('/basic/') || path.startsWith('/basicdata/')) return '基础数据'
  if (path === '/outpatient/doctor-station' || path === '/inpatient/doctor-station') return '临床工作站'
  if (path === '/outpatient/nurse-station' || path === '/inpatient/nurse-station') return '临床工作站'
  if (path.startsWith('/outpatient/charge/') || path.startsWith('/inpatient/charge/')) return '收费结算'
  if (path.startsWith('/booking/')) return '排班预约'
  // 发药管理：门诊/住院发药、退药、发药记录
  if (
    path.startsWith('/pharmacy/dispense/') ||
    path.startsWith('/pharmacy/return-medicine/') ||
    path.startsWith('/pharmacy/dispense-record/') ||
    path.startsWith('/pharmacy/outpatient-dispense')
  ) return '发药管理'
  // 药品管理：药库/药房库存相关
  if (path.startsWith('/warehouse/') || path.startsWith('/inventory/')) return '药品管理'
  if (path.startsWith('/pharmacy/')) return '药品管理'
  if (path.startsWith('/report/')) return '报表中心'
  if (path === '/profile') return '个人'
  return '常用功能'
}

/** 从前端菜单配置生成兜底的快捷入口（J2 无数据时使用） */
function buildFallbackActions(): WorkbenchAction[] {
  const actions: WorkbenchAction[] = []
  const menus = createFrontendMenus()
  for (const group of menus) {
    for (const child of group.children ?? []) {
      if (!child.href) continue
      if (child.children && child.children.length > 0) {
        for (const sub of child.children) {
          if (!sub.href) continue
          actions.push({
            id: sub.id,
            title: sub.text,
            path: sub.href,
            icon: sub.icon || 'Menu',
            group: resolveActionCategory(sub.href)
          })
        }
      } else {
        actions.push({
          id: child.id,
          title: child.text,
          path: child.href,
          icon: child.icon || 'Menu',
          group: resolveActionCategory(child.href)
        })
      }
    }
  }
  return actions
}

/** 读取当前登录上下文中的 tenantId / userId */
function getLoginContext(): { tenantId: string; userId: string } {
  let tenantId = ''
  let userId = ''
  try {
    // 优先从 Pinia user store 读取
    const store = useUserStore()
    if (store.user?.id) userId = String(store.user.id)
    if (store.tenantId) tenantId = String(store.tenantId)
  } catch {
    /* store 未初始化时忽略 */
  }
  try {
    // 其次从登录上下文（loginContext）读取
    const ctxRaw = localStorage.getItem('loginContext') || ''
    if (ctxRaw) {
      const ctx = JSON.parse(ctxRaw) as { tenantId?: string | number | null }
      if (!tenantId && ctx.tenantId != null) tenantId = String(ctx.tenantId)
    }
  } catch {
    /* ignore */
  }
  // 最后兜底：从 JWT token 解析 userId
  if (!userId) {
    try {
      const token = localStorage.getItem('token') || ''
      if (token) {
        const parts = token.split('.')
        if (parts.length === 3) {
          const payload = JSON.parse(atob(parts[1].replace(/-/g, '+').replace(/_/g, '/')))
          userId = String(payload.id ?? payload.userId ?? payload.sub ?? '')
        }
      }
    } catch { /* ignore */ }
  }
  return { tenantId, userId }
}

function unwrap<T>(promise: Promise<JsonVO<T>>): Promise<T> {
  return promise.then((res) => {
    // 后端成功码为 10000；非 10000 视为失败
    if (res.code !== 10000 || res.data === undefined) {
      throw new Error(res.message || '工作台数据请求失败')
    }
    return res.data
  })
}

/**
 * 加载工作台数据
 * 优先使用 J2 后端接口；返回空时从前端菜单兜底生成
 */
export async function loadWorkbenchData(
  roles: string[] = [],
  keywordOrMenus?: string | Menu[],
  legacyMenus: Menu[] = []
): Promise<WorkbenchData> {
  const keyword = typeof keywordOrMenus === 'string' ? keywordOrMenus : ''
  const menus = Array.isArray(keywordOrMenus) ? keywordOrMenus : legacyMenus
  if (useMock) return mockLoadWorkbenchData(roles, menus)
  const { tenantId, userId } = getLoginContext()

  const params: Record<string, string> = {}
  if (tenantId) params.tenantId = tenantId
  if (userId) params.userId = userId
  if (keyword) params.keyword = keyword

  let groups: BackendUserWorkbenchVO[] = []
  try {
    console.log('[workbench] 请求参数:', { tenantId, userId })
    groups = await unwrap(workbenchHttp.get<BackendUserWorkbenchVO[]>('/query-workbench', params))
    console.log('[workbench] 后端返回分组数:', groups?.length ?? 0)
  } catch (e) {
    console.warn('[workbench] J2 请求失败，使用前端兜底:', e)
  }

  // 如果 J2 返回空数据，使用前端菜单兜底
  if (!groups || groups.length === 0) {
    console.log('[workbench] J2 无数据，从前端菜单生成兜底快捷入口')
    const fallback = buildFallbackActions()
      .filter((a) => isValidFrontendPath(a.path))
      .map((a) => ({ ...a, group: resolveActionCategory(a.path) }))
    console.log('[workbench] 兜底快捷入口数:', fallback.length)
    return {
      notices: [],
      actions: fallback,
      statistics: []
    }
  }

  const actions: WorkbenchAction[] = []
  for (const group of groups) {
    for (const menu of group.menus ?? []) {
      const title = fixEncoding(menu.displayName || menu.name || menu.code || '')
      if (!title) continue
      const menuPath = menu.path || ''
      // 过滤掉没有对应前端路由的快捷方式
      if (!isValidFrontendPath(menuPath)) {
        console.log('[workbench] 跳过无页面的快捷方式:', title, menuPath)
        continue
      }
      actions.push({
        id: String(menu.id ?? menu.code ?? menuPath ?? Math.random()),
        title,
        path: menuPath,
        icon: menu.icon || 'Menu',
        group: resolveActionCategory(menuPath)
      })
    }
  }

  // J2 有数据但全部过滤完了，也兜底
  if (actions.length === 0) {
    console.log('[workbench] J2 有分组但无有效菜单，使用前端兜底')
    return {
      notices: [],
      actions: buildFallbackActions()
        .filter((a) => isValidFrontendPath(a.path))
        .map((a) => ({ ...a, group: resolveActionCategory(a.path) })),
      statistics: []
    }
  }

  return {
    notices: [],
    actions,
    statistics: []
  }
}

/**
 * 从 J2 工作台数据构建导航菜单（已按角色过滤）。
 * 当 /auth/menus 不可用或返回空时，作为导航菜单的兜底数据源。
 * 工作台 actions 是扁平结构，这里按 group 字段重新聚合成两级菜单。
 * 每个角色都需要保留"工作台"入口，放在菜单首位。
 */
export async function loadMenusForNavigation(): Promise<Menu[]> {
  const data = await loadWorkbenchData()
  const groupMap = new Map<string, WorkbenchAction[]>()
  for (const action of data.actions) {
    const group = action.group || '常用功能'
    if (!groupMap.has(group)) groupMap.set(group, [])
    groupMap.get(group)!.push(action)
  }
  const menus: Menu[] = [
    {
      id: 'frontend-workbench',
      text: '工作台',
      icon: 'IconTickets',
      href: '/workbench'
    }
  ]
  for (const [group, actions] of groupMap) {
    const groupId = `workbench-${group}`
    menus.push({
      id: groupId,
      text: group,
      icon: 'IconMenu',
      children: actions.map((a) => ({
        id: a.id,
        pid: groupId,
        text: a.title,
        icon: a.icon || 'IconTickets',
        href: a.path
      }))
    })
  }
  console.log('[menus] 从工作台数据构建导航菜单，分组数:', menus.length)
  return menus
}

export type * from './type'
