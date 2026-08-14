export type ApiProfile = 'c6' | 'j4'

const STORAGE_KEY = 'his-api-profile'

function defaultProfile(): ApiProfile {
  return import.meta.env.VITE_API_PROFILE === 'c6' ? 'c6' : 'j4'
}

export function getApiProfile(): ApiProfile {
  if (typeof window === 'undefined') return defaultProfile()
  const stored = window.localStorage.getItem(STORAGE_KEY)
  return stored === 'c6' || stored === 'j4' ? stored : defaultProfile()
}

export function setApiProfile(profile: ApiProfile): void {
  if (typeof window !== 'undefined') window.localStorage.setItem(STORAGE_KEY, profile)
}

export function getApiProfileLabel(profile: ApiProfile = getApiProfile()): string {
  return profile === 'c6' ? 'C6' : 'J4'
}

export function getApiBaseUrl(profile: ApiProfile = getApiProfile()): string {
  const configured =
    profile === 'c6' ? import.meta.env.VITE_C6_API_URL : import.meta.env.VITE_J4_API_URL
  return configured || import.meta.env.VITE_API_URL
}

export function isMockEnabled(): boolean {
  return import.meta.env.VITE_USE_MOCK === 'true'
}

export async function mockOrThrow<T>(error: unknown, fallback: () => T | Promise<T>): Promise<T> {
  if (isMockEnabled()) return fallback()
  throw error
}
