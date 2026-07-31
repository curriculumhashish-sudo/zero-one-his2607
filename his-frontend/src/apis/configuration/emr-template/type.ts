import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface DocumentTypeRecord {
  id: string
  code: string
  name: string
  description: string
}

export interface EmrTemplateRecord {
  id: string
  code: string
  name: string
  documentTypeCode: string
  documentTypeName: string
  applicableScope: string
  content: string
  enabled: boolean
}

export interface MetadataFieldRecord {
  id: string
  code: string
  name: string
  dataType: string
  defaultValue: string
  description: string
  enabled: boolean
}

export interface EmrTemplateQuery extends BasicPageQuery {}
export type DocumentTypeSavePayload = Omit<DocumentTypeRecord, 'id'>
export type EmrTemplateSavePayload = Omit<EmrTemplateRecord, 'id' | 'documentTypeName'>
export type MetadataFieldSavePayload = Omit<MetadataFieldRecord, 'id'>
export interface NameOption {
  label: string
  value: string
}
