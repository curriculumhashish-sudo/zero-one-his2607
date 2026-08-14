import dayjs from 'dayjs'

import { useHttp } from '@/plugins/http'
import { useUserStore } from '@/stores/user'

import type { PageDTO } from '../../type'
import type {
  CreateInpatientMedicalOrderDTO,
  InpatientMedicalOrderBatchActionDTO,
  InpatientMedicalOrderBatchResultVO,
  InpatientMedicalOrderQuery,
  InpatientMedicalOrderRecordVO,
  InpatientMedicalOrderTherapyType,
  UpdateInpatientMedicalOrderDTO
} from './type'

const realApiBaseURL =
  import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'

interface BackendOrderRow {
  request_id?: string
  tenant_id?: string
  encounter_id?: string
  patient_id?: string
  group_id?: string
  status?: string
  medicine_type?: string
  therapy_type?: string
  medicine_name?: string
  specification?: string
  dose?: string
  dose_unit?: string
  method?: string
  rate?: string
  taotal_price?: string
  description?: string
  authored_by?: string
  skin_text?: boolean
  product_table_name?: string
}

interface BackendOrderPageDTO {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendOrderRow[]
}

const categoryNames = {
  DRUG: '药品',
  LAB: '检验',
  EXAM: '检查',
  TREATMENT: '治疗'
} as const

const statusNames = {
  DRAFT: '待签发',
  SIGNED: '已签发',
  STOPPED: '已停嘱',
  WITHDRAWN: '已撤回'
} as const

function unwrapData<T>(response: { data?: T; message?: string }): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '后端未返回有效医嘱数据')
  }
  return response.data
}

function toOrderError(error: unknown, fallback: string): Error {
  if (error instanceof Error) return error
  if (error && typeof error === 'object' && 'message' in error) {
    const message = (error as { message?: unknown }).message
    if (typeof message === 'string' && message.trim()) return new Error(message)
  }
  return new Error(fallback)
}

function getTenantId() {
  const tenantId = useUserStore().tenantId
  if (tenantId === null || tenantId === undefined || String(tenantId).trim() === '') {
    throw new Error('当前登录上下文缺少租户 ID，无法访问住院医嘱')
  }
  return String(tenantId)
}

function currentOperator() {
  const user = useUserStore().getUser
  return user?.id || user?.username || 'current-user'
}

function normalizeCategory(value?: string): InpatientMedicalOrderRecordVO['category'] {
  const normalized = value?.trim().toUpperCase() ?? ''
  if (normalized.includes('DRUG') || normalized.includes('MEDICINE') || normalized.includes('药')) {
    return 'DRUG'
  }
  if (normalized.includes('LAB') || normalized.includes('检验')) return 'LAB'
  if (normalized.includes('EXAM') || normalized.includes('检查')) return 'EXAM'
  return 'TREATMENT'
}

function normalizeTherapyType(value?: string): InpatientMedicalOrderTherapyType {
  const normalized = value?.trim().toUpperCase() ?? ''
  return normalized.includes('LONG') || normalized === '1' || value?.includes('长期')
    ? 'LONG_TERM'
    : 'TEMPORARY'
}

function normalizeStatus(value?: string): InpatientMedicalOrderRecordVO['status'] {
  const normalized = value?.trim().toUpperCase() ?? ''
  if (normalized === 'SIGNED' || normalized === 'ACTIVE') return 'SIGNED'
  if (normalized === 'STOPPED') return 'STOPPED'
  if (normalized === 'WITHDRAWN' || normalized === 'CANCELLED') return 'WITHDRAWN'
  return 'DRAFT'
}

function parseNumber(value?: string): number | undefined {
  if (value === undefined || value.trim() === '') return undefined
  const parsed = Number(value)
  return Number.isFinite(parsed) ? parsed : undefined
}

function mapOrderRow(
  row: BackendOrderRow,
  index: number,
  query: InpatientMedicalOrderQuery
): InpatientMedicalOrderRecordVO {
  const category = normalizeCategory(row.medicine_type)
  const therapyType = normalizeTherapyType(row.therapy_type)
  const status = normalizeStatus(row.status)
  const id = row.request_id?.trim() || `real-order-${query.patientId || 'unknown'}-${index}`
  const encounterId = row.encounter_id || query.encounterId
  const encounterMismatch = Boolean(query.encounterId && encounterId !== query.encounterId)

  return {
    id,
    source: 'real',
    encounterId,
    patientId: row.patient_id || query.patientId || '',
    catalogId: id,
    code: row.medicine_type || id,
    name: row.medicine_name || row.description || '未命名医嘱',
    category,
    categoryName: categoryNames[category],
    specification: row.specification || undefined,
    dose: parseNumber(row.dose),
    unit: row.dose_unit || undefined,
    frequency: row.rate || undefined,
    route: row.method || undefined,
    quantity: 1,
    quantityUnit: '项',
    remark: row.description || undefined,
    therapyType,
    therapyTypeName: therapyType === 'LONG_TERM' ? '长期' : '临时',
    groupId: row.group_id || undefined,
    status,
    statusName: statusNames[status],
    orderNo: index + 1,
    authoredAt: '',
    updatedAt: '',
    version: 0,
    productTableName: row.product_table_name,
    readonly: encounterMismatch || !row.request_id,
    encounterMismatch,
    rawStatus: row.status,
    authoredByName: row.authored_by,
    totalPrice: row.taotal_price,
    skinTest: row.skin_text
  }
}

