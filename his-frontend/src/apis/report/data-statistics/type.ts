export type StatisticsGranularity = 'DAY' | 'WEEK' | 'MONTH'
export type StatisticsProvider = 'j8' | 'j6' | 'c1'

export interface StatisticsQuery {
  dateFrom: string
  dateTo: string
  granularity: StatisticsGranularity
  organizationId?: string
}

export interface StatisticsPoint {
  label: string
  value: number
}

export interface TrendResponse {
  points: StatisticsPoint[]
  unit: string
}

export type MonthlyVisitCountResponse = TrendResponse

export interface DiseaseDistributionItem {
  name: string
  value: number
}

export interface DiseaseDistributionResponse {
  items: DiseaseDistributionItem[]
  unit: string
}

export interface InventoryAmountComparisonResponse {
  categories: string[]
  series: Array<{
    name: string
    direction: 'INBOUND' | 'OUTBOUND'
    values: number[]
  }>
  unit: string
}

export interface InventoryExpiryWarningDistributionResponse {
  items: DiseaseDistributionItem[]
  unit: string
}

export interface RequestContext {
  signal?: AbortSignal
}
