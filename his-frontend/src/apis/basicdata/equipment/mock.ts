import type {
  EquipmentDetail,
  EquipmentFormOptions,
  EquipmentPageResult,
  EquipmentQuery,
  EquipmentRecord,
  EquipmentSavePayload,
  SelectOption
} from './type'

// ==================== 下拉选项 ====================

const categoryOptions: SelectOption[] = [
  { label: '耗材', value: 'CONSUMABLE' },
  { label: '器械', value: 'INSTRUMENT' },
  { label: '设备', value: 'EQUIPMENT' }
]

const unitOptions: SelectOption[] = [
  { label: '支', value: '支' },
  { label: '套', value: '套' },
  { label: '台', value: '台' },
  { label: '个', value: '个' },
  { label: '包', value: '包' },
  { label: '片', value: '片' },
  { label: '盒', value: '盒' },
  { label: '瓶', value: '瓶' }
]

// ==================== 类别名称映射 ====================

const categoryNameMap = new Map(categoryOptions.map((item) => [item.value, item.label]))

// ==================== Mock 数据 ====================

let sequence = 6

function now(): string {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

const details: EquipmentDetail[] = [
  {
    id: 'eq-001',
    code: 'EQ001',
    name: '一次性注射器',
    pyCode: 'YXZSQ',
    category: 'CONSUMABLE',
    specification: '5ml',
    model: '',
    unit: '支',
    price: 0.5,
    enabled: true,
    sort: 1,
    remark: ''
  },
  {
    id: 'eq-002',
    code: 'EQ002',
    name: '一次性输液器',
    pyCode: 'YXSYQ',
    category: 'CONSUMABLE',
    specification: '',
    model: '',
    unit: '套',
    price: 1.2,
    enabled: true,
    sort: 2,
    remark: ''
  },
  {
    id: 'eq-003',
    code: 'EQ003',
    name: '医用纱布',
    pyCode: 'YYSB',
    category: 'CONSUMABLE',
    specification: '5cm*10cm',
    model: '',
    unit: '包',
    price: 3.0,
    enabled: true,
    sort: 3,
    remark: ''
  },
  {
    id: 'eq-004',
    code: 'EQ004',
    name: '手术刀片',
    pyCode: 'SSDP',
    category: 'INSTRUMENT',
    specification: '22#',
    model: '',
    unit: '片',
    price: 8.0,
    enabled: true,
    sort: 4,
    remark: ''
  },
  {
    id: 'eq-005',
    code: 'EQ005',
    name: '血压计',
    pyCode: 'XYJ',
    category: 'EQUIPMENT',
    specification: '',
    model: '台式',
    unit: '台',
    price: 280.0,
    enabled: true,
    sort: 5,
    remark: ''
  },
  {
    id: 'eq-006',
    code: 'EQ006',
    name: '听诊器',
    pyCode: 'TZQ',
    category: 'INSTRUMENT',
    specification: '',
    model: '单用',
    unit: '个',
    price: 45.0,
    enabled: false,
    sort: 6,
    remark: ''
  }
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: EquipmentDetail): EquipmentRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    pyCode: detail.pyCode,
    category: detail.category,
    categoryName: categoryNameMap.get(detail.category) ?? detail.category,
    specification: detail.specification,
    model: detail.model,
    unit: detail.unit,
    price: detail.price,
    enabled: detail.enabled,
    sort: detail.sort,
    remark: detail.remark,
    updatedAt: (detail as EquipmentRecord).updatedAt ?? now()
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

function requireDetail(id: string): EquipmentDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('器材记录不存在')
  return detail
}

function validatePayload(payload: EquipmentSavePayload) {
  if (!payload.code?.trim()) throw new Error('器材编码不能为空')
  if (!payload.name?.trim()) throw new Error('器材名称不能为空')
  if (!payload.category) throw new Error('请选择器材分类')
  if (!payload.unit?.trim()) throw new Error('计量单位不能为空')
  if (payload.price < 0) throw new Error('单价不能为负数')
}

// ==================== Mock API ====================

export async function mockListEquipment(
  query: EquipmentQuery
): Promise<EquipmentPageResult> {
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

export async function mockGetEquipmentDetail(
  id: string
): Promise<EquipmentDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateEquipment(
  payload: EquipmentSavePayload
): Promise<EquipmentDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `eq-${String(sequence).padStart(3, '0')}`
  const detail: EquipmentDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateEquipment(
  id: string,
  payload: EquipmentSavePayload
): Promise<EquipmentDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetEquipmentEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveEquipment(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('器材记录不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadEquipmentFormOptions(): Promise<EquipmentFormOptions> {
  await delay()
  return {
    categoryOptions,
    unitOptions
  }
}
