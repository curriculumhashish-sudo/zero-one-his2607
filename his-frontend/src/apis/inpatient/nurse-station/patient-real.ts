import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO,
  NurseStationPatientQueueQuery,
  NurseStationPatientStatisticsVO
} from './type'
import {
  isNurseStationRealApiEnabled,
  realApiBaseURL
} from './runtime-config'

export { isNurseStationRealApiEnabled, realApiBaseURL } from './runtime-config'

interface BackendPatientQueuePage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: NurseStationPatientQueueItemVO[]
}

/**
 * 统一处理后端 JsonVO 包装。
 *
 * useHttp 已经负责处理 HTTP 错误和登录失效；这里再检查一次 data，
 * 是为了防止后端返回 200 但业务数据为空时，页面继续渲染出半成品。
 */
function unwrapData<T>(response: { data?: T; message?: string }): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '后端未返回有效数据')
  }
  return response.data
}

/** 用同一个配置访问护士站后端，确保 Authorization 由全局 HTTP 插件统一注入。 */
function getRealHttp() {
  return useHttp()
}

/**
 * 查询真实患者统计。
 * 后端接口与前端筛选条件同名，因此只需要去掉 undefined 字段即可。
 */
export async function realGetNurseStationPatientStatistics(
  query: NurseStationPatientFilterQuery
): Promise<NurseStationPatientStatisticsVO> {
  const response = await getRealHttp().get<NurseStationPatientStatisticsVO>(
    '/encountermanage/query-patient-statistics',
    query,
    { baseURL: realApiBaseURL }
  )
  return unwrapData(response)
}

/**
 * 查询真实患者队列。
 * Swagger 返回的 data 是标准分页对象，前端保留 PageDTO 结构，页面无需改动。
 */
export async function realPageNurseStationPatientQueue(
  query: NurseStationPatientQueueQuery
): Promise<PageDTO<NurseStationPatientQueueItemVO>> {
  const response = await getRealHttp().get<BackendPatientQueuePage>(
    '/encountermanage/query-patient-queue',
    query,
    { baseURL: realApiBaseURL }
  )
  const page = unwrapData(response)
  const pageIndex = page.pageIndex ?? query.pageIndex
  const pageSize = page.pageSize ?? query.pageSize
  const total = page.total ?? page.rows?.length ?? 0
  const rows = page.rows ?? []
  return {
    pageIndex,
    pageSize,
    total,
    pages: page.pages ?? Math.ceil(total / Math.max(pageSize, 1)),
    rows
  }
}

/**
 * 读取全部患者，用于在后端尚未提供病区树接口时动态生成病区/病房筛选项。
 * 这是前端降级方案：真实患者队列仍来自后端，不依赖固定 Mock ID。
 */
export async function realLoadAllNurseStationPatientRows(
  query: NurseStationPatientFilterQuery = {}
): Promise<NurseStationPatientQueueItemVO[]> {
  const rows: NurseStationPatientQueueItemVO[] = []
  let pageIndex = 1
  let total = 0
  const pageSize = 100

  do {
    const page = await realPageNurseStationPatientQueue({ ...query, pageIndex, pageSize })
    const pageRows = page.rows ?? []
    rows.push(...pageRows)
    total = page.total
    pageIndex += 1
    if (!pageRows.length) break
  } while (rows.length < total)

  return rows
}
