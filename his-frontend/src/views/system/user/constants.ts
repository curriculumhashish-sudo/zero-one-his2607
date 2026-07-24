import { Plus, Refresh } from '@element-plus/icons-vue'
import type { ActionToolbarItem, StatusDisplayOption } from '@/components/common/type'

export const USER_ENABLED_OPTIONS = [
  { label: '全部状态', value: '' },
  { label: '启用', value: 'enabled' },
  { label: '停用', value: 'disabled' }
]

export const USER_LOCKED_OPTIONS = [
  { label: '全部', value: '' },
  { label: '已锁定', value: 'locked' },
  { label: '未锁定', value: 'unlocked' }
]

export const USER_STATUS_MAP: StatusDisplayOption[] = [
  { value: 'enabled', label: '启用', type: 'success' },
  { value: 'disabled', label: '停用', type: 'info' }
]

export const USER_LOCK_MAP: StatusDisplayOption[] = [
  { value: 'locked', label: '已锁定', type: 'danger' },
  { value: 'unlocked', label: '未锁定', type: 'success' }
]

export const USER_TOOLBAR_ACTIONS: ActionToolbarItem[] = [
  { key: 'create', label: '新增用户', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh }
]
