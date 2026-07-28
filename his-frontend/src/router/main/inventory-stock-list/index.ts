import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/inventory/stock-list',
    name: 'InventoryStockList',
    meta: { label: '库存明细' },
    component: () => import('@/views/inventory/stock-list/Index.vue')
  }
]

export default routes
