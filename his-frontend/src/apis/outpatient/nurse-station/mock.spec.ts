import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import dayjs from 'dayjs'

import {
  mockCancelNurseExecution,
  mockGetNurseConsumables,
  mockGetNurseExecutionItems,
  mockPageNursePatientQueue,
  mockPerformNurseExecutions,
  mockPrintNurseDocuments
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

  it('按日期和关键词查询患者队列', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    const page = await resolveMockRequest(
      mockPageNursePatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: 'M20260728001',
        startDate: today,
        endDate: today
      })
    )

    expect(page.total).toBe(1)
    expect(page.rows?.[0]?.patientName).toBe('林安然')
  })

  it('按就诊隔离执行项目和耗材', async () => {
    const executionRequest = mockGetNurseExecutionItems('enc-nur-20260728-002')
    const consumableRequest = mockGetNurseConsumables('enc-nur-20260728-002')

    await vi.runAllTimersAsync()
    const [executionItems, consumables] = await Promise.all([executionRequest, consumableRequest])

    expect(executionItems.map((item) => item.itemId)).toEqual(['exec-004', 'exec-005'])
    expect(consumables.map((item) => item.dispenseItemId)).toEqual(['dispense-003'])
  })

  it('提交单一打印任务并返回打印机信息', async () => {
    const result = await resolveMockRequest(
      mockPrintNurseDocuments({
        encounterId: 'enc-nur-20260728-001',
        documentIds: ['exec-001', 'exec-002'],
        documentType: 'TREATMENT_SHEET',
        reprint: false
      })
    )

    expect(result.success).toBe(true)
    expect(result.printer).toBeTruthy()
    expect(result.message).toContain('2')
    expect(result.documentType).toBe('TREATMENT_SHEET')
  })

  it('使用幂等键防止同一执行请求重复计数', async () => {
    const request = {
      encounterId: 'enc-nur-20260728-001',
      items: [{ itemId: 'exec-001', version: 0 }],
      idempotencyKey: 'idempotency-exec-001'
    }

    const firstRequest = mockPerformNurseExecutions(request)
    await vi.runAllTimersAsync()
    const first = await firstRequest

    const repeatedRequest = mockPerformNurseExecutions(request)
    await vi.runAllTimersAsync()
    const repeated = await repeatedRequest

    expect(first.successItems[0]?.executedCount).toBe(1)
    expect(repeated).toEqual(first)

    const rowsRequest = mockGetNurseExecutionItems(request.encounterId)
    await vi.runAllTimersAsync()
    const rows = await rowsRequest
    expect(rows.find((item) => item.itemId === 'exec-001')?.executedCount).toBe(1)

    const queueRequest = mockPageNursePatientQueue({
      pageIndex: 1,
      pageSize: 20,
      keyword: 'M20260728001'
    })
    await vi.runAllTimersAsync()
    const queue = await queueRequest
    expect(queue.rows?.[0]?.pendingCount).toBe(3)
  })

  it('批量执行允许部分成功并返回逐项失败原因', async () => {
    const resultRequest = mockPerformNurseExecutions({
      encounterId: 'enc-nur-20260728-002',
      items: [
        { itemId: 'exec-004', version: 0 },
        { itemId: 'exec-005', version: 0 }
      ],
      idempotencyKey: 'partial-success'
    })
    await vi.runAllTimersAsync()
    const result = await resultRequest

    expect(result.successItems.map((item) => item.itemId)).toEqual(['exec-004'])
    expect(result.failedItems).toEqual([
      expect.objectContaining({ itemId: 'exec-005', code: 'INVALID_STATUS' })
    ])
  })

  it('取消最近一次执行并校验版本冲突', async () => {
    const cancelledRequest = mockCancelNurseExecution({
      encounterId: 'enc-nur-20260728-003',
      item: { itemId: 'exec-006', version: 0 },
      reason: '登记错误'
    })
    await vi.runAllTimersAsync()
    const cancelled = await cancelledRequest

    expect(cancelled.successItems[0]).toMatchObject({
      itemId: 'exec-006',
      status: 'PENDING',
      executedCount: 0,
      version: 1
    })

    const staleRequest = mockCancelNurseExecution({
      encounterId: 'enc-nur-20260728-003',
      item: { itemId: 'exec-006', version: 0 },
      reason: '重复取消'
    })
    await vi.runAllTimersAsync()
    const stale = await staleRequest

    expect(stale.failedItems[0]?.code).toBe('VERSION_CONFLICT')
  })
})
