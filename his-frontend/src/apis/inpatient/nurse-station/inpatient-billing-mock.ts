import dayjs from 'dayjs'

import { createPageDTO } from '../../type'
import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationAutoBillingDataVO,
  NurseStationAutoBillingMutationResultVO,
  NurseStationAutoBillingQueryDTO,
  NurseStationAutoBillingRuleVO,
  NurseStationBillingCatalogItemVO,
  NurseStationBillingCatalogQueryDTO,
  NurseStationBillingWriteResultVO,
  NurseStationCancelBillingDTO,
  NurseStationCancelBillingResultVO,
  NurseStationConfirmBillingDTO,
  NurseStationDeleteAutoBillingRuleDTO,
  NurseStationPricedOrderQueryDTO,
  NurseStationPricedOrderVO,
  NurseStationSaveAutoBillingRuleDTO,
  NurseStationSetAutoBillingRuleStatusDTO,
  NurseStationSupplementOrderQueryDTO,
  NurseStationSupplementOrderVO
} from './inpatient-billing-type'

const catalogItems: NurseStationBillingCatalogItemVO[] = [
  {
    productTableName: 'device_definition',
    productDefinitionId: 'device-infusion-set',
    chargeItemDefinitionId: 'charge-infusion-set',
    busNo: 'HC000101',
    name: '一次性使用输液器',
    category: 'DEVICE',
    categoryText: '卫生材料',
    specificationText: '带针 0.7mm',
    unitCode: 'SET',
    unitName: '套',
    unitBands: [{ unitCode: 'SET', unitName: '套', ratioToSplit: 1 }],
    unitPrice: 6.8,
    defaultDispenseLocationId: 'warehouse-inpatient',
    defaultDispenseLocationName: '住院耗材库'
  },
  {
    productTableName: 'device_definition',
    productDefinitionId: 'device-syringe-20',
    chargeItemDefinitionId: 'charge-syringe-20',
    busNo: 'HC000205',
    name: '一次性使用无菌注射器',
    category: 'DEVICE',
    categoryText: '卫生材料',
    specificationText: '20ml',
    unitCode: 'PCS',
    unitName: '支',
    unitBands: [{ unitCode: 'PCS', unitName: '支', ratioToSplit: 1 }],
    unitPrice: 1.5,
    defaultDispenseLocationId: 'warehouse-inpatient',
    defaultDispenseLocationName: '住院耗材库'
  },
  {
    productTableName: 'device_definition',
    productDefinitionId: 'device-dressing',
    chargeItemDefinitionId: 'charge-dressing',
    busNo: 'HC000318',
    name: '医用无菌敷贴',
    category: 'DEVICE',
    categoryText: '卫生材料',
    specificationText: '10cm×10cm',
    unitCode: 'PCS',
    unitName: '片',
    unitBands: [{ unitCode: 'PCS', unitName: '片', ratioToSplit: 1 }],
    unitPrice: 4.2,
    defaultDispenseLocationId: 'warehouse-inpatient',
    defaultDispenseLocationName: '住院耗材库'
  },
  {
    productTableName: 'healthcare_service',
    productDefinitionId: 'service-oxygen',
    chargeItemDefinitionId: 'charge-oxygen',
    busNo: 'ZL010101',
    name: '氧气吸入',
    category: 'SERVICE',
    categoryText: '治疗费',
    unitCode: 'HOUR',
    unitName: '小时',
    unitBands: [{ unitCode: 'HOUR', unitName: '小时', ratioToSplit: 1 }],
    unitPrice: 5
  },
  {
    productTableName: 'healthcare_service',
    productDefinitionId: 'service-nebulization',
    chargeItemDefinitionId: 'charge-nebulization',
    busNo: 'ZL010208',
    name: '雾化吸入治疗',
    category: 'SERVICE',
    categoryText: '治疗费',
    unitCode: 'TIME',
    unitName: '次',
    unitBands: [{ unitCode: 'TIME', unitName: '次', ratioToSplit: 1 }],
    unitPrice: 12
  },
  {
    productTableName: 'healthcare_service',
    productDefinitionId: 'service-nursing',
    chargeItemDefinitionId: 'charge-nursing',
    busNo: 'HL020301',
    name: 'Ⅱ级护理',
    category: 'SERVICE',
    categoryText: '护理费',
    unitCode: 'DAY',
    unitName: '日',
    unitBands: [{ unitCode: 'DAY', unitName: '日', ratioToSplit: 1 }],
    unitPrice: 18
  }
]

