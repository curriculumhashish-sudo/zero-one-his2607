import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type {
  WarehousePharmacyNameOption,
  WarehousePharmacyQuery,
  WarehousePharmacyRecord,
  WarehousePharmacySavePayload
} from './type'

const baseUrl = '/j3-storehouse/storehouses'

export async function loadWarehousePharmacyNameOptions(
  keyword = ''
): Promise<WarehousePharmacyNameOption[]> {
  const http = useHttp()
  const rows = unwrap(await http.get<BackendStoreHouse[]>(`${baseUrl}/getStoreHouseNames`, {
    formEnum: undefined
  }))
  const term = keyword.trim().toLowerCase()
  return rows
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo?.toLowerCase().includes(term))
    .map((row) => ({ label: row.name, value: String(row.id) }))
}

export async function pageWarehousePharmacies(query: WarehousePharmacyQuery) {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPageStoreHouse>(`${baseUrl}/list`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}

export async function createWarehousePharmacy(data: WarehousePharmacySavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.post<string>(`${baseUrl}/add`, {
    name: data.name,
    formEnum: data.formEnum,
    organizationId: data.organizationId ? Number(data.organizationId) : undefined
  })))
}

export async function updateWarehousePharmacy(id: string, data: WarehousePharmacySavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.put<string>(`${baseUrl}/update`, {
    id,
    name: data.name,
    formEnum: data.formEnum,
    displayOrder: data.displayOrder,
    organizationId: data.organizationId ? Number(data.organizationId) : undefined,
    parentId: data.parentId,
    pyStr: data.pyStr,
    wbStr: data.wbStr
  })))
}

export async function removeWarehousePharmacy(id: string) {
  const http = useHttp()
  unwrap(await http.delete<string>(`${baseUrl}/delete/${id}`))
}

export async function setWarehousePharmacyEnabled(id: string, enabled: boolean) {
  const http = useHttp()
  const current = (await pageWarehousePharmacies({ pageIndex: 1, pageSize: 1000 })).rows.find((row) => row.id === id)
  if (!current) throw new Error('库房或药房不存在')
  return unwrap(await http.put<string>(`${baseUrl}/toggleEnabled`, undefined, {
    params: { id, isEnabled: enabled ? 1 : 0 }
  }))
}

interface BackendPageStoreHouse {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: BackendStoreHouse[]
}

interface BackendStoreHouse {
  id: string | number
  busNo?: string
  name: string
  statusEnum?: string
  statusEnumText?: string
  formEnum?: 'WAREHOUSE' | 'PHARMACY'
  formEnumText?: string
  displayOrder?: number
  organizationId?: string
  parentId?: string
  pyStr?: string
  wbStr?: string
  createdAt?: string
  updatedAt?: string
}

function toRecord(item: BackendStoreHouse): WarehousePharmacyRecord {
  const statusEnum = item.statusEnum === 'DISABLED' || item.statusEnum === 'INACTIVE' ? 'INACTIVE' : 'ACTIVE'
  return {
    id: String(item.id),
    busNo: item.busNo ?? '',
    name: item.name,
    formEnum: item.formEnum ?? 'WAREHOUSE',
    formEnumText: item.formEnumText ?? item.formEnum ?? '',
    statusEnum,
    statusEnumText: item.statusEnumText ?? statusEnum,
    displayOrder: item.displayOrder ?? 0,
    organizationId: item.organizationId ?? '',
    parentId: item.parentId ?? '',
    pyStr: item.pyStr ?? '',
    wbStr: item.wbStr ?? '',
    createdAt: item.createdAt ?? '',
    updatedAt: item.updatedAt ?? ''
  }
}

export type * from './type'
