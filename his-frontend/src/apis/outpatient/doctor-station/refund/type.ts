/**
 * 退费清单中的单条收费明细。
 *
 * 字段与后端 Refund2DetailDTO 保持一致；后端文档将这些字段定义为可选，
 * 因此页面展示时必须为缺失值提供兜底文本。
 */
export interface DoctorRefundItemVO extends Record<string, unknown> {
  /** 患者姓名 */
  patientName?: string
  /** 支付单号 */
  paymentNo?: string
  /** 收费项目名称 */
  itemName?: string
  /** 收费数量 */
  quantity?: string
  /** 收费金额 */
  amount?: string
  /** 收费状态 */
  chargeStatus?: string
  /** 退费状态 */
  refundStatus?: string
}

/** GET /cyclectrl/refund_query 的查询参数。 */
export interface DoctorRefundQuery {
  /** 患者病历号，对应后端查询参数 num */
  num?: string
}

/** POST /cyclectrl/refund_apply 的请求体。 */
export interface DoctorRefundApplicationDTO {
  /** 患者病历号 */
  patientNo?: string
  /** 医生勾选的退费明细 */
  apply?: DoctorRefundItemVO[]
  /** 勾选明细数量，不是申请单号 */
  applyNo?: number
  /** 勾选明细金额合计 */
  applySum?: number
}
