import * as mock from './mock'
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

export const loadInpatientRegistrationInit = mock.mockLoadInpatientRegistrationInit
export const pageRegistrationPatientOptions = mock.mockPageRegistrationPatientOptions

export function pageInpatientRegistrationQueue(
  query: RegistrationQueueQuery,
  context: RequestContext = {},
  mockFilter: RegistrationQueueMockFilter = {}
) {
  return mock.mockPageInpatientRegistrationQueue(query, context, mockFilter)
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
  return mock.mockGetInpatientRegistrationContext({ encounterId }, context)
}

export function listRegistrationWards(departmentId: string, context: RequestContext = {}) {
  return mock.mockListRegistrationWards({ organizationId: departmentId }, context)
}

export function getWardCapacity(wardId: string, context: RequestContext = {}) {
  return mock.mockGetWardCapacity({ wardId }, context)
}

export function listAttendingDoctors(query: AttendingDoctorQuery, context: RequestContext = {}) {
  return mock.mockListAttendingDoctors(query, context)
}

export function submitInpatientRegistration(data: SubmitRegistrationPayload) {
  return mock.mockSubmitInpatientRegistration(data)
}

function getOperatorId() {
  const operatorId = useUserStore().getUser?.id
  if (!operatorId) throw new Error('当前登录用户信息未加载，无法执行登记操作')
  return operatorId
}

export function cancelInpatientRegistration(
  id: string,
  reason?: string,
  context: RequestContext = {}
) {
  return mock.mockCancelInpatientRegistration(
    { cancelReason: reason, encounterId: id, operatorId: getOperatorId() },
    context
  )
}

export function revokeInsuranceAdmission(
  id: string,
  reason?: string,
  context: RequestContext = {}
) {
  return mock.mockRevokeInsuranceAdmission(
    { cancelReason: reason, encounterId: id, operatorId: getOperatorId() },
    context
  )
}

export type * from './type'
export type {
  LoadRegisterBenefitPreviewParams,
  LoadRegisterBenefitPreviewResult
} from '@/apis/outpatient/charge/register'
