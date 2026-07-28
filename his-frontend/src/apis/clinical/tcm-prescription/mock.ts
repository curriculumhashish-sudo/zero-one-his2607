import { createPageDTO, type PageDTO } from '../../type'
import type {
  SaveTcmPrescriptionDTO,
  TcmHerbCatalogItemVO,
  TcmHerbCatalogQuery,
  TcmPrescriptionActionDTO,
  TcmPrescriptionHistoryQuery,
  TcmPrescriptionHistoryVO,
  TcmPrescriptionQuery,
  TcmPrescriptionStatus,
  TcmPrescriptionVO
} from './type'

const herbs: TcmHerbCatalogItemVO[] = [
  {
    id: 'herb-001',
    code: 'ZY0001',
    name: '黄芪',
    specification: '饮片',
    defaultDose: 15,
    doseUnit: 'g',
    retailPrice: 0.18
  },
  {
    id: 'herb-002',
    code: 'ZY0002',
    name: '党参',
    specification: '饮片',
    defaultDose: 12,
    doseUnit: 'g',
    retailPrice: 0.22
  },
  {
    id: 'herb-003',
    code: 'ZY0003',
    name: '白术',
    specification: '饮片',
    defaultDose: 10,
    doseUnit: 'g',
    retailPrice: 0.16
  },
  {
    id: 'herb-004',
    code: 'ZY0004',
    name: '茯苓',
    specification: '饮片',
    defaultDose: 12,
    doseUnit: 'g',
    retailPrice: 0.14
  },
  {
    id: 'herb-005',
    code: 'ZY0005',
    name: '炙甘草',
    specification: '饮片',
    defaultDose: 6,
    doseUnit: 'g',
    retailPrice: 0.12
  },
  {
    id: 'herb-006',
    code: 'ZY0006',
    name: '桂枝',
    specification: '饮片',
    defaultDose: 9,
    doseUnit: 'g',
    retailPrice: 0.2,
    defaultDecoctionMethod: '后下'
  },
  {
    id: 'herb-007',
    code: 'ZY0007',
    name: '附子',
    specification: '制附片',
    defaultDose: 6,
    doseUnit: 'g',
    retailPrice: 0.36,
    defaultDecoctionMethod: '先煎'
  },
  {
    id: 'herb-008',
    code: 'ZY0008',
    name: '当归',
    specification: '饮片',
    defaultDose: 10,
    doseUnit: 'g',
    retailPrice: 0.28
  }
]

const statusNames: Record<TcmPrescriptionStatus, string> = {
  DRAFT: '待签发',
  SIGNED: '已签发',
  STOPPED: '已停嘱',
  WITHDRAWN: '已撤回'
}

