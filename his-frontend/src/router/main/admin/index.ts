import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/system/user',
    name: 'SystemUserManagement',
    meta: { label: '用户管理' },
    component: () => import('@/views/system/user/Index.vue')
  },
  {
    path: '/basic/department',
    name: 'BasicDepartmentManagement',
    meta: { label: '科室管理' },
    component: () => import('@/views/basic/department/Index.vue')
  }
]

export default routes
