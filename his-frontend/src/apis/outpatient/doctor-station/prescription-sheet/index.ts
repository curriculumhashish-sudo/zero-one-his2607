import { mockQueryDoctorPrescriptionSheets } from './mock'
import type { DoctorPrescriptionSheetQuery, DoctorPrescriptionSheetVO } from './type'

/**
 * 查询患者处方单。
 *
 * 真实后端接口：GET /cyclectrl/prescription_query。
 * 当前阶段沿用医生站 Mock-first 结构，联调时只需替换本函数内部实现。
 */
export function queryDoctorPrescriptionSheets(
  query: DoctorPrescriptionSheetQuery
): Promise<DoctorPrescriptionSheetVO[]> {
  return mockQueryDoctorPrescriptionSheets(query)
}

export type * from './type'
