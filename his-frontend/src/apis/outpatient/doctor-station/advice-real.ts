import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  CreateDoctorAdviceOrderDTO,
  DeleteDoctorAdviceOrdersDTO,
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderHistoryDetailQuery,
  DoctorAdviceOrderHistoryEncounterVO,
  DoctorAdviceOrderHistoryQuery,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderPageVO,
  DoctorAdviceOrderQuery,
  DoctorAdviceOrderVO,
  GroupDoctorAdviceOrdersDTO,
  SaveDoctorAdviceOrderItemDTO,
  SignDoctorAdviceOrdersDTO,
  UngroupDoctorAdviceOrdersDTO,
  UpdateDoctorAdviceOrderDTO,
  WithdrawDoctorAdviceOrdersDTO
} from './type'

const realApiBaseURL = import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_URL || undefined

export const isDoctorAdviceRealApiEnabled =
  import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_MODE === 'real'

interface BackendJsonVO<T> {
  code?: number
  message?: string
  data?: T | null
}

interface BackendPage<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
  data?: T[]
  encounterId?: string
  version?: number
  updatedAt?: string
}

interface BackendOrder {
  orderId?: string
  id?: string
  encounterId?: string
  catalogId?: string
  itemCode?: string
  code?: string
  itemName?: string
  name?: string
  spec?: string
  specification?: string
  orderType?: string
  category?: string
  categoryCode?: string
  categoryName?: string
  dose?: number
  doseUnit?: string
  unit?: string
  frequency?: string
  routeCode?: string
  route?: string
  useDays?: number
  days?: number
  quantity?: number
  quantityUnit?: string
  contentPerUnit?: number | string
  dosePerQuantityUnit?: number
  isSkinTest?: boolean
  remark?: string
  status?: string
  statusDisplay?: string
  groupId?: string
  prescriptionNo?: string
  serviceRequestId?: string
  orderNo?: number
  version?: number
  updatedAt?: string
  createTime?: string
}

interface BackendCatalogItem extends BackendOrder {
  defaultDose?: number
  defaultUnit?: string
  defaultFrequency?: string
  defaultRoute?: string
  defaultDays?: number
  defaultQuantity?: number
}

interface BackendHistoryEncounter {
  visitId?: string
  encounterId?: string
  visitTime?: string
  visitType?: string
  mainDiagnosis?: string
  doctorName?: string
  drugCount?: number
  examCount?: number
  materialCount?: number
}

interface BackendHistoryOrder {
  orderId?: string
  orderType?: string
  orderName?: string
  quantity?: number
  executedCount?: number
  sampleType?: string
  executeHospital?: string
  createTime?: string
  diagnosis?: string
  doctorName?: string
  isSkinTest?: number
}

interface BackendMutationResult {
  version?: number
  updatedAt?: string
}

function text(value: unknown): string | undefined {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function number(value: unknown, fallback = 0): number {
  return typeof value === 'number' && Number.isFinite(value) ? value : fallback
}

function unwrapData<T>(response: BackendJsonVO<T>, fallback: string): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || fallback)
  }
  return response.data
}

function toCategory(row: BackendOrder): DoctorAdviceOrderVO['category'] {
  const category = text(row.category)?.toUpperCase()
  if (
    category === 'DRUG' ||
    category === 'LAB' ||
    category === 'EXAM' ||
    category === 'TREATMENT'
  ) {
    return category
  }
  const orderType = text(row.orderType)?.toLowerCase()
  if (orderType === 'medication') return 'DRUG'
  if (orderType === 'service' && row.categoryCode === '22') return 'LAB'
  if (orderType === 'service' && row.categoryCode === '23') return 'EXAM'
  return 'TREATMENT'
}

function categoryName(category: DoctorAdviceOrderVO['category'], value?: string) {
  return text(value) || { DRUG: '药品', LAB: '检验', EXAM: '检查', TREATMENT: '治疗' }[category]
}

function toStatus(value?: string): DoctorAdviceOrderVO['status'] {
  const status = text(value)?.toUpperCase()
  if (status === 'ACTIVE' || status === 'SIGNED') return 'SIGNED'
  if (status === 'CANCELLED' || status === 'WITHDRAWN') return 'WITHDRAWN'
  return 'DRAFT'
}

