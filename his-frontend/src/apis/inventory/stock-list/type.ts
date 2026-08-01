/** 库存明细行 */
export interface StockListRow {
  id: string
  productBusNo: string // 编码
  name: string // 项目名称
  merchandiseName: string // 商品名称
  categoryLabel: string // 项目类别
  lotNumber: string // 批号
  packageStockText: string // 库存(包装)
  splitStockText: string // 库存(最小)
  packageUnitRatio: number // 拆零换算比
  warehouseName: string // 库房
  totalVolume: string // 规格
  manufacturer: string // 厂家
  supplierName: string // 供应商
  doseFormLabel: string // 剂型
  approvalNumber: string | null // 国药准字
  ybNo: string | null // 医保码
  expirationDate: string // 效期
  productionDate: string // 生产日期
  remainingDays: number | null // 剩余天数
  inventoryStatusLabel: string // 供应状态
  inventoryStatusEnum: string // 供应状态码 ACTIVE|INACTIVE
  chrgitmLvLabel: string | null // 医保等级
  lotCostPrice: number // 进价
  lotSalePrice: number // 售价
  lineCostTotal: number // 进价总额
  linePriceTotal: number // 售价总额
  traceNo: string | null // 追溯码
  /** mock 内部过滤用 */
  medicationCategoryCode?: string
  deviceCategoryCode?: string
}

/** 库存明细查询参数 */
export interface StockListQuery {
  pageNo: number
  pageSize: number
  keyword?: string
  warehouseNames?: string[]
  supplierNames?: string[]
  expireWithinDays?: number
  medicationCategoryCodes?: string[]
  deviceCategoryCodes?: string[]
  inventoryStatusLabels?: string[]
  chrgitmLvLabels?: string[]
  zeroStockOnly?: boolean // true=仅零库存，false=仅非零库存
}

/** 分页结果 */
export interface PageResult<T> {
  rows: T[]
  total: number
}
