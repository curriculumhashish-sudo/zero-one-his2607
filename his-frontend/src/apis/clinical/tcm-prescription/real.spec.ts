import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const put = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, put })
}))

import {
  realDeleteTcmPrescription,
  realListTcmPrescriptions,
  realPageTcmHerbCatalog,
  realSaveTcmPrescriptionDraft,
  realSignTcmPrescription,
  realWithdrawTcmPrescription
} from './real'

const query = {
  scope: 'outpatient' as const,
  encounterId: 'enc-1',
  patientId: 'patient-1'
}

describe('tcm prescription real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('loads prescription summaries and details from the opdoctor routes', async () => {
    get
      .mockResolvedValueOnce({ data: { records: [{ prescriptionNo: 'RX-001' }] } })
      .mockResolvedValueOnce({
        data: {
          prescriptionId: 'RX-001',
          encounterId: 'enc-1',
          patientId: 'patient-1',
          diagnosis: 'diagnosis-1',
          diagnosisName: '气虚证',
          status: 'ACTIVE',
          usage: '水煎服',
          frequency: '每日一剂',
          days: 7,
          copies: 7,
          decoction: true,
          version: 2,
          details: [
            {
              medicineId: 'herb-1',
              medicineCode: 'H001',
              name: '黄芪',
              quantity: 15,
              unit: 'g',
              unitPrice: 0.18,
              sortNumber: 1
            }
          ]
        }
      })

    const result = await realListTcmPrescriptions(query)

    expect(get).toHaveBeenNthCalledWith(
      1,
      '/chinapres',
      { patientId: 'patient-1', encounterId: 'enc-1' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(get).toHaveBeenNthCalledWith(
      2,
      '/chinapres-detail',
      { patientId: 'patient-1', presId: 'RX-001', encounterId: 'enc-1' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result[0]).toEqual(
      expect.objectContaining({
        id: 'RX-001',
        encounterDiagnosisId: 'diagnosis-1',
        status: 'SIGNED',
        doseCount: 7
      })
    )
  })

  it('uses the real herb catalog endpoint', async () => {
    post.mockResolvedValue({ data: { pageIndex: 1, pageSize: 20, total: 0, rows: [] } })
    await realPageTcmHerbCatalog({ scope: 'outpatient', pageIndex: 1, pageSize: 20 })
    expect(post).toHaveBeenCalledWith(
      '/tcm-prescription/page-herb-catalog',
      { scope: 'outpatient', pageIndex: 1, pageSize: 20 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('maps add and update requests to the backend DTO', async () => {
    const data = {
      ...query,
      encounterDiagnosisId: 'diagnosis-1',
      diagnosisName: '气虚证',
      copies: 5,
      usage: '水煎服',
      frequency: '每日一剂',
      days: 5,
      decoction: 1,
      details: [{ medicineId: 'herb-1', quantity: 15, unit: 'g', sortNo: 1 }]
    }
    post.mockResolvedValue({ id: 'request-1', prescriptionNo: 'RX-001', status: 'UNSIGNED' })
    put.mockResolvedValue({ id: 'request-1', prescriptionNo: 'RX-001', status: 'UNSIGNED' })

    await realSaveTcmPrescriptionDraft(data)
    await realSaveTcmPrescriptionDraft({ ...data, id: 'request-1', version: 2 })

    expect(post).toHaveBeenCalledWith(
      '/chinapres2/add',
      expect.objectContaining({
        encounterDiagnosisId: 'diagnosis-1',
        copies: 5,
        usage: '水煎服',
        details: [{ medicineId: 'herb-1', quantity: 15, unit: 'g', sortNo: 1 }]
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(put).toHaveBeenCalledWith(
      '/chinapres2/update',
      expect.objectContaining({ id: 'request-1', version: 2 }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('uses only backend-supported delete, issue, and revoke fields', async () => {
    post.mockResolvedValue({ data: {} })
    const action = {
      scope: 'outpatient' as const,
      encounterId: 'enc-1',
      id: 'request-1',
      version: 2
    }

    await realDeleteTcmPrescription(action)
    await realSignTcmPrescription(action)
    await realWithdrawTcmPrescription(action)

    expect(post).toHaveBeenNthCalledWith(
      1,
      '/chinapres2/remove',
      { id: 'request-1', encounterId: 'enc-1', version: 2 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(post).toHaveBeenNthCalledWith(
      2,
      '/chinapres3/issue',
      { encounterId: 'enc-1', signTargets: [{ medicationRequestId: 'request-1' }] },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(post).toHaveBeenNthCalledWith(
      3,
      '/chinapres3/revoke',
      { medicationRequestIds: ['request-1'] },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })
})