function mapOrder(row: BackendOrder, fallbackEncounterId = ''): DoctorAdviceOrderVO {
  const category = toCategory(row)
  return {
    id: text(row.orderId) || text(row.id) || '',
    encounterId: text(row.encounterId) || fallbackEncounterId,
    catalogId: text(row.catalogId) || '',
    code: text(row.itemCode) || text(row.code) || '',
    name: text(row.itemName) || text(row.name) || '',
    category,
    categoryName: categoryName(category, row.categoryName),
    specification: text(row.spec) || text(row.specification),
    dose: row.dose,
    unit: text(row.doseUnit) || text(row.unit),
    frequency: text(row.frequency),
    route: text(row.routeCode) || text(row.route),
    days: row.useDays ?? row.days,
    quantity: number(row.quantity),
    quantityUnit: text(row.quantityUnit) || '项',
    dosePerQuantityUnit:
      typeof row.dosePerQuantityUnit === 'number'
        ? row.dosePerQuantityUnit
        : typeof row.contentPerUnit === 'number'
          ? row.contentPerUnit
          : undefined,
    remark: text(row.remark),
    status: toStatus(row.status),
    groupId: text(row.groupId),
    prescriptionNo: text(row.prescriptionNo),
    serviceRequestId: text(row.serviceRequestId),
    orderNo: number(row.orderNo),
    version: row.version
  }
}

function mapCatalog(row: BackendCatalogItem): DoctorAdviceOrderCatalogItemVO {
  const category = toCategory(row)
  return {
    id: text(row.id) || text(row.catalogId) || '',
    code: text(row.code) || text(row.itemCode) || '',
    name: text(row.name) || text(row.itemName) || '',
    category,
    categoryName: categoryName(category, row.categoryName),
    specification: text(row.specification) || text(row.spec),
    defaultDose: row.defaultDose ?? row.dose,
    defaultUnit: text(row.defaultUnit) || text(row.doseUnit),
    defaultFrequency: text(row.defaultFrequency) || text(row.frequency),
    defaultRoute: text(row.defaultRoute) || text(row.routeCode),
    defaultDays: row.defaultDays ?? row.useDays,
    defaultQuantity: row.defaultQuantity ?? number(row.quantity, 1),
    quantityUnit: text(row.quantityUnit) || '项',
    dosePerQuantityUnit:
      row.dosePerQuantityUnit ??
      (typeof row.contentPerUnit === 'number' ? row.contentPerUnit : undefined)
  }
}

function toOrderType(item: SaveDoctorAdviceOrderItemDTO) {
  return item.category === 'DRUG' ? 'medication' : 'service'
}

function toBackendItem(item: SaveDoctorAdviceOrderItemDTO, orderNo = item.orderNo) {
  return {
    catalogId: item.catalogId,
    orderType: toOrderType(item),
    dose: item.dose,
    doseUnit: item.unit,
    frequency: item.frequency,
    routeCode: item.route,
    useDays: item.days,
    quantity: item.quantity,
    quantityUnit: item.quantityUnit,
    remark: item.remark,
    orderNo
  }
}

function emptyMutationResult(
  encounterId: string,
  data?: BackendMutationResult
): DoctorAdviceOrderListVO {
  return {
    encounterId,
    rows: [],
    version: data?.version,
    updatedAt: data?.updatedAt
  }
}

function pageResult<T>(data: BackendPage<unknown>, rows: T[]): PageDTO<T> {
  return {
    pageIndex: data.pageIndex ?? 1,
    pageSize: data.pageSize ?? rows.length,
    total: data.total ?? rows.length,
    pages:
      data.pages ??
      Math.ceil((data.total ?? rows.length) / Math.max(data.pageSize ?? rows.length, 1)),
    rows
  }
}

