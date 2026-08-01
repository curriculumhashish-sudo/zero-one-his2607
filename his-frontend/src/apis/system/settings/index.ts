import {
  mockCreateSetting,
  mockDeleteSetting,
  mockGetProviderKeys,
  mockInitSetting,
  mockInitSettingDefinitions,
  mockPageSettings,
  mockUpdateSetting
} from './mock'
import type {
  ProviderKeyOption,
  SettingDefinition,
  SettingInitResult,
  SettingRecord,
  SettingSavePayload,
  SettingUpdatePayload
} from './type'

export const initSettingDefinitions = (): Promise<SettingDefinition[]> => mockInitSettingDefinitions()
export const initSetting = (): Promise<SettingInitResult> => mockInitSetting()
export const getProviderKeys = (providerName: string): Promise<ProviderKeyOption[]> =>
  mockGetProviderKeys(providerName)
export const pageSettings = (name: string): Promise<SettingRecord[]> => mockPageSettings(name)
export const createSetting = (data: SettingSavePayload): Promise<SettingRecord> => mockCreateSetting(data)
export const updateSetting = (id: number, data: SettingUpdatePayload): Promise<SettingRecord> =>
  mockUpdateSetting(id, data)
export const deleteSetting = (id: number): Promise<void> => mockDeleteSetting(id)

export type * from './type'
