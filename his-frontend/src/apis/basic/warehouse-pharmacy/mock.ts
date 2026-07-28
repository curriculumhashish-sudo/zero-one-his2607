import { createCrudStore } from '@/apis/basic/crud-mock'
import type { WarehouseForm, WarehousePharmacyRecord } from './type'

const formText: Record<WarehouseForm, string> = { WAREHOUSE: '库房', PHARMACY: '药房' }

export const warehousePharmacyStore = createCrudStore<WarehousePharmacyRecord>(
  'warehouse',
  [
    seed('warehouse-001', 'WH001', '中心药库', 'WAREHOUSE', 10),
    seed('warehouse-002', 'PH001', '门诊药房', 'PHARMACY', 20),
    seed('warehouse-003', 'PH002', '住院药房', 'PHARMACY', 30)
  ],
  ['name', 'busNo', 'formEnumText']
)

function seed(
  id: string,
  busNo: string,
  name: string,
  formEnum: WarehouseForm,
  displayOrder: number
): WarehousePharmacyRecord {
  return {
    id,
    busNo,
    name,
    formEnum,
    formEnumText: formText[formEnum],
    statusEnum: 'ACTIVE',
    statusEnumText: '启用',
    displayOrder
  }
}

export { formText as warehouseFormText }
