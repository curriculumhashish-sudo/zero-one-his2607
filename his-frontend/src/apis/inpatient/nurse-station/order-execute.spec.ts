import { describe, expect, it } from 'vitest'

import { executeNurseStationOrders, queryNurseStationOrderExecutions } from './index'

describe('住院护士站医嘱执行 Mock API', () => {
  it('按所选患者返回已校对诊疗医嘱', async () => {
    const result = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-001', 'encounter-004']
    })

    expect(result.groups.map((group) => group.encounterId)).toEqual([
      'encounter-001',
      'encounter-004'
    ])
    expect(result.groups[0]?.orders).toHaveLength(2)
    expect(result.groups[0]?.orders[0]).toEqual(
      expect.objectContaining({
        id: expect.any(String),
        statusEnum: 'verified',
        encounterId: 'encounter-001'
      })
    )
  })

  it('开始执行后从已校对列表移除医嘱', async () => {
    const result = await executeNurseStationOrders({
      performerId: 'nurse-001',
      serviceRequestIds: ['execute-service-006']
    })
    const pending = await queryNurseStationOrderExecutions({
      encounterIds: ['encounter-006']
    })

    expect(result).toEqual(expect.objectContaining({ success: true, processedCount: 1 }))
    expect(pending.groups[0]?.orders).toHaveLength(0)
  })

  it('不再接受药品、计划时点和取消执行字段', async () => {
    const result = await queryNurseStationOrderExecutions({ encounterIds: ['encounter-004'] })
    const order = result.groups[0]?.orders[0]

    expect(order).toBeDefined()
    expect(order).not.toHaveProperty('slots')
    expect(order).not.toHaveProperty('bindItems')
    expect(order).not.toHaveProperty('medicineCategory')
    expect(order).not.toHaveProperty('procedureId')
  })
})
