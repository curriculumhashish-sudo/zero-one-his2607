import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({ get: vi.fn<(...args: unknown[]) => unknown>() }))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realQueryNurseStationCostDetail,
  realQueryNurseStationCostSummary
} from './cost-detail-real'

beforeEach(() => {
  httpMock.get.mockReset()
})

describe('住院护士站费用明细真实接口适配', () => {
  it('透传后端查询字段并标准化分页结果', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 1,
        rows: [
          {
            id: 'charge-001',
            patientId: 'patient-001',
            encounterId: 'encounter-001',
            chargeName: 'Ⅱ级护理',
            typeCode: 'SERVICE',
            unitPrice: 18,
            quantityValue: 1,
            quantityUnit: '日',
            totalPrice: 18,
            baseAmount: 18,
            discountAmount: 0,
            occurrenceTime: '2026-08-10 00:05:00',
            performingOrgId: 'org-001',
            statusEnum: 'PENDING_SETTLEMENT',
            busNo: 'CHG-001'
          }
        ]
      }
    })
    const query = {
      pageIndex: 1,
      pageSize: 20,
      patientId: 'patient-001',
      encounterId: 'encounter-001',
      startDate: '2026-08-10',
      endDate: '2026-08-10'
    }

    const result = await realQueryNurseStationCostDetail(query)

    expect(httpMock.get).toHaveBeenCalledWith('/chargedetails/query-details', query, {
      baseURL: expect.any(String)
    })
    expect(result).toEqual(
      expect.objectContaining({
        total: 1,
        pages: 1,
        rows: [expect.objectContaining({ id: 'charge-001', chargeName: 'Ⅱ级护理' })]
      })
    )
  })

  it('调用费用分类日期汇总接口', async () => {
    httpMock.get.mockResolvedValue({
      data: [
        {
          typeCode: 'SERVICE',
          typeName: 'SERVICE',
          totalAmount: 18,
          count: 1,
          insuranceAmount: 0,
          selfPayAmount: 18,
          summaryDate: '2026-08-10'
        }
      ]
    })
    const query = {
      encounterId: 'encounter-001',
      startDate: '2026-08-10',
      endDate: '2026-08-10'
    }

    const result = await realQueryNurseStationCostSummary(query)

    expect(httpMock.get).toHaveBeenCalledWith('/chargedetails/query-summary', query, {
      baseURL: expect.any(String)
    })
    expect(result[0]).toEqual(expect.objectContaining({ typeCode: 'SERVICE', totalAmount: 18 }))
  })

  it('后端未返回 data 时抛出明确错误', async () => {
    httpMock.get.mockResolvedValue({ message: '查询失败' })
    await expect(
      realQueryNurseStationCostSummary({
        encounterId: 'encounter-001',
        startDate: '2026-08-10',
        endDate: '2026-08-10'
      })
    ).rejects.toThrow('查询失败')
  })
})
