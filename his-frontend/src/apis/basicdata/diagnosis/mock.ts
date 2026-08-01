import type {
  DiagnosisDetail,
  DiagnosisFormOptions,
  DiagnosisPageResult,
  DiagnosisQuery,
  DiagnosisRecord,
  DiagnosisSavePayload
} from './type'
import type { SelectOption } from '../../basicdata/type'

// ==================== 下拉选项 ====================

const typeOptions: SelectOption[] = [
  { label: '西医', value: 'WESTERN' },
  { label: '中医', value: 'TCM' }
]

const categoryOptions: SelectOption[] = [
  { label: '门诊诊断', value: 'OUTPATIENT' },
  { label: '住院诊断', value: 'INPATIENT' },
  { label: '手术诊断', value: 'SURGERY' },
  { label: '病因诊断', value: 'ETIOLOGY' }
]

// ==================== 类别名称映射 ====================

const typeNameMap = new Map(typeOptions.map((item) => [item.value, item.label]))
const categoryNameMap = new Map(categoryOptions.map((item) => [item.value, item.label]))

// ==================== Mock 数据 ====================

let sequence = 6

function now(): string {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

const details: DiagnosisDetail[] = [
  {
    id: 'diag-001',
    code: 'J00',
    name: '急性鼻咽炎[普通感冒]',
    pyCode: 'JXBRY',
    type: 'WESTERN',
    category: 'OUTPATIENT',
    enabled: true,
    sort: 1,
    remark: '常见呼吸道感染'
  },
  {
    id: 'diag-002',
    code: 'J01',
    name: '急性鼻窦炎',
    pyCode: 'JXBDY',
    type: 'WESTERN',
    category: 'OUTPATIENT',
    enabled: true,
    sort: 2,
    remark: ''
  },
  {
    id: 'diag-003',
    code: 'J20',
    name: '急性支气管炎',
    pyCode: 'JXZQGY',
    type: 'WESTERN',
    category: 'OUTPATIENT',
    enabled: true,
    sort: 3,
    remark: ''
  },
  {
    id: 'diag-004',
    code: 'K29',
    name: '胃炎和十二指肠炎',
    pyCode: 'WYHESZCY',
    type: 'WESTERN',
    category: 'INPATIENT',
    enabled: true,
    sort: 4,
    remark: ''
  },
  {
    id: 'diag-005',
    code: 'N18',
    name: '慢性肾脏病',
    pyCode: 'MXSZB',
    type: 'WESTERN',
    category: 'INPATIENT',
    enabled: true,
    sort: 5,
    remark: ''
  },
  {
    id: 'diag-006',
    code: 'S82',
    name: '小腿骨折',
    pyCode: 'XTGZ',
    type: 'WESTERN',
    category: 'SURGERY',
    enabled: false,
    sort: 6,
    remark: '需手术处理'
  }
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: DiagnosisDetail): DiagnosisRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    pyCode: detail.pyCode,
    type: detail.type,
    typeName: typeNameMap.get(detail.type) ?? detail.type,
    category: detail.category,
    categoryName: categoryNameMap.get(detail.category) ?? detail.category,
    enabled: detail.enabled,
    sort: detail.sort,
    remark: detail.remark,
    updatedAt: (detail as DiagnosisRecord).updatedAt ?? now()
  }
}

/** 为每条记录附加 updatedAt（详情中不含该字段，首次转换时补上） */
const recordTimestamps = new Map<string, string>()
for (const d of details) {
  recordTimestamps.set(d.id ?? '', '2026-07-15 09:30:00')
}

function getRecordTimestamp(id: string): string {
  return recordTimestamps.get(id) ?? now()
}

function requireDetail(id: string): DiagnosisDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('诊断记录不存在')
  return detail
}

function validatePayload(payload: DiagnosisSavePayload) {
  if (!payload.code?.trim()) throw new Error('诊断编码不能为空')
  if (!payload.name?.trim()) throw new Error('诊断名称不能为空')
  if (!payload.type) throw new Error('请选择诊断类型')
  if (!payload.category) throw new Error('请选择诊断分类')
}

// ==================== Mock API ====================

export async function mockListDiagnosis(
  query: DiagnosisQuery
): Promise<DiagnosisPageResult> {
  await delay()
  let filtered = details.slice()

  const keyword = query.keyword?.trim().toLowerCase()
  if (keyword) {
    filtered = filtered.filter(
      (item) =>
        item.name.toLowerCase().includes(keyword) ||
        item.code.toLowerCase().includes(keyword) ||
        item.pyCode.toLowerCase().includes(keyword)
    )
  }
  if (query.type) {
    filtered = filtered.filter((item) => item.type === query.type)
  }
  if (query.enabled != null) {
    filtered = filtered.filter((item) => item.enabled === query.enabled)
  }

  filtered.sort((a, b) => a.sort - b.sort)

  const total = filtered.length
  const start = (query.pageIndex - 1) * query.pageSize
  const pageRows = filtered.slice(start, start + query.pageSize).map((item) => {
    const record = toRecord(item)
    record.updatedAt = getRecordTimestamp(item.id ?? '')
    return record
  })

  return { rows: pageRows, total }
}

export async function mockGetDiagnosisDetail(
  id: string
): Promise<DiagnosisDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateDiagnosis(
  payload: DiagnosisSavePayload
): Promise<DiagnosisDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `diag-${String(sequence).padStart(3, '0')}`
  const detail: DiagnosisDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateDiagnosis(
  id: string,
  payload: DiagnosisSavePayload
): Promise<DiagnosisDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetDiagnosisEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveDiagnosis(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('诊断记录不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadDiagnosisFormOptions(): Promise<DiagnosisFormOptions> {
  await delay()
  return {
    typeOptions,
    categoryOptions
  }
}
