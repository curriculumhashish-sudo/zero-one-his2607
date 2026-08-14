import { useHttp, DataUpType } from '@/plugins/http'
import type {
  PageResult,
  PurchaseInboundDetail,
  PurchaseInboundDetailResult,
  PurchaseInboundOrder,
  PurchaseInboundStatus
} from '@/apis/inventory/common/type'
import {
  mockApprovePurchaseInbound,
  mockCreatePurchaseInbound,
  mockGetPurchaseInboundDetail,
  mockPagePurchaseInboundOrders,
  mockRejectPurchaseInbound,
  mockRemovePurchaseInbound,
  mockSubmitPurchaseInbound,
  mockUpdatePurchaseInbound,
  mockWithdrawPurchaseInbound
} from '@/apis/inventory/common/mock'
import { STOCKTAKE_STATUS_ENUM_MAP, STOCKTAKE_STATUS_MAP, STOCKTAKE_STATUS_TEXT } from './type.ts'
import type { StocktakeCreateResponse, StocktakeDetail, StocktakeRecord } from './type.ts'

// 复用公共 API 实现，可在此覆写差异接口
export * from '@/apis/inventory/common/index'
export type * from '@/apis/inventory/common/type'

/** j7药房管理模块 baseURL */
const baseURL = '/j7-pharmacy'

// ==================== 列表查询 ====================

/**
 * 分页查询药房盘点单列表（左侧卡片队列）
 * 对接后端 POST /j7-pharmacy/api/inventory-check/list
 */
export async function pagePurchaseInboundOrders(
  query: Record<string, any>
): Promise<PageResult<PurchaseInboundOrder>> {
  try {
    const http = useHttp()
    const res = await http.post<{ rows: StocktakeRecord[]; total: number }>(
      '/api/inventory-check/list',
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        busNo: query.checkNo || undefined,
        warehouseName: query.checkType || undefined,
        statusEnum: query.statusEnum || undefined,
        warehouseId: query.warehouseId || undefined,
        applicantId: query.applicantId || undefined,
        startTime: query.startDate || undefined,
        endTime: query.endDate || undefined
      },
      { baseURL, upType: DataUpType.json }
    )
    const data = res.data!
    const rows: PurchaseInboundOrder[] = (data.rows ?? []).map((item) => {
      const status: PurchaseInboundStatus = STOCKTAKE_STATUS_MAP[item.statusEnum] ?? 0
      return {
        id: item.id,
        status,
        category: '',
        busNo: item.busNo,
        statusEnum: status,
        statusEnum_enumText: STOCKTAKE_STATUS_TEXT[item.statusEnum] ?? '',
        applicantId_dictText: item.createdByName,
        applyTime: item.createdAt,
        approverId_dictText: item.approverName,
        approvalTime: item.approvalTime,
        supplierId_dictText: '',
        sourceLocationId: item.warehouseId,
        sourceLocationId_dictText: item.stocktakePharmacy || item.warehouseName,
        purposeLocationId: '',
        purposeLocationId_dictText: '',
        totalAmount: item.totalPrice
      }
    })
    return { rows, total: data.total ?? 0 }
  } catch {
    return mockPagePurchaseInboundOrders(query as any)
  }
}

// ==================== 详情查询 ====================

/**
 * 获取盘点单详情（右侧主面板）
 * 对接后端 GET /j7-pharmacy/api/inventory-check/detail/{id}
 */
export async function getPurchaseInboundDetail(id: string): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.get<StocktakeDetail>(
      `/api/inventory-check/detail/${encodeURIComponent(id)}`,
      undefined,
      { baseURL }
    )
    const data = res.data!
    const status: PurchaseInboundStatus = STOCKTAKE_STATUS_MAP[data.statusEnum] ?? 0
    const order: PurchaseInboundOrder = {
      id: data.id,
      status,
      category: '',
      busNo: data.busNo,
      statusEnum: status,
      statusEnum_enumText: STOCKTAKE_STATUS_TEXT[data.statusEnum] ?? '',
      applicantId_dictText: data.applicantName,
      applyTime: data.applyTime,
      approverId_dictText: data.approverName,
      approvalTime: data.approvalTime,
      supplierId_dictText: '',
      sourceLocationId: data.warehouseId,
      sourceLocationId_dictText: data.stocktakePharmacy || data.warehouseName,
      purposeLocationId: '',
      purposeLocationId_dictText: '',
      totalAmount: data.totalPrice
    }
    const details: PurchaseInboundDetail[] = (data.items ?? []).map((item) => ({
      id: item.id,
      itemCode: item.itemId,
      inventoryItemId: item.itemId,
      itemName: item.itemName,
      specification: item.specification,
      manufacturer: item.manufacturer,
      batchNo: item.lotNumber,
      bookQuantity: item.bookQuantity,
      actualQuantity: item.totalQuantity,
      actualUnit: item.unitName,
      profitLoss: item.itemQuantity,
      unitPrice: item.price,
      actualAmount: item.totalPrice,
      profitLossAmount: item.profitLossPrice,
      remark: item.remark
    }))
    return { order, details }
  } catch {
    return mockGetPurchaseInboundDetail(id)
  }
}

// ==================== 新增单据 ====================

/**
 * 新增盘点单草稿（先创建空白单据，有明细时自动保存）
 * 对接后端 GET /j7-pharmacy/api/inventory-check/new + POST /api/inventory-check/save
 */
