import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realQueryNurseStationOrderAudits,
  realRejectNurseStationOrders,
  realVerifyNurseStationOrders
} from './order-audit-real'

const patientContexts = [
  { encounterId: 'encounter-001', patientId: 'patient-001', patientName: '张三' },
  { encounterId: 'encounter-002', patientId: 'patient-002', patientName: '李四' }
]

function backendRecord(name: string, requestId: string) {
  return {
    requestId,
    orderType: 'MED_REQUEST',
    name,
    isPassed: 'false',
    content: `${name}的医嘱`,
    type: 'LONG_TERM'
  }
}

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站医嘱校对真实接口适配', () => {
  it('只发送后端支持的姓名和校对状态并映射最小字段', async () => {
    httpMock.get.mockImplementation((_url: string, params: Record<string, unknown>) =>
      Promise.resolve({
        data: [backendRecord(String(params.name), `request-${params.name}`)]
      })
    )

    const result = await realQueryNurseStationOrderAudits({
      patientContexts,
      auditStatus: 'PENDING'
    })

    expect(httpMock.get).toHaveBeenCalledTimes(2)
    expect(httpMock.get.mock.calls[0]?.[1]).toEqual({ name: '张三', isPassed: 'false' })
    expect(httpMock.get.mock.calls[1]?.[1]).toEqual({ name: '李四', isPassed: 'false' })
    expect(result.groups[0]?.orders[0]).toEqual({
      listRowKey: 'MED_REQUEST:request-张三',
      requestId: 'request-张三',
      orderType: 'MED_REQUEST',
      content: '张三的医嘱'
    })
  })

  it('同名患者存在时阻止调用旧查询接口', async () => {
    await expect(
      realQueryNurseStationOrderAudits({
        patientContexts: [
          patientContexts[0]!,
          { encounterId: 'encounter-003', patientId: 'patient-003', patientName: '张三' }
        ],
        auditStatus: 'PENDING'
      })
    ).rejects.toThrow('无法安全查询同名患者')
    expect(httpMock.get).not.toHaveBeenCalled()
  })

  it('后端缺少必填字段时报告契约错误', async () => {
    httpMock.get.mockResolvedValue({
      data: [{ orderType: 'MED_REQUEST', name: '张三', isPassed: 'false', content: '医嘱' }]
    })

    await expect(
      realQueryNurseStationOrderAudits({
        patientContexts: [patientContexts[0]!],
        auditStatus: 'PENDING'
      })
    ).rejects.toThrow('requestId')
  })

  it('校对和退回只发送当前后端支持的字段', async () => {
    httpMock.post.mockResolvedValue({ data: { success: true, processedCount: 1 } })
    const items = [{ orderType: 'MED_REQUEST' as const, requestId: 'request-001' }]

    const verified = await realVerifyNurseStationOrders({ items })
    const rejected = await realRejectNurseStationOrders({
      items,
      reason: '患者信息不完整'
    })

    expect(httpMock.post).toHaveBeenNthCalledWith(
      1,
      '/medicalOrderVerification/verify-pass',
      { items },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(httpMock.post).toHaveBeenNthCalledWith(
      2,
      '/medicalOrderVerification/reject-orders',
      { items, reason: '患者信息不完整' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(verified).toEqual({ success: true, processedCount: 1 })
    expect(rejected).toEqual({ success: true, processedCount: 1 })
  })

  it('批量结果不完整和网络失败时不伪造结果或回退 Mock', async () => {
    httpMock.post.mockResolvedValueOnce({ data: { success: true } })
    await expect(
      realVerifyNurseStationOrders({
        items: [{ orderType: 'SERVICE_REQUEST', requestId: 'request-002' }]
      })
    ).rejects.toThrow('批量处理结果不完整')

    httpMock.get.mockRejectedValueOnce(new Error('医嘱查询失败'))
    await expect(
      realQueryNurseStationOrderAudits({
        patientContexts: [patientContexts[0]!],
        auditStatus: 'PENDING'
      })
    ).rejects.toThrow('医嘱查询失败')
  })
})
