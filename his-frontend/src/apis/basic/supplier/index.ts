import type { SupplierNameOption, SupplierQuery, SupplierSavePayload } from './type'
import { enrichSupplier, supplierStore } from './mock'

/** 获取供应商名称。 */
export async function loadSupplierNameOptions(keyword = ''): Promise<SupplierNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return (await supplierStore.list())
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: row.name, value: row.id }))
}
/** 获取供应商列表。 */
export const pageSuppliers = (query: SupplierQuery) => supplierStore.page(query)
/** 获取供应商详情。 */
export const getSupplierDetail = (id: string) => supplierStore.detail(id)
/** 新增供应商。 */
export const createSupplier = (data: SupplierSavePayload) =>
  supplierStore.create(enrichSupplier(data))
/** 修改供应商。 */
export const updateSupplier = (id: string, data: SupplierSavePayload) =>
  supplierStore.update(id, enrichSupplier(data))
/** 删除供应商。 */
export const removeSupplier = (id: string) => supplierStore.remove(id)
/** 启用或停用供应商，支持单条或批量标识。 */
export async function setSuppliersEnabled(ids: string[], enabled: boolean) {
  const changed = []
  for (const id of [...new Set(ids)]) {
    changed.push(
      await supplierStore.update(id, {
        statusEnum: enabled ? 'ACTIVE' : 'INACTIVE',
        statusEnumText: enabled ? '启用' : '停用'
      })
    )
  }
  return changed
}

export type * from './type'
