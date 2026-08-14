import * as mock from './mock'
import * as c1 from '@/apis/c1/charge-client'
import type {
  AttendingDoctorQuery,
  RegistrationQueueMockFilter,
  RegistrationQueueQuery,
  RequestContext,
  SubmitRegistrationPayload
} from './type'
import { useUserStore } from '@/stores/user'
import { loadRegisterBenefitPreview as loadSharedRegisterBenefitPreview } from '@/apis/outpatient/charge/register'
import type { LoadRegisterBenefitPreviewParams } from '@/apis/outpatient/charge/register'
const useC1 = import.meta.env.VITE_CHARGE_PROVIDER === 'c1'

export const loadInpatientRegistrationInit = (useC1 ? c1.c1LoadInpatientRegistrationInit : mock.mockLoadInpatientRegistrationInit) as typeof mock.mockLoadInpatientRegistrationInit
export const pageRegistrationPatientOptions = (useC1 ? c1.c1PageRegistrationPatientOptions : mock.mockPageRegistrationPatientOptions) as typeof mock.mockPageRegistrationPatientOptions

export function pageInpatientRegistrationQueue(
  query: RegistrationQueueQuery,
  context: RequestContext = {},
  mockFilter: RegistrationQueueMockFilter = {}
) {
  return (useC1 ? c1.c1PageInpatientRegistrationQueue(query, context, mockFilter) : mock.mockPageInpatientRegistrationQueue(query, context, mockFilter)) as ReturnType<typeof mock.mockPageInpatientRegistrationQueue>
}

export function loadInpatientRegistrationBenefitPreview(
  params: LoadRegisterBenefitPreviewParams,
  context: RequestContext = {}
) {
  return loadSharedRegisterBenefitPreview(params, context)
}

export function getInpatientRegistrationContext(
  encounterId?: string,
  context: RequestContext = {}
) {
  return (useC1 ? c1.c1GetInpatientRegistrationContext(encounterId, context) : mock.mockGetInpatientRegistrationContext({ encounterId }, context)) as ReturnType<typeof mock.mockGetInpatientRegistrationContext>
}

export function listRegistrationWards(departmentId: string, context: RequestContext = {}) {
  return (useC1 ? c1.c1ListRegistrationWards(departmentId, context) : mock.mockListRegistrationWards({ organizationId: departmentId }, context)) as ReturnType<typeof mock.mockListRegistrationWards>
}

export function getWardCapacity(wardId: string, context: RequestContext = {}) {
  return (useC1 ? c1.c1GetWardCapacity(wardId, context) : mock.mockGetWardCapacity({ wardId }, context)) as ReturnType<typeof mock.mockGetWardCapacity>
}

export function listAttendingDoctors(query: AttendingDoctorQuery, context: RequestContext = {}) {
  return (useC1 ? c1.c1ListAttendingDoctors(query, context) : mock.mockListAttendingDoctors(query, context)) as ReturnType<typeof mock.mockListAttendingDoctors>
}

export function submitInpatientRegistration(data: SubmitRegistrationPayload) {
  return (useC1 ? c1.c1SubmitInpatientRegistration(data) : mock.mockSubmitInpatientRegistration(data)) as ReturnType<typeof mock.mockSubmitInpatientRegistration>
}

function readOperatorIdFromToken(token: string) {
  try {
    const encoded = token.split('.')[1]
    if (!encoded || typeof atob !== 'function') return ''
    const base64 = encoded.replaceAll('-', '+').replaceAll('_', '/')
    const bytes = atob(base64.padEnd(Math.ceil(base64.length / 4) * 4, '='))
    const json = decodeURIComponent(
      Array.from(bytes, (char) => `%${char.charCodeAt(0).toString(16).padStart(2, '0')}`).join('')
    )
    const claims = JSON.parse(json) as Record<string, unknown>
    return String(claims.operatorId ?? claims.operator_id ?? claims.userId ?? claims.user_id ?? claims.id ?? '').trim()
  } catch {
    return ''
  }
}

function getOperatorId() {
  const store = useUserStore()
  const fromUser = String(store.getUser?.id ?? '').trim()
  if (fromUser) return fromUser
  if (typeof localStorage !== 'undefined') {
    const fromStorage = String(localStorage.getItem('operatorId') ?? localStorage.getItem('userId') ?? '').trim()
    if (fromStorage) return fromStorage
  }
  const fromToken = readOperatorIdFromToken(String(store.getToken ?? '').trim())
  if (fromToken) return fromToken
  throw new Error('当前登录用户信息未加载，无法执行登记操作')
}

export function cancelInpatientRegistration(
  id: string,
  reason?: string,
  context: RequestContext = {}
) {
  const payload = { cancelReason: reason, encounterId: id, operatorId: getOperatorId() }
  return (useC1 ? c1.c1CancelInpatientRegistration(payload, context) : mock.mockCancelInpatientRegistration(payload, context)) as ReturnType<typeof mock.mockCancelInpatientRegistration>
}

export function revokeInsuranceAdmission(
  id: string,
  reason?: string,
  context: RequestContext = {}
) {
  const payload = { cancelReason: reason, encounterId: id, operatorId: getOperatorId() }
  return (useC1 ? c1.c1RevokeInsuranceAdmission(payload, context) : mock.mockRevokeInsuranceAdmission(payload, context)) as ReturnType<typeof mock.mockRevokeInsuranceAdmission>
}

export type * from './type'
export type {
  LoadRegisterBenefitPreviewParams,
  LoadRegisterBenefitPreviewResult
} from '@/apis/outpatient/charge/register'
