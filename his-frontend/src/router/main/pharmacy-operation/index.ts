import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  { path: '/pharmacy/dispense/outpatient', name: 'OutpatientDispense', meta: { label: '门诊发药', anonymous: true }, component: () => import('@/views/pharmacy/dispense/outpatient/Index.vue') },
  { path: '/pharmacy/dispense/inpatient', name: 'InpatientDispense', meta: { label: '住院发药' }, component: () => import('@/views/pharmacy/dispense/inpatient/Index.vue') },
  { path: '/pharmacy/return-medicine/outpatient', name: 'OutpatientReturnMedicine', meta: { label: '门诊退药', anonymous: true }, component: () => import('@/views/pharmacy/return-medicine/outpatient/Index.vue') },
  { path: '/pharmacy/return-medicine/inpatient', name: 'InpatientReturnMedicine', meta: { label: '住院退药' }, component: () => import('@/views/pharmacy/return-medicine/inpatient/Index.vue') }
]

export default routes
