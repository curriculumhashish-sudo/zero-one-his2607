import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  getInpatientMedicalRecord,
  listInpatientMedicalRecordDocumentTypes,
  saveInpatientMedicalRecord,
  signInpatientMedicalRecord,
  type InpatientDoctorPatientQueueItemVO,
  type InpatientMedicalRecordVO
} from '@/apis/inpatient/doctor-station'
import { useInpatientMedicalRecord } from './useInpatientMedicalRecord'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  getInpatientMedicalRecord: vi.fn(),
  listInpatientMedicalRecordDocumentTypes: vi.fn(),
  saveInpatientMedicalRecord: vi.fn(),
  signInpatientMedicalRecord: vi.fn()
}))

const patient: InpatientDoctorPatientQueueItemVO = {
  encounterId: 'enc-1',
  patientId: 'patient-1',
  patientName: '测试患者',
  genderName: '男',
  ageText: '50岁',
  patientNo: 'P001',
  inpatientNo: 'ZY001',
  admissionAt: '2026-07-28 08:00',
  departmentName: '心血管内科',
  wardId: 'ward-1',
  wardName: '心内一病区',
  bedId: 'bed-1',
  bedName: '01床',
  encounterStatus: 'IN_HOSPITAL',
  encounterStatusName: '在院',
  version: 1
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
        id: undefined,
        encounterId: patient.encounterId,
        patientId: patient.patientId,
        documentTypeCode: 'DAILY_COURSE_RECORD',
        version: 0
      })
    )
    expect(saved?.id).toBe(created.id)
    expect(state.dirty.value).toBe(false)
    expect(state.canSign.value).toBe(true)
  })

  it('病历有未保存修改时禁止签名，保存后签名会切换为只读', async () => {
    const draft = draftRecord()
    const updated = draftRecord({ content: '<p>病情继续改善。</p>', version: 2 })
    const signed = draftRecord({
      content: updated.content,
      status: 'SIGNED',
      statusName: '已签名',
      version: 3,
      readonly: true
    })
    vi.mocked(getInpatientMedicalRecord).mockResolvedValue(draft)
    vi.mocked(saveInpatientMedicalRecord).mockResolvedValue(updated)
    vi.mocked(signInpatientMedicalRecord).mockResolvedValue(signed)
    const state = useInpatientMedicalRecord()

    await state.loadMedicalRecord(draft.id)
    state.content.value = updated.content

    await expect(state.signMedicalRecordDraft()).rejects.toThrow('请先保存')
    await state.saveMedicalRecordDraft(patient)
    await state.signMedicalRecordDraft()

    expect(signInpatientMedicalRecord).toHaveBeenCalledWith({
      id: draft.id,
      version: updated.version
    })
    expect(state.record.value?.status).toBe('SIGNED')
    expect(state.readonly.value).toBe(true)
    expect(state.dirty.value).toBe(false)
  })
})
