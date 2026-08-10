import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NurseBatchResultVO,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  PerformNurseExecutionsDTO
} from './type'

/** 门诊护士站独立服务地址；开发环境由 Vite 代理到 j5-ons。 */
export const outpatientNurseStationApiBaseURL =
  import.meta.env.VITE_OUTPATIENT_NURSE_STATION_API_URL || '/j5-ons'

/** 仅显式配置 real 时访问门诊护士站后端，其余环境使用可交互 Mock。 */
export const isOutpatientNurseStationRealApiEnabled =
  import.meta.env.VITE_OUTPATIENT_NURSE_STATION_API_MODE === 'real'

interface BackendPage<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
}

interface BackendPatientQueueRow {
  id?: string
  appointment_kind?: string
  bus_no?: string
  patient_id?: string
  patient_name?: string
  patient_gender?: string
  patient_age?: string
  encounter_id?: string
  organization_id?: string
  destination_location_id?: string
  destination_location_name?: string
  triage_level?: string
  triage_level_name?: string
  triage_priority_code?: string
  triage_time?: string
  display_order?: number
  service_type_code?: string
  service_type_name?: string
  appointment_type_name?: string
  start_instant?: string
  booking_date?: string
  triage_status?: string
  triage_status_name?: string
  created_at?: string
}

interface BackendExecutionRecord {
  id?: string
  bus_no?: string
  status_enum?: string
  status_name?: string
  category_enum?: string
  category_name?: string
  priority_name?: string
  item_name?: string
  quantity?: number
  unit_code?: string
  unit_name?: string
  patient_id?: string
  encounter_id?: string
  authored_time?: string
  performer_name?: string
  check_time?: string
  occurrence_start_time?: string
  occurrence_end_time?: string
  remark?: string
  execution_count?: number
  execution_status?: number
  execution_status_name?: string
  created_at?: string
}

interface BackendConsumableItem {
  id?: string
  busNo?: string
  deviceReqId?: string
  statusEnum?: string
  quantity?: number
  unitCode?: string
  lotNumber?: string
  deviceDefId?: string
  patientId?: string
  encounterId?: string
  locationId?: string
  partTypeCode?: string
  dispenseQuantity?: number
  dispenseTime?: string
  desLocationId?: string
  supportInfo?: string
  usageInstruction?: string
  traceNo?: string
}

interface BackendResponse<T> {
  code?: number
  message?: string
  data?: T | null
}

function getRealHttp() {
  return useHttp()
}

function assertSuccess(response: BackendResponse<unknown>) {
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '门诊护士站后端处理失败')
  }
}

function unwrapData<T>(response: BackendResponse<T>): T {
  assertSuccess(response)
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '门诊护士站后端未返回有效数据')
  }
  return response.data
}

