import dayjs from 'dayjs'

import { mockFindNurseStationPatient, mockUpdateNurseStationPatient } from './mock'
import type {
  NurseStationCancelExecutionsDTO,
  NurseStationExecuteOrdersDTO,
  NurseStationOrderExecuteBatchResultVO,
  NurseStationOrderExecuteQueryDTO,
  NurseStationOrderExecuteRowVO,
  NurseStationOrderExecuteSlotVO,
  NurseStationOrderExecuteWriteItemDTO,
  NurseStationOrderExecuteWriteResultVO,
  NurseStationOrderExecuteStatus,
  NurseStationSkipOrdersDTO
} from './order-execute-type'

/**
 * Mock 内部保存一份包含全部状态计划点的医嘱。
 * 页面接口每次只返回当前状态的计划点，这个内部结构用来模拟数据库中的状态迁移。
 */
type MockExecuteOrder = Omit<NurseStationOrderExecuteRowVO, 'slots'> & {
  allSlots: NurseStationOrderExecuteSlotVO[]
}

type MockOrderInput = Omit<
  MockExecuteOrder,
  'allSlots' | 'bindItems' | 'listRowKey' | 'statusDisplay' | 'statusEnum'
> & {
  allSlots: NurseStationOrderExecuteSlotVO[]
  bindItems?: NurseStationOrderExecuteRowVO['bindItems']
}

function createOrder(input: MockOrderInput): MockExecuteOrder {
  return {
    ...input,
    listRowKey: `${input.orderType}:${input.orderId}`,
    statusEnum: 'VERIFIED',
    statusDisplay: '已校对',
    bindItems: input.bindItems ?? []
  }
}

/**
 * 医嘱执行 Mock 数据与患者队列使用相同 encounterId。
 * 因此执行成功后可以同步修改左侧患者的待执行数量，模拟真实后端读模型刷新。
 */
const executeOrders: MockExecuteOrder[] = [
  createOrder({
    orderId: 'execute-med-001',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-001',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '头孢曲松钠',
    specificationText: '1g/支',
    dosageDisplay: '2g',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '每日两次',
    authoredTime: '2026-07-31 08:30:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    medicineCategory: 'WEST',
    allSlots: [
      { planTime: '2026-08-01 08:00:00', status: 'PENDING' },
      { planTime: '2026-08-01 20:00:00', status: 'PENDING' }
    ],
    bindItems: [
      {
        bindingId: 'binding-needle-001',
        bindType: 'DEVICE',
        chargeItemDefinitionId: 'charge-needle-001',
        displayName: '一次性使用输液器',
        specificationText: '精密过滤型',
        unitPrice: 6.8,
        quantity: 1,
        unitText: '套',
        priority: 1
      }
    ]
  }),
  createOrder({
    orderId: 'execute-service-002',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-001',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '氧气吸入',
    dosageDisplay: '2L/min',
    usageDisplay: '鼻导管吸氧',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 09:10:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    medicineCategory: 'WEST',
    allSlots: [{ planTime: '2026-08-01 10:00:00', status: 'PENDING' }],
    bindItems: [
      {
        bindingId: 'binding-oxygen-001',
        bindType: 'ACTIVITY',
        chargeItemDefinitionId: 'charge-oxygen-001',
        displayName: '氧气吸入费',
        unitPrice: 3,
        quantity: 1,
        unitText: '小时',
        priority: 1
      }
    ]
  }),
  createOrder({
    orderId: 'execute-med-history-001',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-001',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '奥美拉唑肠溶胶囊',
    specificationText: '20mg×14粒',
    dosageDisplay: '20mg',
    usageDisplay: '口服',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-07-30 15:20:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    medicineCategory: 'WEST',
    allSlots: [
      {
        planTime: '2026-08-01 07:00:00',
        procedureId: 'procedure-executed-001',
        status: 'EXECUTED',
        recordedTime: '2026-08-01 07:03:00'
      },
      {
        planTime: '2026-07-31 07:00:00',
        status: 'SKIPPED',
        recordedTime: '2026-07-31 07:05:00'
      },
      {
        planTime: '2026-07-30 07:00:00',
        procedureId: 'procedure-cancelled-001',
        status: 'CANCELLED',
        recordedTime: '2026-07-30 08:00:00'
      }
    ]
  }),
  createOrder({
    orderId: 'execute-med-004',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-004',
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
    medicineCategory: 'WEST',
    allSlots: [
      { planTime: '2026-08-01 09:00:00', status: 'PENDING' },
      { planTime: '2026-08-01 21:00:00', status: 'PENDING' }
    ]
  }),
  createOrder({
    orderId: 'execute-tcm-004',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-004',
    therapyEnum: 'TEMPORARY',
    therapyDisplay: '临时',
    adviceDisplayName: '术后益气活血方（黄芪、当归等 8 味）',
    dosageDisplay: '每味单次量',
    usageDisplay: '水煎服',
    frequencyDisplay: '每日一剂',
    authoredTime: '2026-08-01 09:00:00',
    requesterName: '王医生',
    performerLocationDisplay: '中药房',
    medicineCategory: 'TCM',
    tcmFlag: true,
    prescriptionNo: 'TCM20260801001',
    lineRequestIds: ['execute-tcm-line-001', 'execute-tcm-line-002'],
    chineseHerbsDoseQuantity: 3,
    allSlots: [{ planTime: '2026-08-01 18:00:00', status: 'PENDING' }]
  }),
  createOrder({
    orderId: 'execute-service-006',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-006',
    therapyEnum: 'LONG_TERM',
    therapyDisplay: '长期',
    adviceDisplayName: '静脉输液治疗',
    dosageDisplay: '1次',
    usageDisplay: '静脉输液',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-08-01 08:42:00',
    requesterName: '周医生',
    performerLocationDisplay: '外科疗区',
    medicineCategory: 'WEST',
    allSlots: [{ planTime: '2026-08-01 11:00:00', status: 'PENDING' }]
  }),
  createOrder({
    orderId: 'execute-med-007',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-007',
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
    medicineCategory: 'WEST',
    allSlots: [
      { planTime: '2026-08-01 08:30:00', status: 'PENDING' },
      { planTime: '2026-08-01 14:30:00', status: 'PENDING' },
      { planTime: '2026-08-01 20:30:00', status: 'PENDING' }
    ]
  })
]

const idempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationOrderExecuteBatchResultVO }
>()

function cloneBindItems(items: NurseStationOrderExecuteRowVO['bindItems']) {
  return items.map((item) => ({ ...item }))
}

function cloneBatchResult(result: NurseStationOrderExecuteBatchResultVO) {
  return {
    ...result,
    rebuiltSummaryNos: result.rebuiltSummaryNos ? [...result.rebuiltSummaryNos] : undefined,
    results: result.results.map((item) => ({ ...item }))
  }
}

/** Mock：按患者、状态、长临、中西药和截止时间返回稳定分组。 */
export async function mockQueryNurseStationOrderExecutions(
  query: NurseStationOrderExecuteQueryDTO
) {
  await Promise.resolve()
  const encounterIds = [...new Set(query.encounterIds.map((id) => id.trim()).filter(Boolean))]
  const deadline = dayjs(query.executionDeadline)
  if (!deadline.isValid()) throw new Error('执行截止时间格式不正确')

  return {
    groups: encounterIds.map((encounterId) => ({
      encounterId,
      orders: executeOrders
        .filter((order) => {
          if (order.encounterId !== encounterId) return false
          if (query.therapyType !== 'ALL' && order.therapyEnum !== query.therapyType) return false
          return order.medicineCategory === query.medicineCategory
        })
        .map((order): NurseStationOrderExecuteRowVO | undefined => {
          const slots = order.allSlots
            .filter(
              (slot) =>
                slot.status === query.executeStatus && !dayjs(slot.planTime).isAfter(deadline)
            )
            .sort((left, right) => left.planTime.localeCompare(right.planTime))
            .map((slot) => ({ ...slot }))
          if (!slots.length) return undefined
          const { allSlots: _allSlots, ...row } = order
          return {
            ...row,
            slots,
            bindItems: cloneBindItems(order.bindItems),
            lineRequestIds: order.lineRequestIds ? [...order.lineRequestIds] : undefined
          }
        })
        .filter((order): order is NurseStationOrderExecuteRowVO => Boolean(order))
    }))
  }
}

function validateBatch(idempotencyKey: string, itemCount: number) {
  if (!idempotencyKey.trim()) throw new Error('幂等键不能为空')
  if (itemCount < 1 || itemCount > 50) throw new Error('单次只能处理 1～50 个计划点')
}

function findOrder(item: Pick<NurseStationOrderExecuteWriteItemDTO, 'orderId' | 'orderType'>) {
  return executeOrders.find(
    (order) => order.orderId === item.orderId && order.orderType === item.orderType
  )
}

function updatePendingCount(encounterId: string, delta: number) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient) return
  mockUpdateNurseStationPatient(encounterId, {
    pendingExecuteCount: Math.max(0, (patient.pendingExecuteCount ?? 0) + delta)
  })
}

function itemKey(item: NurseStationOrderExecuteWriteItemDTO) {
  return `${item.orderType}:${item.orderId}:${item.planTime}`
}

/**
 * 执行带幂等能力的 Mock 写操作。
 * 相同幂等键和相同请求会返回第一次结果；相同键携带不同内容则直接报错，避免误复用。
 */
async function runIdempotentWrite(
  idempotencyKey: string,
  payload: unknown,
  writer: () => NurseStationOrderExecuteBatchResultVO
) {
  await Promise.resolve()
  const fingerprint = JSON.stringify(payload)
  const cached = idempotentResults.get(idempotencyKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return cloneBatchResult(cached.result)
  }
  const result = writer()
  idempotentResults.set(idempotencyKey, { fingerprint, result: cloneBatchResult(result) })
  return cloneBatchResult(result)
}

