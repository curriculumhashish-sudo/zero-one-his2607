import type {
  RegistrationTypeNameOption,
  RegistrationTypeQuery,
  RegistrationTypeSavePayload
} from './type'
import { registrationTypeStore } from './mock'

/** 获取挂号类型名称。 */
export async function loadRegistrationTypeNameOptions(
  keyword = ''
): Promise<RegistrationTypeNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return (await registrationTypeStore.list())
    .filter((row) => !term || row.name.toLowerCase().includes(term) || row.busNo.includes(term))
    .map((row) => ({ label: row.name, value: row.id }))
}
/** 获取挂号类型列表。 */
export const pageRegistrationTypes = (query: RegistrationTypeQuery) =>
  registrationTypeStore.page(query)
/** 添加挂号类型。 */
export async function createRegistrationType(data: RegistrationTypeSavePayload) {
  const rows = await registrationTypeStore.list()
  return registrationTypeStore.create({
    ...data,
    busNo: `REG${String(rows.length + 1).padStart(3, '0')}`,
    activeFlagText: data.activeFlag === 1 ? '启用' : '停用'
  })
}
/** 修改挂号类型。 */
export const updateRegistrationType = (id: string, data: RegistrationTypeSavePayload) =>
  registrationTypeStore.update(id, {
    ...data,
    activeFlagText: data.activeFlag === 1 ? '启用' : '停用'
  })
/** 删除挂号类型。 */
export const removeRegistrationType = (id: string) => registrationTypeStore.remove(id)

export type * from './type'
