import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { ChargeTypeNameOption, ChargeTypeQuery, ChargeTypeRecord, ChargeTypeSavePayload } from './type'

const baseUrl = '/j3-fee/fee-natures'

export async function loadChargeTypeNameOptions(keyword = ''): Promise<ChargeTypeNameOption[]> {
  const http = useHttp()
  const rows = unwrap(await http.get<BackendFeeNature[]>(`${baseUrl}/nameList`))
  const term = keyword.trim().toLowerCase()
  return rows
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: row.name, value: row.id }))
}

export async function pageChargeTypes(query: ChargeTypeQuery) {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPageFeeNature>(`${baseUrl}/page`, query))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}

export async function createChargeType(data: ChargeTypeSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.post<string>(baseUrl, toPayload(data))))
}

export async function updateChargeType(id: string, data: ChargeTypeSavePayload) {
  const http = useHttp()
  return toMutationResult(unwrap(await http.put<string>(`${baseUrl}/${id}`, undefined, {
    params: toPayload(data)
  })))
}

export async function removeChargeType(id: string) {
  const http = useHttp()
  unwrap(await http.delete<string>(`${baseUrl}/${id}`))
}

export async function setChargeTypeEnabled(id: string, enabled: boolean) {
  const http = useHttp()
  return unwrap(await http.put<string>(`${baseUrl}/status`, undefined, {
    params: { id, isEnabled: enabled ? 1 : 0 }
  }))
}

interface BackendPageFeeNature {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: BackendFeeNature[]
}

interface BackendFeeNature {
  id: string
  name: string
  busNo: string
  displayOrder: number
  categoryEnum?: number
  activeFlag?: number
}

function toRecord(item: BackendFeeNature): ChargeTypeRecord {
  return {
    id: item.id,
    busNo: item.busNo,
    name: item.name,
    displayOrder: item.displayOrder ?? 0,
    categoryEnum: fromCategory(item.categoryEnum),
    categoryEnumText: categoryText[item.categoryEnum ?? 0] ?? '',
    activeFlag: item.activeFlag ?? 0,
    activeFlagText: item.activeFlag === 1 ? '启用' : '停用'
  }
}

function toPayload(data: ChargeTypeSavePayload) {
  return {
    name: data.name,
    busNo: data.busNo,
    displayOrder: data.displayOrder,
    categoryEnum: toCategory(data.categoryEnum)
  }
}

function fromCategory(value?: number) {
  return ({ 1: 'SELF_PAY', 2: 'PUBLIC_PAY', 3: 'MEDICAL_INSURANCE', 4: 'MEDICAL_INSURANCE' } as Record<number, string>)[value ?? 0] ?? ''
}

function toCategory(value: string) {
  return ({ SELF_PAY: 1, PUBLIC_PAY: 2, MEDICAL_INSURANCE: 3 } as Record<string, number>)[value] ?? 1
}

const categoryText: Record<number, string> = {
  1: '自费',
  2: '公费',
  3: '省医保',
  4: '市医保'
}

export type * from './type'
