import type { BasicPageQuery, BasicPageResult } from '@/apis/basic/crud-mock'

export interface BasicCrudRecord extends Record<string, unknown> {
  id: string
}

export interface BasicCrudService {
  page: (query: BasicPageQuery) => Promise<BasicPageResult<BasicCrudRecord>>
  detail?: (id: string) => Promise<BasicCrudRecord>
  create: (data: Record<string, unknown>) => Promise<BasicCrudRecord>
  update: (id: string, data: Record<string, unknown>) => Promise<BasicCrudRecord>
  remove: (id: string) => Promise<void>
  setEnabled?: (id: string, enabled: boolean) => Promise<BasicCrudRecord>
}
