import { useHttp } from '@/plugins/http'
import {
  mockCreateDiagnosis,
  mockGetDiagnosisDetail,
  mockListDiagnosis,
  mockLoadDiagnosisFormOptions,
  mockRemoveDiagnosis,
  mockSetDiagnosisEnabled,
  mockUpdateDiagnosis
} from './mock'
import type {
  DiagnosisDetail,
  DiagnosisFormOptions,
  DiagnosisQuery,
  DiagnosisRecord,
  DiagnosisSavePayload
} from './type'

const currBaseUrl = '/diagnosis'

/** 分页查询诊断（后端未就绪时自动回退 Mock） */
export const listDiagnosis = async (
  query: DiagnosisQuery
): Promise<{ rows: DiagnosisRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: DiagnosisRecord[]
      content?: DiagnosisRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageDiagnosis', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListDiagnosis(query)
  }
}

/** 获取诊断详情 */
export const getDiagnosisDetail = async (id: string): Promise<DiagnosisDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<DiagnosisDetail>(currBaseUrl + '/getDiagnosis', { id })
    if (!res.data) throw new Error('获取诊断详情失败')
    return res.data
  } catch {
    return mockGetDiagnosisDetail(id)
  }
}

/** 新增诊断 */
export const createDiagnosis = async (data: DiagnosisSavePayload): Promise<DiagnosisDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<DiagnosisDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增诊断失败')
    return res.data
  } catch {
    return mockCreateDiagnosis(data)
  }
}

/** 修改诊断 */
export const updateDiagnosis = async (
  id: string,
  data: DiagnosisSavePayload
): Promise<DiagnosisDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<DiagnosisDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改诊断失败')
    return res.data
  } catch {
    return mockUpdateDiagnosis(id, data)
  }
}

/** 启用/停用诊断 */
export const setDiagnosisEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetDiagnosisEnabled(id, enabled)
  }
}

/** 删除诊断 */
export const removeDiagnosis = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveDiagnosis(id)
  }
}

/** 获取表单选项 */
export const loadDiagnosisFormOptions = async (): Promise<DiagnosisFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<DiagnosisFormOptions>(currBaseUrl + '/getDiagnosisInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadDiagnosisFormOptions()
  }
}

export type * from './type'
