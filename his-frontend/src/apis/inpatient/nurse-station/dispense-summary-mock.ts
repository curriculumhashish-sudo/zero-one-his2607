import dayjs from 'dayjs'

import { createPageDTO } from '../../type'
import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationCancelDispenseSummariesDTO,
  NurseStationCreateDispenseSummaryDTO,
  NurseStationCreateDispenseSummaryResultVO,
  NurseStationDispenseMedicineCategory,
  NurseStationDispenseOrderRowVO,
  NurseStationDispensePatientGroupVO,
  NurseStationDispensePendingQueryDTO,
  NurseStationDispensePendingResultVO,
  NurseStationDispenseSummaryDetailVO,
  NurseStationDispenseSummaryHeaderVO,
  NurseStationDispenseSummaryLineVO,
  NurseStationDispenseSummaryQueryDTO,
  NurseStationDispenseSummaryStatus,
  NurseStationDispenseSummaryWriteResultVO,
  NurseStationDispenseTimePointVO,
  NurseStationSubmitDispenseSummariesDTO
} from './dispense-summary-type'

/**
 * Mock 内部把“患者、医嘱和发药时点”放在一条扁平记录中。
 *
 * 这种结构便于按照每个时点分页、占用和释放；查询返回前再组装成“患者 → 医嘱 → 时点”，
 * 因而能模拟真实后端既要处理并发状态、又要给页面返回分组数据的过程。
 */
interface MockDispenseRecord {
  encounterId: string
  patientId: string
  patientName: string
  bedDisplay: string
  medicineCategory: NurseStationDispenseMedicineCategory
  medicationRequestId: string
  medicationId: string
  medicationName: string
  specificationText: string
  therapyType: 'LONG' | 'TEMP'
  therapyTypeText: string
  dosageInstruction: string
  methodText: string
  frequencyText: string
  pharmacyId: string
  pharmacyName: string
  packageUnitText?: string
  packageRatio?: number
  point: NurseStationDispenseTimePointVO
  /** 有值表示该时点已被某张汇总单占用，不能再次汇总。 */
  reservedSummaryNo?: string
}

const today = dayjs().format('YYYY-MM-DD')

