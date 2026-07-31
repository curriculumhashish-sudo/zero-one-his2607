import type {
  InpatientSettlePresentationMode,
  InpatientSettleQueueTab
} from '@/apis/inpatient/charge/settle'

export interface InpatientSettlePageQuery {
  admissionDate: string | null
  keyword: string
  pageNo: number
  pageSize: number
  presentationMode: InpatientSettlePresentationMode
  settleQueueTab: InpatientSettleQueueTab
}
