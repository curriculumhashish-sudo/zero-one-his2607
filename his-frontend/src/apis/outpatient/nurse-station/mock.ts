import { createPageDTO, type PageDTO } from '../../type'
import dayjs from 'dayjs'
import type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionCommandResultVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NursePrintDocumentType,
  NurseExecutionRecordVO,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  PerformNurseExecutionsDTO,
  NursePrintRequestDTO,
  NursePrintResultVO
} from './type'

const MOCK_DELAY = 180
const MOCK_TODAY = dayjs().format('YYYY-MM-DD')

const printDocumentLabels: Record<NursePrintDocumentType, string> = {
  BOTTLE_LABEL: '瓶贴',
  TREATMENT_SHEET: '处置单',
  SPECIMEN_BARCODE: '采血码',
  PRESCRIPTION: '处方单',
  SKIN_TEST_LABEL: '皮试标签',
  NEBULIZATION_LABEL: '雾化标签',
  PROCEDURE_APPLICATION: '处置申请单'
}

const patients: NursePatientQueueItemVO[] = [
  {
    encounterId: 'enc-nur-20260728-001',
    patientId: 'pat-001',
    patientName: '林安然',
    genderName: '女',
    ageText: '32岁',
    medicalRecordNo: 'M20260728001',
    visitNo: 'V20260728001',
    queueNumber: 8,
    registeredAt: `${MOCK_TODAY} 08:35`,
    departmentName: '内科门诊',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    paymentTypeName: '医保',
    pendingCount: 4,
    riskHint: '青霉素过敏'
  },
  {
    encounterId: 'enc-nur-20260725-001',
    patientId: 'pat-001',
    patientName: '林安然',
    genderName: '女',
    ageText: '32岁',
    medicalRecordNo: 'M20260728001',
    visitNo: 'V20260725001',
    queueNumber: 3,
    registeredAt: '2026-07-25 09:12',
    departmentName: '内科门诊',
    visitTypeName: '普通门诊',
    status: 'COMPLETED',
    statusName: '已完成',
    paymentTypeName: '医保',
    pendingCount: 0,
    riskHint: '青霉素过敏'
  },
  {
    encounterId: 'enc-nur-20260728-002',
    patientId: 'pat-002',
    patientName: '周远航',
    genderName: '男',
    ageText: '46岁',
    medicalRecordNo: 'M20260728002',
    visitNo: 'V20260728002',
    queueNumber: 11,
    registeredAt: `${MOCK_TODAY} 09:05`,
    departmentName: '外科门诊',
    visitTypeName: '普通门诊',
    status: 'WAITING',
    statusName: '候诊',
    paymentTypeName: '自费',
    pendingCount: 2,
    priorityName: '优先'
  },
  {
    encounterId: 'enc-nur-20260728-003',
    patientId: 'pat-003',
    patientName: '许知夏',
    genderName: '女',
    ageText: '28岁',
    medicalRecordNo: 'M20260728003',
    visitNo: 'V20260728003',
    queueNumber: 15,
    registeredAt: `${MOCK_TODAY} 09:42`,
    departmentName: '内科门诊',
    visitTypeName: '专家门诊',
    status: 'COMPLETED',
    statusName: '已完成',
    paymentTypeName: '医保',
    pendingCount: 0
  }
]

