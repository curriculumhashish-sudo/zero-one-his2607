import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realDeleteNurseStationNursingRecord,
  realQueryNurseStationNursingRecords,
  realSaveNurseStationNursingRecords
} from './nursing-record-real'

const query = {
  encounterId: 'encounter-001',
  patientId: 'patient-001',
  recordedAtFrom: '2026-08-03 00:00:00',
  recordedAtTo: '2026-08-09 23:59:59',
  sortAsc: true as const,
  patientContext: { patientName: '孙晓梅', bedName: '01床' }
}

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站护理记录真实接口适配', () => {
  it('按后端查询参数读取记录，并补齐旧接口缺失的患者抬头', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        paperHeader: {},
        rows: [
          {
            vitalSignId: 'vital-001',
            recordDate: '2026-08-08',
            recordTime: '08:30',
            temperature: '36.5',
            pulse: '80'
          }
        ]
      }
    })

    const result = await realQueryNurseStationNursingRecords(query)

    expect(httpMock.get).toHaveBeenCalledWith(
      '/nursing/list-records',
      {
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        recordedAtFrom: '2026-08-03 00:00:00',
        recordedAtTo: '2026-08-09 23:59:59',
        sortAsc: true
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.paperHeader).toEqual(expect.objectContaining({ patientName: '孙晓梅' }))
    expect(result.rows[0]).toEqual(
      expect.objectContaining({ vitalSignId: 'vital-001', temperature: '36.5', version: 0 })
    )
  })

  it('保存时只发送 Swagger 已声明的字段，并将整体响应包装为逐条结果', async () => {
    httpMock.post.mockResolvedValue({
      data: {
        savedCount: 1,
        rows: [
          {
            vitalSignId: 'vital-001',
            recordDate: '2026-08-08',
            recordTime: '08:30',
            temperature: '36.8',
            version: 2
          }
        ]
      }
    })

    const result = await realSaveNurseStationNursingRecords({
      idempotencyKey: 'local-only',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      records: [
        {
          clientRowId: 'client-001',
          vitalSignId: 'vital-001',
          expectedVersion: 1,
          recordDate: '2026-08-08',
          recordTime: '08:30',
          temperature: '36.8',
          nurseSignature: '不发送',
          rowType: 'extra'
        }
      ]
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/nursing/save-records',
      {
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        records: [
          expect.objectContaining({
            vitalSignId: 'vital-001',
            recordDate: '2026-08-08',
            recordTime: '08:30',
            temperature: '36.8'
          })
        ]
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    const body = httpMock.post.mock.calls[0]?.[1] as { records: Array<Record<string, unknown>> }
    expect(body.records[0]).not.toHaveProperty('clientRowId')
    expect(body.records[0]).not.toHaveProperty('expectedVersion')
    expect(body.records[0]).not.toHaveProperty('nurseSignature')
    expect(result).toEqual({
      success: true,
      savedCount: 1,
      results: [
        expect.objectContaining({
          clientRowId: 'client-001',
          success: true,
          row: expect.objectContaining({ vitalSignId: 'vital-001', version: 2 })
        })
      ]
    })
  })

  it('真实模式不伪造后端未提供的删除接口', async () => {
    await expect(
      realDeleteNurseStationNursingRecord({
        idempotencyKey: 'delete-001',
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        vitalSignId: 'vital-001',
        expectedVersion: 1
      })
    ).rejects.toThrow('暂未提供护理记录删除接口')
    expect(httpMock.post).not.toHaveBeenCalled()
  })
})
