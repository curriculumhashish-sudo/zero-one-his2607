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
  RawStockOutDetail,
  RawStockOutItem,
  StockOutAddDTO,
  StockOutApproveDTO,
  StockOutApprovalResult,
  StockOutQuery,
  StockOutRejectDTO,
  StockOutSaveDTO,
  StockOutSubmitDTO,
  StockOutWithdrawDTO
} from './type'
import { STATUS_MAP, STATUS_ENUM_MAP, STATUS_TEXT_MAP, toDateTime } from './utils'

// 复用公共 API 实现（remove / submit / approve 等），可在此覆写差异接口
export * from '@/apis/inventory/common/index'
export type * from '@/apis/inventory/common/type'

/** j6药库管理模块 baseURL */
const baseURL = '/j6-pharmacy'

/** 出库单固定枚举值 */
const TYPE_ENUM = 'WAREHOUSE_STOCKOUT'
const CATEGORY_ENUM = 'STOCK_OUT'
const SOURCE_TYPE_ENUM = 'WAREHOUSE'
const PURPOSE_TYPE_ENUM = 'PHARMACY'

/** 药源库名称 → ID 映射 */
const DRUG_DATABASE_ID_MAP: Record<string, string> = {
  耗材库: '322954857622802432',
  西药库: '322954885380706304',
  中药库: '322954919841107968'
}

// ==================== 工具函数 ====================

/** 将后端 StockOutDetailVO 映射为前端 PurchaseInboundDetailResult */
function mapDetailResult(data: RawStockOutDetail): PurchaseInboundDetailResult {
  const status: PurchaseInboundStatus = (STATUS_MAP[data.statusEnum] ?? 0) as PurchaseInboundStatus
  const order: PurchaseInboundOrder = {
    id: data.busNo,
    status,
    category: '',
    busNo: data.busNo,
    statusEnum: status,
    statusEnum_enumText: STATUS_TEXT_MAP[data.statusEnum] ?? '',
    applicantId_dictText: data.creatorName,
    applyTime: data.applyTime,
    approverId_dictText: data.approverName,
    approvalTime: data.approvalTime,
    supplierId_dictText: '',
    sourceLocationId: '322954885380706304',
    sourceLocationId_dictText: data.sourceLocationName,
    purposeLocationId: data.purposeLocationId,
    purposeLocationId_dictText: data.purposeLocationName,
    totalAmount: data.totalAmount
  }
  const details: PurchaseInboundDetail[] = (data.lines ?? []).map((line) => ({
    id: line.id,
    itemCode: line.itemId,
    inventoryItemId: line.itemId,
    itemName: line.productName,
    specification: line.specification,
    manufacturer: line.manufacturer,
    batchNo: line.lotNumber,
    bookQuantity: line.itemQuantity,
    actualQuantity: line.itemQuantity,
    actualUnit: line.unitCodeDictText,
    unitCode: line.unitCode,
    profitLoss: 0,
    unitPrice: line.price,
    actualAmount: line.totalPrice,
    profitLossAmount: 0,
    remark: line.remake ?? ''
  }))
  return { order, details }
}

