export function savedEntityId(data: unknown, fallback = '') {
  if (typeof data === 'string' || typeof data === 'number') return String(data)
  if (data && typeof data === 'object') {
    return String((data as Record<string, unknown>).id ?? fallback)
  }
  return fallback
}
