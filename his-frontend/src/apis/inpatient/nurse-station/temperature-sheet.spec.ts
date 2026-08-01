import { describe, expect, it } from 'vitest'

import { queryNurseStationTemperatureSheet, saveNurseStationTemperatureSheet } from './index'

describe('住院护士站体温单 Mock API', () => {
  it('返回七天、每天六时次的完整纸质单骨架', async () => {
    const result = await queryNurseStationTemperatureSheet({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      windowRange: '7d',
      focusRecordDate: '2026-08-01'
    })

    expect(result.paperHeader).toEqual(
      expect.objectContaining({
        patientName: '孙晓梅',
        bedName: '01床',
        encounterBusNo: 'ZY202607210001'
      })
    )
    expect(result.gridDays).toHaveLength(7)
    expect(result.gridDays.every((day) => day.slots.length === 6)).toBe(true)
    expect(result.gridDays.at(-1)?.recordDate).toBe('2026-08-01')
    // 本项目已明确取消皮试管理，因此体温单日汇总契约中也不能重新出现皮试字段。
    expect(result.gridDays.at(-1)?.dailySupplement).not.toHaveProperty('skinTestPositive')
  })

  it('批量保存新时次支持幂等重试，并可在查询结果中读取', async () => {
    const request = {
      idempotencyKey: 'temperature-save-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordDate: '2026-08-01',
      slots: [
        {
          timeSlot: '14' as const,
          temperature: '36.9',
          pulse: '80',
          respiratory: '18',
          specialEvents: []
        }
      ],
      dailySupplement: { urine: '1500', intake: '1850' }
    }

    const first = await saveNurseStationTemperatureSheet(request)
    const repeated = await saveNurseStationTemperatureSheet(request)
    const refreshed = await queryNurseStationTemperatureSheet({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      windowRange: '7d',
      focusRecordDate: '2026-08-01'
    })

    expect(first).toEqual(expect.objectContaining({ success: true, savedCount: 1 }))
    expect(repeated).toEqual(first)
    expect(refreshed.gridDays.at(-1)?.slots.find((slot) => slot.timeSlot === '14')).toEqual(
      expect.objectContaining({ temperature: '36.9', pulse: '80', version: 1 })
    )
    expect(refreshed.gridDays.at(-1)?.dailySupplement).toEqual(
      expect.objectContaining({ urine: '1500', intake: '1850' })
    )
  })

  it('更新已有时次时拒绝过期版本', async () => {
    const result = await saveNurseStationTemperatureSheet({
      idempotencyKey: 'temperature-stale-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordDate: '2026-08-01',
      slots: [
        {
          timeSlot: '06',
          vitalSignId: 'temperature-005',
          expectedVersion: 99,
          temperature: '37.0',
          specialEvents: []
        }
      ]
    })

    expect(result).toEqual(expect.objectContaining({ success: false, savedCount: 0 }))
    expect(result.results[0]).toEqual(
      expect.objectContaining({ success: false, message: '该时次已被他人修改' })
    )
  })

  it('转科等特殊事件缺少发生时间时拒绝保存', async () => {
    const result = await saveNurseStationTemperatureSheet({
      idempotencyKey: 'temperature-event-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordDate: '2026-08-01',
      slots: [
        {
          timeSlot: '18',
          specialEvents: [{ code: 'TRANSFER', displayText: '转科' }]
        }
      ]
    })

    expect(result).toEqual(expect.objectContaining({ success: false, savedCount: 0 }))
    expect(result.results[0]?.message).toBe('特殊事件缺少发生时间')
  })
})
