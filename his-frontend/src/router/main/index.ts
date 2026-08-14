import type { RouteRecordRawArray } from '@/types/Route'
import type { RouteRecordRaw } from 'vue-router'

const routes = [
  {
    path: '/home',
    name: 'ApplicationLayout',
    redirect: { path: '/workbench' },
    component: () => import('@/views/HomeView.vue'),
    children: [] as RouteRecordRaw[]
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
