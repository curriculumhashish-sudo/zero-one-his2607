import { describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const del = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, delete: del })
}))

import {
  realCreateDiagnosis,
  realDeleteDiagnosis,
  realGetDiagnoses,
  realListDoctorPatientQueue,
  realPagePatientDiagnosisHistory
} from './queue-diagnosis-real'

describe('outpatient doctor station real queue and diagnosis APIs', () => {
  it('maps the backend patient queue response and uses the backend query names', async () => {
    get.mockResolvedValueOnce({
      data: [
        {
          patientId: 'patient-1',
          encounterId: 'encounter-1',
          patientName: 'Alice',
          age: 36,
          visitStatus: '在诊',
          payType: '医保'
        }
      ]
    })

    const result = await realListDoctorPatientQueue({
      visitDate: '2026-08-12',
      patientName: 'Alice',
      visitStatus: 'in-progress'
    })

    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/queue',
      { date: '2026-08-12', patientName: 'Alice', visitStatus: 'in-progress' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result[0]).toMatchObject({
      patientId: 'patient-1',
      encounterId: 'encounter-1',
      visitStatus: 'in-progress',
      paymentTypeName: '医保'
    })
  })

  it('loads diagnoses by patient and keeps only the selected encounter', async () => {
    get.mockResolvedValueOnce({
      data: {
        pageIndex: 1,
        pageSize: 1000,
        total: 2,
        rows: [
          {
            id: 'diagnosis-1',
            encounterId: 'encounter-1',
            conditionId: 'condition-1',
            diagCode: 100,
            description: 'Hypertension',
            diagTypeCode: 'WESTERN',
            primaryFlag: 1,
            rank: 1,
            createdAt: '2026-08-12 10:00:00'
          },
          { id: 'diagnosis-2', encounterId: 'encounter-2', description: 'Other' }
        ]
      }
    })

    const result = await realGetDiagnoses('encounter-1', 'patient-1')

    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/diagnose/patient',
      { patientId: 'patient-1', pageIndex: 1, pageSize: 1000 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.rows).toHaveLength(1)
    expect(result.rows[0]).toMatchObject({
      id: 'diagnosis-1',
      catalogId: 'condition-1',
      code: '100',
      primary: true
    })
  })

  it('creates a diagnosis through the backend and refreshes the selected encounter', async () => {
    post.mockResolvedValueOnce({ data: { id: 'diagnosis-3', encounterId: 'encounter-3' } })
    get.mockResolvedValueOnce({ data: { rows: [] } })

    const result = await realCreateDiagnosis({
      encounterId: 'encounter-3',
      conditionId: 'condition-3',
      diagCode: '300',
      description: 'Diabetes',
      diagTypeCode: 'WESTERN',
      primaryFlag: 1,
      rank: 1
    }, 'patient-3')

    expect(post).toHaveBeenCalledWith(
      '/c2-opdoctor/diagnose/add',
      expect.objectContaining({ encounterId: 'encounter-3', diagCode: 300 }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual({ encounterId: 'encounter-3', rows: [], updatedAt: undefined })
  })

  it('rejects non-numeric ICD codes before sending an incompatible backend payload', async () => {
    const callCount = post.mock.calls.length
    await expect(
      realCreateDiagnosis(
        {
          encounterId: 'encounter-4',
          conditionId: 'condition-4',
          diagCode: 'I10',
          description: 'Hypertension',
          diagTypeCode: 'WESTERN',
          primaryFlag: 1,
          rank: 1
        },
        'patient-4'
      )
    ).rejects.toThrow('后端暂不支持字符串诊断编码')
    expect(post).toHaveBeenCalledTimes(callCount)
  })

  it('deletes a diagnosis and reloads the selected patient diagnoses', async () => {
    del.mockResolvedValueOnce({ data: { encounterId: 'encounter-5' } })
    get.mockResolvedValueOnce({ data: { rows: [] } })

    await realDeleteDiagnosis('diagnosis-5', 'patient-5')

    expect(del).toHaveBeenCalledWith(
      '/c2-opdoctor/diagnose/delete/diagnosis-5',
      undefined,
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/diagnose/patient',
      { patientId: 'patient-5', pageIndex: 1, pageSize: 1000 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('passes patient diagnosis history filters to the backend', async () => {
    get.mockResolvedValueOnce({ data: { pageIndex: 1, pageSize: 10, total: 0, rows: [] } })

    await realPagePatientDiagnosisHistory({
      patientId: 'patient-1',
      keyword: 'hypertension',
      startDate: '2026-08-01',
      endDate: '2026-08-12',
      pageIndex: 1,
      pageSize: 10
    })

    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/diagnose/patient',
      expect.objectContaining({
        patientId: 'patient-1',
        keyword: '%hypertension%',
        startDate: '2026-08-01',
        endDate: '2026-08-12'
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })
})
