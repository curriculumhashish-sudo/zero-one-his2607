import type {
  DiseaseDistributionResponse,
  InventoryAmountComparisonResponse,
  InventoryExpiryWarningDistributionResponse,
  MonthlyVisitCountResponse,
  RequestContext,
  StatisticsQuery,
  TrendResponse
} from './type'

function delay(context: RequestContext = {}) {
  return new Promise<void>((resolve, reject) => {
    const timer = setTimeout(resolve, 80)
    if (context.signal) {
      const abort = () => {
        clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      }
      if (context.signal.aborted) abort()
      else context.signal.addEventListener('abort', abort, { once: true })
    }
  })
}

function labels(query: StatisticsQuery) {
  const suffix = query.granularity === 'MONTH' ? ['1月', '2月', '3月', '4月', '5月', '6月'] : ['周一', '周二', '周三', '周四', '周五', '周六', '周日']
  return suffix
}

function points(query: StatisticsQuery, factor: number): TrendResponse {
  return { points: labels(query).map((label, index) => ({ label, value: factor + index * 13 })), unit: '人次' }
}

export async function mockGetTrafficTrend(query: StatisticsQuery, context: RequestContext = {}) {
  await delay(context)
  return points(query, query.organizationId ? 120 : 160)
}

export async function mockGetMonthlyVisitCount(_query: StatisticsQuery, context: RequestContext = {}): Promise<MonthlyVisitCountResponse> {
  await delay(context)
  return {
    points: Array.from({ length: 12 }, (_, index) => ({
      label: `${index + 1}月`,
      value: 8620 + index * 385 + ((index % 3) - 1) * 240
    })),
    unit: '人次'
  }
}

export async function mockGetWeeklyOutpatientTrend(query: StatisticsQuery, context: RequestContext = {}) {
  await delay(context)
  return points(query, 86)
}

export async function mockGetMonthlyOutpatientTrend(query: StatisticsQuery, context: RequestContext = {}) {
  await delay(context)
  return points({ ...query, granularity: 'MONTH' }, 420)
}

export async function mockGetPatientDiseaseDistribution(_query: StatisticsQuery, context: RequestContext = {}): Promise<DiseaseDistributionResponse> {
  await delay(context)
  return { items: [{ name: '呼吸系统疾病', value: 320 }, { name: '心血管疾病', value: 260 }, { name: '消化系统疾病', value: 180 }, { name: '其他', value: 140 }], unit: '人次' }
}

export async function mockGetInventoryAmountComparison(_query: StatisticsQuery, context: RequestContext = {}): Promise<InventoryAmountComparisonResponse> {
  await delay(context)
  return {
    categories: ['1月', '2月', '3月', '4月', '5月', '6月'],
    series: [
      { name: '药品入库', direction: 'INBOUND', values: [21, 28, 25, 35, 38, 33] },
      { name: '耗材入库', direction: 'INBOUND', values: [11, 17, 16, 23, 24, 22] },
      { name: '药品出库', direction: 'OUTBOUND', values: [18, 24, 31, 27, 33, 38] },
      { name: '耗材出库', direction: 'OUTBOUND', values: [10, 14, 18, 17, 19, 22] }
    ],
    unit: '万元'
  }
}

export async function mockGetInventoryExpiryWarningDistribution(_query: StatisticsQuery, context: RequestContext = {}): Promise<InventoryExpiryWarningDistributionResponse> {
  await delay(context)
  return { items: [{ name: '已过期', value: 3 }, { name: '30天内', value: 8 }, { name: '31-90天', value: 15 }, { name: '90天以上', value: 42 }], unit: '批' }
}