/** 将前端保存参数转为后端 StockOutSaveDTO */
function buildSaveDTO(
  payload: Record<string, any>,
  busNo: string,
  version: number
): StockOutSaveDTO {
  return {
    busNo,
    originalBusNo: (payload.originalBusNo as string) || undefined,
    typeEnum: TYPE_ENUM,
    categoryEnum: CATEGORY_ENUM,
    statusEnum: STATUS_ENUM_MAP[payload.targetStatus as number] || 'DRAFT',
    sourceTypeEnum: SOURCE_TYPE_ENUM,
    sourceLocationId:
      (payload.sourceLocationId as string) ||
      (payload.sourceLocationId_dictText as string) ||
      (payload.pharmacy as string) ||
      '',
    sourceLocationName:
      (payload.sourceLocationId_dictText as string) || (payload.pharmacy as string) || '',
    purposeTypeEnum: PURPOSE_TYPE_ENUM,
    purposeLocationId:
      (payload.purposeLocationId as string) ||
      (payload.purposeLocationId_dictText as string) ||
      (payload.targetPharmacy as string) ||
      '',
    purposeLocationName:
      (payload.purposeLocationId_dictText as string) || (payload.targetPharmacy as string) || '',
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
    remark: (payload.remark as string) || '',
    version,
    lines: ((payload.details || []) as any[]).map((d) => ({
      itemTable: 'med_medication_definition',
      itemId: (d.inventoryItemId as string) || '',
      itemQuantity: Number(d.actualQuantity) || 0,
      unitCode: (d.unitCode as string) || '',
      productName: (d.itemName as string) || '', // 项目名称
      specification: (d.specification as string) || '', // 规格
      manufacturer: (d.manufacturer as string) || '', // 厂家
      lotNumber: (d.batchNo as string) || '',
      traceNo: (d.traceNo as string) || '',
      price: Number(d.unitPrice) || 0,
      totalPrice: Number(d.actualAmount) || 0,
      occurrenceTime: '',
      startTime: '',
      endTime: '',
      remake: (d.remark as string) || ''
    }))
  }
}

// ==================== 列表查询 ====================

/**
 * 分页查询出库单列表（左侧卡片队列）
 * 对接后端 GET /j6-pharmacy/stockout/getPharmacy
 */
export async function pagePurchaseInboundOrders(
  query: StockOutQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  try {
    const http = useHttp()
    const res = await http.get<{
      rows?: RawStockOutItem[]
      total?: number
    }>(
      '/stockout/getPharmacy',
      {
        keyword: query.busNokey,
        applyTimeFrom: toDateTime(query.startDate, false),
        applyTimeTo: toDateTime(query.endDate, true),
        sourceLocationId: DRUG_DATABASE_ID_MAP[query.drugDatabase ?? ''] ?? query.drugDatabase,
        purposeLocationId: query.pharmacyWarehouse,
        pageIndex: String(query.pageIndex),
        pageSize: String(query.pageSize)
      },
      { baseURL }
    )
    const data = res.data
    const rows: PurchaseInboundOrder[] = (data?.rows ?? []).map((item) => {
      const status: PurchaseInboundStatus = (STATUS_MAP[item.statusEnum] ??
        0) as PurchaseInboundStatus
      return {
        id: item.busNo,
        status,
        category: '',
        busNo: item.busNo,
        statusEnum: status,
        statusEnum_enumText: item.statusEnumText || (STATUS_TEXT_MAP[item.statusEnum] ?? ''),
        applicantId_dictText: item.createdBy,
        applyTime: item.applyTime,
        approverId_dictText: item.approverName,
        approvalTime: item.approvalTime,
        supplierId_dictText: '',
        sourceLocationId: item.sourceLocationId,
        sourceLocationId_dictText: item.sourceLocationName,
        purposeLocationId: item.purposeLocationId,
        purposeLocationId_dictText: item.purposeLocationName,
        totalAmount: item.totalAmount
      }
    })
    return { rows, total: data?.total ?? 0 }
  } catch {
    return mockPagePurchaseInboundOrders(query as any)
  }
}

// ==================== 详情查询 ====================

/**
 * 获取出库单详情（右侧主面板）
 * 对接后端 GET /j6-pharmacy/stockout/{busNo}
 */
export async function getPurchaseInboundDetail(
  busNo: string
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.get<RawStockOutDetail>(
      `/stockout/${encodeURIComponent(busNo)}`,
      undefined,
      { baseURL }
    )
    return mapDetailResult(res.data!)
  } catch {
    return mockGetPurchaseInboundDetail(busNo)
  }
}

// ==================== 新增（空白草稿） ====================

/**
 * 新增空白出库单草稿
 * 对接后端 POST /j6-pharmacy/stockout/add
 */
