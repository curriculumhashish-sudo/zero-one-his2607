import { afterEach, describe, expect, it, vi } from 'vitest'

async function loadMode() {
  vi.resetModules()
  return import('./mode')
}

describe('outpatient doctor-station API mode', () => {
  afterEach(() => {
    vi.unstubAllEnvs()
    vi.resetModules()
  })

  it('uses the shared real switch when configured', async () => {
    vi.stubEnv('VITE_OUTPATIENT_DOCTOR_STATION_API_MODE', 'real')
    vi.stubEnv('VITE_OUTPATIENT_DOCTOR_STATION_API_URL', '/doctor-station-api')

    const mode = await loadMode()

    expect(mode.outpatientDoctorStationApiMode).toBe('real')
    expect(mode.isOutpatientDoctorStationRealApiEnabled).toBe(true)
    expect(mode.isOutpatientDoctorStationMockApiEnabled).toBe(false)
    expect(mode.outpatientDoctorStationApiBaseURL).toBe('/doctor-station-api')
  })

  it('falls back safely to mock for missing or invalid values', async () => {
    vi.stubEnv('VITE_OUTPATIENT_DOCTOR_STATION_API_MODE', 'mixed')

    const mode = await loadMode()

    expect(mode.outpatientDoctorStationApiMode).toBe('mock')
    expect(mode.isOutpatientDoctorStationRealApiEnabled).toBe(false)
    expect(mode.isOutpatientDoctorStationMockApiEnabled).toBe(true)
  })
})

