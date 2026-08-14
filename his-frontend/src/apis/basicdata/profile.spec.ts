import { beforeEach, describe, expect, it, vi } from 'vitest'
import {
  getApiBaseUrl,
  getApiProfile,
  getApiProfileLabel,
  isMockEnabled,
  setApiProfile
} from './profile'

describe('基础数据接口来源配置', () => {
  beforeEach(() => {
    window.localStorage.clear()
    vi.unstubAllEnvs()
  })

  it('支持持久化切换 C6 和 J4', () => {
    vi.stubEnv('VITE_API_PROFILE', 'j4')
    expect(getApiProfile()).toBe('j4')
    setApiProfile('c6')
    expect(getApiProfile()).toBe('c6')
    expect(getApiProfileLabel()).toBe('C6')
  })

  it('按当前来源选择独立 baseURL', () => {
    vi.stubEnv('VITE_C6_API_URL', '/api/c6')
    vi.stubEnv('VITE_J4_API_URL', '/api/j4')
    expect(getApiBaseUrl('c6')).toBe('/api/c6')
    expect(getApiBaseUrl('j4')).toBe('/api/j4')
  })

  it('Mock 只能通过显式环境变量启用', () => {
    vi.stubEnv('VITE_USE_MOCK', 'false')
    expect(isMockEnabled()).toBe(false)
    vi.stubEnv('VITE_USE_MOCK', 'true')
    expect(isMockEnabled()).toBe(true)
  })
})
