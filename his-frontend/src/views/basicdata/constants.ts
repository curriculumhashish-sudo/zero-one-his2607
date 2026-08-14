import { Delete, Download, Plus, Refresh, Upload } from '@element-plus/icons-vue'
import type { ActionToolbarItem, StatusDisplayOption } from '@/components/common/type'

/** 药品分类筛选选项（含"全部"） */
export const MEDICATION_CATEGORY_FILTER = [
  { label: '全部分类', value: '' },
  { label: '西药', value: 'WM' },
  { label: '中成药', value: 'TCM' },
  { label: '中草药', value: 'HM' },
  { label: '生物制品', value: 'BP' },
  { label: '原料药', value: 'RM' }
]

/** 启用状态筛选 */
export const MEDICATION_ENABLED_OPTIONS = [
  { label: '全部状态', value: '' },
  { label: '启用', value: 'enabled' },
  { label: '停用', value: 'disabled' }
]

/** 列表状态标签映射 */
export const MEDICATION_STATUS_MAP: StatusDisplayOption[] = [
  { value: true, label: '启用', type: 'success' },
  { value: false, label: '停用', type: 'info' }
]

/** 工具栏操作 */
export const MEDICATION_TOOLBAR_ACTIONS: ActionToolbarItem[] = [
  { key: 'create', label: '新增药品', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh },
  { key: 'import', label: '导入', icon: Upload },
  { key: 'export', label: '导出', icon: Download },
  { key: 'batchEnable', label: '批量启用', type: 'success', confirmText: '确认批量启用选中药品？' },
  { key: 'batchDisable', label: '批量停用', type: 'warning', confirmText: '确认批量停用选中药品？' },
  { key: 'batchDelete', label: '批量删除', type: 'danger', confirmText: '确认批量删除选中药品？删除后无法恢复。' }
]
