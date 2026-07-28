import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/inpatient/doctor-station',
    name: 'InpatientDoctorStation',
    meta: {
      label: '住院医生工作站'
    },
    component: () => import('@/views/inpatient/doctor-station/Index.vue')
  }
]

export default routes
