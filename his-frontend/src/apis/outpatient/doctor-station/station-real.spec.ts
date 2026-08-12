import { describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const put = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, put })
}))

import { realAcceptPatient, realCompletePatient, realLeavePatient } from './station-real'

describe('outpatient doctor station patient transitions', () => {
  it('accepts a patient with encounterId and maps the context', async () => {
    post.mockResolvedValueOnce({
      code: 10000,
      data: {
        patientId: 'patient-1',
        encounterId: 'encounter-1',
        visitStatus: '在诊',
        phoneNum: '13800000000',
        attendingDoctorName: 'Doctor'
      }
    })

    await expect(realAcceptPatient('encounter-1')).resolves.toMatchObject({
      encounterId: 'encounter-1',
      patientId: 'patient-1',
      visitStatus: 'in-progress',
      phone: '13800000000'
    })
    expect(post).toHaveBeenCalledWith(
      '/c2-opdoctor/accept',
      { encounterId: 'encounter-1' },
      expect.anything()
    )
  })

  it('uses PUT transition payloads and maps leave/finish statuses', async () => {
    put.mockResolvedValueOnce({ code: 10000, data: { encounterId: 'encounter-1', visitStatus: 'arrived' } })
    put.mockResolvedValueOnce({ code: 10000, data: { encounterId: 'encounter-1', visitStatus: 'finished' } })

    await expect(realLeavePatient('encounter-1')).resolves.toMatchObject({
      encounterId: 'encounter-1',
      visitStatus: 'arrived'
    })
    await expect(realCompletePatient('encounter-1')).resolves.toMatchObject({
      encounterId: 'encounter-1',
      visitStatus: 'finished'
    })
    expect(put).toHaveBeenNthCalledWith(
      1,
      '/c2-opdoctor/leave',
      { encounterId: 'encounter-1' },
      expect.anything()
    )
    expect(put).toHaveBeenNthCalledWith(
      2,
      '/c2-opdoctor/finish',
      { encounterId: 'encounter-1' },
      expect.anything()
    )
  })

  it('surfaces backend business errors', async () => {
    post.mockResolvedValueOnce({ code: 10001, message: 'Patient not found', data: null })
    await expect(realAcceptPatient('missing')).rejects.toThrow('Patient not found')
  })
})