const executionItems: Record<string, NurseExecutionItemVO[]> = {
  'enc-nur-20260725-001': [
    {
      itemId: 'exec-hist-001',
      requestId: 'service-010',
      requestType: 'SERVICE',
      requestTypeName: '处置',
      orderContent: '静脉采血',
      specification: '静脉血 5ml',
      status: 'COMPLETED',
      statusName: '已完成',
      plannedCount: 1,
      executedCount: 1,
      executorName: '护士甲',
      executedAt: '2026-07-25 09:45',
      version: 1,
      printTypes: ['SPECIMEN_BARCODE']
    },
    {
      itemId: 'exec-hist-002',
      requestId: 'service-011',
      requestType: 'SERVICE',
      requestTypeName: '处置',
      orderContent: '血压测量',
      specification: '单次',
      status: 'COMPLETED',
      statusName: '已完成',
      plannedCount: 1,
      executedCount: 1,
      executorName: '护士丙',
      executedAt: '2026-07-25 09:30',
      version: 1,
      printTypes: ['TREATMENT_SHEET']
    },
    {
      itemId: 'exec-hist-003',
      requestId: 'medication-010',
      requestType: 'MEDICATION',
      requestTypeName: '药品',
      orderContent: '阿莫西林皮试',
      specification: '0.1ml 皮内注射',
      status: 'COMPLETED',
      statusName: '已完成',
      plannedCount: 1,
      executedCount: 1,
      executorName: '护士甲',
      executedAt: '2026-07-25 09:20',
      version: 1,
      printTypes: ['SKIN_TEST_LABEL']
    }
  ],
  'enc-nur-20260728-001': [
    {
      itemId: 'exec-001',
      requestId: 'service-001',
      requestType: 'SERVICE',
      requestTypeName: '处置',
      orderContent: '静脉输液',
      specification: '0.9%氯化钠注射液 250ml，静滴',
      status: 'PENDING',
      statusName: '待执行',
      plannedCount: 1,
      executedCount: 0,
      version: 0,
      printTypes: ['TREATMENT_SHEET', 'PROCEDURE_APPLICATION']
    },
    {
      itemId: 'exec-002',
      requestId: 'service-002',
      requestType: 'SERVICE',
      requestTypeName: '治疗',
      orderContent: '雾化吸入治疗',
      specification: '每日 1 次',
      status: 'PENDING',
      statusName: '待执行',
      plannedCount: 2,
      executedCount: 1,
      version: 0,
      printTypes: ['NEBULIZATION_LABEL']
    },
    {
      itemId: 'exec-003',
      requestId: 'medication-001',
      requestType: 'MEDICATION',
      requestTypeName: '药品',
      orderContent: '维生素 C 注射液',
      specification: '2ml:0.5g × 2 支',
      status: 'PENDING',
      statusName: '待执行',
      plannedCount: 1,
      executedCount: 0,
      version: 0,
      printTypes: ['BOTTLE_LABEL', 'PRESCRIPTION', 'SKIN_TEST_LABEL']
    },
    {
      itemId: 'exec-007',
      requestId: 'service-006',
      requestType: 'SERVICE',
      requestTypeName: '检验',
      orderContent: '血常规',
      specification: '静脉血 2ml',
      status: 'PENDING',
      statusName: '待执行',
      plannedCount: 1,
      executedCount: 0,
      version: 0,
      printTypes: ['SPECIMEN_BARCODE']
    }
  ],
  'enc-nur-20260728-002': [
    {
      itemId: 'exec-004',
      requestId: 'service-003',
      requestType: 'SERVICE',
      requestTypeName: '处置',
      orderContent: '伤口换药',
      specification: '小换药',
      status: 'PENDING',
      statusName: '待执行',
      plannedCount: 1,
      executedCount: 0,
      version: 0,
      printTypes: ['TREATMENT_SHEET', 'PROCEDURE_APPLICATION']
    },
    {
      itemId: 'exec-005',
      requestId: 'service-004',
      requestType: 'SERVICE',
      requestTypeName: '治疗',
      orderContent: '红外线治疗',
      specification: '20 分钟',
      status: 'COMPLETED',
      statusName: '已完成',
      plannedCount: 1,
      executedCount: 1,
      executorName: '护士甲',
      executedAt: '2026-07-28 09:36',
      version: 0,
      printTypes: ['TREATMENT_SHEET', 'PROCEDURE_APPLICATION']
    }
  ],
  'enc-nur-20260728-003': [
    {
      itemId: 'exec-006',
      requestId: 'service-005',
      requestType: 'SERVICE',
      requestTypeName: '处置',
      orderContent: '肌内注射',
      specification: '单次',
      status: 'COMPLETED',
      statusName: '已完成',
      plannedCount: 1,
      executedCount: 1,
      executorName: '护士乙',
      executedAt: '2026-07-28 10:02',
      version: 0,
      printTypes: ['TREATMENT_SHEET', 'PROCEDURE_APPLICATION']
    }
  ]
}

