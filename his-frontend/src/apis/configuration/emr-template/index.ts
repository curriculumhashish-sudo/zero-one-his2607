import type {
  DocumentTypeSavePayload,
  EmrTemplateQuery,
  EmrTemplateSavePayload,
  MetadataFieldSavePayload
} from './type'
import {
  documentTypeStore,
  metadataStore,
  mockCreateDocumentType,
  mockCreateEmrTemplate,
  mockCreateMetadataField,
  mockLoadDocumentTypeNameOptions,
  mockLoadMetadataFieldNameOptions,
  mockUpdateDocumentType,
  mockUpdateEmrTemplate,
  mockUpdateMetadataField,
  templateStore
} from './mock'

/** 获取文书类型名称。 */
export const loadDocumentTypeNameOptions = () => mockLoadDocumentTypeNameOptions()
/** 获取文书类型列表（条件 + 分页）。 */
export const pageDocumentTypes = (query: EmrTemplateQuery) => documentTypeStore.page(query)
/** 获取文书类型详情。 */
export const getDocumentTypeDetail = (id: string) => documentTypeStore.detail(id)
/** 新增文书类型。 */
export const createDocumentType = (data: DocumentTypeSavePayload) => mockCreateDocumentType(data)
/** 修改文书类型。 */
export const updateDocumentType = (id: string, data: DocumentTypeSavePayload) =>
  mockUpdateDocumentType(id, data)
/** 删除文书类型。 */
export const removeDocumentType = (id: string) => documentTypeStore.remove(id)

/** 获取病历模板列表。 */
export const pageEmrTemplates = (query: EmrTemplateQuery) => templateStore.page(query)
/** 获取病历模板详情。 */
export const getEmrTemplateDetail = (id: string) => templateStore.detail(id)
/** 新增病历模板。 */
export const createEmrTemplate = (data: EmrTemplateSavePayload) => mockCreateEmrTemplate(data)
/** 修改病历模板。 */
export const updateEmrTemplate = (id: string, data: EmrTemplateSavePayload) =>
  mockUpdateEmrTemplate(id, data)
/** 删除病历模板。 */
export const removeEmrTemplate = (id: string) => templateStore.remove(id)

/** 获取元数据字段名称。 */
export const loadMetadataFieldNameOptions = () => mockLoadMetadataFieldNameOptions()
/** 获取元数据字段列表（条件 + 分页）。 */
export const pageMetadataFields = (query: EmrTemplateQuery) => metadataStore.page(query)
/** 获取元数据字段详情。 */
export const getMetadataFieldDetail = (id: string) => metadataStore.detail(id)
/** 新增元数据字段。 */
export const createMetadataField = (data: MetadataFieldSavePayload) => mockCreateMetadataField(data)
/** 修改元数据字段。 */
export const updateMetadataField = (id: string, data: MetadataFieldSavePayload) =>
  mockUpdateMetadataField(id, data)
/** 删除元数据字段。 */
export const removeMetadataField = (id: string) => metadataStore.remove(id)

export type * from './type'
