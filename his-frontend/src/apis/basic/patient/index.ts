import type { PatientNameOption, PatientQuery, PatientSavePayload } from './type'
import { mockCreatePatient, mockUpdatePatient, patientStore } from './mock'

/** 获取患者名称选项。 */
export async function loadPatientNameOptions(keyword = ''): Promise<PatientNameOption[]> {
  const rows = await patientStore.list()
  const term = keyword.trim().toLowerCase()
  return rows
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: `${row.name}（${row.busNo}）`, value: row.id }))
}

/** 分页查询患者列表。 */
export const pagePatients = (query: PatientQuery) => patientStore.page(query)
/** 获取患者详情。 */
export const getPatientDetail = (id: string) => patientStore.detail(id)
/** 新增患者。 */
export const createPatient = (data: PatientSavePayload) => mockCreatePatient(data)
/** 修改患者。 */
export const updatePatient = (id: string, data: PatientSavePayload) => mockUpdatePatient(id, data)
/** 删除患者。 */
export const removePatient = (id: string) => patientStore.remove(id)

export type * from './type'
