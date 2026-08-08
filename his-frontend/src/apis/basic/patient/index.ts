import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { PatientNameOption, PatientQuery, PatientRecord, PatientSavePayload } from './type'

const baseUrl = '/c5-basicdata-api/pat'

interface BackendPatient {
  id: string | number; version?: number; busNo?: string; name: string; nameJson?: string; age?: number
  genderEnum?: string; genderEnumText?: string; birthDate?: string; idCard?: string; phone?: string
  statusEnum?: string; activeFlag?: boolean; tempFlag?: boolean; address?: string; addressProvince?: string
  addressCity?: string; addressDistrict?: string; addressStreet?: string; addressExtendJson?: string
  workCompany?: string; maritalStatusEnum?: string; pyStr?: string; bloodTypeAbo?: number; bloodTypeRh?: number
  nationalityCode?: string; countryCode?: string; linkName?: string; linkTelcom?: string
  linkRelationCode?: number | string; prfsEnum?: string | number; wbStr?: string; nativePlace?: string
  deceasedDate?: string; organizationId?: string | number
}
interface BackendPage { pageIndex: number; pageSize: number; total: number; pages?: number; rows: BackendPatient[] }

export async function loadPatientNameOptions(keyword = ''): Promise<PatientNameOption[]> {
  const page = unwrap(await useHttp().get<BackendPage>(`${baseUrl}/select/page`, { keyword, pageIndex: 1, pageSize: 100 }))
  return page.rows.map((row) => ({ label: `${row.name}（${row.busNo ?? row.id}）`, value: String(row.id) }))
}
export async function pagePatients(query: PatientQuery) {
  const page = unwrap(await useHttp().get<BackendPage>(`${baseUrl}/page`, {
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    name: query.keyword || undefined
  }))
  return toPageResult({ ...page, rows: page.rows.map(toRecord) })
}
export async function getPatientDetail(id: string) {
  return toRecord(unwrap(await useHttp().get<BackendPatient>(`${baseUrl}/query-by-id`, { id })))
}
export async function createPatient(data: PatientSavePayload) {
  return toMutationResult<PatientRecord>(unwrap(await useHttp().post<string>(`${baseUrl}/add`, toPayload(data, false))))
}
export async function updatePatient(id: string, data: PatientSavePayload) {
  return toMutationResult<PatientRecord>(unwrap(await useHttp().put<string>(`${baseUrl}/modify`, { id, version: data.version, ...toPayload(data, true) })))
}
export async function removePatient(id: string) {
  unwrap(await useHttp().delete<string>(`${baseUrl}/delete`, [id]))
}

function toRecord(item: BackendPatient): PatientRecord {
  const gender = item.genderEnum ?? 'UNKNOWN'
  const active = item.activeFlag ?? item.statusEnum === 'ACTIVE'
  return {
    id: String(item.id), version: item.version, busNo: item.busNo ?? '', name: item.name, nameJson: item.nameJson,
    genderEnum: gender, genderEnumText: item.genderEnumText ?? ({ MALE: '男', FEMALE: '女', UNKNOWN: '未知' }[gender] ?? gender),
    age: item.age ?? 0, birthDate: item.birthDate ?? '', phone: item.phone ?? '', idCard: item.idCard ?? '',
    nationalityCode: item.nationalityCode ?? item.countryCode ?? '', workCompany: item.workCompany ?? '',
    maritalStatusEnum: item.maritalStatusEnum ?? 'UNKNOWN', bloodAbo: ({ 1: 'A', 2: 'B', 3: 'AB', 4: 'O' }[item.bloodTypeAbo ?? 0] ?? '未知'),
    bloodRh: ({ 1: 'POSITIVE', 2: 'NEGATIVE' }[item.bloodTypeRh ?? 0] ?? 'UNKNOWN'), addressProvince: item.addressProvince ?? '',
    addressCity: item.addressCity ?? '', addressDistrict: item.addressDistrict ?? '', addressStreet: item.addressStreet ?? '',
    addressExtendJson: item.addressExtendJson ?? '', address: item.address ?? '', linkName: item.linkName ?? '',
    linkTelcom: item.linkTelcom ?? '', linkRelationCode: item.linkRelationCode, prfsEnum: item.prfsEnum, pyStr: item.pyStr ?? '', wbStr: item.wbStr ?? '',
    nativePlace: item.nativePlace ?? '', tempFlag: item.tempFlag ?? false, deceasedDate: item.deceasedDate ?? '', organizationId: item.organizationId == null ? '' : String(item.organizationId),
    statusEnum: item.statusEnum ?? '', activeFlag: active
  }
}
function toPayload(data: PatientSavePayload, isUpdate: boolean) {
  return {
    name: data.name, nameJson: data.nameJson, genderEnum: data.genderEnum, birthDate: data.birthDate || null,
    idCard: data.idCard,
    maritalStatusEnum: data.maritalStatusEnum, phone: data.phone, address: data.address, addressProvince: data.addressProvince,
    addressCity: data.addressCity, addressDistrict: data.addressDistrict, addressStreet: data.addressStreet, addressExtendJson: data.addressExtendJson,
    countryCode: data.nationalityCode, bloodTypeAbo: ({ A: 1, B: 2, AB: 3, O: 4, 未知: 0 } as Record<string, number>)[data.bloodAbo] ?? 0,
    bloodTypeRh: ({ POSITIVE: 1, NEGATIVE: 2, UNKNOWN: 0 } as Record<string, number>)[data.bloodRh] ?? 0, workCompany: data.workCompany,
    linkName: data.linkName, linkTelcom: data.linkTelcom, linkRelationCode: data.linkRelationCode === '' ? null : data.linkRelationCode,
    prfsEnum: data.prfsEnum, pyStr: data.pyStr, wbStr: data.wbStr, nativePlace: data.nativePlace, tempFlag: data.tempFlag,
    organizationId: data.organizationId, activeFlag: data.activeFlag,
    statusEnum: data.activeFlag ? 'ACTIVE' : 'INACTIVE',
    ...(isUpdate ? { deceasedDate: data.deceasedDate || null } : {})
  }
}
export type * from './type'
