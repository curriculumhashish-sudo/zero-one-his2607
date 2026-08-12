export interface PrintDoctorAdviceApplicationFormDTO {
  serviceRequestId: string
}

export interface DoctorAdviceApplicationFormVO {
  hospitalName: string
  applicationNo: string
  serviceRequestId: string
  categoryName: string
  patientName: string
  genderName: string
  age: number
  patientNo: string
  visitNo: string
  phone: string
  departmentName: string
  attendingDoctorName: string
  requestedAt: string
  diagnosisNames: string
  chiefComplaint: string
  adviceId: string
  itemCode: string
  itemName: string
  specification?: string
  quantity: number
  quantityUnit: string
  remark?: string
}