export async function createPurchaseInbound(
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    // 1. 创建空白草稿，获取真实后端 ID 和 busNo
    const newRes = await http.get<StocktakeCreateResponse>('/api/inventory-check/new', undefined, {
      baseURL
    })
    const data = newRes.data!
    // 2. 有明细数据时保存到新草稿
    const details = (payload.details || []) as any[]
    if (details.length > 0) {
      await http.post('/api/inventory-check/save', buildStocktakeSaveDTO(data.id, payload), {
        baseURL,
        upType: DataUpType.json
      })
    }
    // 3. 拉取最新详情（含已保存的明细）
    const detail = await getPurchaseInboundDetail(data.id)
    return detail
  } catch {
    return mockCreatePurchaseInbound(payload as any)
  }
}

// ==================== 保存单据 ====================

/** 将前端 payload 转为后端 StocktakeSaveDTO */
function buildStocktakeSaveDTO(id: string, payload: Record<string, any>): Record<string, any> {
  const details = (payload.details || []) as any[]
  return {
    id,
    busNo: (payload.busNo as string) || undefined,
    statusEnum:
      payload.targetStatus != null
        ? STOCKTAKE_STATUS_ENUM_MAP[payload.targetStatus as number]
        : undefined,
    warehouseId:
      (payload.sourceLocationId_dictText as string) || (payload.pharmacy as string) || '',
    occurrenceTime: new Date().toISOString(),
    remark: (payload.remark as string) || '',
    purposeTypeEnum: (payload.itemType as string) || '',
    applicantId: (payload.applicantId as string) || 'admin',
    tenantId: '1',
    totalPrice: details.reduce((sum, d) => sum + (Number(d.actualAmount) || 0), 0),
    items: details.map((d) => ({
      id: d.id ? String(d.id) : undefined,
      itemTable: 'med_medication_definition',
      itemId: (d.inventoryItemId as string) || '',
      itemName: (d.itemName as string) || '',
      specification: (d.specification as string) || '',
      manufacturer: (d.manufacturer as string) || '',
      lotNumber: (d.batchNo as string) || '',
      traceNo: (d.traceCode as string) || '',
      bookQuantity: Number(d.bookQuantity) || 0,
      totalQuantity: Number(d.actualQuantity) || 0,
      itemQuantity: Number(d.profitLoss) || 0,
      unitCode: (d.actualUnit as string) || '',
      price: Number(d.unitPrice) || 0,
      totalPrice: Number(d.actualAmount) || 0,
      actualAmount: Number(d.actualAmount) || 0,
      profitLossPrice: Number(d.profitLossAmount) || 0,
      remark: (d.remark as string) || ''
    }))
  }
}

/**
 * 保存盘点单（全量更新主表+明细）
 * 对接后端 POST /j7-pharmacy/api/inventory-check/save
 */
export async function updatePurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    await http.post('/api/inventory-check/save', buildStocktakeSaveDTO(id, payload), {
      baseURL,
      upType: DataUpType.json
    })
    // 保存成功，重新拉取最新详情
    return await getPurchaseInboundDetail(id)
  } catch {
    return mockUpdatePurchaseInbound(id, payload as any)
  }
}

// ==================== 审批流操作 ====================

/**
 * 提交审批
 * 对接后端 PUT /j7-pharmacy/j7/stocktake-approval/submit
 */
export async function submitPurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    console.log('busNo', busNo)

    await http.put(
      '/j7/stocktake-approval/submit',
      { busNo, operatorId: 'admin' },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockSubmitPurchaseInbound(id)
  }
}

/**
 * 保存并提交审批（保存明细 + 提交，一次调用）
 * 对接后端 POST /j7-pharmacy/api/inventory-check/save-submit
 */
export async function saveAndSubmitPurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post('/api/inventory-check/save-submit', buildStocktakeSaveDTO(id, payload), {
      baseURL,
      upType: DataUpType.json
    })
    // 保存并提交成功，重新拉取最新详情
    const detail = await getPurchaseInboundDetail(id)
    return detail.order
  } catch {
    // 回退：先保存再提交
    const result = await mockUpdatePurchaseInbound(id, payload as any)
    return mockSubmitPurchaseInbound(result.order.id as string)
  }
}

/**
 * 撤回审批
 * 对接后端 PUT /j7-pharmacy/j7/stocktake-approval/withdraw
 */
export async function withdrawPurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    await http.put(
      '/j7/stocktake-approval/withdraw',
      { busNo, operatorId: 'admin' },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockWithdrawPurchaseInbound(id)
  }
}

/**
 * 驳回审批
 * 对接后端 PUT /j7-pharmacy/j7/stocktake-approval/reject
 */
export async function rejectPurchaseInbound(
  id: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    await http.put(
      '/j7/stocktake-approval/reject',
      { busNo, approverId: 'admin', remark: reason, approvalTime: new Date().toISOString() },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockRejectPurchaseInbound(id, reason)
  }
}

/**
 * 审批通过
 * 对接后端 PUT /j7-pharmacy/j7/stocktake-approval/approve
 */
export async function approvePurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.put(
      '/j7/stocktake-approval/approve',
      { busNo, approverId: 'admin', approvalTime: new Date().toISOString() },
      { baseURL, upType: DataUpType.json }
    )
    // 审批通过，重新拉取最新状态
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockApprovePurchaseInbound(busNo)
  }
}

// ==================== 删除单据 ====================

/**
 * 删除盘点单（软删除，仅草稿/待审核状态可删）
 * 对接后端 DELETE /j7-pharmacy/j7/stocktake-approval/delete
 */
export async function removePurchaseInbound(busNo: string): Promise<void> {
  try {
    const http = useHttp()
    await http.delete('/j7/stocktake-approval/delete', { busNo }, { baseURL })
  } catch {
    return mockRemovePurchaseInbound(busNo)
  }
}
