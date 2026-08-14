import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { WarehousePharmacyNameOption, WarehousePharmacyQuery, WarehousePharmacyRecord, WarehousePharmacySavePayload } from './type'

const baseUrl = '/c5-basicdata-api/pharm'
interface BackendStore { id: string | number; busNo?: string; name: string; formEnum?: 'WAREHOUSE' | 'PHARMACY'; formEnumText?: string; statusEnum?: string; statusEnumText?: string; displayOrder?: number; organizationId?: string | number; parentId?: string | number; pyStr?: string; wbStr?: string; createdAt?: string; updatedAt?: string }
interface BackendPage { pageIndex: number; pageSize: number; total: number; pages?: number; rows: BackendStore[] }
export async function loadWarehousePharmacyNameOptions(keyword = ''): Promise<WarehousePharmacyNameOption[]> {
  const rows = unwrap(await useHttp().get<BackendStore[]>(`${baseUrl}/name-list`, { keyword: keyword || undefined }))
  return rows.map((row) => ({ label: row.name, value: String(row.id) }))
}
export async function pageWarehousePharmacies(query: WarehousePharmacyQuery) {
  const page = unwrap(await useHttp().get<BackendPage>(`${baseUrl}/list`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}
export async function createWarehousePharmacy(data: WarehousePharmacySavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(baseUrl, toPayload(data))))
}
export async function updateWarehousePharmacy(id: string, data: WarehousePharmacySavePayload) {
  return toMutationResult(unwrap(await useHttp().put<string>(baseUrl, { id, ...toPayload(data) })))
}
export async function removeWarehousePharmacy(id: string) { unwrap(await useHttp().delete<string>(`${baseUrl}/${id}`)) }
export async function setWarehousePharmacyEnabled(id: string, enabled: boolean) {
  return toMutationResult(unwrap(await useHttp().put<string>(`${baseUrl}/status`, { id, statusEnum: enabled ? 'ACTIVE' : 'INACTIVE' })))
}
function toRecord(item: BackendStore): WarehousePharmacyRecord {
  const statusEnum = item.statusEnum === 'INACTIVE' || item.statusEnum === 'DISABLED' ? 'INACTIVE' : 'ACTIVE'
  return { id: String(item.id), busNo: item.busNo ?? '', name: item.name, formEnum: item.formEnum ?? 'WAREHOUSE', formEnumText: item.formEnumText ?? item.formEnum ?? '', statusEnum, statusEnumText: item.statusEnumText ?? statusEnum, displayOrder: item.displayOrder ?? 0, organizationId: item.organizationId == null ? '' : String(item.organizationId), parentId: item.parentId == null ? '' : String(item.parentId), pyStr: item.pyStr ?? '', wbStr: item.wbStr ?? '', createdAt: item.createdAt ?? '', updatedAt: item.updatedAt ?? '' }
}
function toPayload(data: WarehousePharmacySavePayload) { return { name: data.name, formEnum: data.formEnum, organizationId: data.organizationId, parentId: data.parentId || null, displayOrder: data.displayOrder, pyStr: data.pyStr, wbStr: data.wbStr } }
export type * from './type'
