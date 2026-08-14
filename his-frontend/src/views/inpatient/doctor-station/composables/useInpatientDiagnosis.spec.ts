import { beforeEach, describe, expect, it, vi } from 'vitest'

import {
  createInpatientDiagnosis,
  deleteInpatientDiagnosis,
  listInpatientDiagnosisBrief,
  pageInpatientDiagnosisRecords,
  updateInpatientDiagnosis,
  type InpatientDiagnosisRecordVO
} from '@/apis/inpatient/doctor-station'
import { useInpatientDiagnosis } from './useInpatientDiagnosis'

vi.mock('@/apis/inpatient/doctor-station', () => ({
  createInpatientDiagnosis: vi.fn(),
  deleteInpatientDiagnosis: vi.fn(),
  listInpatientDiagnosisBrief: vi.fn(),
  pageInpatientDiagnosisRecords: vi.fn(),
  updateInpatientDiagnosis: vi.fn()
}))

function record(
  id: string,
  description: string,
  primaryFlag: number,
  rank: number
): InpatientDiagnosisRecordVO {
  return {
    id,
    conditionId: `condition-${id}`,
    definitionCode: id.toUpperCase(),
    description,
    kindName: '西医',
    diagnosisTypeCode: '入院初步诊断',
    diagnosisTypeName: '入院初步诊断',
    primaryFlag,
    rank,
    tcmFlag: 0
  }
}

function page(rows: InpatientDiagnosisRecordVO[]) {
  return {
    pageIndex: 1,
    pageSize: 100,
    total: rows.length,
    pages: rows.length > 0 ? 1 : 0,
    rows
  }
}

describe('useInpatientDiagnosis', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    vi.mocked(listInpatientDiagnosisBrief).mockResolvedValue([])
  })

  it('loads current encounter diagnoses and creates a clean snapshot', async () => {
    vi.mocked(pageInpatientDiagnosisRecords).mockResolvedValue(
      page([record('diag-2', '心房颤动', 0, 2), record('diag-1', '原发性高血压', 1, 1)])
    )
    const state = useInpatientDiagnosis()

    await state.loadDiagnoses('enc-1', 'patient-1')

    expect(pageInpatientDiagnosisRecords).toHaveBeenCalledWith({
      pageIndex: 1,
      pageSize: 100,
      encounterId: 'enc-1',
      patientId: 'patient-1'
    })
    expect(state.diagnoses.value.map((item) => item.description)).toEqual([
      '原发性高血压',
      '心房颤动'
    ])
    expect(state.diagnosesDirty.value).toBe(false)
  })

  it('adds a simplified history diagnosis without duplicates', async () => {
    vi.mocked(pageInpatientDiagnosisRecords).mockResolvedValue(page([]))
    vi.mocked(listInpatientDiagnosisBrief).mockResolvedValue([{ definitionName: '原发性高血压' }])
    const state = useInpatientDiagnosis()
    await state.loadDiagnoses('enc-1', 'patient-1')
    await state.loadDiagnosisBriefs()

    state.addDiagnosisFromBrief(state.diagnosisBriefs.value[0]!)
    state.addDiagnosisFromBrief(state.diagnosisBriefs.value[0]!)

    expect(state.diagnoses.value).toHaveLength(1)
    expect(state.diagnoses.value[0]).toEqual(
      expect.objectContaining({ description: '原发性高血压', primaryFlag: 1, rank: 1 })
    )
  })

  it('persists deletes, updates and creates with the minimal contract', async () => {
    const first = record('diag-1', '原发性高血压', 1, 1)
    const second = record('diag-2', '心房颤动', 0, 2)
    vi.mocked(pageInpatientDiagnosisRecords)
      .mockResolvedValueOnce(page([first, second]))
      .mockResolvedValueOnce(page([record('diag-2', '心房颤动', 1, 1)]))
    vi.mocked(deleteInpatientDiagnosis).mockResolvedValue()
    vi.mocked(updateInpatientDiagnosis).mockResolvedValue()
    vi.mocked(createInpatientDiagnosis).mockResolvedValue(record('diag-3', '慢性心力衰竭', 0, 2))
    const state = useInpatientDiagnosis()
    await state.loadDiagnoses('enc-1', 'patient-1')

    state.diagnoses.value = [
      {
        ...state.diagnoses.value[1]!,
        diagnosisTypeCode: '出院诊断',
        diagnosisTypeName: '出院诊断',
        primaryFlag: 1
      },
      {
        key: 'draft-1',
        definitionCode: 'I50.9',
        description: '慢性心力衰竭',
        kindName: '西医',
        diagnosisTypeCode: '入院初步诊断',
        diagnosisTypeName: '入院初步诊断',
        primaryFlag: 0,
        rank: 2,
        tcmFlag: 0
      }
    ]

    const changedCount = await state.saveDiagnoses('enc-1', 'patient-1')

    expect(changedCount).toBe(3)
    expect(deleteInpatientDiagnosis).toHaveBeenCalledWith({
      id: 'diag-1',
      conditionId: 'condition-diag-1'
    })
    expect(updateInpatientDiagnosis).toHaveBeenCalledWith({
      id: 'diag-2',
      diagnosisTypeCode: '出院诊断'
    })
    expect(createInpatientDiagnosis).toHaveBeenCalledWith({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      definitionCode: 'I50.9',
      description: '慢性心力衰竭',
      diagnosisTypeCode: '入院初步诊断',
      primaryFlag: 0,
      rank: 2,
      tcmFlag: 0
    })
  })

  it('rejects multiple primary diagnoses before saving', async () => {
    vi.mocked(pageInpatientDiagnosisRecords).mockResolvedValue(page([]))
    const state = useInpatientDiagnosis()
    await state.loadDiagnoses('enc-1', 'patient-1')
    state.diagnoses.value = [
      {
        key: 'draft-1',
        definitionCode: 'I10',
        description: '原发性高血压',
        kindName: '西医',
        diagnosisTypeCode: '入院初步诊断',
        diagnosisTypeName: '入院初步诊断',
        primaryFlag: 1,
        rank: 1,
        tcmFlag: 0
      },
      {
        key: 'draft-2',
        definitionCode: 'I48.9',
        description: '心房颤动',
        kindName: '西医',
        diagnosisTypeCode: '入院初步诊断',
        diagnosisTypeName: '入院初步诊断',
        primaryFlag: 1,
        rank: 2,
        tcmFlag: 0
      }
    ]

    await expect(state.saveDiagnoses('enc-1', 'patient-1')).rejects.toThrow('只能存在一个主诊断')
  })
})
