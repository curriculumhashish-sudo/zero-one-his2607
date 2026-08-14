import type { PatientSummary } from '@/components/patient/type'
import type { AdviceOrder } from '@/components/medical/type'

export type DispenseQueuePatient = PatientSummary & {
  patientId?: string
  patientName?: string
  genderText?: string
  ageText?: string
  prescriptionCount?: number
  registerTime?: string
}

export type DispensePrescriptionItemRow = AdviceOrder & {
  id: string
  rxId: string
  rxNo: string
  isFirstInRx: boolean
  isLastInRx: boolean
  rxRowSpan: number
  itemName: string
  spec: string
  quantity: number
  dosage: string
  days: number
  totalPrice: number
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  medicationType: 'western' | 'chinese'
  batchNo: string
  traceCode: string
  factory: string
  inventoryPickOptions: { label: string; value: string }[]
  checked?: boolean
}

export type DispensePrescriptionBlock = {
  id: string
  rxNo: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: DispensePrescriptionItemRow[]
}
