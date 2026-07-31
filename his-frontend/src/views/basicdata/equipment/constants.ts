import { Delete, Download, Plus, Refresh, Upload } from '@element-plus/icons-vue'
import type { ActionToolbarItem, StatusDisplayOption } from '@/components/common/type'

/** 器材分类筛选选项（含"全部"） */
export const EQUIPMENT_CATEGORY_FILTER = [
  { label: '全部分类', value: '' },
  { label: '耗材', value: 'CONSUMABLE' },
  { label: '器械', value: 'INSTRUMENT' },
  { label: '设备', value: 'EQUIPMENT' }
]

/** 启用状态筛选 */
export const EQUIPMENT_ENABLED_OPTIONS = [
  { label: '全部状态', value: '' },
  { label: '启用', value: 'enabled' },
  { label: '停用', value: 'disabled' }
]

/** 列表状态标签映射 */
export const EQUIPMENT_STATUS_MAP: StatusDisplayOption[] = [
  { value: true, label: '启用', type: 'success' },
  { value: false, label: '停用', type: 'info' }
]

/** 工具栏操作 */
export const EQUIPMENT_TOOLBAR_ACTIONS: ActionToolbarItem[] = [
  { key: 'create', label: '新增器材', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh },
  { key: 'import', label: '导入', icon: Upload },
  { key: 'export', label: '导出', icon: Download },
  { key: 'batchEnable', label: '批量启用', type: 'success', confirmText: '确认批量启用选中器材？' },
  { key: 'batchDisable', label: '批量停用', type: 'warning', confirmText: '确认批量停用选中器材？' },
  { key: 'batchDelete', label: '批量删除', type: 'danger', confirmText: '确认批量删除选中器材？删除后无法恢复。' }
]
