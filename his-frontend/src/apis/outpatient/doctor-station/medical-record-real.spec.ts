import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post })
}))

import {
  realGetMedicalRecord,
  realPagePatientMedicalRecordHistory,
  realSaveMedicalRecord
} from './medical-record-real'

const jsonContent = JSON.stringify({
  type: 'doc',
  content: [{ type: 'paragraph', content: [{ type: 'text', text: '主诉' }] }]
})

describe('outpatient medical record real api', () => {
  beforeEach(() => vi.clearAllMocks())

  it('accepts backend code 0 for detail responses', async () => {
    get.mockResolvedValue({
      code: 0,
      message: 'success',
      data: {
        id: 'record-1',
        encounterId: 'encounter-1',
        content: '<p>主诉</p>',
        updatedAt: '2026-08-10 09:00:00'
      }
    })

    await expect(realGetMedicalRecord('encounter-1')).resolves.toMatchObject({
      id: 'record-1',
      encounterId: 'encounter-1',
      content: '<p>主诉</p>'
    })
  })

  it('rejects WangEditor HTML before sending to the JSON-only backend', async () => {
    await expect(
      realSaveMedicalRecord('encounter-1', { content: '<p>补充内容</p>' })
    ).rejects.toThrow('JSON')
    expect(post).not.toHaveBeenCalled()
  })

  it('accepts JSON content and keeps it when save returns metadata only', async () => {
    post.mockResolvedValue({
      code: 10000,
      message: 'success',
      data: {
        id: 'record-1',
        encounterId: 'encounter-1',
        updatedAt: '2026-08-10 09:05:00'
      }
    })

    const result = await realSaveMedicalRecord('encounter-1', { content: jsonContent })

    expect(post).toHaveBeenCalledWith(
      '/c2-opdoctor/outpatientrecord3/save',
      { encounterId: 'encounter-1', content: jsonContent },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.content).toBe(jsonContent)
  })

  it('maps code 0 history responses and keeps only backend fields', async () => {
    get.mockResolvedValue({
      code: 0,
      message: 'success',
      data: {
        pageIndex: 1,
        pageSize: 10,
        total: 1,
        rows: [
          {
            id: 'encounter-2',
            encounterId: 'encounter-2',
            visitDate: '2026-08-09 10:00:00',
            diagnosis: '高血压',
            doctorName: '李医生',
            deptName: '心血管内科'
          }
        ]
      }
    })

    const result = await realPagePatientMedicalRecordHistory({
      patientId: 'patient-1',
      keyword: '高血压',
      pageIndex: 1,
      pageSize: 10
    })

    expect(result.rows).toEqual([
      {
        encounterId: 'encounter-2',
        visitDate: '2026-08-09 10:00:00',
        diagnosis: '高血压',
        doctorName: '李医生',
        deptName: '心血管内科'
      }
    ])
  })

  it('slices an unpaged backend history response on page two', async () => {
    const rows = Array.from({ length: 12 }, (_, index) => ({
      encounterId: `encounter-${index + 1}`,
      visitDate: `2026-08-${String(index + 1).padStart(2, '0')}`
    }))
    get
      .mockResolvedValueOnce({
        code: 0,
        data: { pageIndex: 1, pageSize: 10, total: 12, rows }
      })
      .mockResolvedValueOnce({
        code: 0,
        data: { pageIndex: 2, pageSize: 10, total: 12, rows }
      })

    const firstPage = await realPagePatientMedicalRecordHistory({
      patientId: 'patient-1',
      pageIndex: 1,
      pageSize: 10
    })
    const secondPage = await realPagePatientMedicalRecordHistory({
      patientId: 'patient-1',
      pageIndex: 2,
      pageSize: 10
    })

    expect(firstPage.rows).toHaveLength(10)
    expect(secondPage.rows).toHaveLength(2)
    expect(secondPage.rows?.[0]?.encounterId).toBe('encounter-11')
  })
})
