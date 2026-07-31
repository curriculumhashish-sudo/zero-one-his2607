// @vitest-environment jsdom
import { describe, expect, it } from 'vitest'
import {
  getReleasePolicy,
  listHolidayExceptions,
  loadScheduleTemplateInit,
  pageHolidayExceptions,
  pageReleasePolicies,
  pageTimePeriods,
  pageWeeklyTemplates,
  saveHolidayException,
  saveReleasePolicy,
  saveTimePeriod,
  saveWeeklyTemplate,
  removeReleasePolicy
} from '@/apis/booking/schedule-template'
import {
  getPendingChargeDetail,
  getChargeReceipt,
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
      organizationId: init.organizationId,
      templateName: '测试时段',
      slotTemplates: [{ startTime: '09:00', endTime: '10:00', capacity: 10 }],
      enabled: true
    })
    expect(Object.keys(created).sort()).toEqual(['id'])
    const row = (await pageTimePeriods({ keyword: '测试时段' })).content.find(
      (item) => item.id === created.id
    )!
    expect(
      (await pageTimePeriods({ keyword: '测试时段' })).content.some((item) => item.id === created.id)
    ).toBe(true)
    expect(row.segments[0]?.capacity).toBe(10)
    expect(row).toMatchObject({
      templateName: '测试时段',
      enabled: true,
      organizationId: init.organizationId
    })
    expect(row.slotTemplates?.[0]).toMatchObject({ startTime: '09:00', endTime: '10:00' })
  })

  it('周模板按模板承载多条星期明细，节假日支持范围和生成开关', async () => {
    const init = await loadScheduleTemplateInit()
    const weekly = await saveWeeklyTemplate({
      organizationId: init.organizationId!,
      templateName: '测试周模板',
      isDefault: false,
      fallbackBookable: true,
      enabled: true,
      items: [
        { weekday: 2, dayScheduleTemplateId: 'daily-1', enabled: true },
        { weekday: 4, dayScheduleTemplateId: 'daily-2', enabled: false }
      ]
    })
    expect(Object.keys(weekly).sort()).toEqual(['id'])
    expect((await pageWeeklyTemplates({ keyword: '测试周模板' })).content[0]?.items[1]?.enabled).toBe(
      false
    )
    await saveHolidayException({
      organizationId: init.organizationId,
      exceptionDate: '2026-11-01',
      rangeEnd: '2026-11-03',
      exceptionType: 'REDUCED',
      overrideScheduleTemplateId: 'daily-1',
      reason: '测试减量',
      autoApplyOnGenerate: false
    })
    expect(await listHolidayExceptions({ start: '2026-11-02', end: '2026-11-02' })).toHaveLength(1)
    const paged = await pageHolidayExceptions({
      exceptionType: 'REDUCED',
      rangeStart: '2026-11-01',
      rangeEnd: '2026-11-30',
      keyword: '测试减量',
      pageNo: 1,
      pageSize: 10
    })
    expect(paged.content).toHaveLength(1)
  })

  it('节假日例外说明可选，结束日不能早于起始日', async () => {
    const created = await saveHolidayException({
      organizationId: 'org-main',
      exceptionDate: '2026-12-01',
      exceptionType: 'CLOSED',
      autoApplyOnGenerate: true
    })
    expect(created.id).toBeTruthy()
    await expect(
      saveHolidayException({
        organizationId: 'org-main',
        exceptionDate: '2026-12-10',
        rangeEnd: '2026-12-09',
        exceptionType: 'CLOSED'
      })
    ).rejects.toThrow('结束日期不能早于起始日')
  })

  it('放号策略按原型字段支持分页、新增、修改和删除', async () => {
    const created = await saveReleasePolicy({
      policyCode: 'TEST_POLICY',
      policyName: '测试放号策略',
      advanceDays: 7,
      slotMinutes: 20,
      capacityPerSlot: 8,
      channelQuotaJson: '{"WINDOW":60,"MINIAPP":40}'
    })
    expect(Object.keys(created).sort()).toEqual(['id'])
    expect(
      (await pageReleasePolicies({ keyword: 'TEST_POLICY', pageNo: 1, pageSize: 10 })).content
    ).toHaveLength(1)
    const updated = await saveReleasePolicy({
      id: created.id,
      policyCode: 'TEST_POLICY',
      policyName: '测试放号策略（已修改）',
      advanceDays: 10,
      slotMinutes: 30,
      capacityPerSlot: 12,
      channelQuotaJson: '{"WINDOW":70,"MINIAPP":30}'
    })
    expect(Object.keys(updated).sort()).toEqual(['id'])
    expect(
      (await pageReleasePolicies({ keyword: 'TEST_POLICY', pageNo: 1, pageSize: 10 })).content[0]
    ).toMatchObject({
      policyName: '测试放号策略（已修改）',
      advanceDays: 10,
      slotMinutes: 30,
      capacityPerSlot: 12,
      channelQuotas: { WINDOW: 70, MINIAPP: 30 }
    })
    await removeReleasePolicy(created.id!)
    expect(
      (await pageReleasePolicies({ keyword: 'TEST_POLICY', pageNo: 1, pageSize: 10 })).content
    ).toHaveLength(0)
  })

  it('门诊收费支持预结算和成功状态流转', async () => {
    const queue = await pageChargePatientQueue({ status: 'pending', page: 1, pageSize: 10 })
    const patient = queue.rows[0]!
    expect(patient).toMatchObject({
      patientId: patient.id,
      patientName: patient.name,
      encounterBusNo: patient.visitNo,
      queueLabelCode: 'PENDING'
    })
    const detail = await getPendingChargeDetail(patient.encounterId)
    const item = detail.items.find((row) => row.status === 'pending')!
    const settlement = await preSettleOutpatientCharge({
      encounterId: patient.encounterId,
      chargeItemIds: [item.id]
    })
    expect(Number(settlement.displayAmount)).toBe(item.amount)
    const record = await settleOutpatientCharge({
      paymentDraftId: settlement.paymentDraftId,
      encounterId: patient.encounterId,
      chargeItemIds: [item.id],
      billDate: '2026-07-25',
      outcomeEnum: 'COMPLETED',
      paymentDetails: [{ amount: item.amount, payChannelCode: 'card' }]
    })
    expect(Object.keys(record).sort()).toEqual([
      'invoiceId',
      'invoiceStatus',
      'paymentId',
      'successMessageCode',
      'ybSetlId'
    ])
    const receipt = await getChargeReceipt({
      encounterId: patient.encounterId,
      paymentId: record.paymentId
    })
    expect(receipt.slip.encounterId).toBe(patient.encounterId)
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
      chargeItemIds: [selected!.id]
    })
    const pending = await settleOutpatientCharge({
      paymentDraftId: settlement.paymentDraftId,
      encounterId: 'enc-1002',
      chargeItemIds: [selected!.id],
      billDate: '2026-07-25',
      outcomeEnum: 'COMPLETED',
      paymentDetails: [{ amount: selected!.amount, payChannelCode: 'wechat' }],
      clientScanResult: {
        merTradeNo: 'MER-TEST',
        tradeNo: 'SCAN-TEST',
        payType: 'wechat',
        respCode: 'PROCESSING',
        respMsg: '等待渠道确认'
      }
    })
    expect((await getPendingChargeDetail('enc-1002')).interruptedPaymentId).toBe(pending.paymentId)
    await queryPaymentResult(pending.paymentId)
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
      verifyOutpatientRefund({
        encounterId: 'enc-2001',
        refundingChargeItemIds: [first.id, other.id]
      })
    ).rejects.toThrow('同一笔原支付')
    const check = await verifyOutpatientRefund({
      encounterId: 'enc-2001',
      refundingChargeItemIds: [first.id]
    })
    expect(check).toEqual({ allowed: true })
    const retained = detail.items.find(
      (item) => item.paymentRecordId === first.paymentRecordId && item.id !== first.id
    )!
    const recalculated = await switchRetainedItemsInsuranceMode({
      encounterId: 'enc-2001'
    })
    expect(recalculated).toEqual({ success: true })
    expect(
      (await getRefundableExecutionDetail('enc-2001')).items.find((item) => item.id === retained.id)
        ?.selfPayAmount
    ).toBe(retained.amount)
    const result = await refundOutpatientCharge({
      encounterId: 'enc-2001',
      originalPaymentId: first.paymentRecordId,
      refundingChargeItemIds: [first.id],
      preservedNewChargeItemIds: [retained.id],
      refundPaymentDetails: [{ amount: first.refundableAmount, payChannelCode: first.channel }]
    })
    expect(Object.keys(result).sort()).toEqual([
      'halfRefundExecuted',
      'insuranceRevoked',
      'originalPaymentId',
      'reversePaymentId',
      'success'
    ])
    expect(result.success).toBe(true)
  })

  it('扫码退费刷新后仍可查询恢复，并阻止同原支付单重复退款', async () => {
    const detail = await getRefundableExecutionDetail('enc-2001')
    const item = detail.items.find(
      (row) => row.paymentRecordId === 'pay-9002' && row.status === 'refundable'
    )!
    const check = await verifyOutpatientRefund({
      encounterId: 'enc-2001',
      refundingChargeItemIds: [item.id]
    })
    expect(check.allowed).toBe(true)
    const pending = await refundOutpatientCharge({
      encounterId: 'enc-2001',
      originalPaymentId: item.paymentRecordId,
      refundingChargeItemIds: [item.id],
      refundPaymentDetails: [{ amount: item.refundableAmount, payChannelCode: item.channel }],
      clientRefundResult: {
        merTradeNo: item.paymentRecordId,
        tradeNo: 'REFUND-TEST',
        respCode: 'PROCESSING',
        respMsg: '等待渠道确认',
        refundAmountYuan: item.refundableAmount
      }
    })
    expect(pending.success).toBe(false)
    expect((await getRefundableExecutionDetail('enc-2001')).interruptedRefundId).toBe(
      pending.reversePaymentId
    )
    await expect(
      verifyOutpatientRefund({ encounterId: 'enc-2001', refundingChargeItemIds: [item.id] })
    ).rejects.toThrow('处理中的退款')
    expect((await queryRefundResult(pending.reversePaymentId!)).status).toBe('success')
  })

  it('住院预交金支持收款和不超过余额的退款', async () => {
    const before = await getInpatientPrepaySummary('inp-3001')
    const collected = await collectInpatientPrepay({
      encounterId: 'inp-3001',
      patientId: before.patient.patientId!,
      amount: 300,
      paymentDetails: [{ amount: 300, payChannelCode: 'card' }]
    })
    const rows = await pagePrepayRecords('inp-3001', { page: 1, pageSize: 20 })
    const source = rows.rows.find((row) => row.id === collected.paymentId)!
    expect(source).toMatchObject({
      paymentId: source.id,
      paymentEnum: 'CARD',
      statusEnum: 'SETTLED'
    })
    const refunded = await refundInpatientPrepay({
      encounterId: 'inp-3001',
      patientId: before.patient.patientId!,
      originalPaymentId: source.id,
      refundAmount: 100,
      paymentDetails: [{ amount: 100, payChannelCode: 'card' }]
    })
    expect(Object.keys(refunded).sort()).toEqual(['accountBalance', 'paymentId'])
    expect((await getInpatientPrepaySummary('inp-3001')).balance).toBe(before.balance + 200)
  })

  it('住院扫码交易支持取消，扫码退款仅在成功确认后扣减一次', async () => {
    const beforeCancel = await getInpatientPrepaySummary('inp-3002')
    const pendingCollect = await collectInpatientPrepay({
      encounterId: 'inp-3002',
      patientId: beforeCancel.patient.patientId!,
      amount: 200,
      paymentDetails: [{ amount: 200, payChannelCode: 'wechat' }],
      clientScanResult: {
        merTradeNo: 'MER-PREPAY-TEST',
        tradeNo: 'PREPAY-TEST',
        payType: 'wechat',
        respCode: 'PROCESSING',
        respMsg: '等待渠道确认',
        txnAmtFen: 20000
      }
    })
    await cancelPrepayPayment(pendingCollect.paymentId)
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeCancel.balance)

    const source = (await pagePrepayRecords('inp-3002', { page: 1, pageSize: 20 })).rows.find(
      (row) => row.id === 'prepay-3'
    )!
    const beforeRefund = await getInpatientPrepaySummary('inp-3002')
    const pendingRefund = await refundInpatientPrepay({
      encounterId: 'inp-3002',
      patientId: beforeRefund.patient.patientId!,
      originalPaymentId: source.id,
      refundAmount: 100,
      paymentDetails: [{ amount: 100, payChannelCode: source.channel }],
      clientRefundResult: {
        merTradeNo: source.id,
        tradeNo: 'PREPAY-REFUND',
        respCode: 'PROCESSING',
        respMsg: '等待渠道确认',
        refundAmountYuan: 100
      }
    })
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeRefund.balance)
    await queryPrepayPaymentResult(pendingRefund.paymentId)
    expect((await getInpatientPrepaySummary('inp-3002')).balance).toBe(beforeRefund.balance - 100)
  })
})
