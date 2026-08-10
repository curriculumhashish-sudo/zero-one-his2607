import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realCancelNurseStationDispenseSummaries,
  realCreateNurseStationDispenseSummary,
  realGetNurseStationDispenseSummaryDetail,
  realQueryNurseStationDispensePending,
  realQueryNurseStationDispenseSummaries,
  realSubmitNurseStationDispenseSummaries
} from './dispense-summary-real'

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站汇总领药真实接口适配', () => {
  it('转换患者、药品分类、长临筛选和后端字段名', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 1,
        patientGroups: [
          {
            encounterId: 'ENC-1',
            patientId: 'PAT-1',
            patientName: '测试患者',
            bedDisplay: '01床',
            orderRows: [
              {
                medReqId: 'REQ-1',
                medicationId: 'MED-1',
                medicationName: '测试药品',
                specificationText: '1盒',
                therapyEnum: 'LONG_TERM',
                therapyEnumText: '长期',
                dosageInstruction: '1片',
                methodCodeText: '口服',
                frequencyCodeText: '每日一次',
                locationId: 'PHARM-1',
                locationName: '住院药房',
                timePoints: [
                  {
                    dispenseId: 'DSP-1',
                    procedureId: 'PROC-1',
                    plannedDispenseTime: '2026-08-10 08:00:00',
                    quantity: 1,
                    unitCode: 'TABLET',
                    unitCodeText: '片',
                    chargeUnitPrice: 2.5
                  }
                ]
              }
            ]
          }
        ]
      }
    })

    const result = await realQueryNurseStationDispensePending({
      pageIndex: 1,
      pageSize: 20,
      encounterIds: ['ENC-1', 'ENC-2'],
      medicineCategory: 'WEST',
      therapyFilter: 'LONG',
      queryDeadline: '2026-08-10 23:59:59'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/medicationcollection/query-summary-order-items',
      expect.objectContaining({
        encounterIds: 'ENC-1,ENC-2',
        medicineCategoryTab: 'west',
        therapyFilter: 'LONG_TERM'
      }),
      expect.any(Object)
    )
    expect(result.patientGroups[0]?.orderRows[0]).toEqual(
      expect.objectContaining({
        medicationRequestId: 'REQ-1',
        therapyType: 'LONG',
        pharmacyName: '住院药房'
      })
    )
  })

  it('创建、发送和撤销仅提交后端支持的最小字段', async () => {
    httpMock.post.mockResolvedValue({ data: { summaryNos: ['MS-1'], message: '处理成功' } })

    await realCreateNurseStationDispenseSummary({
      dispenseItems: [{ dispenseId: 'DSP-1', encounterId: 'ENC-1' }]
    })
    await realSubmitNurseStationDispenseSummaries({ summaryNos: ['MS-1'] })
    await realCancelNurseStationDispenseSummaries({ summaryNos: ['MS-1'] })

    expect(httpMock.post.mock.calls.map(([url, body]) => [url, body])).toEqual([
      [
        '/medicationcollection/summary-medication',
        { dispenseItems: [{ dispenseId: 'DSP-1', encounterId: 'ENC-1' }] }
      ],
      ['/medicationcollection/submit-summary', { summaryNos: ['MS-1'] }],
      ['/medicationcollection/cancel-summary', { summaryNos: ['MS-1'] }]
    ])
  })

  it('按后端列表和明细结构返回页面数据', async () => {
    httpMock.get
      .mockResolvedValueOnce({
        data: {
          pageIndex: 1,
          pageSize: 20,
          total: 1,
          pages: 1,
          rows: [
            {
              summaryNo: 'MS-1',
              statusEnum: 'SUMMARIZED',
              desLocationId: 'WARD-1',
              itemCount: 2,
              plannedDispenseTime: '2026-08-10 08:00:00',
              dispenseTime: ''
            }
          ]
        }
      })
      .mockResolvedValueOnce({
        data: [{ id: 'DSP-1', name: '测试药品', quantity: 2, unitCode: '片' }]
      })

    const page = await realQueryNurseStationDispenseSummaries({ pageIndex: 1, pageSize: 20 })
    const lines = await realGetNurseStationDispenseSummaryDetail('MS-1')

    expect(page.rows?.[0]).toEqual(
      expect.objectContaining({ summaryNo: 'MS-1', statusEnum: 'SUMMARIZED' })
    )
    expect(lines).toEqual([{ id: 'DSP-1', name: '测试药品', quantity: 2, unitCode: '片' }])
    expect(httpMock.get).toHaveBeenLastCalledWith(
      '/medicationcollection/detail',
      { summaryNo: 'MS-1' },
      expect.any(Object)
    )
  })
})
