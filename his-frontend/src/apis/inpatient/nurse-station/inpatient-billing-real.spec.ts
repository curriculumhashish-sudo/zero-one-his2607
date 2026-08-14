import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realCancelNurseStationBilling,
  realConfirmNurseStationBilling,
  realDeleteNurseStationAutoBillingRule,
  realQueryNurseStationAutoBillingRules,
  realQueryNurseStationBillingCatalog,
  realQueryNurseStationPricedOrders,
  realQueryNurseStationSupplementOrders,
  realSaveNurseStationAutoBillingRule
} from './inpatient-billing-real'

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站住院记账真实接口适配', () => {
  it('映射目录分页参数，并把缺少收费字段的项目标记为只读', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 10,
        total: 2,
        rows: [
          {
            productDefinitionId: 'device-001',
            name: '一次性输液器',
            unitPrice: 6.8,
            categoryCode: 'MATERIAL',
            categoryCodeText: '耗材',
            unitCode: 'SET',
            unitCodeText: '套',
            specificationText: '带针'
          },
          {
            productDefinitionId: 'drug-001',
            name: '不应直接划价的药品',
            categoryCode: 'DRUG',
            unitCode: 'BOX'
          }
        ]
      }
    })

    const result = await realQueryNurseStationBillingCatalog({
      pageIndex: 1,
      pageSize: 10,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      category: 'DEVICE',
      keyword: '输液'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/inpatientbilling/page-pricing-catalog',
      expect.objectContaining({
        pageIndex: 1,
        pageSize: 10,
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        keyword: '输液',
        categoryCode: 'DEVICE'
      }),
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result.rows).toHaveLength(1)
    expect(result.rows?.[0]).toEqual(
      expect.objectContaining({
        category: 'DEVICE',
        writable: false,
        missingFields: expect.arrayContaining(['chargeItemDefinitionId'])
      })
    )
  })

  it('直接划价和医嘱补费分别调用两个接口，并转换后端请求字段', async () => {
    httpMock.post.mockResolvedValue({
      data: { success: true, processedCount: 1, createdServiceRequestIds: ['SR-001'] }
    })
    const baseItem = {
      clientItemId: 'client-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      accountId: 'account-001',
      productTableName: 'device_definition',
      productDefinitionId: 'device-001',
      chargeItemDefinitionId: 'charge-001',
      quantity: 2,
      unitCode: 'SET',
      previewUnitPrice: 6.8
    }

    const direct = await realConfirmNurseStationBilling({
      idempotencyKey: 'local-direct-key',
      mode: 'DIRECT',
      items: [baseItem]
    })
    expect(httpMock.post.mock.calls[0]?.[0]).toBe('/inpatientbilling/confirm-direct-pricing')
    expect(httpMock.post.mock.calls[0]?.[1]).toEqual({
      items: [expect.objectContaining({ unitPrice: 6.8, totalPrice: 13.6, enabled: true })]
    })
    expect(direct.results[0]).toEqual(
      expect.objectContaining({ requestId: 'SR-001', finalTotalPrice: 13.6 })
    )

    await realConfirmNurseStationBilling({
      idempotencyKey: 'local-supplement-key',
      mode: 'SUPPLEMENT',
      items: [
        { ...baseItem, parentOrderAnchor: { orderType: 'SERVICE_REQUEST', requestId: 'ORDER-001' } }
      ]
    })
    expect(httpMock.post.mock.calls[1]?.[0]).toBe('/inpatientbilling/confirm-supplement-pricing')
    expect(httpMock.post.mock.calls[1]?.[1]).toEqual({
      items: [expect.objectContaining({ parentRequestAnchor: 'ORDER_ORDER-001' })]
    })
  })

  it('按患者分组转换父医嘱和已划价列表，并降级为只读字段', async () => {
    httpMock.get
      .mockResolvedValueOnce({
        data: {
          groups: [
            {
              encounterId: 'encounter-001',
              orders: [
                {
                  orderId: 'order-001',
                  orderName: '雾化治疗',
                  therapyType: 'TEMPORARY',
                  frequencyCodeText: '立即'
                }
              ]
            }
          ]
        }
      })
      .mockResolvedValueOnce({
        data: {
          groups: [
            {
              encounterId: 'encounter-001',
              pricedOrders: [
                {
                  serviceRequestId: 'SR-001',
                  productName: '输液器',
                  quantity: 1,
                  unitCode: '套',
                  unitPrice: 6.8,
                  totalPrice: 6.8,
                  pricedTime: '2026-08-08 09:00:00'
                }
              ]
            }
          ]
        }
      })

    const parents = await realQueryNurseStationSupplementOrders({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      therapyType: 'ALL'
    })
    expect(parents[0]).toEqual(
      expect.objectContaining({
        requestId: 'order-001',
        adviceDisplayName: '雾化治疗',
        therapyType: 'TEMP'
      })
    )

    const priced = await realQueryNurseStationPricedOrders({
      encounterId: 'encounter-001',
      patientId: 'patient-001'
    })
    expect(priced[0]).toEqual(
      expect.objectContaining({
        requestId: 'SR-001',
        totalPrice: 6.8,
        cancellable: false,
        backendFieldsIncomplete: true
      })
    )
  })

  it('自动规则只映射和提交后端当前支持的字段', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        configs: [
          {
            ruleId: 'R-001',
            ruleName: '晨间计费',
            activityId: 'A-001',
            chargeDefinitionId: 'C-001',
            quantityValue: 1,
            isActive: true
          }
        ]
      }
    })
    httpMock.post
      .mockResolvedValueOnce({ data: { ruleId: 'R-002' } })
      .mockResolvedValueOnce({ data: { success: true } })

    const list = await realQueryNurseStationAutoBillingRules({
      encounterId: 'encounter-001',
      patientId: 'patient-001'
    })
    expect(list.rules[0]).toEqual(
      expect.objectContaining({ ruleId: 'R-001', status: 'ACTIVE', quantity: 1 })
    )

    await realSaveNurseStationAutoBillingRule({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleName: '新规则',
      activityId: 'A-002',
      chargeDefinitionId: 'C-002',
      quantity: 2
    })
    expect(httpMock.post.mock.calls[0]?.[0]).toBe('/inpatientbilling/create-auto-gen-rule')
    expect(httpMock.post.mock.calls[0]?.[1]).toEqual({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleName: '新规则',
      activityId: 'A-002',
      chargeDefinitionId: 'C-002',
      quantityValue: 2
    })

    await realDeleteNurseStationAutoBillingRule({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleId: 'R-001'
    })
    expect(httpMock.post.mock.calls[1]?.[0]).toBe('/inpatientbilling/delete-auto-gen-rule')
    expect(httpMock.post).toHaveBeenCalledTimes(2)
  })

  it('取消划价只发送后端当前支持的字段', async () => {
    httpMock.post.mockResolvedValue({
      data: { success: true, processedCount: 1, cancelledServiceRequestIds: ['SR-001'] }
    })
    const result = await realCancelNurseStationBilling({
      idempotencyKey: 'cancel',
      reason: '误操作',
      items: [{ requestId: 'SR-001', orderType: 'SERVICE_REQUEST', encounterId: 'encounter-001' }]
    })
    expect(httpMock.post).toHaveBeenCalledWith(
      '/inpatientbilling/cancel-pricing',
      { items: [{ orderType: 'DIRECT', requestId: 'SR-001', encounterId: 'encounter-001' }] },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual(expect.objectContaining({ success: true, processedCount: 1 }))
  })

  it('真实请求失败时直接抛错，不伪造 Mock 数据', async () => {
    httpMock.get.mockRejectedValue(new Error('网络不可用'))
    await expect(
      realQueryNurseStationPricedOrders({ encounterId: 'encounter-001', patientId: 'patient-001' })
    ).rejects.toThrow('网络不可用')
  })
})
