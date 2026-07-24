import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/booking/outpatient-schedule',
    name: 'BookingOutpatientSchedule',
    meta: { label: '门诊排班' },
    component: () => import('@/views/booking/outpatient-schedule/Index.vue')
  },
  {
    path: '/booking/slot-calendar',
    name: 'BookingSlotCalendar',
    meta: { label: '号源查看' },
    component: () => import('@/views/booking/slot-calendar/Index.vue')
  }
]

export default routes
