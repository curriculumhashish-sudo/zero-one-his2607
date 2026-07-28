export const ORDER_COLUMNS = [
  { prop: 'rxNo', label: '处方号', width: 160, fixed: 'left' as const },
  { prop: 'status', label: '状态', width: 80, fixed: 'left' as const },
  { prop: 'itemName', label: '药品名称', minWidth: 160 },
  { prop: 'quantity', label: '数量', width: 80 },
  { prop: 'spec', label: '规格', width: 120 },
  { prop: 'batchNo', label: '批次号', width: 100 },
  { prop: 'totalPrice', label: '金额', width: 100 },
  { prop: 'pharmacy', label: '发放库房', width: 150 },
  { prop: 'traceCode', label: '追溯码', width: 120 },
  { prop: 'factory', label: '厂家', minWidth: 160 }
]

export const SUMMARY_COLUMNS = [
  { prop: 'summaryNo', label: '汇总单号', width: 160 },
  { prop: 'status', label: '状态', width: 80 },
  { prop: 'receiverName', label: '领药人', width: 100 },
  { prop: 'createTime', label: '创建时间', width: 160 }
]

export const DISPENSE_STATUS_MAP: Record<string, string> = {
  pending: '待发药',
  partial: '部分可发',
  completed: '已发药',
  blocking: '拦截'
}

export const SUMMARY_STATUS_MAP: Record<string, string> = {
  pending: '待处理',
  completed: '已发药',
  rejected: '已驳回'
}

export const DISPENSE_MODE_OPTIONS = [
  { value: 'patient', label: '按患者发药' },
  { value: 'summary', label: '汇总发药' }
]
