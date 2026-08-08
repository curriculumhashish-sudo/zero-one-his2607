import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { SupplierNameOption, SupplierQuery, SupplierRecord, SupplierSavePayload } from './type'

const baseUrl = '/j3-supplier/suppliers'

export async function loadSupplierNameOptions(keyword = ''): Promise<SupplierNameOption[]> {
  const http = useHttp()
  const rows = unwrap(await http.get<BackendSupplierName[]>(`${baseUrl}/names`, {
    keyword: keyword || undefined
  }))
  return rows.map((row) => ({ label: row.name, value: String(row.id) }))
}

export async function pageSuppliers(query: SupplierQuery) {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPageSupplier>(`${baseUrl}/list`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}

export async function getSupplierDetail(id: string) {
  const http = useHttp()
  return toRecord(unwrap(await http.get<BackendSupplier>(`${baseUrl}/detail`, { id })))
}

export async function createSupplier(data: SupplierSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.post<string>(`${baseUrl}/add`, toPayload(data))))
}

export async function updateSupplier(id: string, data: SupplierSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.put<string>(`${baseUrl}/update/${id}`, {
    ...toPayload(data),
    id
  })))
}

export async function removeSupplier(id: string) {
  const http = useHttp()
  unwrap(await http.delete<string>(`${baseUrl}/delete/${id}`))
}

export async function setSuppliersEnabled(ids: string[], enabled: boolean) {
  const http = useHttp()
  unwrap(await http.put<string>(`${baseUrl}/batchUpdateStatus`, {
    ids,
    statusEnum: enabled ? 'ACTIVE' : 'INACTIVE'
  }))
  return ids.map((id) => ({ id, statusEnum: enabled ? 'ACTIVE' : 'INACTIVE' }))
}

interface BackendPageSupplier {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: BackendSupplier[]
}

interface BackendSupplierName {
  id: string
  name: string
}

interface BackendSupplier {
  id: string
  version?: number
  code?: string
  busNo?: string
  name: string
  typeEnum?: string
  creditCode?: string
  address?: string
  pyStr?: string
  wbStr?: string
  phone?: string
  email?: string
  orgId?: string
  legalRepresentative?: string
  bankName?: string
  bankAccount?: string
  displayOrder?: number
  note?: string
  statusEnum?: string
}

function toRecord(item: BackendSupplier): SupplierRecord {
  const statusEnum = item.statusEnum === 'INACTIVE' ? 'INACTIVE' : 'ACTIVE'
  const typeEnum = item.typeEnum === 'ENTERPRISE' ? '1' : item.typeEnum === 'INDIVIDUAL' ? '2' : item.typeEnum ?? ''
  return {
    id: String(item.id),
    version: item.version,
    code: item.code,
    busNo: item.busNo ?? '',
    name: item.name,
    typeEnum,
    typeEnumText: supplierTypeText[typeEnum] ?? typeEnum,
    creditCode: item.creditCode ?? '',
    organizationId: item.orgId ?? '',
    organizationName: '',
    address: item.address ?? '',
    phone: item.phone ?? '',
    email: item.email ?? '',
    pyStr: item.pyStr ?? '',
    wbStr: item.wbStr ?? '',
    statusEnum,
    statusEnumText: statusEnum,
    legalRepresentative: item.legalRepresentative ?? '',
    bankName: item.bankName ?? '',
    bankAccount: item.bankAccount ?? '',
    displayOrder: item.displayOrder ?? 0,
    note: item.note ?? ''
  }
}

const supplierTypeText: Record<string, string> = {
  '1': '企业',
  '2': '个体',
  ENTERPRISE: '企业',
  INDIVIDUAL: '个体'
}

function toPayload(data: SupplierSavePayload) {
  return {
    name: data.name,
    typeEnum: data.typeEnum === '1' ? 'ENTERPRISE' : data.typeEnum === '2' ? 'INDIVIDUAL' : data.typeEnum,
    creditCode: data.creditCode,
    address: data.address,
    pyStr: data.pyStr,
    wbStr: data.wbStr,
    phone: data.phone,
    email: data.email,
    orgId: data.organizationId,
    statusEnum: data.statusEnum === 'INACTIVE' ? 'INACTIVE' : 'ACTIVE',
    legalRepresentative: data.legalRepresentative,
    bankName: data.bankName,
    bankAccount: data.bankAccount,
    displayOrder: data.displayOrder,
    note: data.note
  }
}

export type * from './type'
