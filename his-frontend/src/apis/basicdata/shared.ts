export interface BackendPage<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  totalElements?: number
  pages?: number
  rows?: T[]
  content?: T[]
}

export function asString(value: unknown, fallback = ''): string {
  if (value === null || value === undefined) return fallback
  return String(value)
}

export function asNumber(value: unknown, fallback = 0): number {
  const parsed = typeof value === 'number' ? value : Number(value)
  return Number.isFinite(parsed) ? parsed : fallback
}

export function parseStatus(value: unknown): boolean {
  if (typeof value === 'boolean') return value
  if (typeof value === 'number') return value === 1
  if (typeof value !== 'string') return false
  return ['1', 'true', 'active', 'publish', 'published', 'mq=='].includes(
    value.trim().toLowerCase()
  )
}

export function normalizePage<T>(page?: BackendPage<T>): { rows: T[]; total: number } {
  return {
    rows: page?.rows ?? page?.content ?? [],
    total: page?.total ?? page?.totalElements ?? 0
  }
}
