import type {
  OutpatientDispenseRecordItemDTO
} from '@/apis/pharmacy/dispense-record/outpatient/type'
import type {
  InpatientDispenseRecordItemDTO
} from '@/apis/pharmacy/dispense-record/inpatient/type'
import type { DispenseRecordItem, RecordType } from './type'
import { getRecordTypeLabel } from './constants'

export function adaptOutpatientRecord(
  dto: OutpatientDispenseRecordItemDTO
): DispenseRecordItem {
  return {
    id: dto.id,
    recordId: dto.recordId,
    prescriptionNo: dto.prescriptionNo,
    recordType: dto.recordType as RecordType,
    recordTypeLabel: getRecordTypeLabel(dto.recordType),
    patientId: dto.patientId,
    patientName: dto.patientName,
    visitNo: dto.outpatientNo,
    departmentName: dto.departmentName,
    dispenseTime: dto.dispenseTime,
    pharmacyName: dto.pharmacyName,
    dispenserName: dto.dispenserName,
    drugName: dto.drugName,
    spec: dto.spec,
    quantity: dto.quantity,
    unit: dto.unit,
    price: dto.price,
    amount: dto.amount,
    batchNo: dto.batchNo,
    traceCode: dto.traceCode,
    dosage: dto.dosage,
    summaryNo: dto.summaryNo
  }
}

export function adaptInpatientRecord(
  dto: InpatientDispenseRecordItemDTO
): DispenseRecordItem {
  return {
    id: dto.id,
    recordId: dto.recordId,
    prescriptionNo: dto.prescriptionNo,
    recordType: dto.recordType as RecordType,
    recordTypeLabel: getRecordTypeLabel(dto.recordType),
    patientId: dto.patientId,
    patientName: dto.patientName,
    visitNo: dto.inpatientNo,
    departmentName: dto.departmentName,
    wardName: dto.wardName,
    bedNo: dto.bedNo,
    dispenseTime: dto.dispenseTime,
    pharmacyName: dto.pharmacyName,
    dispenserName: dto.dispenserName,
    drugName: dto.drugName,
    spec: dto.spec,
    quantity: dto.quantity,
    unit: dto.unit,
    price: dto.price,
    amount: dto.amount,
    batchNo: dto.batchNo,
    traceCode: dto.traceCode,
    dosage: dto.dosage,
    summaryNo: dto.summaryNo
  }
}