import type {
  NurseStationOperationConfig,
  NurseStationOperationTab,
  NurseStationTaskFilter
} from './type'

/** 本项目当前启用的八个护士站业务模块。 */
export const NURSE_STATION_OPERATION_CONFIGS: readonly NurseStationOperationConfig[] = [
  {
    id: '入出转管理',
    description: '办理床位预约、出院及结算后清床。',
    selectionPolicy: 'single'
  },
  {
    id: '医嘱校对',
    description: '按患者批量查看、校对或退回住院医嘱。',
    selectionPolicy: 'multiple'
  },
  {
    id: '医嘱执行',
    description: '按计划时点批量执行、不执行或取消执行医嘱。',
    selectionPolicy: 'multiple'
  },
  {
    id: '汇总领药',
    description: '汇总选中患者的领药明细并维护汇总单。',
    selectionPolicy: 'multiple'
  },
  {
    id: '护理记录',
    description: '按护理时点录入并维护患者护理记录。',
    selectionPolicy: 'single'
  },
  {
    id: '体温单',
    description: '查看生命体征曲线并录入患者体温单。',
    selectionPolicy: 'single'
  },
  {
    id: '住院记账',
    description: '处理直接划价、医嘱补费和自动计费规则。',
    selectionPolicy: 'single'
  },
  {
    id: '费用明细',
    description: '查询当前住院患者的费用明细与费用汇总。',
    selectionPolicy: 'single'
  }
]

export const DEFAULT_NURSE_STATION_TAB: NurseStationOperationTab = '医嘱执行'

export const NURSE_STATION_TASK_FILTER_OPTIONS: readonly {
  label: string
  value: NurseStationTaskFilter
}[] = [
  { label: '全部状态', value: 'all' },
  { label: '待校对', value: 'pending-audit' },
  { label: '待执行', value: 'pending-execute' }
]

export function getNurseStationOperationConfig(tab: NurseStationOperationTab) {
  return NURSE_STATION_OPERATION_CONFIGS.find((item) => item.id === tab)
}
