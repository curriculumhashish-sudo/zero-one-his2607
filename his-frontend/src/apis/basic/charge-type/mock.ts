import { createCrudStore } from '@/apis/basic/crud-mock'
import type { ChargeTypeRecord, ChargeTypeSavePayload } from './type'

const categoryText: Record<string, string> = {
  SELF_PAY: '自费',
  MEDICAL_INSURANCE: '医保',
  PUBLIC_PAY: '公费'
}

export const chargeTypeStore = createCrudStore<ChargeTypeRecord>(
  'charge-type',
  [
    seed('charge-001', 'SELF', '普通自费', 10, 'SELF_PAY'),
    seed('charge-002', 'MEDICAL', '基本医疗保险', 20, 'MEDICAL_INSURANCE'),
    seed('charge-003', 'PUBLIC', '公费医疗', 30, 'PUBLIC_PAY')
  ],
  ['name', 'busNo', 'categoryEnumText']
)

function seed(
  id: string,
  busNo: string,
  name: string,
  displayOrder: number,
  categoryEnum: string
): ChargeTypeRecord {
  return {
    id,
    busNo,
    name,
    displayOrder,
    categoryEnum,
    categoryEnumText: categoryText[categoryEnum] ?? '',
    activeFlag: 1,
    activeFlagText: '启用'
  }
}

export function enrichChargeType(data: ChargeTypeSavePayload) {
  return {
    ...data,
    categoryEnumText: categoryText[data.categoryEnum] ?? '',
    activeFlagText: data.activeFlag === 1 ? '启用' : '停用'
  }
}
