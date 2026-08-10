/** 盘点单状态: 0=编辑中 1=待审核 2=审核中 3=已撤回 4=同意 5=驳回 */
export type PurchaseInboundStatus = 0 | 1 | 2 | 3 | 4 | 5

/** 当前页面沿用原目录名，实际展示的是药房盘点单。 */
export interface PurchaseInboundOrder {
  id: string
  /** 0=编辑中 1=待审核 2=审核中 3=已撤回 4=同意 5=驳回 */
  status: PurchaseInboundStatus
  /** 药品或耗材。 */
  category: string
  rejectReason?: string

  // ==================== 卡片队列展示字段（与 DocumentQueueList 对齐） ====================

  /** 单据号（卡片头部主标识，多字段 fallback 的第一个候选） */
  busNo?: string
  /** 状态枚举值（用于审批判定） */
  statusEnum?: number | string
  /** 状态展示文本 */
  statusEnum_enumText?: string
  /** 制单人展示名 */
  applicantId_dictText?: string
  /** 制单时间 */
  applyTime?: string
  /** 审批人展示名 */
  approverId_dictText?: string
  /** 审批时间 */
  approvalTime?: string
  /** 供应商展示名（showSupplier=true 时显示） */
  supplierId_dictText?: string
  /** 源库房ID（调拨出库方向） */
  sourceLocationId?: string
  /** 源库房展示名（调拨出库方向） */
  sourceLocationId_dictText?: string
  /** 目标库房ID（调拨入库方向） */
  purposeLocationId?: string
  /** 目标库房展示名（调拨入库方向） */
  purposeLocationId_dictText?: string
  /** 总金额 */
  totalAmount?: number
  /** 草稿标记 */
  _draft?: boolean
  /** 草稿客户端唯一标识 */
  _clientDraftId?: string
}

// 药房盘点列表分页查询请求参数
export interface PurchaseInboundQuery {
  checkNo?: string // 盘点单号
  pageSize: number // 每页显示条数
  checkType?: string // 盘点类型
  startDate?: string // 开始时间
  endDate?: string // 结束时间
  pageIndex: number // 当前页码
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
  /** 保存后的目标状态（编辑中→待审核=1，保存并提交→审核中=2） */
  targetStatus?: PurchaseInboundStatus
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
  selectTableColumns: {
    prop: string
    label: string
    width?: number | string
    minWidth?: number | string
  }[]
}

/** 药品选择表格数据源。 */
export interface SupplierItem extends Record<string, unknown> {
  itemCode: string
  itemName: string
  manufacturer: string
  purchasePrice: number
  retailPrice: number
  packageUnit: string
  /** 单位编码（如 "1"=盒），后端 SelectTable API 返回 */
  unitCode?: string
  specification: string
  stockQuantity: number
  approvalNo: string
}
