import type {
  ProviderKeyOption,
  SettingDefinition,
  SettingInitResult,
  SettingRecord,
  SettingSavePayload,
  SettingUpdatePayload
} from './type'

const DEFINITIONS: SettingDefinition[] = [
  { id: 'd1', title: '系统名称', name: 'system.title', description: '系统显示名称' },
  { id: 'd2', title: '登录页标题', name: 'login.pageTitle', description: '登录页大标题' },
  { id: 'd3', title: '登录页副标题', name: 'login.subTitle', description: '登录页副标题' },
  { id: 'd4', title: '主题色', name: 'theme.primaryColor', description: '系统主题色' },
  { id: 'd5', title: '页面宽度', name: 'layout.contentWidth', description: '内容区域宽度：fluid 或 fixed' },
  { id: 'd6', title: '多标签', name: 'layout.multiTab', description: '是否开启多标签页' },
  { id: 'd7', title: '水印文本', name: 'layout.watermarkText', description: '水印文字，空则不显示' },
  { id: 'd8', title: '版本名称', name: 'about.versionName', description: '关于页版本名称' }
]

const PROVIDERS = [
  { label: '全局 G', value: 'G' },
  { label: '用户 U', value: 'U' },
  { label: '角色 R', value: 'R' },
  { label: '租户 T', value: 'T' }
]

const USER_KEYS: ProviderKeyOption[] = [
  { key: 'u1', label: '张三' },
  { key: 'u2', label: '李四' },
  { key: 'u3', label: '王五' }
]

const ROLE_KEYS: ProviderKeyOption[] = [
  { key: 'admin', label: '管理员' },
  { key: 'doctor', label: '医生' },
  { key: 'nurse', label: '护士' }
]

const TENANT_KEYS: ProviderKeyOption[] = [
  { key: 't1', label: '总部租户' },
  { key: 't2', label: '华东分公司' }
]

let settingSeed = 100
const SETTINGS: SettingRecord[] = [
  { id: settingSeed++, name: 'system.title', description: '系统显示名称', value: 'HIS 管理系统', providerName: 'G', providerKey: '', createdAt: '2025-06-01', updatedAt: '2025-06-01' },
  { id: settingSeed++, name: 'theme.primaryColor', description: '系统主题色', value: '#409EFF', providerName: 'G', providerKey: '', createdAt: '2025-06-01', updatedAt: '2025-06-01' },
  { id: settingSeed++, name: 'layout.multiTab', description: '是否开启多标签页', value: 'true', providerName: 'G', providerKey: '', createdAt: '2025-06-01', updatedAt: '2025-06-02' },
  { id: settingSeed++, name: 'layout.multiTab', description: '是否开启多标签页', value: 'false', providerName: 'U', providerKey: 'u1', createdAt: '2025-06-03', updatedAt: '2025-06-03' }
]

function delay<T>(data: T, ms = 150): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(data), ms))
}

export function mockInitSettingDefinitions(): Promise<SettingDefinition[]> {
  return delay([...DEFINITIONS])
}

export function mockInitSetting(): Promise<SettingInitResult> {
  return delay({ providers: [...PROVIDERS] })
}

export function mockGetProviderKeys(providerName: string): Promise<ProviderKeyOption[]> {
  if (providerName === 'U') return delay([...USER_KEYS])
  if (providerName === 'R') return delay([...ROLE_KEYS])
  if (providerName === 'T') return delay([...TENANT_KEYS])
  return delay([])
}

export function mockPageSettings(name: string): Promise<SettingRecord[]> {
  const list = SETTINGS.filter((s) => s.name === name)
  return delay(list)
}

export function mockCreateSetting(data: SettingSavePayload): Promise<SettingRecord> {
  const record: SettingRecord = {
    id: settingSeed++,
    name: data.name,
    description: data.description,
    value: data.value,
    providerName: data.providerName,
    providerKey: data.providerKey,
    createdAt: new Date().toISOString().slice(0, 10),
    updatedAt: new Date().toISOString().slice(0, 10)
  }
  SETTINGS.push(record)
  return delay(record)
}

export function mockUpdateSetting(id: number, data: SettingUpdatePayload): Promise<SettingRecord> {
  const idx = SETTINGS.findIndex((s) => s.id === id)
  if (idx < 0) throw new Error('配置不存在')
  const existing = SETTINGS[idx]!
  const updated: SettingRecord = {
    ...existing,
    description: data.description ?? existing.description,
    value: data.value ?? existing.value,
    updatedAt: new Date().toISOString().slice(0, 10)
  }
  SETTINGS[idx] = updated
  return delay(updated)
}

export function mockDeleteSetting(id: number): Promise<void> {
  const idx = SETTINGS.findIndex((s) => s.id === id)
  if (idx < 0) throw new Error('配置不存在')
  SETTINGS.splice(idx, 1)
  return delay(undefined)
}
