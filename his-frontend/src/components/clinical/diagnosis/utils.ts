import type { ClinicalDiagnosisCandidate, ClinicalDiagnosisItem } from './type'

/** 把诊断字典候选项转换成一条尚未保存的诊断。 */
function candidateToDiagnosis(candidate: ClinicalDiagnosisCandidate): ClinicalDiagnosisItem {
  return {
    key: 'draft-' + candidate.id,
    catalogId: candidate.id,
    code: candidate.code,
    name: candidate.name,
    typeName: candidate.typeName,
    primary: false
  }
}

/**
 * 添加诊断并自动去重。
 * 如果此前没有诊断，第一条新诊断自动成为主诊断。
 */
export function addClinicalDiagnoses(
  current: ClinicalDiagnosisItem[],
  candidates: ClinicalDiagnosisCandidate[]
): ClinicalDiagnosisItem[] {
  const existingCatalogIds = new Set(current.map((item) => item.catalogId))
  const additions = candidates
    .filter((candidate) => {
      if (existingCatalogIds.has(candidate.id)) return false
      existingCatalogIds.add(candidate.id)
      return true
    })
    .map(candidateToDiagnosis)

  if (current.length === 0 && additions[0]) additions[0].primary = true
  return [...current.map((item) => ({ ...item })), ...additions]
}

/** 删除诊断；若删除的是主诊断，自动把剩余第一条设为主诊断。 */
export function removeClinicalDiagnosis(
  current: ClinicalDiagnosisItem[],
  key: ClinicalDiagnosisItem['key']
): ClinicalDiagnosisItem[] {
  const removed = current.find((item) => item.key === key)
  const result = current.filter((item) => item.key !== key).map((item) => ({ ...item }))

  if (removed?.primary && result[0]) result[0].primary = true
  return result
}

/** 设置唯一主诊断。 */
export function setPrimaryClinicalDiagnosis(
  current: ClinicalDiagnosisItem[],
  key: ClinicalDiagnosisItem['key']
): ClinicalDiagnosisItem[] {
  return current.map((item) => ({ ...item, primary: item.key === key }))
}

/** 上移或下移诊断；超出首尾时保持原顺序。 */
export function moveClinicalDiagnosis(
  current: ClinicalDiagnosisItem[],
  key: ClinicalDiagnosisItem['key'],
  direction: -1 | 1
): ClinicalDiagnosisItem[] {
  const result = current.map((item) => ({ ...item }))
  const currentIndex = result.findIndex((item) => item.key === key)
  const targetIndex = currentIndex + direction

  if (currentIndex < 0 || targetIndex < 0 || targetIndex >= result.length) return result
  ;[result[currentIndex], result[targetIndex]] = [result[targetIndex]!, result[currentIndex]!]
  return result
}

/** 保存前校验：至少有一条诊断，并且恰好有一个主诊断。 */
export function hasValidPrimaryDiagnosis(items: ClinicalDiagnosisItem[]) {
  return items.length > 0 && items.filter((item) => item.primary).length === 1
}
