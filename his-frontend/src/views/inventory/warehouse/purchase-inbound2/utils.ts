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
 * 计算盈亏金额：实盘金额 - (进货价 × 账面数量)
 */
export function computeProfitLossAmount(
  actualAmount: number,
  unitPrice: number,
  bookQuantity: number
): number {
  return actualAmount - unitPrice * bookQuantity
}
