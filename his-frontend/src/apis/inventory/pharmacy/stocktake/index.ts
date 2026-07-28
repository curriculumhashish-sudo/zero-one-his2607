// import { useHttp } from '@/plugins/http'
import type {
  FormConfig,
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
  mockGetFormConfig,
  mockGetPurchaseInboundDetail,
  mockLoadPurchaseInboundInit,
  mockLoadSupplierOptions,
  mockPagePurchaseInboundOrders, // 已替换为真实接口
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

// 分页查询
export async function pagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  // mock模拟数据
  return mockPagePurchaseInboundOrders(query)

  // 真实接口调用
  // const http = useHttp()
  // const res = await http.get<PageResult<PurchaseInboundOrder>>(
  //   '/pharmacy/inventory-check/list',
  //   query
  // )
  // return res.data!
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

/** 获取表单配置（列定义、默认值、SelectTable 配置） */
export function getFormConfig(): Promise<FormConfig> {
  return mockGetFormConfig()
}

export function loadSupplierOptions(query: {
  keyword: string
  pageIndex: number
  pageSize: number
}): Promise<{ rows: SupplierItem[]; total: number }> {
  return mockLoadSupplierOptions(query)
}

export type * from './type'