const dispenseRecords: MockDispenseRecord[] = [
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    patientName: '孙晓梅',
    bedDisplay: '301病房 01床',
    medicineCategory: 'WEST',
    medicationRequestId: 'med-req-001',
    medicationId: 'drug-ambroxol',
    medicationName: '盐酸氨溴索注射液',
    specificationText: '2ml:15mg/支',
    therapyType: 'LONG',
    therapyTypeText: '长期',
    dosageInstruction: '15mg',
    methodText: '静脉滴注',
    frequencyText: '每日两次',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    packageUnitText: '盒',
    packageRatio: 5,
    point: {
      dispenseId: 'dispense-001',
      procedureId: 'procedure-001',
      plannedDispenseTime: `${today} 08:00:00`,
      quantity: 2,
      unitCode: 'PCS',
      unitCodeText: '支',
      chargeUnitPrice: 6.2
    }
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    patientName: '孙晓梅',
    bedDisplay: '301病房 01床',
    medicineCategory: 'WEST',
    medicationRequestId: 'med-req-001',
    medicationId: 'drug-ambroxol',
    medicationName: '盐酸氨溴索注射液',
    specificationText: '2ml:15mg/支',
    therapyType: 'LONG',
    therapyTypeText: '长期',
    dosageInstruction: '15mg',
    methodText: '静脉滴注',
    frequencyText: '每日两次',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    packageUnitText: '盒',
    packageRatio: 5,
    point: {
      dispenseId: 'dispense-002',
      procedureId: 'procedure-002',
      plannedDispenseTime: `${today} 16:00:00`,
      quantity: 2,
      unitCode: 'PCS',
      unitCodeText: '支',
      chargeUnitPrice: 6.2
    }
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    patientName: '孙晓梅',
    bedDisplay: '301病房 01床',
    medicineCategory: 'WEST',
    medicationRequestId: 'med-req-002',
    medicationId: 'drug-ceftriaxone',
    medicationName: '注射用头孢曲松钠',
    specificationText: '2g/瓶',
    therapyType: 'TEMP',
    therapyTypeText: '临时',
    dosageInstruction: '2g',
    methodText: '静脉滴注',
    frequencyText: '立即',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    packageUnitText: '盒',
    packageRatio: 10,
    point: {
      dispenseId: 'dispense-003',
      procedureId: 'procedure-003',
      plannedDispenseTime: `${today} 10:00:00`,
      quantity: 1,
      unitCode: 'VIAL',
      unitCodeText: '瓶',
      chargeUnitPrice: 18.5
    }
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    patientName: '孙晓梅',
    bedDisplay: '301病房 01床',
    medicineCategory: 'TCM',
    medicationRequestId: 'med-req-003',
    medicationId: 'drug-tcm-lung',
    medicationName: '清肺化痰方',
    specificationText: '院内协定方',
    therapyType: 'TEMP',
    therapyTypeText: '临时',
    dosageInstruction: '1剂',
    methodText: '水煎服',
    frequencyText: '每日一剂',
    pharmacyId: 'pharmacy-inpatient-tcm',
    pharmacyName: '住院中药房',
    point: {
      dispenseId: 'dispense-004',
      procedureId: 'procedure-004',
      plannedDispenseTime: `${today} 11:00:00`,
      quantity: 3,
      unitCode: 'DOSE',
      unitCodeText: '剂',
      chargeUnitPrice: 32
    }
  },
  {
    encounterId: 'encounter-002',
    patientId: 'patient-002',
    patientName: '李建国',
    bedDisplay: '301病房 02床',
    medicineCategory: 'WEST',
    medicationRequestId: 'med-req-101',
    medicationId: 'drug-telmisartan',
    medicationName: '替米沙坦片',
    specificationText: '40mg*14片/盒',
    therapyType: 'LONG',
    therapyTypeText: '长期',
    dosageInstruction: '40mg',
    methodText: '口服',
    frequencyText: '每日一次',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    packageUnitText: '盒',
    packageRatio: 14,
    point: {
      dispenseId: 'dispense-101',
      procedureId: 'procedure-101',
      plannedDispenseTime: `${today} 08:00:00`,
      quantity: 1,
      unitCode: 'TABLET',
      unitCodeText: '片',
      chargeUnitPrice: 2.5
    }
  },
  {
    encounterId: 'encounter-002',
    patientId: 'patient-002',
    patientName: '李建国',
    bedDisplay: '301病房 02床',
    medicineCategory: 'WEST',
    medicationRequestId: 'med-req-102',
    medicationId: 'drug-budesonide',
    medicationName: '布地奈德混悬液',
    specificationText: '2ml:1mg/支',
    therapyType: 'TEMP',
    therapyTypeText: '临时',
    dosageInstruction: '1mg',
    methodText: '雾化吸入',
    frequencyText: '立即',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    packageUnitText: '盒',
    packageRatio: 5,
    point: {
      dispenseId: 'dispense-102',
      procedureId: 'procedure-102',
      plannedDispenseTime: `${today} 09:30:00`,
      quantity: 1,
      unitCode: 'PCS',
      unitCodeText: '支',
      chargeUnitPrice: 13.8
    }
  }
]

const summaryHeaders: NurseStationDispenseSummaryHeaderVO[] = [
  {
    summaryNo: 'MS-HISTORY-0001',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    applicantId: 'nurse-001',
    applicantName: '李护士',
    applyTime: `${today} 07:30:00`,
    status: 'PENDING_SUBMIT',
    statusText: '待发送',
    itemCount: 1,
    version: 1
  },
  {
    summaryNo: 'MS-HISTORY-0002',
    pharmacyId: 'pharmacy-inpatient-west',
    pharmacyName: '住院西药房',
    applicantId: 'nurse-002',
    applicantName: '周护士',
    applyTime: `${today} 06:50:00`,
    receiverId: 'nurse-002',
    receiverName: '周护士',
    dispenseTime: `${today} 07:20:00`,
    status: 'DISPENSED',
    statusText: '已发药',
    itemCount: 1,
    version: 3
  }
]

const summaryLines = new Map<string, NurseStationDispenseSummaryLineVO[]>([
  [
    'MS-HISTORY-0001',
    [
      {
        lineId: 'summary-line-history-001',
        medicationId: 'drug-vitamin-c',
        medicationName: '维生素C片',
        specificationText: '0.1g*100片/瓶',
        itemQuantity: 6,
        unitCode: 'TABLET',
        unitCodeText: '片',
        displayQuantityText: '6片',
        unitPrice: 0.12,
        totalPrice: 0.72
      }
    ]
  ],
  [
    'MS-HISTORY-0002',
    [
      {
        lineId: 'summary-line-history-002',
        medicationId: 'drug-sodium-chloride',
        medicationName: '0.9%氯化钠注射液',
        specificationText: '250ml/袋',
        itemQuantity: 2,
        unitCode: 'BAG',
        unitCodeText: '袋',
        displayQuantityText: '2袋',
        unitPrice: 4.5,
        totalPrice: 9
      }
    ]
  ]
])

