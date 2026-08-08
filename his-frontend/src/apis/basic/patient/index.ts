import { useHttp } from '@/plugins/http'
import { toMutationResult, toPageResult, unwrap } from '../remote'
import type { PatientNameOption, PatientQuery, PatientRecord, PatientSavePayload } from './type'

const baseUrl = '/j3-patient/patients'

export async function loadPatientNameOptions(keyword = ''): Promise<PatientNameOption[]> {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPagePatient>(`${baseUrl}/options`, {
    keyword: keyword || undefined,
    pageIndex: 1,
    pageSize: 100
  }))
  return page.rows.map((row) => ({ label: `${row.name}（${row.busNo ?? row.id}）`, value: row.id }))
}

export async function pagePatients(query: PatientQuery) {
  const http = useHttp()
  const page = unwrap(await http.get<BackendPagePatient>(`${baseUrl}/page`, query))
  return toPageResult({
    ...page,
    rows: page.rows.map(toPatientRecord)
  })
}

export async function getPatientDetail(id: string) {
  const http = useHttp()
  return toPatientRecord(unwrap(await http.get<BackendPatient>(`${baseUrl}/${id}`)))
}

export async function createPatient(data: PatientSavePayload) {
  const http = useHttp()
  return toMutationResult<PatientRecord>(unwrap(await http.post<string>(baseUrl, toPatientPayload(data))))
}

export async function updatePatient(id: string, data: PatientSavePayload) {
  const http = useHttp()
  return toMutationResult<PatientRecord>(unwrap(await http.put<string>(`${baseUrl}/${id}`, {
    ...(data.version === undefined ? {} : { version: data.version }),
    ...toPatientPayload(data)
  })))
}

export async function removePatient(id: string) {
  const http = useHttp()
  unwrap(await http.delete<string>(`${baseUrl}/${id}`))
}

interface BackendPagePatient {
  pageIndex: number
  pageSize: number
  total: number
  pages?: number
  rows: BackendPatient[]
}

interface BackendPatient {
  id: string
  busNo?: string
  name: string
  nameJson?: string
  age?: number
  genderEnum?: string
  genderEnumText?: string
  birthDate?: string
  idCard?: string
  phone?: string
  statusEnum?: string
  activeFlag?: boolean
  tempFlag?: boolean
  address?: string
  addressProvince?: string
  addressCity?: string
  addressDistrict?: string
  addressStreet?: string
  addressExtendJson?: string
  workCompany?: string
  maritalStatusEnum?: string
  pyStr?: string
  bloodTypeAbo?: number
  bloodTypeRh?: number
  countryCode?: string
  nationalityCode?: string
  linkName?: string
  linkTelcom?: string
  linkRelationCode?: number | string
  prfsEnum?: string | number
  wbStr?: string
  nativePlace?: string
  deceasedDate?: string
  createdAt?: string
  updatedAt?: string
  organizationId?: string
  gender?: number
  version?: number
}

function toPatientRecord(item: BackendPatient): PatientRecord {
  return {
    id: String(item.id),
    version: item.version,
    busNo: item.busNo ?? '',
    name: item.name,
    nameJson: item.nameJson,
    genderEnum: item.genderEnum ?? genderEnum[item.gender ?? 0] ?? 'UNKNOWN',
    genderEnumText: item.genderEnumText ?? genderText[item.genderEnum ?? 'UNKNOWN'] ?? '未知',
    age: item.age ?? 0,
    birthDate: item.birthDate ?? '',
    phone: item.phone ?? '',
    idCard: item.idCard ?? '',
    nationalityCode: item.nationalityCode ?? item.countryCode ?? '',
    workCompany: item.workCompany ?? '',
    maritalStatusEnum: item.maritalStatusEnum ?? 'UNKNOWN',
    bloodAbo: bloodAboText[item.bloodTypeAbo ?? 0] ?? '未知',
    bloodRh: bloodRhText[item.bloodTypeRh ?? 0] ?? 'UNKNOWN',
    addressProvince: item.addressProvince ?? '',
    addressCity: item.addressCity ?? '',
    addressDistrict: item.addressDistrict ?? '',
    addressStreet: item.addressStreet ?? '',
    addressExtendJson: item.addressExtendJson ?? '',
    address: item.address ?? '',
    linkName: item.linkName ?? '',
    linkTelcom: item.linkTelcom ?? '',
    linkRelationCode: item.linkRelationCode,
    prfsEnum: item.prfsEnum,
    pyStr: item.pyStr ?? '',
    wbStr: item.wbStr ?? '',
    nativePlace: item.nativePlace ?? '',
    tempFlag: item.tempFlag ?? false,
    deceasedDate: item.deceasedDate ?? '',
    organizationId: item.organizationId ?? '',
    statusEnum: item.statusEnum ?? '',
    activeFlag: item.activeFlag ?? item.statusEnum === 'ACTIVE'
  }
}

function toPatientPayload(data: PatientSavePayload) {
  const payload: Record<string, unknown> = {
    name: data.name,
    genderEnum: data.genderEnum,
    birthDate: data.birthDate || null,
    age: data.age,
    idCard: data.idCard,
    maritalStatusEnum: data.maritalStatusEnum,
    phone: data.phone,
    address: data.address,
    addressProvince: data.addressProvince,
    addressCity: data.addressCity,
    addressDistrict: data.addressDistrict,
    addressStreet: data.addressStreet,
    addressExtendJson: data.addressExtendJson,
    nationalityCode: data.nationalityCode,
    bloodTypeAbo: toBloodAbo(data.bloodAbo),
    bloodTypeRh: toBloodRh(data.bloodRh),
    workCompany: data.workCompany,
    linkName: data.linkName,
    linkTelcom: data.linkTelcom,
    linkRelationCode: data.linkRelationCode === '' ? null : data.linkRelationCode,
    prfsEnum: data.prfsEnum,
    pyStr: data.pyStr,
    wbStr: data.wbStr,
    nativePlace: data.nativePlace,
    tempFlag: data.tempFlag,
    deceasedDate: data.deceasedDate || null,
    organizationId: data.organizationId,
    activeFlag: data.activeFlag,
    statusEnum: data.activeFlag ? 'ACTIVE' : 'INACTIVE'
  }
  // C5 新增接口接受 activeFlag；保留它，避免新增患者只能使用后端默认启用状态。
  return payload
}

const genderText: Record<string, string> = { MALE: '男', FEMALE: '女', UNKNOWN: '未知' }
const genderEnum: Record<number, string> = { 1: 'MALE', 2: 'FEMALE', 0: 'UNKNOWN' }
const bloodAboText: Record<number, string> = { 0: '未知', 1: 'A', 2: 'B', 3: 'AB', 4: 'O' }
const bloodRhText: Record<number, string> = { 0: 'UNKNOWN', 1: 'POSITIVE', 2: 'NEGATIVE' }

function toBloodAbo(value: string) {
  return { A: 1, B: 2, AB: 3, O: 4, 未知: 0 }[value as 'A' | 'B' | 'AB' | 'O' | '未知'] ?? 0
}

function toBloodRh(value: string) {
  return { POSITIVE: 1, NEGATIVE: 2, UNKNOWN: 0 }[value as 'POSITIVE' | 'NEGATIVE' | 'UNKNOWN'] ?? 0
}

export type * from './type'
