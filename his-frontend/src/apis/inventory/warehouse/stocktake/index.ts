import { useHttp, DataUpType } from '@/plugins/http'
import type {
  PageResult,
  PurchaseInboundDetail,
  PurchaseInboundDetailResult,
  PurchaseInboundOrder,
  PurchaseInboundStatus
} from '@/apis/inventory/common/type'
import {
  mockCreatePurchaseInbound,
  mockGetPurchaseInboundDetail,
  mockPagePurchaseInboundOrders,
  mockUpdatePurchaseInbound,
  mockSubmitPurchaseInbound,
  mockWithdrawPurchaseInbound,
  mockApprovePurchaseInbound,
  mockRejectPurchaseInbound,
  mockRemovePurchaseInbound
} from '@/apis/inventory/common/mock'
import type {
  RawStocktakeDetail,
  RawStocktakeItem,
  StocktakeAddDTO,
  StocktakeApproveDTO,
  StocktakeApprovalResult,
  StocktakeRejectDTO,
  StocktakeSaveDTO,
  StocktakeSubmitDTO,
  StocktakeWithdrawDTO
} from './type'
import { STATUS_MAP, STATUS_ENUM_MAP, STATUS_TEXT_MAP, getUnitName } from './utils'

// 复用公共 API 和类型（loadSupplierOptions 等），本地同名导出会覆写
export * from '@/apis/inventory/common/index'
export type * from '@/apis/inventory/common/type'

/** j6药库管理模块 baseURL */
const baseURL = '/j6-pharmacy'

/** 盘点单固定枚举值 */
const CATEGORY_ENUM = 'PHARMACY_STOCKTAKE'

// ==================== 工具函数 ====================

/** 将后端 StocktakeDetailVO 映射为前端 PurchaseInboundDetailResult */
function mapDetailResult(data: RawStocktakeDetail): PurchaseInboundDetailResult {
  const status: PurchaseInboundStatus = (STATUS_MAP[data.statusEnum] ?? 0) as PurchaseInboundStatus
  const order: PurchaseInboundOrder = {
    id: data.busNo,
    status,
    category: '',
    busNo: data.busNo,
    statusEnum: status,
    statusEnum_enumText: STATUS_TEXT_MAP[data.statusEnum] ?? '',
    applicantId_dictText: data.applicantName,
    applyTime: data.applyTime,
    approverId_dictText: data.approverName,
    approvalTime: data.approvalTime,
    supplierId_dictText: '',
    sourceLocationId: data.purposeLocationId,
    sourceLocationId_dictText: data.purposeLocationName,
    purposeLocationId: data.purposeLocationId,
    purposeLocationId_dictText: data.purposeLocationName,
    totalAmount: data.totalAmount
  }
  const details: PurchaseInboundDetail[] = (data.lines ?? []).map((line) => ({
    id: line.lineId,
    itemCode: line.itemId,
    itemName: line.productName,
    specification: line.specification,
    manufacturer: line.manufacturer,
    batchNo: line.lotNumber,
    bookQuantity: line.bookQuantity,
    actualQuantity: line.actualQuantity,
    actualUnit: line.unitName || getUnitName(line.unitCode),
    unitCode: line.unitCode,
    profitLoss: line.profitLossQuantity,
    unitPrice: line.price,
    actualAmount: line.actualAmount,
    profitLossAmount: line.profitLossAmount,
    remark: line.remake ?? '',
    // 扩展字段：保存时需回传后端
    inventoryItemId: line.inventoryItemId,
    traceNo: line.traceNo
  }))
  return { order, details }
}