let summarySequence = 1
const createIdempotency = new Map<string, NurseStationCreateDispenseSummaryResultVO>()
const submitIdempotency = new Map<string, NurseStationDispenseSummaryWriteResultVO>()
const cancelIdempotency = new Map<string, NurseStationDispenseSummaryWriteResultVO>()

function clone<T>(value: T): T {
  return structuredClone(value)
}

function roundMoney(value: number) {
  return Number(value.toFixed(2))
}

function assertPage(pageIndex: number, pageSize: number) {
  if (!Number.isInteger(pageIndex) || pageIndex < 1) throw new Error('页码必须从 1 开始')
  if (!Number.isInteger(pageSize) || pageSize < 1) throw new Error('每页条数必须大于 0')
}

function assertIdempotencyKey(key: string) {
  if (!key.trim()) throw new Error('幂等键不能为空')
}

function statusText(status: NurseStationDispenseSummaryStatus) {
  const text: Record<NurseStationDispenseSummaryStatus, string> = {
    CANCELLED: '已撤销',
    DISPENSED: '已发药',
    PENDING_SUBMIT: '待发送',
    SUBMITTED: '已发送'
  }
  return text[status]
}

/** 把分页后的扁平时点重新组装为页面需要的患者折叠卡结构。 */
function groupPendingRecords(records: MockDispenseRecord[]) {
  const patientGroups = new Map<string, NurseStationDispensePatientGroupVO>()
  for (const record of records) {
    let patientGroup = patientGroups.get(record.encounterId)
    if (!patientGroup) {
      patientGroup = {
        encounterId: record.encounterId,
        patientId: record.patientId,
        patientName: record.patientName,
        bedDisplay: record.bedDisplay,
        orderRows: []
      }
      patientGroups.set(record.encounterId, patientGroup)
    }

    let orderRow = patientGroup.orderRows.find(
      (item) => item.medicationRequestId === record.medicationRequestId
    )
    if (!orderRow) {
      orderRow = {
        medicationRequestId: record.medicationRequestId,
        medicationId: record.medicationId,
        medicationName: record.medicationName,
        specificationText: record.specificationText,
        therapyType: record.therapyType,
        therapyTypeText: record.therapyTypeText,
        dosageInstruction: record.dosageInstruction,
        methodText: record.methodText,
        frequencyText: record.frequencyText,
        pharmacyId: record.pharmacyId,
        pharmacyName: record.pharmacyName,
        timePoints: []
      }
      patientGroup.orderRows.push(orderRow)
    }
    orderRow.timePoints.push(clone(record.point))
  }
  return [...patientGroups.values()]
}

/** 查询选中患者仍未被汇总单占用的发药时点。 */
export async function mockQueryNurseStationDispensePending(
  data: NurseStationDispensePendingQueryDTO
): Promise<NurseStationDispensePendingResultVO> {
  assertPage(data.pageIndex, data.pageSize)
  if (!data.encounterIds.length) {
    return { pageIndex: data.pageIndex, pageSize: data.pageSize, total: 0, patientGroups: [] }
  }
  for (const encounterId of data.encounterIds) {
    if (!mockFindNurseStationPatient(encounterId)) throw new Error('包含无效的住院就诊 ID')
  }
  if (!dayjs(data.queryDeadline).isValid()) throw new Error('查询截止时间格式不正确')

  const encounterSet = new Set(data.encounterIds)
  const keyword = data.keyword?.trim().toLowerCase()
  const filtered = dispenseRecords
    .filter((record) => !record.reservedSummaryNo)
    .filter((record) => encounterSet.has(record.encounterId))
    .filter((record) => record.medicineCategory === data.medicineCategory)
    .filter((record) => data.therapyFilter === 'ALL' || record.therapyType === data.therapyFilter)
    .filter(
      (record) =>
        dayjs(record.point.plannedDispenseTime).valueOf() <= dayjs(data.queryDeadline).valueOf()
    )
    .filter((record) => {
      if (!keyword) return true
      return [record.medicationName, record.medicationRequestId, record.patientName].some((value) =>
        value.toLowerCase().includes(keyword)
      )
    })
    .sort((left, right) =>
      [left.encounterId, left.point.plannedDispenseTime, left.point.dispenseId]
        .join('|')
        .localeCompare(
          [right.encounterId, right.point.plannedDispenseTime, right.point.dispenseId].join('|')
        )
    )

  const start = (data.pageIndex - 1) * data.pageSize
  const currentPage = filtered.slice(start, start + data.pageSize)
  return {
    pageIndex: data.pageIndex,
    pageSize: data.pageSize,
    total: filtered.length,
    patientGroups: groupPendingRecords(currentPage)
  }
}

