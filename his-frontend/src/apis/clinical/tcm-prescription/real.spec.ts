import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const del = vi.fn()
const userStore = { tenantId: 'tenant-1' as string | number | null }

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, delete: del })
}))

vi.mock('@/stores/user', () => ({
  useUserStore: () => userStore
}))

import {
  realDeleteTcmPrescription,
  realListTcmPrescriptions,
  realPageTcmHerbCatalog,
  realSaveTcmPrescriptionDraft,
  realSignTcmPrescription,
  realWithdrawTcmPrescription,
  realStopTcmPrescription,
  realCancelStopTcmPrescription
} from './real'

const query = {
  scope: 'inpatient' as const,
  encounterId: 'enc-1',
  patientId: 'patient-1'
}

describe('tcm prescription real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    userStore.tenantId = 'tenant-1'
  })

  it('loads current prescriptions from the backend list endpoint', async () => {
    get.mockResolvedValueOnce({
      data: {
        prescriptions: [
          {
            id: 'RX-001',
            prescriptionGroupKey: 'group-1',
            statusEnum: 'COMPLETED',
            days: 7,
            chineseHerbsDoseQuantity: 7,
            sufferingFlag: 1,
            description: '气虚证',
            lines: [
              {
                medicationRequestId: 'request-1',
                medicationDefinitionId: 'herb-1',
                medicationName: '黄芪',
                dose: 15,
                unitName: 'g'
              }
            ]
          }
        ]
      }
    })

    const result = await realListTcmPrescriptions(query)

    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/chinapres',
      { encounterId: 'enc-1', patientId: 'patient-1' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result[0]).toEqual(
      expect.objectContaining({
        id: 'RX-001',
        prescriptionGroupKey: 'group-1',
        status: 'COMPLETED',
        doseCount: 7
      })
    )
    expect(result[0]?.lines[0]).toEqual(
      expect.objectContaining({
        medicationRequestId: 'request-1',
        medicationDefinitionId: 'herb-1'
      })
    )
  })

  it('loads the minimal herb catalog endpoint', async () => {
    post.mockResolvedValue({ data: { pageIndex: 1, pageSize: 20, total: 1, rows: [] } })
    await realPageTcmHerbCatalog({ scope: 'inpatient', pageIndex: 1, pageSize: 20 })
    expect(post).toHaveBeenCalledWith(
      '/tcm-prescription/page-herb-catalog',
      { scope: 'inpatient', keyword: undefined, pageIndex: 1, pageSize: 20 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('maps a new prescription to the backend add DTO', async () => {
    post.mockResolvedValue({ data: { success: true, newMedicationRequestIds: ['request-1'] } })
    const data = {
      ...query,
      prescriptionGroupKey: 'group-1',
      therapyEnum: 'TEMPORARY',
      chineseHerbsDoseQuantity: 5,
      sufferingFlag: true,
      days: 5,
      details: [{ medicineId: 'herb-1', quantity: 15, doseUnitCode: 'g' }]
    }

    await realSaveTcmPrescriptionDraft(data)

    expect(post).toHaveBeenCalledWith(
      '/c2-opdoctor/chinapres2/add',
      {
        encounterId: 'enc-1',
        patientId: 'patient-1',
        encounterDiagnosisId: '',
        usage: '',
        frequency: '',
        days: 5,
        copies: 5,
        decoction: 1,
        remark: '',
        details: [{ medicineId: 'herb-1', quantity: 15, dose: undefined, doseUnit: 'g', unit: undefined, sortNo: 1 }]
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('uses the backend delete, sign, and withdraw contracts', async () => {
    del.mockResolvedValue({ data: {} })
    post.mockResolvedValue({ data: { success: true } })
    const action = {
      ...query,
      patientId: 'patient-1',
      id: 'RX-001',
      prescriptionNo: 'RX-001',
      prescriptionGroupKey: 'group-1',
      medicationRequestIds: ['request-1']
    }

    await realDeleteTcmPrescription(action)
    post.mockResolvedValueOnce({
      data: {
        success: true,
        prescriptionNoByGroupKey: { 'group-1': 'CP202608120001' }
      }
    })
    const signed = await realSignTcmPrescription(action)
    await realWithdrawTcmPrescription(action)
    expect(signed.prescriptionNo).toBe('CP202608120001')

    expect(post).toHaveBeenNthCalledWith(
      1,
      '/c2-opdoctor/chinapres2/remove',
      {
        id: 'RX-001',
        encounterId: 'enc-1',
        version: 0
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(post).toHaveBeenNthCalledWith(
      2,
      '/c2-opdoctor/chinapres3/issue',
      expect.objectContaining({
        encounterId: 'enc-1',
        signTargets: [{ medicationRequestId: 'RX-001' }]
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(post).toHaveBeenNthCalledWith(
      3,
      '/c2-opdoctor/chinapres3/revoke',
      { medicationRequestIds: ['RX-001'] },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('supports stopping and canceling a long-term prescription', async () => {
    const action = {
      ...query,
      patientId: 'patient-1',
      prescriptionNo: 'CP202608120001'
    }

    await expect(realStopTcmPrescription(action)).rejects.toThrow('后端暂未提供中医处方停用接口')
    await expect(realCancelStopTcmPrescription(action)).rejects.toThrow('后端暂未提供中医处方取消停用接口')
  })
})
