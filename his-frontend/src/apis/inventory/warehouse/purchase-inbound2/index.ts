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
  RawInboundDetail,
  RawInboundItem,
  InboundAddDTO,
  InboundApproveDTO,
  InboundApprovalResult,
  InboundDeleteDTO,
  InboundRejectDTO,
  InboundSaveDTO,
  InboundSubmitDTO,
  InboundWithdrawDTO
} from './type'
import { STATUS_MAP, STATUS_ENUM_MAP, STATUS_TEXT_MAP, toDateTime } from './utils'

// 复用公共 API 和类型，本地同名导出会覆写
export * from '@/apis/inventory/common/index'
export type * from '@/apis/inventory/common/type'

/** j6药库管理模块 baseURL */
const baseURL = '/j6-pharmacy'

/** 进货单固定枚举值 */
const TYPE_ENUM = 'CABINET_INBOUND_RECEIPT'
const CATEGORY_ENUM = 'PURCHASE'
const SOURCE_TYPE_ENUM = 'SUPPLIER'
const PURPOSE_TYPE_ENUM = 'WAREHOUSE'
const ITEM_TABLE_DEFAULT = 'med_medication_definition'

// ==================== 工具函数 ====================

/** 将后端 InboundReceiptDetailVO 映射为前端 PurchaseInboundDetailResult */
function mapDetailResult(data: RawInboundDetail): PurchaseInboundDetailResult {
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
    supplierId_dictText: data.supplierName,
    sourceLocationId: data.sourceLocationId,
    sourceLocationId_dictText: data.purposeLocationName,
    purposeLocationId: data.purposeLocationId,
    purposeLocationId_dictText: data.purposeLocationName,
    totalAmount: data.totalAmount
  }
  const details: PurchaseInboundDetail[] = (data.lines ?? []).map((line) => ({
    id: line.id,
    itemCode: line.itemId,
    itemName: line.productName,
    specification: line.specification,
    manufacturer: line.manufacturer,
    batchNo: line.lotNumber,
    bookQuantity: 0,
    actualQuantity: line.itemQuantity,
    actualUnit: line.unitCodeDictText,
    unitCode: line.unitCode,
    profitLoss: 0,
    unitPrice: line.price,
    actualAmount: line.totalPrice,
    profitLossAmount: 0,
    remark: line.remake ?? '',
    // 扩展字段：保存时需回传
    traceNo: line.traceNo
  }))
  return { order, details }
}

/** 将前端保存参数转为后端 InboundSaveDTO */
function buildSaveDTO(
  payload: Record<string, any>,
  busNo: string,
  version: number
): InboundSaveDTO {
  const targetStatus = payload.targetStatus as number | undefined
  return {
    busNo,
    originalBusNo: (payload.originalBusNo as string) || undefined,
    typeEnum: TYPE_ENUM,
    categoryEnum: CATEGORY_ENUM,
    statusEnum: targetStatus != null ? STATUS_ENUM_MAP[targetStatus] : undefined,
    sourceTypeEnum: SOURCE_TYPE_ENUM,
    sourceLocationId: (payload.sourceLocationId as string) || '',
    purposeTypeEnum: PURPOSE_TYPE_ENUM,
    purposeLocationId: (payload.purposeLocationId as string) || (payload.sourceLocationId as string) || (payload.warehouse as string) || '',
    purposeLocationName: (payload.purposeLocationId_dictText as string) || (payload.sourceLocationId_dictText as string) || (payload.warehouse as string) || '',
    supplierId: (payload.supplierId as string) || (payload.supplierId_dictText as string) || (payload.supplier as string) || '',
    supplierName: (payload.supplierId_dictText as string) || (payload.supplier as string) || '',
    practitionerId: (payload.practitionerId as string) || '',
    creatorId: (payload.creatorId as string) || '',
    creatorName: (payload.creatorName as string) || '',
    applicantId: (payload.applicantId as string) || '',
    applicantName: (payload.applicantName as string) || '',
    approverId: (payload.approverId as string) || '',
    approverName: (payload.approverName as string) || '',
    applyTime: (payload.applyTime as string) || '',
    approvalTime: (payload.approvalTime as string) || '',
    totalAmount: (payload.totalAmount as number) || 0,
    reason: (payload.remark as string) || '',
    version,
    lines: ((payload.details || []) as any[]).map((d) => ({
      itemTable: ITEM_TABLE_DEFAULT,
      itemId: (d.itemCode as string) || '',
      itemQuantity: Number(d.actualQuantity) || 0,
      unitCode: (d.unitCode as string) || '',
      productName: (d.itemName as string) || '',
      specification: (d.specification as string) || '',
      manufacturer: (d.manufacturer as string) || '',
      lotNumber: (d.batchNo as string) || '',
      traceNo: (d.traceNo as string) || '',
      price: Number(d.unitPrice) || 0,
      totalPrice: Number(d.actualAmount) || 0,
      occurrenceTime: (d.occurrenceTime as string) || '',
      startTime: '',
      endTime: '',
      remake: (d.remark as string) || ''
    }))
  }
}

// ==================== 列表查询 ====================

/**
 * 分页查询进货单列表（左侧卡片队列）
 * 对接后端 GET /j6-pharmacy/inbound/list
 */
