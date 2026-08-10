import { describe, expect, it } from 'vitest'

import {
  mockDeleteTcmPrescription,
  mockListTcmPrescriptionHistory,
  mockListTcmPrescriptions,
  mockPageTcmHerbCatalog,
  mockSaveTcmPrescriptionDraft,
  mockSignTcmPrescription,
  mockWithdrawTcmPrescription
} from './mock'

const scope = 'outpatient' as const

describe('tcm prescription mock api', () => {
  it('pages the herb catalog', async () => {
    const result = await mockPageTcmHerbCatalog({
      scope,
      pageIndex: 1,
      pageSize: 2,
      keyword: '黄芪'
    })
    expect(result.total).toBe(1)
    expect(result.rows?.[0]).toEqual(expect.objectContaining({ id: 'herb-001', name: '黄芪' }))
  })

  it('saves backend-aligned prescription fields', async () => {
    const encounterId = `enc-mock-${Date.now()}`
    const saved = await mockSaveTcmPrescriptionDraft({
      scope,
      encounterId,
      patientId: 'patient-mock',
      encounterDiagnosisId: 'diagnosis-mock',
      diagnosisName: '气虚证',
      copies: 5,
      usage: '水煎服',
      frequency: '每日一剂',
      days: 5,
      decoction: 1,
      details: [
        {
          medicineId: 'herb-001',
          quantity: 15,
          unit: 'g',
          dose: 15,
          doseUnit: 'g',
          sortNo: 1
        }
      ]
    })

    expect(saved).toEqual(
      expect.objectContaining({
        encounterDiagnosisId: 'diagnosis-mock',
        syndromeName: '气虚证',
        doseCount: 5,
        method: '水煎服',
        decoctionMethod: '代煎',
        status: 'DRAFT'
      })
    )
    expect(saved.lines[0]).toEqual(expect.objectContaining({ catalogId: 'herb-001', dose: 15 }))

    const listed = await mockListTcmPrescriptions({ scope, encounterId, patientId: 'patient-mock' })
    expect(listed).toHaveLength(1)
  })

  it('supports delete, issue, and revoke without unsupported lifecycle fields', async () => {
    const encounterId = `enc-actions-${Date.now()}`
    const draft = await mockSaveTcmPrescriptionDraft({
      scope,
      encounterId,
      patientId: 'patient-actions',
      encounterDiagnosisId: 'diagnosis-actions',
      diagnosisName: '脾气虚证',
      copies: 3,
      usage: '水煎服',
      frequency: '每日一剂',
      days: 3,
      details: [{ medicineId: 'herb-002', quantity: 12, unit: 'g', sortNo: 1 }]
    })
    const action = { scope, encounterId, id: draft.id, version: draft.version }

    const signed = await mockSignTcmPrescription(action)
    expect(signed.status).toBe('SIGNED')

    const withdrawn = await mockWithdrawTcmPrescription({ ...action, version: signed.version })
    expect(withdrawn.status).toBe('DRAFT')

    await mockDeleteTcmPrescription({ ...action, version: withdrawn.version })
    await expect(
      mockListTcmPrescriptions({ scope, encounterId, patientId: 'patient-actions' })
    ).resolves.toEqual([])
  })

  it('filters history by scope and current encounter', async () => {
    const history = await mockListTcmPrescriptionHistory({
      scope,
      patientId: 'patient-demo-001',
      currentEncounterId: 'enc-demo-in-progress'
    })
    expect(history.every((item) => item.encounterId !== 'enc-demo-in-progress')).toBe(true)
  })
})
