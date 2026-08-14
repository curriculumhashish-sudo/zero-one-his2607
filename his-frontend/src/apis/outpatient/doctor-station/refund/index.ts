import { mockApplyDoctorRefund, mockQueryDoctorRefundItems } from './mock'
import type { DoctorRefundApplicationDTO, DoctorRefundItemVO, DoctorRefundQuery } from './type'

/**
 * 获取患者退费清单。
 *
 * 真实后端接口：GET /cyclectrl/refund_query。
 * 当前阶段沿用医生站 Mock-first 结构，联调时只需替换本函数内部实现。
 */
export function queryDoctorRefundItems(query: DoctorRefundQuery): Promise<DoctorRefundItemVO[]> {
  return mockQueryDoctorRefundItems(query)
}

/**
 * 提交退费申请。
 *
 * 真实后端接口：POST /cyclectrl/refund_apply，返回退费申请号。
 */
export function applyDoctorRefund(data: DoctorRefundApplicationDTO): Promise<string> {
  return mockApplyDoctorRefund(data)
}

export type * from './type'