const records: TcmPrescriptionVO[] = [
  {
    id: 'tcm-rx-inpatient-001',
    scope: 'inpatient',
    encounterId: 'ipt-enc-20260701-001',
    patientId: 'patient-10001',
    prescriptionNo: 'ZYCF20260727001',
    therapyType: 'LONG_TERM',
    syndromeName: '气虚血瘀证',
    doseCount: 7,
    method: '水煎服',
    frequency: '每日一剂',
    days: 7,
    decoctionMethod: '代煎',
    remark: '早晚温服',
    status: 'SIGNED',
    statusName: '已签发',
    lines: [
      {
        id: 'tcm-line-in-001',
        catalogId: 'herb-001',
        code: 'ZY0001',
        name: '黄芪',
        specification: '饮片',
        dose: 15,
        doseUnit: 'g',
        retailPrice: 0.18,
        orderNo: 1
      },
      {
        id: 'tcm-line-in-002',
        catalogId: 'herb-008',
        code: 'ZY0008',
        name: '当归',
        specification: '饮片',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.28,
        orderNo: 2
      }
    ],
    authoredAt: '2026-07-26 14:20:00',
    signedAt: '2026-07-26 14:30:00',
    updatedAt: '2026-07-26 14:30:00',
    version: 1
  },
  {
    id: 'tcm-rx-outpatient-001',
    scope: 'outpatient',
    encounterId: 'enc-20260723-003',
    patientId: 'patient-1003',
    prescriptionNo: 'MZCF20260723001',
    syndromeName: '脾气虚证',
    doseCount: 5,
    method: '水煎服',
    frequency: '每日一剂',
    days: 5,
    remark: '饭后温服',
    status: 'SIGNED',
    statusName: '已签发',
    lines: [
      {
        id: 'tcm-line-out-001',
        catalogId: 'herb-002',
        code: 'ZY0002',
        name: '党参',
        specification: '饮片',
        dose: 12,
        doseUnit: 'g',
        retailPrice: 0.22,
        orderNo: 1
      },
      {
        id: 'tcm-line-out-002',
        catalogId: 'herb-003',
        code: 'ZY0003',
        name: '白术',
        specification: '饮片',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.16,
        orderNo: 2
      }
    ],
    authoredAt: '2026-07-23 10:10:00',
    signedAt: '2026-07-23 10:20:00',
    updatedAt: '2026-07-23 10:20:00',
    version: 1
  },
  {
    id: 'tcm-rx-history-001',
    scope: 'outpatient',
    encounterId: 'enc-history-10001',
    patientId: 'patient-10001',
    prescriptionNo: 'MZCF20260112008',
    syndromeName: '胸痹气虚血瘀证',
    doseCount: 7,
    method: '水煎服',
    frequency: '每日一剂',
    days: 7,
    status: 'SIGNED',
    statusName: '已签发',
    lines: [
      {
        id: 'tcm-line-history-001',
        catalogId: 'herb-001',
        code: 'ZY0001',
        name: '黄芪',
        specification: '饮片',
        dose: 15,
        doseUnit: 'g',
        retailPrice: 0.18,
        orderNo: 1
      },
      {
        id: 'tcm-line-history-002',
        catalogId: 'herb-008',
        code: 'ZY0008',
        name: '当归',
        specification: '饮片',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.28,
        orderNo: 2
      }
    ],
    authoredAt: '2026-01-12 09:30:00',
    signedAt: '2026-01-12 09:35:00',
    updatedAt: '2026-01-12 09:35:00',
    version: 1
  }
]

let nextPrescriptionId = 100
let nextLineId = 1000

function clonePrescription(record: TcmPrescriptionVO): TcmPrescriptionVO {
  return { ...record, lines: record.lines.map((line) => ({ ...line })) }
}

function delay<T>(value: T): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(value), 120))
}

function delayReject(message: string): Promise<never> {
  return new Promise((_, reject) => setTimeout(() => reject(new Error(message)), 120))
}

function findRecord(data: TcmPrescriptionActionDTO) {
  const record = records.find((item) => item.id === data.id)
  if (
    !record ||
    record.scope !== data.scope ||
    record.encounterId !== data.encounterId ||
    record.patientId !== data.patientId
  ) {
    throw new Error('中医处方不存在或不属于当前患者')
  }
  if (record.version !== data.version) throw new Error('中医处方已被他人修改，请刷新后重试')
  return record
}

function touch(record: TcmPrescriptionVO, patch: Partial<TcmPrescriptionVO>) {
  Object.assign(record, patch, {
    updatedAt: '2026-07-27 11:30:00',
    version: record.version + 1
  })
}

export function mockListTcmPrescriptions(query: TcmPrescriptionQuery) {
  return delay(
    records
      .filter(
        (item) =>
          item.scope === query.scope &&
          item.encounterId === query.encounterId &&
          item.patientId === query.patientId
      )
      .map(clonePrescription)
  )
}

export function mockListTcmPrescriptionHistory(
  query: TcmPrescriptionHistoryQuery
): Promise<TcmPrescriptionHistoryVO[]> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const result = records
    .filter(
      (item) =>
        item.patientId === query.patientId &&
        item.encounterId !== query.currentEncounterId &&
        item.status !== 'DRAFT'
    )
    .filter((item) => {
      if (!keyword) return true
      return [item.prescriptionNo, item.syndromeName, ...item.lines.map((line) => line.name)].some(
        (value) => value?.toLowerCase().includes(keyword)
      )
    })
    .map((item) => ({
      id: `history-${item.id}`,
      prescriptionNo: item.prescriptionNo ?? '-',
      encounterId: item.encounterId,
      patientId: item.patientId,
      authoredAt: item.authoredAt,
      summary: `${item.syndromeName} · ${item.doseCount}付`,
      herbNames: item.lines.map((line) => line.name)
    }))
  return delay(result)
}

export function mockPageTcmHerbCatalog(
  query: TcmHerbCatalogQuery
): Promise<PageDTO<TcmHerbCatalogItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const matched = herbs.filter(
    (item) =>
      !keyword ||
      [item.code, item.name, item.specification].some((value) =>
        value.toLowerCase().includes(keyword)
      )
  )
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const start = (pageIndex - 1) * pageSize
  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows: matched.slice(start, start + pageSize).map((item) => ({ ...item }))
    })
  )
}

