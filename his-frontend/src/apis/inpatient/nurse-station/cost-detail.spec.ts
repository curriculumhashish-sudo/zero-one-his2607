import dayjs from 'dayjs'
import { describe, expect, it } from 'vitest'

import { loadNurseStationCostDetailInit, queryNurseStationCostDetail } from './index'

describe('住院护士站费用明细 Mock API', () => {
  it('初始化返回默认日期、展示模式和费用大类顺序', async () => {
    const result = await loadNurseStationCostDetailInit()

    expect(result.dateFrom).toBe(dayjs().format('YYYY-MM-DD'))
    expect(result.dateTo).toBe(dayjs().format('YYYY-MM-DD'))
    expect(result.presentationOptions).toEqual([
      { code: 'DETAIL', display: '明细' },
      { code: 'SUMMARY', display: '汇总' }
    ])
    expect(result.expenseBlockOrder).toEqual(['MEDICATION', 'SERVICE', 'DEVICE'])
  })

  it('明细分页不影响基于完整筛选结果计算的合计', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    const result = await queryNurseStationCostDetail({
      pageIndex: 1,
      pageSize: 3,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      dateFrom: today,
      dateTo: today,
      presentationMode: 'DETAIL'
    })

    expect(result.detailPage.rows).toHaveLength(3)
    expect(result.detailPage.total).toBe(7)
    expect(result.totalDetailCount).toBe(7)
    expect(result.totals).toEqual(
      expect.objectContaining({
        medicationTotal: 49.4,
        serviceTotal: 28,
        deviceTotal: 8.3,
        grandTotal: 85.7
      })
    )
    expect(result.summaryItemCount).toBe(6)
  })

  it('退费行使用负数量负金额，并在汇总中抵减原收费', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    const result = await queryNurseStationCostDetail({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      dateFrom: today,
      dateTo: today,
      presentationMode: 'SUMMARY'
    })

    expect(result.detailPage.rows?.find((row) => row.refundHighlight)).toEqual(
      expect.objectContaining({ quantity: -1, totalPrice: -1.5, status: 'REFUNDED' })
    )
    expect(result.summaryLines.find((row) => row.productId === 'device-syringe-20')).toEqual(
      expect.objectContaining({ splitUnitQuantity: 1, lineTotalAmount: 1.5 })
    )
  })

  it('昨日快捷日期只返回昨日产生的费用', async () => {
    const yesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')
    const result = await queryNurseStationCostDetail({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      dateFrom: yesterday,
      dateTo: yesterday,
      presentationMode: 'DETAIL'
    })

    expect(result.totalDetailCount).toBe(1)
    expect(result.detailPage.rows?.[0]?.itemName).toBe('血细胞分析')
    expect(result.totals.grandTotal).toBe(28)
  })

  it('拒绝患者和住院就诊不匹配的查询', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    await expect(
      queryNurseStationCostDetail({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'encounter-001',
        patientId: 'patient-004',
        dateFrom: today,
        dateTo: today,
        presentationMode: 'DETAIL'
      })
    ).rejects.toThrow('患者与住院记录不匹配')
  })
})
