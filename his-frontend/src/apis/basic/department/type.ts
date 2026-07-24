export type DepartmentEnabled = boolean

export interface DepartmentQuery {
  keyword?: string
  type?: string
  category?: string
  enabled?: DepartmentEnabled | null
}

export interface DepartmentRecord {
  id: string
  code: string
  name: string
  parentId: string | null
  parentName: string
  type: string
  category: string
  leaderId: string
  leaderName: string
  sort: number
  enabled: DepartmentEnabled
  children?: DepartmentRecord[]
  createdAt: string
  updatedAt: string
}

export interface DepartmentSavePayload {
  name: string
  parentId: string | null
  type: string
  category: string
  leaderId: string
  sort: number
  enabled: DepartmentEnabled
}

export interface SelectOption {
  label: string
  value: string
}

export interface DepartmentTreeOption extends SelectOption {
  children?: DepartmentTreeOption[]
}

export interface DepartmentFormOptions {
  typeOptions: SelectOption[]
  categoryOptions: SelectOption[]
  leaderOptions: SelectOption[]
  departmentOptions: DepartmentTreeOption[]
}
