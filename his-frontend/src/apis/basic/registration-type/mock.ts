import { createCrudStore } from '@/apis/basic/crud-mock'
import type { RegistrationTypeRecord } from './type'

export const registrationTypeStore = createCrudStore<RegistrationTypeRecord>(
  'registration-type',
  [
    seed('registration-001', 'REG001', '普通门诊', '11', 10),
    seed('registration-002', 'REG002', '专家门诊', '12', 30),
    seed('registration-003', 'REG003', '急诊', '13', 20)
  ],
  ['name', 'busNo', 'ybNo']
)

function seed(id: string, busNo: string, name: string, ybNo: string, amount: number) {
  return { id, busNo, name, ybNo, amount, activeFlag: 1, activeFlagText: '启用' }
}