const consumables: Record<string, NurseConsumableItemVO[]> = {
  'enc-nur-20260728-001': [
    {
      dispenseItemId: 'dispense-001',
      materialId: 'material-001',
      materialName: '一次性使用输液器',
      specification: '带针 0.7mm',
      quantity: 1,
      unitName: '套',
      roomName: '门诊耗材室',
      status: 'PENDING',
      statusName: '待发放'
    },
    {
      dispenseItemId: 'dispense-002',
      materialId: 'material-002',
      materialName: '医用棉签',
      specification: '10cm',
      quantity: 2,
      unitName: '包',
      roomName: '门诊耗材室',
      status: 'DISPENSED',
      statusName: '已发放'
    }
  ],
  'enc-nur-20260728-002': [
    {
      dispenseItemId: 'dispense-003',
      materialId: 'material-003',
      materialName: '无菌敷贴',
      specification: '10cm × 10cm',
      quantity: 2,
      unitName: '片',
      roomName: '门诊耗材室',
      status: 'PENDING',
      statusName: '待发放'
    }
  ]
}

const executionIdempotencyResults = new Map<string, NurseExecutionCommandResultVO>()

function syncPatientPendingCount(encounterId: string) {
  const patient = patients.find((item) => item.encounterId === encounterId)
  if (!patient) return
  patient.pendingCount = (executionItems[encounterId] ?? []).filter(
    (item) => item.status === 'PENDING'
  ).length
  patient.status = patient.pendingCount > 0 ? 'READY' : 'COMPLETED'
  patient.statusName = patient.pendingCount > 0 ? '待执行' : '已完成'
}

function delay<T>(value: T): Promise<T> {
  return new Promise((resolve) => {
    window.setTimeout(() => resolve(value), MOCK_DELAY)
  })
}

function copy<T>(value: T): T {
  return structuredClone(value)
}

/** Mock：分页查询患者队列。 */
export async function mockPageNursePatientQueue(
  query: NursePatientQueueQuery
): Promise<PageDTO<NursePatientQueueItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const filtered = patients.filter((patient) => {
    const matchedKeyword =
      !keyword ||
      [patient.patientName, patient.medicalRecordNo, patient.visitNo].some((value) =>
        value.toLowerCase().includes(keyword)
      )
    const visitDate = patient.registeredAt.slice(0, 10)
    const matchedStart = !query.startDate || visitDate >= query.startDate
    const matchedEnd = !query.endDate || visitDate <= query.endDate
    return matchedKeyword && matchedStart && matchedEnd
  })
  const start = Math.max(0, (query.pageIndex - 1) * query.pageSize)
  const page = createPageDTO<NursePatientQueueItemVO>()
  page.pageIndex = query.pageIndex
  page.pageSize = query.pageSize
  page.total = filtered.length
  page.pages = Math.ceil(filtered.length / query.pageSize)
  page.rows = copy(filtered.slice(start, start + query.pageSize))
  return delay(page)
}

/** Mock：查询当前就诊的执行项目。 */
export function mockGetNurseExecutionItems(encounterId: string) {
  return delay(copy(executionItems[encounterId] ?? []))
}

function formatMockDateTime() {
  return new Intl.DateTimeFormat('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    hour12: false
  })
    .format(new Date())
    .replaceAll('/', '-')
}

/** Mock：批量执行选中的护士项目，支持幂等与逐项版本检查。 */
export function mockPerformNurseExecutions(
  request: PerformNurseExecutionsDTO
): Promise<NurseExecutionCommandResultVO> {
  const cached = executionIdempotencyResults.get(request.idempotencyKey)
  if (cached) return delay(copy(cached))

  const rows = executionItems[request.encounterId] ?? []
  const result: NurseExecutionCommandResultVO = {
    successItems: [],
    failedItems: []
  }

  request.items.forEach((command) => {
    const row = rows.find((item) => item.itemId === command.itemId)
    if (!row) {
      result.failedItems.push({
        itemId: command.itemId,
        code: 'ITEM_NOT_FOUND',
        message: '执行项目不存在'
      })
      return
    }
    if (row.version !== command.version) {
      result.failedItems.push({
        itemId: command.itemId,
        code: 'VERSION_CONFLICT',
        message: '项目已被其他人修改，请刷新后重试'
      })
      return
    }
    if (row.status !== 'PENDING') {
      result.failedItems.push({
        itemId: command.itemId,
        code: 'INVALID_STATUS',
        message: '当前状态不可执行'
      })
      return
    }

    row.executedCount += 1
    row.status = row.executedCount >= row.plannedCount ? 'COMPLETED' : 'PENDING'
    row.statusName = row.status === 'COMPLETED' ? '已完成' : '待执行'
    row.executorName = '模拟护士'
    row.executedAt = formatMockDateTime()
    row.version += 1
    result.successItems.push(copy(row))
  })

  syncPatientPendingCount(request.encounterId)
  executionIdempotencyResults.set(request.idempotencyKey, copy(result))
  return delay(result)
}

