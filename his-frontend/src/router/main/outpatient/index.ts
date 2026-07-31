// `import type` 只导入 TypeScript 类型，打包后的 JavaScript 中不会包含它。
// RouteRecordRaw 是 Vue Router 提供的标准路由配置类型。
import type { RouteRecordRaw } from 'vue-router'

/**
 * E 组负责的门诊模块路由。
 *
 * 项目的主路由会自动读取 `src/router/main/各模块/index.ts`，
 * 所以这里只需要导出本模块的路由，不需要修改公共主路由文件。
 */
const routes: RouteRecordRaw[] = [
  {
    // 浏览器访问地址。
    path: '/outpatient/doctor-station',
    // 路由的唯一名称，可用于 router.push({ name: 'OutpatientDoctorStation' })。
    name: 'OutpatientDoctorStation',
    meta: {
      // 页面显示名称，菜单或标签页可以读取这个字段。
      label: '门诊医生工作站'
    },
    // 使用懒加载：只有用户真正打开医生站时，才下载并加载这个页面。
    component: () => import('@/views/outpatient/doctor-station/Index.vue')
  },
  {
    path: '/outpatient/nurse-station',
    name: 'OutpatientNurseStation',
    meta: {
      label: '门诊护士站'
    },
    component: () => import('@/views/outpatient/nurse-station/Index.vue')
  }
]

// 主路由自动加载器读取的是默认导出，因此这里必须使用 export default。
export default routes