/** 将前端保存参数转为后端 StocktakeSaveDTO */
function buildSaveDTO(payload: Record<string, any>, busNo: string): StocktakeSaveDTO {
  const targetStatus = payload.targetStatus as number | undefined
  return {
    busNo,
    statusEnum: targetStatus != null ? STATUS_ENUM_MAP[targetStatus] : undefined,
    purposeLocationId:
      (payload.purposeLocationId as string) ||
      (payload.sourceLocationId as string) ||
      (payload.pharmacy as string) ||
      '',
    purposeLocationName:
      (payload.purposeLocationId_dictText as string) ||
      (payload.sourceLocationId_dictText as string) ||
      (payload.pharmacy as string) ||
      '',
    practitionerId: (payload.practitionerId as string) || '',
    practitionerName: (payload.practitionerName as string) || '',
    creatorId: (payload.creatorId as string) || '',
    creatorName: (payload.creatorName as string) || '',
    applicantId: (payload.applicantId as string) || '',
    applicantName: (payload.applicantName as string) || '',
    approverId: (payload.approverId as string) || '',
    approverName: (payload.approverName as string) || '',
    applyTime: (payload.applyTime as string) || '',
    approvalTime: (payload.approvalTime as string) || '',
    totalAmount: (payload.totalAmount as number) || 0,
    categoryEnum: CATEGORY_ENUM,
    lines: ((payload.details || []) as any[]).map((d) => ({
      inventoryItemId: (d.inventoryItemId as string) || (d.id as string) || '',
      actualQuantity: Number(d.actualQuantity) || 0,
      profitLossQuantity: Number(d.profitLoss) || 0,
      unitCode: (d.unitCode as string) || '',
      lotNumber: (d.batchNo as string) || '',
      traceNo: (d.traceNo as string) || '',
      remake: (d.remark as string) || ''
    }))
  }
}

// ==================== 列表查询 ====================

/**
 * 分页查询盘点单列表（左侧卡片队列）
 * 对接后端 GET /j6-pharmacy/stocktake/list
 */
export async function pagePurchaseInboundOrders(
  query: Record<string, any>
): Promise<PageResult<PurchaseInboundOrder>> {
  try {
    const http = useHttp()
    const res = await http.get<{
      content?: RawStocktakeItem[]
      totalCount?: number
    }>(
      '/stocktake/list',
      {
        stocktakeNo: query.checkNo || query.busNokey,
        startDate: query.startDate,
        endDate: query.endDate,
        pharmacy: query.checkType,
        pageIndex: String(query.pageIndex),
        pageSize: String(query.pageSize)
      },
      { baseURL }
    )
    const data = res.data
    const rows: PurchaseInboundOrder[] = (data?.content ?? []).map((item) => {
      const status: PurchaseInboundStatus = (STATUS_MAP[item.statusEnum] ??
        0) as PurchaseInboundStatus
      return {
        id: item.busNo,
        status,
        category: '',
        busNo: item.busNo,
        statusEnum: status,
        statusEnum_enumText: item.statusEnumText || (STATUS_TEXT_MAP[item.statusEnum] ?? ''),
        applicantId_dictText: item.applicantName,
        applyTime: item.applyTime,
        approverId_dictText: item.approverName,
        approvalTime: item.approvalTime,
        supplierId_dictText: '',
        sourceLocationId: item.purposeLocationId,
        sourceLocationId_dictText: item.purposeLocationName,
        purposeLocationId: item.purposeLocationId,
        purposeLocationId_dictText: item.purposeLocationName,
        totalAmount: item.actualAmountSum
      }
    })
    return { rows, total: data?.totalCount ?? 0 }
  } catch {
    return mockPagePurchaseInboundOrders(query as any)
  }
}

// ==================== 详情查询 ====================

/**
 * 获取盘点单详情（右侧主面板）
 * 对接后端 GET /j6-pharmacy/stocktake/detail?stocktakeNo=
 */
export async function getPurchaseInboundDetail(
  busNo: string
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.get<RawStocktakeDetail>(
      '/stocktake/detail',
      { stocktakeNo: busNo },
      { baseURL }
    )
    return mapDetailResult(res.data!)
  } catch {
    return mockGetPurchaseInboundDetail(busNo)
  }
}

// ==================== 新增（空白草稿） ====================

/**
 * 新增空白盘点单草稿
 * 对接后端 POST /j6-pharmacy/stocktake/create
 */
async function addStocktake(params: StocktakeAddDTO): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.post<RawStocktakeDetail>('/stocktake/create', params, {
      baseURL,
      upType: DataUpType.json
    })
    return mapDetailResult(res.data!)
  } catch {
    return mockCreatePurchaseInbound({
      pharmacy: params.purposeLocationId,
      itemType: '',
      details: []
    } as any)
  }
}

