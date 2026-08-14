/**
 * 指标趋势方向
 */
export type TrendType = 'up' | 'down'

/**
 * 顶部核心指标卡片数据模型（用于 StatCard）
 */
export interface StatCardItem {
  /** 指标名称 */
  label: string
  /** 目标值 */
  value: number
  /** 动画展示值（从 0 增长到 value） */
  displayValue: number
  /** 单位（如 %） */
  unit: string
  /** 趋势文案（如 12%） */
  trendValue: string
  /** 趋势类型 */
  trendType: TrendType
  /** Element Plus 图标组件 */
  icon: any
  /** 图标/主色 */
  color: string
  /** 图标背景色 */
  bgColor: string
}

/**
 * 底部资源监控数据模型（用于 ResourceMonitor）
 */
export interface ResourceItem {
  /** 资源名称（CPU/内存/磁盘/网络等） */
  name: string
  /** 目标百分比（0-100） */
  percentage: number
  /** 动画展示值（从 0 增长到 percentage） */
  displayValue: number
  /** 展示颜色 */
  color: string
  /** Element Plus 图标组件 */
  icon: any
}

/**
 * 系统动态条目数据模型（用于 SystemTimeline）
 */
export interface Activity {
  /** 标题 */
  title: string
  /** 内容描述 */
  content: string
  /** 时间文案 */
  time: string
  /** 时间轴类型 */
  type: 'primary' | 'success' | 'warning' | 'danger' | 'info'
  /** 自定义颜色 */
  color: string
}

/**
 * 常用功能入口数据模型（用于 FeatureGrid）
 */
export interface FeatureItem {
  /** 功能名称 */
  name: string
  /** 功能描述 */
  desc: string
  /** Element Plus 图标组件 */
  icon: any
  /** 图标背景色 */
  bgColor: string
  /** 路由路径 */
  path: string
  /** 是否允许点击跳转 */
  navigable?: boolean
}
