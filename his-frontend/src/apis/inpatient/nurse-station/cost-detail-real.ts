import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import { realApiBaseURL } from './patient-real'
import type {
  NurseStationCostDetailLineVO,
  NurseStationCostDetailQueryDTO,
  NurseStationCostSummaryLineVO,
  NurseStationCostSummaryQueryDTO
} from './cost-detail-type'

interface BackendCostDetailPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: NurseStationCostDetailLineVO[]
}

function unwrapData<T>(response: { data?: T; message?: string }, emptyMessage: string): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || emptyMessage)
  }
  return response.data
}

function http() {
  return useHttp()
}

export async function realQueryNurseStationCostDetail(
  query: NurseStationCostDetailQueryDTO
): Promise<PageDTO<NurseStationCostDetailLineVO>> {
  const response = await http().get<BackendCostDetailPage>('/chargedetails/query-details', query, {
    baseURL: realApiBaseURL
  })
  const page = unwrapData(response, '后端未返回费用明细')
  const rows = page.rows ?? []
  const pageIndex = page.pageIndex ?? query.pageIndex
  const pageSize = page.pageSize ?? query.pageSize
  const total = page.total ?? rows.length
  return {
    pageIndex,
    pageSize,
    total,
    pages: page.pages ?? Math.ceil(total / Math.max(pageSize, 1)),
    rows
  }
}

export async function realQueryNurseStationCostSummary(
  query: NurseStationCostSummaryQueryDTO
): Promise<NurseStationCostSummaryLineVO[]> {
  const response = await http().get<NurseStationCostSummaryLineVO[]>(
    '/chargedetails/query-summary',
    query,
    { baseURL: realApiBaseURL }
  )
  return unwrapData(response, '后端未返回费用汇总')
}
