
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'

/** 患者状态映射（用于 PatientQueuePanel） */
export const PATIENT_STATUS_OPTIONS = [
  { key: 'pending', label: '待发药' },
  { key: 'completed', label: '已发药' },
]

/** 处方状态映射（用于 AdviceOrderTable） */
export const PRESCRIPTION_STATUS_MAP: Record<string, string> = {
  pending: '待发药',
  partial: '部分可发',
  completed: '已发药',
  blocking: '拦截',
}

/** 处方状态标签样式 */
export const PRESCRIPTION_STATUS_TAG = {
  pending: { type: 'warning' as const, text: '待发药' },
  partial: { type: 'warning' as const, text: '部分可发' },
  completed: { type: 'success' as const, text: '已发药' },
  blocking: { type: 'danger' as const, text: '拦截' },
}

/** AdviceOrderTable 列配置 */
export const ORDER_COLUMNS = [
  { prop: 'itemName', label: '药品名称', minWidth: 140 },
  { prop: 'spec', label: '规格', width: 120 },
  { prop: 'dosage', label: '单次剂量', width: 100 },
  { prop: 'route', label: '途径', width: 80 },
  { prop: 'frequency', label: '频次', width: 80 },
  { prop: 'days', label: '天数', width: 60 },
  { prop: 'quantity', label: '数量', width: 80 },
]

/** 处方汇总 DescriptionList 配置 */
export const PRESCRIPTION_SUMMARY_ITEMS: DescriptionItem[] = [
  { prop: 'prescriptionCount', label: '处方数' },
  { prop: 'drugCount', label: '药品总数' },
  {
    prop: 'totalAmount',
    label: '总金额',
    formatter: (val: number) => `¥${val.toFixed(2)}`,
  },
  { prop: 'dispenserName', label: '调配药师' },
]

/** 工具栏操作配置 */
export const TOOLBAR_ACTIONS = [
  { key: 'refresh', label: '刷新', icon: 'Refresh' },
  { key: 'trace', label: '录入追溯码', icon: 'Scan' },
  { key: 'view', label: '处方查看', icon: 'Document' },
  { key: 'dispense', label: '确认发药', type: 'primary' as const, icon: 'Check' },
]