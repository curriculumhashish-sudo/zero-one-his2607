/** 采购入库单 */
export interface PurchaseInboundOrder {
  id: string
  /** 单据号 */
  docNo: string
  /** 日期 */
  date: string
  /** 经办人 */
  handler: string
  /** 往来单位 */
  counterparty: string
  /** 状态 */
  status: number
  /** 类型 */
  type?: string
  /** 类别 */
  category?: string
}

/** 采购入库单查询参数 */
export interface PurchaseInboundQuery {
  keyword?: string
  dateRange?: [string, string] | null
  type?: string | number
  category?: string | number
}

/** 分页结果 */
export interface PageResult<T> {
  rows: T[]
  total: number
}

/** 采购入库明细行 */
export interface PurchaseInboundDetail {
  id: string
  orderId: string
  drugCode: string
  drugName: string
  specification: string
  unit: string
  quantity: number
  unitPrice: number
  amount: number
  batchNo: string
  expiryDate: string
}

/** 供应商/药品选项（SelectTable 数据源） */
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
