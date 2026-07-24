import { Plus, Refresh } from '@element-plus/icons-vue'
import type { ActionToolbarItem, StatusDisplayOption } from '@/components/common/type'

export const DEPARTMENT_TYPE_OPTIONS = [
  { label: '全部类型', value: '' },
  { label: '医院', value: '医院' },
  { label: '临床科室', value: '临床科室' },
  { label: '医技科室', value: '医技科室' },
  { label: '行政科室', value: '行政科室' }
]

export const DEPARTMENT_CATEGORY_OPTIONS = [
  { label: '全部类别', value: '' },
  { label: '门诊', value: '门诊' },
  { label: '住院', value: '住院' },
  { label: '医技', value: '医技' },
  { label: '职能', value: '职能' }
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
