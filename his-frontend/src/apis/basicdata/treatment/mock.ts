import type {
  SelectOption,
  TreatmentCategory,
  TreatmentDetail,
  TreatmentFormOptions,
  TreatmentPageResult,
  TreatmentQuery,
  TreatmentRecord,
  TreatmentSavePayload
} from './type'

// ==================== 下拉选项 ====================

const categoryOptions: SelectOption[] = [
  { label: '检查', value: 'EXAM' },
  { label: '检验', value: 'LAB' },
  { label: '治疗', value: 'THERAPY' },
  { label: '手术', value: 'SURGERY' }
]

const unitOptions: SelectOption[] = [
  { label: '次', value: '次' },
  { label: '项', value: '项' },
  { label: '人次', value: '人次' },
  { label: '部位', value: '部位' }
]

// ==================== 类别名称映射 ====================

const categoryNameMap = new Map(categoryOptions.map((item) => [item.value, item.label]))

// ==================== Mock 数据 ====================

let sequence = 6

function now(): string {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

function seedDetail(
  id: string,
  code: string,
  name: string,
  pyCode: string,
  category: TreatmentCategory,
  unit: string,
  price: number,
  enabled: boolean,
  sort: number,
  remark = '',
  overrides: Partial<TreatmentDetail> = {}
): TreatmentDetail {
  return {
    id,
    code,
    name,
    pyCode,
    category,
    unit,
    price,
    enabled,
    sort,
    remark,
    ...overrides
  }
}

const details: TreatmentDetail[] = [
  seedDetail(
    'treat-001', 'EXAM001', '血常规检查', 'XCGJC', 'EXAM', '次', 25.00, true, 1,
    '通过血液分析仪检测红细胞、白细胞、血小板等常规指标'
  ),
  seedDetail(
    'treat-002', 'EXAM002', '尿常规检查', 'NCGJC', 'EXAM', '次', 20.00, true, 2,
    '通过尿液分析检测尿蛋白、尿糖、红细胞等指标'
  ),
  seedDetail(
    'treat-003', 'LAB001', '肝功能检查', 'GGNJC', 'LAB', '项', 80.00, true, 3,
    '检测谷丙转氨酶、谷草转氨酶等肝功能指标'
  ),
  seedDetail(
    'treat-004', 'THER001', '静脉输液', 'JMSY', 'THERAPY', '次', 15.00, true, 4,
    '通过静脉途径输入液体、电解质或药物'
  ),
  seedDetail(
    'treat-005', 'THER002', '雾化吸入', 'WHXR', 'THERAPY', '次', 30.00, false, 5,
    '通过雾化装置将药液转化为气雾吸入呼吸道'
  ),
  seedDetail(
    'treat-006', 'SURG001', '清创缝合', 'QCFH', 'SURGERY', '次', 200.00, true, 6,
    '对开放性伤口进行清洁、清创及缝合处理'
  )
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: TreatmentDetail): TreatmentRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    pyCode: detail.pyCode,
    category: detail.category,
    categoryName: categoryNameMap.get(detail.category) ?? detail.category,
    unit: detail.unit,
    price: detail.price,
    enabled: detail.enabled,
    sort: detail.sort,
    updatedAt: (detail as TreatmentRecord).updatedAt ?? now()
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

function requireDetail(id: string): TreatmentDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('诊疗项目不存在')
  return detail
}

function validatePayload(payload: TreatmentSavePayload) {
  if (!payload.code?.trim()) throw new Error('诊疗项目编码不能为空')
  if (!payload.name?.trim()) throw new Error('诊疗项目名称不能为空')
  if (!payload.category) throw new Error('请选择诊疗类别')
  if (!payload.unit?.trim()) throw new Error('计价单位不能为空')
  if (payload.price < 0) throw new Error('价格不能为负数')
  if (payload.sort < 0) throw new Error('排序号不能为负数')
}

// ==================== Mock API ====================

export async function mockListTreatment(
  query: TreatmentQuery
): Promise<TreatmentPageResult> {
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
  if (query.category) {
    filtered = filtered.filter((item) => item.category === query.category)
  }
  if (query.enabled != null) {
    filtered = filtered.filter((item) => item.enabled === query.enabled)
  }

  // 排序号升序
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

export async function mockGetTreatmentDetail(
  id: string
): Promise<TreatmentDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateTreatment(
  payload: TreatmentSavePayload
): Promise<TreatmentDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `treat-${String(sequence).padStart(3, '0')}`
  const detail: TreatmentDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateTreatment(
  id: string,
  payload: TreatmentSavePayload
): Promise<TreatmentDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetTreatmentEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveTreatment(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('诊疗项目不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadTreatmentFormOptions(): Promise<TreatmentFormOptions> {
  await delay()
  return {
    categoryOptions,
    unitOptions
  }
}