export async function addStockOut(params: StockOutAddDTO): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.post<RawStockOutDetail>('/stockout/add', params, {
      baseURL,
      upType: DataUpType.json
    })
    return mapDetailResult(res.data!)
  } catch {
    // 回退 mock：用空明细模拟新增
    return mockCreatePurchaseInbound({
      pharmacy: params.sourceLocationId,
      itemType: '',
      details: []
    } as any)
  }
}

// ==================== 保存（覆写 common 的 create / update） ====================

/**
 * 新增并保存出库单（兼容旧流程：add → save）
 * 对接后端 POST /j6-pharmacy/stockout/add → POST /j6-pharmacy/stockout/save
 */
export async function createPurchaseInbound(
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    // 第一步：新增空白草稿，获取 busNo
    const addResult = await addStockOut({
      sourceLocationId:
        (payload.sourceLocationId as string) ||
        (payload.sourceLocationId_dictText as string) ||
        (payload.pharmacy as string) ||
        '',
      purposeLocationId:
        (payload.purposeLocationId as string) ||
        (payload.purposeLocationId_dictText as string) ||
        (payload.targetPharmacy as string) ||
        ''
    })

    // 第二步：保存明细
    const busNo = addResult.order.busNo!
    const saveDTO = buildSaveDTO(payload, busNo, 0)
    return await updatePurchaseInbound(busNo, saveDTO)
  } catch {
    return mockCreatePurchaseInbound(payload as any)
  }
}

/**
 * 保存已有出库单（保存并提交时也会走到这里）
 * 对接后端 POST /j6-pharmacy/stockout/save
 */
export async function updatePurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    // 如果 payload 已经是 StockOutSaveDTO 格式则直接使用，否则转换
    const saveDTO: StockOutSaveDTO = (payload as any).typeEnum
      ? (payload as unknown as StockOutSaveDTO)
      : buildSaveDTO(payload, id, (payload as any).version ?? 0)
    const res = await http.post<RawStockOutDetail>('/stockout/save', saveDTO, {
      baseURL,
      upType: DataUpType.json
    })
    return mapDetailResult(res.data!)
  } catch {
    return mockUpdatePurchaseInbound(id, payload as any)
  }
}

// ==================== 审批流操作 ====================

/**
 * 提交审批
 * 对接后端 POST /j6-pharmacy/stockout/submit-approval
 * 审批接口只返回 success/statusEnum，调用后重新拉取详情获取完整 order
 */
export async function submitPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StockOutApprovalResult>(
      '/stockout/submit-approval',
      { busNo, approverId: '308791351298756608' } satisfies StockOutSubmitDTO,
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
 * 对接后端 POST /j6-pharmacy/stockout/withdraw-approval
 */
export async function withdrawPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StockOutApprovalResult>(
      '/stockout/withdraw-approval',
      { busNo } satisfies StockOutWithdrawDTO,
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
 * 对接后端 POST /j6-pharmacy/stockout/agree
 */
export async function approvePurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StockOutApprovalResult>(
      '/stockout/agree',
      {
        busNo,
        approverId: '308791351298756608',
        approvalClock: new Date().toISOString()
      } satisfies StockOutApproveDTO,
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
 * 对接后端 POST /j6-pharmacy/stockout/reject
 */
export async function rejectPurchaseInbound(
  busNo: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<StockOutApprovalResult>(
      '/stockout/reject',
      { busNo, approverId: 'admin', rejectReason: reason } satisfies StockOutRejectDTO,
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
 * 对接后端 DELETE /j6-pharmacy/stockout/{busNo}/delete
 */
export async function removePurchaseInbound(busNo: string): Promise<void> {
  try {
    const http = useHttp()
    await http.delete(`/stockout/${encodeURIComponent(busNo)}/delete`, undefined, { baseURL })
  } catch {
    return mockRemovePurchaseInbound(busNo)
  }
}