export async function realPageAdviceOrders(
  query: DoctorAdviceOrderQuery
): Promise<DoctorAdviceOrderPageVO> {
  const response = await useHttp().get<BackendPage<BackendOrder>>(
    '/createOrder/query',
    {
      encounterId: query.encounterId,
      keyword: query.keyword,
      orderType: query.category === 'DRUG' ? 'medication' : query.category ? 'service' : undefined,
      statusList: query.status
        ? [
            query.status === 'SIGNED'
              ? 'ACTIVE'
              : query.status === 'WITHDRAWN'
                ? 'CANCELLED'
                : 'DRAFT'
          ]
        : undefined,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '门诊医嘱加载失败')
  const rows = (data.rows ?? data.data ?? []).map((row) => mapOrder(row, query.encounterId))
  return {
    ...pageResult(data, rows),
    encounterId: data.encounterId || query.encounterId,
    version: data.version,
    updatedAt: data.updatedAt
  }
}

export async function realPageAdviceOrderCatalog(
  query: DoctorAdviceOrderCatalogQuery
): Promise<PageDTO<DoctorAdviceOrderCatalogItemVO>> {
  const response = await useHttp().get<BackendPage<BackendCatalogItem>>(
    '/doctor-station/page-advice-order-catalog',
    { ...query },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '医嘱项目目录加载失败')
  return pageResult(data, (data.rows ?? data.data ?? []).map(mapCatalog))
}

export async function realCreateAdviceOrder(data: CreateDoctorAdviceOrderDTO) {
  const payload = { ...toBackendItem(data.item), encounterId: data.encounterId }
  const response = await useHttp().post<BackendOrder>('/createOrder/add', payload, {
    baseURL: realApiBaseURL
  })
  const result = unwrapData(response, '门诊医嘱新增失败')
  return {
    ...emptyMutationResult(data.encounterId),
    rows: [mapOrder(result, data.encounterId)]
  }
}

export async function realUpdateAdviceOrder(data: UpdateDoctorAdviceOrderDTO) {
  const payload = {
    orderId: data.adviceId,
    encounterId: data.encounterId,
    version: data.version,
    ...toBackendItem(data.item)
  }
  const response = await useHttp().put<BackendOrder>('/createOrder/modify', payload, {
    baseURL: realApiBaseURL
  })
  const result = unwrapData(response, '门诊医嘱修改失败')
  return {
    ...emptyMutationResult(data.encounterId),
    rows: [mapOrder(result, data.encounterId)]
  }
}

async function realOperation(
  method: 'delete' | 'put',
  url: string,
  data: { encounterId: string; version?: number; adviceIds: string[] }
) {
  const response =
    method === 'delete'
      ? await useHttp().delete<BackendMutationResult>(url, data, { baseURL: realApiBaseURL })
      : await useHttp().put<BackendMutationResult>(url, data, { baseURL: realApiBaseURL })
  const result = unwrapData(response, '门诊医嘱操作失败')
  return emptyMutationResult(data.encounterId, result)
}

export function realDeleteAdviceOrders(data: DeleteDoctorAdviceOrdersDTO) {
  return realOperation('delete', '/prescription3/batch', data)
}

export function realGroupAdviceOrders(data: GroupDoctorAdviceOrdersDTO) {
  return realOperation('put', '/prescription3/group/merge', data)
}

export function realUngroupAdviceOrders(data: UngroupDoctorAdviceOrdersDTO) {
  return realOperation('put', '/prescription3/group/split', data)
}

export function realSignAdviceOrders(data: SignDoctorAdviceOrdersDTO) {
  return realOperation('put', '/medical-advice/sign', data)
}

export function realWithdrawAdviceOrders(data: WithdrawDoctorAdviceOrdersDTO) {
  return realOperation('put', '/medical-advice/withdraw', data)
}

export async function realPagePatientAdviceOrderHistory(
  query: DoctorAdviceOrderHistoryQuery
): Promise<PageDTO<DoctorAdviceOrderHistoryEncounterVO>> {
  const response = await useHttp().get<BackendPage<BackendHistoryEncounter>>(
    '/historyOrder/list',
    {
      patientId: query.patientId,
      keyword: query.keyword,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '历史医嘱就诊加载失败')
  const rows = (data.rows ?? data.data ?? []).map((row) => ({
    encounterId: text(row.visitId) || text(row.encounterId) || '',
    visitNo: text(row.visitId) || '',
    departmentName: text(row.visitType) || '',
    attendingDoctorName: text(row.doctorName) || '',
    visitedAt: text(row.visitTime) || '',
    signedCount: number(row.drugCount) + number(row.examCount) + number(row.materialCount),
    withdrawnCount: 0,
    visitTime: text(row.visitTime),
    visitType: text(row.visitType),
    mainDiagnosis: text(row.mainDiagnosis),
    doctorName: text(row.doctorName),
    drugCount: number(row.drugCount),
    examCount: number(row.examCount),
    materialCount: number(row.materialCount)
  }))
  return pageResult(data, rows)
}

export async function realPagePatientAdviceOrderHistoryDetails(
  query: DoctorAdviceOrderHistoryDetailQuery
): Promise<PageDTO<DoctorAdviceOrderVO>> {
  const response = await useHttp().get<BackendPage<BackendHistoryOrder>>(
    '/historyOrder/detail',
    {
      visitId: query.encounterId,
      orderType: query.category === 'DRUG' ? 'medication' : query.category ? 'service' : undefined,
      keyword: query.keyword,
      pageIndex: query.pageIndex,
      pageSize: query.pageSize
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '历史医嘱明细加载失败')
  const rows = (data.rows ?? data.data ?? []).map((row) => {
    const category: DoctorAdviceOrderVO['category'] =
      row.orderType?.toLowerCase() === 'medication' ? 'DRUG' : 'TREATMENT'
    return {
      id: text(row.orderId) || '',
      encounterId: query.encounterId,
      catalogId: '',
      code: '',
      name: text(row.orderName) || '',
      category,
      categoryName: category === 'DRUG' ? '药品' : '治疗',
      quantity: number(row.quantity),
      quantityUnit: '项',
      status: 'SIGNED' as const,
      orderNo: 0,
      remark: text(row.diagnosis),
      prescriptionNo: undefined,
      serviceRequestId: undefined,
      executedCount: number(row.executedCount),
      sampleType: text(row.sampleType),
      executeHospital: text(row.executeHospital),
      createTime: text(row.createTime),
      diagnosis: text(row.diagnosis),
      doctorName: text(row.doctorName),
      isSkinTest: row.isSkinTest === 1
    }
  })
  return pageResult(data, rows)
}
