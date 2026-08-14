import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/inpatient/charge/register',
    name: 'InpatientChargeRegister',
    meta: { label: '住院登记' },
    component: () => import('@/views/inpatient/charge/register/Index.vue')
  },
  {
    path: '/inpatient/charge/settle',
    name: 'InpatientChargeSettle',
    meta: { label: '住院结算' },
    component: () => import('@/views/inpatient/charge/settle/Index.vue')
  },
  {
    path: '/outpatient/charge/register',
    name: 'OutpatientChargeRegister',
    meta: { label: '门诊挂号' },
    component: () => import('@/views/outpatient/charge/register/Index.vue')
  }
]

export default routes
