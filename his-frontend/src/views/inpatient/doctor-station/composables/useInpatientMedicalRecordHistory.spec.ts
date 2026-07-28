import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  getInpatientMedicalRecordHistoryDetail,
  pageInpatientMedicalRecordHistory,
  type InpatientMedicalRecordHistoryDetailVO,
  type InpatientMedicalRecordHistoryItemVO
} from '@/apis/inpatient/doctor-station'
import { useInpatientMedicalRecordHistory } from './useInpatientMedicalRecordHistory'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  getInpatientMedicalRecordHistoryDetail: vi.fn(),
  pageInpatientMedicalRecordHistory: vi.fn()
}))

function historyItem(id: string, encounterId: string): InpatientMedicalRecordHistoryItemVO {
  return {
    id,
    patientId: 'patient-1',
    encounterId,
    inpatientNo: `ZY-${encounterId}`,
    documentTypeCode: 'ADMISSION_RECORD',
    documentTypeName: '入院记录',
    title: `${id}入院记录`,
    status: 'SIGNED',
    statusName: '已签名',
    departmentName: '心血管内科',
    authoredByName: '李医生',
    authoredAt: '2026-07-27 09:00:00',
    updatedAt: '2026-07-27 09:30:00'
  }
}

function historyDetail(item: InpatientMedicalRecordHistoryItemVO) {
  return {
    ...item,
    content: `<p>${item.title}</p>`,
    version: 1,
    readonly: true
  } satisfies InpatientMedicalRecordHistoryDetailVO
}

function page(rows: InpatientMedicalRecordHistoryItemVO[]) {
  return {
    pageIndex: 1,
    pageSize: 10,
    total: rows.length,
    pages: 1,
    rows
  }
}

describe('useInpatientMedicalRecordHistory', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('加载历史列表后默认选择第一份文书并获取详情', async () => {
    const first = historyItem('record-1', 'enc-current')
    const second = historyItem('record-2', 'enc-old')
    vi.mocked(pageInpatientMedicalRecordHistory).mockResolvedValue(page([first, second]))
    vi.mocked(getInpatientMedicalRecordHistoryDetail).mockResolvedValue(historyDetail(first))
    const state = useInpatientMedicalRecordHistory()

    await state.loadHistory('patient-1', 'enc-current')

    expect(pageInpatientMedicalRecordHistory).toHaveBeenCalledWith(
      expect.objectContaining({
        patientId: 'patient-1',
        currentEncounterId: 'enc-current'
      })
    )
    expect(state.selectedRecordId.value).toBe('record-1')
    expect(state.selectedRecordDetail.value?.content).toContain('record-1')
  })

  it('搜索历史时修剪关键字并回到第一页', async () => {
    const record = historyItem('record-1', 'enc-current')
    vi.mocked(pageInpatientMedicalRecordHistory).mockResolvedValue(page([record]))
    vi.mocked(getInpatientMedicalRecordHistoryDetail).mockResolvedValue(historyDetail(record))
    const state = useInpatientMedicalRecordHistory()
    await state.loadHistory('patient-1', 'enc-current')
    state.historyQuery.pageIndex = 3

    await state.searchHistory(' 首次病程 ')

    expect(state.historyQuery.keyword).toBe('首次病程')
    expect(state.historyQuery.pageIndex).toBe(1)
  })

  it('快速选择文书时忽略较早返回的详情', async () => {
    const first = historyItem('record-1', 'enc-current')
    const second = historyItem('record-2', 'enc-old')
    vi.mocked(pageInpatientMedicalRecordHistory).mockResolvedValue(page([first, second]))
    vi.mocked(getInpatientMedicalRecordHistoryDetail).mockResolvedValueOnce(historyDetail(first))
    const state = useInpatientMedicalRecordHistory()
    await state.loadHistory('patient-1', 'enc-current')

    let resolveOld!: (detail: InpatientMedicalRecordHistoryDetailVO) => void
    const oldRequest = new Promise<InpatientMedicalRecordHistoryDetailVO>((resolve) => {
      resolveOld = resolve
    })
    vi.mocked(getInpatientMedicalRecordHistoryDetail)
      .mockReturnValueOnce(oldRequest)
      .mockResolvedValueOnce(historyDetail(second))

    const oldLoading = state.loadHistoryDetail(first.id)
    const newLoading = state.selectHistoryRecord(second)
    await newLoading
    resolveOld(historyDetail(first))
    await oldLoading

    expect(state.selectedRecordId.value).toBe(second.id)
    expect(state.selectedRecordDetail.value?.id).toBe(second.id)
  })
})
