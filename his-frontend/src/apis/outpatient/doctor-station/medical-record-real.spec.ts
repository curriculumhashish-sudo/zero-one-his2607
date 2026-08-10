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

describe('outpatient medical record real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('maps a detail response to the editor contract', async () => {
    get.mockResolvedValue({
      data: {
        id: 'record-1',
        encounterId: 'encounter-1',
        content: '<p>主诉</p>',
        updatedAt: '2026-08-10 09:00:00'
      }
    })

    const result = await realGetMedicalRecord('encounter-1')

    expect(get).toHaveBeenCalledWith(
      '/outpatientrecord2/detail/encounter-1',
      undefined,
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual({
      id: 'record-1',
      encounterId: 'encounter-1',
      content: '<p>主诉</p>',
      updatedAt: '2026-08-10 09:00:00'
    })
  })

  it('keeps the submitted content when save response only returns metadata', async () => {
    post.mockResolvedValue({
      data: {
        id: 'record-1',
        encounterId: 'encounter-1',
        updatedAt: '2026-08-10 09:05:00'
      }
    })

    const result = await realSaveMedicalRecord('encounter-1', { content: '<p>补充内容</p>' })

    expect(post).toHaveBeenCalledWith(
      '/outpatientrecord3/save',
      { encounterId: 'encounter-1', content: '<p>补充内容</p>' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.content).toBe('<p>补充内容</p>')
  })

  it('maps backend history summaries without frontend-only fields', async () => {
    get.mockResolvedValue({
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

    expect(get).toHaveBeenCalledWith(
      '/outpatientrecord2',
      { patientId: 'patient-1', keyword: '高血压', pageIndex: 1, pageSize: 10 },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
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
})
