import dayjs from 'dayjs'
import { describe, expect, it } from 'vitest'

import { queryNurseStationCostDetail, queryNurseStationCostSummary } from './index'

describe('住院护士站费用明细 Mock API', () => {
  it('按后端字段分页返回指定患者的费用明细', async () => {
    const date = dayjs().format('YYYY-MM-DD')
    const result = await queryNurseStationCostDetail({
      pageIndex: 1,
      pageSize: 2,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      startDate: date,
      endDate: date
    })

    expect(result.total).toBe(3)
    expect(result.rows).toHaveLength(2)
    expect(result.rows?.[0]).toEqual(
      expect.objectContaining({
        id: expect.any(String),
        chargeName: expect.any(String),
        quantityValue: expect.any(Number),
        occurrenceTime: expect.stringContaining(date),
        statusEnum: 'PENDING_SETTLEMENT'
      })
    )
  })

  it('支持后端已有的费用类型和业务流水号筛选', async () => {
    const date = dayjs().format('YYYY-MM-DD')
    const result = await queryNurseStationCostDetail({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      startDate: date,
      endDate: date,
      typeCode: 'DEVICE',
      busNo: 'CHG-003'
    })

    expect(result.rows).toEqual([expect.objectContaining({ typeCode: 'DEVICE', busNo: 'CHG-003' })])
  })

  it('按费用类型和日期返回后端口径的汇总', async () => {
    const date = dayjs().format('YYYY-MM-DD')
    const result = await queryNurseStationCostSummary({
      encounterId: 'encounter-001',
      startDate: date,
      endDate: date
    })

    expect(result).toHaveLength(3)
    expect(result).toEqual(
      expect.arrayContaining([
        expect.objectContaining({
          typeCode: 'DRUG',
          summaryDate: date,
          count: 1,
          totalAmount: 37
        })
      ])
    )
  })
})
