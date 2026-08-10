import dayjs from 'dayjs'

import { useHttp } from '@/plugins/http'
import { useUserStore } from '@/stores/user'

import type {
  CreateInpatientDiagnosisDTO,
  InpatientDiagnosisRecordVO,
  UpdateInpatientDiagnosisDTO
} from './type'

const realApiBaseURL =
  import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'

function unwrapId(data: unknown, message: string | undefined, fallback: string): string {
  const id = typeof data === 'string' ? data.trim() : ''
  if (id) return id
  if (fallback) return fallback
  throw new Error(message || '后端未返回诊断记录 ID')
}

function toDiagnosisError(error: unknown, fallback: string): Error {
  if (error instanceof Error) return error
  if (error && typeof error === 'object' && 'message' in error) {
    const message = (error as { message?: unknown }).message
    if (typeof message === 'string' && message.trim()) return new Error(message)
  }
  return new Error(fallback)
}

function getTenantId(): string {
  const tenantId = useUserStore().tenantId
  if (tenantId === null || tenantId === undefined || String(tenantId).trim() === '') {
    throw new Error('当前登录上下文缺少租户 ID，无法保存诊断')
  }
  return String(tenantId)
}

function toConditionCode(code: string): number | undefined {
  const normalized = code.trim()
  if (!/^\d+$/.test(normalized)) return undefined
  const value = Number(normalized)
  return Number.isSafeInteger(value) ? value : undefined
}

/** 调用真实后端新增诊断。 */
export async function realCreateInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  try {
    const payload: Record<string, unknown> = {
      tenant_id: getTenantId(),
      encounter_id: data.encounterId,
      description: data.name,
      tcm_flag: 0,
      primary_flag: data.primary ? 1 : 0,
      rank: data.orderNo
    }
    const conditionCode = toConditionCode(data.code)
    if (conditionCode !== undefined) payload.condition_code = conditionCode

    const response = await useHttp().post<string>('/diagnosis/add', payload, {
      baseURL: realApiBaseURL
    })
    const id = unwrapId(response.data, response.message, '')
    const now = dayjs().format('YYYY-MM-DD HH:mm:ss')
    return {
      id,
      backendId: id,
      source: 'real',
      readonlyFields: true,
      encounterId: data.encounterId,
      patientId: data.patientId,
      catalogId: data.catalogId,
      code: data.code,
      name: data.name,
      typeName: data.typeName,
      typeCode: data.typeName,
      primary: data.primary,
      orderNo: data.orderNo,
      diagnosedAt: now,
      updatedAt: now,
      version: 0
    }
  } catch (error) {
    throw toDiagnosisError(error, '新增住院诊断失败')
  }
}

/** 调用真实后端修改诊断类型；其他字段由页面层阻止提交。 */
export async function realUpdateInpatientDiagnosis(
  diagnosisId: string,
  data: UpdateInpatientDiagnosisDTO,
  current: InpatientDiagnosisRecordVO
): Promise<InpatientDiagnosisRecordVO> {
  try {
    const response = await useHttp().put<string>('/diagnosis/modify', {
      id: current.backendId || diagnosisId,
      diag_type_code: data.typeName
    }, {
      baseURL: realApiBaseURL
    })
    unwrapId(response.data, response.message, current.backendId || diagnosisId)
    return {
      ...current,
      typeName: data.typeName,
      typeCode: data.typeName,
      updatedAt: dayjs().format('YYYY-MM-DD HH:mm:ss')
    }
  } catch (error) {
    throw toDiagnosisError(error, '修改住院诊断类型失败')
  }
}

/** 调用真实后端删除诊断。 */
export async function realDeleteInpatientDiagnosis(
  diagnosisId: string,
  backendId?: string
): Promise<void> {
  try {
    const id = backendId || diagnosisId
    const response = await useHttp().delete<boolean>(`/diagnosis/${id}`, undefined, {
      baseURL: realApiBaseURL
    })
    if (response.data === false) throw new Error(response.message || '后端未删除诊断')
  } catch (error) {
    throw toDiagnosisError(error, '删除住院诊断失败')
  }
}
