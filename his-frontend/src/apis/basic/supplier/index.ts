import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { SupplierNameOption, SupplierQuery, SupplierRecord, SupplierSavePayload } from './type'

const baseUrl = '/c5-basicdata-api/sup'
interface BackendSupplier { id: string | number; version?: number; busNo?: string; name: string; typeEnum?: string | number; creditCode?: string; address?: string; pyStr?: string; wbStr?: string; phone?: string; email?: string; organizationId?: string | number; orgId?: string | number; legalRepresentative?: string; bankName?: string; bankAccount?: string; displayOrder?: number; note?: string; statusEnum?: string }
interface BackendPage { pageIndex: number; pageSize: number; total: number; pages?: number; rows: BackendSupplier[] }
export async function loadSupplierNameOptions(keyword = ''): Promise<SupplierNameOption[]> {
  const page = unwrap(await useHttp().get<BackendPage>(`${baseUrl}/query-all`, { keyword, pageIndex: 1, pageSize: 100 }))
  return page.rows.map((row) => ({ label: row.name, value: String(row.id) }))
}
export async function pageSuppliers(query: SupplierQuery) {
  const page = unwrap(await useHttp().get<BackendPage>(`${baseUrl}/query-all`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}
export async function getSupplierDetail(id: string) { return toRecord(unwrap(await useHttp().get<BackendSupplier>(`${baseUrl}/query-one`, { id }))) }
export async function createSupplier(data: SupplierSavePayload) { return toMutationResult(unwrap(await useHttp().post<string>('/c5-basicdata-api/supplier/add-supplier', toPayload(data, true)))) }
export async function updateSupplier(id: string, data: SupplierSavePayload) { return toMutationResult(unwrap(await useHttp().post<string>('/c5-basicdata-api/supplier/modify-supplier', { id, version: data.version, ...toPayload(data, false) }))) }
export async function removeSupplier(id: string) { unwrap(await useHttp().delete<string>(`${baseUrl}/batch-remove`, [id])) }
export async function setSuppliersEnabled(ids: string[], enabled: boolean) {
  unwrap(await useHttp().put<string>('/c5-basicdata-api/sup/batch-enable-disable', { ids, statusEnum: enabled ? 'NORMAL' : 'DISABLED' }))
  return ids.map((id) => ({ id, statusEnum: enabled ? 'ACTIVE' as const : 'INACTIVE' as const }))
}
function toRecord(item: BackendSupplier): SupplierRecord {
  const statusEnum = item.statusEnum === 'DISABLED' || item.statusEnum === 'INACTIVE' ? 'INACTIVE' : 'ACTIVE'; const type = String(item.typeEnum ?? '')
  const typeEnum = type === 'ENTERPRISE' ? '1' : type === 'INDIVIDUAL' ? '2' : type
  return { id: String(item.id), version: item.version, busNo: item.busNo ?? '', name: item.name, typeEnum, typeEnumText: { '1': '企业', '2': '个体' }[typeEnum] ?? typeEnum, creditCode: item.creditCode ?? '', organizationId: String(item.organizationId ?? item.orgId ?? ''), organizationName: '', address: item.address ?? '', phone: item.phone ?? '', email: item.email ?? '', pyStr: item.pyStr ?? '', wbStr: item.wbStr ?? '', statusEnum, statusEnumText: statusEnum, legalRepresentative: item.legalRepresentative ?? '', bankName: item.bankName ?? '', bankAccount: item.bankAccount ?? '', displayOrder: item.displayOrder ?? 0, note: item.note ?? '' }
}
function toPayload(data: SupplierSavePayload, includeBusNo: boolean) {
  return { ...(includeBusNo ? { busNo: data.busNo } : {}), name: data.name, typeEnum: data.typeEnum, creditCode: data.creditCode, address: data.address, pyStr: data.pyStr, wbStr: data.wbStr, phone: data.phone, email: data.email, orgId: data.organizationId, statusEnum: data.statusEnum === 'INACTIVE' ? 'DISABLED' : 'NORMAL', legalRepresentative: data.legalRepresentative, bankName: data.bankName, bankAccount: data.bankAccount, displayOrder: data.displayOrder, note: data.note }
}
export type * from './type'
