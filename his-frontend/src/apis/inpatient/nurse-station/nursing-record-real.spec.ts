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
        paperHeader: {
          inpatientNo: 'ZY202608080001',
          name: '后端患者',
          gender: 'FEMALE',
          age: 42,
          admitAt: '2026-08-01 10:00:00',
          wardName: '内科一病区',
          bedNo: '02床'
        },
        rows: [
          {
            vitalSignId: 'vital-001',
            recordDate: '2026-08-08',
            recordTime: '08:30',
            temperature: '36.5',
            pulse: '80',
            respiration: '18',
            intakeVolume: '500',
            outputVolume: '300',
            outputColor: '淡黄',
            observation: '情况稳定'
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
    expect(result.paperHeader).toEqual(
      expect.objectContaining({
        patientName: '后端患者',
        bedName: '02床',
        encounterBusNo: 'ZY202608080001',
        admissionDate: '2026-08-01'
      })
    )
    expect(result.rows[0]).toEqual(
      expect.objectContaining({
        vitalSignId: 'vital-001',
        temperature: '36.5',
        respiration: '18',
        intakeVolume: '500',
        outputVolume: '300',
        outputColor: '淡黄',
        observation: '情况稳定',
        version: 0
      })
    )
  })

  it('保存时发送后端 DTO 字段，并将整体响应包装为逐条结果', async () => {
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
          nurseSignature: '护士甲'
        }
      ]
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/nursing/save-records',
      {
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        idempotencyKey: 'local-only',
        records: [
          expect.objectContaining({
            clientRowId: 'client-001',
            vitalSignId: 'vital-001',
            recordDate: '2026-08-08',
            recordTime: '08:30',
            temperature: '36.8',
            nurseSignature: '护士甲'
          })
        ]
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    const body = httpMock.post.mock.calls[0]?.[1] as { records: Array<Record<string, unknown>> }
    expect(body.records[0]).toHaveProperty('clientRowId', 'client-001')
    expect(body.records[0]).toHaveProperty('expectedVersion', 1)
    expect(body.records[0]).toHaveProperty('nurseSignature', '护士甲')
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

  it('调用后端删除护理记录接口', async () => {
    httpMock.post.mockResolvedValue({
      data: {
        success: true,
        vitalSignId: 'vital-001',
        statusEnum: 'DELETED'
      }
    })

    const result = await realDeleteNurseStationNursingRecord({
      idempotencyKey: 'delete-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      vitalSignId: 'vital-001',
      expectedVersion: 1
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/nursing/delete-record',
      expect.objectContaining({ vitalSignId: 'vital-001', expectedVersion: 1 }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual({
      success: true,
      vitalSignId: 'vital-001',
      message: '护理记录已删除'
    })
  })
})
