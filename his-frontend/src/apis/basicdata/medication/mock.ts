import type {
  MedicationDetail,
  MedicationFormOptions,
  MedicationPageResult,
  MedicationQuery,
  MedicationRecord,
  MedicationSavePayload,
  SelectOption
} from './type'

// ==================== 下拉选项 ====================

const categoryOptions: SelectOption[] = [
  { label: '西药', value: 'WM' },
  { label: '中成药', value: 'TCM' },
  { label: '中草药', value: 'HM' },
  { label: '生物制品', value: 'BP' }
]

const dosageFormOptions: SelectOption[] = [
  { label: '片剂', value: 'TABLET' },
  { label: '胶囊', value: 'CAPSULE' },
  { label: '注射液', value: 'INJECTION' },
  { label: '口服液', value: 'ORAL_LIQUID' },
  { label: '颗粒剂', value: 'GRANULE' },
  { label: '软膏', value: 'OINTMENT' }
]

// ==================== 类别名称映射 ====================

const categoryNameMap = new Map(categoryOptions.map((item) => [item.value, item.label]))
const dosageFormNameMap = new Map(dosageFormOptions.map((item) => [item.value, item.label]))

// ==================== Mock 数据 ====================

let sequence = 6

function now(): string {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

function seedDetail(
  id: string,
  code: string,
  name: string,
  genericName: string,
  category: string,
  specification: string,
  dosageForm: string,
  unit: string,
  manufacturer: string,
  retailPrice: number,
  enabled: boolean,
  sort: number,
  remark = '',
  overrides: Partial<MedicationDetail> = {}
): MedicationDetail {
  return {
    id,
    code,
    name,
    genericName,
    category,
    specification,
    dosageForm,
    unit,
    manufacturer,
    retailPrice,
    enabled,
    sort,
    remark,
    ...overrides
  }
}

const details: MedicationDetail[] = [
  seedDetail(
    'med-001', 'MED001', '阿莫西林胶囊', '阿莫仙', 'WM',
    '0.25g*50粒', 'CAPSULE', '盒', '珠海联邦制药', 14.8, true, 1
  ),
  seedDetail(
    'med-002', 'MED002', '布洛芬缓释胶囊', '芬必得', 'WM',
    '0.3g*20粒', 'CAPSULE', '盒', '中美天津史克', 21.6, true, 2
  ),
  seedDetail(
    'med-003', 'MED003', '氯化钠注射液', '生理盐水', 'WM',
    '0.9%*250ml', 'INJECTION', '瓶', '石家庄四药', 4.2, true, 3
  ),
  seedDetail(
    'med-004', 'MED004', '板蓝根颗粒', '板蓝根', 'TCM',
    '10g*20袋', 'GRANULE', '盒', '广州白云山', 10.2, true, 4
  ),
  seedDetail(
    'med-005', 'MED005', '头孢克肟分散片', '世福素', 'WM',
    '0.1g*6片', 'TABLET', '盒', '广州白云山', 26.4, false, 5
  ),
  seedDetail(
    'med-006', 'MED006', '双黄连口服液', '双黄连', 'TCM',
    '10ml*10支', 'ORAL_LIQUID', '盒', '哈药三精', 14.2, true, 6
  )
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: MedicationDetail): MedicationRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    genericName: detail.genericName,
    category: detail.category,
    categoryName: categoryNameMap.get(detail.category) ?? detail.category,
    specification: detail.specification,
    dosageForm: detail.dosageForm,
    dosageFormName: dosageFormNameMap.get(detail.dosageForm) ?? detail.dosageForm,
    unit: detail.unit,
    manufacturer: detail.manufacturer,
    retailPrice: detail.retailPrice,
    enabled: detail.enabled,
    sort: detail.sort,
    remark: detail.remark,
    updatedAt: (detail as MedicationRecord).updatedAt ?? now()
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

function requireDetail(id: string): MedicationDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('药品不存在')
  return detail
}

function validatePayload(payload: MedicationSavePayload) {
  if (!payload.code?.trim()) throw new Error('药品编码不能为空')
  if (!payload.name?.trim()) throw new Error('药品名称不能为空')
  if (!payload.category) throw new Error('请选择药品类别')
  if (!payload.specification?.trim()) throw new Error('规格不能为空')
  if (!payload.dosageForm) throw new Error('请选择剂型')
  if (!payload.unit?.trim()) throw new Error('单位不能为空')
  if (payload.retailPrice < 0) throw new Error('零售价不能为负数')
}

// ==================== Mock API ====================

export async function mockListMedication(
  query: MedicationQuery
): Promise<MedicationPageResult> {
  await delay()
  let filtered = details.slice()

  const keyword = query.keyword?.trim().toLowerCase()
  if (keyword) {
    filtered = filtered.filter(
      (item) =>
        item.name.toLowerCase().includes(keyword) ||
        item.genericName.toLowerCase().includes(keyword) ||
        item.code.toLowerCase().includes(keyword)
    )
  }
  if (query.category) {
    filtered = filtered.filter((item) => item.category === query.category)
  }
  if (query.enabled != null) {
    filtered = filtered.filter((item) => item.enabled === query.enabled)
  }

  const total = filtered.length
  const start = (query.pageIndex - 1) * query.pageSize
  const pageRows = filtered.slice(start, start + query.pageSize).map((item) => {
    const record = toRecord(item)
    record.updatedAt = getRecordTimestamp(item.id ?? '')
    return record
  })

  return { rows: pageRows, total }
}

export async function mockGetMedicationDetail(
  id: string
): Promise<MedicationDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateMedication(
  payload: MedicationSavePayload
): Promise<MedicationDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `med-${String(sequence).padStart(3, '0')}`
  const detail: MedicationDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateMedication(
  id: string,
  payload: MedicationSavePayload
): Promise<MedicationDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetMedicationEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveMedication(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('药品不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadMedicationFormOptions(): Promise<MedicationFormOptions> {
  await delay()
  return {
    categoryOptions,
    dosageFormOptions
  }
}
