import type {
  ClinicalOrderCandidate,
  ClinicalOrderContext,
  ClinicalOrderDraft,
  ClinicalOrderValidationResult
} from './type'

/**
 * 医嘱场景适配器。
 *
 * 公共组件只依赖这个接口，不判断当前是门诊还是住院。
 * 具体适配器可以返回自己的保存 DTO，公共接口使用 unknown 保持场景无关。
 */
export interface AdviceScenarioAdapter {
  /** 适配器名称，便于调试 */
  scope: 'outpatient' | 'inpatient'
  /** 将目录候选项转换成可编辑草稿 */
  createDraft(candidate: ClinicalOrderCandidate, context: ClinicalOrderContext): ClinicalOrderDraft
  /** 执行当前场景的保存前校验 */
  validate(
    orders: ClinicalOrderDraft[],
    context: ClinicalOrderContext
  ): ClinicalOrderValidationResult
  /** 将公共草稿转换为当前场景保存请求 */
  mapSaveRequest(
    orders: ClinicalOrderDraft[],
    context: ClinicalOrderContext,
    version: number
  ): unknown
}
