import { useHttp } from '@/plugins/http'
import { useUserStore } from '@/stores/user'

import type { PageDTO } from '../../type'
import type {
  CreateInpatientDiagnosisDTO,
  DeleteInpatientDiagnosisDTO,
  InpatientDiagnosisAddResultVO,
  InpatientDiagnosisBriefVO,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  UpdateInpatientDiagnosisDTO
} from './type'

const realApiBaseURL =
  import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'

interface BackendDiagnosisRow {
  id?: string
  diagnosisId?: string
  diagnosis_id?: string
  conditionId?: string
  condition_id?: string
  definitionCode?: string
  definition_code?: string
  description?: string
  primaryFlag?: number | boolean | string
  primary_flag?: number | boolean | string
  main_flag?: number | boolean | string
  rank?: number | string
  tcmFlag?: number | boolean | string
  tcm_flag?: number | boolean | string
  kindName?: string
  kind_name?: string
  diagnosisTypeCode?: string
  diagnosis_type_code?: string
  diagTypeCode?: string
  diag_type_code?: string
  typeCode?: string
  type_code?: string
  diagnosisTypeName?: string
  diagnosis_type_name?: string
  typeName?: string
  type_name?: string
}

interface BackendDiagnosisPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendDiagnosisRow[]
}

interface BackendDiagnosisBriefRow {
  conditionId?: string
  condition_id?: string
  definitionName?: string
  definition_name?: string
}

interface BackendDiagnosisBriefPage {
  rows?: BackendDiagnosisBriefRow[]
}

type BackendDiagnosisBriefData =
  | BackendDiagnosisBriefRow
  | BackendDiagnosisBriefRow[]
  | BackendDiagnosisBriefPage

type BackendDiagnosisAddResult =
  | string
  | {
      diagnosisId?: string
      diagnosis_id?: string
      conditionId?: string
      condition_id?: string
      clnId?: string
      cln_id?: string
    }

function getTenantId(): string {
  const tenantId = useUserStore().tenantId
  const value = tenantId === null || tenantId === undefined ? '' : String(tenantId).trim()
  if (!value) throw new Error('当前登录上下文缺少租户 ID，无法操作诊断')
  return value
}

function toDiagnosisError(error: unknown, fallback: string): Error {
  if (error instanceof Error) return error
  if (error && typeof error === 'object' && 'message' in error) {
    const message = (error as { message?: unknown }).message
    if (typeof message === 'string' && message.trim()) return new Error(message)
  }
  return new Error(fallback)
}

function unwrapData<T>(response: { data?: T; message?: string }): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || '后端未返回有效诊断数据')
  }
  return response.data
}

function toFlag(value: number | boolean | string | undefined, trueText?: string): number {
  if (value === true || value === 1 || value === '1') return 1
  if (typeof value === 'string') {
    const normalized = value.trim().toLowerCase()
    if (normalized === 'true' || normalized === '是' || normalized === trueText) return 1
  }
  return 0
}

function mapDiagnosisRow(row: BackendDiagnosisRow, index: number): InpatientDiagnosisRecordVO {
  const conditionId = row.conditionId ?? row.condition_id ?? ''
  const kindName = row.kindName ?? row.kind_name ?? ''
  const diagnosisTypeName =
    row.diagnosisTypeName ?? row.diagnosis_type_name ?? row.typeName ?? row.type_name ?? ''
  const diagnosisTypeCode =
    row.diagnosisTypeCode ??
    row.diagnosis_type_code ??
    row.diagTypeCode ??
    row.diag_type_code ??
    row.typeCode ??
    row.type_code ??
    diagnosisTypeName
  return {
    id: row.diagnosisId ?? row.diagnosis_id ?? row.id ?? conditionId,
    conditionId,
    definitionCode: (row.definitionCode ?? row.definition_code ?? '').trim(),
    description: row.description?.trim() ?? '',
    kindName: kindName.trim() || (toFlag(row.tcmFlag ?? row.tcm_flag) === 1 ? '中医' : '西医'),
    diagnosisTypeCode: diagnosisTypeCode.trim(),
    diagnosisTypeName: diagnosisTypeName.trim() || diagnosisTypeCode.trim(),
    primaryFlag: toFlag(row.primaryFlag ?? row.primary_flag ?? row.main_flag),
    rank: Number(row.rank) || index + 1,
    tcmFlag: toFlag(row.tcmFlag ?? row.tcm_flag ?? kindName, '中医')
  }
}

