import { describe, expect, it } from 'vitest'

import {
  cancelNurseStationExecutions,
  executeNurseStationOrders,
  queryNurseStationOrderExecutions,
  skipNurseStationOrders
} from './index'

const queryDeadline = '2026-08-02 23:59:59'

describe('住院护士站医嘱执行 Mock API', () => {
  it('按患者和组合筛选返回计划时点分组', async () => {
    const result = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-001', 'encounter-004'],
      executeStatus: 'PENDING',
      therapyType: 'ALL',
      medicineCategory: 'WEST',
      executionDeadline: queryDeadline
    })

    expect(result.groups.map((group) => group.encounterId)).toEqual([
      'encounter-001',
      'encounter-004'
    ])
    expect(result.groups[0]?.orders).toHaveLength(2)
    expect(result.groups[0]?.orders[0]).toEqual(
      expect.objectContaining({
        orderId: expect.any(String),
        slots: expect.arrayContaining([
          expect.objectContaining({ status: 'PENDING', planTime: expect.any(String) })
        ])
      })
    )
  })

  it('中药页签返回整方执行所需的处方信息', async () => {
    const result = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-004'],
      executeStatus: 'PENDING',
      therapyType: 'ALL',
      medicineCategory: 'TCM',
      executionDeadline: queryDeadline
    })

    expect(result.groups[0]?.orders).toEqual([
      expect.objectContaining({
        tcmFlag: true,
        prescriptionNo: 'TCM20260801001',
        lineRequestIds: expect.arrayContaining(['execute-tcm-line-001'])
      })
    ])
  })

  it('执行计划点支持幂等重试并进入已执行列表', async () => {
    const request = {
      idempotencyKey: 'execute-spec-001',
      items: [
        {
          orderId: 'execute-med-007',
          orderType: 'MED_REQUEST' as const,
          encounterId: 'encounter-007',
          planTime: '2026-08-01 14:30:00',
          accountId: 'account-007',
          actualTime: '2026-08-01 14:32:00'
        }
      ]
    }

    const first = await executeNurseStationOrders(request)
    const repeated = await executeNurseStationOrders(request)
    const executed = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-007'],
      executeStatus: 'EXECUTED',
      therapyType: 'ALL',
      medicineCategory: 'WEST',
      executionDeadline: queryDeadline
    })

    expect(first).toEqual(expect.objectContaining({ success: true, processedCount: 1 }))
    expect(repeated).toEqual(first)
    expect(executed.groups[0]?.orders[0]?.slots).toEqual([
      expect.objectContaining({
        planTime: '2026-08-01 14:30:00',
        procedureId: expect.any(String),
        status: 'EXECUTED'
      })
    ])
  })

  it('不执行会把待执行计划点迁移到不执行列表', async () => {
    const result = await skipNurseStationOrders({
      idempotencyKey: 'skip-spec-001',
      reason: '患者外出检查',
      items: [
        {
          orderId: 'execute-med-004',
          orderType: 'MED_REQUEST',
          encounterId: 'encounter-004',
          planTime: '2026-08-01 21:00:00'
        }
      ]
    })
    const skipped = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-004'],
      executeStatus: 'SKIPPED',
      therapyType: 'ALL',
      medicineCategory: 'WEST',
      executionDeadline: queryDeadline
    })

    expect(result.processedCount).toBe(1)
    expect(skipped.groups[0]?.orders[0]?.slots[0]).toEqual(
      expect.objectContaining({ planTime: '2026-08-01 21:00:00', status: 'SKIPPED' })
    )
  })

  it('取消执行必须使用执行记录 ID 并返回重建汇总单号', async () => {
    const result = await cancelNurseStationExecutions({
      idempotencyKey: 'cancel-spec-001',
      reason: '患者暂缓执行',
      items: [
        {
          encounterId: 'encounter-001',
          procedureId: 'procedure-executed-001'
        }
      ]
    })
    const cancelled = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-001'],
      executeStatus: 'CANCELLED',
      therapyType: 'ALL',
      medicineCategory: 'WEST',
      executionDeadline: queryDeadline
    })

    expect(result).toEqual(
      expect.objectContaining({
        success: true,
        processedCount: 1,
        rebuiltSummaryNos: expect.arrayContaining([expect.any(String)])
      })
    )
    expect(
      cancelled.groups[0]?.orders.some((order) =>
        order.slots.some((slot) => slot.procedureId === 'procedure-executed-001')
      )
    ).toBe(true)
  })
})
