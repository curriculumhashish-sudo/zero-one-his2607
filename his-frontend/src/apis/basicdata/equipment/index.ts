import { useHttp } from '@/plugins/http'
import {
  mockCreateEquipment,
  mockGetEquipmentDetail,
  mockListEquipment,
  mockLoadEquipmentFormOptions,
  mockRemoveEquipment,
  mockSetEquipmentEnabled,
  mockUpdateEquipment
} from './mock'
import type {
  EquipmentDetail,
  EquipmentFormOptions,
  EquipmentQuery,
  EquipmentRecord,
  EquipmentSavePayload
} from './type'

const currBaseUrl = '/equipment'

/** 分页查询器材（后端未就绪时自动回退 Mock） */
export const listEquipment = async (
  query: EquipmentQuery
): Promise<{ rows: EquipmentRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: EquipmentRecord[]
      content?: EquipmentRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageEquipment', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListEquipment(query)
  }
}

/** 获取器材详情 */
export const getEquipmentDetail = async (id: string): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<EquipmentDetail>(currBaseUrl + '/getEquipment', { id })
    if (!res.data) throw new Error('获取器材详情失败')
    return res.data
  } catch {
    return mockGetEquipmentDetail(id)
  }
}

/** 新增器材 */
export const createEquipment = async (data: EquipmentSavePayload): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<EquipmentDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增器材失败')
    return res.data
  } catch {
    return mockCreateEquipment(data)
  }
}

/** 修改器材 */
export const updateEquipment = async (
  id: string,
  data: EquipmentSavePayload
): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<EquipmentDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改器材失败')
    return res.data
  } catch {
    return mockUpdateEquipment(id, data)
  }
}

/** 启用/停用器材 */
export const setEquipmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetEquipmentEnabled(id, enabled)
  }
}

/** 删除器材 */
export const removeEquipment = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveEquipment(id)
  }
}

/** 获取表单选项 */
export const loadEquipmentFormOptions = async (): Promise<EquipmentFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<EquipmentFormOptions>(currBaseUrl + '/getEquipmentInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadEquipmentFormOptions()
  }
}

export type * from './type'
