import { createCrudStore } from '@/apis/basic/crud-mock'
import type { ExecuteLocationRecord, ExecuteLocationSavePayload } from './type'

const store = createCrudStore<ExecuteLocationRecord>(
  'execute-location',
  [
    {
      id: 'execute-location-001',
      departmentName: '检验科',
      itemCode: 'LAB-BLOOD',
      itemName: '血常规',
      serviceCategory: '检验',
      startDate: '2026-01-01',
      endDate: '',
      description: '门诊、住院均可执行'
    },
    {
      id: 'execute-location-002',
      departmentName: '放射科',
      itemCode: 'RAD-CT',
      itemName: '胸部 CT',
      serviceCategory: '检查',
      startDate: '2026-01-01',
      endDate: '',
      description: '需要预约检查'
    }
  ],
  ['departmentName', 'itemCode', 'itemName', 'serviceCategory']
)

function normalize(payload: ExecuteLocationSavePayload): ExecuteLocationSavePayload {
  if (!payload.departmentName.trim() || !payload.itemCode.trim() || !payload.itemName.trim())
    throw new Error('执行科室、项目编码和项目名称不能为空')
  return {
    ...payload,
    departmentName: payload.departmentName.trim(),
    itemCode: payload.itemCode.trim(),
    itemName: payload.itemName.trim(),
    serviceCategory: payload.serviceCategory.trim(),
    description: payload.description.trim()
  }
}

export async function mockCreateExecuteLocation(payload: ExecuteLocationSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (
    rows.some(
      (item) =>
        item.departmentName === normalized.departmentName && item.itemCode === normalized.itemCode
    )
  )
    throw new Error('该科室已配置此执行项目')
  return store.create(normalized)
}

export async function mockUpdateExecuteLocation(id: string, payload: ExecuteLocationSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (
    rows.some(
      (item) =>
        item.id !== id &&
        item.departmentName === normalized.departmentName &&
        item.itemCode === normalized.itemCode
    )
  )
    throw new Error('该科室已配置此执行项目')
  return store.update(id, normalized)
}

export async function mockLoadExecuteDepartmentOptions() {
  return ['检验科', '放射科', '超声科', '心电图室'].map((name) => ({ label: name, value: name }))
}

export { store as executeLocationStore }
