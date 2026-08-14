import type { JSONContent } from '@tiptap/core'

/** 临床病历编辑器使用的文书类型选项。 */
export interface ClinicalMedicalRecordDocumentTypeOption {
  /** 稳定文书类型编码。 */
  value: string
  /** 展示名称。 */
  label: string
}

/** 病历状态标签颜色。 */
export type ClinicalMedicalRecordStatusType = 'primary' | 'success' | 'info' | 'warning' | 'danger'

/**
 * JSON 病历文档。
 *
 * 该结构遵循 Tiptap/ProseMirror JSON 文档格式，正文始终是对象而不是 HTML 字符串。
 */
export interface MedicalRecordJsonDocument extends JSONContent {
  type: 'doc'
  content?: JSONContent[]
}

/** 创建一个可直接交给 JSON 病历编辑器使用的空文档。 */
export function createEmptyMedicalRecordDocument(): MedicalRecordJsonDocument {
  return {
    type: 'doc',
    content: [{ type: 'paragraph' }]
  }
}

/** 判断未知数据是否具有 JSON 病历文档的根节点结构。 */
export function isMedicalRecordJsonDocument(value: unknown): value is MedicalRecordJsonDocument {
  if (!value || typeof value !== 'object') return false

  const document = value as Record<string, unknown>
  return (
    document.type === 'doc' && (document.content === undefined || Array.isArray(document.content))
  )
}
