export interface ReturnMedicinePrescriptionBlock {
  slipId: string
  rxNo: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: ReturnMedicinePrescriptionItemRow[]
}

export interface ReturnMedicinePrescriptionItemRow {
  key: string
  rxNo: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  medicationType: 'western' | 'chinese'
  itemName: string
  quantity: number
  spec: string
  dosage: string
  days: number
  totalPrice: number
  batchNo: string
  traceCode: string
  factory: string
  pharmacy: string
  reason?: string
}