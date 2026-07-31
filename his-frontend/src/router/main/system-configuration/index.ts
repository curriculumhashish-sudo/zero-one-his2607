import type { RouteRecordRaw } from 'vue-router'

const routes: RouteRecordRaw[] = [
  {
    path: '/system/permission',
    name: 'SystemPermissionManagement',
    meta: { label: '权限管理' },
    component: () => import('@/views/system/permission/Index.vue')
  },
  {
    path: '/system/menu',
    name: 'SystemMenuManagement',
    meta: { label: '菜单管理' },
    component: () => import('@/views/system/menu/Index.vue')
  },
  {
    path: '/system/role',
    name: 'SystemRoleManagement',
    meta: { label: '角色管理' },
    component: () => import('@/views/system/role/Index.vue')
  },
  {
    path: '/configuration/emr-template',
    name: 'ConfigurationEmrTemplate',
    meta: { label: '病历模板配置' },
    component: () => import('@/views/configuration/emr-template/Index.vue')
  },
  {
    path: '/configuration/execute-location',
    name: 'ConfigurationExecuteLocation',
    meta: { label: '执行地点' },
    component: () => import('@/views/configuration/execute-location/Index.vue')
  },
  {
    path: '/configuration/order-template',
    name: 'ConfigurationOrderTemplate',
    meta: { label: '组套模板' },
    component: () => import('@/views/configuration/order-template/Index.vue')
  },
  {
    path: '/configuration/medication-location',
    name: 'ConfigurationMedicationLocation',
    meta: { label: '取药科室' },
    component: () => import('@/views/configuration/medication-location/Index.vue')
  },
  {
    path: '/configuration/service-device-binding',
    name: 'ConfigurationServiceDeviceBinding',
    meta: { label: '诊疗耗材绑定' },
    component: () => import('@/views/configuration/service-device-binding/Index.vue')
  }
]

export default routes
