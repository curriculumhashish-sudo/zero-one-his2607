import { describe, expect, it } from 'vitest'

import { queryNurseStationTemperatureSheet, saveNurseStationTemperatureSheet } from './index'

describe('mock temperature sheet API', () => {
  it('returns the requested number of days and six time slots per day', async () => {
    const result = await queryNurseStationTemperatureSheet({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      windowRange: '7d',
      focusRecordDate: '2026-08-01'
    })

    expect(result.gridDays).toHaveLength(7)
    expect(result.gridDays.every((day) => day.slots.length === 6)).toBe(true)
    expect(result.gridDays.at(-1)?.recordDate).toBe('2026-08-01')
  })

  it('saves a slot and returns it on the next query', async () => {
    const request = {
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
      ]
    }

    const saved = await saveNurseStationTemperatureSheet(request)
    const refreshed = await queryNurseStationTemperatureSheet({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      windowRange: '7d',
      focusRecordDate: '2026-08-01'
    })

    expect(saved).toEqual(expect.objectContaining({ success: true, savedCount: 1 }))
    expect(refreshed.gridDays.at(-1)?.slots.find((slot) => slot.timeSlot === '14')).toEqual(
      expect.objectContaining({ temperature: '36.9', pulse: '80' })
    )
  })

  it('passes special events through the frontend aggregation', async () => {
    const result = await saveNurseStationTemperatureSheet({
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

    expect(result).toEqual(expect.objectContaining({ success: true, savedCount: 1 }))
  })
})
