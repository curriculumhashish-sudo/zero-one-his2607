import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  createDiagnosis,
  deleteDiagnosis,
  getDiagnoses,
  pageDiagnosisUsageHistory,
  pagePatientDiagnosisHistory
} from './index'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

async function rejectMockRequest(request: Promise<unknown>) {
  let capturedError: unknown
  const handledRequest = request.catch((error) => {
    capturedError = error
  })
  await vi.runAllTimersAsync()
  await handledRequest
  return capturedError
}

describe('doctor-station diagnosis API', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('分页查询诊断使用历史', async () => {
    const result = await resolveMockRequest(
      pageDiagnosisUsageHistory({
        patientId: 'patient-1003',
        pageIndex: 1,
        pageSize: 2
      })
    )

    expect(result.total).toBeGreaterThan(0)
    expect(result.rows).toHaveLength(Math.min(2, result.total))
  })

  it('按患者、关键词和主次条件分页查询历史诊断', async () => {
    const result = await resolveMockRequest(
      pagePatientDiagnosisHistory({
        patientId: 'patient-1003',
        keyword: '心血管',
        pageIndex: 1,
        pageSize: 5
      })
    )

    expect(result.total).toBeGreaterThan(0)
    expect(result.rows?.every((item) => Boolean(item.encounterId))).toBe(true)
    expect(result.rows?.some((item) => item.encounterId === 'enc-20260723-003')).toBe(false)
  })

  it('独立新增和删除诊断，并在删除主诊断后自动补位', async () => {
    const encounterId = 'enc-extra-14'
    await resolveMockRequest(getDiagnoses(encounterId))

    const first = await resolveMockRequest(
      createDiagnosis({
        encounterId,
        conditionId: 'diag-hypertension',
        diagCode: 'I10',
        description: '原发性高血压',
        diagTypeCode: '西医诊断',
        primaryFlag: 0,
        rank: 1
      })
    )
    expect(first.rows).toHaveLength(1)
    expect(first.rows[0]?.primary).toBe(true)

    const second = await resolveMockRequest(
      createDiagnosis({
        encounterId,
        conditionId: 'diag-palpitations',
        diagCode: 'R00.2',
        description: '心悸',
        diagTypeCode: '症状诊断',
        primaryFlag: 0,
        rank: 2
      })
    )
    const deleted = await resolveMockRequest(
      deleteDiagnosis(second.rows.find((item) => item.primary)!.id)
    )

    expect(deleted.rows).toHaveLength(1)
    expect(deleted.rows[0]?.name).toBe('心悸')
    expect(deleted.rows[0]?.primary).toBe(true)
  })

  it('不允许删除最后一条诊断', async () => {
    const encounterId = 'enc-extra-17'
    await resolveMockRequest(getDiagnoses(encounterId))
    const current = await resolveMockRequest(
      createDiagnosis({
        encounterId,
        conditionId: 'diag-angina',
        diagCode: 'I20.9',
        description: '心绞痛',
        diagTypeCode: '西医诊断',
        primaryFlag: 1,
        rank: 1
      })
    )

    const deleteError = await rejectMockRequest(
      deleteDiagnosis(current.rows[0]!.id)
    )
    expect(deleteError).toEqual(
      expect.objectContaining({ message: expect.stringContaining('至少需要保留一条诊断') })
    )
  })
})
