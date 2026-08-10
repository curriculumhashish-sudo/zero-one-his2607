import { useHttp, DataUpType } from '@/plugins/http'
import type {
  PageResult,
  PurchaseInboundOrder,
  PurchaseInboundStatus
} from '@/apis/inventory/common/type'
import { mockPagePurchaseInboundOrders } from '@/apis/inventory/common/mock'
import { STATUS_NAME_MAP, STATUS_TEXT } from './type.ts'
import type { PharmacyInboundRecord } from './type.ts'
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
