import type {
  ClinicalOrderDraft,
  ClinicalOrderValidationError,
  ClinicalOrderValidationResult
} from './type'

/** 删除一条医嘱，不修改调用方传入的原数组。 */
export function removeClinicalOrder(
  orders: ClinicalOrderDraft[],
  key: ClinicalOrderDraft['key']
): ClinicalOrderDraft[] {
  return orders.filter((item) => item.key !== key).map((item) => ({ ...item }))
}

/** 上移或下移医嘱；超出首尾时保持原顺序。 */
export function moveClinicalOrder(
  orders: ClinicalOrderDraft[],
  key: ClinicalOrderDraft['key'],
  direction: -1 | 1
): ClinicalOrderDraft[] {
  const result = orders.map((item) => ({ ...item }))
  const currentIndex = result.findIndex((item) => item.key === key)
  const targetIndex = currentIndex + direction
  if (currentIndex < 0 || targetIndex < 0 || targetIndex >= result.length) return result
  ;[result[currentIndex], result[targetIndex]] = [result[targetIndex]!, result[currentIndex]!]
  return result
}

/** 更新一条医嘱的部分字段，保持受控数组不可变。 */
export function updateClinicalOrder(
  orders: ClinicalOrderDraft[],
  key: ClinicalOrderDraft['key'],
  patch: Partial<ClinicalOrderDraft>
): ClinicalOrderDraft[] {
  return orders.map((item) => (item.key === key ? { ...item, ...patch } : { ...item }))
}

/** 公共基础校验；门诊和住院适配器可在此结果上追加场景规则。 */
export function validateClinicalOrders(
  orders: ClinicalOrderDraft[]
): ClinicalOrderValidationResult {
  const errors: ClinicalOrderValidationError[] = []

  if (orders.length === 0) {
    return {
      valid: false,
      errors: [{ key: 'all', field: 'order', message: '请至少添加一条医嘱' }]
    }
  }

  for (const order of orders) {
    if (!order.name.trim()) {
      errors.push({ key: order.key, field: 'name', message: '医嘱项目名称不能为空' })
    }

    if (order.category === 'DRUG') {
      if (!order.dose || Number(order.dose) <= 0) {
        errors.push({ key: order.key, field: 'dose', message: order.name + '：请填写单次剂量' })
      }
      if (!order.unit) {
        errors.push({ key: order.key, field: 'unit', message: order.name + '：请选择剂量单位' })
      }
      if (!order.frequency) {
        errors.push({ key: order.key, field: 'frequency', message: order.name + '：请选择频次' })
      }
      if (!order.route) {
        errors.push({ key: order.key, field: 'route', message: order.name + '：请选择用法' })
      }
      if (!order.days || Number(order.days) <= 0) {
        errors.push({ key: order.key, field: 'days', message: order.name + '：请填写疗程天数' })
      }
    }

    if (!order.quantity || order.quantity <= 0) {
      errors.push({ key: order.key, field: 'quantity', message: order.name + '：请填写开立数量' })
    }
  }

  return { valid: errors.length === 0, errors }
}
