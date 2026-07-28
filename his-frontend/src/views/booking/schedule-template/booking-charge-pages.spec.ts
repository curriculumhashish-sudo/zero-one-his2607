// @vitest-environment jsdom
import { describe, expect, it } from 'vitest'
import {
  getReleasePolicy,
  listHolidayExceptions,
  loadScheduleTemplateInit,
  pageDailyTemplates,
  pageTimePeriods,
  pageWeeklyTemplates,
  saveHolidayException,
  saveTimePeriod,
  saveWeeklyTemplate
} from '@/apis/booking/schedule-template'
import {
  getPendingChargeDetail,
  pageChargePatientQueue,
  preSettleOutpatientCharge,
  queryPaymentResult,
  settleOutpatientCharge
} from '@/apis/outpatient/charge/charge'
import {
  getRefundableExecutionDetail,
  queryRefundResult,
  refundOutpatientCharge,
  switchRetainedItemsInsuranceMode,
  verifyOutpatientRefund
} from '@/apis/outpatient/charge/refund'
import {
  cancelPrepayPayment,
  collectInpatientPrepay,
  getInpatientPrepaySummary,
  pagePrepayRecords,
  queryPrepayPaymentResult,
  refundInpatientPrepay
} from '@/apis/inpatient/charge/prepay'

