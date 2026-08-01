import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/report/outpatient-fee-detail',
    name: 'ReportOutpatientFeeDetail',
    meta: { label: '门诊费用明细' },
    component: () => import('@/views/report/outpatient-fee-detail/Index.vue')
  },
  {
    path: '/report/data-statistics',
    name: 'ReportDataStatistics',
    meta: { label: '数据统计' },
    component: () => import('@/views/report/data-statistics/Index.vue')
  }
]

export default routes
