export interface InpatientReturnMedicineItemRow {
  key: string
  itemId: string
  itemName: string
  spec: string
  quantity: number
  unit: string
  batchNo: string
  totalPrice: number
  pharmacy: string
  traceCode: string
  factory: string
  status: 'refundable' | 'partially-refundable' | 'non-refundable' | 'refunded'
  refundQuantity: number
  originalQuantity: number
}