/** Mock：把待执行或已取消计划点迁移到已执行状态。 */
export async function mockExecuteNurseStationOrders(data: NurseStationExecuteOrdersDTO) {
  validateBatch(data.idempotencyKey, data.items.length)
  return runIdempotentWrite(data.idempotencyKey, data, () => {
    const results: NurseStationOrderExecuteWriteResultVO[] = data.items.map((item, index) => {
      const order = findOrder(item)
      const slot = order?.allSlots.find(
        (candidate) => candidate.planTime === item.planTime && candidate.status !== 'EXECUTED'
      )
      const patient = mockFindNurseStationPatient(item.encounterId)
      if (!order || !slot || !['PENDING', 'CANCELLED'].includes(slot.status)) {
        return { itemKey: itemKey(item), success: false, message: '计划点状态已变化' }
      }
      if (!patient?.accountId || patient.accountId !== item.accountId) {
        return { itemKey: itemKey(item), success: false, message: '住院账户无效' }
      }
      if (!item.actualTime || !dayjs(item.actualTime).isValid()) {
        return { itemKey: itemKey(item), success: false, message: '实际执行时间无效' }
      }

      slot.status = 'EXECUTED'
      slot.recordedTime = item.actualTime
      slot.procedureId = `procedure-${Date.now()}-${index}`
      if (item.bindItems) order.bindItems = cloneBindItems(item.bindItems)
      updatePendingCount(item.encounterId, -1)
      return { itemKey: itemKey(item), success: true, message: '执行成功' }
    })
    const processedCount = results.filter((item) => item.success).length
    return { success: processedCount === results.length, processedCount, results }
  })
}

/** Mock：把待执行计划点迁移到不执行状态。 */
export async function mockSkipNurseStationOrders(data: NurseStationSkipOrdersDTO) {
  validateBatch(data.idempotencyKey, data.items.length)
  return runIdempotentWrite(data.idempotencyKey, data, () => {
    const recordedTime = dayjs().format('YYYY-MM-DD HH:mm:ss')
    const results = data.items.map((item): NurseStationOrderExecuteWriteResultVO => {
      const order = findOrder(item)
      const slot = order?.allSlots.find(
        (candidate) => candidate.planTime === item.planTime && candidate.status === 'PENDING'
      )
      if (!order || !slot) {
        return { itemKey: itemKey(item), success: false, message: '计划点已不在待执行状态' }
      }
      slot.status = 'SKIPPED'
      slot.recordedTime = recordedTime
      updatePendingCount(item.encounterId, -1)
      return { itemKey: itemKey(item), success: true, message: '已登记不执行' }
    })
    const processedCount = results.filter((item) => item.success).length
    return { success: processedCount === results.length, processedCount, results }
  })
}

/** Mock：按 procedureId 把已执行计划点迁移到取消执行状态。 */
export async function mockCancelNurseStationExecutions(data: NurseStationCancelExecutionsDTO) {
  validateBatch(data.idempotencyKey, data.items.length)
  if (!data.reason.trim()) throw new Error('取消原因不能为空')
  return runIdempotentWrite(data.idempotencyKey, data, () => {
    const recordedTime = dayjs().format('YYYY-MM-DD HH:mm:ss')
    const results = data.items.map((item): NurseStationOrderExecuteWriteResultVO => {
      // 同一患者通常有多条医嘱，必须在该患者的全部医嘱中查找 procedureId。
      const order = executeOrders.find(
        (candidate) =>
          candidate.encounterId === item.encounterId &&
          candidate.allSlots.some(
            (slot) => slot.procedureId === item.procedureId && slot.status === 'EXECUTED'
          )
      )
      const slot = order?.allSlots.find(
        (candidate) => candidate.procedureId === item.procedureId && candidate.status === 'EXECUTED'
      )
      if (!order || !slot) {
        return { itemKey: item.procedureId, success: false, message: '执行记录已不可取消' }
      }
      slot.status = 'CANCELLED'
      slot.recordedTime = recordedTime
      updatePendingCount(item.encounterId, 1)
      return { itemKey: item.procedureId, success: true, message: '取消执行成功' }
    })
    const processedCount = results.filter((item) => item.success).length
    return {
      success: processedCount === results.length,
      processedCount,
      rebuiltSummaryNos: processedCount ? [`SUMMARY-${dayjs().format('YYYYMMDDHHmmss')}`] : [],
      results
    }
  })
}

/** 仅供单元测试读取状态，不暴露给页面业务代码。 */
export function mockFindExecutionSlot(
  orderId: string,
  planTime: string,
  status?: NurseStationOrderExecuteStatus
) {
  const slot = executeOrders
    .find((order) => order.orderId === orderId)
    ?.allSlots.find((item) => item.planTime === planTime && (!status || item.status === status))
  return slot ? { ...slot } : undefined
}
