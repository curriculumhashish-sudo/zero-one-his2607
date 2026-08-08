import { toC7PageResult, toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { ExecuteLocationQuery, ExecuteLocationSavePayload } from './type'

const baseUrl = '/c7-configuration-api/execlocation'

export async function pageExecuteLocations(query: ExecuteLocationQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/query-all`, toC7Query(query)))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function createExecuteLocation(data: ExecuteLocationSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/add-exec-location`, data)))
}

export async function updateExecuteLocation(id: string, data: ExecuteLocationSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/modify-exec-location`, { id, ...data })))
}

export async function removeExecuteLocation(id: string) {
  unwrap(await useHttp().delete(`${baseUrl}/remove-exec-location`, [id]))
}

function toC7Query(query: { pageIndex: number; pageSize: number } & object) {
  const { pageIndex, pageSize, ...filters } = query
  const itemTable = (filters as Record<string, unknown>).itemTable
  return {
    pageIndex,
    pageSize,
    ...(itemTable ? { itemTable } : {})
  }
}

export type * from './type'
