/**
 * 患者来源类型。
 *
 * 使用 string 扩展位，避免组件模型与后端枚举强耦合。
 */
export type PatientType = 'outpatient' | 'inpatient' | 'emergency' | (string & {})

/** 患者状态在卡片中的语义色。 */
export type PatientStatusTone = 'primary' | 'success' | 'warning' | 'danger' | 'info'

/**
 * 跨门诊、住院、急诊场景使用的统一患者摘要模型。
 *
 * 页面层负责将接口 DTO 转换为该模型，PatientCard 不感知接口结构。
 */
export interface PatientSummary {
  /** 患者或当前就诊记录的唯一标识。 */
  id: string | number
  /** 患者姓名；空值时组件显示“无档案登记”。 */
  name?: string | null
  gender?: string | null
  age?: string | number | null
  patientType?: PatientType | null
  /** 就诊类别、床位或其他需要展示在姓名右侧的摘要。 */
  serviceType?: string | null
  /** 病历号、住院号等患者级编号。 */
  patientNo?: string | null
  /** 就诊号、流水号等本次就诊编号。 */
  visitNo?: string | null
  queueNumber?: string | number | null
  visitTime?: string | null
  departmentName?: string | null
  /** 供业务流程判断使用的原始状态码；status 仍用于展示。 */
  statusCode?: string | null
  status?: string | null
  statusTone?: PatientStatusTone
  paymentType?: string | null
  priority?: string | null
}
