import { describe, expect, it } from 'vitest'

import type { ClinicalOrderDraft } from './type'
import {
  buildClinicalOrderDraftChanges,
  calculateSuggestedOrderQuantity,
  groupClinicalOrders,
  removeClinicalOrder,
  ungroupClinicalOrders,
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
    quantityUnit: '片',
    dosePerQuantityUnit: 5,
    status: 'DRAFT'
  },
  {
    key: 'o2',
    catalogId: 'lab-1',
    code: 'JY0001',
    name: '血常规',
    category: 'LAB',
    categoryName: '检验',
    quantity: 1,
    quantityUnit: '次',
    status: 'DRAFT'
  }
]

describe('clinical order utils', () => {
  it('更新医嘱时不修改原数组', () => {
    const result = updateClinicalOrder(orders, 'o1', { dose: 10 })

    expect(result[0]?.dose).toBe(10)
    expect(orders[0]?.dose).toBe(5)
  })

  it('支持删除医嘱', () => {
    const removed = removeClinicalOrder(orders, 'o1')

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

  it('允许删除最后一条草稿并保存空列表', () => {
    expect(validateClinicalOrders([])).toEqual({ valid: true, errors: [] })
  })

  it('已签发和已撤回医嘱只读且不参与草稿校验', () => {
    const formalOrders: ClinicalOrderDraft[] = [
      { ...orders[0]!, status: 'SIGNED', quantity: 0 },
      { ...orders[1]!, status: 'WITHDRAWN', name: '' }
    ]

    expect(updateClinicalOrder(formalOrders, 'o1', { quantity: 9 })[0]?.quantity).toBe(0)
    expect(validateClinicalOrders(formalOrders)).toEqual({ valid: true, errors: [] })
  })

  it('支持跨分类任意并组、重新并组和孤立组清理', () => {
    const grouped = groupClinicalOrders(orders, ['o1', 'o2'], 'group-a')
    expect(grouped.map((item) => item.groupId)).toEqual(['group-a', 'group-a'])

    const third: ClinicalOrderDraft = {
      ...orders[1]!,
      key: 'o3',
      recordId: 'record-3',
      catalogId: 'exam-1',
      category: 'EXAM',
      categoryName: '检查'
    }
    const regrouped = groupClinicalOrders([...grouped, third], ['o2', 'o3'], 'group-b')
    expect(regrouped.map((item) => item.groupId)).toEqual([undefined, 'group-b', 'group-b'])

    const ungrouped = ungroupClinicalOrders(regrouped, ['o2'])
    expect(ungrouped.map((item) => item.groupId)).toEqual([undefined, undefined, undefined])
  })

  it('按剂量、频次和疗程计算建议数量，必要时用药保留手工输入', () => {
    expect(calculateSuggestedOrderQuantity(orders[0]!)).toBe(7)
    expect(calculateSuggestedOrderQuantity({ ...orders[0]!, frequency: '每日三次', days: 5 })).toBe(
      15
    )
    expect(calculateSuggestedOrderQuantity({ ...orders[0]!, frequency: '必要时' })).toBeUndefined()
  })

  it('计算统一保存所需的新增、修改和删除差异', () => {
    const saved: ClinicalOrderDraft[] = [
      { ...orders[0]!, recordId: 'record-1' },
      { ...orders[1]!, recordId: 'record-2' }
    ]
    const current: ClinicalOrderDraft[] = [
      { ...saved[0]!, quantity: 9 },
      {
        ...orders[1]!,
        key: 'new-row',
        catalogId: 'exam-2',
        category: 'EXAM',
        categoryName: '检查'
      }
    ]

    const changes = buildClinicalOrderDraftChanges(current, saved)
    expect(changes.updated.map((item) => item.recordId)).toEqual(['record-1'])
    expect(changes.created.map((item) => item.key)).toEqual(['new-row'])
    expect(changes.deletedRecordIds).toEqual(['record-2'])
  })
})
