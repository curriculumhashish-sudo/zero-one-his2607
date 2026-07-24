import type {
  PageResult,
  PurchaseInboundDetailResult,
  PurchaseInboundOrder,
  PurchaseInboundQuery,
  PurchaseInboundSavePayload,
  SupplierItem
} from './type'
import {
  mockApprovePurchaseInbound,
  mockCreatePurchaseInbound,
  mockGetPurchaseInboundDetail,
  mockLoadPurchaseInboundInit,
  mockLoadSupplierOptions,
  mockPagePurchaseInboundOrders,
  mockRejectPurchaseInbound,
  mockRemovePurchaseInbound,
  mockSubmitPurchaseInbound,
  mockUpdatePurchaseInbound,
  mockWithdrawPurchaseInbound
} from './mock'

/** 页面初始化选项。后端接入时只替换本文件内部实现。 */
export function loadPurchaseInboundInit() {
  return mockLoadPurchaseInboundInit()
}

export function pagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  return mockPagePurchaseInboundOrders(query)
}

export function getPurchaseInboundDetail(id: string): Promise<PurchaseInboundDetailResult> {
  return mockGetPurchaseInboundDetail(id)
}

export function createPurchaseInbound(
  payload: PurchaseInboundSavePayload
): Promise<PurchaseInboundDetailResult> {
  return mockCreatePurchaseInbound(payload)
}

export function updatePurchaseInbound(
  id: string,
  payload: PurchaseInboundSavePayload
): Promise<PurchaseInboundDetailResult> {
  return mockUpdatePurchaseInbound(id, payload)
}

export function removePurchaseInbound(id: string): Promise<void> {
  return mockRemovePurchaseInbound(id)
}

export function submitPurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  return mockSubmitPurchaseInbound(id)
}

export function withdrawPurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  return mockWithdrawPurchaseInbound(id)
}

export function approvePurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  return mockApprovePurchaseInbound(id)
}

export function rejectPurchaseInbound(id: string, reason: string): Promise<PurchaseInboundOrder> {
  return mockRejectPurchaseInbound(id, reason)
}

export function loadSupplierOptions(query: {
  keyword: string
  pageIndex: number
  pageSize: number
}): Promise<{ rows: SupplierItem[]; total: number }> {
  return mockLoadSupplierOptions(query)
}

export type * from './type'
