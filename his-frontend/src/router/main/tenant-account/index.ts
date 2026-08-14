import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  { path: '/system/tenant', name: 'TenantManagement', meta: { label: '租户管理' }, component: () => import('@/views/system/tenant/Index.vue') },
  { path: '/system/settings', name: 'SettingsManagement', meta: { label: '设置管理' }, component: () => import('@/views/system/settings/Index.vue') },
  { path: '/profile', name: 'MyProfile', meta: { label: '个人中心' }, component: () => import('@/views/account/profile/Index.vue') },
  { path: '/workbench', name: 'WorkbenchHome', meta: { label: '工作台' }, component: () => import('@/views/workbench/Index.vue') }
]

export default routes