const supplementOrders: NurseStationSupplementOrderVO[] = [
  {
    requestId: 'order-001-antibiotic',
    orderType: 'MED_REQUEST',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    therapyType: 'LONG',
    therapyTypeText: '长期',
    adviceDisplayName: '头孢曲松钠 2g 静脉滴注',
    specificationText: '2g/瓶',
    dosageDisplay: '2g',
    usageDisplay: '静脉滴注',
    frequencyDisplay: '每日一次',
    authoredTime: '2026-07-29 09:10:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    statusText: '已校对',
    chargeItems: [
      {
        requestId: 'billing-supplement-001',
        productName: '一次性使用输液器',
        quantity: 1,
        unitName: '套',
        unitPrice: 6.8,
        totalPrice: 6.8,
        pricedTime: '2026-08-01 08:45:00',
        chargeStatusText: '待收费'
      }
    ]
  },
  {
    requestId: 'order-001-nebulization',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    therapyType: 'TEMP',
    therapyTypeText: '临时',
    adviceDisplayName: '雾化吸入治疗',
    dosageDisplay: '1次',
    usageDisplay: '雾化吸入',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 10:20:00',
    requesterName: '张医生',
    performerLocationDisplay: '内科疗区',
    statusText: '已校对',
    chargeItems: []
  },
  {
    requestId: 'order-004-dressing',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-004',
    patientId: 'patient-004',
    therapyType: 'TEMP',
    therapyTypeText: '临时',
    adviceDisplayName: '切口换药',
    frequencyDisplay: '立即',
    authoredTime: '2026-08-01 09:00:00',
    requesterName: '刘医生',
    performerLocationDisplay: '外科疗区',
    statusText: '已停止',
    chargeItems: []
  }
]

const pricedOrders: NurseStationPricedOrderVO[] = [
  {
    requestId: 'billing-direct-001',
    orderType: 'DEVICE_REQUEST',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    productDefinitionId: 'device-syringe-20',
    productName: '一次性使用无菌注射器',
    quantity: 2,
    unitCode: 'PCS',
    unitName: '支',
    unitPrice: 1.5,
    totalPrice: 3,
    pricedTime: '2026-08-01 08:32:00',
    pricingMode: 'DIRECT',
    pricingModeText: '直接划价',
    chargeStatus: 'PENDING',
    chargeStatusText: '待收费',
    dispenseStatusText: '未发放',
    cancellable: true
  },
  {
    requestId: 'billing-supplement-001',
    orderType: 'DEVICE_REQUEST',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    productDefinitionId: 'device-infusion-set',
    productName: '一次性使用输液器',
    quantity: 1,
    unitCode: 'SET',
    unitName: '套',
    unitPrice: 6.8,
    totalPrice: 6.8,
    pricedTime: '2026-08-01 08:45:00',
    pricingMode: 'SUPPLEMENT',
    pricingModeText: '医嘱补费',
    parentOrderAnchor: { orderType: 'MED_REQUEST', requestId: 'order-001-antibiotic' },
    chargeStatus: 'PENDING',
    chargeStatusText: '待收费',
    dispenseStatusText: '未发放',
    cancellable: true
  },
  {
    requestId: 'billing-charged-001',
    orderType: 'SERVICE_REQUEST',
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    productDefinitionId: 'service-oxygen',
    productName: '氧气吸入',
    quantity: 2,
    unitCode: 'HOUR',
    unitName: '小时',
    unitPrice: 5,
    totalPrice: 10,
    pricedTime: '2026-07-31 14:20:00',
    pricingMode: 'DIRECT',
    pricingModeText: '直接划价',
    chargeStatus: 'CHARGED',
    chargeStatusText: '已收费',
    cancellable: false,
    cancelReasonDisabled: '已收费项目需要走退费流程'
  }
]

