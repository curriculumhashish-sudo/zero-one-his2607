import type { CommonComponentKey } from '@/components/common/type'

/**
 * 医生工作站顶部页签的公共数据结构。
 *
 * 公共组件只关心“唯一值、显示文字、是否禁用”，不会理解
 * “病历”“医嘱”或“长期医嘱”等具体业务含义。
 */
export interface DoctorStationTabItem {
  key: CommonComponentKey
  label: string
  disabled?: boolean
}
