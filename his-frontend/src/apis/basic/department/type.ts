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
  ybNo?: string
  ybName?: string
  medinsId?: string
  medinsAdmdvs?: string
  medinsType?: string
  medinsLv?: string
  caty?: string
  pyStr?: string
  wbStr?: string
  name: string
  parentId: string | null
  parentName: string
  type: string
  typeText?: string
  category: string
  categoryText?: string
  leaderId: string
  leaderName: string
  sort: number
  enabled: DepartmentEnabled
  children?: DepartmentRecord[]
  createdAt: string
  updatedAt: string
}

export interface DepartmentSavePayload {
  code?: string
  name: string
  parentId: string | null
  type: string
  category: string
  leaderId: string
  sort: number
  enabled: DepartmentEnabled
  ybNo?: string
  ybName?: string
  medinsId?: string
  medinsAdmdvs?: string
  medinsType?: string
  medinsLv?: string
  caty?: string
  pyStr?: string
  wbStr?: string
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
