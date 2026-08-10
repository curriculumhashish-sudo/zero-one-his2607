import HttpClient, { DataUpType } from '@/apis/http'
import { mockExportOutpatientFeeDetails, mockPageOutpatientFeeDetails } from './mock'
import type {
  ExportOutpatientFeeDetailParams,
  ExportOutpatientFeeDetailResponse,
  OutpatientFeeDetailAmountSummaryDto,
  OutpatientFeeDetailBackendRowDto,
  OutpatientFeeDetailEncounterBlockDto,
  OutpatientFeeDetailLineDto,
  PageOutpatientFeeDetailBackendResponse,
  PageOutpatientFeeDetailParams,
  PageOutpatientFeeDetailResponse,
  RequestContext
} from './type'

type FeeProvider = 'j8' | 'j6' | 'c1'
type LooseRecord = Record<string, any>

const feeProvider = String(import.meta.env.VITE_OUTPATIENT_FEE_PROVIDER ?? 'j8').toLowerCase() as FeeProvider
const useMock = import.meta.env.MODE === 'test' || import.meta.env.VITE_J8_OUTPATIENT_FEE_MOCK === 'true'

const feeHttp = new HttpClient(
  {
    baseURL:
      feeProvider === 'j6'
        ? import.meta.env.VITE_J6_OUTPATIENT_FEE_SERVER_URL || '/j6-report'
        : feeProvider === 'c1'
          ? import.meta.env.VITE_C1_OUTPATIENT_FEE_SERVER_URL || '/c1-report'
          : import.meta.env.VITE_J8_OUTPATIENT_FEE_SERVER_URL || '/j8-outpatient'
  },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token')
      const tokenHead = typeof localStorage === 'undefined' ? 'Bearer' : (localStorage.getItem('tokenHead') || 'Bearer')
      if (token) config.headers.Authorization = tokenHead + ' ' + token
      if (config.upType === DataUpType.json) config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

type BackendEnvelope<T> = { code?: number; message?: string; data?: T }

async function unwrap<T>(request: Promise<BackendEnvelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '门诊费用明细接口请求失败')
  }
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '门诊费用明细接口未返回数据')
  }
  return response.data
}

function requestConfig(context?: RequestContext) {
  return context?.signal ? { signal: context.signal } : undefined
}

function toBase64(bytes: Uint8Array) {
  let binary = ''
  bytes.forEach((byte) => {
    binary += String.fromCharCode(byte)
  })
  return btoa(binary)
}

async function requestExportFile(
  path: string,
  payload: LooseRecord,
  context?: RequestContext,
  method: 'get' | 'post' = 'post'
): Promise<ExportOutpatientFeeDetailResponse> {
  const response = await feeHttp.getInstance().request<ArrayBuffer>({
    method,
    url: path,
    ...(method === 'get' ? { params: payload } : { data: payload }),
    responseType: 'arraybuffer',
    signal: context?.signal
  })
  const contentType = String(response.headers['content-type'] ?? 'application/octet-stream')
  const disposition = String(response.headers['content-disposition'] ?? '')
  const fileName = disposition.match(/filename\*?=(?:UTF-8''|"?)([^";]+)/i)?.[1]
  return {
    fileName: fileName ? decodeURIComponent(fileName) : '门诊费用明细.xlsx',
    contentType,
    body: toBase64(new Uint8Array(response.data)),
    exportedRowCount: 0,
    truncated: false
  }
}

function amountSummary(response: PageOutpatientFeeDetailBackendResponse): OutpatientFeeDetailAmountSummaryDto {
  if (response.summary) {
    return {
      baseAmountTotal: response.summary.baseAmountTotal,
      discountAmountTotal: response.summary.discountAmountTotal,
      receivableAmountTotal: response.summary.receivableAmountTotal,
      discountedAmountTotal: response.summary.discountedAmountTotal,
      lineAmountTotal: response.summary.lineAmountTotal,
      actualReceivedAmountTotal: response.summary.actualReceivedAmountTotal,
      insuranceAmountTotal: response.summary.insuranceAmountTotal,
      selfPayAmountTotal: response.summary.selfPayAmountTotal
    }
  }
  return {
    baseAmountTotal: response.baseAmountSum,
    discountAmountTotal: response.discountAmountSum,
    receivableAmountTotal: response.receivableAmountSum,
    discountedAmountTotal: response.discountedAmountSum,
    lineAmountTotal: response.lineAmountSum,
    actualReceivedAmountTotal: response.actualReceivedAmountSum,
    insuranceAmountTotal: response.insuranceAmountSum,
    selfPayAmountTotal: response.selfPayAmountSum
  }
}

