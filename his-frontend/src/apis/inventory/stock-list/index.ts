import { useHttp, DataUpType } from '@/plugins/http'
import type { StockListQuery, PageResult, StockListRow, RawInventoryDetailVO, InventoryStatusDTO } from './type'
import { mockQueryStockList, mockRetireItem, mockResumeItem } from './mock'

/** j6药库管理模块 baseURL */
const baseURL = '/j6-pharmacy'

// ==================== 工具函数 ====================

/** 将后端 InventoryDetailVO 映射为前端 StockListRow */
function mapRow(raw: RawInventoryDetailVO): StockListRow {
  return {
    id: raw.id,
    productBusNo: raw.productBusNo,
    name: raw.name,
    merchandiseName: raw.merchandiseName,
    categoryLabel: raw.categoryLabel,
    lotNumber: raw.lotNumber,
    packageStockText: raw.packageStockText,
    splitStockText: raw.splitStockText,
    packageUnitRatio: raw.packageUnitRatio,
    warehouseId: raw.warehouseId,
    warehouseName: raw.warehouseName,
    supplierId: raw.supplierId,
    supplierName: raw.supplierName,
    totalVolume: raw.specification,
    manufacturer: raw.manufacturer,
    doseFormLabel: raw.doseFormLabel,
    approvalNumber: raw.approvalNumber ?? null,
    ybNo: raw.ybNo ?? null,
    expirationDate: raw.expirationDate,
    productionDate: raw.productionDate,
    remainingDays: raw.remainingDays,
    inventoryStatusLabel: raw.inventoryStatusLabel,
    inventoryStatusEnum: raw.inventoryStatusEnum,
    chrgitmLvLabel: raw.chrgitmLvLabel ?? null,
    chrgitmLv: raw.chrgitmLv,
    lotCostPrice: raw.lotCostPrice,
    lotSalePrice: raw.lotSalePrice,
    lineCostTotal: raw.lineCostTotal,
    linePriceTotal: raw.linePriceTotal,
    traceNo: raw.traceNo ?? null,
    medicationCategoryCode: raw.medicationCategoryCode,
    deviceCategoryCode: raw.deviceCategoryCode
  }
}

// ==================== 列表查询 ====================

/**
 * 分页查询库存明细
 * 对接后端 POST /j6-pharmacy/inventory/pageInventoryDetailCurrent
 */
export async function queryStockList(query: StockListQuery): Promise<PageResult<StockListRow>> {
  try {
    const http = useHttp()
    const res = await http.post<{ records: RawInventoryDetailVO[]; total: number }>(
      '/inventory/pageInventoryDetailCurrent',
      {
        pageIndex: query.pageNo,
        pageSize: query.pageSize,
        keyword: query.keyword,
        medicationCategoryCodes: query.medicationCategoryCodes,
        deviceCategoryCodes: query.deviceCategoryCodes,
        warehouseIds: query.warehouseIds,
        supplierIds: query.supplierIds,
        chrgitmLvList: query.chrgitmLvList,
        zeroStockOnly: query.zeroStockOnly,
        expireWithinDays: query.expireWithinDays,
        inventoryStatusEnums: query.inventoryStatusEnums
      },
      { baseURL, upType: DataUpType.json }
    )
    const data = res.data!
    return {
      rows: (data.records ?? []).map(mapRow),
      total: data.total ?? 0
    }
  } catch {
    return mockQueryStockList(query)
  }
}

// ==================== 停供/取消停供 ====================

/**
 * 批量修改商品停供状态（后端自行判断当前行状态进行切换）
 * 对接后端 POST /j6-pharmacy/inventory/resumeInventoryItems
 */
async function updateInventoryStatus(inventoryItemIds: string[]): Promise<void> {
  try {
    const http = useHttp()
    await http.post<number>(
      '/inventory/resumeInventoryItems',
      { inventoryItemIds } satisfies InventoryStatusDTO,
      { baseURL, upType: DataUpType.json }
    )
  } catch {
    // mock fallback handled per-call
    throw new Error('mock')
  }
}

/** 停供 */
export async function retireItem(id: string): Promise<void> {
  try {
    await updateInventoryStatus([id])
  } catch {
    return mockRetireItem(id)
  }
}

/** 取消停供 */
export async function resumeItem(id: string): Promise<void> {
  try {
    await updateInventoryStatus([id])
  } catch {
    return mockResumeItem(id)
  }
}

// ==================== 导出数据 ====================

/**
 * 导出库存明细 Excel
 * 对接后端 POST /j6-pharmacy/inventory/exportInventoryDetailCurrent
 * 返回 base64 文件内容
 */
export async function exportStockList(query: StockListQuery): Promise<Blob> {
  const http = useHttp()
  // 导出接口返回原始 Excel 二进制流（非 JsonVO），用 arraybuffer 接收后手动构建 Blob
  const response = await http.getInstance().request({
    method: 'post',
    url: '/inventory/exportInventoryDetailCurrent',
    baseURL,
    responseType: 'arraybuffer',
    upType: DataUpType.json,
    data: {
      keyword: query.keyword,
      medicationCategoryCodes: query.medicationCategoryCodes,
      deviceCategoryCodes: query.deviceCategoryCodes,
      warehouseIds: query.warehouseIds,
      supplierIds: query.supplierIds,
      chrgitmLvList: query.chrgitmLvList,
      zeroStockOnly: query.zeroStockOnly,
      expireWithinDays: query.expireWithinDays,
      inventoryStatusEnums: query.inventoryStatusEnums
    }
  } as any)
  return new Blob([response.data as ArrayBuffer], {
    type: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'
  })
}

export type * from './type'
