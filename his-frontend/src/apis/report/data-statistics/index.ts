import HttpClient, { DataUpType } from '@/apis/http'
import {
  mockGetInventoryAmountComparison,
  mockGetInventoryExpiryWarningDistribution,
  mockGetMonthlyOutpatientTrend,
  mockGetMonthlyVisitCount,
  mockGetPatientDiseaseDistribution,
  mockGetTrafficTrend,
  mockGetWeeklyOutpatientTrend
} from './mock'
import type {
  DiseaseDistributionResponse,
  InventoryAmountComparisonResponse,
  InventoryExpiryWarningDistributionResponse,
  MonthlyVisitCountResponse,
  RequestContext,
  StatisticsQuery,
  StatisticsProvider,
  TrendResponse
} from './type'

type Provider = 'mock' | 'j8' | 'j6' | 'c1'
type LooseRecord = Record<string, any>
type BackendEnvelope<T> = { code?: number; message?: string; data?: T }

const configuredProvider = String(import.meta.env.VITE_REPORT_STATISTICS_PROVIDER ?? 'mock').toLowerCase() as Provider
let provider: Provider = configuredProvider

function useMockProvider() {
  return import.meta.env.MODE === 'test' || provider === 'mock'
}

export function setStatisticsProvider(nextProvider: StatisticsProvider) {
  provider = nextProvider
}

export function getStatisticsProvider(): StatisticsProvider {
  return provider === 'j6' || provider === 'c1' ? provider : 'j8'
}
// 有效期预警不使用时间统计粒度。后端未指定 goodsType 时返回全部物品类型，
// 因此页面当前不把 DAY/WEEK/MONTH 错误映射成 drug/consumable/device。
const expiryWarningPath = import.meta.env.VITE_J8_STATISTICS_EXPIRY_PATH || '/j8/statdata/getExpireWarningPie'
const expiryWarningGoodsType = String(import.meta.env.VITE_J8_STATISTICS_EXPIRY_GOODS_TYPE ?? '').trim()
const statisticsHttp = new HttpClient(
  {
    baseURL:
      provider === 'j6'
        ? import.meta.env.VITE_J6_STATISTICS_SERVER_URL || '/j6-report'
        : provider === 'c1'
          ? import.meta.env.VITE_C1_STATISTICS_SERVER_URL || '/c1-report'
          : import.meta.env.VITE_J8_STATISTICS_SERVER_URL || '/j8-statdata'
  },
  {
    requestInterceptor: (config) => {
      const token = typeof localStorage === 'undefined' ? '' : localStorage.getItem('token')
      const tokenHead = typeof localStorage === 'undefined' ? 'Bearer' : localStorage.getItem('tokenHead') || 'Bearer'
      if (token) config.headers.Authorization = `${tokenHead} ${token}`
      if (config.upType === DataUpType.json) config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

async function unwrap<T>(request: Promise<BackendEnvelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000) {
    throw new Error(response.message || '数据统计接口请求失败')
  }
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '数据统计接口未返回数据')
  }
  return response.data
}

function requestConfig(context?: RequestContext) {
  return context?.signal ? { signal: context.signal } : undefined
}

function commonParams(query: StatisticsQuery, granularity = query.granularity) {
  return {
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    granularity: granularity.toLowerCase(),
    ...(query.organizationId ? { organizationId: query.organizationId } : {})
  }
}

function monthParams(query: StatisticsQuery) {
  return {
    // J8 month-trend 实际按月份解析，使用 yyyy-MM。
    dateFrom: query.dateFrom.slice(0, 7),
    dateTo: query.dateTo.slice(0, 7),
    granularity: 'month',
    ...(query.organizationId ? { organizationId: query.organizationId } : {})
  }
}

function inventoryMonthParams(query: StatisticsQuery) {
  return {
    // J8 in-out 实际按 LocalDate 解析，必须使用 yyyy-MM-dd。
    dateFrom: query.dateFrom,
    dateTo: query.dateTo,
    granularity: 'month',
    ...(query.organizationId ? { organizationId: query.organizationId } : {})
  }
}

function toTrend(data: LooseRecord): TrendResponse {
  if (Array.isArray(data?.points)) return { points: data.points, unit: String(data.unit ?? '人次') }
  const rows = Array.isArray(data)
    ? data
    : Array.isArray(data?.points)
      ? data.points
      : Array.isArray(data?.trendList)
        ? data.trendList
        : Array.isArray(data?.data)
          ? data.data
          : Array.isArray(data?.rows)
            ? data.rows
            : []
  return {
    points: rows.map((item: LooseRecord) => ({
      label: String(item.label ?? item.dayName ?? item.dateRange ?? item.date ?? item.xLabel ?? item.month ?? item.hour ?? ''),
      value: Number(item.value ?? item.count ?? item.patientCount ?? item.visitCount ?? item.totalVisit ?? 0)
    })),
    unit: String(data?.unit ?? '人次')
  }
}

