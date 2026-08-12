import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()
const post = vi.fn()
const put = vi.fn()
const deleteRequest = vi.fn()
const userStore = { tenantId: '1001' as string | number | null }

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get, post, put, delete: deleteRequest })
}))

vi.mock('@/stores/user', () => ({
  useUserStore: () => userStore
}))

import {
  realCreateInpatientDiagnosis,
  realDeleteInpatientDiagnosis,
  realListInpatientDiagnosisBrief,
  realPageInpatientDiagnosisRecords,
  realUpdateInpatientDiagnosis
} from './diagnosis-real'

describe('inpatient diagnosis real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
    userStore.tenantId = '1001'
  })

  it('loads current encounter diagnoses and normalizes backend fields', async () => {
    get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 1,
        rows: [
          {
            diagnosis_id: 'diag-1',
            condition_id: 'condition-1',
            definition_code: 'I10',
            description: '原发性高血压',
            main_flag: '是',
            rank: 1,
            kind_name: '西医',
            type_name: '入院初步诊断'
          }
        ]
      }
    })

    const result = await realPageInpatientDiagnosisRecords({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'enc-1',
      patientId: 'patient-1'
    })

    expect(get).toHaveBeenCalledWith(
      '/diagnosis-entry/detail',
      {
        tenant_id: '1001',
        'patient-id': 'patient-1',
        pageIndex: 1,
        pageSize: 20
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.rows).toEqual([
      {
        id: 'diag-1',
        conditionId: 'condition-1',
        definitionCode: 'I10',
        description: '原发性高血压',
        kindName: '西医',
        diagnosisTypeCode: '入院初步诊断',
        diagnosisTypeName: '入院初步诊断',
        primaryFlag: 1,
        rank: 1,
        tcmFlag: 0
      }
    ])
  })

  it('loads a simplified diagnosis history list', async () => {
    get.mockResolvedValue({
      data: {
        rows: [
          { definition_name: '原发性高血压' },
          { definition_name: '原发性高血压' },
          { definition_name: '心房颤动' }
        ]
      }
    })

    await expect(realListInpatientDiagnosisBrief()).resolves.toEqual([
      { definitionName: '原发性高血压' },
      { definitionName: '心房颤动' }
    ])
  })

  it('creates a diagnosis with backend snake_case fields and parses both ids', async () => {
    post.mockResolvedValue({
      data: { diagnosisId: 'diag-1', conditionId: 'condition-1' }
    })

    const result = await realCreateInpatientDiagnosis({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      description: '原发性高血压',
      primaryFlag: 1,
      rank: 1,
      tcmFlag: 0
    })

    expect(post).toHaveBeenCalledWith(
      '/diagnosis/add',
      {
        tenant_id: '1001',
        encounter_id: 'enc-1',
        description: '原发性高血压',
        primary_flag: 1,
        set_always: 0,
        rank: 1,
        tcm_flag: 0
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.id).toBe('diag-1')
    expect(result.conditionId).toBe('condition-1')
  })

  it('updates the diagnosis type code', async () => {
    put.mockResolvedValue({ data: 'diag-1' })

    await realUpdateInpatientDiagnosis({ id: 'diag-1', diagnosisTypeCode: '入院初步诊断' })

    expect(put).toHaveBeenCalledWith(
      '/diagnosis/modify',
      { id: 'diag-1', diag_type_code: '入院初步诊断' },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })

  it('deletes with diagnosis and condition ids', async () => {
    deleteRequest.mockResolvedValue({ data: true })

    await realDeleteInpatientDiagnosis({ id: 'diag-1', conditionId: 'condition-1' })

    expect(deleteRequest).toHaveBeenCalledWith(
      '/diagnosis/diag-1',
      undefined,
      expect.objectContaining({ baseURL: expect.any(String) })
    )
  })
})