function parseAddResult(data: BackendDiagnosisAddResult): InpatientDiagnosisAddResultVO {
  if (typeof data === 'object') {
    const diagnosisId = data.diagnosisId ?? data.diagnosis_id ?? ''
    const conditionId = data.conditionId ?? data.condition_id ?? data.clnId ?? data.cln_id ?? ''
    if (diagnosisId && conditionId) return { diagnosisId, conditionId }
  }

  if (typeof data === 'string') {
    const diagnosisId = data.match(/diagnosis_id:\s*([^,]+)/i)?.[1]?.trim() ?? ''
    const conditionId = data.match(/(?:cln_id|condition_id):\s*([^,]+)/i)?.[1]?.trim() ?? ''
    if (diagnosisId && conditionId && !data.includes('insert failed')) {
      return { diagnosisId, conditionId }
    }
  }

  throw new Error('后端未返回完整的诊断 ID 和病情 ID')
}

export async function realPageInpatientDiagnosisRecords(
  query: InpatientDiagnosisRecordQuery
): Promise<PageDTO<InpatientDiagnosisRecordVO>> {
  try {
    const response = await useHttp().get<BackendDiagnosisPage>(
      '/diagnosis-entry/detail',
      {
        tenant_id: getTenantId(),
        'patient-id': query.patientId,
        pageIndex: query.pageIndex,
        pageSize: query.pageSize
      },
      { baseURL: realApiBaseURL }
    )
    const page = unwrapData(response)
    const rows = (page.rows ?? [])
      .map(mapDiagnosisRow)
      .filter((row) => Boolean(row.id && row.conditionId && row.description))
      .sort((left, right) => left.rank - right.rank)
    return {
      pageIndex: page.pageIndex ?? query.pageIndex,
      pageSize: page.pageSize ?? query.pageSize,
      total: page.total ?? rows.length,
      pages: page.pages ?? Math.ceil((page.total ?? rows.length) / Math.max(query.pageSize, 1)),
      rows
    }
  } catch (error) {
    throw toDiagnosisError(error, '住院诊断加载失败')
  }
}

export async function realListInpatientDiagnosisBrief(): Promise<InpatientDiagnosisBriefVO[]> {
  try {
    const response = await useHttp().get<BackendDiagnosisBriefData>(
      '/diagnosis-entry/brief',
      { tenant_id: getTenantId() },
      { baseURL: realApiBaseURL }
    )
    const data = unwrapData(response)
    let rows: BackendDiagnosisBriefRow[]
    if (Array.isArray(data)) rows = data
    else if ('rows' in data) rows = data.rows ?? []
    else rows = [data as BackendDiagnosisBriefRow]
    return [...new Set(rows.map((row) => row.definitionName ?? row.definition_name ?? ''))]
      .map((definitionName) => definitionName.trim())
      .filter(Boolean)
      .map((definitionName) => ({ definitionName }))
  } catch (error) {
    throw toDiagnosisError(error, '历史诊断加载失败')
  }
}

export async function realCreateInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  try {
    const response = await useHttp().post<BackendDiagnosisAddResult>(
      '/diagnosis/add',
      {
        tenant_id: getTenantId(),
        encounter_id: data.encounterId,
        ...(data.definitionCode ? { condition_code: data.definitionCode } : {}),
        description: data.description,
        primary_flag: data.primaryFlag,
        set_always: 0,
        rank: data.rank,
        tcm_flag: data.tcmFlag
      },
      { baseURL: realApiBaseURL }
    )
    const result = parseAddResult(unwrapData(response))
    return {
      id: result.diagnosisId,
      conditionId: result.conditionId,
      definitionCode: data.definitionCode ?? '',
      description: data.description,
      kindName: data.tcmFlag === 1 ? '中医' : '西医',
      diagnosisTypeCode: data.diagnosisTypeCode ?? '',
      diagnosisTypeName: data.diagnosisTypeCode ?? '',
      primaryFlag: data.primaryFlag,
      rank: data.rank,
      tcmFlag: data.tcmFlag
    }
  } catch (error) {
    throw toDiagnosisError(error, '新增住院诊断失败')
  }
}

export async function realUpdateInpatientDiagnosis(
  data: UpdateInpatientDiagnosisDTO
): Promise<void> {
  try {
    await useHttp().put(
      '/diagnosis/modify',
      {
        id: data.id,
        diag_type_code: data.diagnosisTypeCode
      },
      { baseURL: realApiBaseURL }
    )
  } catch (error) {
    throw toDiagnosisError(error, '修改住院诊断失败')
  }
}

export async function realDeleteInpatientDiagnosis(
  data: DeleteInpatientDiagnosisDTO
): Promise<void> {
  try {
    await useHttp().delete(`/diagnosis/${encodeURIComponent(data.id)}`, undefined, {
      baseURL: realApiBaseURL
    })
  } catch (error) {
    throw toDiagnosisError(error, '删除住院诊断失败')
  }
}
