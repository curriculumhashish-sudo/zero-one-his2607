import { createCrudStore } from '@/apis/basic/crud-mock'
import type {
  DocumentTypeRecord,
  DocumentTypeSavePayload,
  EmrTemplateRecord,
  EmrTemplateSavePayload,
  MetadataFieldRecord,
  MetadataFieldSavePayload
} from './type'

const documentTypeStore = createCrudStore<DocumentTypeRecord>(
  'document-type',
  [
    {
      id: 'document-type-001',
      code: 'OUTPATIENT',
      name: '门诊病历',
      description: '门诊接诊时使用的病历文书'
    },
    {
      id: 'document-type-002',
      code: 'ADMISSION',
      name: '入院记录',
      description: '住院患者入院时使用的文书'
    },
    {
      id: 'document-type-003',
      code: 'PROGRESS',
      name: '病程记录',
      description: '住院期间病程记录文书'
    }
  ],
  ['code', 'name', 'description']
)

const templateStore = createCrudStore<EmrTemplateRecord>(
  'emr-template',
  [
    seedTemplate(
      'emr-template-001',
      'OP-FIRST',
      '门诊初诊模板',
      'OUTPATIENT',
      '门诊',
      '<p>主诉、现病史、诊断与处理意见。</p>',
      true
    ),
    seedTemplate(
      'emr-template-002',
      'ADMISSION-GENERAL',
      '通用入院记录',
      'ADMISSION',
      '住院',
      '<p>一般情况、既往史、体格检查。</p>',
      true
    )
  ],
  ['code', 'name', 'documentTypeName', 'applicableScope']
)

const metadataStore = createCrudStore<MetadataFieldRecord>(
  'metadata-field',
  [
    {
      id: 'metadata-field-001',
      code: 'PATIENT_NAME',
      name: '患者姓名',
      dataType: 'TEXT',
      defaultValue: '',
      description: '患者基本信息字段',
      enabled: true
    },
    {
      id: 'metadata-field-002',
      code: 'VISIT_DATE',
      name: '就诊日期',
      dataType: 'DATE',
      defaultValue: 'CURRENT_DATE',
      description: '本次就诊日期',
      enabled: true
    },
    {
      id: 'metadata-field-003',
      code: 'DIAGNOSIS',
      name: '诊断',
      dataType: 'TEXT',
      defaultValue: '',
      description: '诊断结论字段',
      enabled: true
    }
  ],
  ['code', 'name', 'dataType', 'description']
)

function getDocumentTypeName(code: string) {
  return { OUTPATIENT: '门诊病历', ADMISSION: '入院记录', PROGRESS: '病程记录' }[code] ?? code
}

function seedTemplate(
  id: string,
  code: string,
  name: string,
  documentTypeCode: string,
  applicableScope: string,
  content: string,
  enabled: boolean
): EmrTemplateRecord {
  return {
    id,
    code,
    name,
    documentTypeCode,
    documentTypeName: getDocumentTypeName(documentTypeCode),
    applicableScope,
    content,
    enabled
  }
}

function requireCode(code: string, label: string) {
  if (!code.trim()) throw new Error(`${label}不能为空`)
}

export async function mockCreateDocumentType(payload: DocumentTypeSavePayload) {
  requireCode(payload.code, '文书类型编码')
  requireCode(payload.name, '文书类型名称')
  const rows = await documentTypeStore.list()
  if (rows.some((item) => item.code.toLowerCase() === payload.code.trim().toLowerCase()))
    throw new Error('文书类型编码已存在')
  return documentTypeStore.create({
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim()
  })
}

export async function mockUpdateDocumentType(id: string, payload: DocumentTypeSavePayload) {
  requireCode(payload.code, '文书类型编码')
  requireCode(payload.name, '文书类型名称')
  const rows = await documentTypeStore.list()
  if (
    rows.some(
      (item) => item.id !== id && item.code.toLowerCase() === payload.code.trim().toLowerCase()
    )
  )
    throw new Error('文书类型编码已存在')
  return documentTypeStore.update(id, {
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim()
  })
}

export async function mockCreateEmrTemplate(payload: EmrTemplateSavePayload) {
  requireCode(payload.code, '病历模板编码')
  requireCode(payload.name, '病历模板名称')
  const rows = await templateStore.list()
  if (rows.some((item) => item.code.toLowerCase() === payload.code.trim().toLowerCase()))
    throw new Error('病历模板编码已存在')
  return templateStore.create({
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    documentTypeName: getDocumentTypeName(payload.documentTypeCode)
  })
}

export async function mockUpdateEmrTemplate(id: string, payload: EmrTemplateSavePayload) {
  requireCode(payload.code, '病历模板编码')
  requireCode(payload.name, '病历模板名称')
  const rows = await templateStore.list()
  if (
    rows.some(
      (item) => item.id !== id && item.code.toLowerCase() === payload.code.trim().toLowerCase()
    )
  )
    throw new Error('病历模板编码已存在')
  return templateStore.update(id, {
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    documentTypeName: getDocumentTypeName(payload.documentTypeCode)
  })
}

export async function mockCreateMetadataField(payload: MetadataFieldSavePayload) {
  requireCode(payload.code, '元数据字段编码')
  requireCode(payload.name, '元数据字段名称')
  const rows = await metadataStore.list()
  if (rows.some((item) => item.code.toLowerCase() === payload.code.trim().toLowerCase()))
    throw new Error('元数据字段编码已存在')
  return metadataStore.create({
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim()
  })
}

export async function mockUpdateMetadataField(id: string, payload: MetadataFieldSavePayload) {
  requireCode(payload.code, '元数据字段编码')
  requireCode(payload.name, '元数据字段名称')
  const rows = await metadataStore.list()
  if (
    rows.some(
      (item) => item.id !== id && item.code.toLowerCase() === payload.code.trim().toLowerCase()
    )
  )
    throw new Error('元数据字段编码已存在')
  return metadataStore.update(id, {
    ...payload,
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim()
  })
}

export async function mockLoadDocumentTypeNameOptions() {
  return (await documentTypeStore.list()).map((item) => ({ label: item.name, value: item.code }))
}

export async function mockLoadMetadataFieldNameOptions() {
  return (await metadataStore.list()).map((item) => ({ label: item.name, value: item.code }))
}

export { documentTypeStore, metadataStore, templateStore }
