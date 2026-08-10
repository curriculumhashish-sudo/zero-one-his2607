/** 处方单查询范围，与参考项目的“本次就诊 / 历史处方”页签对应。 */
export type DoctorPrescriptionScope = 'CURRENT_ENCOUNTER' | 'HISTORY'

/** GET /cyclectrl/prescription_query 的查询参数。 */
export interface DoctorPrescriptionSheetQuery {
  /** 患者病历号，对应后端参数 num */
  num?: string
  /** 查询本次就诊或历史处方，对应后端参数 type */
  type?: DoctorPrescriptionScope
}

/**
 * 门诊处方单详情。
 *
 * 字段与后端 Prescription2DetailDTO 保持一致；文档中的 patientId 实际说明为门诊号。
 */
export interface DoctorPrescriptionSheetVO extends Record<string, unknown> {
  /** 患者姓名 */
  patientName?: string
  /** 年龄 */
  age?: number
  /** 性别 */
  gender?: string
  /** 开方科室 */
  department?: string
  /** 门诊号；沿用后端字段名 patientId */
  patientId?: string
  /** 处方号 */
  prescriptionNo?: string
  /** 费用性质 */
  costType?: string
  /** 开方日期 */
  date?: string
  /** 开单医生 */
  doctor?: string
  /** 临床诊断 */
  diagnosis?: string
  /** 处方总金额 */
  totalPrice?: number
  /** 调配药师 */
  pharmacist?: string
  /** 发药药师 */
  dispenser?: string
  /** 划价人员 */
  priceChecker?: string
  /** Rp 处方正文 */
  items?: string
}
