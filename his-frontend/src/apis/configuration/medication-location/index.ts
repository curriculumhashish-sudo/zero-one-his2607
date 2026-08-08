import { toC7PageResult, unwrap } from '@/apis/basic/remote'
import { useHttp } from '@/plugins/http'
import type { MedicationLocationRuleQuery, MedicationLocationRuleSavePayload } from './type'

const baseUrl = '/c7-configuration-api/pharmacy'

export async function pageMedicationLocationRules(query: MedicationLocationRuleQuery) {
  const page = unwrap(await useHttp().get(`${baseUrl}/query-all-pharmacy-item`, {
    organizationId: query.organizationId,
    pageNo: query.pageIndex,
    pageSize: query.pageSize
  }))
  return toC7PageResult(page, query.pageIndex, query.pageSize)
}

export async function saveMedicationLocationRules(data: MedicationLocationRuleSavePayload) {
  return unwrap(await useHttp().post<boolean>(`${baseUrl}/save-pharmacy-item`, data))
}

export type * from './type'