const autoBillingRules: NurseStationAutoBillingRuleVO[] = [
  {
    ruleId: 'auto-rule-001',
    version: 1,
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    ruleName: '每日氧气吸入计费',
    status: 'ACTIVE',
    statusText: '启用',
    activityId: 'activity-oxygen',
    activityName: '氧气吸入',
    chargeDefinitionId: 'charge-oxygen',
    chargeDefinitionName: '氧气吸入费',
    quantity: 2,
    performingOrgId: 'dept-internal',
    performingOrgName: '住院内科',
    priority: 10,
    remark: '每天执行一次',
    updatedAt: '2026-08-01 08:00:00'
  }
]

let billingRequestSeed = 200
let autoRuleSeed = 20

const billingIdempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationBillingWriteResultVO }
>()
const cancelIdempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationCancelBillingResultVO }
>()
const autoMutationIdempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationAutoBillingMutationResultVO }
>()

function clonePricedOrder(row: NurseStationPricedOrderVO): NurseStationPricedOrderVO {
  return {
    ...row,
    parentOrderAnchor: row.parentOrderAnchor ? { ...row.parentOrderAnchor } : undefined
  }
}

function cloneRule(rule: NurseStationAutoBillingRuleVO): NurseStationAutoBillingRuleVO {
  return { ...rule }
}

function cloneMutationResult(
  result: NurseStationAutoBillingMutationResultVO
): NurseStationAutoBillingMutationResultVO {
  return { ...result, rule: result.rule ? cloneRule(result.rule) : undefined }
}

function assertPatient(encounterId: string, patientId: string, accountId?: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient || patient.patientId !== patientId) throw new Error('患者与住院记录不匹配')
  if (accountId && patient.accountId !== accountId) throw new Error('住院账户与患者不匹配')
  if (!patient.accountId) throw new Error('患者缺少有效住院账户')
  return patient
}

function assertIdempotencyKey(key: string) {
  if (!key.trim()) throw new Error('幂等键不能为空')
}

/** Mock：分页查询诊疗项目或耗材目录。 */
export async function mockQueryNurseStationBillingCatalog(
  query: NurseStationBillingCatalogQueryDTO
) {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  const keyword = query.keyword?.trim().toLowerCase()
  const filtered = catalogItems.filter(
    (item) =>
      item.category === query.category &&
      (!keyword ||
        item.name.toLowerCase().includes(keyword) ||
        item.busNo.toLowerCase().includes(keyword))
  )
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const start = (pageIndex - 1) * pageSize
  return createPageDTO({
    pageIndex,
    pageSize,
    total: filtered.length,
    rows: filtered.slice(start, start + pageSize).map((item) => ({
      ...item,
      unitBands: item.unitBands.map((unit) => ({ ...unit }))
    }))
  })
}

/** Mock：查询所选患者可以补费的父医嘱和既有补费明细。 */
export async function mockQueryNurseStationSupplementOrders(
  query: NurseStationSupplementOrderQueryDTO
) {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  const keyword = query.keyword?.trim().toLowerCase()
  return supplementOrders
    .filter(
      (order) =>
        order.encounterId === query.encounterId &&
        (!query.therapyType ||
          query.therapyType === 'ALL' ||
          order.therapyType === query.therapyType) &&
        (!keyword || order.adviceDisplayName.toLowerCase().includes(keyword))
    )
    .map((order) => ({ ...order, chargeItems: order.chargeItems.map((item) => ({ ...item })) }))
}

