import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  mockCancelStopTcmPrescription,
  mockDeleteTcmPrescription,
  mockListTcmPrescriptionHistory,
  mockListTcmPrescriptions,
  mockPageTcmHerbCatalog,
  mockSaveTcmPrescriptionDraft,
  mockSignTcmPrescription,
  mockStopTcmPrescription,
  mockWithdrawTcmPrescription
} from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

async function expectMockRequestToReject(request: Promise<unknown>, message: string) {
  const expectation = expect(request).rejects.toThrow(message)
  await vi.runAllTimersAsync()
  await expectation
}

describe('clinical tcm prescription mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('为门诊和住院共用同一套饮片目录查询', async () => {
    const inpatient = await resolveMockRequest(
      mockPageTcmHerbCatalog({
        scope: 'inpatient',
        pageIndex: 1,
        pageSize: 10,
        keyword: '黄芪'
      })
    )
    const outpatient = await resolveMockRequest(
      mockPageTcmHerbCatalog({
        scope: 'outpatient',
        pageIndex: 1,
        pageSize: 10,
        keyword: '黄芪'
      })
    )

    expect(inpatient.rows).toEqual(outpatient.rows)
    expect(inpatient.rows?.[0]).toEqual(
      expect.objectContaining({ code: 'ZY0001', name: '黄芪', doseUnit: 'g' })
    )
  })

  it('门诊处方保存时不产生住院长临类型并支持签发、撤回', async () => {
    const saved = await resolveMockRequest(
      mockSaveTcmPrescriptionDraft({
        scope: 'outpatient',
        encounterId: 'enc-20260723-002',
        patientId: 'patient-1002',
        syndromeName: '脾气虚证',
        doseCount: 5,
        method: '水煎服',
        frequency: '每日一剂',
        days: 5,
        lines: [
          {
            catalogId: 'herb-002',
            code: 'ZY0002',
            name: '党参',
            specification: '饮片',
            dose: 12,
            doseUnit: 'g',
            retailPrice: 0.22,
            orderNo: 1
          }
        ],
        version: 0
      })
    )
    expect(saved.therapyType).toBeUndefined()
    expect(saved.status).toBe('DRAFT')

    const signed = await resolveMockRequest(
      mockSignTcmPrescription({
        scope: saved.scope,
        encounterId: saved.encounterId,
        patientId: saved.patientId,
        id: saved.id,
        version: saved.version
      })
    )
    expect(signed.status).toBe('SIGNED')
    expect(signed.prescriptionNo).toMatch(/^MZCF/)

    const withdrawn = await resolveMockRequest(
      mockWithdrawTcmPrescription({
        scope: signed.scope,
        encounterId: signed.encounterId,
        patientId: signed.patientId,
        id: signed.id,
        version: signed.version
      })
    )
    expect(withdrawn.status).toBe('WITHDRAWN')
  })

  it('住院长期处方支持签发、停嘱和取消停嘱', async () => {
    const saved = await resolveMockRequest(
      mockSaveTcmPrescriptionDraft({
        scope: 'inpatient',
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002',
        therapyType: 'LONG_TERM',
        syndromeName: '气血两虚证',
        doseCount: 7,
        method: '水煎服',
        frequency: '每日一剂',
        days: 7,
        lines: [
          {
            catalogId: 'herb-001',
            code: 'ZY0001',
            name: '黄芪',
            specification: '饮片',
            dose: 15,
            doseUnit: 'g',
            retailPrice: 0.18,
            orderNo: 1
          }
        ],
        version: 0
      })
    )
    const signed = await resolveMockRequest(
      mockSignTcmPrescription({
        scope: saved.scope,
        encounterId: saved.encounterId,
        patientId: saved.patientId,
        id: saved.id,
        version: saved.version
      })
    )
    const stopped = await resolveMockRequest(
      mockStopTcmPrescription({
        scope: signed.scope,
        encounterId: signed.encounterId,
        patientId: signed.patientId,
        id: signed.id,
        version: signed.version
      })
    )
    expect(stopped.status).toBe('STOPPED')

    const resumed = await resolveMockRequest(
      mockCancelStopTcmPrescription({
        scope: stopped.scope,
        encounterId: stopped.encounterId,
        patientId: stopped.patientId,
        id: stopped.id,
        version: stopped.version
      })
    )
    expect(resumed.status).toBe('SIGNED')
  })

  it('门诊处方和住院临时处方不能使用停嘱接口', async () => {
    const outpatient = (
      await resolveMockRequest(
        mockListTcmPrescriptions({
          scope: 'outpatient',
          encounterId: 'enc-20260723-003',
          patientId: 'patient-1003'
        })
      )
    )[0]!
    await expectMockRequestToReject(
      mockStopTcmPrescription({
        scope: outpatient.scope,
        encounterId: outpatient.encounterId,
        patientId: outpatient.patientId,
        id: outpatient.id,
        version: outpatient.version
      }),
      '只有住院长期'
    )
  })

  it('支持查询跨就诊历史处方并删除未签发草稿', async () => {
    const history = await resolveMockRequest(
      mockListTcmPrescriptionHistory({
        patientId: 'patient-10001',
        currentEncounterId: 'ipt-enc-20260701-001',
        keyword: '黄芪'
      })
    )
    expect(history[0]).toEqual(
      expect.objectContaining({
        prescriptionNo: 'MZCF20260112008',
        herbNames: expect.arrayContaining(['黄芪'])
      })
    )

    const saved = await resolveMockRequest(
      mockSaveTcmPrescriptionDraft({
        scope: 'inpatient',
        encounterId: 'ipt-enc-20260703-003',
        patientId: 'patient-10003',
        therapyType: 'TEMPORARY',
        syndromeName: '痰湿证',
        doseCount: 3,
        method: '水煎服',
        frequency: '每日一剂',
        days: 3,
        lines: [
          {
            catalogId: 'herb-004',
            code: 'ZY0004',
            name: '茯苓',
            specification: '饮片',
            dose: 12,
            doseUnit: 'g',
            retailPrice: 0.14,
            orderNo: 1
          }
        ],
        version: 0
      })
    )
    await resolveMockRequest(
      mockDeleteTcmPrescription({
        scope: saved.scope,
        encounterId: saved.encounterId,
        patientId: saved.patientId,
        id: saved.id,
        version: saved.version
      })
    )
    const rows = await resolveMockRequest(
      mockListTcmPrescriptions({
        scope: saved.scope,
        encounterId: saved.encounterId,
        patientId: saved.patientId
      })
    )
    expect(rows.some((item) => item.id === saved.id)).toBe(false)
  })
})
