/**
 * 根据项目编码生成批号：001-00 + 编码最后一位
 * 例：MD0029 → 001-009
 */
export function computeBatchNo(itemCode: string): string {
  const lastChar = itemCode.slice(-1)
  return `001-00${lastChar}`
}

/**
 * 计算实盘金额：进货价 × 实盘数量
 */
export function computeActualAmount(unitPrice: number, actualQuantity: number): number {
  return unitPrice * actualQuantity
}

/**
 * 计算盈亏数量：实盘数量 - 账面数量
 */
export function computeProfitLoss(actualQuantity: number, bookQuantity: number): number {
  return actualQuantity - bookQuantity
}

/**
 * 计算盈亏金额：盈亏数量 × 单价
 */
export function computeProfitLossAmount(profitLoss: number, unitPrice: number): number {
  return profitLoss * unitPrice
}
