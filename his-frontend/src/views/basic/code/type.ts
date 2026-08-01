import type { CodeItemSavePayload, CodeTypeSavePayload } from '@/apis/basic/code'

/** 当前激活的编码系统 */
export type CodeSystem = 'dict' | 'enum'

/** 类型表单模式 */
export type TypeFormMode = 'create' | 'edit'

/** 编码项表单模式 */
export type ItemFormMode = 'create' | 'edit'

/** 类型表单模型 */
export interface TypeFormModel extends CodeTypeSavePayload {
  id: string
}

/** 编码项表单模型 */
export interface ItemFormModel extends CodeItemSavePayload {
  id: string
  typeId: string
}

/** 创建空的类型表单 */
export function createEmptyTypeForm(): TypeFormModel {
  return { id: '', name: '', title: '', version: '', description: '' }
}

/** 创建空的编码项表单 */
export function createEmptyItemForm(typeId?: string): ItemFormModel {
  return { id: '', code: '', name: '', description: '', sort: 0, typeId: typeId ?? '' }
}
