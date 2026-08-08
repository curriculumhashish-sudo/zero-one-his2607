import { toMutationResult, toPageResult, unwrap } from '../remote'
import { useHttp } from '@/plugins/http'
import type {
  WardBedForm,
  WardBedNameOption,
  WardBedQuery,
  WardBedRecord,
  WardBedSavePayload
} from './type'

const baseUrl = '/j3-ward/wards'

export async function loadBedNameOptions(keyword = '') { return loadNames('BED', keyword) }
export async function loadRoomNameOptions(keyword = '') { return loadNames('HOUSE', keyword) }
export async function loadWardNameOptions(keyword = '') { return loadNames('WARD', keyword) }

async function loadNames(form: WardBedForm, keyword: string): Promise<WardBedNameOption[]> {
  const tree = unwrap(await useHttp().post<BackendWardNode[]>(`${baseUrl}/tree`, {}))
  const term = keyword.trim().toLowerCase()
  return flattenTree(tree)
    .filter((row) => row.formEnum === form && (!term || row.name.toLowerCase().includes(term)))
    .map((row) => ({ label: row.name, value: row.id }))
}

export async function pageWardBeds(query: WardBedQuery) {
  const tree = unwrap(await useHttp().post<BackendWardNode[]>(`${baseUrl}/tree`, {
    name: query.keyword || undefined
  }))
  const rows = flattenTree(tree)
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const start = (pageIndex - 1) * pageSize
  return toPageResult({ pageIndex, pageSize, total: rows.length, pages: Math.ceil(rows.length / pageSize), rows: rows.slice(start, start + pageSize) })
}

export async function createWardBed(data: WardBedSavePayload) {
  const http = useHttp()
  if (data.formEnum === 'WARD') {
    return toMutationResult(unwrap(await http.post<string>(baseUrl, { wardName: data.name, departmentCode: data.organizationId })))
  }
  if (data.formEnum === 'HOUSE') {
    return toMutationResult(unwrap(await http.post<string>(`${baseUrl}/room`, { roomName: data.name, wardCode: data.parentId })))
  }
  return toMutationResult(unwrap(await http.post<string>(`${baseUrl}/bed`, { bedName: data.name, roomCode: data.parentId })))
}

export async function updateWardBed(id: string, data: WardBedSavePayload) {
  const status = data.statusEnum === 'ACTIVE' ? 1 : 0
  if (data.formEnum === 'WARD') return unwrap(await useHttp().put<string>(`${baseUrl}/${id}`, { wardName: data.name, status }))
  if (data.formEnum === 'HOUSE') return unwrap(await useHttp().put<string>(`${baseUrl}/room/${id}`, { roomName: data.name, status }))
  return unwrap(await useHttp().put<string>(`${baseUrl}/bed/${id}`, { bedName: data.name, status }))
}

export async function removeWardBed(id: string) {
  const row = await findWardBed(id)
  const path = row.formEnum === 'WARD' ? baseUrl : row.formEnum === 'HOUSE' ? `${baseUrl}/room` : `${baseUrl}/bed`
  unwrap(await useHttp().delete<string>(`${path}/${id}`))
}

export async function setWardBedEnabled(id: string, enabled: boolean) {
  const row = await findWardBed(id)
  return updateWardBed(id, { ...row, statusEnum: enabled ? 'ACTIVE' : 'INACTIVE' })
}

async function findWardBed(id: string) {
  const tree = unwrap(await useHttp().post<BackendWardNode[]>(`${baseUrl}/tree`, {}))
  const row = flattenTree(tree).find((item) => item.id === id)
  if (!row) throw new Error('病区、病房或床位不存在')
  return row
}

interface BackendWardNode {
  id: string
  name: string
  type?: string
  formEnum?: WardBedForm
  status?: number
  statusEnum?: string
  children?: BackendWardNode[]
}

function flattenTree(items: BackendWardNode[], parent?: WardBedRecord): WardBedRecord[] {
  return items.flatMap((item) => {
    const formEnum = item.formEnum ?? (item.type === 'ROOM' ? 'HOUSE' : item.type === 'BED' ? 'BED' : 'WARD')
    const enabled = item.status === 1 || item.statusEnum === 'ACTIVE'
    const row: WardBedRecord = {
      id: String(item.id),
      busNo: '',
      name: item.name,
      formEnum,
      formEnumText: formText[formEnum],
      organizationId: '',
      organizationName: '',
      parentId: parent?.id ?? '',
      parentName: parent?.name ?? '',
      displayOrder: 0,
      statusEnum: enabled ? 'ACTIVE' : 'INACTIVE',
      statusEnumText: enabled ? '启用' : '停用',
      operationalEnum: 'NOT_IN_USE',
      operationalEnumText: '未使用'
    }
    return [row, ...flattenTree(item.children ?? [], row)]
  })
}

const formText: Record<WardBedForm, string> = { WARD: '病区', HOUSE: '病房', BED: '床位' }

export type * from './type'
