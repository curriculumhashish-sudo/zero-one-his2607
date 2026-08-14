import type {
  FrequencyDetail,
  FrequencyFormOptions,
  FrequencyPageResult,
  FrequencyQuery,
  FrequencyRecord,
  FrequencySavePayload,
  SelectOption
} from './type'

// ==================== 下拉选项 ====================

const typeOptions: SelectOption[] = [
  { label: '每日', value: 'DAILY' },
  { label: '每周', value: 'WEEKLY' },
  { label: '每小时', value: 'HOURLY' },
  { label: '按需', value: 'AS_NEEDED' }
]

// ==================== 类别名称映射 ====================

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
  type: string,
  timesPerDay: number,
  intervalHours: number,
  timePoints: string,
  enabled: boolean,
  sort: number,
  remark: string = ''
): FrequencyDetail {
  return {
    id,
    code,
    name,
    type,
    timesPerDay,
    intervalHours,
    timePoints,
    enabled,
    sort,
    remark
  }
}

const details: FrequencyDetail[] = [
  seedDetail(
    'freq-001', 'QD', '每日一次', 'DAILY', 1, 24, '08:00', true, 1, '每日清晨执行一次'
  ),
  seedDetail(
    'freq-002', 'BID', '每日两次', 'DAILY', 2, 12, '08:00,20:00', true, 2, '早晚各执行一次'
  ),
  seedDetail(
    'freq-003', 'TID', '每日三次', 'DAILY', 3, 8, '08:00,12:00,20:00', true, 3, '三餐后执行'
  ),
  seedDetail(
    'freq-004', 'QID', '每日四次', 'DAILY', 4, 6, '08:00,12:00,16:00,20:00', true, 4, '日间每6小时一次'
  ),
  seedDetail(
    'freq-005', 'Q4H', '每4小时一次', 'HOURLY', 6, 4, '08:00,12:00,16:00,20:00,00:00,04:00', true, 5, '全天每4小时一次'
  ),
  seedDetail(
    'freq-006', 'PRN', '必要时', 'AS_NEEDED', 0, 0, '遵医嘱', true, 6, '根据患者需要遵医嘱执行'
  )
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: FrequencyDetail): FrequencyRecord {
  return {
    id: detail.id ?? '',
    code: detail.code,
    name: detail.name,
    type: detail.type,
    typeName: typeNameMap.get(detail.type) ?? detail.type,
    timesPerDay: detail.timesPerDay,
    intervalHours: detail.intervalHours,
    timePoints: detail.timePoints,
    enabled: detail.enabled,
    sort: detail.sort,
    updatedAt: ''
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

function requireDetail(id: string): FrequencyDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('频次不存在')
  return detail
}

function validatePayload(payload: FrequencySavePayload) {
  if (!payload.code?.trim()) throw new Error('频次编码不能为空')
  if (!payload.name?.trim()) throw new Error('频次名称不能为空')
  if (!payload.type) throw new Error('请选择频次类型')
  if (payload.timesPerDay < 0) throw new Error('每日执行次数不能为负数')
  if (payload.intervalHours < 0) throw new Error('间隔时间不能为负数')
}

// ==================== Mock API ====================

export async function mockListFrequency(
  query: FrequencyQuery
): Promise<FrequencyPageResult> {
  await delay()
  let filtered = details.slice()

  const keyword = query.keyword?.trim().toLowerCase()
  if (keyword) {
    filtered = filtered.filter(
      (item) =>
        item.name.toLowerCase().includes(keyword) ||
        item.code.toLowerCase().includes(keyword)
    )
  }
  if (query.type) {
    filtered = filtered.filter((item) => item.type === query.type)
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

export async function mockGetFrequencyDetail(
  id: string
): Promise<FrequencyDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateFrequency(
  payload: FrequencySavePayload
): Promise<FrequencyDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `freq-${String(sequence).padStart(3, '0')}`
  const detail: FrequencyDetail = {
    ...payload,
    id
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateFrequency(
  id: string,
  payload: FrequencySavePayload
): Promise<FrequencyDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetFrequencyEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.enabled = enabled
  recordTimestamps.set(id, now())
}

export async function mockRemoveFrequency(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('频次不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadFrequencyFormOptions(): Promise<FrequencyFormOptions> {
  await delay()
  return {
    typeOptions
  }
}