export async function pagePurchaseInboundOrders(
  query: Record<string, any>
): Promise<PageResult<PurchaseInboundOrder>> {
  try {
    const http = useHttp()
    const res = await http.get<RawInboundItem[]>(
      '/inbound/list',
      {
        keyword: query.checkNo || query.busNokey,
        applyTimeFrom: toDateTime(query.startDate, false),
        applyTimeTo: toDateTime(query.endDate, true),
        purposeLocationId: query.checkType,
        pageIndex: String(query.pageIndex),
        pageSize: String(query.pageSize)
      },
      { baseURL }
    )
    const data = res.data ?? []
    const rows: PurchaseInboundOrder[] = data.map((item) => {
      const status: PurchaseInboundStatus = (STATUS_MAP[item.statusEnum] ?? 0) as PurchaseInboundStatus
      return {
        id: item.busNo,
        status,
        category: '',
        busNo: item.busNo,
        statusEnum: status,
        statusEnum_enumText: STATUS_TEXT_MAP[item.statusEnum] ?? '',
        applicantId_dictText: item.creatorName,
        applyTime: item.applyTime,
        approverId_dictText: item.approverName,
        approvalTime: item.approvalTime,
        supplierId_dictText: item.supplierName,
        sourceLocationId_dictText: item.purposeLocationName,
        purposeLocationId_dictText: item.purposeLocationName,
        totalAmount: item.totalAmount
      }
    })
    return { rows, total: data.length }
  } catch {
    return mockPagePurchaseInboundOrders(query as any)
  }
}

// ==================== 详情查询 ====================

/**
 * 获取进货单详情（右侧主面板）
 * 对接后端 GET /j6-pharmacy/inbound/detail?busNo=
 */
export async function getPurchaseInboundDetail(
  busNo: string
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.get<RawInboundDetail>(
      '/inbound/detail',
      { busNo },
      { baseURL }
    )
    return mapDetailResult(res.data!)
  } catch {
    return mockGetPurchaseInboundDetail(busNo)
  }
}

// ==================== 新增（空白草稿） ====================

/**
 * 新增空白进货单草稿
 * 对接后端 POST /j6-pharmacy/inbound/add
 */
async function addInbound(params: InboundAddDTO): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    const res = await http.post<RawInboundDetail>(
      '/inbound/add',
      params,
      { baseURL, upType: DataUpType.json }
    )
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
 * 新增并保存进货单（兼容旧流程：add → save）
 * 对接后端 POST /j6-pharmacy/inbound/add → PUT /j6-pharmacy/inbound/save
 */
export async function createPurchaseInbound(
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    // 第一步：新增空白草稿，获取 busNo
    const addResult = await addInbound({
      purposeLocationId: (payload.purposeLocationId as string) || (payload.sourceLocationId as string) || (payload.warehouse as string) || '',
      supplierId: (payload.supplierId as string) || (payload.supplierId_dictText as string) || (payload.supplier as string) || '',
      practitionerId: (payload.practitionerId as string) || '',
      originalBusNo: (payload.originalBusNo as string) || undefined,
      reason: (payload.remark as string) || ''
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
 * 保存已有进货单
 * 对接后端 PUT /j6-pharmacy/inbound/save
 */
export async function updatePurchaseInbound(
  id: string,
  payload: Record<string, any>
): Promise<PurchaseInboundDetailResult> {
  try {
    const http = useHttp()
    // 如果 payload 已经是 InboundSaveDTO 格式则直接使用，否则转换
    const saveDTO: InboundSaveDTO = (payload as any).typeEnum === TYPE_ENUM
      ? (payload as unknown as InboundSaveDTO)
      : buildSaveDTO(payload, id, (payload as any).version ?? (payload as any).rowVersion ?? 0)
    const res = await http.put<RawInboundDetail>(
      '/inbound/save',
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
 * 对接后端 POST /j6-pharmacy/inbound/submitInboundReceiptApproval
 * 审批接口只返回 success/statusEnum，调用后重新拉取详情获取完整 order
 */
export async function submitPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<InboundApprovalResult>(
      '/inbound/submitInboundReceiptApproval',
      { busNo, approverId: 'admin' } satisfies InboundSubmitDTO,
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
 * 对接后端 POST /j6-pharmacy/inbound/withdrawInboundReceiptApproval
 */
export async function withdrawPurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<InboundApprovalResult>(
      '/inbound/withdrawInboundReceiptApproval',
      { busNo } satisfies InboundWithdrawDTO,
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
 * 对接后端 POST /j6-pharmacy/inbound/agreeInboundReceipt
 */
export async function approvePurchaseInbound(busNo: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<InboundApprovalResult>(
      '/inbound/agreeInboundReceipt',
      { busNo, approverId: 'admin' } satisfies InboundApproveDTO,
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
 * 对接后端 POST /j6-pharmacy/inbound/rejectInboundReceipt
 */
export async function rejectPurchaseInbound(busNo: string, reason: string): Promise<PurchaseInboundOrder> {
  try {
    const http = useHttp()
    await http.post<InboundApprovalResult>(
      '/inbound/rejectInboundReceipt',
      { busNo, rejectReason: reason, approverId: 'admin' } satisfies InboundRejectDTO,
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
 * 对接后端 DELETE /j6-pharmacy/inbound/remove
 * 注意：此接口需将 busNo 放入请求体
 */
export async function removePurchaseInbound(busNo: string): Promise<void> {
  try {
    const http = useHttp()
    await http.delete(
      '/inbound/remove',
      { busNo } satisfies InboundDeleteDTO,
      { baseURL }
    )
  } catch {
    return mockRemovePurchaseInbound(busNo)
  }
}
