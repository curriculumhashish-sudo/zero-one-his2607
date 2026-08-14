import { createPageDTO, type PageDTO } from '../../type'
import dayjs from 'dayjs'
import type {
  CancelNurseExecutionDTO,
  NurseConsumableItemVO,
  NurseExecutionHistoryQuery,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NurseBatchResultVO,
  NursePatientQueueItemVO,
  NursePatientQueueQuery,
  PrintNurseExecutionsDTO,
  PerformNurseExecutionsDTO
} from './type'

const MOCK_DELAY = 180
const MOCK_TODAY = dayjs().format('YYYY-MM-DD')
const MOCK_HISTORY_DATE = dayjs().subtract(7, 'day').format('YYYY-MM-DD')

const FULL_DEMO_PATIENT_ID = 'mock-patient-001'
const FULL_DEMO_ENCOUNTER_ID = 'mock-encounter-001'
const FULL_DEMO_HISTORY_ENCOUNTER_ID = 'mock-encounter-001-history'
const EMPTY_DEMO_PATIENT_ID = 'mock-patient-002'
const EMPTY_DEMO_ENCOUNTER_ID = 'mock-encounter-002'

/**
 * 两位体验患者：
 * - 林安然：覆盖批量执行、取消执行、耗材真实字段及历史记录分页。
 * - 周远航：覆盖无耗材、无历史记录，以及执行后从空状态产生记录。
 */
const patients: NursePatientQueueItemVO[] = [
  {
    encounterId: FULL_DEMO_ENCOUNTER_ID,
    patientId: FULL_DEMO_PATIENT_ID,
    patientName: '林安然',
    genderName: '女',
    ageText: '32岁',
    bookingDate: MOCK_TODAY,
    departmentName: '内科门诊',
    visitTypeName: '普通门诊',
    status: 'READY',
    statusName: '待执行',
    priorityName: '全功能演示'
  },
  {
    encounterId: FULL_DEMO_HISTORY_ENCOUNTER_ID,
    patientId: FULL_DEMO_PATIENT_ID,
    patientName: '林安然',
    genderName: '女',
    ageText: '32岁',
    bookingDate: MOCK_HISTORY_DATE,
    departmentName: '内科门诊',
    visitTypeName: '普通门诊',
    status: 'COMPLETED',
    statusName: '已完成'
  },
  {
    encounterId: EMPTY_DEMO_ENCOUNTER_ID,
    patientId: EMPTY_DEMO_PATIENT_ID,
    patientName: '周远航',
    genderName: '男',
    ageText: '46岁',
    bookingDate: MOCK_TODAY,
    departmentName: '外科门诊',
    visitTypeName: '普通门诊',
    status: 'WAITING',
    statusName: '候诊',
    priorityName: '空状态演示'
  }
]

const historyItemNames = [
  '静脉采血',
  '血压测量',
  '血糖监测',
  '皮下注射',
  '心电图检查准备',
  '雾化吸入治疗',
  '伤口换药',
  '输液巡视',
  '标本送检',
  '过敏试验观察',
  '健康宣教'
]

const historicalExecutionItems: NurseExecutionItemVO[] = historyItemNames.map(
  (itemName, index) => ({
    itemId: `mock-exec-history-${String(index + 1).padStart(2, '0')}`,
    encounterId: FULL_DEMO_HISTORY_ENCOUNTER_ID,
    categoryCode: 'SERVICE',
    categoryName: index === 8 ? '检验' : index === 10 ? '护理' : '处置',
    itemName,
    quantity: 1,
    unitName: '次',
    status: 'COMPLETED',
    statusName: '已完成',
    executionCount: 1,
    executorName: index % 2 === 0 ? '护士李' : '护士王',
    executedAt: dayjs(`${MOCK_HISTORY_DATE} 08:20`)
      .add(index * 8, 'minute')
      .format('YYYY-MM-DD HH:mm')
  })
)

