import { useHttp } from '@/plugins/http'
import {
  mockCreateMedication,
  mockGetMedicationDetail,
  mockListMedication,
  mockLoadMedicationFormOptions,
  mockRemoveMedication,
  mockSetMedicationEnabled,
  mockUpdateMedication
} from './mock'
import type {
  MedicationDetail,
  MedicationFormOptions,
  MedicationQuery,
  MedicationRecord,
  MedicationSavePayload
} from './type'

const currBaseUrl = '/medication'

/** 分页查询药品（后端未就绪时自动回退 Mock） */
export const listMedication = async (
  query: MedicationQuery
): Promise<{ rows: MedicationRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: MedicationRecord[]
      content?: MedicationRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageMedication', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListMedication(query)
  }
}

/** 获取药品详情 */
export const getMedicationDetail = async (id: string): Promise<MedicationDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationDetail>(currBaseUrl + '/getMedication', { id })
    if (!res.data) throw new Error('获取药品详情失败')
    return res.data
  } catch {
    return mockGetMedicationDetail(id)
  }
}

/** 新增药品 */
export const createMedication = async (data: MedicationSavePayload): Promise<MedicationDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增药品失败')
    return res.data
  } catch {
    return mockCreateMedication(data)
  }
}

/** 修改药品 */
export const updateMedication = async (
  id: string,
  data: MedicationSavePayload
): Promise<MedicationDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<MedicationDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改药品失败')
    return res.data
  } catch {
    return mockUpdateMedication(id, data)
  }
}

/** 启用/停用药品 */
export const setMedicationEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetMedicationEnabled(id, enabled)
  }
}

/** 删除药品 */
export const removeMedication = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveMedication(id)
  }
}

/** 获取表单选项 */
export const loadMedicationFormOptions = async (): Promise<MedicationFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationFormOptions>(currBaseUrl + '/getMedicationInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadMedicationFormOptions()
  }
}

export type * from './type'
