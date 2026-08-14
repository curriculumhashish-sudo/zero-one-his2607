import type { DepartmentSavePayload } from '@/apis/basic/department'

export interface DepartmentFormModel extends DepartmentSavePayload {
  id: string
}

export type DepartmentEnabledFilter = '' | 'enabled' | 'disabled'

export interface DepartmentDialogOpenOptions {
  id?: string
  parentId?: string
}
