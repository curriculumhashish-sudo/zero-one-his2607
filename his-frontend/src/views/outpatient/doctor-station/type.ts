import type { DoctorVisitStatus } from '@/apis/outpatient/doctor-station'

/**
 * 医生工作站中间区域允许出现的页签。
 *
 * 使用联合类型后，如果误写成 `activeTab = 'test'`，
 * TypeScript 会在开发阶段直接提示错误。
 */
export type DoctorStationTab = 'record' | 'diagnosis' | 'orders'

/**
 * 顶部工具栏允许发出的业务动作。
 * 子组件只告诉父页面“用户点了什么”，真正的接口调用由 Index.vue 处理。
 */
export type DoctorStationAction = 'accept' | 'save' | 'refresh' | 'leave' | 'complete'

/**
 * 单个页签的显示配置。
 * key 用于程序判断，label 用于展示给用户。
 */
export interface DoctorStationTabOption {
  key: DoctorStationTab
  label: string
}

/**
 * 左侧患者队列的状态筛选值。
 * ALL 只存在于前端；请求接口时会转换为 undefined。
 */
export type DoctorQueueStatusFilter = 'ALL' | DoctorVisitStatus

/**
 * 状态筛选组件使用的单个选项。
 */
export interface DoctorQueueStatusOption {
  label: string
  value: DoctorQueueStatusFilter
}
