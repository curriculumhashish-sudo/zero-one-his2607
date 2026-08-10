import { useHttp } from '@/plugins/http'

import { realApiBaseURL } from './patient-real'
import type {
  NurseStationOrderAuditBatchResultVO,
  NurseStationOrderAuditListVO,
  NurseStationOrderAuditOrderType,
  NurseStationOrderAuditPatientContextDTO,
  NurseStationOrderAuditQueryDTO,
  NurseStationOrderAuditRowVO,
  NurseStationRejectOrdersDTO,
  NurseStationVerifyOrdersDTO
} from './order-audit-type'

interface BackendAuditRecord {
  requestId: string
  orderType: NurseStationOrderAuditOrderType
  name: string
  isPassed: string | boolean | number
  content: string
  type: string
}

interface BackendAuditBatchResult {
  success: boolean
  processedCount: number
}

function getRealHttp() {
  return useHttp()
}

function readRequiredString(value: unknown, fieldName: string) {
  if (typeof value !== 'string' || !value.trim()) {
    throw new Error(`后端未返回有效的 ${fieldName}`)
  }
  return value.trim()
}

function isOrderType(value: unknown): value is NurseStationOrderAuditOrderType {
  return value === 'MED_REQUEST' || value === 'SERVICE_REQUEST'
}

function parseRecord(value: unknown): BackendAuditRecord {
  if (!value || typeof value !== 'object') {
    throw new Error('后端返回了无效的医嘱记录')
  }

  const record = value as Record<string, unknown>
  const requestId = readRequiredString(record.requestId, 'requestId')
  const name = readRequiredString(record.name, 'name')
  const orderType = record.orderType
  if (!isOrderType(orderType)) {
    throw new Error('后端返回了不支持的 orderType')
  }
  if (!['string', 'boolean', 'number'].includes(typeof record.isPassed)) {
    throw new Error('后端未返回有效的 isPassed')
  }
  if (typeof record.content !== 'string' || typeof record.type !== 'string') {
    throw new Error('后端返回的医嘱内容字段不完整')
  }

  return {
    requestId,
    orderType,
    name,
    isPassed: record.isPassed as string | boolean | number,
    content: record.content,
    type: record.type
  }
}

function parseRecords(data: unknown) {
  if (!Array.isArray(data)) {
    throw new Error('后端未返回医嘱列表')
  }
  return data.map(parseRecord)
}

function mapRecord(record: BackendAuditRecord): NurseStationOrderAuditRowVO {
  return {
    listRowKey: `${record.orderType}:${record.requestId}`,
    requestId: record.requestId,
    orderType: record.orderType,
    content: record.content
  }
}

function assertDistinctPatientNames(contexts: NurseStationOrderAuditPatientContextDTO[]) {
  const names = new Set<string>()
  for (const context of contexts) {
    const patientName = context.patientName.trim()
    if (!patientName) throw new Error('所选患者缺少有效姓名')
    if (names.has(patientName)) {
      throw new Error('当前后端无法安全查询同名患者，请调整选择后重试')
    }
    names.add(patientName)
  }
}

async function queryByPatient(
  context: NurseStationOrderAuditPatientContextDTO,
  auditStatus: NurseStationOrderAuditQueryDTO['auditStatus']
) {
  const response = await getRealHttp().get<BackendAuditRecord[]>(
    '/medicalOrderVerification/query',
    {
      name: context.patientName,
      isPassed: auditStatus === 'PASSED' ? 'true' : 'false'
    },
    { baseURL: realApiBaseURL }
  )
  const records = parseRecords(response.data)
  if (records.some((record) => record.name !== context.patientName.trim())) {
    throw new Error('后端返回了非当前患者的医嘱，已阻止展示')
  }
  return records
}

/** Query the legacy endpoint once for each selected patient. */
export async function realQueryNurseStationOrderAudits(
  query: NurseStationOrderAuditQueryDTO
): Promise<NurseStationOrderAuditListVO> {
  assertDistinctPatientNames(query.patientContexts)

  const groups = []
  for (const context of query.patientContexts) {
    const records = await queryByPatient(context, query.auditStatus)
    groups.push({
      encounterId: context.encounterId,
      orders: records.map(mapRecord)
    })
  }
  return { groups }
}

function parseBatchResult(data: unknown): NurseStationOrderAuditBatchResultVO {
  if (!data || typeof data !== 'object') {
    throw new Error('后端未返回有效的批量处理结果')
  }

  const result = data as Partial<BackendAuditBatchResult>
  if (typeof result.success !== 'boolean' || typeof result.processedCount !== 'number') {
    throw new Error('后端返回的批量处理结果不完整')
  }
  return {
    success: result.success,
    processedCount: result.processedCount
  }
}

/** Verify selected medical orders with the minimal backend payload. */
export async function realVerifyNurseStationOrders(
  data: NurseStationVerifyOrdersDTO
): Promise<NurseStationOrderAuditBatchResultVO> {
  const response = await getRealHttp().post<BackendAuditBatchResult>(
    '/medicalOrderVerification/verify-pass',
    data,
    { baseURL: realApiBaseURL }
  )
  return parseBatchResult(response.data)
}

/** Reject selected medical orders with a required reason. */
export async function realRejectNurseStationOrders(
  data: NurseStationRejectOrdersDTO
): Promise<NurseStationOrderAuditBatchResultVO> {
  const response = await getRealHttp().post<BackendAuditBatchResult>(
    '/medicalOrderVerification/reject-orders',
    data,
    { baseURL: realApiBaseURL }
  )
  return parseBatchResult(response.data)
}
