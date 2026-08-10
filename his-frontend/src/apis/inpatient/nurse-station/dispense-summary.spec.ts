import dayjs from 'dayjs'
import { describe, expect, it } from 'vitest'

import {
  cancelNurseStationDispenseSummaries,
  createNurseStationDispenseSummary,
  getNurseStationDispenseSummaryDetail,
  queryNurseStationDispensePending,
  queryNurseStationDispenseSummaries,
  submitNurseStationDispenseSummaries
} from './index'

const deadline = dayjs().endOf('day').format('YYYY-MM-DD HH:mm:ss')

describe('住院护士站汇总领药 Mock API', () => {
  it('按多患者、中西药和长临条件返回患者分组与可勾选时点总数', async () => {
    const result = await queryNurseStationDispensePending({
      pageIndex: 1,
      pageSize: 20,
      encounterIds: ['encounter-001', 'encounter-002'],
      medicineCategory: 'WEST',
      therapyFilter: 'LONG',
      queryDeadline: deadline
    })

    expect(result.total).toBe(3)
    expect(result.patientGroups).toHaveLength(2)
    expect(result.patientGroups[0]).toEqual(
      expect.objectContaining({ patientName: '孙晓梅', bedDisplay: '301病房 01床' })
    )
    expect(result.patientGroups[0]?.orderRows[0]?.timePoints).toHaveLength(2)
  })

  it('创建和撤销使用后端最小契约，并在撤销后恢复发药时点', async () => {
    const created = await createNurseStationDispenseSummary({
      dispenseItems: [
        { encounterId: 'encounter-002', dispenseId: 'dispense-101' },
        { encounterId: 'encounter-002', dispenseId: 'dispense-102' }
      ]
    })

    expect(created.summaryNos).toHaveLength(1)
    const summaryNo = created.summaryNos[0]!
    const detail = await getNurseStationDispenseSummaryDetail(summaryNo)
    expect(detail.map((item) => item.name)).toEqual(['替米沙坦片', '布地奈德混悬液'])

    const cancelled = await cancelNurseStationDispenseSummaries({ summaryNos: [summaryNo] })
    expect(cancelled.summaryNos).toEqual([summaryNo])

    const pendingAgain = await queryNurseStationDispensePending({
      pageIndex: 1,
      pageSize: 20,
      encounterIds: ['encounter-002'],
      medicineCategory: 'WEST',
      therapyFilter: 'ALL',
      queryDeadline: deadline
    })
    expect(pendingAgain.total).toBe(2)
  })

  it('发送后状态改为待药房处理，不能再撤销', async () => {
    const created = await createNurseStationDispenseSummary({
      dispenseItems: [{ encounterId: 'encounter-001', dispenseId: 'dispense-004' }]
    })
    const summaryNo = created.summaryNos[0]!
    const submitted = await submitNurseStationDispenseSummaries({ summaryNos: [summaryNo] })

    expect(submitted.summaryNos).toEqual([summaryNo])
    const page = await queryNurseStationDispenseSummaries({
      pageIndex: 1,
      pageSize: 20,
      statusEnum: 'WAIT_REVIEW'
    })
    expect(page.rows?.some((item) => item.summaryNo === summaryNo)).toBe(true)

    await expect(cancelNurseStationDispenseSummaries({ summaryNos: [summaryNo] })).rejects.toThrow(
      '仅待发送汇总单可操作'
    )
  })

  it('汇总单列表按后端状态字段分页', async () => {
    const page = await queryNurseStationDispenseSummaries({
      pageIndex: 1,
      pageSize: 20,
      statusEnum: 'SUMMARIZED'
    })
    expect(page.rows?.some((item) => item.summaryNo === 'MS-HISTORY-0001')).toBe(true)
  })
})
