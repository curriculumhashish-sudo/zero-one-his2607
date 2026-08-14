import type { NurseStationSelectionPolicy } from './type'

/**
 * 收敛患者选择集：清理已离开当前队列的患者，并遵守当前页签的单/多选策略。
 */
export function normalizeSelectedPatientIds(
  selectedIds: readonly string[],
  availableIds: readonly string[],
  policy: NurseStationSelectionPolicy
) {
  const availableSet = new Set(availableIds)
  const normalized = [...new Set(selectedIds)].filter((id) => availableSet.has(id))
  return policy === 'single' ? normalized.slice(0, 1) : normalized
}