export function mockSaveTcmPrescriptionDraft(
  data: SaveTcmPrescriptionDTO
): Promise<TcmPrescriptionVO> {
  if (data.lines.length === 0) return delayReject('请至少添加一味中药饮片')
  if (data.scope === 'inpatient' && !data.therapyType) {
    return delayReject('住院中医处方必须选择长期或临时')
  }

  const current = data.id ? records.find((item) => item.id === data.id) : undefined
  if (data.id && !current) return delayReject('中医处方不存在或已被删除')
  if (current && current.status !== 'DRAFT') return delayReject('只有待签发处方可以修改')
  if (current && current.version !== data.version) {
    return delayReject('中医处方已被他人修改，请刷新后重试')
  }

  const lines = data.lines.map((line, index) => ({
    ...line,
    id: line.id || `tcm-line-${nextLineId++}`,
    orderNo: index + 1
  }))
  if (current) {
    touch(current, {
      therapyType: data.scope === 'inpatient' ? data.therapyType : undefined,
      syndromeName: data.syndromeName,
      doseCount: data.doseCount,
      method: data.method,
      frequency: data.frequency,
      days: data.days,
      decoctionMethod: data.decoctionMethod,
      remark: data.remark,
      lines
    })
    return delay(clonePrescription(current))
  }

  const created: TcmPrescriptionVO = {
    id: `tcm-rx-${nextPrescriptionId++}`,
    scope: data.scope,
    encounterId: data.encounterId,
    patientId: data.patientId,
    therapyType: data.scope === 'inpatient' ? data.therapyType : undefined,
    syndromeName: data.syndromeName,
    doseCount: data.doseCount,
    method: data.method,
    frequency: data.frequency,
    days: data.days,
    decoctionMethod: data.decoctionMethod,
    remark: data.remark,
    status: 'DRAFT',
    statusName: statusNames.DRAFT,
    lines,
    authoredAt: '2026-07-27 11:30:00',
    updatedAt: '2026-07-27 11:30:00',
    version: 1
  }
  records.push(created)
  return delay(clonePrescription(created))
}

export function mockDeleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  try {
    const record = findRecord(data)
    if (record.status !== 'DRAFT') throw new Error('只有待签发处方可以删除')
    records.splice(records.indexOf(record), 1)
    return delay(undefined)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '中医处方删除失败')
  }
}

function changeStatus(
  data: TcmPrescriptionActionDTO,
  expected: TcmPrescriptionStatus,
  target: TcmPrescriptionStatus
) {
  try {
    const record = findRecord(data)
    if (record.status !== expected) throw new Error(`当前处方状态不允许${statusNames[target]}`)
    if (
      (target === 'STOPPED' || (expected === 'STOPPED' && target === 'SIGNED')) &&
      (record.scope !== 'inpatient' || record.therapyType !== 'LONG_TERM')
    ) {
      throw new Error('只有住院长期中医处方支持停嘱操作')
    }
    touch(record, {
      status: target,
      statusName: statusNames[target],
      prescriptionNo:
        target === 'SIGNED' && !record.prescriptionNo
          ? `${record.scope === 'inpatient' ? 'ZYCF' : 'MZCF'}20260727${String(nextPrescriptionId++).padStart(3, '0')}`
          : record.prescriptionNo,
      signedAt: target === 'SIGNED' ? '2026-07-27 11:35:00' : record.signedAt,
      stoppedAt: target === 'STOPPED' ? '2026-07-27 11:40:00' : undefined
    })
    return delay(clonePrescription(record))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '中医处方状态更新失败')
  }
}

export function mockSignTcmPrescription(data: TcmPrescriptionActionDTO) {
  return changeStatus(data, 'DRAFT', 'SIGNED')
}

export function mockWithdrawTcmPrescription(data: TcmPrescriptionActionDTO) {
  return changeStatus(data, 'SIGNED', 'WITHDRAWN')
}

export function mockStopTcmPrescription(data: TcmPrescriptionActionDTO) {
  return changeStatus(data, 'SIGNED', 'STOPPED')
}

export function mockCancelStopTcmPrescription(data: TcmPrescriptionActionDTO) {
  return changeStatus(data, 'STOPPED', 'SIGNED')
}
