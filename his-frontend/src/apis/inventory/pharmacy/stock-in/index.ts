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
import { STATUS_ENUM_MAP, STATUS_NAME_MAP, STATUS_TEXT } from './type.ts'
import type {
  PharmacyInboundCreateResponse,
  PharmacyInboundDetail,
  PharmacyInboundRecord
} from './type.ts'
// 复用公共 API 实现，可在此覆写差异接口
export * from '@/apis/inventory/common/index'
export type * from '@/apis/inventory/common/type'

/** j7药房管理模块 baseURL */
const baseURL = '/j7-pharmacy'

// ==================== 列表查询 ====================

/**
 * 分页查询药房入库单列表（左侧卡片队列）
 * 对接后端 POST /j7-pharmacy/j7/inbound-approval/list
 */
export async function pagePurchaseInboundOrders(
  query: Record<string, any>
): Promise<PageResult<PurchaseInboundOrder>> {
  try {
    const http = useHttp()
    const res = await http.post<{ records: PharmacyInboundRecord[]; total: number }>(
      '/j7/inbound-approval/list',
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        busNo: query.checkNo || undefined,
        sourceLocationId: query.checkType || undefined,
        purposeLocationId: query.purposeLocationId || undefined,
        statusEnum: query.statusEnum || undefined,
        startTime: query.startDate || undefined,
        endTime: query.endDate || undefined
      },
      { baseURL, upType: DataUpType.json }
    )
    const data = res.data!
    const rows: PurchaseInboundOrder[] = (data.records ?? []).map((item) => {
      const status: PurchaseInboundStatus = STATUS_NAME_MAP[item.statusName] ?? 0
      return {
        id: item.id,
        status,
        category: '',
        busNo: item.busNo,
        statusEnum: status,
        statusEnum_enumText: STATUS_TEXT[item.statusName] ?? '',
        applicantId_dictText: item.createByName,
        applyTime: item.applyTime,
        approverId_dictText: item.approverName,
        approvalTime: item.approveTime,
        supplierId_dictText: '',
        sourceLocationId: '',
        sourceLocationId_dictText: item.sourceLocationName,
        purposeLocationId: '',
        purposeLocationId_dictText: item.purposeLocationName,
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
 * 获取入库单详情（右侧主面板）
 * 对接后端 GET /j7-pharmacy/j7/inbound-approval/detail/{id}
 */
export async function getPurchaseInboundDetail(id: string): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.get<PharmacyInboundDetail>(
      `/j7/inbound-approval/detail/${encodeURIComponent(id)}`,
      undefined,
      { baseURL }
    )
    const data = res.data!
    const status: PurchaseInboundStatus = STATUS_NAME_MAP[data.statusEnum] ?? 0
    const order: PurchaseInboundOrder = {
      id: data.id,
      status,
      category: '',
      busNo: data.busNo,
      statusEnum: status,
      statusEnum_enumText: STATUS_TEXT[data.statusEnum] ?? '',
      applicantId_dictText: data.createByName,
      applyTime: data.applyTime,
      approverId_dictText: data.approverName,
      approvalTime: data.approveTime,
      supplierId_dictText: data.supplierName,
      sourceLocationId: '',
      sourceLocationId_dictText: '',
      purposeLocationId: data.purposeLocationId,
      purposeLocationId_dictText: data.purposeLocationName,
      totalAmount: data.totalPrice
    }
    const details: PurchaseInboundDetail[] = (data.items ?? []).map((item) => {
      const line: PurchaseInboundDetail = {
        id: item.id,
        itemCode: item.itemId,
        itemName: item.itemName,
        specification: item.spec,
        manufacturer: item.manufacturer,
        batchNo: item.lotNumber,
        bookQuantity: 0,
        actualQuantity: item.quantity,
        actualUnit: item.unit,
        profitLoss: 0,
        unitPrice: item.price,
        actualAmount: item.subTotal,
        profitLossAmount: 0,
        traceCode: item.traceCode,
        remark: item.remark
      }
      return line
    })
    return { order, details }
  } catch {
    return mockGetPurchaseInboundDetail(id)
  }
}

// ==================== 新增 / 保存单据 ====================

/** 将前端 payload 转为后端 InStockOrderSaveDTO */
function buildInStockOrderDTO(payload: Record<string, any>): Record<string, any> {
  const details = (payload.details || []) as any[]
  return {
    sourceLocationId:
      (payload.sourceLocationId_dictText as string) || (payload.sourceLocationId as string) || '',
    warehouseId:
      (payload.purposeLocationId_dictText as string) || (payload.purposeLocationId as string) || '',
    projectType: (payload.itemType as string) || (payload.category as string) || '',
    inDate: new Date().toISOString().split('T')[0],
    statusEnum:
      payload.targetStatus != null ? STATUS_ENUM_MAP[payload.targetStatus as number] : undefined,
    remark: (payload.remark as string) || '',
    items: details.map((d, i) => ({
      seqNo: i + 1,
      itemId: (d.inventoryItemId as string) || '',
      drugName: (d.itemName as string) || '',
      spec: (d.specification as string) || '',
      manufacturer: (d.manufacturer as string) || '',
      quantity: Number(d.actualQuantity) || 0,
      unitPrice: Number(d.unitPrice) || 0,
      totalAmount: Number(d.actualAmount) || 0,
      batchNo: (d.batchNo as string) || '',
      expireDate: (d.expireDate as string) || '2028-07-30',
      traceNo: (d.traceCode as string) || '',
      itemRemark: (d.remark as string) || ''
    }))
  }
}

/** 将后端 InStockOrderSaveDTO 响应映射为 PurchaseInboundDetailResult */
function mapCreateResponse(data: PharmacyInboundCreateResponse): PurchaseInboundDetailResult {
  const status: PurchaseInboundStatus = STATUS_NAME_MAP[data.statusEnum] ?? 0
  const order: PurchaseInboundOrder = {
    id: data.id,
    status,
    category: '',
    busNo: data.orderNo,
    statusEnum: status,
    statusEnum_enumText: STATUS_TEXT[data.statusEnum] ?? '',
    applicantId_dictText: data.createdBy,
    applyTime: data.createdAt,
    approverId_dictText: data.approverId,
    approvalTime: data.approvalTime,
    supplierId_dictText: '',
    sourceLocationId: data.sourceLocationId,
    sourceLocationId_dictText: '',
    purposeLocationId: data.warehouseId,
    purposeLocationId_dictText: '',
    totalAmount: data.totalPrice
  }
  const details: PurchaseInboundDetail[] = (data.items ?? []).map((item) => ({
    id: String(item.id ?? ''),
    itemCode: item.itemId,
    inventoryItemId: item.itemId,
    itemName: item.drugName,
    specification: item.spec,
    manufacturer: item.manufacturer,
    batchNo: item.batchNo,
    bookQuantity: 0,
    actualQuantity: item.quantity,
    actualUnit: '',
    profitLoss: 0,
    unitPrice: item.unitPrice,
    actualAmount: item.totalAmount,
    profitLossAmount: 0,
    traceCode: item.traceNo,
    remark: item.itemRemark
  }))
  return { order, details }
}

/**
 * 新增药房入库单（含明细行）
 * 对接后端 POST /j7-pharmacy/j7/inbound-approval/addInStockOrder
 */
export async function createPurchaseInbound(
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.post<PharmacyInboundCreateResponse>(
      '/j7/inbound-approval/addInStockOrder',
      buildInStockOrderDTO(payload),
      { baseURL, upType: DataUpType.json }
    )
    return mapCreateResponse(res.data!)
  } catch {
    return mockCreatePurchaseInbound(payload as any)
  }
}

/**
 * 保存已有药房入库单（全量更新明细）
 * 对接后端 PUT /j7-pharmacy/j7/inbound-approval/{orderId}/saveInStockOrder
 */
export async function updatePurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const dto = buildInStockOrderDTO(payload)
    // 保存时需传入单据ID和单号
    dto.id = id
    dto.orderNo = (payload.busNo as string) || undefined
    // 保留已有明细行ID（后端用于区分新增/修改/删除）
    dto.items = dto.items.map((item: any, i: number) => ({
      ...item,
      id: ((payload.details || [])[i] as any)?.id
        ? Number(((payload.details || [])[i] as any).id)
        : undefined
    }))
    await http.put(`/j7/inbound-approval/${encodeURIComponent(id)}/saveInStockOrder`, dto, {
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
 * 提交审批（仅变更状态，不保存明细）
 * 对接后端 PUT /j7-pharmacy/j7/inbound-approval/{orderId}/submitApproval
 */
export async function submitPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.put(`/j7/inbound-approval/${encodeURIComponent(busNo)}/submitApproval`, undefined, {
      baseURL
    })
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockSubmitPurchaseInbound(busNo)
  }
}

/**
 * 保存并提交审批（保存明细 + 变更状态，一次调用）
 * 对接后端 PUT /j7-pharmacy/j7/inbound-approval/{orderId}/submitInStockOrder
 */
export async function saveAndSubmitPurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const dto = buildInStockOrderDTO(payload)
    dto.id = id
    dto.orderNo = (payload.busNo as string) || undefined
    dto.items = dto.items.map((item: any, i: number) => ({
      ...item,
      id: ((payload.details || [])[i] as any)?.id
        ? Number(((payload.details || [])[i] as any).id)
        : undefined
    }))
    await http.put(`/j7/inbound-approval/${encodeURIComponent(id)}/submitInStockOrder`, dto, {
      baseURL,
      upType: DataUpType.json
    })
    const detail = await getPurchaseInboundDetail(id)
    return detail.order
  } catch {
    // 回退：先保存再提交
    const result = await mockUpdatePurchaseInbound(id, payload as any)
    return mockSubmitPurchaseInbound(result.order.id as string)
  }
}

