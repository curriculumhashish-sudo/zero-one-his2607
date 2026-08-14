export interface OutpatientRegisterFormModel {
  appointmentId: string
  appointmentMode: string
  age: number | null
  birthDate: string
  contractId: string
  gender: string
  idCardNo: string
  name: string
  organizationId: string
  patientId: string
  patientBusNo: string
  phone: string
  practitionerId: string
  registrationDate: string
  serviceTypeId: string
  slotId: string
  ybRegisterFee: boolean
  ybUseAcctPay: boolean
  psnNo: string
  insuplcAdmdvs: string
  insutype: string
  ybMediumType: string
  ybCardInfo: string
  medType: string
}

export interface RegisterHistoryQueryModel {
  keyword: string
  pageIndex: number
  pageSize: number
}
