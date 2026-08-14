import type { DepartmentRecord } from '@/apis/basic/department'
import type { DepartmentFormModel } from './type'

export function createEmptyDepartmentForm(parentId: string | null = null): DepartmentFormModel {
  return {
    id: '',
    code: '',
    name: '',
    parentId,
    type: 'DEPARTMENT',
    category: 'OUTPATIENT',
    leaderId: '',
    sort: 0,
    enabled: true,
    ybNo: '',
    ybName: '',
    medinsId: '',
    medinsAdmdvs: '',
    medinsType: '',
    medinsLv: '',
    caty: '',
    pyStr: '',
    wbStr: ''
  }
}

export function toDepartmentForm(department: DepartmentRecord): DepartmentFormModel {
  return {
    id: department.id,
    code: department.code,
    name: department.name,
    parentId: department.parentId,
    type: department.type,
    category: department.category,
    leaderId: department.leaderId,
    sort: department.sort,
    enabled: department.enabled,
    ybNo: department.ybNo ?? '',
    ybName: department.ybName ?? '',
    medinsId: department.medinsId ?? '',
    medinsAdmdvs: department.medinsAdmdvs ?? '',
    medinsType: department.medinsType ?? '',
    medinsLv: department.medinsLv ?? '',
    caty: department.caty ?? '',
    pyStr: department.pyStr ?? '',
    wbStr: department.wbStr ?? ''
  }
}
