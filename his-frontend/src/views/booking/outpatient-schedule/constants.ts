import type { ScheduleStatusMap } from '@/components/schedule/type'
import type { StatusDisplayOption } from '@/components/common/type'

export const scheduleStatusMap: ScheduleStatusMap = {
  draft: { text: '草稿', type: 'info' },
  published: { text: '已发布', type: 'success' },
  stopped: { text: '停诊', type: 'danger' }
}

export const scheduleStatusOptions: StatusDisplayOption[] = [
  { value: 'draft', label: '草稿', type: 'info' },
  { value: 'published', label: '已发布', type: 'success' },
  { value: 'stopped', label: '停诊', type: 'danger' }
]