/**
 * 删除单据（软删除）
 * 对接后端 DELETE /j7-pharmacy/j7/inbound-approval/delete
 */
export async function removePurchaseInbound(busNo: string): Promise<void> {
  try {
    const http = useHttp()
    await http.delete('/j7/inbound-approval/delete', { busNo }, { baseURL })
  } catch {
    return mockRemovePurchaseInbound(busNo)
  }
}

/**
 * 审批通过
 * 对接后端 POST /j7-pharmacy/j7/inbound-approval/approve
 */
export async function approvePurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    await http.post(
      '/j7/inbound-approval/approve',
      {
        busNo,
        approverId: 'admin',
        approvalTime: new Date().toISOString()
      },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockApprovePurchaseInbound(id)
  }
}

/**
 * 撤回审批
 * 对接后端 POST /j7-pharmacy/j7/inbound-approval/withdraw
 */
export async function withdrawPurchaseInbound(id: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    await http.post(
      '/j7/inbound-approval/withdraw',
      {
        busNo,
        approverId: 'admin',
        approvalTime: new Date().toISOString()
      },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockWithdrawPurchaseInbound(id)
  }
}

/**
 * 驳回审批
 * 对接后端 POST /j7-pharmacy/j7/inbound-approval/reject
 */
export async function rejectPurchaseInbound(
  id: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    const detail = await getPurchaseInboundDetail(id)
    const busNo = detail.order.busNo || id
    await http.post(
      '/j7/inbound-approval/reject',
      {
        busNo,
        remark: reason,
        approverId: 'admin',
        approvalTime: new Date().toISOString()
      },
      { baseURL, upType: DataUpType.json }
    )
    return detail.order
  } catch {
    return mockRejectPurchaseInbound(id, reason)
  }
}