const executionItems: Record<string, NurseExecutionItemVO[]> = {
  [FULL_DEMO_HISTORY_ENCOUNTER_ID]: historicalExecutionItems,
  [FULL_DEMO_ENCOUNTER_ID]: [
    {
      itemId: 'mock-exec-pending-001',
      encounterId: FULL_DEMO_ENCOUNTER_ID,
      categoryCode: 'SERVICE',
      categoryName: '处置',
      itemName: '静脉输液',
      quantity: 250,
      unitName: 'ml',
      status: 'PENDING',
      statusName: '待执行',
      executionCount: 0
    },
    {
      itemId: 'mock-exec-pending-002',
      encounterId: FULL_DEMO_ENCOUNTER_ID,
      categoryCode: 'SERVICE',
      categoryName: '治疗',
      itemName: '雾化吸入治疗',
      quantity: 1,
      unitName: '次',
      status: 'PENDING',
      statusName: '待执行',
      executionCount: 0,
      remark: '体验批量勾选后执行'
    },
    {
      itemId: 'mock-exec-completed-001',
      encounterId: FULL_DEMO_ENCOUNTER_ID,
      categoryCode: 'SERVICE',
      categoryName: '护理',
      itemName: '生命体征测量',
      quantity: 1,
      unitName: '次',
      status: 'COMPLETED',
      statusName: '已完成',
      executionCount: 1,
      executorName: '护士李',
      executedAt: `${MOCK_TODAY} 08:50`,
      remark: '可体验取消执行'
    },
    {
      itemId: 'mock-exec-cancelled-001',
      encounterId: FULL_DEMO_ENCOUNTER_ID,
      categoryCode: 'SERVICE',
      categoryName: '处置',
      itemName: '肌内注射',
      quantity: 1,
      unitName: '次',
      status: 'CANCELLED',
      statusName: '已取消',
      executionCount: 0,
      remark: '展示取消状态'
    }
  ],
  [EMPTY_DEMO_ENCOUNTER_ID]: [
    {
      itemId: 'mock-exec-empty-001',
      encounterId: EMPTY_DEMO_ENCOUNTER_ID,
      categoryCode: 'SERVICE',
      categoryName: '处置',
      itemName: '伤口换药',
      quantity: 1,
      unitName: '次',
      status: 'PENDING',
      statusName: '待执行',
      executionCount: 0,
      remark: '执行后可在执行记录标签中查看'
    }
  ]
}

const consumables: Record<string, NurseConsumableItemVO[]> = {
  [FULL_DEMO_ENCOUNTER_ID]: [
    {
      deviceDefId: 'MOCK-DEVICE-INFUSION-001',
      quantity: 1,
      unitCode: '套',
      usageInstruction: '静脉输液使用',
      lotNumber: 'MOCK-LOT-001',
      statusEnum: 'PENDING',
      supportInfo: '一次性使用输液器'
    },
    {
      deviceDefId: 'MOCK-DEVICE-DRESSING-002',
      quantity: 2,
      unitCode: '片',
      usageInstruction: '10cm × 10cm，外用',
      lotNumber: 'MOCK-LOT-002',
      statusEnum: 'DISPENSED',
      dispenseTime: `${MOCK_TODAY} 08:55`
    }
  ],
  [EMPTY_DEMO_ENCOUNTER_ID]: []
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
  const filtered = patients.filter(
    (patient) => !query.bookingDate || patient.bookingDate === query.bookingDate
  )
  const start = Math.max(0, (query.pageIndex - 1) * query.pageSize)
  const page = createPageDTO<NursePatientQueueItemVO>()
  page.pageIndex = query.pageIndex
  page.pageSize = query.pageSize
  page.total = filtered.length
  page.pages = Math.ceil(filtered.length / query.pageSize)
  page.rows = copy(filtered.slice(start, start + query.pageSize))
  return delay(page)
}

/** Mock：查询当前就诊的执行项目，签名与真实接口保持一致。 */
export function mockGetNurseExecutionItems(_patientId: string, encounterId: string) {
  return delay(copy(executionItems[encounterId] ?? []))
}

function formatMockDateTime() {
  return dayjs().format('YYYY-MM-DD HH:mm:ss')
}

