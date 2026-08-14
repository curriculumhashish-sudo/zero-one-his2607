// @vitest-environment jsdom

import { computed, effectScope, ref } from 'vue'
import { beforeEach, describe, expect, it, vi } from 'vitest'

const { getMedicalRecord, saveMedicalRecord } = vi.hoisted(() => ({
  getMedicalRecord: vi.fn(),
  saveMedicalRecord: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', () => ({
  getMedicalRecord,
  saveMedicalRecord
}))

import { useDoctorMedicalRecord } from './useDoctorMedicalRecord'

describe('useDoctorMedicalRecord', () => {
  beforeEach(() => vi.clearAllMocks())

  function createState() {
    const scope = effectScope()
    const selectedPatientId = ref<string | number | null>('encounter-1')
    const state = scope.run(() =>
      useDoctorMedicalRecord({
        selectedPatientId,
        patientAccepted: computed(() => true)
      })
    )!
    return { scope, state }
  }

  it('creates an empty editable record when backend has no content', async () => {
    getMedicalRecord.mockRejectedValue(new Error('Medical record content not found'))

    const { scope, state } = createState()
    await state.loadCurrentMedicalRecord('encounter-1')

    expect(state.medicalRecordError.value).toBe('')
    expect(state.medicalRecord.value).toEqual({ encounterId: 'encounter-1', content: '' })
    expect(state.medicalRecordContent.value).toBe('')
    scope.stop()
  })

  it('does not turn permission errors into an empty record', async () => {
    getMedicalRecord.mockRejectedValue(new Error('没有权限查看该病历'))

    const { scope, state } = createState()
    await state.loadCurrentMedicalRecord('encounter-1')

    expect(state.medicalRecord.value).toBeNull()
    expect(state.medicalRecordError.value).toBe('没有权限查看该病历')
    scope.stop()
  })

  it('allows first save without an existing record object', async () => {
    getMedicalRecord.mockRejectedValue(new Error('Medical record content not found'))
    saveMedicalRecord.mockResolvedValue({
      id: 'record-1',
      encounterId: 'encounter-1',
      content: '<p>主诉</p>'
    })

    const { scope, state } = createState()
    await state.loadCurrentMedicalRecord('encounter-1')
    state.medicalRecordContent.value = '<p>主诉</p>'

    await expect(state.saveCurrentMedicalRecord(false)).resolves.toBe(true)
    expect(saveMedicalRecord).toHaveBeenCalledWith('encounter-1', { content: '<p>主诉</p>' })
    scope.stop()
  })
})
