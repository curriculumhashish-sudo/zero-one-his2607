import type { StatisticsGranularity } from '@/apis/report/data-statistics'

export interface DataStatisticsQuery {
  dateRange: [string, string]
  granularity: StatisticsGranularity
  organizationId?: string
}

export interface ChartState {
  loading: boolean
  error: string
  empty: boolean
}
