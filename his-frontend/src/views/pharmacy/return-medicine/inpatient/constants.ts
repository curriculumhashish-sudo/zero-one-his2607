export const INPATIENT_RETURN_COLUMNS = [
  { prop: 'itemName', label: '药品名称', minWidth: 160 },
  { prop: 'spec', label: '规格', width: 120 },
  { prop: 'quantity', label: '实退数量', width: 100 },
  { prop: 'unit', label: '单位', width: 60 },
  { prop: 'batchNo', label: '批次号', width: 100 },
  { prop: 'totalPrice', label: '金额', width: 100 },
  { prop: 'pharmacy', label: '发放库房', width: 150 },
  { prop: 'traceCode', label: '追溯码', width: 120 },
  { prop: 'factory', label: '厂家', minWidth: 160 },
  { prop: 'status', label: '状态', width: 100 }
]

export const INPATIENT_RETURN_STATUS_MAP: Record<string, string> = {
  'refundable': '可退',
  'partially-refundable': '部分可退',
  'non-refundable': '不可退',
  'refunded': '已退药'
}

export const INPATIENT_RETURN_STATUS_TAG_TYPE: Record<string, string> = {
  'refundable': 'success',
  'partially-refundable': 'warning',
  'non-refundable': 'danger',
  'refunded': 'info'
}
