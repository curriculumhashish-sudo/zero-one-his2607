import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { ElMessage } from 'element-plus'

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
    inpatientNo: `ZY-${encounterId}`,
    admissionAt: '2026-07-27 08:00',
    departmentName: '心血管内科',
    wardId: 'ward-1',
    wardName: '心内一病区',
    bedId: `bed-${encounterId}`,
    bedName: `${encounterId}床`,
    statusEnum: 'IN_PROGRESS'
  }
}

function page(
  rows: InpatientDoctorPatientQueueItemVO[],
  overrides: Partial<{
    pageIndex: number
    pageSize: number
    total: number
    pages: number
  }> = {}
) {
  return {
    pageIndex: 1,
    pageSize: 50,
    total: rows.length,
    pages: 1,
    rows,
    ...overrides
  }
}

describe('useInpatientPatientQueue', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  afterEach(() => {
    vi.restoreAllMocks()
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

  it('切换页码并按新页码重新加载队列', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue).mockResolvedValue(
      page([patient('enc-51', '第五十一位患者')], {
        pageIndex: 2,
        total: 51,
        pages: 2
      })
    )
    const state = useInpatientPatientQueue()

    const status = await state.changePatientPage(2)

    expect(status).toBe('success')
    expect(pageInpatientDoctorPatientQueue).toHaveBeenCalledWith(
      expect.objectContaining({ pageIndex: 2, pageSize: 50 })
    )
    expect(state.queueQuery.pageIndex).toBe(2)
  })

  it('当前页因数据减少越界时自动回退到最后一页', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue)
      .mockResolvedValueOnce(page([], { pageIndex: 2, total: 50, pages: 1 }))
      .mockResolvedValueOnce(page([patient('enc-1', '王建国')], { total: 50, pages: 1 }))
    const state = useInpatientPatientQueue()
    state.queueQuery.pageIndex = 2

    const status = await state.loadPatients()

    expect(status).toBe('success')
    expect(state.queueQuery.pageIndex).toBe(1)
    expect(pageInpatientDoctorPatientQueue).toHaveBeenNthCalledWith(
      2,
      expect.objectContaining({ pageIndex: 1 })
    )
  })

  it('队列清空后将当前页复位到第一页', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue).mockResolvedValue(
      page([], { pageIndex: 2, total: 0, pages: 0 })
    )
    const state = useInpatientPatientQueue()
    state.queueQuery.pageIndex = 2

    const status = await state.loadPatients()

    expect(status).toBe('success')
    expect(state.queueQuery.pageIndex).toBe(1)
  })

  it('出院后刷新未找到原就诊时返回已移除', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue).mockResolvedValue(page([patient('enc-2', '赵桂芳')]))
    const state = useInpatientPatientQueue()

    await expect(state.reconcileDischarge('enc-1')).resolves.toBe('removed')
  })

  it('出院后刷新仍找到原就诊时返回仍在队列', async () => {
    vi.mocked(pageInpatientDoctorPatientQueue).mockResolvedValue(page([patient('enc-1', '王建国')]))
    const state = useInpatientPatientQueue()

    await expect(state.reconcileDischarge('enc-1')).resolves.toBe('still-present')
  })

  it('出院后静默刷新失败时返回刷新失败且不重复弹出错误', async () => {
    const errorSpy = vi.spyOn(ElMessage, 'error').mockImplementation(() => undefined as never)
    vi.mocked(pageInpatientDoctorPatientQueue).mockRejectedValue(new Error('网络错误'))
    const state = useInpatientPatientQueue()

    await expect(state.reconcileDischarge('enc-1')).resolves.toBe('refresh-failed')
    expect(errorSpy).not.toHaveBeenCalled()
  })
})
