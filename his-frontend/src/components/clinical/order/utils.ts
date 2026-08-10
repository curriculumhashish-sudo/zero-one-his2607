import type {
  ClinicalOrderDraft,
  ClinicalOrderValidationError,
  ClinicalOrderValidationResult
} from './type'

/** 服务端保存时需要提交的草稿差异。 */
export interface ClinicalOrderDraftChanges {
  created: ClinicalOrderDraft[]
  updated: ClinicalOrderDraft[]
  deletedRecordIds: string[]
}

const frequencyTimesPerDay: Record<string, number> = {
  每日一次: 1,
  每日两次: 2,
  每日三次: 3,
  每晚一次: 1,
  QD: 1,
  BID: 2,
  TID: 3,
  QN: 1
}

function cloneOrders(orders: ClinicalOrderDraft[]) {
  return orders.map((item) => ({ ...item }))
}

function comparableOrder(order: ClinicalOrderDraft, orderNo: number) {
  return {
    catalogId: order.catalogId,
    code: order.code,
    name: order.name,
    category: order.category,
    categoryName: order.categoryName,
    specification: order.specification,
    dose: order.dose,
    unit: order.unit,
    frequency: order.frequency,
    route: order.route,
    days: order.days,
    quantity: order.quantity,
    quantityUnit: order.quantityUnit,
    dosePerQuantityUnit: order.dosePerQuantityUnit,
    remark: order.remark,
    orderNo
  }
}

/** 删除一条医嘱，不修改调用方传入的原数组。 */
export function removeClinicalOrder(
  orders: ClinicalOrderDraft[],
  key: ClinicalOrderDraft['key']
): ClinicalOrderDraft[] {
  return normalizeOrphanClinicalOrderGroups(
    orders.filter((item) => item.key !== key).map((item) => ({ ...item }))
  )
}

/** 只更新草稿医嘱；已签发和已撤回医嘱始终保持只读。 */
export function updateClinicalOrder(
  orders: ClinicalOrderDraft[],
  key: ClinicalOrderDraft['key'],
  patch: Partial<ClinicalOrderDraft>
): ClinicalOrderDraft[] {
  return orders.map((item) =>
    item.key === key && item.status === 'DRAFT' ? { ...item, ...patch } : { ...item }
  )
}

/** 清理只剩一条草稿的孤立分组。 */
export function normalizeOrphanClinicalOrderGroups(
  orders: ClinicalOrderDraft[]
): ClinicalOrderDraft[] {
  const counts = new Map<string, number>()
  for (const order of orders) {
    if (order.status === 'DRAFT' && order.groupId) {
      counts.set(order.groupId, (counts.get(order.groupId) ?? 0) + 1)
    }
  }
  return orders.map((order) =>
    order.status === 'DRAFT' && order.groupId && (counts.get(order.groupId) ?? 0) < 2
      ? { ...order, groupId: undefined }
      : { ...order }
  )
}

/**
 * 任意并组：只要求至少选择两条草稿，可跨药品、检查、检验和治疗分类。
 * 已在其他组中的草稿会改用新的 groupId。
 */
export function groupClinicalOrders(
  orders: ClinicalOrderDraft[],
  keys: ClinicalOrderDraft['key'][],
  groupId: string
): ClinicalOrderDraft[] {
  const selectedKeys = new Set(keys)
  const selected = orders.filter((item) => selectedKeys.has(item.key))
  if (
    selected.length < 2 ||
    selected.some((item) => item.status !== 'DRAFT') ||
    selected.length !== selectedKeys.size
  ) {
    return cloneOrders(orders)
  }
  return normalizeOrphanClinicalOrderGroups(
    orders.map((item) => (selectedKeys.has(item.key) ? { ...item, groupId } : { ...item }))
  )
}

/** 拆除选中草稿的分组，并自动清理旧组中剩余的孤立行。 */
export function ungroupClinicalOrders(
  orders: ClinicalOrderDraft[],
  keys: ClinicalOrderDraft['key'][]
): ClinicalOrderDraft[] {
  const selectedKeys = new Set(keys)
  if (orders.filter((item) => selectedKeys.has(item.key)).some((item) => item.status !== 'DRAFT')) {
    return cloneOrders(orders)
  }
  return normalizeOrphanClinicalOrderGroups(
    orders.map((item) =>
      selectedKeys.has(item.key) ? { ...item, groupId: undefined } : { ...item }
    )
  )
}

