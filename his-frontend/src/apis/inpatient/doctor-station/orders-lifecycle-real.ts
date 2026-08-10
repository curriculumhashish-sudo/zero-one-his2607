import { useHttp } from '@/plugins/http'
import { useUserStore } from '@/stores/user'

import type {
  GroupInpatientMedicalOrdersDTO,
  InpatientMedicalOrderBatchActionDTO,
  InpatientMedicalOrderBatchResultVO
} from './type'

const realApiBaseURL =
  import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_URL || '/inpatient-doctor-station-api'

interface JsonVO<T> {
  code?: number
  message?: string
  data?: T
}

interface MergeResultDTO {
  groupId?: string
  errorMessage?: string
}

interface SplitResultDTO {
  splitCount?: number
  errorMessage?: string
}

interface StopFailDetailDTO {
  requestId?: string
  reason?: string
}

interface StopResultDTO {
  successCount?: number
  failCount?: number
  failDetails?: StopFailDetailDTO[]
}

function getOperatorId(): string {
  const user = useUserStore().getUser
  const operatorId = user?.id || user?.username
  if (!operatorId?.trim()) throw new Error('当前登录用户缺少操作人 ID，无法执行医嘱操作')
  return operatorId
}

function toError(error: unknown, fallback: string): Error {
  if (error instanceof Error) return error
  if (error && typeof error === 'object' && 'message' in error) {
    const message = (error as { message?: unknown }).message
    if (typeof message === 'string' && message.trim()) return new Error(message)
  }
  return new Error(fallback)
}

function unwrap<T>(response: { data?: T | JsonVO<T>; message?: string }, fallback: string): T {
  const body = response.data
  if (body && typeof body === 'object' && 'data' in body) {
    const wrapped = body as JsonVO<T>
    if (wrapped.data === undefined || wrapped.data === null) {
      throw new Error(wrapped.message || response.message || fallback)
    }
    return wrapped.data
  }
  if (body === undefined || body === null) throw new Error(response.message || fallback)
  return body as T
}

function ensureProductTableNames(data: InpatientMedicalOrderBatchActionDTO) {
  const missing = data.items.find((item) => !item.id || !item.productTableName?.trim())
  if (missing) throw new Error('所选医嘱缺少产品定义表名，暂不能执行真实生命周期操作')
}

function basePayload(data: InpatientMedicalOrderBatchActionDTO) {
  return {
    encounterId: data.encounterId,
    patientId: data.patientId,
    operatorId: getOperatorId()
  }
}

function countResult(updatedCount: number, extra: Partial<InpatientMedicalOrderBatchResultVO> = {}) {
  return { updatedCount, rows: [], ...extra }
}

/** 调用真实后端签发医嘱接口。 */
export async function realSignInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    ensureProductTableNames(data)
    const response = await useHttp().post<string | JsonVO<string>>(
      '/medical-order/sign',
      {
        encounter_id: data.encounterId,
        patient_id: data.patientId,
        signTargets: data.items.map((item) => ({
          productTableName: item.productTableName,
          id: item.id
        }))
      },
      { baseURL: realApiBaseURL }
    )
    unwrap(response, '真实医嘱签发失败')
    return countResult(data.items.length)
  } catch (error) {
    throw toError(error, '真实医嘱签发失败')
  }
}

/** 调用真实后端撤回医嘱接口。 */
export async function realWithdrawInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    ensureProductTableNames(data)
    const response = await useHttp().post<string | JsonVO<string>>(
      '/medical-order/withdraw',
      {
        encounter_id: data.encounterId,
        patient_id: data.patientId,
        targets: data.items.map((item) => ({
          productTableName: item.productTableName,
          id: item.id
        }))
      },
      { baseURL: realApiBaseURL }
    )
    unwrap(response, '真实医嘱撤回失败')
    return countResult(data.items.length)
  } catch (error) {
    throw toError(error, '真实医嘱撤回失败')
  }
}

/** 调用真实后端停嘱接口。 */
export async function realStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    ensureProductTableNames(data)
    if (!Number.isInteger(data.stopReason)) throw new Error('请填写有效的停嘱原因码')
    const response = await useHttp().post<StopResultDTO | JsonVO<StopResultDTO>>(
      '/order/stop-order',
      {
        ...basePayload(data),
        stopReason: data.stopReason,
        lines: data.items.map((item) => ({
          requestId: item.id,
          productTableName: item.productTableName
        }))
      },
      { baseURL: realApiBaseURL }
    )
    const result = unwrap(response, '真实医嘱停嘱失败')
    const failureMessages = (result.failDetails ?? [])
      .map((item) => item.reason?.trim())
      .filter((reason): reason is string => Boolean(reason))
    return countResult(result.successCount ?? 0, {
      failedCount: result.failCount ?? 0,
      failureMessages
    })
  } catch (error) {
    throw toError(error, '真实医嘱停嘱失败')
  }
}

/** 调用真实后端取消停嘱接口。 */
export async function realCancelStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    ensureProductTableNames(data)
    const response = await useHttp().post<string | JsonVO<string>>(
      '/medical-order/cancel-stop',
      {
        encounter_id: data.encounterId,
        patient_id: data.patientId,
        lines: data.items.map((item) => ({
          productTableName: item.productTableName,
          id: item.id
        }))
      },
      { baseURL: realApiBaseURL }
    )
    unwrap(response, '真实医嘱取消停嘱失败')
    return countResult(data.items.length)
  } catch (error) {
    throw toError(error, '真实医嘱取消停嘱失败')
  }
}

/** 调用真实后端并组医嘱接口。 */
export async function realGroupInpatientMedicalOrders(
  data: GroupInpatientMedicalOrdersDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const response = await useHttp().post<MergeResultDTO | JsonVO<MergeResultDTO>>(
      '/order/merge_order',
      {
        ...basePayload(data),
        clearGroupIds: [...new Set(data.items.map((item) => item.groupId).filter(Boolean))],
        assignGroup: {
          groupId: data.groupId || data.items.find((item) => item.groupId)?.groupId,
          medicationRequestIds: data.items.map((item) => item.id)
        }
      },
      { baseURL: realApiBaseURL }
    )
    const result = unwrap(response, '真实医嘱并组失败')
    if (result.errorMessage) throw new Error(result.errorMessage)
    return countResult(data.items.length, { groupId: result.groupId })
  } catch (error) {
    throw toError(error, '真实医嘱并组失败')
  }
}

/** 调用真实后端拆组医嘱接口。 */
export async function realUngroupInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const response = await useHttp().post<SplitResultDTO | JsonVO<SplitResultDTO>>(
      '/order/split-order',
      {
        ...basePayload(data),
        orderIds: data.items.map((item) => item.id)
      },
      { baseURL: realApiBaseURL }
    )
    const result = unwrap(response, '真实医嘱拆组失败')
    if (result.errorMessage) throw new Error(result.errorMessage)
    return countResult(result.splitCount ?? data.items.length)
  } catch (error) {
    throw toError(error, '真实医嘱拆组失败')
  }
}
