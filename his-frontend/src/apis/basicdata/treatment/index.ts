import { useProfiledHttp } from '../client'
import { getApiProfile, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
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

interface BackendTreatment {
  id?: unknown
  busNo?: unknown
  code?: unknown
  name?: unknown
  pyStr?: unknown
  categoryCode?: unknown
  categoryName?: unknown
  typeEnum?: unknown
  packageUnitCode?: unknown
  retailPrice?: unknown
  price?: unknown
  statusEnum?: unknown
  status?: unknown
  activeFlag?: unknown
  sortNo?: unknown
  descriptionText?: unknown
  comment?: unknown
  updatedAt?: unknown
}

function mapTreatment(item: BackendTreatment): TreatmentRecord {
  return {
    id: asString(item.id),
    code: asString(item.busNo ?? item.code),
    name: asString(item.name),
    pyCode: asString(item.pyStr),
    category: asString(item.categoryCode ?? item.typeEnum),
    categoryName: asString(item.categoryName ?? item.categoryCode ?? item.typeEnum),
    unit: asString(item.packageUnitCode),
    price: asNumber(item.retailPrice ?? item.price),
    enabled: parseStatus(item.activeFlag ?? item.statusEnum ?? item.status),
    sort: asNumber(item.sortNo),
    updatedAt: asString(item.updatedAt)
  }
}

function mapDetail(item: BackendTreatment): TreatmentDetail {
  const record = mapTreatment(item)
  return {
    id: record.id,
    code: record.code,
    name: record.name,
    pyCode: record.pyCode,
    category: record.category,
    unit: record.unit,
    price: record.price,
    enabled: record.enabled,
    sort: record.sort,
    remark: asString(item.descriptionText ?? item.comment)
  }
}

function toC6DTO(data: TreatmentSavePayload, id?: string) {
  return {
    id,
    name: data.name,
    categoryCode: data.category,
    typeEnum: data.category,
    packageUnitCode: data.unit,
    pyStr: data.pyCode,
    retailPrice: data.price,
    maximumRetailPrice: data.price,
    descriptionText: data.remark,
    status: data.enabled ? 1 : 0
  }
}

function toJ4DTO(data: TreatmentSavePayload, id?: string) {
  return {
    id,
    busNo: data.code,
    name: data.name,
    pyStr: data.pyCode,
    categoryCode: data.category,
    packageUnitCode: data.unit,
    retailPrice: data.price,
    maximumRetailPrice: data.price,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    activeFlag: data.enabled,
    sortNo: data.sort,
    descriptionText: data.remark,
    editabilityCode: id ? 'OK' : undefined
  }
}

function localDetail(id: string | undefined, data: TreatmentSavePayload): TreatmentDetail {
  return { ...data, id }
}

export const listTreatment = async (
  query: TreatmentQuery
): Promise<{ rows: TreatmentRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendTreatment>>('/treatment/query', {
            keywords: query.keyword ?? '',
            status: query.enabled == null ? undefined : query.enabled ? 1 : 0,
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendTreatment>>('/treatment/query-treatment-list', {
            keyword: query.keyword ?? '',
            categoryCode: query.category ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    const rows = page.rows
      .map(mapTreatment)
      .filter((row) => (query.enabled == null ? true : row.enabled === query.enabled))
    return { rows, total: query.enabled == null ? page.total : rows.length }
  } catch (error) {
    return mockOrThrow(error, () => mockListTreatment(query))
  }
}

export const getTreatmentDetail = async (id: string): Promise<TreatmentDetail> => {
  try {
    const http = useProfiledHttp()
    const res =
      getApiProfile() === 'c6'
        ? await http.get<BackendTreatment>('/treatment/query-by-id', { id })
        : await http.get<BackendTreatment>('/treatment/get-treatment', { id })
    if (!res.data) throw new Error('获取诊疗项目详情失败')
    return mapDetail(res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetTreatmentDetail(id))
  }
}

export const createTreatment = async (data: TreatmentSavePayload): Promise<TreatmentDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res = await http.post<BackendTreatment | string>(
      '/treatment/add-treatment',
      profile === 'c6' ? toC6DTO(data) : toJ4DTO(data)
    )
    return typeof res.data === 'object' && res.data
      ? mapDetail(res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateTreatment(data))
  }
}

export const updateTreatment = async (
  id: string,
  data: TreatmentSavePayload
): Promise<TreatmentDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendTreatment | string>(
            '/treatment/modify-treatment',
            toC6DTO(data, id)
          )
        : await http.put<BackendTreatment | string>(
            '/treatment/modify-treatment',
            toJ4DTO(data, id)
          )
    return typeof res.data === 'object' && res.data ? mapDetail(res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateTreatment(id, data))
  }
}

export const setTreatmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.post('/treatment/modify-status', { ids: [id], status: enabled ? 1 : 0 })
    } else {
      await http.put('/treatment/modify-treatment-status', {
        id,
        statusEnum: enabled ? 'ACTIVE' : 'INACTIVE'
      })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetTreatmentEnabled(id, enabled))
  }
}

export const removeTreatment = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.delete('/treatment/remove-treatment', [id])
    } else {
      await http.delete('/treatment/remove-treatment', undefined, { params: { id } })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveTreatment(id))
  }
}

/** 表单选项暂无两端统一接口，明确使用本地配置。 */
export const loadTreatmentFormOptions = async (): Promise<TreatmentFormOptions> =>
  mockLoadTreatmentFormOptions()

export const selectTreatment = async (
  keyword: string,
  pageIndex = 1,
  pageSize = 20
): Promise<{ rows: TreatmentRecord[]; total: number }> => {
  const query = { keyword, pageIndex, pageSize }
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendTreatment>>('/api/v2/treatments/options', {
            keyword,
            pageIndex,
            pageSize
          })
        : await http.get<BackendPage<BackendTreatment>>('/treatment/query-select-treatment-list', {
            keyword,
            pageIndex,
            pageSize
          })
    const page = normalizePage(res.data)
    return { rows: page.rows.map(mapTreatment), total: page.total }
  } catch (error) {
    return mockOrThrow(error, () => mockListTreatment(query))
  }
}

export type * from './type'
