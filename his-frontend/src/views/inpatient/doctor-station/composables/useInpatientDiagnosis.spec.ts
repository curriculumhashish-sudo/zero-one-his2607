import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  createInpatientDiagnosis,
  deleteInpatientDiagnosis,
  getInpatientDiagnosisUsageHistory,
  pageInpatientDiagnosisDictionary,
  pageInpatientDiagnosisRecords,
  updateInpatientDiagnosis,
  type InpatientDiagnosisRecordVO,
  type InpatientDiagnosisUsageHistoryVO
} from '@/apis/inpatient/doctor-station'
import { useInpatientDiagnosis } from './useInpatientDiagnosis'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  createInpatientDiagnosis: vi.fn(),
  deleteInpatientDiagnosis: vi.fn(),
  getInpatientDiagnosisUsageHistory: vi.fn(),
  pageInpatientDiagnosisDictionary: vi.fn(),
  pageInpatientDiagnosisRecords: vi.fn(),
  updateInpatientDiagnosis: vi.fn()
}))

function record(
  id: string,
  catalogId: string,
  code: string,
  primary: boolean,
  orderNo: number
): InpatientDiagnosisRecordVO {
  return {
    id,
    encounterId: 'enc-1',
    patientId: 'patient-1',
    catalogId,
    code,
    name: `诊断-${code}`,
    typeName: '西医诊断',
    primary,
    orderNo,
    diagnosedAt: '2026-07-27 09:00:00',
    updatedAt: '2026-07-27 09:00:00',
    version: 1
  }
}

function page(rows: InpatientDiagnosisRecordVO[]) {
  return {
    pageIndex: 1,
    pageSize: 100,
    total: rows.length,
    pages: 1,
    rows
  }
}

const usageHistory: InpatientDiagnosisUsageHistoryVO = {
  id: 'catalog-3',
  code: 'R07.4',
  name: '胸痛',
  typeName: '症状诊断',
  useCount: 12,
  lastUsedAt: '2026-07-27 09:00:00'
}

describe('useInpatientDiagnosis', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    vi.mocked(getInpatientDiagnosisUsageHistory).mockResolvedValue([usageHistory])
  })

  it('加载住院诊断后转换为公共模型并建立未修改快照', async () => {
    vi.mocked(pageInpatientDiagnosisRecords).mockResolvedValue(
      page([
        record('record-2', 'catalog-2', 'E78.5', false, 2),
        record('record-1', 'catalog-1', 'I10', true, 1)
      ])
    )
    const state = useInpatientDiagnosis()

    await state.loadDiagnoses('enc-1')

    expect(state.diagnoses.value.map((item) => item.code)).toEqual(['I10', 'E78.5'])
    expect(state.diagnosesDirty.value).toBe(false)
    expect(state.diagnosisUpdatedAt.value).toBe('2026-07-27 09:00:00')
  })

  it('保存时将列表差异转换为删除、修改和新增接口', async () => {
    const first = record('record-1', 'catalog-1', 'I10', true, 1)
    const second = record('record-2', 'catalog-2', 'E78.5', false, 2)
    const savedSecond = { ...second, primary: true, orderNo: 1, version: 2 }
    const createdThird = record('record-3', usageHistory.id, usageHistory.code, false, 2)

    vi.mocked(pageInpatientDiagnosisRecords)
      .mockResolvedValueOnce(page([first, second]))
      .mockResolvedValueOnce(page([savedSecond, createdThird]))
    vi.mocked(deleteInpatientDiagnosis).mockResolvedValue()
    vi.mocked(updateInpatientDiagnosis).mockResolvedValue(savedSecond)
    vi.mocked(createInpatientDiagnosis).mockResolvedValue(createdThird)
    const state = useInpatientDiagnosis()
    await state.loadDiagnoses('enc-1')

    state.diagnoses.value = [
      { ...state.diagnoses.value[1]!, primary: true },
      {
        key: `draft-${usageHistory.id}`,
        catalogId: usageHistory.id,
        code: usageHistory.code,
        name: usageHistory.name,
        typeName: usageHistory.typeName,
        primary: false
      }
    ]

    const changedCount = await state.saveDiagnoses('enc-1', 'patient-1')

    expect(changedCount).toBe(3)
    expect(deleteInpatientDiagnosis).toHaveBeenCalledWith(first.id, first.version)
    expect(updateInpatientDiagnosis).toHaveBeenCalledWith(
      second.id,
      expect.objectContaining({ primary: true, orderNo: 1, version: second.version })
    )
    expect(createInpatientDiagnosis).toHaveBeenCalledWith(
      expect.objectContaining({
        encounterId: 'enc-1',
        patientId: 'patient-1',
        catalogId: usageHistory.id,
        orderNo: 2
      })
    )
    expect(state.diagnosesDirty.value).toBe(false)
  })

  it('可从诊断使用历史快速添加且不会重复', async () => {
    vi.mocked(pageInpatientDiagnosisRecords).mockResolvedValue(
      page([record('record-1', 'catalog-1', 'I10', true, 1)])
    )
    const state = useInpatientDiagnosis()
    await state.loadDiagnoses('enc-1')

    state.addDiagnosisFromHistory(usageHistory)
    state.addDiagnosisFromHistory(usageHistory)

    expect(state.diagnoses.value.map((item) => item.catalogId)).toEqual([
      'catalog-1',
      usageHistory.id
    ])
    expect(state.diagnosesDirty.value).toBe(true)
  })

  it('诊断字典加载器会适配公共选择弹窗模型', async () => {
    vi.mocked(pageInpatientDiagnosisDictionary).mockResolvedValue({
      pageIndex: 1,
      pageSize: 10,
      total: 1,
      pages: 1,
      rows: [
        {
          id: 'catalog-1',
          code: 'I10',
          name: '原发性高血压',
          typeName: '西医诊断'
        }
      ]
    })
    const state = useInpatientDiagnosis()
    const result = await state.loadDiagnosisDictionary(
      { pageIndex: 1, pageSize: 10, keyword: '高血压' },
      { signal: new AbortController().signal }
    )

    expect(result.rows).toEqual([
      {
        id: 'catalog-1',
        code: 'I10',
        name: '原发性高血压',
        typeName: '西医诊断'
      }
    ])
  })
})
