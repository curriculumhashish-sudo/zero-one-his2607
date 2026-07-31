import { useHttp } from '@/plugins/http'
import {
  mockCreateMedicationCatalog,
  mockGetMedicationCatalogDetail,
  mockListMedicationCatalog,
  mockLoadMedicationCatalogFormOptions,
  mockRemoveMedicationCatalog,
  mockSetMedicationCatalogEnabled,
  mockUpdateMedicationCatalog
} from './mock'
import type {
  MedicationCatalogDetail,
  MedicationCatalogFormOptions,
  MedicationCatalogQuery,
  MedicationCatalogRecord,
  MedicationCatalogSavePayload
} from './type'

const currBaseUrl = '/medicationCatalog'

/** 分页查询药品目录（后端未就绪时自动回退 Mock） */
export const listMedicationCatalog = async (
  query: MedicationCatalogQuery
): Promise<{ rows: MedicationCatalogRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.post<{
      rows?: MedicationCatalogRecord[]
      content?: MedicationCatalogRecord[]
      total?: number
      totalElements?: number
    }>(currBaseUrl + '/pageMedicationCatalog', query)
    const data = res.data
    return {
      rows: data?.rows ?? data?.content ?? [],
      total: data?.total ?? data?.totalElements ?? 0
    }
  } catch {
    return mockListMedicationCatalog(query)
  }
}

/** 获取药品目录详情 */
export const getMedicationCatalogDetail = async (id: string): Promise<MedicationCatalogDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationCatalogDetail>(currBaseUrl + '/getMedicationCatalog', { id })
    if (!res.data) throw new Error('获取药品目录详情失败')
    return res.data
  } catch {
    return mockGetMedicationCatalogDetail(id)
  }
}

/** 新增药品目录 */
export const createMedicationCatalog = async (
  data: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationCatalogDetail>(currBaseUrl + '/add', data)
    if (!res.data) throw new Error('新增药品目录失败')
    return res.data
  } catch {
    return mockCreateMedicationCatalog(data)
  }
}

/** 修改药品目录 */
export const updateMedicationCatalog = async (
  id: string,
  data: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<MedicationCatalogDetail>(`${currBaseUrl}/update/${id}`, data)
    if (!res.data) throw new Error('修改药品目录失败')
    return res.data
  } catch {
    return mockUpdateMedicationCatalog(id, data)
  }
}

/** 启用/停用药品目录 */
export const setMedicationCatalogEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.post(currBaseUrl + '/setEnabled', { id, enabled })
  } catch {
    await mockSetMedicationCatalogEnabled(id, enabled)
  }
}

/** 删除药品目录 */
export const removeMedicationCatalog = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + `/delete/${id}`)
  } catch {
    await mockRemoveMedicationCatalog(id)
  }
}

/** 获取表单选项 */
export const loadMedicationCatalogFormOptions = async (): Promise<MedicationCatalogFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<MedicationCatalogFormOptions>(currBaseUrl + '/getMedicationInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadMedicationCatalogFormOptions()
  }
}

export type * from './type'