/** Mock：查询直接划价和医嘱补费产生的统一明细。 */
export async function mockQueryNurseStationPricedOrders(query: NurseStationPricedOrderQueryDTO) {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  return pricedOrders
    .filter(
      (row) =>
        row.encounterId === query.encounterId &&
        (!query.pricedTimeFrom || row.pricedTime >= query.pricedTimeFrom) &&
        (!query.pricedTimeTo || row.pricedTime <= query.pricedTimeTo) &&
        (!query.parentRequestId || row.parentOrderAnchor?.requestId === query.parentRequestId)
    )
    .map(clonePricedOrder)
}

/** Mock：处理直接划价或医嘱补费，并返回逐项结果。 */
export async function mockConfirmNurseStationBilling(data: NurseStationConfirmBillingDTO) {
  await Promise.resolve()
  assertIdempotencyKey(data.idempotencyKey)
  if (!data.items.length) throw new Error('请选择至少一个收费项目')
  if (data.items.length > 50) throw new Error('单批最多处理 50 个收费项目')

  const fingerprint = JSON.stringify(data)
  const cacheKey = `${data.mode}:${data.idempotencyKey}`
  const cached = billingIdempotentResults.get(cacheKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return structuredClone(cached.result)
  }

  const results = data.items.map((item) => {
    try {
      assertPatient(item.encounterId, item.patientId, item.accountId)
      const catalog = catalogItems.find(
        (candidate) => candidate.productDefinitionId === item.productDefinitionId
      )
      if (!catalog) throw new Error('收费项目已失效')
      if (!Number.isFinite(item.quantity) || item.quantity <= 0) throw new Error('数量必须大于 0')
      if (data.mode === 'DIRECT' && item.parentOrderAnchor)
        throw new Error('直接划价不能关联父医嘱')
      if (data.mode === 'SUPPLEMENT' && !item.parentOrderAnchor) {
        throw new Error('医嘱补费必须关联父医嘱')
      }
      if (
        item.parentOrderAnchor &&
        !supplementOrders.some(
          (order) =>
            order.requestId === item.parentOrderAnchor?.requestId &&
            order.orderType === item.parentOrderAnchor.orderType &&
            order.encounterId === item.encounterId
        )
      ) {
        throw new Error('父医嘱不存在或不属于当前患者')
      }

      const requestId = `billing-${++billingRequestSeed}`
      const finalTotalPrice = Number((catalog.unitPrice * item.quantity).toFixed(2))
      const pricedOrder: NurseStationPricedOrderVO = {
        requestId,
        orderType: catalog.category === 'DEVICE' ? 'DEVICE_REQUEST' : 'SERVICE_REQUEST',
        encounterId: item.encounterId,
        patientId: item.patientId,
        productDefinitionId: catalog.productDefinitionId,
        productName: catalog.name,
        quantity: item.quantity,
        unitCode: item.unitCode,
        unitName:
          catalog.unitBands.find((unit) => unit.unitCode === item.unitCode)?.unitName ??
          catalog.unitName,
        unitPrice: catalog.unitPrice,
        totalPrice: finalTotalPrice,
        pricedTime: dayjs().format('YYYY-MM-DD HH:mm:ss'),
        pricingMode: data.mode,
        pricingModeText: data.mode === 'DIRECT' ? '直接划价' : '医嘱补费',
        parentOrderAnchor: item.parentOrderAnchor ? { ...item.parentOrderAnchor } : undefined,
        chargeStatus: 'PENDING',
        chargeStatusText: '待收费',
        dispenseStatusText: catalog.category === 'DEVICE' ? '未发放' : undefined,
        cancellable: true
      }
      pricedOrders.unshift(pricedOrder)

      const parent = item.parentOrderAnchor
        ? supplementOrders.find((order) => order.requestId === item.parentOrderAnchor?.requestId)
        : undefined
      parent?.chargeItems.unshift({
        requestId,
        productName: catalog.name,
        quantity: item.quantity,
        unitName: pricedOrder.unitName,
        unitPrice: catalog.unitPrice,
        totalPrice: finalTotalPrice,
        pricedTime: pricedOrder.pricedTime,
        chargeStatusText: pricedOrder.chargeStatusText
      })
      return {
        clientItemId: item.clientItemId,
        success: true,
        message: '划价成功',
        requestId,
        finalUnitPrice: catalog.unitPrice,
        finalTotalPrice
      }
    } catch (error) {
      return {
        clientItemId: item.clientItemId,
        success: false,
        message: error instanceof Error ? error.message : '划价失败'
      }
    }
  })
  const result: NurseStationBillingWriteResultVO = {
    success: results.every((item) => item.success),
    processedCount: results.filter((item) => item.success).length,
    results
  }
  billingIdempotentResults.set(cacheKey, { fingerprint, result: structuredClone(result) })
  return structuredClone(result)
}