function buildDisplayQuantity(record: MockDispenseRecord, quantity: number) {
  const ratio = record.packageRatio ?? 0
  if (!ratio || !record.packageUnitText) return `${quantity}${record.point.unitCodeText}`
  const packages = Math.floor(quantity / ratio)
  const remainder = roundMoney(quantity - packages * ratio)
  const parts: string[] = []
  if (packages) parts.push(`${packages}${record.packageUnitText}`)
  if (remainder) parts.push(`${remainder}${record.point.unitCodeText}`)
  return parts.join('') || `0${record.point.unitCodeText}`
}

/** 同一药房内按药品合并数量，生成详情弹窗的数据。 */
function buildSummaryLines(summaryNo: string, records: MockDispenseRecord[]) {
  const grouped = new Map<string, MockDispenseRecord[]>()
  for (const record of records) {
    const rows = grouped.get(record.medicationId) ?? []
    rows.push(record)
    grouped.set(record.medicationId, rows)
  }
  return [...grouped.values()].map((rows, index): NurseStationDispenseSummaryLineVO => {
    const first = rows[0]!
    const quantity = roundMoney(rows.reduce((sum, row) => sum + row.point.quantity, 0))
    return {
      lineId: `${summaryNo}-line-${index + 1}`,
      medicationId: first.medicationId,
      medicationName: first.medicationName,
      specificationText: first.specificationText,
      itemQuantity: quantity,
      unitCode: first.point.unitCode,
      unitCodeText: first.point.unitCodeText,
      displayQuantityText: buildDisplayQuantity(first, quantity),
      unitPrice: first.point.chargeUnitPrice,
      totalPrice: roundMoney(quantity * first.point.chargeUnitPrice)
    }
  })
}

/** 创建汇总单；相同幂等键重复调用只返回第一次结果。 */
export async function mockCreateNurseStationDispenseSummary(
  data: NurseStationCreateDispenseSummaryDTO
): Promise<NurseStationCreateDispenseSummaryResultVO> {
  assertIdempotencyKey(data.idempotencyKey)
  const cached = createIdempotency.get(data.idempotencyKey)
  if (cached) return clone(cached)
  if (!data.dispenseItems.length) throw new Error('请至少选择一个待发药时点')
  if (data.dispenseItems.length > 500) throw new Error('单次最多汇总 500 个发药时点')

  const seen = new Set<string>()
  const accepted: MockDispenseRecord[] = []
  const results = data.dispenseItems.map((item) => {
    const key = `${item.encounterId}|${item.dispenseId}`
    const record = dispenseRecords.find(
      (candidate) =>
        candidate.encounterId === item.encounterId && candidate.point.dispenseId === item.dispenseId
    )
    if (seen.has(key)) return { targetId: item.dispenseId, success: false, message: '重复提交' }
    seen.add(key)
    if (!record) return { targetId: item.dispenseId, success: false, message: '发药时点不存在' }
    if (record.reservedSummaryNo) {
      return { targetId: item.dispenseId, success: false, message: '该时点已被其他汇总单占用' }
    }
    accepted.push(record)
    return { targetId: item.dispenseId, success: true, message: '汇总成功' }
  })

  const byPharmacy = new Map<string, MockDispenseRecord[]>()
  for (const record of accepted) {
    const rows = byPharmacy.get(record.pharmacyId) ?? []
    rows.push(record)
    byPharmacy.set(record.pharmacyId, rows)
  }

  const createdNos: string[] = []
  for (const records of byPharmacy.values()) {
    const first = records[0]!
    const summaryNo = `MS${dayjs().format('YYYYMMDD')}${String(summarySequence++).padStart(4, '0')}`
    createdNos.push(summaryNo)
    for (const record of records) record.reservedSummaryNo = summaryNo
    const lines = buildSummaryLines(summaryNo, records)
    summaryHeaders.unshift({
      summaryNo,
      pharmacyId: first.pharmacyId,
      pharmacyName: first.pharmacyName,
      applicantId: 'nurse-current',
      applicantName: '李护士',
      applyTime: dayjs().format('YYYY-MM-DD HH:mm:ss'),
      status: 'PENDING_SUBMIT',
      statusText: statusText('PENDING_SUBMIT'),
      itemCount: lines.length,
      version: 1
    })
    summaryLines.set(summaryNo, lines)
  }

  const response: NurseStationCreateDispenseSummaryResultVO = {
    summaryNos: createdNos,
    processedCount: accepted.length,
    results,
    message: createdNos.length
      ? `已生成 ${createdNos.length} 张汇总单，请在“汇总单”页签发送`
      : '没有可汇总的发药时点'
  }
  createIdempotency.set(data.idempotencyKey, clone(response))
  return response
}

