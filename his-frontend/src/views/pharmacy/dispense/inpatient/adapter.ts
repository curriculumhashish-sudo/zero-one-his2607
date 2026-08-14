import type { DispensePrescriptionBlock, DispensePrescriptionItemRow, SummarySlipRow } from './type'
import type { InpatientMedicationDispenseSlipDto, MedicationSummarySlipDto } from '@/apis/pharmacy/dispense/inpatient/type'

export function adaptPrescriptionBlocks(dtos: InpatientMedicationDispenseSlipDto[]): DispensePrescriptionBlock[] {
  return dtos.map(dto => ({
    slipId: dto.slipId,
    rxNo: dto.rxNo,
    rxType: dto.rxType,
    status: dto.status,
    items: dto.items.map(item => adaptPrescriptionItemRow(item, dto.rxNo))
  }))
}

function adaptPrescriptionItemRow(item: any, rxNo: string): DispensePrescriptionItemRow {
  return {
    key: item.dispenseId,
    rxNo,
    status: item.status,
    itemName: item.itemName,
    quantity: item.quantity,
    spec: item.spec,
    batchNo: item.batchNo,
    totalPrice: item.totalPrice,
    pharmacy: item.pharmacy,
    traceCode: item.traceCode,
    factory: item.factory,
    sourceStatus: item.sourceStatus,
    dispenseId: item.dispenseId
  }
}

export function adaptSummarySlips(dtos: MedicationSummarySlipDto[]): SummarySlipRow[] {
  return dtos.map(dto => ({
    key: dto.summaryId,
    summaryNo: dto.summaryNo,
    status: dto.status,
    receiverName: dto.receiverName,
    createTime: dto.createTime,
    summaryId: dto.summaryId
  }))
}

export function adaptSummaryDetail(dto: MedicationSummarySlipDto): DispensePrescriptionBlock {
  return {
    slipId: dto.summaryId,
    rxNo: dto.summaryNo,
    rxType: 'western',
    status: dto.status === 'pending' ? 'pending' : 'completed',
    items: dto.items.map(item => ({
      key: item.dispenseId,
      rxNo: dto.summaryNo,
      status: item.status,
      itemName: item.itemName,
      quantity: item.quantity,
      spec: item.spec,
      batchNo: item.batchNo,
      totalPrice: item.totalPrice,
      pharmacy: item.pharmacy,
      traceCode: item.traceCode,
      factory: item.factory,
      sourceStatus: item.sourceStatus,
      dispenseId: item.dispenseId
    }))
  }
}
