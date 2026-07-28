/** 临床病历编辑器使用的文书类型选项。 */
export interface ClinicalMedicalRecordDocumentTypeOption {
  /** 稳定文书类型编码。 */
  value: string
  /** 展示名称。 */
  label: string
}

/** 病历状态标签颜色。 */
export type ClinicalMedicalRecordStatusType = 'primary' | 'success' | 'info' | 'warning' | 'danger'
