import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/pharmacy/outpatient-dispense',
    name: 'PharmacyOutpatientDispense',
    meta: { label: '门诊发药', anonymous: true },
<<<<<<< HEAD
    component: () => import('@/views/pharmacy/outpatient-dispense/Index.vue')
=======
    component: () => import('@/views/pharmacy/dispense/outpatient/Index.vue')
>>>>>>> f1-syd-D
  }
]

export default routes
