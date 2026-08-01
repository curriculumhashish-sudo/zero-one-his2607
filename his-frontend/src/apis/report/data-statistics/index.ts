import {
  mockGetInventoryAmountComparison,
  mockGetInventoryExpiryWarningDistribution,
  mockGetMonthlyOutpatientTrend,
  mockGetMonthlyVisitCount,
  mockGetPatientDiseaseDistribution,
  mockGetTrafficTrend,
  mockGetWeeklyOutpatientTrend
} from './mock'
import type { RequestContext, StatisticsQuery } from './type'

export const getTrafficTrend = (query: StatisticsQuery, context?: RequestContext) => mockGetTrafficTrend(query, context)
export const getMonthlyVisitCount = (query: StatisticsQuery, context?: RequestContext) => mockGetMonthlyVisitCount(query, context)
export const getWeeklyOutpatientTrend = (query: StatisticsQuery, context?: RequestContext) => mockGetWeeklyOutpatientTrend(query, context)
export const getMonthlyOutpatientTrend = (query: StatisticsQuery, context?: RequestContext) => mockGetMonthlyOutpatientTrend(query, context)
export const getPatientDiseaseDistribution = (query: StatisticsQuery, context?: RequestContext) => mockGetPatientDiseaseDistribution(query, context)
export const getInventoryAmountComparison = (query: StatisticsQuery, context?: RequestContext) => mockGetInventoryAmountComparison(query, context)
export const getInventoryExpiryWarningDistribution = (query: StatisticsQuery, context?: RequestContext) => mockGetInventoryExpiryWarningDistribution(query, context)

export type * from './type'
