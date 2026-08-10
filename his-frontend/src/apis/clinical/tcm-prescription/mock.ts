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
    specification: '药材',
    defaultDose: 15,
    doseUnit: 'g',
    retailPrice: 0.18
  },
  {
    id: 'herb-002',
    code: 'ZY0002',
    name: '党参',
    specification: '药材',
    defaultDose: 12,
    doseUnit: 'g',
    retailPrice: 0.22
  },
  {
    id: 'herb-003',
    code: 'ZY0003',
    name: '白术',
    specification: '药材',
    defaultDose: 10,
    doseUnit: 'g',
    retailPrice: 0.16
  },
  {
    id: 'herb-004',
    code: 'ZY0004',
    name: '茯苓',
    specification: '药材',
    defaultDose: 12,
    doseUnit: 'g',
    retailPrice: 0.14
  },
  {
    id: 'herb-005',
    code: 'ZY0005',
    name: '炙甘草',
    specification: '药材',
    defaultDose: 6,
    doseUnit: 'g',
    retailPrice: 0.12
  },
  {
    id: 'herb-006',
    code: 'ZY0006',
    name: '桂枝',
    specification: '药材',
    defaultDose: 9,
    doseUnit: 'g',
    retailPrice: 0.2
  },
  {
    id: 'herb-007',
    code: 'ZY0007',
    name: '附子',
    specification: '制附片',
    defaultDose: 6,
    doseUnit: 'g',
    retailPrice: 0.36
  },
  {
    id: 'herb-008',
    code: 'ZY0008',
    name: '当归',
    specification: '药材',
    defaultDose: 10,
    doseUnit: 'g',
    retailPrice: 0.28
  }
]

const statusNames: Record<TcmPrescriptionStatus, string> = {
  DRAFT: '待签发',
  SIGNED: '已签发'
}

const records: TcmPrescriptionVO[] = [
  {
    id: 'tcm-rx-inpatient-001',
    scope: 'inpatient',
    encounterId: 'ipt-enc-20260701-001',
    patientId: 'patient-10001',
    prescriptionNo: 'ZYCF20260727001',
    encounterDiagnosisId: 'diagnosis-inpatient-001',
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
        specification: '药材',
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
        specification: '药材',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.28,
        orderNo: 2
      }
    ],
    authoredAt: '2026-07-26 14:20:00',
    updatedAt: '2026-07-26 14:30:00'
  },
  {
    id: 'tcm-rx-outpatient-001',
    scope: 'outpatient',
    encounterId: 'enc-20260723-003',
    patientId: 'patient-1003',
    prescriptionNo: 'MZCF20260723001',
    encounterDiagnosisId: 'diagnosis-outpatient-001',
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
        specification: '药材',
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
        specification: '药材',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.16,
        orderNo: 2
      }
    ],
    authoredAt: '2026-07-23 10:10:00',
    updatedAt: '2026-07-23 10:20:00'
  },
  {
    id: 'tcm-rx-outpatient-demo-draft',
    scope: 'outpatient',
    encounterId: 'enc-demo-in-progress',
    patientId: 'patient-demo-001',
    encounterDiagnosisId: 'diagnosis-demo-001',
    syndromeName: '气虚血瘀证',
    doseCount: 7,
    method: '水煎服',
    frequency: '每日一剂',
    days: 7,
    decoctionMethod: '代煎',
    remark: '全功能演示：可修改、统一保存、删除或签发',
    status: 'DRAFT',
    statusName: '待签发',
    lines: [
      {
        id: 'tcm-line-demo-draft-001',
        catalogId: 'herb-001',
        code: 'ZY0001',
        name: '黄芪',
        specification: '药材',
        dose: 15,
        doseUnit: 'g',
        retailPrice: 0.18,
        orderNo: 1
      },
      {
        id: 'tcm-line-demo-draft-002',
        catalogId: 'herb-008',
        code: 'ZY0008',
        name: '当归',
        specification: '药材',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.28,
        orderNo: 2
      }
    ],
    authoredAt: '2026-08-02 07:40:00',
    updatedAt: '2026-08-02 07:40:00'
  },
  {
    id: 'tcm-rx-outpatient-demo-signed',
    scope: 'outpatient',
    encounterId: 'enc-demo-in-progress',
    patientId: 'patient-demo-001',
    prescriptionNo: 'MZCF20260802001',
    encounterDiagnosisId: 'diagnosis-demo-002',
    syndromeName: '心脾两虚证',
    doseCount: 5,
    method: '水煎服',
    frequency: '每日一剂',
    days: 5,
    decoctionMethod: '自煎',
    remark: '全功能演示：已签发处方可执行撤回',
    status: 'SIGNED',
    statusName: '已签发',
    lines: [
      {
        id: 'tcm-line-demo-signed-001',
        catalogId: 'herb-002',
        code: 'ZY0002',
        name: '党参',
        specification: '药材',
        dose: 12,
        doseUnit: 'g',
        retailPrice: 0.22,
        orderNo: 1
      },
      {
        id: 'tcm-line-demo-signed-002',
        catalogId: 'herb-004',
        code: 'ZY0004',
        name: '茯苓',
        specification: '药材',
        dose: 12,
        doseUnit: 'g',
        retailPrice: 0.14,
        orderNo: 2
      }
    ],
    authoredAt: '2026-08-02 07:35:00',
    updatedAt: '2026-08-02 07:38:00'
  },
  {
    id: 'tcm-rx-outpatient-demo-history',
    scope: 'outpatient',
    encounterId: 'enc-demo-history-001',
    patientId: 'patient-demo-001',
    prescriptionNo: 'MZCF20260518006',
    encounterDiagnosisId: 'diagnosis-history-001',
    syndromeName: '痰瘀互结证',
    doseCount: 7,
    method: '水煎服',
    frequency: '每日一剂',
    days: 7,
    status: 'SIGNED',
    statusName: '已签发',
    lines: [
      {
        id: 'tcm-line-demo-history-001',
        catalogId: 'herb-006',
        code: 'ZY0006',
        name: '桂枝',
        specification: '药材',
        dose: 9,
        doseUnit: 'g',
        retailPrice: 0.2,
        orderNo: 1
      }
    ],
    authoredAt: '2026-05-18 09:20:00',
    updatedAt: '2026-05-18 09:25:00'
  },
  {
    id: 'tcm-rx-history-001',
    scope: 'outpatient',
    encounterId: 'enc-history-10001',
    patientId: 'patient-10001',
    prescriptionNo: 'MZCF20260112008',
    encounterDiagnosisId: 'diagnosis-history-002',
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
        specification: '药材',
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
        specification: '药材',
        dose: 10,
        doseUnit: 'g',
        retailPrice: 0.28,
        orderNo: 2
      }
    ],
    authoredAt: '2026-01-12 09:30:00',
    updatedAt: '2026-01-12 09:35:00'
  }
]

