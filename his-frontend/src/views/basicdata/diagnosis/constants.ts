import { Refresh } from '@element-plus/icons-vue'
import type { ActionToolbarItem } from '@/components/common/type'

/** 只读诊断字典仅提供刷新操作。 */
export const DIAGNOSIS_TOOLBAR_ACTIONS: ActionToolbarItem[] = [
  { key: 'refresh', label: '刷新', icon: Refresh }
]
