import { createFrontendMenus } from '@/config/frontend-menus'
import type { Menu } from '@/apis/login/type'
import type { WorkbenchAction, WorkbenchData } from './type'

type WorkbenchRole =
  | 'administrator'
  | 'cashier'
  | 'doctor'
  | 'finance'
  | 'nurse'
  | 'pharmacist'
  | 'reporter'
  | 'technician'
  | 'warehouse'

type ActionRule = {
  group: string
  roles: WorkbenchRole[]
}

const COMMON_ACTIONS: WorkbenchAction[] = [
  { id: 'profile', title: '个人中心', path: '/profile', icon: 'User', group: '个人' },
  { id: 'change-password', title: '修改密码', path: '/profile?tab=password', icon: 'Lock', group: '个人' }
]

const BASE_DATA_RULE: ActionRule = {
  group: '基础数据',
  roles: ['administrator', 'cashier', 'doctor', 'nurse', 'pharmacist', 'technician', 'warehouse']
}

function resolveActionRule(path: string, parentText: string): ActionRule {
  if (path === '/outpatient/doctor-station' || path === '/inpatient/doctor-station') {
    return { group: '医生工作站', roles: ['administrator', 'doctor'] }
  }
  if (path === '/outpatient/nurse-station' || path === '/inpatient/nurse-station') {
    return { group: '护士工作站', roles: ['administrator', 'nurse'] }
  }
  if (path.startsWith('/outpatient/charge') || path.startsWith('/inpatient/charge')) {
    return { group: '收费结算', roles: ['administrator', 'cashier', 'finance'] }
  }
  if (path.startsWith('/booking/')) {
    return { group: '排班预约', roles: ['administrator', 'cashier', 'doctor', 'nurse'] }
  }
  if (path.startsWith('/warehouse/') || path.startsWith('/inventory/')) {
    return { group: '药库药房', roles: ['administrator', 'pharmacist', 'warehouse'] }
  }
  if (path.startsWith('/pharmacy/')) {
    return { group: '发药管理', roles: ['administrator', 'pharmacist'] }
  }
  if (path.startsWith('/report/')) {
    return { group: '报表中心', roles: ['administrator', 'finance', 'reporter'] }
  }
  if (path.startsWith('/system/') || path.startsWith('/configuration/')) {
    return { group: '系统管理', roles: ['administrator'] }
  }
  if (path.startsWith('/basic/')) return BASE_DATA_RULE
  if (path.startsWith('/basicdata/')) {
    return { group: '基础数据', roles: ['administrator', 'doctor', 'nurse', 'pharmacist', 'technician'] }
  }
  return { group: parentText, roles: ['administrator'] }
}

function collectMenuActions(menus: Menu[], roles: Set<string>, enforceRoleRules = false): WorkbenchAction[] {
  const actions: WorkbenchAction[] = []

  for (const menu of menus) {
    const entries = menu.href
      ? [{ id: menu.id, text: menu.text, href: menu.href, icon: menu.icon }]
      : menu.children ?? []

    for (const entry of entries) {
      if (!entry.href || entry.href === '/workbench') continue
      const rule = resolveActionRule(entry.href, menu.text)
      if (enforceRoleRules) {
        const hasAccess = roles.has('administrator') || rule.roles.some((role) => roles.has(role))
        if (!hasAccess) continue
      }
      actions.push({
        id: entry.id,
        title: entry.text,
        path: entry.href,
        icon: entry.icon,
        group: rule.group
      })
    }
  }

  return actions
}

const NOTICES: WorkbenchData['notices'] = [
    { id: 'n1', title: '系统将于本周六凌晨2点进行维护升级', level: 'warning', createdAt: '2025-07-01 10:00' },
    { id: 'n2', title: '新增药品审批流程已上线，请相关人员查阅', level: 'info', createdAt: '2025-06-30 15:30' },
    { id: 'n3', title: '门诊发药窗口调整通知', level: 'success', createdAt: '2025-06-28 09:00' }
]

function delay<T>(data: T, ms = 200): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(data), ms))
}

export function mockLoadWorkbenchData(userRoles: string[] = [], menus: Menu[] = createFrontendMenus()): Promise<WorkbenchData> {
  const roles = new Set(userRoles.map((role) => role.toLowerCase()))
  // 真实登录时 menus 已由后端按角色过滤；工作台不再重复用前端角色名猜测权限。
  const actions = [...collectMenuActions(menus, roles), ...COMMON_ACTIONS]
  const groups = new Set(actions.map((action) => action.group))

  return delay({
    notices: [...NOTICES],
    actions,
    statistics: [
      { key: 'available-actions', label: '可用功能', value: String(actions.length), trendType: 'flat' },
      { key: 'available-groups', label: '业务分组', value: String(groups.size), trendType: 'flat' },
      { key: 'notices', label: '通知公告', value: String(NOTICES.length), trendType: 'flat' }
    ]
  })
}
