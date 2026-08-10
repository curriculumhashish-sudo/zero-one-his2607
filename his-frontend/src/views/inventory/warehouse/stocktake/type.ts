// ==================== 表单列配置 ====================
// 表格列定义
export const FORM_COLUMNS = [
  { prop: 'itemName', label: '项目名称', width: 200, fixed: 'left' as const },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'manufacturer', label: '厂家', minWidth: 160 },
  { prop: 'batchNo', label: '批号', width: 120 },
  { prop: 'bookQuantity', label: '账面数量', width: 200 },
  { prop: 'actualQuantity', label: '实盘数量', width: 200 },
  { prop: 'profitLoss', label: '盈亏数量', width: 200 },
  { prop: 'unitPrice', label: '单价', width: 100 },
  { prop: 'actualAmount', label: '实盘金额', width: 100 },
  { prop: 'profitLossAmount', label: '盈亏金额', width: 100 },
  { prop: 'remark', label: '备注', width: 140 }
]

// 新增行默认值
export const ROW_DEFAULTS: Record<string, any> = {
  itemName: '',
  specification: '',
  manufacturer: '',
  batchNo: '',
  bookQuantity: '',
  actualQuantity: 0,
  actualUnit: '单位',
  profitLoss: 0,
  unitPrice: 0,
  actualAmount: 0,
  profitLossAmount: 0,
  remark: ''
}

// 弹窗列定义
export const SELECT_TABLE_COLUMNS = [
  { prop: 'itemCode', label: '项目编码', width: 100 },
  { prop: 'itemName', label: '项目名称', minWidth: 140 },
  { prop: 'manufacturer', label: '厂家/产地', minWidth: 200 },
  { prop: 'purchasePrice', label: '进货价', minWidth: 80 },
  { prop: 'retailPrice', label: '零售价', minWidth: 80 },
  { prop: 'packageUnit', label: '包装单位', width: 60 },
  { prop: 'specification', label: '规格', width: 120 },
  { prop: 'stockQuantity', label: '库存数量', width: 60 },
  { prop: 'approvalNo', label: '批准文号', width: 60 }
]
