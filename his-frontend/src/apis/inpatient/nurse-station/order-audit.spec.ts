import { describe, expect, it } from 'vitest'

import {
  queryNurseStationOrderAudits,
  rejectNurseStationOrders,
  verifyNurseStationOrders
} from './index'

describe('住院护士站医嘱校对 Mock API', () => {
  it('按患者、状态、药品分类和长临属性返回稳定分组', async () => {
    const result = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-001', 'encounter-004'],
      auditStatus: 'PENDING',
      medicineCategory: 'WEST',
      therapyType: 'ALL'
    })

    expect(result.groups.map((group) => group.encounterId)).toEqual([
      'encounter-001',
      'encounter-004'
    ])
    expect(result.groups[0]?.orders).toHaveLength(2)
    expect(result.groups[0]?.orders[0]).toEqual(
      expect.objectContaining({
        listRowKey: expect.any(String),
        requestId: expect.any(String),
        version: expect.any(Number)
      })
    )
    expect(result.groups[1]?.orders.every((row) => row.categoryEnum !== 'TCM')).toBe(true)
  })

  it('中药页签返回可整方处理的折叠行', async () => {
    const result = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-004'],
      auditStatus: 'PENDING',
      medicineCategory: 'TCM',
      therapyType: 'ALL'
    })

    expect(result.groups[0]?.orders).toEqual([
      expect.objectContaining({
        tcmFlag: true,
        prescriptionNo: 'TCM20260801001',
        lineRequestIds: expect.arrayContaining(['audit-tcm-line-001'])
      })
    ])
  })

  it('校对通过支持幂等重试并进入已校对列表', async () => {
    const pending = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-007'],
      auditStatus: 'PENDING',
      medicineCategory: 'WEST',
      therapyType: 'ALL'
    })
    const row = pending.groups[0]?.orders[0]
    expect(row).toBeDefined()

    const request = {
      idempotencyKey: 'verify-test-001',
      items: [
        {
          orderType: row!.orderType,
          requestId: row!.requestId,
          expectedVersion: row!.version
        }
      ]
    }
    const first = await verifyNurseStationOrders(request)
    const repeated = await verifyNurseStationOrders(request)
    const passed = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-007'],
      auditStatus: 'PASSED',
      medicineCategory: 'WEST',
      therapyType: 'ALL'
    })

    expect(first).toEqual(expect.objectContaining({ success: true, processedCount: 1 }))
    expect(repeated).toEqual(first)
    expect(passed.groups[0]?.orders.some((item) => item.requestId === row!.requestId)).toBe(true)
  })

  it('退回后从未校对列表移除医嘱', async () => {
    const pending = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-006'],
      auditStatus: 'PENDING',
      medicineCategory: 'WEST',
      therapyType: 'TEMPORARY'
    })
    const row = pending.groups[0]?.orders.find((item) => item.requestId === 'audit-med-010')
    expect(row).toBeDefined()

    const result = await rejectNurseStationOrders({
      idempotencyKey: 'reject-test-001',
      items: [
        {
          orderType: row!.orderType,
          requestId: row!.requestId,
          expectedVersion: row!.version
        }
      ],
      reason: '测试退回'
    })
    const refreshed = await queryNurseStationOrderAudits({
      encounterIds: ['encounter-006'],
      auditStatus: 'PENDING',
      medicineCategory: 'WEST',
      therapyType: 'TEMPORARY'
    })

    expect(result.processedCount).toBe(1)
    expect(refreshed.groups[0]?.orders.some((item) => item.requestId === row!.requestId)).toBe(
      false
    )
  })
})
