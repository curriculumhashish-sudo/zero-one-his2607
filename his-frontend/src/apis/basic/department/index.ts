import { useHttp } from '@/plugins/http'
import { toMutationResult, unwrap } from '../remote'
import type { DepartmentFormOptions, DepartmentQuery, DepartmentRecord, DepartmentSavePayload, DepartmentTreeOption } from './type'

const baseUrl = '/c5-basicdata-api/dept'

interface BackendDepartment {
  id: string | number
  busNo?: string
  name: string
  parentId?: string | number | null
  typeEnum?: string | number
  typeEnumDictText?: string
  classEnum?: string | number
  classEnumDictText?: string
  statusEnum?: string | number
  defDoctorId?: string | number
  defDoctorName?: string
  displayOrder?: number
  ybNo?: string
  ybName?: string
  medinsId?: string
  medinsAdmdvs?: string
  medinsType?: string
  medinsLv?: string
  caty?: string
  pyStr?: string
  wbStr?: string
  children?: BackendDepartment[]
}

export async function listDepartments(query: DepartmentQuery) {
  const data = unwrap(await useHttp().get<BackendDepartment[]>(`${baseUrl}/tree`, {
    keyword: query.keyword || undefined,
    typeEnum: query.type || undefined,
    classEnum: query.category || undefined,
    statusEnum: query.enabled == null ? undefined : query.enabled ? '1' : '0'
  }))
  return data.map(toRecord)
}

export async function getDepartmentDetail(id: string) {
  return toRecord(unwrap(await useHttp().get<BackendDepartment>(`${baseUrl}/detail`, { id })))
}

export async function createDepartment(data: DepartmentSavePayload) {
  return toMutationResult<DepartmentRecord>(unwrap(await useHttp().post<string>(`${baseUrl}/createDept`, toPayload(data))))
}

export async function updateDepartment(id: string, data: DepartmentSavePayload) {
  return toMutationResult<DepartmentRecord>(unwrap(await useHttp().put<string>(`${baseUrl}/editDept`, { id, ...toPayload(data) })))
}

export async function setDepartmentEnabled(id: string, enabled: boolean) {
  return toMutationResult<DepartmentRecord>(unwrap(await useHttp().post<string>(`${baseUrl}/batch-status`, { ids: [id], status: enabled ? 1 : 0 })))
}

export async function removeDepartment(id: string) {
  unwrap(await useHttp().delete<string>(`${baseUrl}/deleteDept`, undefined, { params: { id } }))
}

export async function loadDepartmentFormOptions(excludeId?: string): Promise<DepartmentFormOptions> {
  const [tree, leaders] = await Promise.all([
    unwrap(await useHttp().get<BackendDepartment[]>(`${baseUrl}/name-tree`)),
    unwrap(await useHttp().get<Array<{ id: string | number; name: string }>>(`${baseUrl}/practitioner/options`))
  ])
  const excluded = excludeId ? new Set([excludeId]) : new Set<string>()
  return {
    typeOptions: [{ label: '医院', value: '1' }, { label: '科室', value: '2' }],
    categoryOptions: ['门诊', '住院', '药房', '库房', '财务', '护士站', '管理', '后勤支持'].map((label, index) => ({ label, value: String(index + 1) })),
    leaderOptions: leaders.map((item) => ({ label: item.name, value: String(item.id) })),
    departmentOptions: treeToOptions(tree, excluded)
  }
}

function toRecord(item: BackendDepartment): DepartmentRecord {
  const enabled = String(item.statusEnum ?? '') === '1' || item.statusEnum === 'ACTIVE'
  return {
    id: String(item.id), code: item.busNo ?? '', ybNo: item.ybNo ?? '', ybName: item.ybName ?? '',
    medinsId: item.medinsId ?? '', medinsAdmdvs: item.medinsAdmdvs ?? '', medinsType: item.medinsType ?? '', medinsLv: item.medinsLv ?? '',
    caty: item.caty ?? '', pyStr: item.pyStr ?? '', wbStr: item.wbStr ?? '', name: item.name,
    parentId: item.parentId == null ? null : String(item.parentId), parentName: '',
    type: String(item.typeEnum ?? ''), typeText: item.typeEnumDictText ?? String(item.typeEnum ?? ''),
    category: String(item.classEnum ?? ''), categoryText: item.classEnumDictText ?? String(item.classEnum ?? ''),
    leaderId: item.defDoctorId == null ? '' : String(item.defDoctorId), leaderName: item.defDoctorName ?? '',
    sort: Number(item.displayOrder ?? 0), enabled, children: item.children?.map(toRecord), createdAt: '', updatedAt: ''
  }
}

function toPayload(data: DepartmentSavePayload) {
  return {
    name: data.name, parentId: data.parentId || null, typeEnum: data.type, classEnum: data.category,
    defDoctorId: data.leaderId || null, displayOrder: data.sort, ybNo: data.ybNo || null, ybName: data.ybName || null,
    medinsId: data.medinsId || null, medinsAdmdvs: data.medinsAdmdvs || null, medinsType: data.medinsType || null,
    medinsLv: data.medinsLv || null, caty: data.caty || null, pyStr: data.pyStr || null, wbStr: data.wbStr || null
  }
}

function treeToOptions(items: BackendDepartment[], excluded: Set<string>): DepartmentTreeOption[] {
  return items.filter((item) => !excluded.has(String(item.id))).map((item) => ({
    label: item.name, value: String(item.id), children: item.children?.length ? treeToOptions(item.children, excluded) : undefined
  }))
}

export type * from './type'
