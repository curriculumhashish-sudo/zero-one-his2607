import type { RouteRecordRawArray } from '@/types/Route'
import type { RouteRecordRaw } from 'vue-router'

const routes = [
  {
    path: '/home',
    name: 'Home',
    redirect: { name: 'Dashboard' },
    component: () => import('@/views/HomeView.vue'),
    children: [
      {
        path: '/dashboard',
        name: 'Dashboard',
        meta: { label: '工作台' },
        component: () => import('@/views/dashboard/Dashboard.vue')
      },
      {
        // 复用现有首页看板作为当前阶段的工作台入口，后续独立工作台完成后再替换组件。
        path: '/workbench',
        name: 'Workbench',
        meta: { label: '工作台' },
        component: () => import('@/views/dashboard/Dashboard.vue')
      }
    ] as RouteRecordRaw[]
  }
]

// 读取子目录下面菜单设置为二级路由
const secondRouter = import.meta.glob<RouteRecordRawArray>('./**/index.ts', { eager: true })
for (const path in secondRouter) {
  const route = routes[0]
  const children = secondRouter[path]?.default
  if (route?.children && children) {
    route.children.push(...children)
  }
}

export default routes
