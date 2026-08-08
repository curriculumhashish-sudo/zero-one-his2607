import { toC7PageResult, toMutationResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type {
  DocumentTypeQuery,
  DocumentTypeRecord,
  DocumentTypeSavePayload,
  EmrTemplateQuery,
  EmrTemplateRecord,
  EmrTemplateSavePayload,
  MetadataFieldQuery,
  MetadataFieldRecord,
  MetadataFieldSavePayload
} from './type'

const baseUrl = '/c7-configuration-api'

export async function loadDocumentTypeNameOptions() {
  const page = unwrap(await useHttp().get<{ rows?: Array<{ id: string; name: string }> }>(`${baseUrl}/DocType/query-names`, {
    pageIndex: 1,
    pageSize: 1000
  }))
  const rows = Array.isArray(page.rows) ? page.rows : []
  return rows.map((row) => ({ label: row.name, value: row.id }))
}

export async function pageDocumentTypes(query: DocumentTypeQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/DocType`, toC7Query(query, ['code', 'name', 'isEnabled'])))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function getDocumentTypeDetail(id: string) {
  return unwrap(await useHttp().get<DocumentTypeRecord>(`${baseUrl}/DocType/query-one`, { id }))
}

export async function createDocumentType(data: DocumentTypeSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/add-document-type`, data)))
}

export async function updateDocumentType(id: string, data: DocumentTypeSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/modify-document-type`, { id, ...data })))
}

export async function removeDocumentType(id: string) {
  unwrap(await useHttp().post(`${baseUrl}/casetemplate/remove-document-type`, { ids: [id] }))
}

export async function pageEmrTemplates(query: EmrTemplateQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/casetemplate/query-all-emr-template`, toC7Query(query, [
    'name', 'code', 'documentTypeCode', 'isEnabled', 'scopeLevel'
  ])))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function getEmrTemplateDetail(id: string) {
  return unwrap(await useHttp().get<EmrTemplateRecord>(`${baseUrl}/casetemplate/query-one-emr-template`, { id }))
}

export async function createEmrTemplate(data: EmrTemplateSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/add-emr-template`, toEmrTemplatePayload(data))))
}

export async function updateEmrTemplate(id: string, data: EmrTemplateSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/modify-emr-template`, toEmrTemplatePayload({ id, ...data }))))
}

export async function removeEmrTemplate(id: string) {
  unwrap(await useHttp().post(`${baseUrl}/casetemplate/remove-emr-template`, { ids: [id] }))
}

export async function loadMetadataFieldNameOptions() {
  const rows = unwrap(await useHttp().get<Array<{ fieldKey: string; label: string }>>(`${baseUrl}/casetemplate/query-metadata-field-names`))
  return rows.map((row) => ({ label: row.label, value: row.fieldKey }))
}

export async function pageMetadataFields(query: MetadataFieldQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/casetemplate/query-all-metadata-field`, toC7Query(query, [
    'keyword', 'valueSource', 'isEnabled'
  ])))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function getMetadataFieldDetail(id: string) {
  return unwrap(await useHttp().get<MetadataFieldRecord>(`${baseUrl}/casetemplate/query-one-metadata-field`, { id }))
}

export async function createMetadataField(data: MetadataFieldSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/add-metadata-field`, data)))
}

export async function updateMetadataField(id: string, data: MetadataFieldSavePayload) {
  return toMutationResult(unwrap(await useHttp().post<string>(`${baseUrl}/casetemplate/modify-metadata-field`, { id, ...data })))
}

export async function removeMetadataField(id: string) {
  unwrap(await useHttp().post(`${baseUrl}/casetemplate/remove-metadata-field`, id))
}

function toC7Query(query: { pageIndex: number; pageSize: number } & object, fields: string[]) {
  const { pageIndex, pageSize, ...filters } = query
  const params: Record<string, unknown> = { pageIndex, pageSize }
  const filterValues = filters as Record<string, unknown>
  for (const field of fields) {
    if (filterValues[field] !== undefined && filterValues[field] !== '') params[field] = filterValues[field]
  }
  return params
}

function toEmrTemplatePayload(data: EmrTemplateSavePayload & { id?: string }) {
  const payload = {
    name: data.name,
    code: data.code,
    documentTypeCode: data.documentTypeCode,
    organizationId: data.organizationId,
    encounterClassEnum: Number(data.encounterClassEnum),
    encounterTypeEnum: Number(data.encounterTypeEnum),
    content: data.content,
    isEnabled: Number(data.isEnabled),
    scopeLevel: data.scopeLevel,
    ownerPractitionerId: data.ownerPractitionerId,
    encounterClassCode: data.encounterClassCode
  }
  if (data.id) {
    return {
      id: data.id,
      ...payload
    }
  }
  return {
    ...payload,
  }
}

export type * from './type'
