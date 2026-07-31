import type { MedicationLocationRuleQuery, MedicationLocationRuleSavePayload } from './type'
import { mockPageMedicationLocationRules, mockSaveMedicationLocationRules } from './mock'

/** 获取科室项目列表（条件 + 分页）。 */
export const pageMedicationLocationRules = (query: MedicationLocationRuleQuery) =>
  mockPageMedicationLocationRules(query)
/** 保存科室项目。该接口以当前页面的完整规则集为保存单位。 */
export const saveMedicationLocationRules = (data: MedicationLocationRuleSavePayload) =>
  mockSaveMedicationLocationRules(data)

export type * from './type'
