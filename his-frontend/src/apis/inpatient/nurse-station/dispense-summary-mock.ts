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
  NurseStationDispenseSummaryHeaderVO,
  NurseStationDispenseSummaryLineVO,
  NurseStationDispenseSummaryQueryDTO,
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
    desLocationId: 'ward-301',
    plannedDispenseTime: `${today} 08:00:00`,
    statusEnum: 'SUMMARIZED',
    itemCount: 1
  },
  {
    summaryNo: 'MS-HISTORY-0002',
    desLocationId: 'ward-301',
    plannedDispenseTime: `${today} 07:00:00`,
    dispenseTime: `${today} 07:20:00`,
    statusEnum: 'DISPENSED',
    itemCount: 1
  }
]

const summaryLines = new Map<string, NurseStationDispenseSummaryLineVO[]>([
  [
    'MS-HISTORY-0001',
    [
      {
        id: 'summary-line-history-001',
        name: '维生素C片',
        quantity: 6,
        unitCode: 'TABLET'
      }
    ]
  ],
  [
    'MS-HISTORY-0002',
    [
      {
        id: 'summary-line-history-002',
        name: '0.9%氯化钠注射液',
        quantity: 2,
        unitCode: 'BAG'
      }
    ]
  ]
])

let summarySequence = 1

function clone<T>(value: T): T {
  return structuredClone(value)
}

function assertPage(pageIndex: number, pageSize: number) {
  if (!Number.isInteger(pageIndex) || pageIndex < 1) throw new Error('页码必须从 1 开始')
  if (!Number.isInteger(pageSize) || pageSize < 1) throw new Error('每页条数必须大于 0')
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

/** Build raw dispense detail rows using the backend response contract. */
function buildSummaryLines(summaryNo: string, records: MockDispenseRecord[]) {
  return records.map(
    (record, index): NurseStationDispenseSummaryLineVO => ({
      id: `${summaryNo}-line-${index + 1}`,
      name: record.medicationName,
      quantity: record.point.quantity,
      unitCode: record.point.unitCode
    })
  )
}

/** Create summary slips with the same minimal contract as the backend. */
export async function mockCreateNurseStationDispenseSummary(
  data: NurseStationCreateDispenseSummaryDTO
): Promise<NurseStationCreateDispenseSummaryResultVO> {
  if (!data.dispenseItems.length) throw new Error('请至少选择一个待发药时点')
  if (data.dispenseItems.length > 500) throw new Error('单次最多汇总 500 个发药时点')

  const seen = new Set<string>()
  const accepted: MockDispenseRecord[] = []
  for (const item of data.dispenseItems) {
    const key = `${item.encounterId}|${item.dispenseId}`
    const record = dispenseRecords.find(
      (candidate) =>
        candidate.encounterId === item.encounterId && candidate.point.dispenseId === item.dispenseId
    )
    if (seen.has(key)) throw new Error('不能重复提交同一发药时点')
    seen.add(key)
    if (!record) throw new Error('发药时点不存在')
    if (record.reservedSummaryNo) throw new Error('该时点已被其他汇总单占用')
    accepted.push(record)
  }

  const byPharmacy = new Map<string, MockDispenseRecord[]>()
  for (const record of accepted) {
    const rows = byPharmacy.get(record.pharmacyId) ?? []
    rows.push(record)
    byPharmacy.set(record.pharmacyId, rows)
  }

  const createdNos: string[] = []
  for (const records of byPharmacy.values()) {
    const summaryNo = `MS${dayjs().format('YYYYMMDD')}${String(summarySequence++).padStart(4, '0')}`
    createdNos.push(summaryNo)
    for (const record of records) record.reservedSummaryNo = summaryNo
    const lines = buildSummaryLines(summaryNo, records)
    summaryHeaders.unshift({
      summaryNo,
      desLocationId: 'ward-301',
      plannedDispenseTime: records
        .map((record) => record.point.plannedDispenseTime)
        .sort()
        .at(-1)!,
      statusEnum: 'SUMMARIZED',
      itemCount: lines.length
    })
    summaryLines.set(summaryNo, lines)
  }

  const response: NurseStationCreateDispenseSummaryResultVO = {
    summaryNos: createdNos,
    message: createdNos.length
      ? `已生成 ${createdNos.length} 张汇总单，请在“汇总单”页签发送`
      : '没有可汇总的发药时点'
  }
  return response
}

/** 分页查询当前护士站生成的汇总单。 */
export async function mockQueryNurseStationDispenseSummaries(
  data: NurseStationDispenseSummaryQueryDTO
) {
  assertPage(data.pageIndex, data.pageSize)
  const rows = summaryHeaders
    .filter((row) => !data.statusEnum || row.statusEnum === data.statusEnum)
    .filter(
      (row) => !data.dispenseDate || row.plannedDispenseTime.slice(0, 10) === data.dispenseDate
    )
    .sort((left, right) => right.summaryNo.localeCompare(left.summaryNo))
  const start = (data.pageIndex - 1) * data.pageSize
  return createPageDTO({
    pageIndex: data.pageIndex,
    pageSize: data.pageSize,
    total: rows.length,
    rows: clone(rows.slice(start, start + data.pageSize))
  })
}

/** Read raw medication rows for one summary slip. */
export async function mockGetNurseStationDispenseSummaryDetail(
  summaryNo: string
): Promise<NurseStationDispenseSummaryLineVO[]> {
  const header = summaryHeaders.find((row) => row.summaryNo === summaryNo)
  const lines = summaryLines.get(summaryNo)
  if (!header || !lines) throw new Error('汇总单不存在')
  return clone(lines)
}

async function changeSummaryStatus(
  data: NurseStationSubmitDispenseSummariesDTO | NurseStationCancelDispenseSummariesDTO,
  action: 'CANCEL' | 'SUBMIT'
): Promise<NurseStationDispenseSummaryWriteResultVO> {
  if (!data.summaryNos.length) throw new Error('请至少选择一张汇总单')
  if (data.summaryNos.length > 50) throw new Error('单次最多处理 50 张汇总单')

  const headers = data.summaryNos.map((summaryNo) => {
    const header = summaryHeaders.find((row) => row.summaryNo === summaryNo)
    if (!header) throw new Error('汇总单不存在')
    if (header.statusEnum !== 'SUMMARIZED') throw new Error('仅待发送汇总单可操作')
    return header
  })

  for (const header of headers) {
    if (action === 'SUBMIT') {
      header.statusEnum = 'WAIT_REVIEW'
      continue
    }
    for (const record of dispenseRecords) {
      if (record.reservedSummaryNo === header.summaryNo) record.reservedSummaryNo = undefined
    }
    const index = summaryHeaders.indexOf(header)
    if (index >= 0) summaryHeaders.splice(index, 1)
    summaryLines.delete(header.summaryNo)
  }

  return {
    summaryNos: clone(data.summaryNos),
    message: action === 'SUBMIT' ? '汇总单发送成功' : '汇总单撤销成功'
  }
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
