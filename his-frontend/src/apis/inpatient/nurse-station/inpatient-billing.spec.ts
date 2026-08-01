import { describe, expect, it } from 'vitest'

import {
  cancelNurseStationBilling,
  confirmNurseStationBilling,
  queryNurseStationAutoBillingRules,
  queryNurseStationBillingCatalog,
  queryNurseStationPricedOrders,
  saveNurseStationAutoBillingRule,
  setNurseStationAutoBillingRuleStatus
} from './index'

describe('住院护士站住院记账 Mock API', () => {
  it('目录只返回指定类别，并包含划价所需稳定标识', async () => {
    const result = await queryNurseStationBillingCatalog({
      pageIndex: 1,
      pageSize: 10,
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      category: 'DEVICE'
    })

    expect(result.rows?.length).toBeGreaterThan(0)
    expect(result.rows?.every((item) => item.category === 'DEVICE')).toBe(true)
    expect(result.rows?.[0]).toEqual(
      expect.objectContaining({
        productTableName: expect.any(String),
        productDefinitionId: expect.any(String),
        chargeItemDefinitionId: expect.any(String),
        unitBands: expect.any(Array),
        unitPrice: expect.any(Number)
      })
    )
  })

  it('直接划价支持幂等重试，并按服务端目录价格计算金额', async () => {
    const request = {
      idempotencyKey: 'billing-confirm-spec-001',
      mode: 'DIRECT' as const,
      items: [
        {
          clientItemId: 'client-billing-spec-001',
          encounterId: 'encounter-001',
          patientId: 'patient-001',
          accountId: 'account-001',
          productTableName: 'healthcare_service',
          productDefinitionId: 'service-nebulization',
          chargeItemDefinitionId: 'charge-nebulization',
          quantity: 2,
          unitCode: 'TIME',
          previewUnitPrice: 1
        }
      ]
    }

    const first = await confirmNurseStationBilling(request)
    const repeated = await confirmNurseStationBilling(request)
    const rows = await queryNurseStationPricedOrders({
      encounterId: 'encounter-001',
      patientId: 'patient-001'
    })

    expect(first).toEqual(expect.objectContaining({ success: true, processedCount: 1 }))
    expect(repeated).toEqual(first)
    expect(first.results[0]).toEqual(
      expect.objectContaining({ finalUnitPrice: 12, finalTotalPrice: 24 })
    )
    expect(rows.find((row) => row.requestId === first.results[0]?.requestId)).toEqual(
      expect.objectContaining({ productName: '雾化吸入治疗', totalPrice: 24 })
    )
  })

  it('医嘱补费缺少结构化父医嘱锚点时逐项返回失败', async () => {
    const result = await confirmNurseStationBilling({
      idempotencyKey: 'billing-supplement-anchor-spec-001',
      mode: 'SUPPLEMENT',
      items: [
        {
          clientItemId: 'client-billing-spec-002',
          encounterId: 'encounter-001',
          patientId: 'patient-001',
          accountId: 'account-001',
          productTableName: 'device_definition',
          productDefinitionId: 'device-dressing',
          chargeItemDefinitionId: 'charge-dressing',
          quantity: 1,
          unitCode: 'PCS',
          previewUnitPrice: 4.2
        }
      ]
    })

    expect(result).toEqual(expect.objectContaining({ success: false, processedCount: 0 }))
    expect(result.results[0]?.message).toBe('医嘱补费必须关联父医嘱')
  })

  it('取消划价会拒绝已收费项目并返回逐项原因', async () => {
    const result = await cancelNurseStationBilling({
      idempotencyKey: 'billing-cancel-charged-spec-001',
      reason: '测试取消',
      items: [
        {
          requestId: 'billing-charged-001',
          orderType: 'SERVICE_REQUEST',
          encounterId: 'encounter-001'
        }
      ]
    })

    expect(result).toEqual(expect.objectContaining({ success: false, processedCount: 0 }))
    expect(result.results[0]?.message).toBe('已收费项目需要走退费流程')
  })

  it('自动计费规则使用版本校验并显式设置目标状态', async () => {
    const created = await saveNurseStationAutoBillingRule({
      idempotencyKey: 'auto-billing-create-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleName: '测试雾化自动计费',
      activityId: 'activity-nebulization-spec',
      activityName: '雾化吸入治疗',
      chargeDefinitionId: 'charge-nebulization',
      chargeDefinitionName: '雾化吸入治疗费',
      quantity: 1,
      priority: 20
    })
    const rule = created.rule
    expect(rule).toBeDefined()

    const changed = await setNurseStationAutoBillingRuleStatus({
      idempotencyKey: 'auto-billing-status-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleId: rule!.ruleId,
      expectedVersion: rule!.version,
      targetStatus: 'INACTIVE'
    })
    const stale = await setNurseStationAutoBillingRuleStatus({
      idempotencyKey: 'auto-billing-status-stale-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      ruleId: rule!.ruleId,
      expectedVersion: rule!.version,
      targetStatus: 'ACTIVE'
    })
    const refreshed = await queryNurseStationAutoBillingRules({
      encounterId: 'encounter-001',
      patientId: 'patient-001'
    })

    expect(changed.rule).toEqual(expect.objectContaining({ status: 'INACTIVE', version: 2 }))
    expect(stale).toEqual(
      expect.objectContaining({ success: false, message: '规则已被他人修改，请刷新后重试' })
    )
    expect(refreshed.rules.find((item) => item.ruleId === rule!.ruleId)?.status).toBe('INACTIVE')
  })
})
