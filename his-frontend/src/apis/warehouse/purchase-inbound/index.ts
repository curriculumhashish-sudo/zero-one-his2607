import type { PurchaseInboundQuery, PageResult, PurchaseInboundOrder, PurchaseInboundDetail, SupplierItem } from './type'
import {
  mockPagePurchaseInboundOrders,
  mockLoadPurchaseInboundInit,
  mockGetPurchaseInboundDetail,
  mockLoadSupplierOptions
} from './mock'

/** 入库加载初始化 */
export function loadPurchaseInboundInit() {
  return mockLoadPurchaseInboundInit()
}

/** 分页查询采购入库单 */
export function pagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  return mockPagePurchaseInboundOrders(query)
}

/** 获取采购入库详情（含明细行） */
export function getPurchaseInboundDetail(id: string): Promise<{
  order: PurchaseInboundOrder
  details: PurchaseInboundDetail[]
}> {
  return mockGetPurchaseInboundDetail(id)
}

/** 加载药品供应商选项（SelectTable 分页） */
export function loadSupplierOptions(
  query: { keyword: string; pageIndex: number; pageSize: number }
): Promise<{ rows: SupplierItem[]; total: number }> {
  return mockLoadSupplierOptions(query)
}
