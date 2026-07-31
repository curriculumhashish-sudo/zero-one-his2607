import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/basicdata/medication-catalog',
    name: 'BasicdataMedicationCatalog',
    meta: { label: '药品目录' },
    component: () => import('@/views/basicdata/Index.vue')
  },
  {
    path: '/basicdata/frequency',
    name: 'BasicdataFrequency',
    meta: { label: '频次管理' },
    component: () => import('@/views/basicdata/frequency/Index.vue')
  },
  {
    path: '/basicdata/medication',
    name: 'BasicdataMedication',
    meta: { label: '药品管理' },
    component: () => import('@/views/basicdata/medication/Index.vue')
  },
  {
    path: '/basicdata/diagnosis',
    name: 'BasicdataDiagnosis',
    meta: { label: '诊断管理' },
    component: () => import('@/views/basicdata/diagnosis/Index.vue')
  },
  {
    path: '/basicdata/treatment',
    name: 'BasicdataTreatment',
    meta: { label: '诊疗管理' },
    component: () => import('@/views/basicdata/treatment/Index.vue')
  },
  {
    path: '/basicdata/equipment',
    name: 'BasicdataEquipment',
    meta: { label: '器材管理' },
    component: () => import('@/views/basicdata/equipment/Index.vue')
  },
  {
    path: '/basicdata/sample',
    name: 'BasicdataSample',
    meta: { label: '样本管理' },
    component: () => import('@/views/basicdata/sample/Index.vue')
  }
]

export default routes
