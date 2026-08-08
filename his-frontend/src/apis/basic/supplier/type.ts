import type { BasicPageQuery } from '@/apis/basic/crud-mock'

export interface SupplierRecord {
  id: string
  /** 后端乐观锁版本；C5 查询响应当前未返回。 */
  version?: number
  code?: string
  legalRepresentative?: string
  bankName?: string
  bankAccount?: string
  displayOrder?: number
  note?: string
  busNo: string
  name: string
  typeEnum: string
  typeEnumText: string
  creditCode: string
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
