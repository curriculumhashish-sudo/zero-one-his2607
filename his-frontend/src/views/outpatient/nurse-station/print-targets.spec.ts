import { describe, expect, it } from 'vitest'

import type { NurseExecutionItemVO } from '@/apis/outpatient/nurse-station'

import { NURSE_PRINT_OPTIONS, resolveNursePrintTargetIds } from './print-targets'

const rows: NurseExecutionItemVO[] = [
  {
    itemId: 'service',
    requestId: 'service-request',
    requestType: 'SERVICE',
    requestTypeName: '处置',
    orderContent: '换药',
    status: 'PENDING',
    statusName: '待执行',
    plannedCount: 1,
    executedCount: 0,
    version: 0,
    printTypes: ['TREATMENT_SHEET', 'PROCEDURE_APPLICATION']
  },
  {
    itemId: 'medication',
    requestId: 'medication-request',
    requestType: 'MEDICATION',
    requestTypeName: '药品',
    orderContent: '输液药品',
    status: 'PENDING',
    statusName: '待执行',
    plannedCount: 1,
    executedCount: 0,
    version: 0,
    printTypes: ['BOTTLE_LABEL', 'PRESCRIPTION']
  },
  {
    itemId: 'completed',
    requestId: 'completed-request',
    requestType: 'SERVICE',
    requestTypeName: '检验',
    orderContent: '血常规',
    status: 'COMPLETED',
    statusName: '已完成',
    plannedCount: 1,
    executedCount: 1,
    version: 1,
    printTypes: ['SPECIMEN_BARCODE']
  }
]

describe('nurse print targets', () => {
  it('打印入口与原型的七种单据一致', () => {
    expect(NURSE_PRINT_OPTIONS.map((item) => item.label)).toEqual([
      '打印瓶贴',
      '打印处置单',
      '打印采血码',
      '打印处方单',
      '皮试标签',
      '雾化标签',
      '处置申请单'
    ])
  })

  it('未勾选时从全部待执行项目解析目标', () => {
    expect(resolveNursePrintTargetIds(rows, 'PRESCRIPTION')).toEqual(['medication'])
    expect(resolveNursePrintTargetIds(rows, 'SPECIMEN_BARCODE')).toEqual([])
  })

  it('有勾选时只从勾选项目解析目标', () => {
    expect(resolveNursePrintTargetIds(rows, 'SPECIMEN_BARCODE', ['completed'])).toEqual([
      'completed'
    ])
    expect(resolveNursePrintTargetIds(rows, 'PRESCRIPTION', ['service'])).toEqual([])
  })
})
