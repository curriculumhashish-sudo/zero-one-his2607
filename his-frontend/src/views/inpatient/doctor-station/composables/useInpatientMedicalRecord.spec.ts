import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  getInpatientMedicalRecord,
  listInpatientMedicalRecordDocumentTypes,
  saveInpatientMedicalRecord,
  type InpatientDoctorPatientQueueItemVO,
  type InpatientMedicalRecordVO
} from '@/apis/inpatient/doctor-station'
import { useInpatientMedicalRecord } from './useInpatientMedicalRecord'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  getInpatientMedicalRecord: vi.fn(),
  listInpatientMedicalRecordDocumentTypes: vi.fn(),
  saveInpatientMedicalRecord: vi.fn()
}))

const patient: InpatientDoctorPatientQueueItemVO = {
  encounterId: 'enc-1',
  patientId: 'patient-1',
  patientName: '测试患者',
  genderName: '男',
  ageText: '50岁',
  inpatientNo: 'ZY001',
  admissionAt: '2026-07-28 08:00',
  departmentName: '心血管内科',
  wardId: 'ward-1',
  wardName: '心内一病区',
  bedId: 'bed-1',
  bedName: '01床',
  statusEnum: 'IN_PROGRESS'
}

function draftRecord(overrides: Partial<InpatientMedicalRecordVO> = {}): InpatientMedicalRecordVO {
  return {
    id: 'record-1',
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    inpatientNo: patient.inpatientNo,
    documentTypeCode: 'DAILY_COURSE_RECORD',
    documentTypeName: '日常病程记录',
    title: '日常病程记录',
    status: 'DRAFT',
    statusName: '草稿',
    departmentName: patient.departmentName,
    authoredByName: '李医生',
    authoredAt: '2026-07-28 10:00:00',
    updatedAt: '2026-07-28 10:00:00',
    content: '<p>病情平稳。</p>',
    version: 1,
    readonly: false,
    ...overrides
  }
}

describe('useInpatientMedicalRecord', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    vi.mocked(listInpatientMedicalRecordDocumentTypes).mockResolvedValue([
      {
        value: 'DAILY_COURSE_RECORD',
        label: '日常病程记录',
        defaultTitle: '日常病程记录',
        defaultContent: '<h3>日常病程记录</h3><p></p>'
      }
    ])
  })

  it('新建病历后保存草稿，保存结果会成为新的未修改快照', async () => {
    const created = draftRecord()
    vi.mocked(saveInpatientMedicalRecord).mockResolvedValue(created)
    const state = useInpatientMedicalRecord()

    await state.loadDocumentTypes()
    state.startNewMedicalRecord('DAILY_COURSE_RECORD')

    expect(state.dirty.value).toBe(true)
    expect(state.title.value).toBe('日常病程记录')

    const saved = await state.saveMedicalRecordDraft(patient)

    expect(saveInpatientMedicalRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        encounterId: patient.encounterId,
        patientId: patient.patientId,
        documentTypeCode: 'DAILY_COURSE_RECORD'
      })
    )
    expect(saved?.id).toBe(created.id)
    expect(state.dirty.value).toBe(false)
  })
})
