import { describe, expect, it, vi } from 'vitest'

const get = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get })
}))

vi.stubEnv('VITE_OUTPATIENT_DOCTOR_STATION_API_MODE', 'real')

describe('real outpatient prescription query', () => {
  it('translates the UI scope and passes the encounter identifier', async () => {
    const { queryDoctorPrescriptionSheets } = await import('./index')
    get.mockResolvedValueOnce({
      code: 10000,
      data: [
        {
          patientName: 'Alice',
          encounterId: 'encounter-1',
          prescriptionNo: 'RX-1',
          costType: 'WESTERN',
          totalPrice: '12.5'
        }
      ]
    })

    const result = await queryDoctorPrescriptionSheets({
      num: 'MR-1',
      encounterId: 'encounter-1',
      paymentTypeName: '医保',
      type: 'CURRENT_ENCOUNTER'
    })

    expect(get).toHaveBeenCalledWith(
      '/c2-opdoctor/prescription_query',
      { num: 'MR-1', encounterId: 'encounter-1', type: 'today', page: 1, pageSize: 100 },
      expect.anything()
    )
    expect(result[0]).toMatchObject({
      prescriptionNo: 'RX-1',
      costType: '医保',
      totalPrice: 12.5
    })
  })

  it('throws backend errors', async () => {
    const { queryDoctorPrescriptionSheets } = await import('./index')
    get.mockResolvedValueOnce({ code: 10002, message: 'Query failed', data: null })
    await expect(queryDoctorPrescriptionSheets({ num: 'MR-1', type: 'HISTORY' })).rejects.toThrow(
      'Query failed'
    )
  })
})
