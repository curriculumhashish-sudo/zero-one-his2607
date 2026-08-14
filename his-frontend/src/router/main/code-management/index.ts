import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/basic/code',
    name: 'BasicCodeManagement',
    meta: { label: '编码管理' },
    component: () => import('@/views/basic/code/Index.vue')
  }
]

export default routes
