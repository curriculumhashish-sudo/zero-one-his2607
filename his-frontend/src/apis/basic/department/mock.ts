import type {
  DepartmentFormOptions,
  DepartmentQuery,
  DepartmentRecord,
  DepartmentSavePayload,
  DepartmentTreeOption
} from './type'

const typeOptions = ['医院', '临床科室', '医技科室', '行政科室'].map((value) => ({
  label: value,
  value
}))
const categoryOptions = ['门诊', '住院', '医技', '职能'].map((value) => ({ label: value, value }))
const leaderOptions = ['张主任', '李主任', '王主任', '赵主任'].map((label, index) => ({
  label,
  value: `leader-${index + 1}`
}))
const leaderNameMap = new Map(leaderOptions.map((item) => [item.value, item.label]))

let sequence = 9
const departments: DepartmentRecord[] = [
  seed('dept-001', 'H001', '零壹医院', null, '医院', '职能', 'leader-1', 1),
  seed('dept-002', 'C001', '门诊部', 'dept-001', '临床科室', '门诊', 'leader-2', 10),
  seed('dept-003', 'C002', '内科门诊', 'dept-002', '临床科室', '门诊', 'leader-3', 11),
  seed('dept-004', 'C003', '外科门诊', 'dept-002', '临床科室', '门诊', 'leader-4', 12),
  seed('dept-005', 'I001', '住院部', 'dept-001', '临床科室', '住院', 'leader-2', 20),
  seed('dept-006', 'I002', '内科病区', 'dept-005', '临床科室', '住院', 'leader-3', 21),
  seed('dept-007', 'M001', '医学检验科', 'dept-001', '医技科室', '医技', 'leader-4', 30),
  seed('dept-008', 'A001', '信息科', 'dept-001', '行政科室', '职能', 'leader-1', 40, false),
  seed('dept-009', 'A002', '财务科', 'dept-001', '行政科室', '职能', 'leader-2', 41)
]

function seed(
  id: string,
  code: string,
  name: string,
  parentId: string | null,
  type: string,
  category: string,
  leaderId: string,
  sort: number,
  enabled = true
): DepartmentRecord {
  return {
    id,
    code,
    name,
    parentId,
    parentName: '',
    type,
    category,
    leaderId,
    leaderName: leaderNameMap.get(leaderId) ?? '',
    sort,
    enabled,
    createdAt: '2026-07-01 08:00:00',
    updatedAt: '2026-07-01 08:00:00'
  }
}

function delay(ms = 60) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function cloneDepartment(item: DepartmentRecord): DepartmentRecord {
  return {
    ...item,
    children: item.children?.map(cloneDepartment)
  }
}

function requireDepartment(id: string) {
  const department = departments.find((item) => item.id === id)
  if (!department) throw new Error('科室不存在')
  return department
}

function buildTree(rows: DepartmentRecord[]): DepartmentRecord[] {
  const sourceIds = new Set(rows.map((row) => row.id))
  const nodeMap = new Map(
    rows.map((row) => [row.id, { ...row, children: [] as DepartmentRecord[] }])
  )
  const roots: DepartmentRecord[] = []
  for (const row of rows) {
    const node = nodeMap.get(row.id)!
    const parent = row.parentId ? nodeMap.get(row.parentId) : undefined
    node.parentName = row.parentId
      ? (departments.find((item) => item.id === row.parentId)?.name ?? '')
      : ''
    if (parent && sourceIds.has(parent.id)) parent.children!.push(node)
    else roots.push(node)
  }
  const sortTree = (items: DepartmentRecord[]) => {
    items.sort((left, right) => left.sort - right.sort)
    items.forEach((item) => sortTree(item.children ?? []))
  }
  sortTree(roots)
  return roots
}

function includeAncestors(ids: Set<string>) {
  for (const id of [...ids]) {
    let parentId = departments.find((item) => item.id === id)?.parentId
    while (parentId) {
      ids.add(parentId)
      parentId = departments.find((item) => item.id === parentId)?.parentId ?? null
    }
  }
}

