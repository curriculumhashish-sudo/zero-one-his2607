import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  mockCheckInpatientAdmission,
  mockListInpatientAdmissionWards,
  mockLoadInpatientAdmissionOptions,
  mockRequestInpatientAdmission
} from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

function createRequest(encounterId: string) {
  return {
    encounterId,
    patientId: `patient-${encounterId}`,
    targetDepartmentId: 'inpatient-dept-cardio',
    wardId: 'ward-cardio-1',
    admissionDiagnoses: [
      {
        catalogId: 'diagnosis-hypertension',
        diagnosisRecordId: 'saved-diagnosis-1',
        code: 'I10',
        name: '高血压',
        description: '高血压',
        primary: true as const
      }
    ],
    admissionWayCode: 'OUTPATIENT_TRANSFER'
  }
}

describe('inpatient-admission mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('加载办理住院选项，并按科室返回病区', async () => {
    const options = await resolveMockRequest(mockLoadInpatientAdmissionOptions())
    expect(options.departmentOptions.length).toBeGreaterThan(0)
    expect(options.admissionWayOptions).toContainEqual({
      value: 'OUTPATIENT_TRANSFER',
      label: '门诊转住院'
    })

    const wards = await resolveMockRequest(mockListInpatientAdmissionWards('inpatient-dept-cardio'))
    expect(wards).toHaveLength(2)
    expect(wards.every((ward) => ward.departmentId === 'inpatient-dept-cardio')).toBe(true)
  })

  it('提交后生成待登记住院申请，并可查询防重复状态', async () => {
    const encounterId = 'enc-admission-test-success'
    const before = await resolveMockRequest(mockCheckInpatientAdmission(encounterId))
    expect(before.exists).toBe(false)

    const submitted = await resolveMockRequest(
      mockRequestInpatientAdmission(createRequest(encounterId))
    )
    expect(submitted.success).toBe(true)
    expect(submitted.status).toBe('PLANNED')
    expect(submitted.inpatientEncounterNo).toMatch(/^IP/)

    const after = await resolveMockRequest(mockCheckInpatientAdmission(encounterId))
    expect(after.exists).toBe(true)
    expect(after.applicationId).toBe(submitted.applicationId)
  })

  it('拒绝同一次门诊就诊重复提交住院申请', async () => {
    const encounterId = 'enc-admission-test-duplicate'
    await resolveMockRequest(mockRequestInpatientAdmission(createRequest(encounterId)))

    const request = mockRequestInpatientAdmission(createRequest(encounterId))
    const rejection = expect(request).rejects.toThrow('请勿重复提交')
    await vi.runAllTimersAsync()
    await rejection
  })

  it('拒绝与入院科室不匹配的病区', async () => {
    const request = mockRequestInpatientAdmission({
      ...createRequest('enc-admission-test-invalid-ward'),
      targetDepartmentId: 'inpatient-dept-neuro',
      wardId: 'ward-cardio-1'
    })
    const rejection = expect(request).rejects.toThrow('入院病区与入院科室不匹配')
    await vi.runAllTimersAsync()
    await rejection
  })
})
