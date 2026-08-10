import { useProfiledHttp } from '../client'
import { getApiProfile, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
import {
  mockCreateFrequency,
  mockGetFrequencyDetail,
  mockListFrequency,
  mockLoadFrequencyFormOptions,
  mockRemoveFrequency,
  mockSetFrequencyEnabled,
  mockUpdateFrequency
} from './mock'
import type {
  FrequencyDetail,
  FrequencyFormOptions,
  FrequencyQuery,
  FrequencyRecord,
  FrequencySavePayload
} from './type'

interface BackendFrequency {
  id?: unknown
  code?: unknown
  rateCode?: unknown
  name?: unknown
  cycleType?: unknown
  dayCount?: unknown
  totalExecutionCount?: unknown
  executionPeriod?: unknown
  executionPeriodUnit?: unknown
  dayTimes?: unknown
  activeFlag?: unknown
  status?: unknown
  sortNo?: unknown
  description?: unknown
  remark?: unknown
  updatedAt?: unknown
}

function mapFrequency(item: BackendFrequency): FrequencyRecord {
  return {
    id: asString(item.id),
    code: asString(item.rateCode ?? item.code),
    name: asString(item.name),
    type: asString(item.cycleType),
    typeName: asString(item.cycleType),
    timesPerDay: asNumber(item.totalExecutionCount ?? item.dayCount),
    intervalHours: asNumber(item.executionPeriod),
    timePoints: asString(item.dayTimes),
    enabled: parseStatus(item.activeFlag ?? item.status),
    sort: asNumber(item.sortNo),
    updatedAt: asString(item.updatedAt)
  }
}

function mapDetail(item: BackendFrequency): FrequencyDetail {
  const record = mapFrequency(item)
  return {
    id: record.id,
    code: record.code,
    name: record.name,
    type: record.type,
    timesPerDay: record.timesPerDay,
    intervalHours: record.intervalHours,
    timePoints: record.timePoints,
    enabled: record.enabled,
    sort: record.sort,
    remark: asString(item.description ?? item.remark)
  }
}

function toC6DTO(data: FrequencySavePayload, id?: string) {
  return {
    id,
    code: data.code,
    name: data.name,
    status: data.enabled ? 1 : 0,
    cycleType: data.type,
    dayTimes: data.timePoints,
    dayCount: data.timesPerDay,
    executionPeriod: data.intervalHours,
    executionPeriodUnit: 'h',
    totalExecutionCount: data.timesPerDay,
    /** Swagger FrequencyDTO 必填字段，默认不允许跨日 */
    allowCrossDay: 'N',
    sortNo: data.sort
  }
}

function toJ4DTO(data: FrequencySavePayload, id?: string) {
  return {
    id,
    rateCode: data.code,
    name: data.name,
    cycleType: data.type,
    dayTimes: data.timePoints,
    dayCount: data.timesPerDay,
    totalExecutionCount: data.timesPerDay,
    executionPeriod: data.intervalHours,
    executionPeriodUnit: 'h',
    /** J4 FrequencySaveDTO 必填字段 */
    applicableScenes: 'all',
    allowCrossDay: 'same-day',
    activeFlag: data.enabled ? '1' : '0',
    sortNo: data.sort
  }
}

function localDetail(id: string | undefined, data: FrequencySavePayload): FrequencyDetail {
  return { ...data, id }
}

export const listFrequency = async (
  query: FrequencyQuery
): Promise<{ rows: FrequencyRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendFrequency>>('/frequency/query-all', {
            keywords: query.keyword ?? '',
            status: query.enabled == null ? undefined : query.enabled ? 1 : 0,
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendFrequency>>('/frequency/query-frequency-list', {
            keyword: query.keyword ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    return { rows: page.rows.map(mapFrequency), total: page.total }
  } catch (error) {
    return mockOrThrow(error, () => mockListFrequency(query))
  }
}

export const getFrequencyDetail = async (id: string): Promise<FrequencyDetail> => {
  try {
    const http = useProfiledHttp()
    const path =
      getApiProfile() === 'c6' ? '/frequency/query-one' : '/frequency/query-frequency-detail'
    const res = await http.get<BackendFrequency>(path, { id })
    if (!res.data) throw new Error('获取频次详情失败')
    return mapDetail(res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetFrequencyDetail(id))
  }
}

export const createFrequency = async (data: FrequencySavePayload): Promise<FrequencyDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendFrequency | string>('/frequency/add', toC6DTO(data))
        : await http.post<BackendFrequency | string>('/frequency/add-frequency', toJ4DTO(data))
    return typeof res.data === 'object' && res.data
      ? mapDetail(res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateFrequency(data))
  }
}

export const updateFrequency = async (
  id: string,
  data: FrequencySavePayload
): Promise<FrequencyDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.put<BackendFrequency | string>('/frequency/modify', toC6DTO(data, id))
        : await http.put<BackendFrequency | string>(
            '/frequency/modify-frequency',
            toJ4DTO(data, id)
          )
    return typeof res.data === 'object' && res.data ? mapDetail(res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateFrequency(id, data))
  }
}

export const setFrequencyEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.put('/frequency/modify-status', undefined, {
        params: { id, status: enabled ? 1 : 0 }
      })
    } else {
      await http.put('/frequency/modify-frequency-status', {
        id,
        activeFlag: enabled ? '1' : '0'
      })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetFrequencyEnabled(id, enabled))
  }
}

export const removeFrequency = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.delete('/frequency/remove', undefined, { params: { id } })
    } else {
      await http.delete(`/frequency/remove-frequency/${encodeURIComponent(id)}`)
    }
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveFrequency(id))
  }
}

/** 表单类型选项暂无统一后端接口，明确使用本地配置。 */
export const loadFrequencyFormOptions = async (): Promise<FrequencyFormOptions> =>
  mockLoadFrequencyFormOptions()

export type * from './type'
