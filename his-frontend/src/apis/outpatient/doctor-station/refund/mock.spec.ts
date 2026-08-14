import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import { mockApplyDoctorRefund, mockQueryDoctorRefundItems } from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('doctor refund mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('按病历号查询收费和退费状态', async () => {
    const rows = await resolveMockRequest(mockQueryDoctorRefundItems({ num: 'MR20260003' }))
    expect(rows).toHaveLength(4)
    expect(rows.some((row) => row.refundStatus === '未申请')).toBe(true)
    expect(rows.some((row) => row.refundStatus === '已退费')).toBe(true)
  })

  it('提交勾选明细后生成申请号，并把明细更新为退费中', async () => {
    const rows = await resolveMockRequest(mockQueryDoctorRefundItems({ num: 'MR20260008' }))
    const selected = rows.filter(
      (row) => row.chargeStatus === '已收费' && row.refundStatus === '未申请'
    )
    const applicationNo = await resolveMockRequest(
      mockApplyDoctorRefund({
        patientNo: 'MR20260008',
        apply: selected,
        applyNo: selected.length,
        applySum: 28.4
      })
    )
    expect(applicationNo).toMatch(/^RF\d{12}$/)

    const refreshedRows = await resolveMockRequest(
      mockQueryDoctorRefundItems({ num: 'MR20260008' })
    )
    expect(refreshedRows[0]?.refundStatus).toBe('退费中')
    expect(refreshedRows[1]?.chargeStatus).toBe('未收费')
  })

  it('拒绝金额不一致的退费申请', async () => {
    const rows = await resolveMockRequest(mockQueryDoctorRefundItems({ num: 'MR20260003' }))
    const request = mockApplyDoctorRefund({
      patientNo: 'MR20260003',
      apply: [rows[0]!],
      applyNo: 1,
      applySum: 999
    })
    const rejection = expect(request).rejects.toThrow('退费合计金额不一致')
    await vi.runAllTimersAsync()
    await rejection
  })

  it('拒绝已进入退费流程的收费项重复申请', async () => {
    const rows = await resolveMockRequest(mockQueryDoctorRefundItems({ num: 'MR20260005' }))
    const request = mockApplyDoctorRefund({
      patientNo: 'MR20260005',
      apply: rows,
      applyNo: 1,
      applySum: 72
    })
    const rejection = expect(request).rejects.toThrow('当前不可申请退费')
    await vi.runAllTimersAsync()
    await rejection
  })
})
