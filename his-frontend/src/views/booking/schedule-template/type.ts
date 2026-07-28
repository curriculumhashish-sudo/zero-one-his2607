import type { TemplateStatus } from '@/apis/booking/schedule-template'

export interface ScheduleTemplatePageQuery {
  keyword: string
  status: TemplateStatus | ''
  page: number
  pageSize: number
}
