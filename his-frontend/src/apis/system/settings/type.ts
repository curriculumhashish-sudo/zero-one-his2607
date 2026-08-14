export interface SettingDefinition {
  id: string
  title: string
  name: string
  description?: string
  remark?: string
}

export interface SettingRecord {
  id: number
  name: string
  description: string
  value: string
  providerName: string
  providerKey: string
  createdAt?: string
  updatedAt?: string
}

export interface SettingProviderOption {
  label: string
  value: string
}

export interface ProviderKeyOption {
  key: string
  label: string
}

export interface SettingInitResult {
  providers: SettingProviderOption[]
}

export interface SettingSavePayload {
  name: string
  description: string
  value: string
  providerName: string
  providerKey: string
}

export interface SettingUpdatePayload {
  description?: string
  value?: string
}
