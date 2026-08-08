import { useHttp } from '@/plugins/http'
import { toMutationResult, unwrap } from '../remote'
import type {
  DepartmentQuery,
  DepartmentRecord,
  DepartmentSavePayload,
  DepartmentTreeOption
} from './type'

const baseUrl = '/j3-department/departments'

export async function listDepartments(query: DepartmentQuery) {
  const http = useHttp()
  const data = unwrap(await http.post<BackendDepartmentNode[]>(`${baseUrl}/tree`, {
    keyword: query.keyword || undefined,
    typeEnum: query.type ? typeEnumToBackend(query.type) : undefined,
    classEnum: query.category ? classEnumToBackend(query.category) : undefined,
    statusEnum: query.enabled == null ? undefined : query.enabled ? 'ACTIVE' : 'INACTIVE'
  }))
  return data.map(toDepartmentRecord)
}

export async function getDepartmentDetail(id: string) {
  const http = useHttp()
  return toDepartmentRecord(unwrap(await http.get<BackendDepartmentNode>(`${baseUrl}/${id}`)))
}

export async function createDepartment(data: DepartmentSavePayload) {
  const http = useHttp()
  return toMutationResult<DepartmentRecord>(unwrap(await http.post<string>(baseUrl, toDepartmentPayload(data))))
}

export async function updateDepartment(id: string, data: DepartmentSavePayload) {
  const http = useHttp()
  return toMutationResult<DepartmentRecord>(unwrap(await http.put<string>(`${baseUrl}/${id}`, toDepartmentPayload(data))))
}

export async function setDepartmentEnabled(id: string, enabled: boolean) {
  const http = useHttp()
  return toMutationResult<DepartmentRecord>(unwrap(await http.put<string>(`${baseUrl}/batch-status`, {
    ids: [id],
    isEnabled: enabled ? '1' : '0'
  })))
}

export async function removeDepartment(id: string) {
  const http = useHttp()
  unwrap(await http.delete<string>(`${baseUrl}/${id}`))
}

export async function loadDepartmentFormOptions(excludeId?: string) {
  const http = useHttp()
  const tree = unwrap(await http.post<BackendDepartmentNode[]>(`${baseUrl}/tree`, {
    statusEnum: 'ACTIVE'
  }))
  const excluded = excludeId ? new Set([excludeId]) : new Set<string>()
  return {
    typeOptions: [
      { label: '医院', value: 'HOSPITAL' },
      { label: '科室', value: 'DEPARTMENT' }
    ],
    categoryOptions: [
      { label: '门诊', value: 'OUTPATIENT' },
      { label: '住院', value: 'INPATIENT' },
      { label: '药房', value: 'PHARMACY' },
      { label: '库房', value: 'WAREHOUSE' },
      { label: '财务', value: 'FINANCE' },
      { label: '护士站', value: 'NURSE_STATION' },
      { label: '管理', value: 'MANAGEMENT' },
      { label: '后勤支持', value: 'LOGISTICS_SUPPORT' }
    ],
    leaderOptions: [],
    departmentOptions: treeToOptions(tree, excluded)
  }
}

interface BackendDepartmentNode {
  id: string
  busNo?: string
  ybNo?: string
  ybName?: string
  medinsId?: string
  medinsAdmdvs?: string
  medinsType?: string
  medinsLv?: string
  caty?: string
  pyStr?: string
  wbStr?: string
  name: string
  statusEnum?: string
  typeEnum?: string
  typeEnumDictText?: string
  classEnum?: string
  classEnumDictText?: string
  defDoctorId?: string
  displayOrder?: number
  parentId?: string | null
  children?: BackendDepartmentNode[]
  updatedAt?: string
}

function toDepartmentRecord(item: BackendDepartmentNode): DepartmentRecord {
  return {
    id: String(item.id),
    code: item.busNo ?? '',
    ybNo: item.ybNo ?? '',
    ybName: item.ybName ?? '',
    medinsId: item.medinsId ?? '',
    medinsAdmdvs: item.medinsAdmdvs ?? '',
    medinsType: item.medinsType ?? '',
    medinsLv: item.medinsLv ?? '',
    caty: item.caty ?? '',
    pyStr: item.pyStr ?? '',
    wbStr: item.wbStr ?? '',
    name: item.name,
    parentId: item.parentId == null ? null : String(item.parentId),
    parentName: '',
    type: departmentTypeValue(item.typeEnum),
    typeText: item.typeEnumDictText ?? item.typeEnum ?? '',
    category: departmentCategoryValue(item.classEnum),
    categoryText: item.classEnumDictText ?? item.classEnum ?? '',
    leaderId: item.defDoctorId ?? '',
    leaderName: '',
    sort: Number(item.displayOrder ?? 0),
    enabled: item.statusEnum === 'ACTIVE' || item.statusEnum === '1',
    children: item.children?.map(toDepartmentRecord),
    createdAt: '',
    updatedAt: item.updatedAt ?? ''
  }
}

function toDepartmentPayload(data: DepartmentSavePayload) {
  return {
    name: data.name,
    parentId: data.parentId || null,
    typeEnum: typeEnumToBackend(data.type),
    classEnum: classEnumToBackend(data.category),
    defDoctorId: data.leaderId || null,
    displayOrder: String(data.sort),
    ybNo: data.ybNo || null,
    ybName: data.ybName || null,
    medinsId: data.medinsId || null,
    medinsAdmdvs: data.medinsAdmdvs || null,
    medinsType: data.medinsType || null,
    medinsLv: data.medinsLv || null,
    caty: data.caty || null,
    pyStr: data.pyStr || null,
    wbStr: data.wbStr || null
  }
}

function departmentTypeValue(value?: string) {
  return ({ '1': 'HOSPITAL', '2': 'DEPARTMENT' } as Record<string, string>)[value ?? ''] ?? value ?? ''
}

function departmentCategoryValue(value?: string) {
  const map: Record<string, string> = {
    '1': 'OUTPATIENT', '2': 'INPATIENT', '3': 'PHARMACY', '4': 'WAREHOUSE',
    '5': 'FINANCE', '6': 'NURSE_STATION', '7': 'MANAGEMENT', '8': 'LOGISTICS_SUPPORT'
  }
  return map[value ?? ''] ?? value ?? ''
}

const typeEnumMap: Record<string, string> = { HOSPITAL: '1', DEPARTMENT: '2', '1': '1', '2': '2' }
const classEnumMap: Record<string, string> = {
  OUTPATIENT: '1', INPATIENT: '2', PHARMACY: '3', WAREHOUSE: '4', FINANCE: '5',
  NURSE_STATION: '6', MANAGEMENT: '7', LOGISTICS_SUPPORT: '8'
}

function typeEnumToBackend(value: string) { return typeEnumMap[value] ?? value }
function classEnumToBackend(value: string) { return classEnumMap[value] ?? value }

function treeToOptions(items: BackendDepartmentNode[], excluded: Set<string>): DepartmentTreeOption[] {
  return items
    .filter((item) => !excluded.has(String(item.id)))
    .map((item) => ({
      label: item.name,
      value: String(item.id),
      children: item.children?.length ? treeToOptions(item.children, excluded) : undefined
    }))
}

export type * from './type'
