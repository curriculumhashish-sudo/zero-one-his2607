import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import { mockQueryDoctorPrescriptionSheets } from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('doctor prescription sheet mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('按病历号查询本次处方单', async () => {
    const rows = await resolveMockRequest(
      mockQueryDoctorPrescriptionSheets({
        num: 'MR20260008',
        type: 'CURRENT_ENCOUNTER'
      })
    )
    expect(rows).toHaveLength(1)
    expect(rows[0]?.patientName).toBe('吴国平')
    expect(rows[0]?.prescriptionNo).toBe('RX-enc-20260723-008-1')
    expect(rows[0]?.items).toContain('阿司匹林肠溶片')
  })

  it('历史处方与本次处方相互隔离', async () => {
    const currentRows = await resolveMockRequest(
      mockQueryDoctorPrescriptionSheets({ num: 'MR20260003' })
    )
    const historyRows = await resolveMockRequest(
      mockQueryDoctorPrescriptionSheets({ num: 'MR20260003', type: 'HISTORY' })
    )
    expect(currentRows).toEqual([])
    expect(historyRows).toHaveLength(2)
    expect(historyRows.every((row) => row.prescriptionNo?.startsWith('RX-HISTORY'))).toBe(true)
  })

  it('返回副本，页面修改不会污染后续查询', async () => {
    const first = await resolveMockRequest(mockQueryDoctorPrescriptionSheets({ num: 'MR20260008' }))
    first[0]!.patientName = '被页面修改'

    const second = await resolveMockRequest(
      mockQueryDoctorPrescriptionSheets({ num: 'MR20260008' })
    )
    expect(second[0]?.patientName).toBe('吴国平')
  })

  it('缺少病历号时拒绝查询', async () => {
    const request = mockQueryDoctorPrescriptionSheets({})
    const rejection = expect(request).rejects.toThrow('缺少患者病历号')
    await vi.runAllTimersAsync()
    await rejection
  })
})