function toDisease(data: LooseRecord): DiseaseDistributionResponse {
  const rows = Array.isArray(data)
    ? data
    : Array.isArray(data?.items)
    ? data.items
    : Array.isArray(data?.distributionList)
      ? data.distributionList
      : Array.isArray(data?.data)
        ? data.data
        : []
  return {
    items: rows.map((item: LooseRecord) => ({ name: String(item.name ?? item.diseaseName ?? ''), value: Number(item.value ?? item.patientCount ?? item.count ?? 0) })),
    unit: String(data?.unit ?? '人')
  }
}

function toInventory(data: LooseRecord): InventoryAmountComparisonResponse {
  if (Array.isArray(data?.categories) && Array.isArray(data?.series)) return data as InventoryAmountComparisonResponse
  if (Array.isArray(data?.xaxis) && Array.isArray(data?.series)) {
    return {
      categories: data.xaxis.map(String),
      series: data.series.map((item: LooseRecord) => ({
        name: String(item.name ?? ''),
        direction: /out|出库/i.test(String(item.stack ?? item.name ?? '')) ? 'OUTBOUND' : 'INBOUND',
        values: Array.isArray(item.data) ? item.data.map(Number) : []
      })),
      unit: String(data.unit ?? '金额')
    }
  }
  const rows = Array.isArray(data) ? data : Array.isArray(data?.data) ? data.data : Array.isArray(data?.rows) ? data.rows : []
  const categories = [...new Set(rows.map((item: LooseRecord) => String(item.period ?? item.name ?? '')))]
  const hasDirection = rows.some((item: LooseRecord) => item.type !== undefined)
  const inbound = hasDirection ? rows.filter((item: LooseRecord) => /in|入/.test(String(item.type ?? ''))) : rows
  const outbound = hasDirection ? rows.filter((item: LooseRecord) => /out|出/.test(String(item.type ?? ''))) : rows
  return {
    categories,
    series: [
      { name: '入库', direction: 'INBOUND', values: categories.map((c) => Number(inbound.find((x: LooseRecord) => String(x.period ?? x.name) === c)?.totalAmount ?? inbound.find((x: LooseRecord) => String(x.period ?? x.name) === c)?.allmoney ?? 0)) },
      { name: '出库', direction: 'OUTBOUND', values: categories.map((c) => Number(outbound.find((x: LooseRecord) => String(x.period ?? x.name) === c)?.totalAmount ?? outbound.find((x: LooseRecord) => String(x.period ?? x.name) === c)?.outmoney ?? 0)) }
    ],
    unit: String(data?.unit ?? '金额')
  }
}

function toExpiry(data: LooseRecord): InventoryExpiryWarningDistributionResponse {
  const rows = Array.isArray(data) ? data : Array.isArray(data?.items) ? data.items : Array.isArray(data?.data) ? data.data : Array.isArray(data?.rows) ? data.rows : []
  return { items: rows.map((item: LooseRecord) => ({ name: String(item.name ?? item.bus_no ?? item.batch_number ?? ''), value: Number(item.value ?? item.quantity ?? item.numbers ?? 0) })), unit: String(data?.unit ?? '件') }
}

async function getReal<T>(path: string, params: LooseRecord, context?: RequestContext) {
  return unwrap<T>(statisticsHttp.get<T>(path, params, requestConfig(context)))
}

export const getTrafficTrend = async (query: StatisticsQuery, context?: RequestContext): Promise<TrendResponse> => {
  if (useMockProvider()) return mockGetTrafficTrend(query, context)
  if (provider === 'j8') return toTrend(await getReal('/j8/statdata/hour-trend', commonParams(query, 'DAY'), context) as LooseRecord)
  if (provider === 'j6') return toTrend(await getReal('/data-statistics/traffic-trend', commonParams(query), context) as LooseRecord)
  return toTrend(await getReal('/data-statistics/flow-trend', {
    orgId: query.organizationId,
    timeType: query.granularity === 'WEEK' ? 1 : 2,
    startDate: query.dateFrom,
    endDate: query.dateTo,
    pageIndex: 1,
    pageSize: 500
  }, context) as LooseRecord)
}