describe('第二轮 B 组 API 流程', () => {
  it('号源设置初始化和时段单参数保存可用', async () => {
    const init = await loadScheduleTemplateInit()
    expect(init.department.label).toBeTruthy()
    expect((await getReleasePolicy()).openDays).toBeGreaterThan(0)
    const created = await saveTimePeriod({
      name: '测试时段',
      segments: [{ start: '09:00', end: '10:00', capacity: 10 }],
      status: 'enabled'
    })
    expect(created.name).toBe('测试时段')
    expect(
      (await pageTimePeriods({ keyword: '测试时段' })).rows.some((item) => item.id === created.id)
    ).toBe(true)
    expect(created.segments[0]?.capacity).toBe(10)
  })

  it('周模板按模板承载多条星期明细，节假日支持范围和生成开关', async () => {
    const weekly = await saveWeeklyTemplate({
      name: '测试周模板',
      isDefault: false,
      appointmentOpen: true,
      status: 'enabled',
      items: [
        { weekday: 2, dailyTemplateId: 'daily-1', enabled: true },
        { weekday: 4, dailyTemplateId: 'daily-2', enabled: false }
      ]
    })
    expect(weekly.items).toHaveLength(2)
    expect((await pageWeeklyTemplates({ keyword: '测试周模板' })).rows[0]?.items[1]?.enabled).toBe(
      false
    )
    await saveHolidayException({
      date: '2026-11-01',
      rangeEnd: '2026-11-03',
      type: 'reduced',
      replacementDailyTemplateId: 'daily-1',
      reason: '测试减量',
      autoApplyOnGenerate: false
    })
    expect(await listHolidayExceptions({ start: '2026-11-02', end: '2026-11-02' })).toHaveLength(1)
  })

  it('门诊收费支持预结算和成功状态流转', async () => {
    const queue = await pageChargePatientQueue({ status: 'pending', page: 1, pageSize: 10 })
    const patient = queue.rows[0]!
    const detail = await getPendingChargeDetail(patient.encounterId)
    const item = detail.items.find((row) => row.status === 'pending')!
    const settlement = await preSettleOutpatientCharge({
      encounterId: patient.encounterId,
      itemIds: [item.id]
    })
    const record = await settleOutpatientCharge({
      settlementId: settlement.settlementId,
      encounterId: patient.encounterId,
      itemIds: [item.id],
      channel: 'card'
    })
    expect(record.status).toBe('success')
    expect(
      (await getPendingChargeDetail(patient.encounterId)).items.find((row) => row.id === item.id)
        ?.status
    ).toBe('settled')
  })

  it('扫码收费只结算本次选中项目，并保留恢复所需记录', async () => {
    const detail = await getPendingChargeDetail('enc-1002')
    const [selected, unselected] = detail.items.filter((item) => item.status === 'pending')
    const settlement = await preSettleOutpatientCharge({
      encounterId: 'enc-1002',
      itemIds: [selected!.id]
    })
    const pending = await settleOutpatientCharge({
      settlementId: settlement.settlementId,
      encounterId: 'enc-1002',
      itemIds: settlement.itemIds,
      channel: 'wechat'
    })
    expect((await getPendingChargeDetail('enc-1002')).interruptedPaymentId).toBe(pending.id)
    await queryPaymentResult(pending.id)
    const after = await getPendingChargeDetail('enc-1002')
    expect(after.items.find((item) => item.id === selected!.id)?.status).toBe('settled')
    expect(after.items.find((item) => item.id === unselected!.id)?.status).toBe('pending')
  })

  it('门诊退费拒绝跨支付记录，并支持同组退费', async () => {
    const detail = await getRefundableExecutionDetail('enc-2001')
    const first = detail.items.find((item) => item.paymentRecordId === 'pay-9001')!
    const other = detail.items.find(
      (item) => item.paymentRecordId !== first.paymentRecordId && item.status === 'refundable'
    )!
    await expect(
      verifyOutpatientRefund({ encounterId: 'enc-2001', itemIds: [first.id, other.id] })
    ).rejects.toThrow('同一笔原支付')
    const check = await verifyOutpatientRefund({ encounterId: 'enc-2001', itemIds: [first.id] })
    const retained = detail.items.find(
      (item) => item.paymentRecordId === first.paymentRecordId && item.id !== first.id
    )!
    const recalculated = await switchRetainedItemsInsuranceMode({
      encounterId: 'enc-2001',
      itemIds: [retained.id],
      mode: 'self-pay'
    })
    expect(recalculated.items.find((item) => item.id === retained.id)?.selfPayAmount).toBe(
      retained.amount
    )
    const result = await refundOutpatientCharge({
      verificationId: check.verificationId,
      encounterId: 'enc-2001',
      paymentRecordId: check.paymentRecordId,
      itemIds: check.itemIds,
      channel: check.channel
    })
    expect(result.status).toBe('success')
  })

  it('扫码退费刷新后仍可查询恢复，并阻止同原支付单重复退款', async () => {
    const detail = await getRefundableExecutionDetail('enc-2001')
    const item = detail.items.find(
      (row) => row.paymentRecordId === 'pay-9002' && row.status === 'refundable'
    )!
    const check = await verifyOutpatientRefund({ encounterId: 'enc-2001', itemIds: [item.id] })
    const pending = await refundOutpatientCharge({
      verificationId: check.verificationId,
      encounterId: 'enc-2001',
      paymentRecordId: check.paymentRecordId,
      itemIds: check.itemIds,
      channel: check.channel
    })
    expect(pending.status).toBe('pending')
    expect((await getRefundableExecutionDetail('enc-2001')).interruptedRefundId).toBe(pending.id)
    await expect(
      verifyOutpatientRefund({ encounterId: 'enc-2001', itemIds: [item.id] })
    ).rejects.toThrow('处理中的退款')
    expect((await queryRefundResult(pending.id)).status).toBe('success')
  })

  it('住院预交金支持收款和不超过余额的退款', async () => {
    const before = await getInpatientPrepaySummary('inp-3001')
    const collected = await collectInpatientPrepay({
      encounterId: 'inp-3001',
      amount: 300,
      channel: 'card'
    })
    const rows = await pagePrepayRecords('inp-3001', { page: 1, pageSize: 20 })
    const source = rows.rows.find((row) => row.id === collected.id)!
    const refunded = await refundInpatientPrepay({
      encounterId: 'inp-3001',
      recordId: source.id,
      amount: 100,
      channel: 'card'
    })
    expect(refunded.type).toBe('refund')
    expect((await getInpatientPrepaySummary('inp-3001')).balance).toBe(before.balance + 200)
  })

  it('住院扫码交易支持取消，扫码退款仅在成功确认后扣减一次', async () => {
    const beforeCancel = await getInpatientPrepaySummary('inp-3002')
    const pendingCollect = await collectInpatientPrepay({
      encounterId: 'inp-3002',
      amount: 200,
      channel: 'wechat'
    })
    await cancelPrepayPayment(pendingCollect.id)
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeCancel.balance)

    const source = (await pagePrepayRecords('inp-3002', { page: 1, pageSize: 20 })).rows.find(
      (row) => row.id === 'prepay-3'
    )!
    const beforeRefund = await getInpatientPrepaySummary('inp-3002')
    const pendingRefund = await refundInpatientPrepay({
      encounterId: 'inp-3002',
      recordId: source.id,
      amount: 100,
      channel: source.channel
    })
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeRefund.balance)
    await queryPrepayPaymentResult(pendingRefund.id)
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeRefund.balance - 100)
  })
})
