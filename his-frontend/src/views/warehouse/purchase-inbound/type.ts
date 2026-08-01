/** 盘点明细行 */
export interface InventoryLineItem {
  id: string
  /** 项目名称 */
  itemName: string
  /** 规格 */
  specification: string
  /** 厂家 */
  manufacturer: string
  /** 批号 */
  batchNo: string
  /** 账面数量 */
  bookQuantity: number
  /** 实盘数量 */
  actualQuantity: number
  /** 实盘单位 */
  actualUnit: string
  /** 盈亏数量 */
  profitLoss: number
  /** 单价 */
  unitPrice: number
  /** 实盘金额 */
  actualAmount: number
  /** 盈亏金额 */
  profitLossAmount: number
  /** 备注 */
  remark: string
}
