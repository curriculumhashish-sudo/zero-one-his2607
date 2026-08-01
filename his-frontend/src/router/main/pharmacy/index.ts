import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/pharmacy/outpatient-dispense',
    name: 'PharmacyOutpatientDispense',
    meta: { label: '门诊发药', anonymous: true },
    component: () => import('@/views/pharmacy/dispense/outpatient/Index.vue')
  }
]

export default routes
