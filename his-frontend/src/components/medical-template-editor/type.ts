/**
 * 病历模板元数据字段模型
 * 用于定义可插入到模板中的变量字段
 */
export interface MedicalMetadataItem {
  /** 字段唯一标识，用于模板中变量替换，格式如：{{patientName}} */
  key: string
  /** 字段显示名称，用于元数据列表展示 */
  label: string
  /** 是否只读字段，只读字段在模板使用时不可修改 */
  readOnly: boolean
}

/**
 * 病历模板信息模型
 * 用于定义模板的基本属性
 */
export interface MedicalTemplateInfo {
  /** 模板名称 */
  name: string
  /** 模板编码，用于系统识别 */
  code: string
  /** 文书类型，如：病程记录、入院记录、出院记录、医嘱 */
  documentType: string
  /** 适用就诊类型，如：住院、门诊、急诊 */
  visitType: string
  /** 使用范围，如：全院、科室、个人 */
  scope: string
  /** 是否启用 */
  enabled: boolean
}

/**
 * 病历模板数据模型
 * 包含模板信息和模板内容
 */
export interface MedicalTemplate {
  /** 模板基本信息 */
  info: MedicalTemplateInfo
  /** 模板HTML内容 */
  content: string
}

/**
 * 创建病历模板信息实例，用于指定默认值
 * @param init 初始化数据
 * @returns 病历模板信息实例
 */
export function createMedicalTemplateInfo(
  init?: Partial<MedicalTemplateInfo>
): MedicalTemplateInfo {
  return {
    name: '',
    code: '',
    documentType: '病程记录',
    visitType: '住院',
    scope: '全院',
    enabled: true,
    ...init
  }
}

/**
 * 创建病历模板实例，用于指定默认值
 * @param init 初始化数据
 * @returns 病历模板实例
 */
export function createMedicalTemplate(init?: Partial<MedicalTemplate>): MedicalTemplate {
  return {
    info: createMedicalTemplateInfo(init?.info),
    content: init?.content || ''
  }
}

/**
 * 文书类型枚举值
 */
export const DOCUMENT_TYPE_OPTIONS = [
  { label: '病程记录', value: '病程记录' },
  { label: '入院记录', value: '入院记录' },
  { label: '出院记录', value: '出院记录' },
  { label: '医嘱', value: '医嘱' }
]

/**
 * 就诊类型枚举值
 */
export const VISIT_TYPE_OPTIONS = [
  { label: '住院', value: '住院' },
  { label: '门诊', value: '门诊' },
  { label: '急诊', value: '急诊' }
]

/**
 * 使用范围枚举值
 */
export const SCOPE_OPTIONS = [
  { label: '全院', value: '全院' },
  { label: '科室', value: '科室' },
  { label: '个人', value: '个人' }
]

/**
 * 默认元数据字段列表
 */
export const DEFAULT_METADATA_LIST: MedicalMetadataItem[] = [
  { key: 'caseNo', label: '诊号', readOnly: false },
  { key: 'patientName', label: '患者姓名', readOnly: false },
  { key: 'gender', label: '性别', readOnly: true },
  { key: 'age', label: '年龄', readOnly: true },
  { key: 'idCard', label: '证件号', readOnly: true },
  { key: 'medicalRecordNo', label: '病历号', readOnly: true },
  { key: 'phone', label: '电话', readOnly: false },
  { key: 'department', label: '科室', readOnly: true },
  { key: 'doctorName', label: '医师姓名', readOnly: true },
  { key: 'admissionDate', label: '入院日期', readOnly: true },
  { key: 'diagnosis', label: '初步诊断', readOnly: false }
]

/**
 * 模板数据替换模型
 * 用于在渲染模板时替换元数据变量的值
 */
export interface MedicalTemplateData {
  [key: string]: string | number | undefined
}

/**
 * 渲染模板内容，将元数据变量替换为实际数据
 * @param content 模板HTML内容
 * @param data 实际数据，键为元数据key，值为要替换的内容
 * @param options 渲染选项
 * @returns 渲染后的HTML内容
 */
export function renderTemplate(
  content: string,
  data: MedicalTemplateData = {},
  options: {
    placeholder?: string
    preserveReadOnlyStyle?: boolean
  } = {}
): string {
  const { placeholder = '', preserveReadOnlyStyle = false } = options

  return content.replace(
    /<span[^>]*data-metadata-variable[^>]*>(\[([^\]]+)\])?<\/span>/gi,
    (match: string, variableText?: string, label?: string) => {
      const keyMatch = match.match(/data-key="([^"]+)"/)
      const key = keyMatch ? keyMatch[1] : ''

      if (key) {
        const value = data[key]
        if (value !== undefined && value !== null) {
          return String(value)
        }
      }

      if (placeholder) {
        return placeholder
      }

      if (label && preserveReadOnlyStyle) {
        return `<span class="metadata-variable readonly" style="background:#f5f5f5;color:#909399;padding:2px 8px;border-radius:4px;">${variableText || `[${label}]`}</span>`
      }

      return variableText || `[${label || key}]`
    }
  )
}

/**
 * 提取模板中的元数据变量列表
 * @param content 模板HTML内容
 * @returns 元数据key列表
 */
export function extractMetadataKeys(content: string): string[] {
  const keys: string[] = []
  const regex = /<span[^>]*data-metadata-variable[^>]*[^>]*>/gi
  let match

  while ((match = regex.exec(content)) !== null) {
    const keyMatch = match[0].match(/data-key="([^"]+)"/)
    if (keyMatch && keyMatch[1] && !keys.includes(keyMatch[1])) {
      keys.push(keyMatch[1])
    }
  }

  return keys
}