function mapPage(
  page: BackendOrderPageDTO,
  query: InpatientMedicalOrderQuery
): PageDTO<InpatientMedicalOrderRecordVO> {
  const rows = (page.rows ?? []).map((row, index) => mapOrderRow(row, index, query))
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? rows.length,
    pages: page.pages ?? Math.ceil((page.total ?? rows.length) / Math.max(query.pageSize, 1)),
    rows
  }
}

function toStringValue(value: number | undefined) {
  return value === undefined ? '' : String(value)
}

function buildCreatePayload(data: CreateInpatientMedicalOrderDTO) {
  return {
    tenant_id: getTenantId(),
    encounter_id: data.encounterId,
    patient_id: data.patientId,
    group_id: '',
    status: 'DRAFT',
    medicine_type: data.category,
    therapy_type: data.therapyType,
    medicine_name: data.name,
    specification: data.specification || '',
    dose: toStringValue(data.dose),
    dose_unit: data.unit || '',
    method: data.route || '',
    rate: data.frequency || '',
    created_at: dayjs().format('YYYY-MM-DD HH:mm:ss'),
    created_by: currentOperator(),
    taotal_price: '0',
    description: data.remark || '',
    skin_text: false
  }
}

function buildUpdatePayload(
  orderId: string,
  data: UpdateInpatientMedicalOrderDTO,
  current: InpatientMedicalOrderRecordVO
) {
  return {
    request_id: orderId,
    medicine_type: data.category,
    therapy_type: data.therapyType,
    medicine_name: data.name,
    specification: data.specification || '',
    dose: toStringValue(data.dose),
    dose_unit: data.unit || '',
    method: data.route || '',
    rate: data.frequency || '',
    created_at: current.authoredAt || dayjs().format('YYYY-MM-DD HH:mm:ss'),
    created_by: current.authoredByName || currentOperator(),
    taotal_price: '0',
    description: data.remark || '',
    skin_text: current.skinTest ?? false
  }
}

/** 查询真实住院医嘱；按当前就诊和患者上下文请求。 */
export async function realPageInpatientMedicalOrders(
  query: InpatientMedicalOrderQuery
): Promise<PageDTO<InpatientMedicalOrderRecordVO>> {
  try {
    if (!query.patientId) throw new Error('缺少患者档案 ID，无法查询住院医嘱')
    const response = await useHttp().get<BackendOrderPageDTO>(
      '/enter-orders/get',
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        tenant_id: getTenantId(),
        patient_id: query.patientId,
        encounter_id: query.encounterId
      },
      { baseURL: realApiBaseURL }
    )
    return mapPage(unwrapData(response), query)
  } catch (error) {
    throw toOrderError(error, '住院医嘱加载失败')
  }
}

/** 新增真实住院医嘱草稿。 */
export async function realCreateInpatientMedicalOrder(
  data: CreateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  try {
    const response = await useHttp().post<string>('/enter-orders/add', buildCreatePayload(data), {
      baseURL: realApiBaseURL
    })
    const id = response.data?.trim()
    if (!id) throw new Error(response.message || '新增医嘱后端未返回医嘱 ID')
    const now = dayjs().format('YYYY-MM-DD HH:mm:ss')
    return {
      id,
      source: 'real',
      encounterId: data.encounterId,
      patientId: data.patientId,
      catalogId: data.catalogId,
      code: data.code,
      name: data.name,
      category: data.category,
      categoryName: data.categoryName,
      specification: data.specification,
      dose: data.dose,
      unit: data.unit,
      frequency: data.frequency,
      route: data.route,
      days: data.days,
      quantity: data.quantity,
      quantityUnit: data.quantityUnit,
      remark: data.remark,
      therapyType: data.therapyType,
      therapyTypeName: data.therapyType === 'LONG_TERM' ? '长期' : '临时',
      status: 'DRAFT',
      statusName: '待签发',
      orderNo: data.orderNo,
      authoredAt: now,
      updatedAt: now,
      version: 0,
      skinTest: false
    }
  } catch (error) {
    throw toOrderError(error, '新增住院医嘱失败')
  }
}

/** 修改真实住院医嘱草稿。 */
export async function realUpdateInpatientMedicalOrder(
  orderId: string,
  data: UpdateInpatientMedicalOrderDTO,
  current: InpatientMedicalOrderRecordVO
): Promise<InpatientMedicalOrderRecordVO> {
  try {
    const response = await useHttp().put<string>(
      '/enter-orders/modify',
      buildUpdatePayload(orderId, data, current),
      { baseURL: realApiBaseURL }
    )
    if (response.data === undefined && !response.message) {
      throw new Error('修改医嘱后端未返回结果')
    }
    return {
      ...current,
      ...data,
      status: 'DRAFT',
      statusName: '待签发',
      updatedAt: dayjs().format('YYYY-MM-DD HH:mm:ss'),
      source: 'real',
      version: current.version
    }
  } catch (error) {
    throw toOrderError(error, '修改住院医嘱失败')
  }
}

/** 批量删除真实住院医嘱草稿。 */
export async function realDeleteInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const ids = data.items.map((item) => item.id).filter(Boolean)
    if (ids.length === 0) return { updatedCount: 0, rows: [] }
    await useHttp().delete<string[]>('/enter-orders/delete', ids, {
      baseURL: realApiBaseURL
    })
    return { updatedCount: ids.length, rows: [] }
  } catch (error) {
    throw toOrderError(error, '删除住院医嘱失败')
  }
}
