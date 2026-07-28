import { createCrudStore } from '@/apis/basic/crud-mock'
import type { PatientRecord, PatientSavePayload } from './type'

const genderText: Record<string, string> = { MALE: '男', FEMALE: '女', UNKNOWN: '未知' }

const store = createCrudStore<PatientRecord>(
  'patient',
  [
    patient('patient-001', 'P000001', '张明', 'MALE', 35, '1991-03-12', '13800001111', '110101199103120018'),
    patient('patient-002', 'P000002', '李娜', 'FEMALE', 28, '1998-06-23', '13900002222', '110101199806230026'),
    patient('patient-003', 'P000003', '王建国', 'MALE', 62, '1964-01-09', '13700003333', '110101196401090017')
  ],
  ['name', 'busNo', 'idCard', 'phone']
)

function patient(
  id: string,
  busNo: string,
  name: string,
  genderEnum: string,
  age: number,
  birthDate: string,
  phone: string,
  idCard: string
): PatientRecord {
  return {
    id,
    busNo,
    name,
    genderEnum,
    genderEnumText: genderText[genderEnum] ?? '未知',
    age,
    birthDate,
    phone,
    idCard,
    nationalityCode: 'CN',
    workCompany: '',
    maritalStatusEnum: 'MARRIED',
    bloodAbo: 'A',
    bloodRh: 'POSITIVE',
    addressProvince: '北京市',
    addressCity: '北京市',
    addressDistrict: '东城区',
    address: '示例街道 1 号',
    linkName: '',
    linkTelcom: '',
    activeFlag: true
  }
}

export async function mockCreatePatient(payload: PatientSavePayload) {
  const rows = await store.list()
  return store.create({
    ...payload,
    busNo: `P${String(rows.length + 1).padStart(6, '0')}`,
    genderEnumText: genderText[payload.genderEnum] ?? '未知'
  })
}

export async function mockUpdatePatient(id: string, payload: PatientSavePayload) {
  return store.update(id, {
    ...payload,
    genderEnumText: genderText[payload.genderEnum] ?? '未知'
  })
}

export const patientStore = store
