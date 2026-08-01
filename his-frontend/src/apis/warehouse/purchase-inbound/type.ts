export type PurchaseInboundStatus = 0 | 1 | 2 | 3

/** 当前页面沿用原目录名，实际展示的是药房盘点单。 */
export interface PurchaseInboundOrder {
  id: string
  docNo: string
  date: string
  handler: string
  counterparty: string
  /** 0 编辑中、1 待审批、2 已完成、3 已驳回。 */
  status: PurchaseInboundStatus
  /** 当前盘点药房。 */
  type: string
  /** 药品或耗材。 */
  category: string
  rejectReason?: string
}

export interface PurchaseInboundQuery {
  keyword?: string
  dateRange?: [string, string] | null
  type?: string | number
  category?: string | number
}

export interface PageResult<T> {
  rows: T[]
  total: number
}

/** 药房盘点明细。金额由数量与单价计算，不接收页面写死结果。 */
export interface PurchaseInboundDetail {
  id: string
  itemCode: string
  itemName: string
  specification: string
  manufacturer: string
  batchNo: string
  bookQuantity: number
  actualQuantity: number
  actualUnit: string
  profitLoss: number
  unitPrice: number
  actualAmount: number
  profitLossAmount: number
  remark: string
}

export interface PurchaseInboundSavePayload {
  pharmacy: string
  itemType: string
  details: PurchaseInboundDetail[]
}

export interface PurchaseInboundDetailResult {
  order: PurchaseInboundOrder
  details: PurchaseInboundDetail[]
}

/** 表单列配置（由父组件传入） */
export interface FormColumnConfig {
  prop: string
  label: string
  width?: number | string
  minWidth?: number | string
  fixed?: 'left' | 'right'
  editable?: boolean
  editorType?: 'input' | 'number' | 'select' | 'date'
  editorProps?: Record<string, any>
}

/** 表单配置（mock 接口返回） */
export interface FormConfig {
  columns: FormColumnConfig[]
  rowDefaults: Record<string, any>
  selectTableColumns: { prop: string; label: string; width?: number | string; minWidth?: number | string }[]
}

/** 药品选择表格数据源。 */
export interface SupplierItem extends Record<string, unknown> {
  itemCode: string
  itemName: string
  manufacturer: string
  purchasePrice: number
  retailPrice: number
  packageUnit: string
  specification: string
  stockQuantity: number
  approvalNo: string
}
