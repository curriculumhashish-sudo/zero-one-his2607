import { mockPrintAdviceApplicationForm } from '../mock'
import type { DoctorAdviceApplicationFormVO, PrintDoctorAdviceApplicationFormDTO } from './type'

/**
 * 获取可打印的医嘱申请单。
 *
 * 当前转调 Mock；接入真实后端时替换为
 * POST /medical-advice/print-application-form 即可，页面无需修改。
 */
export function printAdviceApplicationForm(
  data: PrintDoctorAdviceApplicationFormDTO
): Promise<DoctorAdviceApplicationFormVO> {
  return mockPrintAdviceApplicationForm(data)
}

export type { DoctorAdviceApplicationFormVO, PrintDoctorAdviceApplicationFormDTO } from './type'
