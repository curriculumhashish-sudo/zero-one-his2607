import { createCrudStore } from '@/apis/basic/crud-mock'
import type { SupplierRecord, SupplierSavePayload } from './type'

const typeText: Record<string, string> = { DRUG: '药品', DEVICE: '器械', GENERAL: '综合' }
const organizationText: Record<string, string> = { 'org-001': '药剂科', 'org-002': '设备科' }

export const supplierStore = createCrudStore<SupplierRecord>(
  'supplier',
  [
    seed('supplier-001', 'SUP001', '华康医药有限公司', 'DRUG', 'org-001', '010-88886666'),
    seed('supplier-002', 'SUP002', '康泰医疗器械有限公司', 'DEVICE', 'org-002', '021-66668888')
  ],
  ['name', 'busNo', 'phone', 'email', 'typeEnumText']
)

function seed(
  id: string,
  busNo: string,
  name: string,
  typeEnum: string,
  organizationId: string,
  phone: string
): SupplierRecord {
  return {
    id,
    busNo,
    name,
    typeEnum,
    typeEnumText: typeText[typeEnum] ?? '',
    creditCode: '',
    organizationId,
    organizationName: organizationText[organizationId] ?? '',
    address: '示例产业园区',
    phone,
    email: 'service@example.com',
    pyStr: '',
    wbStr: '',
    statusEnum: 'ACTIVE',
    statusEnumText: '启用'
  }
}

export function enrichSupplier(data: SupplierSavePayload) {
  return {
    ...data,
    typeEnumText: typeText[data.typeEnum] ?? '',
    organizationName: organizationText[data.organizationId] ?? '',
    statusEnumText: data.statusEnum === 'ACTIVE' ? '启用' : '停用'
  }
}
