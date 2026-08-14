import { createCrudStore } from '@/apis/basic/crud-mock'
import type { WardBedForm, WardBedRecord, WardBedSavePayload } from './type'

const formText: Record<WardBedForm, string> = { WARD: '病区', HOUSE: '病房', BED: '床位' }
const organizations: Record<string, string> = { 'dept-001': '内科', 'dept-002': '外科' }
const locationNames: Record<string, string> = {
  'ward-001': '内科一病区',
  'room-001': '内科101病房',
  'bed-001': '01床',
  'bed-002': '02床',
  'ward-002': '外科一病区'
}

const initialRows: WardBedRecord[] = [
  row('ward-001', 'W001', '内科一病区', 'WARD', 'dept-001', '', 10),
  row('room-001', 'R001', '内科101病房', 'HOUSE', 'dept-001', 'ward-001', 20),
  row('bed-001', 'B001', '01床', 'BED', 'dept-001', 'room-001', 30, 'IN_USE'),
  row('bed-002', 'B002', '02床', 'BED', 'dept-001', 'room-001', 31),
  row('ward-002', 'W002', '外科一病区', 'WARD', 'dept-002', '', 40)
]

function row(
  id: string,
  busNo: string,
  name: string,
  formEnum: WardBedForm,
  organizationId: string,
  parentId: string,
  displayOrder: number,
  operationalEnum: 'IN_USE' | 'NOT_IN_USE' = 'NOT_IN_USE'
): WardBedRecord {
  return {
    id,
    busNo,
    name,
    formEnum,
    formEnumText: formText[formEnum],
    organizationId,
    organizationName: organizations[organizationId] ?? '',
    parentId,
    parentName: locationNames[parentId] ?? '',
    displayOrder,
    statusEnum: 'ACTIVE',
    statusEnumText: '启用',
    operationalEnum,
    operationalEnumText: operationalEnum === 'IN_USE' ? '使用中' : '未使用'
  }
}

export const wardBedStore = createCrudStore<WardBedRecord>('ward-bed', initialRows, [
  'name',
  'busNo',
  'organizationName',
  'parentName'
])

export async function enrichWardBed(payload: WardBedSavePayload) {
  const rows = await wardBedStore.list()
  return {
    ...payload,
    formEnumText: formText[payload.formEnum],
    organizationName: organizations[payload.organizationId] ?? '',
    parentName: rows.find((item) => item.id === payload.parentId)?.name ?? '',
    statusEnumText: payload.statusEnum === 'ACTIVE' ? '启用' : '停用',
    operationalEnumText: payload.operationalEnum === 'IN_USE' ? '使用中' : '未使用'
  }
}
