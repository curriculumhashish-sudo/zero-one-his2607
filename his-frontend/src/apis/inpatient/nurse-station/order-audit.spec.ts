import { describe, expect, it } from 'vitest'

import {
  queryNurseStationOrderAudits,
  rejectNurseStationOrders,
  verifyNurseStationOrders
} from './index'

const patient = (encounterId: string) => ({
  encounterId,
  patientId: `patient-${encounterId}`,
  patientName: `Patient ${encounterId}`
})

describe('住院护士站医嘱校对 Mock API', () => {
  it('按患者和校对状态返回最小医嘱字段', async () => {
    const result = await queryNurseStationOrderAudits({
      patientContexts: [patient('encounter-001'), patient('encounter-004')],
      auditStatus: 'PENDING'
    })

    expect(result.groups.map((group) => group.encounterId)).toEqual([
      'encounter-001',
      'encounter-004'
    ])
    expect(result.groups[0]?.orders).toHaveLength(2)
    expect(result.groups[0]?.orders[0]).toEqual({
      listRowKey: 'MED_REQUEST:audit-med-001',
      requestId: 'audit-med-001',
      orderType: 'MED_REQUEST',
      content: '注射用头孢曲松钠'
    })
    expect(result.groups[1]?.orders).toHaveLength(0)
  })

  it('校对通过后进入已校对列表', async () => {
    const pending = await queryNurseStationOrderAudits({
      patientContexts: [patient('encounter-007')],
      auditStatus: 'PENDING'
    })
    const row = pending.groups[0]?.orders[0]
    expect(row).toBeDefined()

    const result = await verifyNurseStationOrders({
      items: [{ orderType: row!.orderType, requestId: row!.requestId }]
    })
    const passed = await queryNurseStationOrderAudits({
      patientContexts: [patient('encounter-007')],
      auditStatus: 'PASSED'
    })

    expect(result).toEqual({ success: true, processedCount: 1 })
    expect(passed.groups[0]?.orders.some((item) => item.requestId === row!.requestId)).toBe(true)
  })

  it('退回原因必填且退回后从未校对列表移除', async () => {
    const pending = await queryNurseStationOrderAudits({
      patientContexts: [patient('encounter-006')],
      auditStatus: 'PENDING'
    })
    const row = pending.groups[0]?.orders[0]
    expect(row).toBeDefined()

    await expect(
      rejectNurseStationOrders({
        items: [{ orderType: row!.orderType, requestId: row!.requestId }],
        reason: '   '
      })
    ).rejects.toThrow('退回原因不能为空')

    const result = await rejectNurseStationOrders({
      items: [{ orderType: row!.orderType, requestId: row!.requestId }],
      reason: 'Patient information is incomplete'
    })
    const refreshed = await queryNurseStationOrderAudits({
      patientContexts: [patient('encounter-006')],
      auditStatus: 'PENDING'
    })

    expect(result).toEqual({ success: true, processedCount: 1 })
    expect(refreshed.groups[0]?.orders).toHaveLength(0)
  })
})
