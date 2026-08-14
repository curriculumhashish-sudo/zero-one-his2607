import { Plus, Refresh } from '@element-plus/icons-vue'
import type { ActionToolbarItem, StatusDisplayOption } from '@/components/common/type'

export const DEPARTMENT_TYPE_OPTIONS = [
  { label: '全部类型', value: '' },
  { label: '医院', value: 'HOSPITAL' },
  { label: '科室', value: 'DEPARTMENT' }
]

export const DEPARTMENT_CATEGORY_OPTIONS = [
  { label: '全部类别', value: '' },
  { label: '门诊', value: 'OUTPATIENT' },
  { label: '住院', value: 'INPATIENT' },
  { label: '药房', value: 'PHARMACY' },
  { label: '库房', value: 'WAREHOUSE' },
  { label: '财务', value: 'FINANCE' },
  { label: '护士站', value: 'NURSE_STATION' },
  { label: '管理', value: 'MANAGEMENT' },
  { label: '后勤支持', value: 'LOGISTICS_SUPPORT' }
]

export const DEPARTMENT_ENABLED_OPTIONS = [
  { label: '全部状态', value: '' },
  { label: '启用', value: 'enabled' },
  { label: '停用', value: 'disabled' }
]

export const DEPARTMENT_STATUS_MAP: StatusDisplayOption[] = [
  { value: 'enabled', label: '启用', type: 'success' },
  { value: 'disabled', label: '停用', type: 'info' }
]

export const DEPARTMENT_TOOLBAR_ACTIONS: ActionToolbarItem[] = [
  { key: 'create', label: '新增顶级科室', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh }
]
