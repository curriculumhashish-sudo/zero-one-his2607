import { describe, expect, it } from 'vitest'

import {
  getNurseStationPatientStatistics,
  loadNurseStationWardTree,
  pageNurseStationPatientQueue
} from './index'

describe('住院护士站患者管理 Mock API', () => {
  it('返回包含稳定 ID 的病区病房树', async () => {
    const tree = await loadNurseStationWardTree()

    expect(tree.wardNodes).toHaveLength(3)
    expect(tree.wardNodes[0]).toEqual(
      expect.objectContaining({
        wardId: 'ward-internal',
        wardName: '内科疗区',
        houseNodes: expect.arrayContaining([
          expect.objectContaining({ houseId: 'house-301', houseName: '301病房' })
        ])
      })
    )
  })

  it('患者统计和队列使用相同筛选口径', async () => {
    const query = {
      wardIds: 'ward-internal,ward-pediatrics',
      nurseStationTaskFilter: 'pending-execute' as const
    }
    const [statistics, page] = await Promise.all([
      getNurseStationPatientStatistics(query),
      pageNurseStationPatientQueue({ ...query, pageIndex: 1, pageSize: 50 })
    ])

    expect(page.rows).toHaveLength(3)
    expect(statistics).toEqual({
      totalCount: page.total,
      pendingAuditCount: 3,
      pendingExecuteCount: 13
    })
  })

  it('支持多病房、关键字和已出院结算范围', async () => {
    const housePage = await pageNurseStationPatientQueue({
      pageIndex: 1,
      pageSize: 50,
      houseIds: 'house-401,house-501',
      keyword: '何雨桐'
    })
    const settledPage = await pageNurseStationPatientQueue({
      pageIndex: 1,
      pageSize: 50,
      nurseStationStatusFilter: 'SETTLED_DISCHARGED'
    })

    expect(housePage.rows?.map((row) => row.patientName)).toEqual(['何雨桐'])
    expect(housePage.rows?.[0]?.houseId).toBe('house-501')
    expect(settledPage.total).toBe(2)
    expect(settledPage.rows?.every((row) => row.recallEnabled)).toBe(true)
  })

  it('正确执行分页并保持总数', async () => {
    const firstPage = await pageNurseStationPatientQueue({ pageIndex: 1, pageSize: 3 })
    const secondPage = await pageNurseStationPatientQueue({ pageIndex: 2, pageSize: 3 })

    expect(firstPage.total).toBe(8)
    expect(firstPage.pages).toBe(3)
    expect(firstPage.rows).toHaveLength(3)
    expect(secondPage.rows).toHaveLength(3)
    expect(firstPage.rows?.map((row) => row.encounterId)).not.toEqual(
      secondPage.rows?.map((row) => row.encounterId)
    )
  })
})
