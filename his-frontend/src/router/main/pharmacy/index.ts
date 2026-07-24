import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/pharmacy/outpatient-dispense',
    name: 'PharmacyOutpatientDispense',
    meta: { label: '门诊发药' },
    component: () => import('@/views/pharmacy/outpatient-dispense/Index.vue')
  }
]

export default routes