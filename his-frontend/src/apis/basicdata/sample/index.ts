import { useHttp } from '@/plugins/http'
import {
  mockCreateSample,
  mockGetSampleDetail,
  mockListSample,
  mockLoadSampleFormOptions,
  mockRemoveSample,
  mockSetSampleEnabled,
  mockUpdateSample
} from './mock'
import type {
  SampleDetail,
  SampleFormOptions,
  SampleQuery,
  SampleRecord,
  SampleSavePayload
} from './type'

const currBaseUrl = '/sample'

/** 分页查询样本（后端未就绪时自动回退 Mock） */
export const listSample = async (
  query: SampleQuery
): Promise<{ rows: SampleRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: SampleRecord[]
      content?: SampleRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageSample', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListSample(query)
  }
}

/** 获取样本详情 */
export const getSampleDetail = async (id: string): Promise<SampleDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<SampleDetail>(currBaseUrl + '/getSample', { id })
    if (!res.data) throw new Error('获取样本详情失败')
    return res.data
  } catch {
    return mockGetSampleDetail(id)
  }
}

/** 新增样本 */
export const createSample = async (data: SampleSavePayload): Promise<SampleDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<SampleDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增样本失败')
    return res.data
  } catch {
    return mockCreateSample(data)
  }
}

/** 修改样本 */
export const updateSample = async (
  id: string,
  data: SampleSavePayload
): Promise<SampleDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<SampleDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改样本失败')
    return res.data
  } catch {
    return mockUpdateSample(id, data)
  }
}

/** 启用/停用样本 */
export const setSampleEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetSampleEnabled(id, enabled)
  }
}

/** 删除样本 */
export const removeSample = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveSample(id)
  }
}

/** 获取表单选项 */
export const loadSampleFormOptions = async (): Promise<SampleFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<SampleFormOptions>(currBaseUrl + '/getSampleInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadSampleFormOptions()
  }
}

export type * from './type'
