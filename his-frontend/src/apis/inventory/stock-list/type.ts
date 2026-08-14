/** 后端 InventoryDetailVO 原始响应行 */
export interface RawInventoryDetailVO {
  id: string
  itemTable: string
  itemId: string
  chargeItemDefinitionId: string
  productBusNo: string
  name: string
  merchandiseName: string
  totalVolume: number
  manufacturer: string
  specification: string
  approvalNumber: string
  ybNo: string
  traceNo: string
  pyStr: string
  wbStr: string
  lotNumber: string
  warehouseId: string
  warehouseName: string
  supplierId: string
  supplierName: string
  productionDate: string
  expirationDate: string
  remainingDays: number
  inventoryStatusEnum: string
  inventoryStatusLabel: string
  chrgitmLv: string
  chrgitmLvLabel: string
  medicationCategoryCode: string
  deviceCategoryCode: string
  categoryLabel: string
  doseFormCode: string
  doseFormLabel: string
  packageUnitCode: string
  packageUnitName: string
  splitUnitName: string
  packageUnitRatio: number
  packageStockText: string
  splitStockText: string
  lotCostPrice: number
  lotSalePrice: number
  lineCostTotal: number
  linePriceTotal: number
}

/** 库存明细行（前端展示） */
export interface StockListRow {
  id: string
  productBusNo: string
  name: string
  merchandiseName: string
  categoryLabel: string
  lotNumber: string
  packageStockText: string
  splitStockText: string
  packageUnitRatio: number
  warehouseId?: string
  warehouseName: string
  totalVolume: string
  manufacturer: string
  supplierId?: string
  supplierName: string
  doseFormLabel: string
  approvalNumber: string | null
  ybNo: string | null
  expirationDate: string
  productionDate: string
  remainingDays: number | null
  inventoryStatusLabel: string
  inventoryStatusEnum: string
  chrgitmLvLabel: string | null
  chrgitmLv?: string
  lotCostPrice: number
  lotSalePrice: number
  lineCostTotal: number
  linePriceTotal: number
  traceNo: string | null
  /** 后端过滤用 */
  medicationCategoryCode?: string
  deviceCategoryCode?: string
}

/** 库存明细查询参数 */
export interface StockListQuery {
  pageNo: number
  pageSize: number
  keyword?: string
  warehouseIds?: string[]
  supplierIds?: string[]
  expireWithinDays?: number
  medicationCategoryCodes?: string[]
  deviceCategoryCodes?: string[]
  inventoryStatusEnums?: string[]
  chrgitmLvList?: string[]
  zeroStockOnly?: boolean
}

/** 分页结果 */
export interface PageResult<T> {
  rows: T[]
  total: number
}

/** 停供/取消停供请求体 */
export interface InventoryStatusDTO {
  inventoryItemIds: string[]
}

/** 导出请求参数（复用查询参数结构） */
export type ExportQuery = Omit<StockListQuery, 'pageNo' | 'pageSize'>
