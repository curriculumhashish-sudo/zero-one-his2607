import type {
  SampleDetail,
  SampleFormOptions,
  SamplePageResult,
  SampleQuery,
  SampleRecord,
  SampleSavePayload,
  SelectOption
} from './type'

// ==================== 下拉选项 ====================

const typeOptions: SelectOption[] = [
  { label: '血液', value: 'BLOOD' },
  { label: '尿液', value: 'URINE' },
  { label: '组织', value: 'TISSUE' },
  { label: '粪便', value: 'STOOL' },
  { label: '痰液', value: 'SPUTUM' },
  { label: '其他', value: 'OTHER' }
]

// ==================== 类型名称映射 ====================

const typeNameMap = new Map(typeOptions.map((item) => [item.value, item.label]))

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
  type: string,
  collectionMethod: string,
  storageCondition: string,
  validHours: number,
  enabled: boolean,
  sort: number,
  remark = ''
): SampleDetail {
  return {
    id,
    code,
    name,
    pyCode,
    type,
    collectionMethod,
    storageCondition,
    validHours,
    enabled,
    sort,
    remark
  }
}

const details: SampleDetail[] = [
  seedDetail('sam-001', 'SAM001', '全血样本', 'QXYB', 'BLOOD', '静脉穿刺', '2-8°C冷藏', 24, true, 1),
  seedDetail('sam-002', 'SAM002', '血清样本', 'QXYB', 'BLOOD', '离心分离', '2-8°C冷藏', 8, true, 2),
  seedDetail('sam-003', 'SAM003', '尿液样本', 'NYXB', 'URINE', '自然排尿', '常温', 4, true, 3),
  seedDetail('sam-004', 'SAM004', '粪便样本', 'FBXB', 'STOOL', '自然采集', '常温', 2, true, 4),
  seedDetail('sam-005', 'SAM005', '痰液样本', 'TYXB', 'SPUTUM', '深咳采集', '2-8°C冷藏', 4, true, 5),
  seedDetail('sam-006', 'SAM006', '组织样本', 'ZZXB', 'TISSUE', '穿刺活检', '福尔马林固定', 72, false, 6)
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: SampleDetail): SampleRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    pyCode: detail.pyCode,
    type: detail.type,
    typeName: typeNameMap.get(detail.type) ?? detail.type,
    collectionMethod: detail.collectionMethod,
    storageCondition: detail.storageCondition,
    validHours: detail.validHours,
    enabled: detail.enabled,
    sort: detail.sort,
    updatedAt: now()
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

function requireDetail(id: string): SampleDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('样本不存在')
  return detail
}

function validatePayload(payload: SampleSavePayload) {
  if (!payload.code?.trim()) throw new Error('样本编码不能为空')
  if (!payload.name?.trim()) throw new Error('样本名称不能为空')
  if (!payload.type) throw new Error('请选择样本类型')
  if (payload.validHours < 0) throw new Error('有效时长不能为负数')
}

// ==================== Mock API ====================

export async function mockListSample(query: SampleQuery): Promise<SamplePageResult> {
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

export async function mockGetSampleDetail(id: string): Promise<SampleDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateSample(payload: SampleSavePayload): Promise<SampleDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `sam-${String(sequence).padStart(3, '0')}`
  const detail: SampleDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateSample(
  id: string,
  payload: SampleSavePayload
): Promise<SampleDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetSampleEnabled(id: string, enabled: boolean): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveSample(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('样本不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadSampleFormOptions(): Promise<SampleFormOptions> {
  await delay()
  return {
    typeOptions
  }
}
