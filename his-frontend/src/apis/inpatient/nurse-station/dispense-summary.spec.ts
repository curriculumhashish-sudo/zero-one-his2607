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

  it('相同幂等键不会重复生成汇总单，撤销后恢复原发药时点', async () => {
    const request = {
      idempotencyKey: 'dispense-summary-create-and-cancel-test',
      dispenseItems: [
        { encounterId: 'encounter-002', dispenseId: 'dispense-101' },
        { encounterId: 'encounter-002', dispenseId: 'dispense-102' }
      ]
    }
    const first = await createNurseStationDispenseSummary(request)
    const repeated = await createNurseStationDispenseSummary(request)

    expect(first.processedCount).toBe(2)
    expect(first.summaryNos).toHaveLength(1)
    expect(repeated).toEqual(first)

    const detail = await getNurseStationDispenseSummaryDetail(first.summaryNos[0]!)
    expect(detail.header.itemCount).toBe(2)
    expect(detail.lines.map((item) => item.medicationName)).toEqual([
      '替米沙坦片',
      '布地奈德混悬液'
    ])

    const cancelled = await cancelNurseStationDispenseSummaries({
      idempotencyKey: 'dispense-summary-cancel-test',
      summaries: [{ summaryNo: detail.header.summaryNo, version: detail.header.version }]
    })
    expect(cancelled.processedCount).toBe(1)

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

  it('发送后状态和版本更新，已发送汇总单不能再撤销', async () => {
    const created = await createNurseStationDispenseSummary({
      idempotencyKey: 'dispense-summary-submit-create-test',
      dispenseItems: [{ encounterId: 'encounter-001', dispenseId: 'dispense-004' }]
    })
    const summaryNo = created.summaryNos[0]!
    const detail = await getNurseStationDispenseSummaryDetail(summaryNo)
    const submitted = await submitNurseStationDispenseSummaries({
      idempotencyKey: 'dispense-summary-submit-test',
      summaries: [{ summaryNo, version: detail.header.version }]
    })

    expect(submitted.processedCount).toBe(1)
    const submittedDetail = await getNurseStationDispenseSummaryDetail(summaryNo)
    expect(submittedDetail.header).toEqual(
      expect.objectContaining({ status: 'SUBMITTED', statusText: '已发送', version: 2 })
    )

    const cancelResult = await cancelNurseStationDispenseSummaries({
      idempotencyKey: 'dispense-summary-cancel-submitted-test',
      summaries: [{ summaryNo, version: submittedDetail.header.version }]
    })
    expect(cancelResult.processedCount).toBe(0)
    expect(cancelResult.results[0]?.message).toContain('仅待发送')
  })

  it('汇总单列表支持状态分页，错误版本不会覆盖新状态', async () => {
    const page = await queryNurseStationDispenseSummaries({
      pageIndex: 1,
      pageSize: 20,
      status: 'PENDING_SUBMIT'
    })
    expect(page.rows?.some((item) => item.summaryNo === 'MS-HISTORY-0001')).toBe(true)

    const result = await submitNurseStationDispenseSummaries({
      idempotencyKey: 'dispense-summary-wrong-version-test',
      summaries: [{ summaryNo: 'MS-HISTORY-0001', version: 99 }]
    })
    expect(result.processedCount).toBe(0)
    expect(result.results[0]?.message).toContain('已更新')
  })
})
