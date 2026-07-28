import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/inventory/pharmacy/stock-in',
    name: 'PharmacyStockIn',
    meta: { label: '药房入库' },
    component: () => import('@/views/inventory/pharmacy/stock-in/Index.vue')
  },
  {
    path: '/inventory/pharmacy/stocktake',
    name: 'PharmacyStocktake',
    meta: { label: '药房盘点' },
    component: () => import('@/views/inventory/pharmacy/stocktake/Index.vue')
  },
  {
    path: '/inventory/warehouse/purchase-inbound2',
    name: 'WarehousePurchaseInbound2',
    meta: { label: '药库进货' },
    component: () => import('@/views/inventory/warehouse/purchase-inbound2/Index.vue')
  },
  {
    path: '/inventory/warehouse/stock-out',
    name: 'WarehouseStockOut',
    meta: { label: '药库出库' },
    component: () => import('@/views/inventory/warehouse/stock-out/Index.vue')
  },
  {
    path: '/inventory/warehouse/stocktake',
    name: 'WarehouseStocktake',
    meta: { label: '药库盘点' },
    component: () => import('@/views/inventory/warehouse/stocktake/Index.vue')
  }
]

export default routes
