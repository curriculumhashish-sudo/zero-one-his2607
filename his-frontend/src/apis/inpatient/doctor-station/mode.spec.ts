import { afterEach, describe, expect, it, vi } from 'vitest'

async function loadMode() {
  vi.resetModules()
  return import('./mode')
}

describe('住院医生站统一接口模式', () => {
  afterEach(() => {
    vi.unstubAllEnvs()
    vi.restoreAllMocks()
    vi.resetModules()
  })

  it('mock 模式启用本地文书类型和医嘱目录', async () => {
    vi.stubEnv('VITE_INPATIENT_DOCTOR_STATION_API_MODE', 'mock')

    const mode = await loadMode()

    expect(mode.inpatientDoctorStationApiMode).toBe('mock')
    expect(mode.isInpatientDoctorStationRealApiEnabled).toBe(false)
    expect(mode.inpatientDoctorStationCapabilities).toEqual({
      medicalRecordDocumentTypes: true,
      medicalOrderCatalog: true
    })
  })

  it('real 模式禁用后端尚未提供的两项目录能力', async () => {
    vi.stubEnv('VITE_INPATIENT_DOCTOR_STATION_API_MODE', 'real')

    const mode = await loadMode()

    expect(mode.inpatientDoctorStationApiMode).toBe('real')
    expect(mode.isInpatientDoctorStationRealApiEnabled).toBe(true)
    expect(mode.inpatientDoctorStationCapabilities).toEqual({
      medicalRecordDocumentTypes: false,
      medicalOrderCatalog: false
    })
  })

  it('非法配置安全回退到 mock', async () => {
    vi.stubEnv('VITE_INPATIENT_DOCTOR_STATION_API_MODE', 'mixed')
    const warn = vi.spyOn(console, 'warn').mockImplementation(() => undefined)

    const mode = await loadMode()

    expect(mode.inpatientDoctorStationApiMode).toBe('mock')
    expect(mode.isInpatientDoctorStationRealApiEnabled).toBe(false)
    if (import.meta.env.DEV) expect(warn).toHaveBeenCalledOnce()
  })

  it(
    '住院中医处方复用住院开关且不影响门诊开关',
    async () => {
      vi.stubEnv('VITE_INPATIENT_DOCTOR_STATION_API_MODE', 'real')
      vi.stubEnv('VITE_OUTPATIENT_DOCTOR_STATION_API_MODE', 'mock')
      vi.resetModules()

      const { isTcmPrescriptionRealApiEnabled } = await import(
        '@/apis/clinical/tcm-prescription/mode'
      )

      expect(isTcmPrescriptionRealApiEnabled('inpatient')).toBe(true)
      expect(isTcmPrescriptionRealApiEnabled('outpatient')).toBe(false)
    },
    5_000
  )
})
