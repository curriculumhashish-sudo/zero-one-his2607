import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import dayjs from 'dayjs'

import {
  mockCancelNurseExecution,
  mockGetNurseConsumables,
  mockGetNurseExecutionItems,
  mockPageNursePatientQueue,
  mockPagePatientExecutionHistory,
  mockPrintNurseExecutions,
  mockPerformNurseExecutions
} from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('nurse-station mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('按预约日期查询患者队列', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    const page = await resolveMockRequest(
      mockPageNursePatientQueue({
        pageIndex: 1,
        pageSize: 20,
        bookingDate: today
      })
    )

    expect(page.total).toBe(2)
    expect(page.rows?.map((row) => row.patientName)).toEqual(['林安然', '周远航'])
    expect(page.rows?.map((row) => row.patientId)).toEqual(['mock-patient-001', 'mock-patient-002'])
  })

  it('全功能患者覆盖待执行、已完成、已取消和耗材真实字段', async () => {
    const executionRequest = mockGetNurseExecutionItems('mock-patient-001', 'mock-encounter-001')
    const consumableRequest = mockGetNurseConsumables('mock-patient-001', 'mock-encounter-001')

    await vi.runAllTimersAsync()
    const [executionItems, consumables] = await Promise.all([executionRequest, consumableRequest])

    expect(executionItems.map((item) => item.status)).toEqual([
      'PENDING',
      'PENDING',
      'COMPLETED',
      'CANCELLED'
    ])
    expect(consumables).toEqual([
      expect.objectContaining({
        deviceDefId: 'MOCK-DEVICE-INFUSION-001',
        supportInfo: '一次性使用输液器',
        statusEnum: 'PENDING'
      }),
      expect.objectContaining({
        deviceDefId: 'MOCK-DEVICE-DRESSING-002',
        statusEnum: 'DISPENSED'
      })
    ])
    expect(consumables[1]?.supportInfo).toBeUndefined()
  })

  it('空状态患者可从无耗材、无历史记录开始体验执行', async () => {
    const executionRequest = mockGetNurseExecutionItems('mock-patient-002', 'mock-encounter-002')
    const consumableRequest = mockGetNurseConsumables('mock-patient-002', 'mock-encounter-002')
    const historyRequest = mockPagePatientExecutionHistory('mock-patient-002', {
      pageIndex: 1,
      pageSize: 10
    })

    await vi.runAllTimersAsync()
    const [executionItems, consumables, history] = await Promise.all([
      executionRequest,
      consumableRequest,
      historyRequest
    ])

    expect(executionItems).toEqual([
      expect.objectContaining({ itemId: 'mock-exec-empty-001', status: 'PENDING' })
    ])
    expect(consumables).toEqual([])
    expect(history.total).toBe(0)
  })

  it('全功能患者提供超过一页的历史执行记录', async () => {
    const firstPageRequest = mockPagePatientExecutionHistory('mock-patient-001', {
      pageIndex: 1,
      pageSize: 10
    })
    await vi.runAllTimersAsync()
    const firstPage = await firstPageRequest

    const secondPageRequest = mockPagePatientExecutionHistory('mock-patient-001', {
      pageIndex: 2,
      pageSize: 10
    })
    await vi.runAllTimersAsync()
    const secondPage = await secondPageRequest

    expect(firstPage.total).toBe(12)
    expect(firstPage.pages).toBe(2)
    expect(firstPage.rows).toHaveLength(10)
    expect(secondPage.rows).toHaveLength(2)
  })

  it('重复执行同一项目时返回状态错误', async () => {
    const request = {
      encounterId: 'mock-encounter-001',
      items: [{ itemId: 'mock-exec-pending-001' }],
      executorId: 'nurse-001'
    }

    const firstRequest = mockPerformNurseExecutions(request)
    await vi.runAllTimersAsync()
    const first = await firstRequest

    const repeatedRequest = mockPerformNurseExecutions(request)
    await vi.runAllTimersAsync()
    const repeated = await repeatedRequest

    expect(first.successIds).toEqual(['mock-exec-pending-001'])
    expect(repeated.failedItems[0]?.errorCode).toBe('INVALID_STATUS')

    const rowsRequest = mockGetNurseExecutionItems('mock-patient-001', request.encounterId)
    await vi.runAllTimersAsync()
    const rows = await rowsRequest
    expect(rows.find((item) => item.itemId === 'mock-exec-pending-001')?.executionCount).toBe(1)

    const queueRequest = mockPageNursePatientQueue({
      pageIndex: 1,
      pageSize: 20,
      bookingDate: dayjs().format('YYYY-MM-DD')
    })
    await vi.runAllTimersAsync()
    const queue = await queueRequest
    expect(queue.rows?.[0]?.status).toBe('READY')
  })

  it('批量执行允许部分成功并返回逐项失败原因', async () => {
    const resultRequest = mockPerformNurseExecutions({
      encounterId: 'mock-encounter-001',
      items: [{ itemId: 'mock-exec-pending-002' }, { itemId: 'mock-exec-completed-001' }],
      executorId: 'nurse-001'
    })
    await vi.runAllTimersAsync()
    const result = await resultRequest

    expect(result.successIds).toEqual(['mock-exec-pending-002'])
    expect(result.failedItems).toEqual([
      expect.objectContaining({
        recordId: 'mock-exec-completed-001',
        errorCode: 'INVALID_STATUS'
      })
    ])
  })

  it('取消最近一次执行并阻止重复取消', async () => {
    const cancelledRequest = mockCancelNurseExecution({
      encounterId: 'mock-encounter-001',
      item: { itemId: 'mock-exec-completed-001' },
      reason: '登记错误'
    })
    await vi.runAllTimersAsync()
    const cancelled = await cancelledRequest

    expect(cancelled.successIds).toEqual(['mock-exec-completed-001'])

    const staleRequest = mockCancelNurseExecution({
      encounterId: 'mock-encounter-001',
      item: { itemId: 'mock-exec-completed-001' },
      reason: '重复取消'
    })
    await vi.runAllTimersAsync()
    const stale = await staleRequest

    expect(stale.failedItems[0]?.errorCode).toBe('INVALID_STATUS')
  })

  it('生成可供浏览器打印的服务请求文本', async () => {
    const content = await resolveMockRequest(
      mockPrintNurseExecutions({ recordIds: ['mock-exec-completed-001'] })
    )

    expect(content).toContain('服务请求打印单')
    expect(content).toContain('生命体征测量')
  })
})
