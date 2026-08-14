import { useProfiledHttp } from '../client'
import { getApiProfile, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
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
  EquipmentCategory,
  EquipmentDetail,
  EquipmentFormOptions,
  EquipmentQuery,
  EquipmentRecord,
  EquipmentSavePayload
} from './type'

interface BackendEquipment {
  id?: unknown
  busNo?: unknown
  code?: unknown
  medcode?: unknown
  name?: unknown
  pyStr?: unknown
  pinyin?: unknown
  categoryCode?: unknown
  categoryName?: unknown
  classification?: unknown
  specification?: unknown
  modelNumber?: unknown
  model?: unknown
  unitCode?: unknown
  unitCode_dictText?: unknown
  packageUnitCode?: unknown
  price?: unknown
  retailPrice?: unknown
  retail?: unknown
  status?: unknown
  statusEnum?: unknown
  activeFlag?: unknown
  sortNo?: unknown
  description?: unknown
  remark?: unknown
  updatedAt?: unknown
}

function mapEquipment(item: BackendEquipment): EquipmentRecord {
  return {
    id: asString(item.id),
    code: asString(item.busNo ?? item.code ?? item.medcode),
    name: asString(item.name),
    pyCode: asString(item.pyStr ?? item.pinyin),
    category: asString(item.categoryCode ?? item.classification),
    categoryName: asString(item.categoryName ?? item.classification),
    specification: asString(item.specification),
    model: asString(item.modelNumber ?? item.model),
    unit: asString(item.unitCode ?? item.packageUnitCode),
    price: asNumber(item.retailPrice ?? item.retail ?? item.price),
    enabled: parseStatus(item.activeFlag ?? item.statusEnum ?? item.status),
    sort: asNumber(item.sortNo),
    remark: asString(item.description ?? item.remark),
    updatedAt: asString(item.updatedAt)
  }
}

function mapDetail(item: BackendEquipment): EquipmentDetail {
  const record = mapEquipment(item)
  return {
    id: record.id,
    code: record.code,
    name: record.name,
    pyCode: record.pyCode,
    category: record.category as EquipmentCategory,
    specification: record.specification,
    model: record.model,
    unit: record.unit,
    price: record.price,
    enabled: record.enabled,
    sort: record.sort,
    remark: record.remark
  }
}

function toC6DTO(data: EquipmentSavePayload, id?: string) {
  return {
    id,
    name: data.name,
    classification: data.category,
    code: data.code,
    medcode: data.code,
    specification: data.specification,
    status: data.enabled ? 1 : 0,
    price: String(data.price),
    retail: data.price,
    pinyin: data.pyCode,
    packaging: data.unit
  }
}

function toJ4DTO(data: EquipmentSavePayload, id?: string) {
  return {
    id,
    busNo: data.code,
    name: data.name,
    pyStr: data.pyCode,
    categoryCode: data.category,
    specification: data.specification,
    modelNumber: data.model,
    unitCode: data.unit,
    packageUnitCode: data.unit,
    retailPrice: data.price,
    activeFlag: data.enabled,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    sortNo: data.sort,
    description: data.remark
  }
}

function localDetail(id: string | undefined, data: EquipmentSavePayload): EquipmentDetail {
  return { ...data, id }
}

export const listEquipment = async (
  query: EquipmentQuery
): Promise<{ rows: EquipmentRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendEquipment>>('/equipment/query', {
            keywords: query.keyword ?? '',
            classification: query.category ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendEquipment>>('/equipment/query-equipment-list', {
            keyword: query.keyword ?? '',
            categoryCode: query.category ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    const rows = page.rows
      .map(mapEquipment)
      .filter((row) => (query.enabled == null ? true : row.enabled === query.enabled))
    return { rows, total: query.enabled == null ? page.total : rows.length }
  } catch (error) {
    return mockOrThrow(error, () => mockListEquipment(query))
  }
}

export const getEquipmentDetail = async (id: string): Promise<EquipmentDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendEquipment>('/equipment/query-by-id', { id })
        : await http.get<BackendEquipment>('/equipment/get-by-id', { equipmentId: id })
    if (!res.data) throw new Error('获取器材详情失败')
    return mapDetail(res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetEquipmentDetail(id))
  }
}

export const createEquipment = async (data: EquipmentSavePayload): Promise<EquipmentDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendEquipment | string>('/equipment/add', toC6DTO(data))
        : await http.post<BackendEquipment | string>('/equipment/add-equipment', toJ4DTO(data))
    return typeof res.data === 'object' && res.data
      ? mapDetail(res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateEquipment(data))
  }
}

export const updateEquipment = async (
  id: string,
  data: EquipmentSavePayload
): Promise<EquipmentDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.put<BackendEquipment | string>('/equipment/modify', toC6DTO(data, id))
        : await http.put<BackendEquipment | string>(
            '/equipment/update-equipment',
            toJ4DTO(data, id)
          )
    return typeof res.data === 'object' && res.data ? mapDetail(res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateEquipment(id, data))
  }
}

export const setEquipmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.put('/equipment/status', { ids: [id], status: enabled ? 1 : 0 })
    } else {
      await http.put('/equipment/modify-equipment-status', { ids: [id], activeFlag: enabled })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetEquipmentEnabled(id, enabled))
  }
}

export const removeEquipment = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.delete('/equipment/delete', { ids: [id] })
    } else {
      await http.delete('/equipment/remove-by-ids', [id])
    }
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveEquipment(id))
  }
}

/** 分类与单位选项尚未形成两端一致的接口，明确使用本地配置。 */
export const loadEquipmentFormOptions = async (): Promise<EquipmentFormOptions> =>
  mockLoadEquipmentFormOptions()

export type * from './type'
