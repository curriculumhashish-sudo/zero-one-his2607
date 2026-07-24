import type { PatientSummary } from '@/components/patient/type'
import type { AdviceOrder } from '@/components/medical/type'

export type DispenseQueuePatient = Omit<PatientSummary, 'status'> & {
  patientId?: string
  patientName?: string
  genderText?: string
  ageText?: string
  status?: 'pending' | 'completed'
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
  /** 保留接口原始状态，区分需要保存调配的行和已经调配完成的行。 */
  sourceStatus: string
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  batchNo: string
  traceCode: string
  factory: string
  inventoryPickOptions: { label: string; value: string }[]
  checked?: boolean
}

export type DispensePrescriptionBlock = {
  id: string
  rxNo: string
  /** 处方类型用于西药/中药页签过滤，避免按钮只改变样式。 */
  rxType: 'western' | 'herbal'
  status: 'pending' | 'partial' | 'completed' | 'blocking'
  items: DispensePrescriptionItemRow[]
}