function text(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function number(value: unknown, fallback = 0) {
  return typeof value === 'number' && Number.isFinite(value) ? value : fallback
}

function mapGender(value?: string) {
  const normalized = value?.toUpperCase()
  if (normalized === 'MALE' || normalized === 'M' || value === '1') return '男'
  if (normalized === 'FEMALE' || normalized === 'F' || value === '2') return '女'
  if (normalized === 'UNKNOWN' || normalized === 'UNDETERMINED') return '未知'
  return value || '未知'
}

function mapPatientStatus(value?: string): NursePatientQueueItemVO['status'] {
  const status = value?.toUpperCase()
  if (['COMPLETED', 'FINISHED', 'CANCELLED', 'CANCELED'].includes(status ?? '')) {
    return 'COMPLETED'
  }
  if (['READY', 'CALLED', 'IN_PROGRESS', 'TRIAGED'].includes(status ?? '')) return 'READY'
  return 'WAITING'
}

function mapPatientQueueRow(row: BackendPatientQueueRow): NursePatientQueueItemVO {
  const encounterId = text(row.encounter_id) || text(row.id) || ''
  const patientId = text(row.patient_id) || ''
  const status = mapPatientStatus(row.triage_status)
  const rawAge = text(row.patient_age) || ''
  return {
    encounterId,
    patientId,
    patientName: text(row.patient_name) || '未提供患者姓名',
    genderName: mapGender(row.patient_gender),
    ageText: rawAge && !/[岁月天]/.test(rawAge) ? `${rawAge}岁` : rawAge || '未知',
    // 当前后端未返回独立就诊号，优先显示队列业务号，其次显示 encounterId。
    visitNo: text(row.bus_no) || encounterId,
    queueNumber: number(row.display_order),
    registeredAt:
      text(row.triage_time) ||
      text(row.start_instant) ||
      text(row.created_at) ||
      text(row.booking_date) ||
      '',
    departmentName: text(row.destination_location_name) || '未提供科室',
    visitTypeName:
      text(row.service_type_name) ||
      text(row.appointment_type_name) ||
      text(row.appointment_kind) ||
      '门诊',
    status,
    statusName: text(row.triage_status_name) || '',
    priorityName: text(row.triage_level_name) || text(row.triage_priority_code)
  }
}

function mapExecutionStatus(row: BackendExecutionRecord): NurseExecutionItemVO['status'] {
  if (row.execution_status === 1) return 'COMPLETED'
  if (row.execution_status === 2) return 'CANCELLED'
  const display = `${row.execution_status_name ?? ''} ${row.status_name ?? ''}`
  if (/取消|作废/.test(display)) return 'CANCELLED'
  if (/已执行|完成/.test(display) && row.execution_status === undefined) return 'COMPLETED'
  return 'PENDING'
}

function mapExecutionRecord(row: BackendExecutionRecord): NurseExecutionItemVO {
  const itemId = text(row.id) || text(row.bus_no) || ''
  const status = mapExecutionStatus(row)
  return {
    itemId,
    encounterId: text(row.encounter_id) || '',
    categoryCode: text(row.category_enum),
    categoryName: text(row.category_name),
    itemName: text(row.item_name) || '',
    quantity: row.quantity,
    unitName: text(row.unit_name) || text(row.unit_code),
    status,
    statusName: text(row.execution_status_name) || text(row.status_name) || '',
    executionCount: number(row.execution_count),
    executorName: text(row.performer_name),
    executedAt:
      text(row.occurrence_end_time) || text(row.occurrence_start_time) || text(row.check_time),
    remark: text(row.remark)
  }
}

function mapConsumable(row: BackendConsumableItem): NurseConsumableItemVO {
  return {
    deviceDefId: text(row.deviceDefId) || '',
    quantity: row.quantity,
    unitCode: text(row.unitCode),
    usageInstruction: text(row.usageInstruction),
    lotNumber: text(row.lotNumber),
    statusEnum: text(row.statusEnum),
    dispenseTime: text(row.dispenseTime),
    supportInfo: text(row.supportInfo)
  }
}

function queueParams(
  query: NursePatientQueueQuery,
  pageIndex = query.pageIndex,
  pageSize = query.pageSize
) {
  return {
    booking_date: query.bookingDate,
    pageIndex,
    pageSize
  }
}

async function fetchPatientQueuePage(
  query: NursePatientQueueQuery,
  pageIndex?: number,
  pageSize?: number
) {
  const response = await getRealHttp().get<BackendPage<BackendPatientQueueRow>>(
    '/ons/patient-queue',
    queueParams(query, pageIndex, pageSize),
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  return unwrapData(response)
}

/** 按后端支持的预约日期分页查询真实患者队列。 */
export async function realPageNursePatientQueue(
  query: NursePatientQueueQuery
): Promise<PageDTO<NursePatientQueueItemVO>> {
  const page = await fetchPatientQueuePage(query)
  const rows = (page.rows ?? []).map(mapPatientQueueRow)
  const pageSize = number(page.pageSize, query.pageSize)
  const total = number(page.total, rows.length)
  return {
    pageIndex: number(page.pageIndex, query.pageIndex),
    pageSize,
    total,
    pages: number(page.pages, Math.ceil(total / Math.max(pageSize, 1))),
    rows
  }
}

/** 按患者读取执行项目，并在前端按当前 encounterId 收窄。 */
export async function realGetNurseExecutionItems(patientId: string, encounterId?: string) {
  const response = await getRealHttp().get<BackendExecutionRecord[]>(
    `/ons/patient/${encodeURIComponent(patientId)}/service-requests`,
    undefined,
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  return unwrapData(response)
    .filter((row) => !encounterId || !row.encounter_id || row.encounter_id === encounterId)
    .map(mapExecutionRecord)
}

/** 按患者读取耗材，并在前端按当前 encounterId 收窄。 */
export async function realGetNurseConsumables(patientId: string, encounterId?: string) {
  const response = await getRealHttp().get<BackendConsumableItem[]>(
    `/ons/patient/${encodeURIComponent(patientId)}/consumable-items`,
    undefined,
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  return unwrapData(response)
    .filter((row) => !encounterId || !row.encounterId || row.encounterId === encounterId)
    .map(mapConsumable)
}

/** 批量执行真实服务请求。 */
export async function realPerformNurseExecutions(
  request: PerformNurseExecutionsDTO
): Promise<NurseBatchResultVO> {
  const recordIds = request.items.map((item) => item.itemId)
  const response = await getRealHttp().post<NurseBatchResultVO>(
    '/ons/execute',
    {
      record_ids: recordIds,
      encounter_id: request.encounterId,
      executor_id: request.executorId,
      executor_name: request.executorName,
      execute_time: request.executeTime,
      idempotency_key: request.idempotencyKey,
      execute_type: request.executeType,
      remark: request.remark
    },
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  return unwrapData(response)
}

/** 批量取消真实执行记录；页面当前一次提交一项。 */
export async function realCancelNurseExecution(
  request: CancelNurseExecutionDTO
): Promise<NurseBatchResultVO> {
  const recordIds = [request.item.itemId]
  const response = await getRealHttp().post<NurseBatchResultVO>(
    '/ons/cancel-execute',
    {
      record_ids: recordIds,
      encounter_id: request.encounterId,
      cancel_reason: request.reason,
      cancel_reason_code: request.reasonCode,
      operator_id: request.operatorId,
      operator_name: request.operatorName,
      idempotency_key: request.idempotencyKey
    },
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  return unwrapData(response)
}

function historyParams(
  query: NurseExecutionHistoryQuery,
  pageIndex = query.pageIndex,
  pageSize = query.pageSize
) {
  return {
    start_date: query.startDate,
    end_date: query.endDate,
    pageIndex,
    pageSize
  }
}

/** 分页读取患者执行记录，查询条件直接使用后端支持的字段。 */
export async function realPagePatientExecutionHistory(
  patientId: string,
  query: NurseExecutionHistoryQuery
): Promise<PageDTO<NurseExecutionRecordVO>> {
  const response = await getRealHttp().get<BackendPage<BackendExecutionRecord>>(
    '/ons/service-requests',
    { patient_id: patientId, ...historyParams(query) },
    { baseURL: outpatientNurseStationApiBaseURL }
  )
  const page = unwrapData(response)
  const rows = (page.rows ?? []).map((row) => ({
    ...mapExecutionRecord(row),
    encounterId: text(row.encounter_id) || '',
    visitNo: text(row.encounter_id) || text(row.bus_no) || ''
  }))
  const pageSize = number(page.pageSize, query.pageSize)
  const total = number(page.total, rows.length)
  return {
    pageIndex: number(page.pageIndex, query.pageIndex),
    pageSize,
    total,
    pages: number(page.pages, Math.ceil(total / Math.max(pageSize, 1))),
    rows
  }
}
