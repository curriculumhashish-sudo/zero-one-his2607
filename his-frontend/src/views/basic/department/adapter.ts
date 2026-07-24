import type { DepartmentRecord } from '@/apis/basic/department'
import type { DepartmentFormModel } from './type'

export function createEmptyDepartmentForm(parentId: string | null = null): DepartmentFormModel {
  return {
    id: '',
    name: '',
    parentId,
    type: '临床科室',
    category: '门诊',
    leaderId: '',
    sort: 0,
    enabled: true
  }
}

export function toDepartmentForm(department: DepartmentRecord): DepartmentFormModel {
  return {
    id: department.id,
    name: department.name,
    parentId: department.parentId,
    type: department.type,
    category: department.category,
    leaderId: department.leaderId,
    sort: department.sort,
    enabled: department.enabled
  }
}
