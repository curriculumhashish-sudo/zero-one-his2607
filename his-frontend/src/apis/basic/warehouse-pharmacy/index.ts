import type {
  WarehousePharmacyNameOption,
  WarehousePharmacyQuery,
  WarehousePharmacySavePayload
} from './type'
import { warehouseFormText, warehousePharmacyStore } from './mock'

/** 获取库房药房名称。 */
export async function loadWarehousePharmacyNameOptions(
  keyword = ''
): Promise<WarehousePharmacyNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return (await warehousePharmacyStore.list())
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: row.name, value: row.id }))
}

/** 获取库房药房列表。 */
export const pageWarehousePharmacies = (query: WarehousePharmacyQuery) =>
  warehousePharmacyStore.page(query)
/** 添加库房药房。 */
export async function createWarehousePharmacy(data: WarehousePharmacySavePayload) {
  const rows = await warehousePharmacyStore.list()
  return warehousePharmacyStore.create({
    ...data,
    busNo: `${data.formEnum === 'WAREHOUSE' ? 'WH' : 'PH'}${String(rows.length + 1).padStart(3, '0')}`,
    formEnumText: warehouseFormText[data.formEnum],
    statusEnumText: data.statusEnum === 'ACTIVE' ? '启用' : '停用'
  })
}
/** 修改库房药房。 */
export const updateWarehousePharmacy = (id: string, data: WarehousePharmacySavePayload) =>
  warehousePharmacyStore.update(id, {
    ...data,
    formEnumText: warehouseFormText[data.formEnum],
    statusEnumText: data.statusEnum === 'ACTIVE' ? '启用' : '停用'
  })
/** 删除库房药房。 */
export const removeWarehousePharmacy = (id: string) => warehousePharmacyStore.remove(id)
/** 启用或停用库房药房。 */
export const setWarehousePharmacyEnabled = (id: string, enabled: boolean) =>
  warehousePharmacyStore.update(id, {
    statusEnum: enabled ? 'ACTIVE' : 'INACTIVE',
    statusEnumText: enabled ? '启用' : '停用'
  })

export type * from './type'
