import { beforeEach, describe, expect, it, vi } from 'vitest'

import { pageInpatientDoctorPatientQueue } from '@/apis/inpatient/doctor-station'
import type { InpatientDoctorPatientQueueItemVO } from '@/apis/inpatient/doctor-station'
import { useInpatientPatientQueue } from './useInpatientPatientQueue'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  pageInpatientDoctorPatientQueue: vi.fn()
}))

function patient(encounterId: string, patientName: string): InpatientDoctorPatientQueueItemVO {
  return {
    encounterId,
    patientId: `patient-${encounterId}`,
    patientName,
    genderName: '男',
    ageText: '50岁',
    patientNo: `P-${encounterId}`,
    inpatientNo: `ZY-${encounterId}`,
    admissionAt: '2026-07-27 08:00',
    departmentName: '心血管内科',
    wardId: 'ward-1',
    wardName: '心内一病区',
    bedId: `bed-${encounterId}`,
    bedName: `${encounterId}床`,
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    version: 1
  }
}

function page(rows: InpatientDoctorPatientQueueItemVO[]) {
  return {
    pageIndex: 1,
    pageSize: 50,
    total: rows.length,
    pages: 1,
    rows
  }
}

describe('useInpatientPatientQueue', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('首次加载后自动选择第一位患者', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue).mockResolvedValue(
      page([patient('enc-1', '王建国'), patient('enc-2', '赵桂芳')])
    )
    const state = useInpatientPatientQueue()

    await state.loadPatients()

    expect(state.patients.value.map((item) => item.name)).toEqual(['王建国', '赵桂芳'])
    expect(state.selectedPatientId.value).toBe('enc-1')
    expect(state.selectedQueueItem.value?.patientName).toBe('王建国')
  })

  it('刷新队列后保留仍然存在的当前患者', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue)
      .mockResolvedValueOnce(page([patient('enc-1', '王建国'), patient('enc-2', '赵桂芳')]))
      .mockResolvedValueOnce(page([patient('enc-2', '赵桂芳'), patient('enc-3', '陈志强')]))
    const state = useInpatientPatientQueue()
    await state.loadPatients()

    state.selectPatient(state.patients.value[1]!)
    await state.loadPatients()

    expect(state.selectedPatientId.value).toBe('enc-2')
    expect(state.selectedPatient.value?.name).toBe('赵桂芳')
  })

  it('搜索时修剪关键字并忽略较早返回的旧请求', async () => {
    let resolveOld!: (value: ReturnType<typeof page>) => void
    const oldRequest = new Promise<ReturnType<typeof page>>((resolve) => {
      resolveOld = resolve
    })
    vi.mocked(pageInpatientDoctorPatientQueue)
      .mockReturnValueOnce(oldRequest)
      .mockResolvedValueOnce(page([patient('enc-new', '新患者')]))
    const state = useInpatientPatientQueue()

    const oldLoading = state.searchPatients(' 旧患者 ')
    const newLoading = state.searchPatients(' 新患者 ')
    await newLoading
    resolveOld(page([patient('enc-old', '旧患者')]))
    await oldLoading

    expect(state.queueQuery.keyword).toBe('新患者')
    expect(state.patients.value.map((item) => item.name)).toEqual(['新患者'])
    expect(state.selectedPatientId.value).toBe('enc-new')
  })

  it('刷新后当前患者不在队列时自动选择当前结果第一位', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue)
      .mockResolvedValueOnce(page([patient('enc-1', '王建国'), patient('enc-2', '赵桂芳')]))
      .mockResolvedValueOnce(page([patient('enc-2', '赵桂芳'), patient('enc-3', '陈志强')]))
    const state = useInpatientPatientQueue()
    await state.loadPatients()
    state.selectPatient(state.patients.value[0]!)

    await state.loadPatients()

    expect(state.selectedPatientId.value).toBe('enc-2')
    expect(state.selectedQueueItem.value?.patientName).toBe('赵桂芳')
  })
})