export const getMonthlyVisitCount = async (query: StatisticsQuery, context?: RequestContext): Promise<MonthlyVisitCountResponse> => {
  if (useMockProvider()) return mockGetMonthlyVisitCount(query, context)
  if (provider === 'j8') return toTrend(await getReal('/j8/statdata/month-trend', monthParams(query), context) as LooseRecord)
  if (provider === 'j6') return toTrend(await getReal('/data-statistics/monthly-access', commonParams(query, 'MONTH'), context) as LooseRecord)
  return toTrend(await getReal('/data-statistics/month-visit', {
    startDate: query.dateFrom,
    endDate: query.dateTo,
    orgId: query.organizationId,
    pageIndex: 1,
    pageSize: 500
  }, context) as LooseRecord)
}

export const getWeeklyOutpatientTrend = async (query: StatisticsQuery, context?: RequestContext): Promise<TrendResponse> => {
  if (useMockProvider()) return mockGetWeeklyOutpatientTrend(query, context)
  if (provider === 'j8') return toTrend(await getReal('/outpatient/visit-trend/weekly', commonParams(query, 'WEEK'), context) as LooseRecord)
  if (provider === 'j6') return toTrend(await getReal('/data-statistics/week-trend', commonParams(query, 'WEEK'), context) as LooseRecord)
  return toTrend(await getReal('/form/data_statistics/query_trend', { timeType: 1, startDate: query.dateFrom, endDate: query.dateTo, orgId: query.organizationId }, context) as LooseRecord)
}

export const getMonthlyOutpatientTrend = async (query: StatisticsQuery, context?: RequestContext): Promise<TrendResponse> => {
  if (useMockProvider()) return mockGetMonthlyOutpatientTrend(query, context)
  if (provider === 'j8') return toTrend(await getReal('/outpatient/visit-trend/monthly', commonParams(query, 'MONTH'), context) as LooseRecord)
  if (provider === 'j6') return toTrend(await getReal('/data-statistics/month-trend', commonParams(query, 'MONTH'), context) as LooseRecord)
  return toTrend(await getReal('/form/data_statistics/query_trend', { timeType: 2, startDate: query.dateFrom, endDate: query.dateTo, orgId: query.organizationId }, context) as LooseRecord)
}

export const getPatientDiseaseDistribution = async (query: StatisticsQuery, context?: RequestContext): Promise<DiseaseDistributionResponse> => {
  if (useMockProvider()) return mockGetPatientDiseaseDistribution(query, context)
  if (provider === 'j8') return toDisease(await getReal('/outpatient/disease-distribution', commonParams(query), context) as LooseRecord)
  if (provider === 'j6') return toDisease(await getReal('/data-statistics/disease-statistic', commonParams(query), context) as LooseRecord)
  return toDisease(await getReal('/form/data_statistics/query_disease_distribution', { startDate: query.dateFrom, endDate: query.dateTo, orgId: query.organizationId }, context) as LooseRecord)
}

export const getInventoryAmountComparison = async (query: StatisticsQuery, context?: RequestContext): Promise<InventoryAmountComparisonResponse> => {
  if (useMockProvider()) return mockGetInventoryAmountComparison(query, context)
  if (provider === 'j8') return toInventory(await getReal('/j8/statdata/in-out', inventoryMonthParams(query), context) as LooseRecord)
  if (provider === 'j6') return toInventory(await getReal('/data-statistics/supply-amount-compare', commonParams(query, 'MONTH'), context) as LooseRecord)
  const data = await getReal<LooseRecord>('/form/data_statistics/InOutbound', { pageIndex: 1, pageSize: 500 }, context)
  return toInventory(data)
}

export const getInventoryExpiryWarningDistribution = async (query: StatisticsQuery, context?: RequestContext): Promise<InventoryExpiryWarningDistributionResponse> => {
  if (useMockProvider()) return mockGetInventoryExpiryWarningDistribution(query, context)
  if (provider === 'j8') {
    const path = expiryWarningGoodsType
      ? `${expiryWarningPath}/${encodeURIComponent(expiryWarningGoodsType)}`
      : expiryWarningPath
    return toExpiry(await getReal(path, commonParams(query), context) as LooseRecord)
  }
  if (provider === 'j6') return toExpiry(await getReal('/data-statistics/expiration-warning', { ...commonParams(query), expireWithinDays: 30 }, context) as LooseRecord)
  const data = await getReal<LooseRecord>('/form/data_statistics/InventoryMaterial', { pageIndex: 1, pageSize: 500 }, context)
  return toExpiry(data)
}

export type * from './type'