function buildDepartmentOptions(excludeId?: string): DepartmentTreeOption[] {
  const excluded = new Set<string>()
  if (excludeId) {
    excluded.add(excludeId)
    let changed = true
    while (changed) {
      changed = false
      for (const item of departments) {
        if (item.parentId && excluded.has(item.parentId) && !excluded.has(item.id)) {
          excluded.add(item.id)
          changed = true
        }
      }
    }
  }
  const tree = buildTree(departments.filter((item) => !excluded.has(item.id)))
  const mapOptions = (items: DepartmentRecord[]): DepartmentTreeOption[] =>
    items.map((item) => ({
      label: item.name,
      value: item.id,
      children: item.children?.length ? mapOptions(item.children) : undefined
    }))
  return mapOptions(tree)
}

export async function mockListDepartments(query: DepartmentQuery) {
  await delay()
  const keyword = query.keyword?.trim().toLowerCase()
  const matchedIds = new Set(
    departments
      .filter((item) => {
        if (keyword && !item.name.toLowerCase().includes(keyword) && !item.code.includes(keyword)) {
          return false
        }
        if (query.type && item.type !== query.type) return false
        if (query.category && item.category !== query.category) return false
        if (query.enabled != null && item.enabled !== query.enabled) return false
        return true
      })
      .map((item) => item.id)
  )
  // 匹配子科室时保留祖先节点，保证搜索结果仍能看出组织层级。
  includeAncestors(matchedIds)
  return buildTree(departments.filter((item) => matchedIds.has(item.id))).map(cloneDepartment)
}

export async function mockGetDepartmentDetail(id: string) {
  await delay()
  return cloneDepartment(requireDepartment(id))
}

function validatePayload(payload: DepartmentSavePayload, currentId?: string) {
  if (!payload.name.trim()) throw new Error('科室名称不能为空')
  if (!payload.type) throw new Error('请选择科室类型')
  if (!payload.category) throw new Error('请选择科室类别')
  if (payload.parentId === currentId) throw new Error('上级科室不能是当前科室')
  if (payload.parentId) requireDepartment(payload.parentId)
}

export async function mockCreateDepartment(payload: DepartmentSavePayload) {
  await delay()
  validatePayload(payload)
  sequence += 1
  const now = new Date().toISOString().replace('T', ' ').slice(0, 19)
  const department: DepartmentRecord = {
    id: `dept-${String(sequence).padStart(3, '0')}`,
    code: `D${String(sequence).padStart(3, '0')}`,
    name: payload.name.trim(),
    parentId: payload.parentId,
    parentName: payload.parentId ? requireDepartment(payload.parentId).name : '',
    type: payload.type,
    category: payload.category,
    leaderId: payload.leaderId,
    leaderName: leaderNameMap.get(payload.leaderId) ?? '',
    sort: payload.sort,
    enabled: payload.enabled,
    createdAt: now,
    updatedAt: now
  }
  departments.push(department)
  return cloneDepartment(department)
}

export async function mockUpdateDepartment(id: string, payload: DepartmentSavePayload) {
  await delay()
  validatePayload(payload, id)
  let parentId = payload.parentId
  while (parentId) {
    if (parentId === id) throw new Error('不能把科室移动到自己的下级')
    parentId = requireDepartment(parentId).parentId
  }
  const department = requireDepartment(id)
  Object.assign(department, {
    ...payload,
    name: payload.name.trim(),
    parentName: payload.parentId ? requireDepartment(payload.parentId).name : '',
    leaderName: leaderNameMap.get(payload.leaderId) ?? '',
    updatedAt: new Date().toISOString().replace('T', ' ').slice(0, 19)
  })
  return cloneDepartment(department)
}

export async function mockSetDepartmentEnabled(id: string, enabled: boolean) {
  await delay()
  const department = requireDepartment(id)
  department.enabled = enabled
  department.updatedAt = new Date().toISOString().replace('T', ' ').slice(0, 19)
  return cloneDepartment(department)
}

export async function mockRemoveDepartment(id: string) {
  await delay()
  const department = requireDepartment(id)
  if (departments.some((item) => item.parentId === id)) throw new Error('存在下级科室，不能删除')
  const index = departments.indexOf(department)
  departments.splice(index, 1)
}

export async function mockLoadDepartmentFormOptions(
  excludeId?: string
): Promise<DepartmentFormOptions> {
  await delay()
  return {
    typeOptions,
    categoryOptions,
    leaderOptions,
    departmentOptions: buildDepartmentOptions(excludeId)
  }
}