/** Mock：取消尚未收费或发放的划价项目。 */
export async function mockCancelNurseStationBilling(data: NurseStationCancelBillingDTO) {
  await Promise.resolve()
  assertIdempotencyKey(data.idempotencyKey)
  if (!data.reason.trim()) throw new Error('取消原因不能为空')
  if (data.items.length > 50) throw new Error('单批最多取消 50 个收费项目')
  const fingerprint = JSON.stringify(data)
  const cached = cancelIdempotentResults.get(data.idempotencyKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return structuredClone(cached.result)
  }

  const results = data.items.map((item) => {
    const row = pricedOrders.find(
      (candidate) =>
        candidate.requestId === item.requestId &&
        candidate.orderType === item.orderType &&
        candidate.encounterId === item.encounterId
    )
    if (!row) return { requestId: item.requestId, success: false, message: '划价记录不存在' }
    if (row.chargeStatus === 'CANCELLED') {
      return { requestId: item.requestId, success: true, message: '该项目已取消，无需重复操作' }
    }
    if (!row.cancellable) {
      return {
        requestId: item.requestId,
        success: false,
        message: row.cancelReasonDisabled ?? '当前状态不能取消'
      }
    }
    row.chargeStatus = 'CANCELLED'
    row.chargeStatusText = '已取消'
    row.cancellable = false
    row.cancelReasonDisabled = '项目已取消'
    return { requestId: item.requestId, success: true, message: '取消成功' }
  })
  const result: NurseStationCancelBillingResultVO = {
    success: results.every((item) => item.success),
    processedCount: results.filter((item) => item.success).length,
    results
  }
  cancelIdempotentResults.set(data.idempotencyKey, {
    fingerprint,
    result: structuredClone(result)
  })
  return structuredClone(result)
}

/** Mock：查询患者自动计费规则与护理级别只读项。 */
export async function mockQueryNurseStationAutoBillingRules(
  query: NurseStationAutoBillingQueryDTO
): Promise<NurseStationAutoBillingDataVO> {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  return {
    nursingLevelReadonly: {
      activityName: 'Ⅱ级护理',
      quantity: 1,
      unitName: '日',
      statusText: '随护理级别医嘱自动生成',
      chargeDefinitionName: 'Ⅱ级护理费',
      billDate: query.billDate ?? dayjs().format('YYYY-MM-DD')
    },
    rules: autoBillingRules.filter((rule) => rule.encounterId === query.encounterId).map(cloneRule)
  }
}

