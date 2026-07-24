import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/warehouse/purchase-inbound',
    name: 'WarehousePurchaseInbound',
    meta: { label: '采购入库' },
    component: () => import('@/views/warehouse/purchase-inbound/Index.vue')
  }
]

export default routes
