// 使用 `import type` 表示这里只需要类型检查，不会产生运行时代码。
import type { DoctorQueueStatusOption, DoctorStationTabOption } from './type'
import type { StatusDisplayOption } from '@/components/common/type'

/**
 * 医生工作站的页签配置。
 *
 * 把固定配置放在 constants.ts 中，可以避免在 Vue 模板里重复写多个
 * el-tab-pane；以后增加新页签时，也能集中维护名称和顺序。
 */
export const DOCTOR_STATION_TABS: DoctorStationTabOption[] = [
  { key: 'record', label: '病历' },
  { key: 'diagnosis', label: '诊断' },
  { key: 'orders', label: '医嘱' },
  { key: 'tcmPrescription', label: '中医处方' }
]

/**
 * 左侧患者队列状态筛选项。
 * label 负责展示，value 负责接口查询和程序判断。
 */
export const DOCTOR_QUEUE_STATUS_OPTIONS: DoctorQueueStatusOption[] = [
  { label: '全部', value: 'ALL' },
  { label: '候诊', value: 'arrived' },
  { label: '在诊', value: 'in-progress' },
  { label: '完诊', value: 'finished' }
]

/**
 * 就诊状态展示配置。
 * 业务状态与颜色在页面模块中定义，不写入公共 StatusDisplay。
 */
export const DOCTOR_VISIT_STATUS_OPTIONS: StatusDisplayOption[] = [
  { value: 'arrived', label: '候诊', type: 'warning' },
  { value: 'in-progress', label: '在诊', type: 'primary' },
  { value: 'finished', label: '完诊', type: 'success' }
]
