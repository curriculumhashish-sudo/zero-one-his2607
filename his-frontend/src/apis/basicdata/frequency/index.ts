import { useHttp } from '@/plugins/http'
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

const currBaseUrl = '/frequency'

/** 分页查询频次（后端未就绪时自动回退 Mock） */
export const listFrequency = async (
  query: FrequencyQuery
): Promise<{ rows: FrequencyRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: FrequencyRecord[]
      content?: FrequencyRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageFrequency', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListFrequency(query)
  }
}

/** 获取频次详情 */
export const getFrequencyDetail = async (id: string): Promise<FrequencyDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<FrequencyDetail>(currBaseUrl + '/getFrequency', { id })
    if (!res.data) throw new Error('获取频次详情失败')
    return res.data
  } catch {
    return mockGetFrequencyDetail(id)
  }
}

/** 新增频次 */
export const createFrequency = async (data: FrequencySavePayload): Promise<FrequencyDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<FrequencyDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增频次失败')
    return res.data
  } catch {
    return mockCreateFrequency(data)
  }
}

/** 修改频次 */
export const updateFrequency = async (
  id: string,
  data: FrequencySavePayload
): Promise<FrequencyDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<FrequencyDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改频次失败')
    return res.data
  } catch {
    return mockUpdateFrequency(id, data)
  }
}

/** 启用/停用频次 */
export const setFrequencyEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetFrequencyEnabled(id, enabled)
  }
}

/** 删除频次 */
export const removeFrequency = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveFrequency(id)
  }
}

/** 获取表单选项 */
export const loadFrequencyFormOptions = async (): Promise<FrequencyFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<FrequencyFormOptions>(currBaseUrl + '/getFrequencyInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadFrequencyFormOptions()
  }
}

export type * from './type'
