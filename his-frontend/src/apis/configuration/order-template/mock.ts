import { createCrudStore } from '@/apis/basic/crud-mock'
import type { OrderTemplateRecord, OrderTemplateSavePayload } from './type'

const store = createCrudStore<OrderTemplateRecord>(
  'order-template',
  [
    seed(
      'order-template-001',
      'OT-COLD',
      '上呼吸道感染组套',
      'GENERAL',
      '呼吸内科',
      '血常规、CRP、口服药品',
      'ENABLED',
      '常用门诊治疗组套'
    ),
    seed(
      'order-template-002',
      'OT-CT',
      '胸部 CT 检查组套',
      'PRICING',
      '呼吸内科',
      '胸部 CT 平扫',
      'ENABLED',
      '影像检查组套'
    )
  ],
  ['code', 'name', 'departmentName', 'itemSummary', 'description']
)

function seed(
  id: string,
  code: string,
  name: string,
  templateType: OrderTemplateRecord['templateType'],
  departmentName: string,
  itemSummary: string,
  status: OrderTemplateRecord['status'],
  description: string
): OrderTemplateRecord {
  return {
    id,
    code,
    name,
    templateType,
    departmentName,
    itemSummary,
    status,
    statusText: status === 'ENABLED' ? '启用' : '停用',
    description
  }
}

function normalize(payload: OrderTemplateSavePayload): OrderTemplateSavePayload {
  if (!payload.code.trim() || !payload.name.trim()) throw new Error('组套编码和组套名称不能为空')
  return {
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    departmentName: payload.departmentName.trim(),
    itemSummary: payload.itemSummary.trim(),
    description: payload.description.trim()
  }
}

export async function mockCreateOrderTemplate(payload: OrderTemplateSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (rows.some((item) => item.code.toLowerCase() === normalized.code.toLowerCase()))
    throw new Error('组套编码已存在')
  return store.create({
    ...normalized,
    statusText: normalized.status === 'ENABLED' ? '启用' : '停用'
  })
}

export async function mockUpdateOrderTemplate(id: string, payload: OrderTemplateSavePayload) {
  const normalized = normalize(payload)
  const rows = await store.list()
  if (
    rows.some((item) => item.id !== id && item.code.toLowerCase() === normalized.code.toLowerCase())
  )
    throw new Error('组套编码已存在')
  return store.update(id, {
    ...normalized,
    statusText: normalized.status === 'ENABLED' ? '启用' : '停用'
  })
}

export async function mockLoadOrderTemplateNameOptions() {
  return (await store.list()).map((item) => ({ label: item.name, value: item.id }))
}

export { store as orderTemplateStore }