function sumLines(lines: OutpatientFeeDetailLineDto[]): OutpatientFeeDetailAmountSummaryDto {
  const sum = (key: keyof OutpatientFeeDetailLineDto) => lines.reduce((total, item) => total + Number(item[key] ?? 0), 0)
  return {
    baseAmountTotal: sum('baseAmount'),
    discountAmountTotal: sum('discountAmount'),
    receivableAmountTotal: sum('receivableAmount'),
    discountedAmountTotal: sum('discountedAmount'),
    lineAmountTotal: sum('lineAmount'),
    actualReceivedAmountTotal: sum('actualReceivedAmount'),
    insuranceAmountTotal: sum('insuranceAmount'),
    selfPayAmountTotal: sum('selfPayAmount')
  }
}

function toLine(row: OutpatientFeeDetailBackendRowDto): OutpatientFeeDetailLineDto {
  return {
    chargeItemId: row.chargeItemId,
    paymentId: row.paymentId,
    feeCategoryCode: row.feeCategoryCode,
    feeCategoryDisplay: row.feeCategoryDisplay,
    subCategoryCode: row.subCategoryCode,
    subCategoryDisplay: row.subCategoryDisplay,
    productName: row.productName,
    productCode: row.productCode,
    specification: row.specification,
    quantityValue: row.quantityValue,
    quantityUnitDisplay: row.quantityUnitDisplay,
    orderingDoctorId: row.orderingDoctorId,
    orderingDoctorName: row.orderingDoctorName,
    cashierId: row.cashierId,
    cashierName: row.cashierName,
    billDate: row.billDate,
    tradeDirection: row.tradeDirection,
    insuranceLevelDisplay: row.insuranceLevelDisplay,
    insuranceTypeDisplay: row.insuranceTypeDisplay,
    baseAmount: row.baseAmount,
    discountAmount: row.discountAmount,
    receivableAmount: row.receivableAmount,
    discountedAmount: row.discountedAmount,
    unitPrice: row.unitPrice,
    lineAmount: row.lineAmount,
    actualReceivedAmount: row.actualReceivedAmount,
    insuranceAmount: row.insuranceAmount,
    selfPayAmount: row.selfPayAmount,
    discountRateDisplay: row.discountRateDisplay
  }
}

function groupRows(rows: OutpatientFeeDetailBackendRowDto[]): OutpatientFeeDetailEncounterBlockDto[] {
  const groups = new Map<string, OutpatientFeeDetailEncounterBlockDto>()
  rows.forEach((row, index) => {
    const key = String(row.outpatientNo ?? `row-${index}`)
    let block = groups.get(key)
    if (!block) {
      block = {
        // J8 返回没有 encounterId，使用门诊号作为稳定的前端分组键。
        encounterId: key,
        patientName: row.patientName,
        busNo: row.outpatientNo,
        registrationDepartmentName: row.registrationDepartmentName ?? row.deptName,
        encounterDate: row.encounterDate,
        lines: [],
        subtotal: undefined
      }
      groups.set(key, block)
    }
    block.lines!.push(toLine(row))
  })
  groups.forEach((block) => {
    block.subtotal = sumLines(block.lines ?? [])
  })
  return [...groups.values()]
}

function normalizePage(response: PageOutpatientFeeDetailBackendResponse): PageOutpatientFeeDetailResponse {
  const rawContent = response.content ?? []
  const firstContent = rawContent[0]
  const contentRows = firstContent && 'chargeItemId' in firstContent
    ? rawContent as OutpatientFeeDetailBackendRowDto[]
    : undefined
  const content = response.rows
    ? groupRows(response.rows)
    : contentRows
      ? groupRows(contentRows)
      : rawContent as OutpatientFeeDetailEncounterBlockDto[]
  const lineItems = content.flatMap((block) => block.lines ?? [])
  return {
    content,
    totalCount: Number(response.totalRowCount ?? response.totalCount ?? 0),
    pageIndex: Number(response.pageNo ?? response.pageIndex ?? 1),
    pageSize: Number(response.pageSize ?? 0),
    summary: response.summary || response.baseAmountSum !== undefined || response.discountAmountSum !== undefined
      ? amountSummary(response)
      : sumLines(lineItems)
  }
}