/**
 * 根据单次剂量、每单位含量、每日频次和疗程计算建议开立数量。
 * “必要时”或无法可靠换算时返回 undefined，由医生手工填写。
 */
export function calculateSuggestedOrderQuantity(
  order: Pick<
    ClinicalOrderDraft,
    'category' | 'dose' | 'dosePerQuantityUnit' | 'frequency' | 'days'
  >
): number | undefined {
  if (order.category !== 'DRUG' || !order.frequency || order.frequency === '必要时') {
    return undefined
  }
  const dose = Number(order.dose)
  const dosePerUnit = Number(order.dosePerQuantityUnit)
  const days = Number(order.days)
  const timesPerDay = frequencyTimesPerDay[order.frequency]
  if (
    !Number.isFinite(dose) ||
    !Number.isFinite(dosePerUnit) ||
    !Number.isFinite(days) ||
    dose <= 0 ||
    dosePerUnit <= 0 ||
    days <= 0 ||
    !timesPerDay
  ) {
    return undefined
  }
  return Math.ceil((dose / dosePerUnit) * timesPerDay * days)
}

/** 比较当前草稿与最近一次服务端快照，供统一保存按 CRUD 提交。 */
export function buildClinicalOrderDraftChanges(
  current: ClinicalOrderDraft[],
  saved: ClinicalOrderDraft[]
): ClinicalOrderDraftChanges {
  const currentRecordIds = new Set(
    current.map((item) => item.recordId).filter((id): id is string => Boolean(id))
  )
  const savedById = new Map(
    saved
      .filter((item): item is ClinicalOrderDraft & { recordId: string } => Boolean(item.recordId))
      .map((item) => [item.recordId, item])
  )

  const created = current.filter((item) => item.status === 'DRAFT' && !item.recordId)
  const updated = current.filter((item, index) => {
    if (item.status !== 'DRAFT' || !item.recordId) return false
    const previous = savedById.get(item.recordId)
    if (!previous) return false
    const previousIndex = saved.findIndex((savedItem) => savedItem.recordId === item.recordId)
    return (
      JSON.stringify(comparableOrder(item, index + 1)) !==
      JSON.stringify(comparableOrder(previous, previousIndex + 1))
    )
  })
  const deletedRecordIds = saved
    .filter(
      (item): item is ClinicalOrderDraft & { recordId: string } =>
        item.status === 'DRAFT' && Boolean(item.recordId) && !currentRecordIds.has(item.recordId!)
    )
    .map((item) => item.recordId)

  return {
    created: cloneOrders(created),
    updated: cloneOrders(updated),
    deletedRecordIds
  }
}

/** 公共基础校验；空列表合法，删除最后一条草稿可以正常保存。 */
export function validateClinicalOrders(
  orders: ClinicalOrderDraft[]
): ClinicalOrderValidationResult {
  const errors: ClinicalOrderValidationError[] = []

  for (const order of orders.filter((item) => item.status === 'DRAFT')) {
    if (!order.name.trim()) {
      errors.push({ key: order.key, field: 'name', message: '医嘱项目名称不能为空' })
    }

    if (order.category === 'DRUG') {
      if (!order.dose || Number(order.dose) <= 0) {
        errors.push({ key: order.key, field: 'dose', message: `${order.name}：请填写单次剂量` })
      }
      if (!order.unit) {
        errors.push({ key: order.key, field: 'unit', message: `${order.name}：请选择剂量单位` })
      }
      if (!order.frequency) {
        errors.push({ key: order.key, field: 'frequency', message: `${order.name}：请选择频次` })
      }
      if (!order.route) {
        errors.push({ key: order.key, field: 'route', message: `${order.name}：请选择用法` })
      }
      if (!order.days || Number(order.days) <= 0) {
        errors.push({ key: order.key, field: 'days', message: `${order.name}：请填写疗程天数` })
      }
    }

    if (!order.quantity || order.quantity <= 0) {
      errors.push({ key: order.key, field: 'quantity', message: `${order.name}：请填写开立数量` })
    }
  }

  return { valid: errors.length === 0, errors }
}