/** Mock：取消项目最近一次执行。 */
export function mockCancelNurseExecution(
  request: CancelNurseExecutionDTO
): Promise<NurseExecutionCommandResultVO> {
  const rows = executionItems[request.encounterId] ?? []
  const row = rows.find((item) => item.itemId === request.item.itemId)
  const result: NurseExecutionCommandResultVO = {
    successItems: [],
    failedItems: []
  }

  if (!row) {
    result.failedItems.push({
      itemId: request.item.itemId,
      code: 'ITEM_NOT_FOUND',
      message: '执行项目不存在'
    })
  } else if (row.version !== request.item.version) {
    result.failedItems.push({
      itemId: request.item.itemId,
      code: 'VERSION_CONFLICT',
      message: '项目已被其他人修改，请刷新后重试'
    })
  } else if (row.executedCount <= 0 || row.status === 'CANCELLED') {
    result.failedItems.push({
      itemId: request.item.itemId,
      code: 'INVALID_STATUS',
      message: '当前项目没有可取消的执行记录'
    })
  } else {
    row.executedCount -= 1
    row.status = 'PENDING'
    row.statusName = '待执行'
    row.executorName = undefined
    row.executedAt = undefined
    row.version += 1
    result.successItems.push(copy(row))
  }

  syncPatientPendingCount(request.encounterId)
  return delay(result)
}

/** Mock：查询当前就诊的耗材项目。 */
export function mockGetNurseConsumables(encounterId: string) {
  return delay(copy(consumables[encounterId] ?? []))
}

/** Mock：提交打印任务。 */
export function mockPrintNurseDocuments(
  request: NursePrintRequestDTO
): Promise<NursePrintResultVO> {
  const count = request.documentIds.length
  return delay({
    success: count > 0,
    printer: '门诊护士站-01',
    message:
      count > 0
        ? `已提交 ${count} 张${printDocumentLabels[request.documentType]}`
        : '没有可打印的项目',
    documentType: request.documentType,
    count
  })
}

/** Mock：分页查询患者历史执行记录（跨就诊）。 */
export async function mockPagePatientExecutionHistory(
  patientId: string,
  query: NurseExecutionHistoryQuery
): Promise<PageDTO<NurseExecutionRecordVO>> {
  // 查找该患者的所有就诊及其执行项目
  const patientEncounters = patients.filter((p) => p.patientId === patientId)
  const allRecords: NurseExecutionRecordVO[] = []

  patientEncounters.forEach((encounter) => {
    const items = executionItems[encounter.encounterId] ?? []
    items.forEach((item) => {
      // 只取已执行过的记录
      if (item.executedCount <= 0 || !item.executedAt) return
      allRecords.push({
        ...copy(item),
        encounterId: encounter.encounterId,
        visitNo: encounter.visitNo
      })
    })
  })

  // 过滤
  let filtered = allRecords
  if (query.startDate) {
    filtered = filtered.filter((r) => (r.executedAt ?? '') >= query.startDate!)
  }
  if (query.endDate) {
    filtered = filtered.filter((r) => (r.executedAt ?? '').slice(0, 10) <= query.endDate!)
  }
  if (query.status) {
    filtered = filtered.filter((r) => r.status === query.status)
  }

  // 按执行时间倒序
  filtered.sort((a, b) => String(b.executedAt).localeCompare(String(a.executedAt)))

  // 分页
  const start = Math.max(0, (query.pageIndex - 1) * query.pageSize)
  const page = createPageDTO<NurseExecutionRecordVO>()
  page.pageIndex = query.pageIndex
  page.pageSize = query.pageSize
  page.total = filtered.length
  page.pages = Math.ceil(filtered.length / query.pageSize)
  page.rows = filtered.slice(start, start + query.pageSize)

  return delay(page)
}
