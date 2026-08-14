import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export type WarehouseForm = 'WAREHOUSE' | 'PHARMACY'

export interface WarehousePharmacyRecord {
  id: string
  busNo: string
  name: string
  formEnum: WarehouseForm
  formEnumText: string
  statusEnum: 'ACTIVE' | 'INACTIVE'
  statusEnumText: string
  displayOrder: number
  organizationId?: string
  parentId?: string
  pyStr?: string
  wbStr?: string
  createdAt?: string
  updatedAt?: string
}

export interface WarehousePharmacyQuery extends BasicPageQuery {}
export type WarehousePharmacySavePayload = Omit<
  WarehousePharmacyRecord,
  'id' | 'busNo' | 'formEnumText' | 'statusEnumText'
>
export interface WarehousePharmacyNameOption {
  label: string
  value: string
}
