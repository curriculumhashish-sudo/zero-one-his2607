import { useProfiledHttp } from '../client'
import { useHttp } from '@/plugins/http'
import { getApiProfile, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
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
  DiagnosisCategory,
  DiagnosisDetail,
  DiagnosisFormOptions,
  DiagnosisQuery,
  DiagnosisRecord,
  DiagnosisSavePayload,
  DiagnosisType
} from './type'

interface BackendDiagnosis {
  id?: unknown
  version?: unknown
  conditionCode?: unknown
  diseaseCode?: unknown
  diagnoseCode?: unknown
  name?: unknown
  diagnoseName?: unknown
  sourceEnum?: unknown
  source_enum?: unknown
  diagnoseType?: unknown
  diagType?: unknown
  typeCode?: unknown
  catName?: unknown
  pyStr?: unknown
  pinyinCode?: unknown
  description?: unknown
  remark?: unknown
  statusEnum?: unknown
  status?: unknown
  updatedAt?: unknown
}

const c6Versions = new Map<string, number>()

function rememberVersion(item: BackendDiagnosis): void {
  const id = asString(item.id)
  const version = asNumber(item.version, Number.NaN)
  if (id && Number.isFinite(version)) c6Versions.set(id, version)
}

function requireC6Version(id: string): number {
  const version = c6Versions.get(id)
  if (version === undefined) {
    throw new Error('C6 诊断数据未返回 version，请先由后端补充版本号后再修改或启停')
  }
  return version
}

function mapListItem(item: BackendDiagnosis): DiagnosisRecord {
  rememberVersion(item)
  return {
    id: asString(item.id),
    code: asString(item.conditionCode ?? item.diagnoseCode ?? item.diseaseCode),
    name: asString(item.name ?? item.diagnoseName),
    pyCode: asString(item.pyStr ?? item.pinyinCode),
    type: asString(item.sourceEnum ?? item.source_enum ?? item.diagnoseType),
    typeName: asString(item.sourceEnum ?? item.source_enum ?? item.diagnoseType),
    category: asString(item.typeCode ?? item.diagType ?? item.catName),
    categoryName: asString(item.catName ?? item.typeCode ?? item.diagType),
    enabled: parseStatus(item.statusEnum ?? item.status),
    sort: 0,
    remark: asString(item.description ?? item.remark),
    updatedAt: asString(item.updatedAt)
  }
}

function mapDetail(item: BackendDiagnosis): DiagnosisDetail {
  const record = mapListItem(item)
  return {
    id: record.id,
    code: record.code,
    name: record.name,
    pyCode: record.pyCode,
    type: record.type as DiagnosisType,
    category: record.category as DiagnosisCategory,
    enabled: record.enabled,
    sort: record.sort,
    remark: record.remark
  }
}

function toJ4CreateDTO(data: DiagnosisSavePayload) {
  return {
    diseaseCode: data.code,
    name: data.name,
    pyStr: data.pyCode,
    sourceEnum: data.type,
    typeCode: data.category,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    description: data.remark
  }
}

function toJ4UpdateDTO(id: string, data: DiagnosisSavePayload) {
  return { ...toJ4CreateDTO(data), id, conditionCode: data.code }
}

function toC6DTO(data: DiagnosisSavePayload, id?: string) {
  return {
    id,
    name: data.name,
    diagType: data.category,
    chronicCode: data.code,
    medicalInsuranceCode: '',
    status: data.enabled ? 'ACTIVE' : 'INACTIVE',
    pinyinCode: data.pyCode,
    remark: data.remark,
    catName: data.category,
    source_enum: data.type,
    ...(id ? { version: requireC6Version(id) } : {})
  }
}

function localDetail(id: string | undefined, data: DiagnosisSavePayload): DiagnosisDetail {
  return { ...data, id }
}

export const listDiagnosis = async (
  query: DiagnosisQuery
): Promise<{ rows: DiagnosisRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendDiagnosis>>('/diagnose/page', {
            keyword: query.keyword ?? '',
            diagnoseType: query.type ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendDiagnosis>>('/diagnosis/query-list', {
            keyword: query.keyword ?? '',
            sourceEnum: query.type ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    const rows = page.rows
      .map(mapListItem)
      .filter((row) => (query.enabled == null ? true : row.enabled === query.enabled))
    return { rows, total: query.enabled == null ? page.total : rows.length }
  } catch (error) {
    return mockOrThrow(error, () => mockListDiagnosis(query))
  }
}

export const getDiagnosisDetail = async (id: string): Promise<DiagnosisDetail> => {
  try {
    const http = useProfiledHttp()
    const res =
      getApiProfile() === 'c6'
        ? await http.get<BackendDiagnosis>('/diagnose/detail', { id })
        : await http.get<BackendDiagnosis>(`/diagnosis/get-detail/${encodeURIComponent(id)}`)
    if (!res.data) throw new Error('获取诊断详情失败')
    return mapDetail(res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetDiagnosisDetail(id))
  }
}

export const createDiagnosis = async (data: DiagnosisSavePayload): Promise<DiagnosisDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendDiagnosis | string>('/diagnose', toC6DTO(data))
        : await http.post<BackendDiagnosis | string>(
            '/diagnosis/add-definition',
            toJ4CreateDTO(data)
          )
    return typeof res.data === 'object' && res.data
      ? mapDetail(res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateDiagnosis(data))
  }
}

export const updateDiagnosis = async (
  id: string,
  data: DiagnosisSavePayload
): Promise<DiagnosisDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.put<BackendDiagnosis | string>('/diagnose', toC6DTO(data, id))
        : await http.put<BackendDiagnosis | string>(
            '/diagnosis/modify-definition',
            toJ4UpdateDTO(id, data)
          )
    return typeof res.data === 'object' && res.data ? mapDetail(res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateDiagnosis(id, data))
  }
}

export const setDiagnosisEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    const status = enabled ? 'ACTIVE' : 'INACTIVE'
    if (getApiProfile() === 'c6') {
      await http.post('/diagnose/batch/status', {
        ids: [{ id, version: requireC6Version(id) }],
        status
      })
    } else {
      await http.put('/diagnosis/modify-batch-status', { ids: [id], status })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetDiagnosisEnabled(id, enabled))
  }
}

export const removeDiagnosis = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    const path = getApiProfile() === 'c6' ? '/diagnose' : '/diagnosis/remove-batch'
    await http.delete(path, { ids: [id] })
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveDiagnosis(id))
  }
}

/** 诊断类型和分类暂无两端统一接口，明确使用本地配置。 */
export const loadDiagnosisFormOptions = async (): Promise<DiagnosisFormOptions> =>
  mockLoadDiagnosisFormOptions()

/** 医生站只读诊断字典接口，保留独立入口，不影响诊断管理接口。 */
export async function listDoctorStationDiagnosis(
  query: DiagnosisQuery
): Promise<{ rows: DiagnosisRecord[]; total: number }> {
  const response = await useHttp().get<{ rows?: DiagnosisRecord[]; total?: number }>(
    '/doctor-station/page-diagnosis-dictionary',
    { pageIndex: query.pageIndex, pageSize: query.pageSize, ...(query.keyword?.trim() ? { keyword: query.keyword.trim() } : {}) }
  )
  if (response.code !== 10000 || !response.data || !Array.isArray(response.data.rows)) {
    throw new Error(response.message || '诊断字典加载失败')
  }
  return { rows: response.data.rows, total: Number(response.data.total ?? 0) }
}

export type * from './type'
