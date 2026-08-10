// @vitest-environment jsdom

import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import { pageAdviceOrders, printAdviceApplicationForm } from '..'

async function resolveRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('doctor advice application form API mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('根据已签发非药品医嘱生成申请单且不改变医嘱', async () => {
    const before = await resolveRequest(
      pageAdviceOrders({ encounterId: 'enc-20260723-005', pageIndex: 1, pageSize: 10 })
    )
    const order = before.rows?.[0]
    expect(order?.status).toBe('SIGNED')
    expect(order?.category).toBe('EXAM')
    expect(order?.serviceRequestId).toBeTruthy()

    const form = await resolveRequest(
      printAdviceApplicationForm({ serviceRequestId: order!.serviceRequestId! })
    )
    expect(form.patientName).toBeTruthy()
    expect(form.itemName).toBe(order?.name)
    expect(form.categoryName).toBe(order?.categoryName)

    const after = await resolveRequest(
      pageAdviceOrders({ encounterId: 'enc-20260723-005', pageIndex: 1, pageSize: 10 })
    )
    expect(after.rows?.[0]).toEqual(before.rows?.[0])
  })

  it('拒绝不存在的服务申请 ID', async () => {
    const request = printAdviceApplicationForm({ serviceRequestId: 'SR-NOT-FOUND' })
    const expectation = expect(request).rejects.toThrow('未找到对应的医嘱申请单')
    await vi.runAllTimersAsync()
    await expectation
  })
})
