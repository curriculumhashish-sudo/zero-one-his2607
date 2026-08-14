import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface DocumentTypeRecord {
  id: string
  code: string
  name: string
  defaultTemplateId?: string
  isEnabled: number
  sortOrder: number
}

export interface DocumentTypeQuery extends BasicPageQuery {
  code?: string
  name?: string
  isEnabled?: number
}

export interface EmrTemplateRecord {
  id: string
  name: string
  code: string
  documentTypeCode: string
  organizationId?: string
  encounterClassEnum: number
  encounterTypeEnum: number
  content: string
  isEnabled: number
  scopeLevel: string
  ownerPractitionerId?: string
  encounterClassCode?: string
  createdAt?: string
  updatedAt?: string
  createdBy?: string
  updatedBy?: string
}

export interface EmrTemplateQuery extends BasicPageQuery {
  name?: string
  code?: string
  documentTypeCode?: string
  isEnabled?: number
  scopeLevel?: string
}

export interface MetadataFieldRecord {
  id: string
  fieldKey: string
  label: string
  dataType: string
  editableDefault: number
  valueSource: string
  sourcePath: string
  sourceConfig?: string
  defaultValue?: string
  maskRule: string
  formatPattern?: string
  sortOrder: number
  isEnabled: number
  remark?: string
  createdAt?: string
}

export interface MetadataFieldQuery extends BasicPageQuery {
  valueSource?: string
  isEnabled?: number
}

export type DocumentTypeSavePayload = Omit<DocumentTypeRecord, 'id'>
export type EmrTemplateSavePayload = Omit<EmrTemplateRecord, 'id' | 'createdAt' | 'updatedAt' | 'createdBy' | 'updatedBy'>
export type MetadataFieldSavePayload = Omit<MetadataFieldRecord, 'id' | 'createdAt'>

