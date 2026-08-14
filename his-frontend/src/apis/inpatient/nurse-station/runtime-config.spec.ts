import { afterEach, describe, expect, it, vi } from 'vitest'

async function loadConfig() {
  vi.resetModules()
  return import('./runtime-config')
}

describe('住院护士站统一接口模式', () => {
  afterEach(() => {
    vi.unstubAllEnvs()
    vi.resetModules()
  })

  it('mock 模式关闭所有真实接口', async () => {
    vi.stubEnv('VITE_NURSE_STATION_API_MODE', 'mock')

    const config = await loadConfig()

    expect(config.nurseStationApiMode).toBe('mock')
    expect(config.isNurseStationRealApiEnabled).toBe(false)
  })

  it('real 模式由统一开关打开真实接口', async () => {
    vi.stubEnv('VITE_NURSE_STATION_API_MODE', 'real')
    vi.stubEnv('VITE_NURSE_STATION_API_URL', '/nurse-station-api')

    const config = await loadConfig()

    expect(config.nurseStationApiMode).toBe('real')
    expect(config.isNurseStationRealApiEnabled).toBe(true)
    expect(config.realApiBaseURL).toBe('/nurse-station-api')
  })

  it('未配置或非法模式安全回退到 mock', async () => {
    vi.stubEnv('VITE_NURSE_STATION_API_MODE', 'mixed')

    const config = await loadConfig()

    expect(config.nurseStationApiMode).toBe('mock')
    expect(config.isNurseStationRealApiEnabled).toBe(false)
  })
})
