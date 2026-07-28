import type { WardBedForm, WardBedNameOption, WardBedQuery, WardBedSavePayload } from './type'
import { enrichWardBed, wardBedStore } from './mock'

async function names(form: WardBedForm, keyword = ''): Promise<WardBedNameOption[]> {
  const term = keyword.trim().toLowerCase()
  return (await wardBedStore.list())
    .filter((row) => row.formEnum === form && (!term || row.name.toLowerCase().includes(term)))
    .map((row) => ({ label: row.name, value: row.id }))
}

/** 获取病床名称。 */
export const loadBedNameOptions = (keyword = '') => names('BED', keyword)
/** 获取病房名称。 */
export const loadRoomNameOptions = (keyword = '') => names('HOUSE', keyword)
/** 获取病区名称。 */
export const loadWardNameOptions = (keyword = '') => names('WARD', keyword)
/** 获取病区、病房、床位列表。 */
export const pageWardBeds = (query: WardBedQuery) => wardBedStore.page(query)
/** 添加病区、病房或床位。 */
export async function createWardBed(data: WardBedSavePayload) {
  const rows = await wardBedStore.list()
  return wardBedStore.create({
    ...(await enrichWardBed(data)),
    busNo: `${data.formEnum.slice(0, 1)}${String(rows.length + 1).padStart(3, '0')}`
  })
}
/** 修改病区、病房或床位。 */
export async function updateWardBed(id: string, data: WardBedSavePayload) {
  return wardBedStore.update(id, await enrichWardBed(data))
}
/** 删除病区、病房或床位。 */
export const removeWardBed = (id: string) => wardBedStore.remove(id)
/** 启用或停用病区、病房或床位。 */
export async function setWardBedEnabled(id: string, enabled: boolean) {
  return wardBedStore.update(id, {
    statusEnum: enabled ? 'ACTIVE' : 'INACTIVE',
    statusEnumText: enabled ? '启用' : '停用'
  })
}

export type * from './type'