function getCachedAutoMutation(key: string, data: object) {
  assertIdempotencyKey(key)
  const fingerprint = JSON.stringify(data)
  const cached = autoMutationIdempotentResults.get(key)
  if (cached && cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
  return { fingerprint, cached: cached ? cloneMutationResult(cached.result) : undefined }
}

function cacheAutoMutation(
  key: string,
  fingerprint: string,
  result: NurseStationAutoBillingMutationResultVO
) {
  autoMutationIdempotentResults.set(key, { fingerprint, result: cloneMutationResult(result) })
  return cloneMutationResult(result)
}

/** Mock：新增或更新自动计费规则。 */
export async function mockSaveNurseStationAutoBillingRule(
  data: NurseStationSaveAutoBillingRuleDTO
) {
  await Promise.resolve()
  assertPatient(data.encounterId, data.patientId)
  const { fingerprint, cached } = getCachedAutoMutation(data.idempotencyKey, data)
  if (cached) return cached
  if (!data.ruleName.trim() || !data.activityId || !data.chargeDefinitionId) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '规则名称、触发项目和收费项目不能为空'
    })
  }

  const current = data.ruleId
    ? autoBillingRules.find(
        (rule) => rule.ruleId === data.ruleId && rule.encounterId === data.encounterId
      )
    : undefined
  if (data.ruleId && !current) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '自动计费规则不存在'
    })
  }
  if (current && current.version !== data.expectedVersion) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '规则已被他人修改，请刷新后重试'
    })
  }

  const saved: NurseStationAutoBillingRuleVO = {
    ruleId: current?.ruleId ?? `auto-rule-${++autoRuleSeed}`,
    version: (current?.version ?? 0) + 1,
    encounterId: data.encounterId,
    patientId: data.patientId,
    ruleName: data.ruleName.trim(),
    status: current?.status ?? 'ACTIVE',
    statusText: (current?.status ?? 'ACTIVE') === 'ACTIVE' ? '启用' : '停用',
    activityId: data.activityId,
    activityName: data.activityName,
    chargeDefinitionId: data.chargeDefinitionId,
    chargeDefinitionName: data.chargeDefinitionName,
    quantity: data.quantity,
    performingOrgId: data.performingOrgId,
    performingOrgName: data.performingOrgName,
    priority: data.priority,
    remark: data.remark,
    updatedAt: dayjs().format('YYYY-MM-DD HH:mm:ss')
  }
  if (current) Object.assign(current, saved)
  else autoBillingRules.push(saved)
  return cacheAutoMutation(data.idempotencyKey, fingerprint, {
    success: true,
    message: current ? '规则修改成功' : '规则新增成功',
    rule: saved
  })
}

/** Mock：软删除自动计费规则。 */
export async function mockDeleteNurseStationAutoBillingRule(
  data: NurseStationDeleteAutoBillingRuleDTO
) {
  await Promise.resolve()
  assertPatient(data.encounterId, data.patientId)
  const { fingerprint, cached } = getCachedAutoMutation(data.idempotencyKey, data)
  if (cached) return cached
  const index = autoBillingRules.findIndex(
    (rule) => rule.ruleId === data.ruleId && rule.encounterId === data.encounterId
  )
  const rule = autoBillingRules[index]
  if (!rule) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '自动计费规则不存在'
    })
  }
  if (rule.version !== data.expectedVersion) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '规则已被他人修改，请刷新后重试'
    })
  }
  autoBillingRules.splice(index, 1)
  return cacheAutoMutation(data.idempotencyKey, fingerprint, {
    success: true,
    message: '规则删除成功'
  })
}

/** Mock：把规则设置为明确的目标状态，重复请求不会把状态反向切换。 */
export async function mockSetNurseStationAutoBillingRuleStatus(
  data: NurseStationSetAutoBillingRuleStatusDTO
) {
  await Promise.resolve()
  assertPatient(data.encounterId, data.patientId)
  const { fingerprint, cached } = getCachedAutoMutation(data.idempotencyKey, data)
  if (cached) return cached
  const rule = autoBillingRules.find(
    (item) => item.ruleId === data.ruleId && item.encounterId === data.encounterId
  )
  if (!rule) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '自动计费规则不存在'
    })
  }
  if (rule.version !== data.expectedVersion) {
    return cacheAutoMutation(data.idempotencyKey, fingerprint, {
      success: false,
      message: '规则已被他人修改，请刷新后重试'
    })
  }
  rule.status = data.targetStatus
  rule.statusText = data.targetStatus === 'ACTIVE' ? '启用' : '停用'
  rule.version += 1
  rule.updatedAt = dayjs().format('YYYY-MM-DD HH:mm:ss')
  return cacheAutoMutation(data.idempotencyKey, fingerprint, {
    success: true,
    message: data.targetStatus === 'ACTIVE' ? '规则已启用' : '规则已停用',
    rule
  })
}
