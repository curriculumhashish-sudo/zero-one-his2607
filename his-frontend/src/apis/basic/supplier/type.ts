import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface SupplierRecord {
  id: string
  busNo: string
  name: string
  typeEnum: string
  typeEnumText: string
  organizationId: string
  organizationName: string
  address: string
  phone: string
  email: string
  pyStr: string
  wbStr: string
  statusEnum: 'ACTIVE' | 'INACTIVE'
  statusEnumText: string
}

export interface SupplierQuery extends BasicPageQuery {}
export type SupplierSavePayload = Omit<
  SupplierRecord,
  'id' | 'typeEnumText' | 'organizationName' | 'statusEnumText'
>
export interface SupplierNameOption {
  label: string
  value: string
}
