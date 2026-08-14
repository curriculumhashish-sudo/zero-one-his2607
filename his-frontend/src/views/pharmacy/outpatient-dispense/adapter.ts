// src/views/pharmacy/outpatient-dispense/adapter.ts

import type {
  DispenseQueuePatient,
  DispensePrescriptionBlock,
  DispensePrescriptionItemRow,
} from './type'

/** ===== 后端队列数据 → 页面患者 ===== */
export function toDispensePatient(apiData: any): DispenseQueuePatient {
  return {
    id: apiData.encounterId,
    patientId: apiData.patientId,
    patientName: apiData.patientName || '',
    genderText: apiData.genderText || '未知',
    ageText: apiData.ageText || '',
    visitNo: apiData.visitNo || '',
    status: apiData.dispenseStatus === 'COMPLETED' ? 'completed' : 'pending',
    prescriptionCount: apiData.prescriptionCount || 0,
    registerTime: apiData.registerTime || '',
    name: apiData.patientName || '',
    gender: apiData.genderText || '未知',
    age: apiData.ageText || '',
  }
}

/** ===== 后端发放行 → 页面处方块 ===== */
export function toPrescriptionBlocks(
  slipRows: any[],
): DispensePrescriptionBlock[] {
  if (!slipRows || slipRows.length === 0) return []

  const groupMap = new Map<string, any[]>()
  for (const row of slipRows) {
    const key = row.prescriptionNo || row.dispenseId
    if (!groupMap.has(key)) {
      groupMap.set(key, [])
    }
    groupMap.get(key)!.push(row)
  }

  const blocks: DispensePrescriptionBlock[] = []
  for (const [rxNo, rows] of groupMap) {
    const status = inferBlockStatus(rows)
    blocks.push({
      id: rxNo,
      rxNo,
      status,
      items: rows.map((row, index) => toPrescriptionItemRow(row, index, rows.length)),
    })
  }

  return blocks
}

/** ===== 推断处方块状态 ===== */
function inferBlockStatus(rows: any[]): 'pending' | 'partial' | 'completed' | 'blocking' {
  const statuses = rows.map((r) => r.statusEnum ?? 'PENDING')
  if (statuses.every((s) => s === 'COMPLETED')) return 'completed'
  if (statuses.every((s) => s === 'PENDING' || s === 'PREPARATION' || s === 'IN_PROGRESS')) return 'pending'
  if (statuses.some((s) => s === 'BLOCKED' || s === 'ERROR')) return 'blocking'
  return 'partial'
}

/** ===== 后端发放行 → 页面明细行 ===== */
function toPrescriptionItemRow(
  row: any,
  index: number,
  total: number,
): DispensePrescriptionItemRow {
  return {
    id: row.dispenseId,
    rxId: row.prescriptionNo || row.dispenseId,
    rxNo: row.prescriptionNo || '',
    isFirstInRx: index === 0,
    isLastInRx: index === total - 1,
    rxRowSpan: total,
    itemName: row.medicationName || '',
    spec: row.specText || '',
    quantity: row.quantity || 0,
    unit: row.unitName || '',
    dosage: row.dosageText || '',
    frequency: row.frequencyText || '',
    route: row.routeText || '',
    days: row.days || 0,
    totalPrice: row.totalPrice || 0,
    status: inferRowStatus(row.statusEnum),
    pharmacy: row.pharmacyName || '',
    batchNo: row.lotNumber || '',
    traceCode: row.traceNo || '',
    factory: row.factoryName || '',
    inventoryPickOptions: buildInventoryOptions(row.inventoryDetailList),
    key: row.dispenseId,
    name: row.medicationName || '',
    specification: row.specText || '',
    dose: row.dosageText || '',
  }
}

/** ===== 推断行状态 ===== */
function inferRowStatus(statusEnum: string): 'pending' | 'partial' | 'completed' | 'blocking' {
  if (statusEnum === 'COMPLETED') return 'completed'
  if (statusEnum === 'BLOCKED' || statusEnum === 'ERROR') return 'blocking'
  if (statusEnum === 'PARTIAL') return 'partial'
  return 'pending'
}

/** ===== 构建库房选择选项 ===== */
function buildInventoryOptions(inventoryList: any[]): { label: string; value: string }[] {
  if (!inventoryList || inventoryList.length === 0) return []
  return inventoryList.map((item) => ({
    label: `${item.pharmacyName || item.pharmacyOrganizationId} (批次: ${item.lotNumber})`,
    value: encodeInventoryPick(item.pharmacyOrganizationId, item.lotNumber),
  }))
}

/** ===== 编码库房选择 ===== */
export function encodeInventoryPick(pharmacyOrganizationId: string, lotNumber: string): string {
  return `${pharmacyOrganizationId}|${lotNumber}`
}

/** ===== 解码库房选择 ===== */
export function decodeInventoryPick(value: string): { pharmacyOrganizationId: string; lotNumber: string } {
  const parts = value.split('|')
  return {
    pharmacyOrganizationId: parts[0] || '',
    lotNumber: parts[1] || '',
  }
}

/** ===== 计算处方汇总 ===== */
export function buildPrescriptionSummary(
  blocks: DispensePrescriptionBlock[],
  dispenserName: string = '',
): {
  prescriptionCount: number
  totalAmount: number
  drugCount: number
  dispenserName: string
} {
  let totalAmount = 0
  let drugCount = 0

  for (const block of blocks) {
    for (const item of block.items) {
      totalAmount += item.totalPrice || 0
      drugCount += 1
    }
  }

  return {
    prescriptionCount: blocks.length,
    totalAmount,
    drugCount,
    dispenserName,
  }
}
