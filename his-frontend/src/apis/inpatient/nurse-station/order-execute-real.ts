import { useHttp } from '@/plugins/http'

import { realApiBaseURL } from './patient-real'
import type {
  NurseStationExecuteOrdersDTO,
  NurseStationOrderExecuteBatchResultVO,
  NurseStationOrderExecuteListVO,
  NurseStationOrderExecuteQueryDTO,
  NurseStationOrderExecuteRowVO
} from './order-execute-type'

/** Enable the real execution API without switching unrelated nurse-station modules. */
export const isNurseStationOrderExecuteRealApiEnabled =
  import.meta.env.VITE_NURSE_STATION_ORDER_EXECUTE_API_MODE === 'real'

interface BackendExecutionItem {
  id?: string
  busNo?: string
  prescriptionNo?: string
  statusEnum?: string
  categoryEnum?: string
  intentEnum?: string
  priorityEnum?: string
  patientId?: string
  encounterId?: string
  authoredTime?: string
  quantity?: number
  unitCode?: string
  requesterId?: string
  performerId?: string
  locationId?: string
  executionCount?: number
  remark?: string
  entrustContent?: string
  reasonText?: string
}

interface BackendExecutionPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendExecutionItem[]
}

function getRealHttp() {
  return useHttp()
}

function readString(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function mapExecutionItem(
  item: BackendExecutionItem,
  fallbackEncounterId: string
): NurseStationOrderExecuteRowVO | undefined {
  const id = readString(item.id)
  if (!id) return undefined
  return {
    listRowKey: id,
    id,
    busNo: readString(item.busNo),
    prescriptionNo: readString(item.prescriptionNo),
    statusEnum: readString(item.statusEnum) || 'verified',
    categoryEnum: readString(item.categoryEnum),
    intentEnum: readString(item.intentEnum),
    priorityEnum: readString(item.priorityEnum),
    patientId: readString(item.patientId),
    encounterId: readString(item.encounterId) || fallbackEncounterId,
    authoredTime: readString(item.authoredTime),
    quantity: typeof item.quantity === 'number' ? item.quantity : undefined,
    unitCode: readString(item.unitCode),
    requesterId: readString(item.requesterId),
    performerId: readString(item.performerId),
    locationId: readString(item.locationId),
    executionCount: typeof item.executionCount === 'number' ? item.executionCount : undefined,
    remark: readString(item.remark),
    entrustContent: readString(item.entrustContent),
    reasonText: readString(item.reasonText)
  }
}

async function queryEncounter(encounterId: string, pageSize: number) {
  const orders: NurseStationOrderExecuteRowVO[] = []
  let pageIndex = 1
  let total = 0

  do {
    const response = await getRealHttp().get<BackendExecutionPage>(
      '/medical-order/execution-items',
      {
        pageIndex,
        pageSize,
        statusEnum: 'verified',
        encounterId,
        patientId: '',
        startDate: '',
        endDate: '',
        locationId: '',
        categoryEnum: '',
        priorityEnum: '',
        intentEnum: ''
      },
      { baseURL: realApiBaseURL }
    )
    const page = response.data
    if (!page) throw new Error(response.message || '后端未返回医嘱执行数据')
    const rows = (page.rows ?? [])
      .map((item) => mapExecutionItem(item, encounterId))
      .filter((item): item is NurseStationOrderExecuteRowVO => Boolean(item))
    orders.push(...rows)
    total = page.total ?? orders.length
    pageIndex += 1
    if (!rows.length) break
  } while (orders.length < total)

  return orders
}

/** Query verified service requests and restore the selected patient grouping. */
export async function realQueryNurseStationOrderExecutions(
  query: NurseStationOrderExecuteQueryDTO
): Promise<NurseStationOrderExecuteListVO> {
  const encounterIds = [...new Set(query.encounterIds.map((id) => id.trim()).filter(Boolean))]
  if (encounterIds.length < 1 || encounterIds.length > 50) {
    throw new Error('一次只能查询 1～50 位患者')
  }
  const pageSize = Math.min(Math.max(query.pageSize ?? 100, 1), 100)
  const orderGroups = await Promise.all(
    encounterIds.map((encounterId) => queryEncounter(encounterId, pageSize))
  )
  return {
    groups: encounterIds.map((encounterId, index) => ({
      encounterId,
      orders: orderGroups[index] ?? []
    }))
  }
}

function errorMessage(error: unknown) {
  return error instanceof Error ? error.message : '开始执行失败'
}

/** Adapt a local batch selection to the backend single-order start endpoint. */
export async function realExecuteNurseStationOrders(
  data: NurseStationExecuteOrdersDTO
): Promise<NurseStationOrderExecuteBatchResultVO> {
  const performerId = data.performerId.trim()
  const serviceRequestIds = [
    ...new Set(data.serviceRequestIds.map((id) => id.trim()).filter(Boolean))
  ]
  if (!performerId) throw new Error('当前登录用户缺少执行人 ID')
  if (serviceRequestIds.length < 1 || serviceRequestIds.length > 50) {
    throw new Error('单次只能处理 1～50 条医嘱')
  }

  const results = []
  for (const serviceRequestId of serviceRequestIds) {
    try {
      const response = await getRealHttp().post<string>(
        '/medical-order/execute-order',
        { serviceRequestId, performerId },
        { baseURL: realApiBaseURL }
      )
      results.push({
        serviceRequestId,
        success: true,
        message: readString(response.data) || response.message || '开始执行成功'
      })
    } catch (error) {
      results.push({ serviceRequestId, success: false, message: errorMessage(error) })
    }
  }
  const processedCount = results.filter((item) => item.success).length
  return {
    success: processedCount === results.length,
    processedCount,
    results
  }
}
