/** 打印申请单接口请求。 */
export interface PrintDoctorAdviceApplicationFormDTO {
  /** 签发非药品医嘱时生成的服务申请 ID。 */
  serviceRequestId: string
}

/** 可打印的医嘱申请单详情。 */
export interface DoctorAdviceApplicationFormVO {
  /** 医疗机构名称。 */
  hospitalName: string
  /** 申请单编号。 */
  applicationNo: string
  /** 后端服务申请 ID。 */
  serviceRequestId: string
  /** 申请单类别，例如检验、检查或治疗。 */
  categoryName: string
  /** 患者姓名。 */
  patientName: string
  /** 性别。 */
  genderName: string
  /** 年龄。 */
  age: number
  /** 病历号。 */
  patientNo: string
  /** 门诊就诊号。 */
  visitNo: string
  /** 联系电话。 */
  phone: string
  /** 申请科室。 */
  departmentName: string
  /** 申请医生。 */
  attendingDoctorName: string
  /** 申请时间。 */
  requestedAt: string
  /** 当前有效诊断，多个诊断以顿号连接。 */
  diagnosisNames: string
  /** 分诊主诉。 */
  chiefComplaint: string
  /** 医嘱记录 ID。 */
  adviceId: string
  /** 项目编码。 */
  itemCode: string
  /** 项目名称。 */
  itemName: string
  /** 项目规格。 */
  specification?: string
  /** 开立数量。 */
  quantity: number
  /** 开立数量单位。 */
  quantityUnit: string
  /** 医嘱备注。 */
  remark?: string
}
