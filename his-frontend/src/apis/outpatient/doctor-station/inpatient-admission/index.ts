import {
  mockCheckInpatientAdmission,
  mockListInpatientAdmissionWards,
  mockLoadInpatientAdmissionOptions,
  mockRequestInpatientAdmission
} from './mock'
import type {
  CheckInpatientAdmissionVO,
  InpatientAdmissionFormOptionsVO,
  InpatientAdmissionWardVO,
  RequestInpatientAdmissionDTO,
  RequestInpatientAdmissionVO
} from './type'

/**
 * 查询当前门诊就诊是否已经存在未取消的住院申请。
 *
 * 接入后端后替换为 POST /doctor-station/inpatient-admission/check。
 */
export function checkInpatientAdmission(encounterId: string): Promise<CheckInpatientAdmissionVO> {
  return mockCheckInpatientAdmission(encounterId)
}

/** 加载住院科室、入院类型、入院方式和优先级。 */
export function loadInpatientAdmissionOptions(): Promise<InpatientAdmissionFormOptionsVO> {
  return mockLoadInpatientAdmissionOptions()
}

/** 根据住院科室加载可选病区。 */
export function listInpatientAdmissionWards(
  departmentId: string
): Promise<InpatientAdmissionWardVO[]> {
  return mockListInpatientAdmissionWards(departmentId)
}

/**
 * 提交住院申请。
 *
 * 医生和操作人身份应由真实后端从登录令牌获取，前端 DTO 不传这两个字段。
 */
export function requestInpatientAdmission(
  data: RequestInpatientAdmissionDTO
): Promise<RequestInpatientAdmissionVO> {
  return mockRequestInpatientAdmission(data)
}

export type * from './type'
