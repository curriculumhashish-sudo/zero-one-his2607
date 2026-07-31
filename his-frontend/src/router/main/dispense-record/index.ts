import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/pharmacy/dispense-record/outpatient',
    name: 'PharmacyOutpatientDispenseRecord',
    meta: { label: '门诊发退药记录', anonymous: true },
    component: () => import('@/views/pharmacy/dispense-record/outpatient/Index.vue')
  },
  {
    path: '/pharmacy/dispense-record/inpatient',
    name: 'PharmacyInpatientDispenseRecord',
    meta: { label: '住院发退药记录', anonymous: true },
    component: () => import('@/views/pharmacy/dispense-record/inpatient/Index.vue')
  }
]

export default routes