let nextPrescriptionId = 100
let nextLineId = 1000

/** 预置处方统一从版本 0 开始，模拟后端初始数据。 */
for (const record of records) {
  if (record.version === undefined) record.version = 0
}

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
  if (!record || record.scope !== data.scope || record.encounterId !== data.encounterId) {
    throw new Error('中医处方不存在或不属于当前患者')
  }
  return record
}

function touch(record: TcmPrescriptionVO, patch: Partial<TcmPrescriptionVO>) {
  Object.assign(record, patch, {
    updatedAt: '2026-07-27 11:30:00',
    version: (record.version ?? 0) + 1
  })
}

/**
 * 校验操作请求携带的处方版本。
 *
 * 模拟后端的乐观锁：请求 version 与当前处方 version 不一致时拒绝。
 * 为兼容单测直接调用操作接口，未携带 version 时不校验。
 */
function assertTcmVersion(record: TcmPrescriptionVO, version?: number) {
  if (version === undefined) return
  if (version !== (record.version ?? 0)) {
    throw new Error('中医处方已被其他操作更新，请刷新后重试')
  }
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
        item.scope === query.scope &&
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
  if (data.details.length === 0) return delayReject('请至少添加一味中药材')
  if (!data.encounterDiagnosisId) return delayReject('请选择已保存的就诊诊断')

  const current = data.id ? records.find((item) => item.id === data.id) : undefined
  if (data.id && !current) return delayReject('中医处方不存在或已被删除')
  if (current && current.status !== 'DRAFT') return delayReject('只有待签发处方可以修改')
  if (current && data.version !== undefined && data.version !== (current.version ?? 0)) {
    return delayReject('中医处方已被其他操作更新，请刷新后重试')
  }
  const lines = data.details.map((line, index) => ({
    id: `tcm-line-${nextLineId++}`,
    catalogId: line.medicineId,
    code: herbs.find((item) => item.id === line.medicineId)?.code || '',
    name: herbs.find((item) => item.id === line.medicineId)?.name || '',
    specification: herbs.find((item) => item.id === line.medicineId)?.specification || '',
    dose: line.quantity,
    doseUnit: line.unit || line.doseUnit || '',
    retailPrice: herbs.find((item) => item.id === line.medicineId)?.retailPrice || 0,
    orderNo: index + 1
  }))
  if (current) {
    touch(current, {
      encounterDiagnosisId: data.encounterDiagnosisId,
      syndromeName: data.diagnosisName,
      doseCount: data.copies,
      method: data.usage,
      frequency: data.frequency,
      days: data.days,
      decoctionMethod: data.decoction ? '代煎' : '自煎',
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
    encounterDiagnosisId: data.encounterDiagnosisId,
    syndromeName: data.diagnosisName,
    doseCount: data.copies,
    method: data.usage,
    frequency: data.frequency,
    days: data.days,
    decoctionMethod: data.decoction ? '代煎' : '自煎',
    remark: data.remark,
    status: 'DRAFT',
    statusName: statusNames.DRAFT,
    lines,
    authoredAt: '2026-07-27 11:30:00',
    updatedAt: '2026-07-27 11:30:00',
    version: 0
  }
  records.push(created)
  return delay(clonePrescription(created))
}

export function mockDeleteTcmPrescription(data: TcmPrescriptionActionDTO): Promise<void> {
  try {
    const record = findRecord(data)
    if (record.status !== 'DRAFT') throw new Error('只有待签发处方可以删除')
    assertTcmVersion(record, data.version)
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
    assertTcmVersion(record, data.version)
    touch(record, {
      status: target,
      statusName: statusNames[target],
      prescriptionNo:
        target === 'SIGNED' && !record.prescriptionNo
          ? `${record.scope === 'inpatient' ? 'ZYCF' : 'MZCF'}20260727${String(nextPrescriptionId++).padStart(3, '0')}`
          : record.prescriptionNo
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
  return changeStatus(data, 'SIGNED', 'DRAFT')
}
