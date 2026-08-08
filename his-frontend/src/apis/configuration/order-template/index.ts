import { toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { OrderTemplateEnvelope, OrderTemplateQuery, OrderTemplateRecord, OrderTemplateSavePayload } from './type'

const baseUrl = '/c7-configuration-api/pack-template'

export async function loadOrderTemplateNameOptions() {
  const names = unwrap(await useHttp().get<string[]>(`${baseUrl}/query-pack-template-names`))
  return (Array.isArray(names) ? names : []).map((name) => ({ label: name, value: name }))
}

export async function pageOrderTemplates(query: OrderTemplateQuery) {
  const result = unwrap(await useHttp().get<{
    data?: Array<Partial<OrderTemplateRecord>>
    rows?: Array<Partial<OrderTemplateRecord>>
    total?: number
  } | null>(`${baseUrl}/query-all-pack-template`, toC7Query(query)))
  const rows = (result?.rows ?? result?.data ?? []) as OrderTemplateRecord[]
  const pageIndex = query.pageIndex
  const pageSize = query.pageSize
  const total = Number(result?.total ?? rows.length)
  return { pageIndex, pageSize, total, pages: Math.ceil(total / Math.max(1, pageSize)), rows }
}

export async function getOrderTemplateDetail(id: string) {
  return (unwrap(await useHttp().get<OrderTemplateRecord | null>(`${baseUrl}/query-one-pack-template`, { id })) ?? {}) as OrderTemplateRecord
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
  unwrap(await useHttp().post(`${baseUrl}/modify-pack-template-status`, { id, statusEnum: enabled ? 'ACTIVE' : 'INACTIVE' }))
}

function toEnvelope(data: OrderTemplateSavePayload & { id?: string }): OrderTemplateEnvelope {
  const { lines, ...header } = data
  let normalizedLines = lines
  if (typeof lines === 'string') {
    try {
      normalizedLines = JSON.parse(lines) as OrderTemplateSavePayload['lines']
    } catch {
      throw new Error('明细 JSON 格式错误，请填写有效的明细数组')
    }
  }
  if (!Array.isArray(normalizedLines) || normalizedLines.length === 0) {
    throw new Error('组套模板至少需要填写一条明细')
  }
  if (normalizedLines.some((line) => line === null || typeof line !== 'object' || Array.isArray(line))) {
    throw new Error('明细 JSON 必须是由对象组成的数组')
  }
  return { header: header as OrderTemplateEnvelope['header'], lines: normalizedLines }
}

function toC7Query(query: { pageIndex: number; pageSize: number } & object) {
  const { pageIndex, pageSize, ...filters } = query
  const filterValues = filters as Record<string, unknown>
  const name = filterValues.name
  const bindingEnum = filterValues.bindingEnum
  const statusEnum = filterValues.statusEnum
  const templateKindEnum = filterValues.templateKindEnum
  return {
    pageIndex,
    pageSize,
    ...(name ? { name } : {}),
    ...(bindingEnum ? { bindingEnum } : {}),
    ...(statusEnum ? { statusEnum } : {}),
    ...(templateKindEnum ? { templateKindEnum } : {})
  }
}

export type * from './type'
