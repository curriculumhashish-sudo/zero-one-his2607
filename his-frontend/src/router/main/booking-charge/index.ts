import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/booking/schedule-template',
    name: 'BookingScheduleTemplate',
    meta: { label: '排班设置' },
    component: () => import('@/views/booking/schedule-template/Index.vue')
  },
  {
    path: '/outpatient/charge/charge',
    name: 'OutpatientCharge',
    meta: { label: '门诊收费' },
    component: () => import('@/views/outpatient/charge/charge/Index.vue')
  },
  {
    path: '/outpatient/charge/refund',
    name: 'OutpatientChargeRefund',
    meta: { label: '门诊退费' },
    component: () => import('@/views/outpatient/charge/refund/Index.vue')
  },
  {
    path: '/inpatient/charge/prepay',
    name: 'InpatientPrepay',
    meta: { label: '预交金管理' },
    component: () => import('@/views/inpatient/charge/prepay/Index.vue')
  }
]

export default routes
