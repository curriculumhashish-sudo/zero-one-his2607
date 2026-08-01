import * as XLSX from 'xlsx'
import dayjs from 'dayjs'
import type {
  ExportOutpatientFeeDetailParams,
  ExportOutpatientFeeDetailResponse,
  OutpatientFeeDetailAmountSummaryDto,
  OutpatientFeeDetailEncounterBlockDto,
  OutpatientFeeDetailLineDto,
  PageOutpatientFeeDetailBackendResponse,
  PageOutpatientFeeDetailParams,
  RequestContext
} from './type'

const departments = [
  { value: 'dept-internal', label: '心血管内科' },
  { value: 'dept-surgery', label: '普通外科' },
  { value: 'dept-pediatrics', label: '儿科' }
]
const cashiers = [
  { value: 'cashier-1', label: '李芳' },
  { value: 'cashier-2', label: '王强' }
]
const feeCategories = [
  { value: 'MEDICATION', label: '药品' },
  { value: 'ACTIVITY', label: '诊疗' },
  { value: 'DEVICE', label: '耗材' }
]
const subCategories = [
  { value: 'WESTERN_MEDICINE', label: '西药' },
  { value: 'EXAMINATION', label: '检查' },
  { value: 'CONSUMABLE', label: '医用耗材' }
]

const doctors = ['王建国', '刘敏', '赵海燕']
const patientNames = ['陈晨', '周明', '孙丽', '赵刚', '林晓']

function delay(context: RequestContext = {}) {
  return new Promise<void>((resolve, reject) => {
    const timer = setTimeout(resolve, 70)
    if (context.signal) {
      const abort = () => {
        clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      }
      if (context.signal.aborted) abort()
      else context.signal.addEventListener('abort', abort, { once: true })
    }
  })
}

function lineFor(index: number, encounterIndex: number): OutpatientFeeDetailLineDto {
  const feeCategory = feeCategories[index % feeCategories.length]!
  const subCategory = subCategories[index % subCategories.length]!
  const baseAmount = 35 + index * 7 + encounterIndex * 3
  const discountAmount = index % 3 === 0 ? 5 : 0
  const receivableAmount = baseAmount - discountAmount
  const insuranceAmount = index % 2 === 0 ? Math.round(receivableAmount * 0.6 * 100) / 100 : 0
  return {
    chargeItemId: `charge-${encounterIndex + 1}-${index + 1}`,
    paymentId: `payment-${encounterIndex + 1}`,
    feeCategoryCode: feeCategory.value,
    feeCategoryDisplay: feeCategory.label,
    subCategoryCode: subCategory.value,
    subCategoryDisplay: subCategory.label,
    productName: index % 3 === 0 ? '门诊常规检查' : index % 3 === 1 ? '口服用药' : '一次性输液器',
    productCode: `ITEM-${String(index + 1).padStart(3, '0')}`,
    specification: index % 2 === 0 ? '10mg*10片' : '标准规格',
    quantityValue: index % 2 === 0 ? 1 : 2,
    quantityUnitDisplay: index % 2 === 0 ? '项' : '盒',
    orderingDoctorId: `doctor-${(index % doctors.length) + 1}`,
    orderingDoctorName: doctors[index % doctors.length],
    cashierId: cashiers[encounterIndex % cashiers.length]!.value,
    cashierName: cashiers[encounterIndex % cashiers.length]!.label,
    billDate: `${dayjs().subtract(encounterIndex % 10, 'day').format('YYYY-MM-DD')} 09:${String(index + 10).padStart(2, '0')}:00`,
    tradeDirection: index % 4 === 0 ? 'REFUND' : 'CHARGE',
    insuranceLevelDisplay: index % 2 === 0 ? '甲类' : '乙类',
    insuranceTypeDisplay: index % 2 === 0 ? '医保' : '自费',
    baseAmount,
    discountAmount,
    receivableAmount,
    discountedAmount: receivableAmount,
    unitPrice: index % 2 === 0 ? baseAmount : baseAmount / 2,
    lineAmount: index % 4 === 0 ? -receivableAmount : receivableAmount,
    actualReceivedAmount: index % 4 === 0 ? -receivableAmount : receivableAmount,
    insuranceAmount: index % 4 === 0 ? -insuranceAmount : insuranceAmount,
    selfPayAmount: index % 4 === 0 ? -(receivableAmount - insuranceAmount) : receivableAmount - insuranceAmount,
    discountRateDisplay: discountAmount ? '85%' : '100%'
  }
}

