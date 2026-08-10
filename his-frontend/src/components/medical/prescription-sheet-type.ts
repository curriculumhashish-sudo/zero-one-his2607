/** 可复用处方笺视图所需的展示数据。 */
export interface PrescriptionSheetData {
  patientName?: string
  age?: number
  gender?: string
  department?: string
  patientId?: string
  prescriptionNo?: string
  costType?: string
  date?: string
  doctor?: string
  diagnosis?: string
  totalPrice?: number
  pharmacist?: string
  dispenser?: string
  priceChecker?: string
  items?: string
}
