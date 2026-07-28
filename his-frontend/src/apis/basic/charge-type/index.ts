import type { ChargeTypeNameOption, ChargeTypeQuery, ChargeTypeSavePayload } from './type'
import { chargeTypeStore, enrichChargeType } from './mock'

/** 获取费用性质名称。 */
export async function loadChargeTypeNameOptions(keyword = ''): Promise<ChargeTypeNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return (await chargeTypeStore.list())
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: row.name, value: row.id }))
}
/** 获取费用性质列表。 */
export const pageChargeTypes = (query: ChargeTypeQuery) => chargeTypeStore.page(query)
/** 添加费用性质。 */
export const createChargeType = (data: ChargeTypeSavePayload) =>
  chargeTypeStore.create(enrichChargeType(data))
/** 修改费用性质。 */
export const updateChargeType = (id: string, data: ChargeTypeSavePayload) =>
  chargeTypeStore.update(id, enrichChargeType(data))
/** 删除费用性质。 */
export const removeChargeType = (id: string) => chargeTypeStore.remove(id)
/** 启用或停用费用性质。 */
export const setChargeTypeEnabled = (id: string, enabled: boolean) =>
  chargeTypeStore.update(id, {
    activeFlag: enabled ? 1 : 0,
    activeFlagText: enabled ? '启用' : '停用'
  })

export type * from './type'