function toJ6Line(line: LooseRecord): OutpatientFeeDetailLineDto {
  const quantity = Number(line.quantityValue ?? 0)
  const unitPrice = Number(line.unitAmount ?? line.unitPrice ?? 0)
  const lineAmount = line.lineAmount ?? (quantity ? unitPrice * quantity : line.actualReceivedAmount)
  return {
    chargeItemId: line.chargeItemId,
    paymentId: line.paymentId,
    feeCategoryCode: line.feeCategoryCode,
    feeCategoryDisplay: line.feeCategoryDisplay,
    subCategoryCode: line.subCategoryCode,
    subCategoryDisplay: line.subCategoryDisplay,
    productName: line.productName,
    productCode: line.productCode,
    specification: line.specification,
    quantityValue: line.quantityValue,
    quantityUnitDisplay: line.quantityUnitDisplay,
    orderingDoctorId: line.orderingDoctorId,
    orderingDoctorName: line.orderingDoctorName,
    cashierId: line.cashierId,
    cashierName: line.cashierName,
    billDate: line.billDate,
    tradeDirection: line.tradeDirection,
    insuranceLevelDisplay: line.insuranceLevelDisplay,
    insuranceTypeDisplay: line.insuranceTypeDisplay,
    baseAmount: line.baseAmount,
    discountAmount: line.discountAmount,
    receivableAmount: line.receivableAmount,
    discountedAmount: line.discountedAmount,
    unitPrice,
    lineAmount,
    actualReceivedAmount: line.actualReceivedAmount,
    insuranceAmount: line.insuranceAmount,
    selfPayAmount: line.selfPayAmount
  }
}

function normalizeJ6Page(response: LooseRecord): PageOutpatientFeeDetailResponse {
  const rows = Array.isArray(response.rows) ? response.rows : []
  const content = rows.map((row: LooseRecord) => ({
    encounterId: row.encounterId,
    patientName: row.patientName,
    busNo: row.busNo,
    registrationDepartmentName: row.registrationDepartmentName,
    encounterDate: row.encounterDate,
    lines: Array.isArray(row.lines) ? row.lines.map(toJ6Line) : [],
    subtotal: row.subTotal ? amountSummary({ summary: row.subTotal }) : undefined
  }))
  const calculatedSummary = sumLines(content.flatMap((block) => block.lines ?? []))
  const backendSummary = response.summary ? amountSummary({ summary: response.summary }) : undefined
  return {
    content,
    totalCount: Number(response.totalCount ?? response.total ?? 0),
    pageIndex: Number(response.pageIndex ?? response.pageNo ?? 1),
    pageSize: Number(response.pageSize ?? 0),
    summary: backendSummary
      ? {
          ...backendSummary,
          lineAmountTotal: backendSummary.lineAmountTotal ?? calculatedSummary.lineAmountTotal
        }
      : calculatedSummary
  }
}

function toC1Row(row: LooseRecord): OutpatientFeeDetailBackendRowDto {
  return {
    chargeItemId: row.id,
    paymentId: row.paymentId,
    feeCategoryCode: row.feeClass,
    feeCategoryDisplay: row.feeClass,
    subCategoryCode: row.detailClass,
    subCategoryDisplay: row.detailClass,
    productName: row.projectName,
    productCode: row.hisID,
    specification: row.spec,
    quantityValue: row.number,
    quantityUnitDisplay: row.unit,
    orderingDoctorName: row.doctor,
    cashierName: row.checker,
    billDate: row.settleTime,
    tradeDirection: row.source,
    insuranceLevelDisplay: row.insuranceLevel,
    insuranceTypeDisplay: row.insuranceType,
    baseAmount: row.originalPrice,
    discountAmount: row.discountAmount,
    receivableAmount: row.receivableAmount,
    discountedAmount: row.discountedPrice,
    unitPrice: row.unitPrice,
    lineAmount: row.lineAmount,
    actualReceivedAmount: row.actualAmount,
    insuranceAmount: row.insuranceAmount,
    selfPayAmount: row.selfPayAmount,
    outpatientNo: row.clinicNo,
    patientName: row.name,
    deptName: row.room,
    encounterDate: row.date
  }
}

function normalizeC1Page(response: LooseRecord): PageOutpatientFeeDetailResponse {
  const rows = Array.isArray(response.rows) ? response.rows.map(toC1Row) : []
  return normalizePage({ rows, totalRowCount: response.total, pageIndex: response.pageIndex, pageSize: response.pageSize })
}

function toBackendQuery(query: PageOutpatientFeeDetailParams) {
  return {
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    ...(query.registrationDepartmentId ? { registrationDepartmentId: query.registrationDepartmentId } : {}),
    ...(query.feeCategoryCodes?.length ? { feeCategoryCodes: query.feeCategoryCodes.join(',') } : {}),
    ...(query.subCategoryCodes?.length ? { subCategoryCodes: query.subCategoryCodes.join(',') } : {}),
    ...(query.patientKeyword ? { patientKeyword: query.patientKeyword } : {}),
    ...(query.productNameKeyword ? { productNameKeyword: query.productNameKeyword } : {}),
    ...(query.productCodeKeyword ? { productCodeKeyword: query.productCodeKeyword } : {}),
    ...(query.cashierId ? { cashierId: query.cashierId } : {}),
    ...(query.tradeDirection ? { tradeDirection: query.tradeDirection } : {}),
    pageIndex: query.pageIndex,
    pageSize: query.pageSize
  }
}

