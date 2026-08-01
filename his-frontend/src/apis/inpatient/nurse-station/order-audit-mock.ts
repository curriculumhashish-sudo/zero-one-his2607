import { mockFindNurseStationPatient, mockUpdateNurseStationPatient } from './mock'
import type {
  NurseStationOrderAuditBatchResultVO,
  NurseStationOrderAuditQueryDTO,
  NurseStationOrderAuditRowVO,
  NurseStationOrderAuditStatus,
  NurseStationOrderAuditWriteResultVO,
  NurseStationRejectOrdersDTO,
  NurseStationVerifyOrdersDTO
} from './order-audit-type'

type MockOrderInput = Omit<
  NurseStationOrderAuditRowVO,
  'listRowKey' | 'patientId' | 'statusDisplay' | 'version'
> & {
  version?: number
}

const statusText: Record<NurseStationOrderAuditRowVO['statusEnum'], string> = {
  PASSED: '已校对',
  PENDING: '未校对',
  REJECTED: '已退回',
  STOPPED: '已停止',
  SUSPENDED: '已作废'
}

function createOrder(input: MockOrderInput): NurseStationOrderAuditRowVO {
  const patient = mockFindNurseStationPatient(input.encounterId)
  if (!patient) throw new Error(`医嘱 Mock 患者不存在：${input.encounterId}`)
  return {
    ...input,
    listRowKey: `${input.orderType}:${input.requestId}`,
    patientId: patient.patientId,
    statusDisplay: statusText[input.statusEnum],
    version: input.version ?? 1
  }
}

/** 医嘱和患者队列共享就诊 ID，写操作后可同步刷新左侧待校对数量。 */
const orderRows: NurseStationOrderAuditRowVO[] = [
  createOrder({
    requestId: 'audit-med-001',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-001',
    statusEnum: 'PENDING',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '头孢曲松钠',
    specificationText: '1g/支',
    dosageDisplay: '2g',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-08-01 08:30:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    categoryEnum: 'WEST',
    groupNo: 1,
    sortNumber: 1
  }),
  createOrder({
    requestId: 'audit-service-002',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-001',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '氧气吸入',
    dosageDisplay: '2L/min',
    usageDisplay: '鼻导管吸氧',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 09:10:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    categoryEnum: 'WEST',
    groupNo: 2,
    sortNumber: 1
  }),
  createOrder({
    requestId: 'audit-med-003',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-003',
    statusEnum: 'PENDING',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '门冬胰岛素注射液',
    specificationText: '3ml:300U',
    dosageDisplay: '6U',
    usageDisplay: '皮下注射',
    frequencyDisplay: '每日三次',
    authoredTime: '2026-08-01 07:45:00',
    requesterName: '刘医生',
    performerLocationDisplay: '内科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-service-004',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-004',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '腹部切口换药',
    dosageDisplay: '1次',
    usageDisplay: '外科换药',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 08:05:00',
    requesterName: '王医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-med-005',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-004',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '注射用头孢呋辛钠',
    specificationText: '0.75g/瓶',
    dosageDisplay: '1.5g',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '每日两次',
    authoredTime: '2026-08-01 08:15:00',
    requesterName: '王医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-tcm-006',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-004',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '术后益气活血方（黄芪、当归等 8 味）',
    dosageDisplay: '每味单次量',
    usageDisplay: '水煎服',
    frequencyDisplay: '每日一剂',
    authoredTime: '2026-08-01 09:00:00',
    requesterName: '王医生',
    performerLocationDisplay: '中药房',
    categoryEnum: 'TCM',
    tcmFlag: true,
    prescriptionNo: 'TCM20260801001',
    lineRequestIds: ['audit-tcm-line-001', 'audit-tcm-line-002', 'audit-tcm-line-003'],
    chineseHerbsDoseQuantity: 3
  }),
  createOrder({
    requestId: 'audit-service-007',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-004',
    statusEnum: 'PENDING',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: 'Ⅱ级护理',
    dosageDisplay: '1项',
    usageDisplay: '护理',
    frequencyDisplay: '持续',
    authoredTime: '2026-08-01 09:20:00',
    requesterName: '王医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-med-008',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-006',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '氯化钠注射液',
    specificationText: '500ml/袋',
    dosageDisplay: '500ml',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 08:40:00',
    requesterName: '周医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-service-009',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-006',
    statusEnum: 'PENDING',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '静脉输液治疗',
    dosageDisplay: '1次',
    usageDisplay: '静脉输液',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-08-01 08:42:00',
    requesterName: '周医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-med-010',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-006',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '布洛芬缓释胶囊',
    specificationText: '0.3g×20粒',
    dosageDisplay: '0.3g',
    usageDisplay: '口服',
    frequencyDisplay: '必要时',
    authoredTime: '2026-08-01 10:00:00',
    requesterName: '周医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-med-011',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-007',
    statusEnum: 'PENDING',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '盐酸氨溴索口服溶液',
    specificationText: '100ml/瓶',
    dosageDisplay: '5ml',
    usageDisplay: '口服',
    frequencyDisplay: '每日三次',
    authoredTime: '2026-08-01 08:20:00',
    requesterName: '何医生',
    performerLocationDisplay: '儿科疗区',
    categoryEnum: 'WEST'
  }),
  createOrder({
    requestId: 'audit-passed-001',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-001',
    statusEnum: 'PASSED',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '奥美拉唑肠溶胶囊',
    specificationText: '20mg×14粒',
    dosageDisplay: '20mg',
    usageDisplay: '口服',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-07-31 15:20:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    categoryEnum: 'WEST',
    version: 2
  }),
  createOrder({
    requestId: 'audit-stopped-001',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-006',
    statusEnum: 'STOPPED',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '复方氯化钠注射液',
    specificationText: '500ml/袋',
    dosageDisplay: '500ml',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-07-28 09:00:00',
    stopTime: '2026-08-01 07:30:00',
    requesterName: '周医生',
    performerLocationDisplay: '外科疗区',
    categoryEnum: 'WEST',
    version: 4
  }),
  createOrder({
    requestId: 'audit-suspended-001',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-004',
    statusEnum: 'SUSPENDED',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '床旁心电图',
    dosageDisplay: '1次',
    usageDisplay: '检查',
    frequencyDisplay: '立即',
    authoredTime: '2026-07-31 11:00:00',
    requesterName: '王医生',
    performerLocationDisplay: '心电图室',
    categoryEnum: 'WEST',
    version: 2
  })
]

const idempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationOrderAuditBatchResultVO }
>()

function cloneRow(row: NurseStationOrderAuditRowVO): NurseStationOrderAuditRowVO {
  return { ...row, lineRequestIds: row.lineRequestIds ? [...row.lineRequestIds] : undefined }
}

/** Mock：按患者和组合筛选返回分组医嘱。 */
export async function mockQueryNurseStationOrderAudits(query: NurseStationOrderAuditQueryDTO) {
  await Promise.resolve()
  const encounterIds = [...new Set(query.encounterIds.map((id) => id.trim()).filter(Boolean))]
  return {
    groups: encounterIds.map((encounterId) => ({
      encounterId,
      orders: orderRows
        .filter((row) => {
          if (row.encounterId !== encounterId || row.statusEnum !== query.auditStatus) return false
          if (query.therapyType !== 'ALL' && row.therapyEnum !== query.therapyType) return false
          return query.medicineCategory === 'TCM'
            ? row.categoryEnum === 'TCM'
            : row.categoryEnum !== 'TCM'
        })
        .sort((a, b) => (a.authoredTime ?? '').localeCompare(b.authoredTime ?? ''))
        .map(cloneRow)
    }))
  }
}

function cloneBatchResult(
  result: NurseStationOrderAuditBatchResultVO
): NurseStationOrderAuditBatchResultVO {
  return { ...result, results: result.results.map((item) => ({ ...item })) }
}

function decrementPendingAuditCount(encounterId: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient) return
  mockUpdateNurseStationPatient(encounterId, {
    pendingAuditCount: Math.max(0, (patient.pendingAuditCount ?? 0) - 1)
  })
}

async function executeWrite(
  action: 'REJECTED' | 'PASSED',
  data: NurseStationRejectOrdersDTO | NurseStationVerifyOrdersDTO
) {
  await Promise.resolve()
  if (!data.items.length) throw new Error('请至少选择一条医嘱')
  if (data.items.length > 50) throw new Error('单次最多处理 50 条医嘱')

  const fingerprint = JSON.stringify({
    action,
    items: data.items,
    reason: 'reason' in data ? data.reason : ''
  })
  const cacheKey = `${action}:${data.idempotencyKey}`
  const cached = idempotentResults.get(cacheKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('相同幂等键不能用于不同请求')
    return cloneBatchResult(cached.result)
  }

  const results: NurseStationOrderAuditWriteResultVO[] = data.items.map((item) => {
    const row = orderRows.find(
      (candidate) =>
        candidate.requestId === item.requestId && candidate.orderType === item.orderType
    )
    if (!row) return { ...item, success: false, message: '医嘱不存在' }
    if (row.statusEnum !== 'PENDING') {
      return { ...item, success: false, message: '医嘱状态已变化，请刷新后重试' }
    }
    if (row.version !== item.expectedVersion) {
      return { ...item, success: false, message: '医嘱版本已变化，请刷新后重试' }
    }

    row.statusEnum = action
    row.statusDisplay = statusText[action]
    row.version += 1
    decrementPendingAuditCount(row.encounterId)
    return {
      orderType: item.orderType,
      requestId: item.requestId,
      success: true,
      targetStatusEnum: action,
      message: action === 'PASSED' ? '校对成功' : '退回成功'
    }
  })
  const processedCount = results.filter((item) => item.success).length
  const result = {
    success: processedCount === results.length,
    processedCount,
    results
  }
  idempotentResults.set(cacheKey, { fingerprint, result: cloneBatchResult(result) })
  return result
}

/** Mock：批量校对通过。 */
export function mockVerifyNurseStationOrders(data: NurseStationVerifyOrdersDTO) {
  return executeWrite('PASSED', data)
}

/** Mock：批量退回医嘱。 */
export function mockRejectNurseStationOrders(data: NurseStationRejectOrdersDTO) {
  return executeWrite('REJECTED', data)
}

/** 测试辅助：按请求 ID 查看共享 Mock 医嘱。 */
export function mockFindNurseStationOrder(requestId: string) {
  return orderRows.find((row) => row.requestId === requestId)
}
