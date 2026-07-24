import { describe, expect, it } from 'vitest'

import type { ClinicalOrderDraft } from './type'
import {
  moveClinicalOrder,
  removeClinicalOrder,
  updateClinicalOrder,
  validateClinicalOrders
} from './utils'

const orders: ClinicalOrderDraft[] = [
  {
    key: 'o1',
    catalogId: 'drug-1',
    code: 'YP0001',
    name: '测试药品',
    category: 'DRUG',
    categoryName: '西药',
    dose: 5,
    unit: 'mg',
    frequency: '每日一次',
    route: '口服',
    days: 7,
    quantity: 7,
    quantityUnit: '片'
  },
  {
    key: 'o2',
    catalogId: 'lab-1',
    code: 'JY0001',
    name: '血常规',
    category: 'LAB',
    categoryName: '检验',
    quantity: 1,
    quantityUnit: '次'
  }
]

describe('clinical order utils', () => {
  it('更新医嘱时不修改原数组', () => {
    const result = updateClinicalOrder(orders, 'o1', { dose: 10 })

    expect(result[0]?.dose).toBe(10)
    expect(orders[0]?.dose).toBe(5)
  })

  it('支持医嘱排序和删除', () => {
    const moved = moveClinicalOrder(orders, 'o2', -1)
    const removed = removeClinicalOrder(moved, 'o1')

    expect(moved[0]?.key).toBe('o2')
    expect(removed.map((item) => item.key)).toEqual(['o2'])
  })

  it('完整药品和项目医嘱能够通过校验', () => {
    expect(validateClinicalOrders(orders)).toEqual({ valid: true, errors: [] })
  })

  it('缺少药品剂量或项目数量时返回明确错误', () => {
    const invalidOrders: ClinicalOrderDraft[] = [
      { ...orders[0]!, dose: undefined },
      { ...orders[1]!, quantity: 0 }
    ]
    const result = validateClinicalOrders(invalidOrders)

    expect(result.valid).toBe(false)
    expect(result.errors.some((error) => error.field === 'dose')).toBe(true)
    expect(result.errors.some((error) => error.field === 'quantity')).toBe(true)
  })
})
