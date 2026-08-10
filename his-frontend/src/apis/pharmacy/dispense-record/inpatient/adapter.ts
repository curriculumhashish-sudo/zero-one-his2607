import type {
  InpatientDispenseRecordItemDTO,
  ExportInpatientDispenseRecordsResult
} from './type'

interface BackendRecordRow {
  id?: string
  record_id?: string
  prescription_no?: string
  record_type?: string
  patient_id?: string
  patient_name?: string
  hospitalization_no?: string
  department_name?: string
  ward_name?: string
  bed_no?: string
  dispense_time?: string
  pharmacy?: string
  dispense_name?: string
  drug_name?: string
  specification?: string
  quantity?: number | string
  unit?: string
  price?: number | string
  money?: number | string
  batch_no?: string
  trace_code?: string
  dosage?: string
  summary_no?: string
}

export function adaptBackendRecordRow(row: BackendRecordRow): InpatientDispenseRecordItemDTO {
  return {
    id: row.id || '',
    recordId: row.record_id || '',
    prescriptionNo: row.prescription_no || '',
    recordType: (row.record_type?.toUpperCase() as 'DISPENSE' | 'REFUND') || 'DISPENSE',
    patientId: row.patient_id || '',
    patientName: row.patient_name || '',
    inpatientNo: row.hospitalization_no || '',
    departmentName: row.department_name || '',
    wardName: row.ward_name || '',
    bedNo: row.bed_no || '',
    dispenseTime: row.dispense_time || '',
    pharmacyName: row.pharmacy || '',
    dispenserName: row.dispense_name || '',
    drugName: row.drug_name || '',
    spec: row.specification || '',
    quantity: Number(row.quantity) || 0,
    unit: row.unit || '',
    price: Number(row.price) || 0,
    amount: Number(row.money) || 0,
    batchNo: row.batch_no || '',
    traceCode: row.trace_code || '',
    dosage: row.dosage || '',
    summaryNo: row.summary_no || ''
  }
}

export function adaptExportResult(success: boolean, message: string): ExportInpatientDispenseRecordsResult {
  return { success, message }
}