function toJ6Query(query: PageOutpatientFeeDetailParams) {
  return {
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    tradeDirection: query.tradeDirection,
    registrationDepartmentId: query.registrationDepartmentId,
    subCategoryCodes: query.subCategoryCodes,
    patientKeyword: query.patientKeyword,
    productNameKeyword: query.productNameKeyword,
    productCodeKeyword: query.productCodeKeyword,
    cashierId: query.cashierId,
  }
}

function toC1Query(query: PageOutpatientFeeDetailParams) {
  return {
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    start_time: query.dateFrom,
    end_time: query.dateTo,
    room: query.registrationDepartmentId,
    feeClass: query.feeCategoryCodes?.join(','),
    detailClass: query.subCategoryCodes?.join(','),
    projectName: query.productNameKeyword,
    hisID: query.productCodeKeyword,
    name: query.patientKeyword,
    checker: query.cashierId,
    feeDir: query.tradeDirection
  }
}

function toBackendExportQuery(query: ExportOutpatientFeeDetailParams) {
  return {
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    ...(query.registrationDepartmentId ? { registrationDepartmentId: query.registrationDepartmentId } : {}),
    ...(query.feeCategoryCodes?.length ? { feeCategoryCodes: query.feeCategoryCodes } : {}),
    ...(query.subCategoryCodes?.length ? { subCategoryCodes: query.subCategoryCodes } : {}),
    ...(query.patientKeyword ? { patientKeyword: query.patientKeyword } : {}),
    ...(query.productNameKeyword ? { productNameKeyword: query.productNameKeyword } : {}),
    ...(query.productCodeKeyword ? { productCodeKeyword: query.productCodeKeyword } : {}),
    ...(query.cashierId ? { cashierId: query.cashierId } : {}),
    ...(query.tradeDirection ? { tradeDirection: query.tradeDirection } : {})
  }
}

function toJ6ExportQuery(query: ExportOutpatientFeeDetailParams) {
  return {
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    tradeDirection: query.tradeDirection,
    registrationDepartmentId: query.registrationDepartmentId,
    subCategoryCodes: query.subCategoryCodes,
    patientKeyword: query.patientKeyword,
    productNameKeyword: query.productNameKeyword,
    productCodeKeyword: query.productCodeKeyword,
    cashierId: query.cashierId,
  }
}

function toC1ExportQuery(query: ExportOutpatientFeeDetailParams) {
  return {
    start_time: query.dateFrom,
    end_time: query.dateTo,
    room: query.registrationDepartmentId,
    feeClass: query.feeCategoryCodes?.join(','),
    detailClass: query.subCategoryCodes?.join(','),
    projectName: query.productNameKeyword,
    hisID: query.productCodeKeyword,
    name: query.patientKeyword,
    checker: query.cashierId,
    feeDir: query.tradeDirection
  }
}

export const pageOutpatientFeeDetails = async (
  query: PageOutpatientFeeDetailParams,
  context?: RequestContext
): Promise<PageOutpatientFeeDetailResponse> => {
  if (useMock) {
    const response = await mockPageOutpatientFeeDetails(query, context)
    return normalizePage(response)
  }
  if (feeProvider === 'j6') {
    const response = await unwrap<LooseRecord>(feeHttp.post('/outpatient/fee-detail', toJ6Query(query), requestConfig(context)))
    return normalizeJ6Page(response)
  }
  if (feeProvider === 'c1') {
    const response = await unwrap<LooseRecord>(feeHttp.get('/form/cost_info/query', toC1Query(query), requestConfig(context)))
    return normalizeC1Page(response)
  }
  const response = await unwrap<PageOutpatientFeeDetailBackendResponse>(feeHttp.get('/j8/mzfymx/getMinXiList', toBackendQuery(query), requestConfig(context)))
  return normalizePage(response)
}

export const exportOutpatientFeeDetails = async (
  query: ExportOutpatientFeeDetailParams,
  context?: RequestContext
): Promise<ExportOutpatientFeeDetailResponse> => {
  if (useMock) return mockExportOutpatientFeeDetails(query, context)
  if (feeProvider === 'j6') {
    return requestExportFile('/outpatient/fee-detail-export', toJ6ExportQuery(query), context)
  }
  if (feeProvider === 'c1') {
    // C1 返回真实 XLSX 二进制流，不能按 JSON 信封解析。
    return requestExportFile('/form/cost_info/export', toC1ExportQuery(query), context, 'get')
  }
  return unwrap<ExportOutpatientFeeDetailResponse>(feeHttp.post('/j8/mzfymx/exportMinXi', toBackendExportQuery(query), requestConfig(context)))
}

export type * from './type'