function sumLines(lines: OutpatientFeeDetailLineDto[]): OutpatientFeeDetailAmountSummaryDto {
  const sum = (key: keyof OutpatientFeeDetailLineDto) =>
    lines.reduce((total, item) => total + Number(item[key] ?? 0), 0)
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

const blocks: OutpatientFeeDetailEncounterBlockDto[] = Array.from({ length: 18 }, (_, index) => {
  const lines = [lineFor(index % 3, index), lineFor((index + 1) % 3, index)]
  return {
    encounterId: `encounter-${index + 1}`,
    patientName: patientNames[index % patientNames.length],
    busNo: `OP-${dayjs().format('YYYYMMDD')}${String(index + 1).padStart(2, '0')}`,
    registrationDepartmentName: departments[index % departments.length]!.label,
    encounterDate: dayjs().subtract(index % 10, 'day').format('YYYY-MM-DD'),
    lines,
    subtotal: sumLines(lines)
  }
})

function matches(block: OutpatientFeeDetailEncounterBlockDto, query: Omit<PageOutpatientFeeDetailParams, 'PageIndex' | 'PageSize'>) {
  const keyword = query.patientKeyword?.trim().toLowerCase()
  const productName = query.productNameKeyword?.trim().toLowerCase()
  const productCode = query.productCodeKeyword?.trim().toLowerCase()
  const department = !query.registrationDepartmentId || departments.find((item) => item.value === query.registrationDepartmentId)?.label === block.registrationDepartmentName
  const patient = !keyword || `${block.patientName}${block.busNo}`.toLowerCase().includes(keyword)
  const lines = block.lines ?? []
  const lineMatch = lines.some((line) => {
    const category = !query.feeCategoryCodes?.length || query.feeCategoryCodes.includes(line.feeCategoryCode ?? '')
    const subCategory = !query.subCategoryCodes?.length || query.subCategoryCodes.includes(line.subCategoryCode ?? '')
    const name = !productName || line.productName?.toLowerCase().includes(productName)
    const code = !productCode || line.productCode?.toLowerCase().includes(productCode)
    const cashier = !query.cashierId || line.cashierId === query.cashierId
    const direction = !query.tradeDirection || query.tradeDirection === 'ALL' || line.tradeDirection === query.tradeDirection
    return category && subCategory && name && code && cashier && direction
  })
  return department && patient && lineMatch && block.encounterDate! >= query.dateFrom && block.encounterDate! <= query.dateTo
}

function flattenLines(source: OutpatientFeeDetailEncounterBlockDto[]) {
  return source.flatMap((block) => block.lines ?? [])
}

export async function mockPageOutpatientFeeDetails(
  query: PageOutpatientFeeDetailParams,
  context: RequestContext = {}
): Promise<PageOutpatientFeeDetailBackendResponse> {
  await delay(context)
  const filters = { ...query }
  const filtered = blocks.filter((block) => matches(block, filters))
  const start = (query.PageIndex - 1) * query.PageSize
  const content = structuredClone(filtered.slice(start, start + query.PageSize))
  const summary = sumLines(flattenLines(filtered))
  return {
    content,
    totalCount: filtered.length,
    pageNo: query.PageIndex,
    pageSize: query.PageSize,
    baseAmountSum: summary.baseAmountTotal,
    discountAmountSum: summary.discountAmountTotal,
    receivableAmountSum: summary.receivableAmountTotal,
    discountedAmountSum: summary.discountedAmountTotal,
    lineAmountSum: summary.lineAmountTotal,
    actualReceivedAmountSum: summary.actualReceivedAmountTotal,
    insuranceAmountSum: summary.insuranceAmountTotal,
    selfPayAmountSum: summary.selfPayAmountTotal
  }
}

function toBase64(value: string) {
  const bytes = new TextEncoder().encode(value)
  let binary = ''
  bytes.forEach((byte) => (binary += String.fromCharCode(byte)))
  return btoa(binary)
}

export async function mockExportOutpatientFeeDetails(
  query: ExportOutpatientFeeDetailParams,
  context: RequestContext = {}
): Promise<ExportOutpatientFeeDetailResponse> {
  await delay(context)
  const filtered = blocks.filter((block) => matches(block, query))
  const rows = filtered.flatMap((block) =>
    (block.lines ?? []).map((line) => ({
      门诊号: block.busNo,
      患者: block.patientName,
      挂号科室: block.registrationDepartmentName,
      就诊日期: block.encounterDate,
      项目名称: line.productName,
      院内编码: line.productCode,
      数量: line.quantityValue,
      单位: line.quantityUnitDisplay,
      收费人: line.cashierName,
      结算时间: line.billDate,
      来源: line.tradeDirection,
      原价: line.baseAmount,
      应收金额: line.receivableAmount,
      行金额: line.lineAmount,
      医保金额: line.insuranceAmount,
      自费金额: line.selfPayAmount
    }))
  )
  const sheet = XLSX.utils.json_to_sheet(rows)
  const workbook = XLSX.utils.book_new()
  XLSX.utils.book_append_sheet(workbook, sheet, '门诊费用明细')
  const body = XLSX.write(workbook, { bookType: 'xlsx', type: 'base64' })
  return {
    fileName: '门诊费用明细.xlsx',
    contentType: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
    body: body || toBase64('暂无数据'),
    rowCount: rows.length,
    exportedRowCount: rows.length,
    truncated: false
  }
}

export const feeDetailOptions = {
  departments,
  cashiers,
  feeCategories,
  subCategories
}
