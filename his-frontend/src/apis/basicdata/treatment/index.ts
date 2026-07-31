import { useHttp } from '@/plugins/http'
import {
  mockCreateTreatment,
  mockGetTreatmentDetail,
  mockListTreatment,
  mockLoadTreatmentFormOptions,
  mockRemoveTreatment,
  mockSetTreatmentEnabled,
  mockUpdateTreatment
} from './mock'
import type {
  TreatmentDetail,
  TreatmentFormOptions,
  TreatmentQuery,
  TreatmentRecord,
  TreatmentSavePayload
} from './type'

const currBaseUrl = '/treatment'

/** 分页查询诊疗项目（后端未就绪时自动回退 Mock） */
export const listTreatment = async (
  query: TreatmentQuery
): Promise<{ rows: TreatmentRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: TreatmentRecord[]
      content?: TreatmentRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageTreatment', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListTreatment(query)
  }
}

/** 获取诊疗项目详情 */
export const getTreatmentDetail = async (id: string): Promise<TreatmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<TreatmentDetail>(currBaseUrl + '/getTreatment', { id })
    if (!res.data) throw new Error('获取诊疗项目详情失败')
    return res.data
  } catch {
    return mockGetTreatmentDetail(id)
  }
}

/** 新增诊疗项目 */
export const createTreatment = async (data: TreatmentSavePayload): Promise<TreatmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<TreatmentDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增诊疗项目失败')
    return res.data
  } catch {
    return mockCreateTreatment(data)
  }
}

/** 修改诊疗项目 */
export const updateTreatment = async (
  id: string,
  data: TreatmentSavePayload
): Promise<TreatmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<TreatmentDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改诊疗项目失败')
    return res.data
  } catch {
    return mockUpdateTreatment(id, data)
  }
}

/** 启用/停用诊疗项目 */
export const setTreatmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetTreatmentEnabled(id, enabled)
  }
}

/** 删除诊疗项目 */
export const removeTreatment = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveTreatment(id)
  }
}

/** 获取表单选项 */
export const loadTreatmentFormOptions = async (): Promise<TreatmentFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<TreatmentFormOptions>(currBaseUrl + '/getTreatmentInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadTreatmentFormOptions()
  }
}

export type * from './type'
