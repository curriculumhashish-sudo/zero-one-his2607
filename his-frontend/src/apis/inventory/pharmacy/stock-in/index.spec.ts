import { describe, expect, it } from 'vitest'

import {
  approvePurchaseInbound,
  createPurchaseInbound,
  getPurchaseInboundDetail,
  rejectPurchaseInbound,
  removePurchaseInbound,
  submitPurchaseInbound,
  updatePurchaseInbound,
  withdrawPurchaseInbound
} from './index'

const detail = {
  id: 'test-line-1',
  itemCode: 'TEST-001',
  itemName: '测试药品',
  specification: '10mg',
  manufacturer: '测试厂家',
  batchNo: 'LOT-001',
  bookQuantity: 10,
  actualQuantity: 9,
  actualUnit: '盒',
  profitLoss: -1,
  unitPrice: 2,
  actualAmount: 18,
  profitLossAmount: -2,
  remark: ''
}

describe('药房盘点模拟接口状态流转', () => {
  it('支持创建、修改、提交、撤回、审批、驳回和删除', async () => {
    const created = await createPurchaseInbound({
      pharmacy: '门诊西药房',
      itemType: '药品',
      details: [detail]
    })
    expect(created.order.status).toBe(0)
    expect((await getPurchaseInboundDetail(created.order.id)).details).toHaveLength(1)

    const updated = await updatePurchaseInbound(created.order.id, {
      pharmacy: '门诊西药房',
      itemType: '药品',
      details: [{ ...detail, actualQuantity: 8, profitLoss: -2 }]
    })
    expect(updated.details[0]?.actualQuantity).toBe(8)

    expect((await submitPurchaseInbound(created.order.id)).status).toBe(1)
    expect((await withdrawPurchaseInbound(created.order.id)).status).toBe(0)
    await submitPurchaseInbound(created.order.id)
    expect((await approvePurchaseInbound(created.order.id)).status).toBe(2)

    const rejectedDraft = await createPurchaseInbound({
      pharmacy: '门诊中药房',
      itemType: '药品',
      details: [detail]
    })
    await submitPurchaseInbound(rejectedDraft.order.id)
    expect((await rejectPurchaseInbound(rejectedDraft.order.id, '数量需要复核')).status).toBe(3)

    const removable = await createPurchaseInbound({
      pharmacy: '住院西药房',
      itemType: '药品',
      details: [detail]
    })
    await removePurchaseInbound(removable.order.id)
    await expect(getPurchaseInboundDetail(removable.order.id)).rejects.toThrow('单据不存在')
  })
})
