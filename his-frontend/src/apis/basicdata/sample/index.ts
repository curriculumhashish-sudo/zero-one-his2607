import { useProfiledHttp } from '../client'
import { getApiProfile, isMockEnabled, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
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

interface BackendSample {
  id?: unknown
  version?: unknown
  busNo?: unknown
  sampleCode?: unknown
  name?: unknown
  sampleName?: unknown
  pyStr?: unknown
  inputCode?: unknown
  wbStr?: unknown
  wbCode?: unknown
  typeEnum?: unknown
  typeName?: unknown
  statusEnum?: unknown
  status?: unknown
  remark?: unknown
  updatedAt?: unknown
}

const sampleCache = new Map<string, BackendSample>()

function rememberSample(item: BackendSample): void {
  const id = asString(item.id)
  if (!id) return
  sampleCache.set(id, { ...sampleCache.get(id), ...item, id })
}

function sampleVersion(id: string): number | undefined {
  const raw = sampleCache.get(id)?.version
  const version = asNumber(raw, Number.NaN)
  return Number.isFinite(version) ? version : undefined
}

function requireJ4Version(id: string): number {
  const version = sampleVersion(id)
  if (version === undefined) {
    throw new Error('J4 样本数据未返回 version，请刷新列表后再执行写操作')
  }
  return version
}

function mapSample(item: BackendSample): SampleRecord {
  rememberSample(item)
  return {
    id: asString(item.id),
    code: asString(item.busNo ?? item.sampleCode),
    name: asString(item.name ?? item.sampleName),
    pyCode: asString(item.pyStr ?? item.inputCode),
    type: asString(item.typeEnum),
    typeName: asString(item.typeName ?? item.typeEnum),
    collectionMethod: '',
    storageCondition: '',
    validHours: 24,
    enabled: parseStatus(item.statusEnum ?? item.status),
    sort: 0,
    updatedAt: asString(item.updatedAt)
  }
}

function mapDetail(item: BackendSample): SampleDetail {
  const record = mapSample(item)
  return {
    id: record.id,
    code: record.code,
    name: record.name,
    pyCode: record.pyCode,
    type: record.type,
    collectionMethod: record.collectionMethod,
    storageCondition: record.storageCondition,
    validHours: record.validHours,
    enabled: record.enabled,
    sort: record.sort,
    remark: asString(item.remark)
  }
}

function toC6DTO(data: SampleSavePayload, id?: string) {
  return {
    id,
    busNo: data.code,
    sampleCode: data.code,
    name: data.name,
    sampleName: data.name,
    pyStr: data.pyCode,
    inputCode: data.pyCode,
    wbStr: '',
    wbCode: '',
    typeEnum: data.type,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    status: data.enabled ? 1 : 0,
    remark: data.remark
  }
}

function toJ4DTO(data: SampleSavePayload, id?: string) {
  return {
    id,
    ...(id ? { version: requireJ4Version(id) } : {}),
    busNo: data.code,
    name: data.name,
    pyStr: data.pyCode,
    wbStr: '',
    typeEnum: data.type,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE'
  }
}

function rememberPayload(id: string, data: SampleSavePayload, extra?: BackendSample): void {
  rememberSample({
    id,
    busNo: data.code,
    name: data.name,
    pyStr: data.pyCode,
    typeEnum: data.type,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    ...extra
  })
}

export const listSample = async (
  query: SampleQuery
): Promise<{ rows: SampleRecord[]; total: number }> => {
  if (isMockEnabled()) return mockListSample(query)
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendSample>>('/sample/list-page', {
            page_index: query.pageIndex,
            page_size: query.pageSize,
            keyword: query.keyword ?? ''
          })
        : await http.get<BackendPage<BackendSample>>('/sample/query-sample-list', {
            pageIndex: query.pageIndex,
            pageSize: query.pageSize,
            keyword: query.keyword ?? '',
            typeEnum: query.type ?? '',
            statusEnum:
              query.enabled === true ? 'ACTIVE' : query.enabled === false ? 'INACTIVE' : ''
          })
    const page = normalizePage(res.data)
    const rows = page.rows.map(mapSample).filter((row) => {
      if (query.type && row.type !== query.type) return false
      return query.enabled == null ? true : row.enabled === query.enabled
    })
    return { rows, total: query.type || query.enabled != null ? rows.length : page.total }
  } catch (error) {
    return mockOrThrow(error, () => mockListSample(query))
  }
}

/** 两套接口都没有稳定的独立详情端点，使用最近一次列表返回的完整记录。 */
export const getSampleDetail = async (id: string): Promise<SampleDetail> => {
  const cached = sampleCache.get(id)
  if (cached) return mapDetail(cached)
  if (isMockEnabled()) return mockGetSampleDetail(id)
  throw new Error('未找到样本详情，请先刷新样本列表后再编辑')
}

export const createSample = async (data: SampleSavePayload): Promise<SampleDetail> => {
  if (isMockEnabled()) return mockCreateSample(data)
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res = await http.post<BackendSample | string>(
      '/sample/add-sample',
      profile === 'c6' ? toC6DTO(data) : toJ4DTO(data)
    )
    const id = typeof res.data === 'object' && res.data ? asString(res.data.id) : asString(res.data)
    rememberPayload(id, data, typeof res.data === 'object' && res.data ? res.data : undefined)
    return { ...data, id }
  } catch (error) {
    return mockOrThrow(error, () => mockCreateSample(data))
  }
}

export const updateSample = async (id: string, data: SampleSavePayload): Promise<SampleDetail> => {
  if (isMockEnabled()) return mockUpdateSample(id, data)
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.put<BackendSample | string>('/sample/edit-sample', toC6DTO(data, id))
        : await http.put<BackendSample | string>('/sample/update-sample', toJ4DTO(data, id))
    rememberPayload(id, data, typeof res.data === 'object' && res.data ? res.data : undefined)
    return { ...data, id }
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateSample(id, data))
  }
}

export const setSampleEnabled = async (id: string, enabled: boolean): Promise<void> => {
  if (isMockEnabled()) return mockSetSampleEnabled(id, enabled)
  try {
    const http = useProfiledHttp()
    const statusEnum = enabled ? 'ACTIVE' : 'INACTIVE'
    if (getApiProfile() === 'c6') {
      await http.put('/sample/switch-status', {
        ids: [id],
        statusEnum,
        status: enabled ? 1 : 0
      })
    } else {
      const res = await http.put<BackendSample>('/sample/update-sample-status', {
        id,
        version: requireJ4Version(id),
        statusEnum
      })
      if (res.data) rememberSample(res.data)
    }
    rememberSample({ id, statusEnum })
  } catch (error) {
    await mockOrThrow(error, () => mockSetSampleEnabled(id, enabled))
  }
}

export const removeSample = async (id: string): Promise<void> => {
  if (isMockEnabled()) return mockRemoveSample(id)
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.delete('/sample/remove-sample', { id })
    } else {
      await http.delete('/sample/delete-sample', { id, version: requireJ4Version(id) })
    }
    sampleCache.delete(id)
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveSample(id))
  }
}

/** 样本类型选项暂无两端统一接口，明确使用本地配置。 */
export const loadSampleFormOptions = async (): Promise<SampleFormOptions> =>
  mockLoadSampleFormOptions()

export type * from './type'
