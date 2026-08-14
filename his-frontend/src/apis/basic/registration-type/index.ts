import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type {
  RegistrationTypeNameOption,
  RegistrationTypeQuery,
  RegistrationTypeRecord,
  RegistrationTypeSavePayload
} from './type'

const baseUrl = '/j3-register/register-types'

export async function loadRegistrationTypeNameOptions(
  _keyword = ''
): Promise<RegistrationTypeNameOption[]> {
  const http = useHttp()
  const rows = unwrap(await http.get<BackendRegistrationName[]>(`${baseUrl}/query-names`))
  return rows.map((row) => ({ label: row.name, value: String(row.id) }))
}

export async function pageRegistrationTypes(query: RegistrationTypeQuery) {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPageRegistration>(`${baseUrl}/query-list`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}

export async function createRegistrationType(data: RegistrationTypeSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.post<boolean>(`${baseUrl}/save-add`, toPayload(data))))
}

export async function updateRegistrationType(id: string, data: RegistrationTypeSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.put<boolean>(`${baseUrl}/save-modify/${id}`, toPayload(data))))
}

export async function removeRegistrationType(id: string) {
  const http = useHttp()
  unwrap(await http.delete<boolean>(`${baseUrl}/remove-by-id/${id}`))
}

interface BackendPageRegistration {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: BackendRegistration[]
}

interface BackendRegistrationName {
  id: number
  name: string
}

interface BackendRegistration {
  id: number
  name: string
  mdtrtCertNo?: string
  businessCode?: string
  amount: number
  status?: number
}

function toRecord(item: BackendRegistration): RegistrationTypeRecord {
  const activeFlag = item.status ?? 0
  return {
    id: String(item.id),
    busNo: item.businessCode ?? '',
    name: item.name,
    ybNo: item.mdtrtCertNo ?? '',
    amount: item.amount,
    activeFlag,
    activeFlagText: activeFlag === 1 ? '启用' : '停用'
  }
}

function toPayload(data: RegistrationTypeSavePayload) {
  return {
    name: data.name,
    mdtrtCertNo: data.ybNo,
    amount: data.amount,
    status: data.activeFlag
  }
}

export type * from './type'