/** Mock：批量执行选中的护士项目。 */
export function mockPerformNurseExecutions(
  request: PerformNurseExecutionsDTO
): Promise<NurseBatchResultVO> {
  const rows = executionItems[request.encounterId] ?? []
  const result: NurseBatchResultVO = {
    totalCount: request.items.length,
    successCount: 0,
    failedCount: 0,
    successIds: [],
    failedItems: []
  }

  request.items.forEach((command) => {
    const row = rows.find((item) => item.itemId === command.itemId)
    if (!row) {
      result.failedItems.push({
        recordId: command.itemId,
        errorCode: 'ITEM_NOT_FOUND',
        reason: '执行项目不存在'
      })
      return
    }
    if (row.status !== 'PENDING') {
      result.failedItems.push({
        recordId: command.itemId,
        errorCode: 'INVALID_STATUS',
        reason: '当前状态不可执行'
      })
      return
    }

    row.executionCount += 1
    row.status = 'COMPLETED'
    row.statusName = row.status === 'COMPLETED' ? '已完成' : '待执行'
    row.executorName = '模拟护士'
    row.executedAt = formatMockDateTime()
    result.successIds.push(row.itemId)
  })

  result.successCount = result.successIds.length
  result.failedCount = result.failedItems.length
  return delay(result)
}

/** Mock：取消项目最近一次执行。 */
export function mockCancelNurseExecution(
  request: CancelNurseExecutionDTO
): Promise<NurseBatchResultVO> {
  const rows = executionItems[request.encounterId] ?? []
  const row = rows.find((item) => item.itemId === request.item.itemId)
  const result: NurseBatchResultVO = {
    totalCount: 1,
    successCount: 0,
    failedCount: 0,
    successIds: [],
    failedItems: []
  }

  if (!row) {
    result.failedItems.push({
      recordId: request.item.itemId,
      errorCode: 'ITEM_NOT_FOUND',
      reason: '执行项目不存在'
    })
  } else if (row.executionCount <= 0 || row.status === 'CANCELLED') {
    result.failedItems.push({
      recordId: request.item.itemId,
      errorCode: 'INVALID_STATUS',
      reason: '当前项目没有可取消的执行记录'
    })
  } else {
    row.executionCount -= 1
    row.status = 'PENDING'
    row.statusName = '待执行'
    row.executorName = undefined
    row.executedAt = undefined
    result.successIds.push(row.itemId)
  }

  result.successCount = result.successIds.length
  result.failedCount = result.failedItems.length
  return delay(result)
}

/** Mock：生成与真实接口一致的纯文本打印内容。 */
export async function mockPrintNurseExecutions(request: PrintNurseExecutionsDTO) {
  const allItems = Object.values(executionItems).flat()
  const records = request.recordIds
    .map((recordId) => allItems.find((item) => item.itemId === recordId))
    .filter((item): item is NurseExecutionItemVO => Boolean(item))

  if (records.length === 0) throw new Error('未找到有效的服务请求记录')

  const lines = [
    '========================================',
    '         服务请求打印单',
    '========================================',
    '打印类型：执行记录',
    `打印时间：${formatMockDateTime()}`,
    '----------------------------------------'
  ]
  records.forEach((record, index) => {
    lines.push(
      '',
      `${index + 1}. 记录ID：${record.itemId}`,
      `   就诊ID：${record.encounterId}`,
      `   项目名称：${record.itemName}`,
      `   数量：${record.quantity ?? '—'} ${record.unitName ?? ''}`.trimEnd(),
      `   状态：${record.statusName}`,
      `   执行者：${record.executorName ?? '未指定'}`,
      `   备注：${record.remark ?? '无'}`
    )
  })
  lines.push('', '========================================')
  return delay(lines.join('\n'))
}

/** Mock：查询当前就诊的耗材项目，签名与真实接口保持一致。 */
export function mockGetNurseConsumables(_patientId: string, encounterId: string) {
  return delay(copy(consumables[encounterId] ?? []))
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
      if (item.executionCount <= 0 || !item.executedAt) return
      allRecords.push({
        ...copy(item),
        encounterId: encounter.encounterId
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