// ==================== 保存（覆写 common 的 create / update） ====================

/**
 * 新增并保存盘点单（兼容旧流程：create → save）
 * 对接后端 POST /j6-pharmacy/stocktake/create → PUT /j6-pharmacy/stocktake/{busNo}/save
 */
export async function createPurchaseInbound(
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    // 第一步：新增空白草稿，获取 busNo
    const addResult = await addStocktake({
      purposeLocationId:
        (payload.purposeLocationId as string) ||
        (payload.sourceLocationId as string) ||
        (payload.pharmacy as string) ||
        '',
      practitionerId: (payload.practitionerId as string) || '',
      categoryEnum: CATEGORY_ENUM
    })

    // 第二步：保存明细
    const busNo = addResult.order.busNo!
    const saveDTO = buildSaveDTO(payload, busNo)
    return await updatePurchaseInbound(busNo, saveDTO)
  } catch {
    return mockCreatePurchaseInbound(payload as any)
  }
}

/**
 * 保存已有盘点单
 * 对接后端 PUT /j6-pharmacy/stocktake/{busNo}/save
 */
export async function updatePurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    // 如果 payload 已经是 StocktakeSaveDTO 格式则直接使用，否则转换
    const saveDTO: StocktakeSaveDTO =
      (payload as any).categoryEnum === CATEGORY_ENUM
        ? (payload as unknown as StocktakeSaveDTO)
        : buildSaveDTO(payload, id)
    const res = await http.put<RawStocktakeDetail>(
      `/stocktake/${encodeURIComponent(saveDTO.busNo)}/save`,
      saveDTO,
      { baseURL, upType: DataUpType.json }
    )
    return mapDetailResult(res.data!)
  } catch {
    return mockUpdatePurchaseInbound(id, payload as any)
  }
}

// ==================== 审批流操作 ====================

/**
 * 提交审批
 * 对接后端 POST /j6-pharmacy/stocktake/submit-approval
 * 审批接口只返回 success/statusEnum，调用后重新拉取详情获取完整 order
 */
export async function submitPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StocktakeApprovalResult>(
      '/stocktake/submit-approval',
      { busNo, approverId: 'admin' } satisfies StocktakeSubmitDTO,
      { baseURL, upType: DataUpType.json }
    )
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockSubmitPurchaseInbound(busNo)
  }
}

/**
 * 撤回审批
 * 对接后端 POST /j6-pharmacy/stocktake/withdraw-approval
 */
export async function withdrawPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StocktakeApprovalResult>(
      '/stocktake/withdraw-approval',
      { busNo } satisfies StocktakeWithdrawDTO,
      { baseURL, upType: DataUpType.json }
    )
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockWithdrawPurchaseInbound(busNo)
  }
}

/**
 * 审批通过
 * 对接后端 POST /j6-pharmacy/stocktake/approve
 */
export async function approvePurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StocktakeApprovalResult>(
      '/stocktake/approve',
      { busNo, approverId: 'admin' } satisfies StocktakeApproveDTO,
      { baseURL, upType: DataUpType.json }
    )
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockApprovePurchaseInbound(busNo)
  }
}

/**
 * 驳回审批
 * 对接后端 POST /j6-pharmacy/stocktake/reject
 */
export async function rejectPurchaseInbound(
  busNo: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StocktakeApprovalResult>(
      '/stocktake/reject',
      { busNo, approverId: 'admin', changeReason: reason } satisfies StocktakeRejectDTO,
      { baseURL, upType: DataUpType.json }
    )
    const detail = await getPurchaseInboundDetail(busNo)
    return detail.order
  } catch {
    return mockRejectPurchaseInbound(busNo, reason)
  }
}

/**
 * 删除单据
 * 对接后端 DELETE /j6-pharmacy/stocktake/{busNo}
 */
export async function removePurchaseInbound(busNo: string): Promise<void> {
  try {
    const http = useHttp()
    await http.delete(`/stocktake/${encodeURIComponent(busNo)}`, undefined, { baseURL })
  } catch {
    return mockRemovePurchaseInbound(busNo)
  }
}
