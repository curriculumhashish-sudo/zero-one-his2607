import type { StockListQuery, PageResult, StockListRow } from './type'
import { mockQueryStockList, mockRetireItem, mockResumeItem } from './mock'

/*
 * ==================== 库存明细接口 ====================
 * 当前使用 mock，对接后端时替换函数体即可，入参/出参不变。
 *
 * 后端接入示例：
 *   import { requestClient } from '#/api/request'
 *   export async function queryStockList(query: StockListQuery) {
 *     const res = await requestClient.post<ApiResponse<PageResult<StockListRow>>>(
 *       '/inventory/stockList/query', query
 *     )
 *     return res.data
 *   }
 */

/** 分页查询库存明细 */
export function queryStockList(query: StockListQuery): Promise<PageResult<StockListRow>> {
  return mockQueryStockList(query)
}

/** 停供 */
export function retireItem(id: string): Promise<void> {
  return mockRetireItem(id)
}

/** 取消停供 */
export function resumeItem(id: string): Promise<void> {
  return mockResumeItem(id)
}

export type * from './type'
