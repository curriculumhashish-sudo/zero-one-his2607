import type { JsonVO } from '@/apis/http'
import type { BasicPageResult } from './crud-mock'

export interface BackendPage<T> {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: T[]
}

export interface C7Page<T> {
  pageIndex?: number
  pageNo?: number
  pageSize?: number
  total?: number | string
  pages?: number | string
  rows?: T[]
  totalCount?: number | string
  content?: T[]
}

export function unwrap<T>(response: JsonVO<T>): T {
  if (response.data === undefined && response.code === 10000) {
    return undefined as T
  }
  if (response.data === undefined) {
    throw new Error(response.message || '服务器未返回有效数据')
  }
  return response.data
}

export function toPageResult<T>(page: BackendPage<T>): BasicPageResult<T> {
  return {
    pageIndex: page.pageIndex,
    pageSize: page.pageSize,
    total: page.total,
    pages: page.pages ?? Math.ceil(page.total / Math.max(1, page.pageSize)),
    rows: page.rows
  }
}

export function toC7PageResult<T>(page: C7Page<T>, fallbackPage = 1, fallbackSize = 10): BasicPageResult<T> {
  const pageIndex = Number(page.pageIndex ?? page.pageNo ?? fallbackPage) || fallbackPage
  const pageSize = Number(page.pageSize ?? fallbackSize) || fallbackSize
  const rows = Array.isArray(page.rows) ? page.rows : Array.isArray(page.content) ? page.content : []
  const total = Number(page.total ?? page.totalCount ?? rows.length) || 0
  return {
    pageIndex,
    pageSize,
    total,
    pages: Number(page.pages) || Math.ceil(total / Math.max(1, pageSize)),
    rows
  }
}

export function toMutationResult<T extends { id: string }>(id: string | number | boolean | undefined): T {
  return { id: String(id ?? '') } as T
}