/** 分页查询当前护士站生成的汇总单。 */
export async function mockQueryNurseStationDispenseSummaries(
  data: NurseStationDispenseSummaryQueryDTO
) {
  assertPage(data.pageIndex, data.pageSize)
  const keyword = data.keyword?.trim().toLowerCase()
  const rows = summaryHeaders
    .filter((row) => !data.pharmacyId || row.pharmacyId === data.pharmacyId)
    .filter((row) => !data.status || row.status === data.status)
    .filter((row) => !data.dateFrom || row.applyTime.slice(0, 10) >= data.dateFrom)
    .filter((row) => !data.dateTo || row.applyTime.slice(0, 10) <= data.dateTo)
    .filter((row) => {
      if (!keyword) return true
      return [row.summaryNo, row.applicantName, row.pharmacyName].some((value) =>
        value.toLowerCase().includes(keyword)
      )
    })
    .sort((left, right) => right.applyTime.localeCompare(left.applyTime))
  const start = (data.pageIndex - 1) * data.pageSize
  return createPageDTO({
    pageIndex: data.pageIndex,
    pageSize: data.pageSize,
    total: rows.length,
    rows: clone(rows.slice(start, start + data.pageSize))
  })
}

/** 按统一单号读取单头和合并后的药品明细。 */
export async function mockGetNurseStationDispenseSummaryDetail(
  summaryNo: string
): Promise<NurseStationDispenseSummaryDetailVO> {
  const header = summaryHeaders.find((row) => row.summaryNo === summaryNo)
  const lines = summaryLines.get(summaryNo)
  if (!header || !lines) throw new Error('汇总单不存在')
  return { header: clone(header), lines: clone(lines) }
}

async function changeSummaryStatus(
  data: NurseStationSubmitDispenseSummariesDTO | NurseStationCancelDispenseSummariesDTO,
  action: 'CANCEL' | 'SUBMIT'
): Promise<NurseStationDispenseSummaryWriteResultVO> {
  assertIdempotencyKey(data.idempotencyKey)
  const cache = action === 'SUBMIT' ? submitIdempotency : cancelIdempotency
  const cached = cache.get(data.idempotencyKey)
  if (cached) return clone(cached)
  if (!data.summaries.length) throw new Error('请至少选择一张汇总单')
  if (data.summaries.length > 50) throw new Error('单次最多处理 50 张汇总单')

  const results = data.summaries.map((item) => {
    const header = summaryHeaders.find((row) => row.summaryNo === item.summaryNo)
    if (!header) return { targetId: item.summaryNo, success: false, message: '汇总单不存在' }
    if (header.version !== item.version) {
      return { targetId: item.summaryNo, success: false, message: '单据已更新，请刷新后重试' }
    }
    if (header.status !== 'PENDING_SUBMIT') {
      return { targetId: item.summaryNo, success: false, message: '仅待发送汇总单可操作' }
    }

    if (action === 'SUBMIT') {
      header.status = 'SUBMITTED'
      header.statusText = statusText('SUBMITTED')
    } else {
      header.status = 'CANCELLED'
      header.statusText = statusText('CANCELLED')
      for (const record of dispenseRecords) {
        if (record.reservedSummaryNo === header.summaryNo) record.reservedSummaryNo = undefined
      }
    }
    header.version += 1
    return {
      targetId: item.summaryNo,
      success: true,
      message: action === 'SUBMIT' ? '发送成功' : '撤销成功'
    }
  })

  const response: NurseStationDispenseSummaryWriteResultVO = {
    processedCount: results.filter((item) => item.success).length,
    results,
    message: action === 'SUBMIT' ? '汇总单发送处理完成' : '汇总单撤销处理完成'
  }
  cache.set(data.idempotencyKey, clone(response))
  return response
}

/** 发送待发送汇总单至药房。 */
export function mockSubmitNurseStationDispenseSummaries(
  data: NurseStationSubmitDispenseSummariesDTO
) {
  return changeSummaryStatus(data, 'SUBMIT')
}

/** 撤销待发送汇总单，并释放其占用的发药时点。 */
export function mockCancelNurseStationDispenseSummaries(
  data: NurseStationCancelDispenseSummariesDTO
) {
  return changeSummaryStatus(data, 'CANCEL')
}
