import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/basic/patient',
    name: 'BasicPatientManagement',
    meta: { label: '患者管理' },
    component: () => import('@/views/basic/patient/Index.vue')
  },
  {
    path: '/basic/ward-bed',
    name: 'BasicWardBedManagement',
    meta: { label: '病区病房' },
    component: () => import('@/views/basic/ward-bed/Index.vue')
  },
  {
    path: '/basic/warehouse-pharmacy',
    name: 'BasicWarehousePharmacyManagement',
    meta: { label: '库房药房' },
    component: () => import('@/views/basic/warehouse-pharmacy/Index.vue')
  },
  {
    path: '/basic/supplier',
    name: 'BasicSupplierManagement',
    meta: { label: '供应商管理' },
    component: () => import('@/views/basic/supplier/Index.vue')
  },
  {
    path: '/basic/charge-type',
    name: 'BasicChargeTypeManagement',
    meta: { label: '费用性质' },
    component: () => import('@/views/basic/charge-type/Index.vue')
  },
  {
    path: '/basic/registration-type',
    name: 'BasicRegistrationTypeManagement',
    meta: { label: '挂号类型' },
    component: () => import('@/views/basic/registration-type/Index.vue')
  }
]

export default routes
