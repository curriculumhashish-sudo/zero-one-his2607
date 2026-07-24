import { describe, expect, it } from 'vitest'

import type { ClinicalDiagnosisCandidate, ClinicalDiagnosisItem } from './type'
import {
  addClinicalDiagnoses,
  hasValidPrimaryDiagnosis,
  moveClinicalDiagnosis,
  removeClinicalDiagnosis,
  setPrimaryClinicalDiagnosis
} from './utils'

const candidates: ClinicalDiagnosisCandidate[] = [
  { id: 'd1', code: 'I10', name: '原发性高血压', typeName: '西医诊断' },
  { id: 'd2', code: 'R00.2', name: '心悸', typeName: '症状诊断' }
]

describe('clinical diagnosis utils', () => {
  it('添加诊断时去重，并把第一条自动设为主诊断', () => {
    const result = addClinicalDiagnoses([], [candidates[0]!, candidates[0]!, candidates[1]!])

    expect(result).toHaveLength(2)
    expect(result[0]?.primary).toBe(true)
    expect(result[1]?.primary).toBe(false)
  })

  it('切换主诊断时始终只保留一个主诊断', () => {
    const source = addClinicalDiagnoses([], candidates)
    const result = setPrimaryClinicalDiagnosis(source, 'draft-d2')

    expect(result.find((item) => item.catalogId === 'd1')?.primary).toBe(false)
    expect(result.find((item) => item.catalogId === 'd2')?.primary).toBe(true)
    expect(hasValidPrimaryDiagnosis(result)).toBe(true)
  })

  it('删除主诊断后自动补位，并支持调整顺序', () => {
    const source = addClinicalDiagnoses([], candidates)
    const moved = moveClinicalDiagnosis(source, 'draft-d2', -1)
    const result = removeClinicalDiagnosis(moved, 'draft-d1')

    expect(moved[0]?.catalogId).toBe('d2')
    expect(result).toHaveLength(1)
    expect(result[0]?.catalogId).toBe('d2')
    expect(result[0]?.primary).toBe(true)
  })

  it('空列表和没有唯一主诊断的列表不能保存', () => {
    const invalidRows: ClinicalDiagnosisItem[] = candidates.map((candidate) => ({
      key: candidate.id,
      catalogId: candidate.id,
      code: candidate.code,
      name: candidate.name,
      typeName: candidate.typeName,
      primary: false
    }))

    expect(hasValidPrimaryDiagnosis([])).toBe(false)
    expect(hasValidPrimaryDiagnosis(invalidRows)).toBe(false)
  })
})
