import type { NurseExecutionItemVO, NursePrintDocumentType } from '@/apis/outpatient/nurse-station'

/** 原型护士站提供的打印入口及展示顺序。 */
export const NURSE_PRINT_OPTIONS: ReadonlyArray<{
  type: NursePrintDocumentType
  label: string
}> = [
  { type: 'BOTTLE_LABEL', label: '打印瓶贴' },
  { type: 'TREATMENT_SHEET', label: '打印处置单' },
  { type: 'SPECIMEN_BARCODE', label: '打印采血码' },
  { type: 'PRESCRIPTION', label: '打印处方单' },
  { type: 'SKIN_TEST_LABEL', label: '皮试标签' },
  { type: 'NEBULIZATION_LABEL', label: '雾化标签' },
  { type: 'PROCEDURE_APPLICATION', label: '处置申请单' }
]

/** 打印类型无目标时的业务提示。 */
export const NURSE_PRINT_EMPTY_MESSAGES: Record<NursePrintDocumentType, string> = {
  BOTTLE_LABEL: '没有可打印的输液医嘱',
  TREATMENT_SHEET: '没有可打印的处置项目',
  SPECIMEN_BARCODE: '没有可打印的采血/检验医嘱',
  PRESCRIPTION: '没有可打印的药品医嘱',
  SKIN_TEST_LABEL: '没有可打印的皮试医嘱',
  NEBULIZATION_LABEL: '没有可打印的雾化/吸入医嘱',
  PROCEDURE_APPLICATION: '没有可打印的治疗/处置医嘱'
}

/**
 * 从执行项目解析打印目标。
 *
 * 有勾选时仅检查勾选行；未勾选时回落到全部待执行项目，与原型交互一致。
 */
export function resolveNursePrintTargetIds(
  rows: NurseExecutionItemVO[],
  documentType: NursePrintDocumentType,
  selectedIds?: string[]
): string[] {
  const selectedSet = selectedIds?.length ? new Set(selectedIds) : null
  const candidates = selectedSet
    ? rows.filter((row) => selectedSet.has(row.itemId))
    : rows.filter((row) => row.status === 'PENDING')

  return candidates.filter((row) => row.printTypes.includes(documentType)).map((row) => row.itemId)
}
