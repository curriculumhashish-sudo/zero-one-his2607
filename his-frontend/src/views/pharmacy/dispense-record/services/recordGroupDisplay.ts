import type {
  DispenseRecordItem,
  GroupedPrescription,
  GroupedEncounter,
  DisplayMode
} from '../type'

export function groupByPrescription(
  records: DispenseRecordItem[]
): GroupedPrescription[] {
  const map = new Map<string, GroupedPrescription>()

  for (const item of records) {
    const key = `${item.prescriptionNo}_${item.recordType}`
    if (!map.has(key)) {
      map.set(key, {
        key,
        prescriptionNo: item.prescriptionNo,
        recordType: item.recordType,
        recordTypeLabel: item.recordTypeLabel,
        patientName: item.patientName,
        visitNo: item.visitNo,
        departmentName: item.departmentName,
        dispenseTime: item.dispenseTime,
        pharmacyName: item.pharmacyName,
        dispenserName: item.dispenserName,
        summaryNo: item.summaryNo,
        items: []
      })
    }
    map.get(key)!.items.push(item)
  }

  return Array.from(map.values())
}

export function groupByEncounter(
  records: DispenseRecordItem[]
): GroupedEncounter[] {
  const map = new Map<string, GroupedEncounter>()

  for (const item of records) {
    if (!map.has(item.visitNo)) {
      map.set(item.visitNo, {
        key: item.visitNo,
        visitNo: item.visitNo,
        patientName: item.patientName,
        departmentName: item.departmentName,
        wardName: item.wardName,
        bedNo: item.bedNo,
        items: []
      })
    }
    map.get(item.visitNo)!.items.push(item)
  }

  return Array.from(map.values())
}

export function getGroupedData(
  records: DispenseRecordItem[],
  mode: DisplayMode
): GroupedPrescription[] | GroupedEncounter[] | DispenseRecordItem[] {
  switch (mode) {
    case 'prescription':
      return groupByPrescription(records)
    case 'encounter':
      return groupByEncounter(records)
    case 'flat':
    default:
      return records
  }
}

export function getSpanCount(
  mode: DisplayMode,
  record: GroupedPrescription | GroupedEncounter | DispenseRecordItem
): number {
  if (mode === 'flat') return 1
  return 'items' in record ? record.items.length : 1
}

export function isGroupRow(
  mode: DisplayMode,
  row: unknown
): boolean {
  if (mode === 'flat') return false
  return row !== null && typeof row === 'object' && 'items' in (row as object)
}