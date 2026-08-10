import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realQueryNurseStationTemperatureSheet,
  realSaveNurseStationTemperatureSheet
} from './temperature-sheet-real'

const query = {
  encounterId: 'encounter-001',
  patientId: 'patient-001',
  windowRange: '3d' as const,
  focusRecordDate: '2026-08-08',
  pageIndex: 0,
  patientContext: { patientName: '孙晓梅', bedName: '01床' }
}

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站体温单真实接口适配', () => {
  it('将旧接口 gridRows/detailItems 补齐为日期和六时次网格', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        gridRows: [
          {
            recordDate: '2026-08-08',
            slotCells: [
              {
                timeSlot: '10',
                vitalSignId: 'temp-001',
                detailItems: [
                  { itemCode: 'TEMPERATURE', itemValue: '36.8' },
                  { itemCode: 'PULSE', itemValue: '80' },
                  { itemCode: 'HEART_RATE', itemValue: '80' }
                ]
              }
            ]
          }
        ],
        pagination: { dateFrom: '2026-08-06', dateTo: '2026-08-08', pageIndex: 0, totalPages: 2 }
      }
    })

    const result = await realQueryNurseStationTemperatureSheet(query)

    expect(httpMock.get).toHaveBeenCalledWith(
      '/temperature/get-data',
      {
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        windowRange: '3d',
        focusRecordDate: '2026-08-08',
        pageIndex: 0
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.paperHeader).toEqual(expect.objectContaining({ patientName: '孙晓梅' }))
    expect(result.gridDays).toHaveLength(3)
    expect(result.gridDays.every((day) => day.slots).valueOf()).toBeTruthy()
    expect(result.gridDays.at(-1)?.slots.find((slot) => slot.timeSlot === '10')).toEqual(
      expect.objectContaining({ temperature: '36.8', pulse: '80', vitalSignId: 'temp-001' })
    )
  })

  it('把批量保存拆成后端支持的单时次请求，并返回逐条结果', async () => {
    httpMock.post
      .mockResolvedValueOnce({ data: { busNo: 'T001', vitalSignId: 'temp-001' } })
      .mockResolvedValueOnce({ data: { busNo: 'T002', vitalSignId: 'temp-002' } })

    const result = await realSaveNurseStationTemperatureSheet({
      idempotencyKey: 'local-only',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordDate: '2026-08-08',
      slots: [
        { timeSlot: '10', temperature: '36.8', pulse: '80', specialEvents: [] },
        { timeSlot: '14', temperature: '37.0', specialEvents: [] }
      ],
      dailySupplement: { urine: '1500' }
    })

    expect(httpMock.post).toHaveBeenCalledTimes(2)
    expect(httpMock.post.mock.calls[0]?.[0]).toBe('/temperature/save-record')
    expect(httpMock.post.mock.calls[0]?.[1]).toEqual(
      expect.objectContaining({
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        recordDate: '2026-08-08',
        timeSlot: '10',
        detailItems: expect.arrayContaining([
          { itemCode: 'TEMPERATURE', itemValue: '36.8' },
          { itemCode: 'PULSE', itemValue: '80' }
        ])
      })
    )
    expect(result).toEqual(
      expect.objectContaining({
        success: true,
        savedCount: 2,
        results: expect.arrayContaining([
          expect.objectContaining({ timeSlot: '10', success: true }),
          expect.objectContaining({ timeSlot: '14', success: true })
        ])
      })
    )
  })

  it('单时次请求失败时保留其他时次成功结果', async () => {
    httpMock.post
      .mockResolvedValueOnce({ data: { vitalSignId: 'temp-001' } })
      .mockRejectedValueOnce(new Error('版本冲突'))

    const result = await realSaveNurseStationTemperatureSheet({
      idempotencyKey: 'local-only-partial',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordDate: '2026-08-08',
      slots: [
        { timeSlot: '10', temperature: '36.8', specialEvents: [] },
        { timeSlot: '14', temperature: '37.0', specialEvents: [] }
      ]
    })

    expect(result.success).toBe(false)
    expect(result.savedCount).toBe(1)
    expect(result.results.find((item) => item.timeSlot === '14')).toEqual(
      expect.objectContaining({ success: false, message: '版本冲突' })
    )
  })
})
