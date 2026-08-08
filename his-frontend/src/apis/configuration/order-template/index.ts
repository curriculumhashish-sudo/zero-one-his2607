import { toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { OrderTemplateEnvelope, OrderTemplateQuery, OrderTemplateRecord, OrderTemplateSavePayload } from './type'

const baseUrl = '/c7-configuration-api/pack-template'

export async function loadOrderTemplateNameOptions() {
  const result = unwrap(await useHttp().get<{ data?: string[] }>(`${baseUrl}/query-pack-template-names`))
  return (result.data ?? []).map((name) => ({ label: name, value: name }))
}

export async function pageOrderTemplates(query: OrderTemplateQuery) {
  const result = unwrap(await useHttp().get<{ data?: Array<Partial<OrderTemplateRecord>>; total?: number }>(`${baseUrl}/query-all-pack-template`, toC7Query(query)))
  const rows = (result.data ?? []) as OrderTemplateRecord[]
  const pageIndex = query.pageIndex
  const pageSize = query.pageSize
  const total = Number(result.total ?? rows.length)
  return { pageIndex, pageSize, total, pages: Math.ceil(total / Math.max(1, pageSize)), rows }
}

export async function getOrderTemplateDetail(id: string) {
  const result = unwrap(await useHttp().get<{ data?: OrderTemplateRecord }>(`${baseUrl}/query-one-pack-template`, { id }))
  return (result.data ?? {}) as OrderTemplateRecord
}

export async function createOrderTemplate(data: OrderTemplateSavePayload) {
  const result = unwrap(await useHttp().post<string>(`${baseUrl}/add-order-template`, toEnvelope(data)))
  return toMutationResult(result)
}

export async function updateOrderTemplate(id: string, data: OrderTemplateSavePayload) {
  const result = unwrap(await useHttp().post<string>(`${baseUrl}/modify-order-template`, toEnvelope({ ...data, id })))
  return toMutationResult(result)
}

export async function removeOrderTemplate(id: string) {
  unwrap(await useHttp().post(`${baseUrl}/remove-pack-template`, { id }))
}

export async function setOrderTemplateEnabled(id: string, enabled: boolean) {
  unwrap(await useHttp().post(`${baseUrl}/modify-pack-template-status`, { id, status: enabled ? 'ACTIVE' : 'INACTIVE' }))
}

function toEnvelope(data: OrderTemplateSavePayload & { id?: string }): OrderTemplateEnvelope {
  const { lines, ...header } = data
  let normalizedLines = lines
  if (typeof lines === 'string') {
    try { normalizedLines = JSON.parse(lines) as OrderTemplateSavePayload['lines'] } catch { normalizedLines = [] }
  }
  return { header: header as OrderTemplateEnvelope['header'], lines: Array.isArray(normalizedLines) ? normalizedLines : [] }
}

function toC7Query(query: { pageIndex: number; pageSize: number } & object) {
  const { pageIndex, pageSize, ...filters } = query
  return { pageNo: pageIndex, pageSize, ...filters }
}

export type * from './type'
