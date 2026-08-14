import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realExecuteNurseStationOrders,
  realQueryNurseStationOrderExecutions
} from './order-execute-real'

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站医嘱执行真实接口适配', () => {
  it('按就诊逐一查询并清空后端示例默认筛选', async () => {
    httpMock.get
      .mockResolvedValueOnce({
        data: {
          pageIndex: 1,
          pageSize: 100,
          total: 1,
          rows: [
            {
              id: 'service-001',
              encounterId: 'encounter-001',
              busNo: 'SR001',
              statusEnum: 'verified',
              quantity: 1,
              unitCode: 'time'
            }
          ]
        }
      })
      .mockResolvedValueOnce({ data: { total: 0, rows: [] } })

    const result = await realQueryNurseStationOrderExecutions({
      encounterIds: ['encounter-001', 'encounter-004']
    })

    expect(httpMock.get).toHaveBeenCalledTimes(2)
    expect(httpMock.get.mock.calls[0]?.[0]).toBe('/medical-order/execution-items')
    expect(httpMock.get.mock.calls[0]?.[1]).toEqual(
      expect.objectContaining({
        statusEnum: 'verified',
        encounterId: 'encounter-001',
        patientId: '',
        categoryEnum: '',
        intentEnum: ''
      })
    )
    expect(result.groups[0]?.orders[0]).toEqual(
      expect.objectContaining({ id: 'service-001', listRowKey: 'service-001' })
    )
    expect(result.groups[1]?.orders).toEqual([])
  })

  it('把页面批量选择拆成后端单条开始执行请求', async () => {
    httpMock.post
      .mockResolvedValueOnce({ data: 'Execution started' })
      .mockRejectedValueOnce(new Error('医嘱状态已变化'))

    const result = await realExecuteNurseStationOrders({
      performerId: 'nurse-001',
      serviceRequestIds: ['service-001', 'service-002']
    })

    expect(httpMock.post).toHaveBeenCalledTimes(2)
    expect(httpMock.post.mock.calls[0]).toEqual([
      '/medical-order/execute-order',
      { serviceRequestId: 'service-001', performerId: 'nurse-001' },
      expect.objectContaining({ baseURL: expect.any(String) })
    ])
    expect(result).toEqual(
      expect.objectContaining({
        success: false,
        processedCount: 1,
        results: expect.arrayContaining([
          expect.objectContaining({ serviceRequestId: 'service-001', success: true }),
          expect.objectContaining({ serviceRequestId: 'service-002', success: false })
        ])
      })
    )
  })
})
