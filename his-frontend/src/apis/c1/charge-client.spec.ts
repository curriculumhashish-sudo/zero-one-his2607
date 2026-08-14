// @vitest-environment jsdom

import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpState = vi.hoisted(() => ({
  calls: [] as Array<{ body?: any; method: string; path: string }>
}))

vi.mock('@/apis/http', () => {
  class FakeHttpClient {
    get(path: string, params?: any) {
      httpState.calls.push({ body: params, method: 'GET', path })
      if (path.endsWith('/get-item-list')) {
        if (params?.encounterId === 'ENC-NO-REFUND') {
          return Promise.resolve({ code: 10000, data: { paymentGroups: [] } })
        }
        return Promise.resolve({
          code: 10000,
          data: {
            paymentGroups: [
              {
                paymentRecordId: 'PAY-1',
                preservedLines: [
                  { chargeItemId: 'ITEM-1', chargeItemStatusEnum: 'SETTLED', lineTotalAmount: 20 },
                  { chargeItemId: 'ITEM-2', chargeItemStatusEnum: 'REFUNDED', lineTotalAmount: 10 }
                ],
                refundableAmount: 20,
                refundingLines: []
              }
            ]
          }
        })
      }
      if (path.endsWith('/get-patient-list')) {
        return Promise.resolve({
          code: 10000,
          data: {
            pageIndex: 1,
            pageSize: 1000,
            total: 2,
            rows: [
              {
                encounterId: 'ENC-1',
                patientId: 'PAT-1',
                patientName: '可退患者',
                queueLabelCode: 'PENDING'
              },
              {
                encounterId: 'ENC-NO-REFUND',
                patientId: 'PAT-2',
                patientName: '无支付记录患者',
                queueLabelCode: 'PENDING'
              }
            ]
          }
        })
      }
      if (path.endsWith('/getseeing')) {
        return Promise.resolve({
          code: 10000,
          data: {
            pageIndex: 1,
            pageSize: 20,
            rows: [
              {
                cost: 0,
                id: 'PAT-1',
                name: '测试患者',
                organization: '门诊内科',
                register_type: '普通号',
                sex: '男',
                statusEnum: 'REGISTERED',
                time: '2026-08-11 09:00:00'
              },
              {
                cost: 0,
                id: 'INPATIENT-1',
                name: '住院患者',
                number: 'ZY2026-08-110001',
                organization: '住院内科',
                register_type: '',
                statusEnum: 'INHOSPITAL',
                time: '2026-08-11 10:00:00'
              }
            ],
            total: 2
          }
        })
      }
      if (path.endsWith('/getpatientpayrecords')) {
        if (params?.hospitalized_number === 'ZY-CASH') {
          return Promise.resolve({
            code: 10000,
            data: {
              pageIndex: 1,
              pageSize: 10,
              total: 1,
              rows: [
                {
                  bill_number: 'CASH-1',
                  cost: 1000,
                  pay_time: '2026-08-12 17:32:13',
                  pay_type: '',
                  service_type: 'PREPAY',
                  status: 'PENDING'
                }
              ]
            }
          })
        }
        return Promise.resolve({
          code: 10000,
          data: {
            pageIndex: 1,
            pageSize: 10,
            total: 1,
            rows: [
              {
                bill_number: 'PENDING-0',
                cost: 0,
                pay_time: '2026-08-12 17:04:06',
                pay_type: 'CASH',
                service_type: 'PREPAY',
                status: 'PENDING'
              }
            ]
          }
        })
      }
      if (path.endsWith('/get-patient-queue')) {
        if (params?.queueTabCode === 'PENDING') {
          return Promise.resolve({
            code: 10000,
            data: { pageIndex: 1, pageSize: 10, total: 0, rows: [] }
          })
        }
        return Promise.resolve({
          code: 10000,
          data: {
            pageIndex: 1,
            pageSize: 1000,
            total: 1,
            rows: [
              {
                encounterId: 'ENC-UNPAID-1',
                encounterBusNo: 'EN2026-08120001',
                patientId: 'PAT-UNPAID-1',
                patientName: '待收费患者',
                queueLabelCode: 'SETTLED',
                totalAmount: 5,
                paidAmount: 0
              }
            ]
          }
        })
      }
      return Promise.resolve({ code: 10000, data: { allowed: true } })
    }

    post(path: string, body?: any) {
      httpState.calls.push({ body, method: 'POST', path })
      if (path.endsWith('/add_register')) {
        return Promise.resolve({ code: 10000, data: 'ENC-REGISTER-1' })
      }
      if (path.endsWith('/chargeconfirmcontroller')) {
        return Promise.resolve({ code: 10000, data: true })
      }
      return Promise.resolve({ code: 10000, data: { success: true } })
    }
  }

  return { DataUpType: { json: 'json' }, default: FakeHttpClient }
})

import {
  c1CancelOutpatientRegister,
  c1ConfirmOutpatientRegister,
  c1GetRefundableExecutionDetail,
  c1GetRegisterTicket,
  c1ListBookableSlotHints,
  c1LoadOutpatientRegisterInit,
  c1PageChargePatientQueue,
  c1PagePrepayRecords,
  c1PageRefundPatientQueue,
  c1PageOutpatientRegisterHistory,
  c1PageRegisterPatientOptions,
  c1PreSettleOutpatientRegister,
  c1RefundInpatientPrepay,
  c1RefundOutpatientCharge,
  c1SettleOutpatientCharge,
  c1VerifyOutpatientRefund
} from './charge-client'

describe('C1 收费工作站请求适配', () => {
  beforeEach(() => {
    httpState.calls.length = 0
    vi.unstubAllEnvs()
    localStorage.clear()
  })

  it('C1 队列状态错误时按未付余额识别待收费患者', async () => {
    const page = await c1PageChargePatientQueue({
      queueTabCode: 'PENDING',
      pageIndex: 1,
      pageSize: 10
    })

    expect(page.total).toBe(1)
    expect(page.rows[0]).toMatchObject({
      encounterId: 'ENC-UNPAID-1',
      patientName: '待收费患者',
      status: 'pending'
    })
  })

  it('C1 待退费队列只保留存在原支付记录和可退项目的患者', async () => {
    const page = await c1PageRefundPatientQueue({
      queueTabCode: 'PENDING',
      pageIndex: 1,
      pageSize: 10
    })

    expect(page.total).toBe(1)
    expect(page.rows).toHaveLength(1)
    expect(page.rows[0]).toMatchObject({ encounterId: 'ENC-1', patientName: '可退患者' })
  })

  it('C1 当日号源为空时可在开发联调中放行旧测试号源', async () => {
    vi.stubEnv('VITE_C1_ALLOW_LEGACY_REGISTER_SLOT', 'true')
    vi.stubEnv('VITE_C1_DEFAULT_ORGANIZATION_ID', '322663308812357632')
    vi.stubEnv('VITE_C1_DEFAULT_TENANT_ID', '1')

    const result = await c1ListBookableSlotHints({})

    expect(result.hints).toEqual([
      {
        fee: 5,
        remainingCount: 30,
        slotEnd: '2026-08-10 18:00:00',
        slotId: '329701000000000070',
        slotStart: '2026-08-10 08:00:00'
      }
    ])

    const preview = await c1PreSettleOutpatientRegister({
      contractId: 'CONTRACT-SELF',
      organizationId: '门诊内科',
      serviceTypeId: 'SERVICE-NORMAL',
      slotId: '329701000000000070'
    })
    expect(preview.totalAmount).toBe(5)
    expect(preview.chargeItems[0]).toMatchObject({ quantity: 1, unitPrice: 5, amount: 5 })

    localStorage.setItem('operatorId', 'OP-1')
    localStorage.setItem('operatorName', '收费员')
    await c1ConfirmOutpatientRegister({
      paymentDraftId: preview.paymentDraftId,
      paymentDetails: [{ payAmount: 5, payChannelCode: 'CASH' }]
    })
    const registerCall = httpState.calls.find((item) => item.path.endsWith('/add_register'))
    expect(registerCall?.body).toMatchObject({
      balance: 0,
      contractNo: '1',
      doctor: '收费员',
      doctor_id: 'OP-1',
      organization_id: '322663308812357632',
      role_code: 'ATTENDING',
      tenant_id: '1',
      type_code: 'PERSONAL_CASH_ACCOUNT'
    })
  })

  it('挂号性别选项和患者性别统一为前端标准枚举', async () => {
    const init = await c1LoadOutpatientRegisterInit()
    expect(init.genderOptions).toEqual([
      { label: '男', value: 'MALE' },
      { label: '女', value: 'FEMALE' }
    ])

    const patients = await c1PageRegisterPatientOptions({ pageIndex: 1, pageSize: 20 })
    expect(patients.records[0]).toMatchObject({ genderCode: 'MALE', genderText: '男' })
  })

  it('REGISTERED 挂号显示为已挂号并启用退号，无费用退号给出明确提示', async () => {
    const page = await c1PageOutpatientRegisterHistory({
      dateFrom: '2026-08-11',
      dateTo: '2026-08-11',
      pageIndex: 1,
      pageSize: 20
    })
    expect(page.total).toBe(1)
    expect(page.records).toHaveLength(1)
    expect(page.records[0]).toMatchObject({ canCancel: true, statusEnum: 'PLANNED' })

    await expect(
      c1CancelOutpatientRegister({ encounterId: 'PAT-1', cancelReason: '测试退号' })
    ).rejects.toThrow('C1 退号接口暂不支持无费用退号')
    expect(httpState.calls.some((item) => item.path.endsWith('/quitnumber'))).toBe(false)
  })

  it('门诊收费只发送后端 DTO 字段，并转换支付渠道和账单时间', async () => {
    await c1SettleOutpatientCharge({
      billDate: '2026-08-11',
      chargeItemIds: ['ITEM-1'],
      clientScanResult: { tradeNo: 'IGNORED' },
      encounterId: 'ENC-1',
      extraField: 'IGNORED',
      outcomeEnum: 'COMPLETED',
      paymentDetails: [{ amount: 20, payChannelCode: 'card' }],
      paymentDraftId: 'DRAFT-1'
    })

    const body = httpState.calls[0]!.body
    expect(body.paymentDetails).toEqual([{ amount: 20, payChannelCode: 'BANK_CARD' }])
    expect(body.billDate).toMatch(/^2026-08-11 \d{2}:\d{2}:\d{2}$/)
    expect(body).not.toHaveProperty('clientScanResult')
    expect(body).not.toHaveProperty('extraField')
  })

  it('退费校验发送逗号分隔项目 ID，退费提交按 DTO 转换', async () => {
    await c1VerifyOutpatientRefund({
      encounterId: 'ENC-1',
      refundingChargeItemIds: ['ITEM-1', 'ITEM-2']
    })
    expect(httpState.calls[0]!.body).toEqual({
      encounterId: 'ENC-1',
      refundingChargeItemIds: 'ITEM-1,ITEM-2'
    })

    await c1RefundOutpatientCharge({
      clientRefundResult: { tradeNo: 'REFUND-1' },
      encounterId: 'ENC-1',
      originalPaymentId: 'PAY-1',
      refundPaymentDetails: [{ amount: 20, payChannelCode: 'card' }],
      refundingChargeItemIds: ['ITEM-1']
    })
    const body = httpState.calls[1]!.body
    expect(body.refundPaymentDetails).toEqual([{ amount: 20, payChannelCode: 'BANK_CARD' }])
    expect(body.clientRefundResult).toBe('{"tradeNo":"REFUND-1"}')
    expect(Object.keys(body).sort()).toEqual([
      'billDate',
      'clientRefundResult',
      'encounterId',
      'originalPaymentId',
      'preservedNewChargeItemIds',
      'refundPaymentDetails',
      'refundingChargeItemIds'
    ])
  })

  it('后端放入 preservedLines 的 SETTLED 项目仍作为可退项目显示', async () => {
    const detail = await c1GetRefundableExecutionDetail('ENC-1')
    expect(detail.items.find((item) => item.chargeItemId === 'ITEM-1')?.status).toBe('refundable')
    expect(detail.items.find((item) => item.chargeItemId === 'ITEM-2')?.status).toBe('unavailable')
  })

  it('挂号小票接口只发送后端 PrintRegisterReceiptDTO 字段', async () => {
    await c1GetRegisterTicket({ encounterId: 'ENC-1' })
    const call = httpState.calls.find((item) => item.path.endsWith('/printregisterreceipt'))
    expect(call?.body).toMatchObject({
      bindingCode: 'op-reg-print',
      bizRefJson: '{"encounterId":"ENC-1"}',
      clientType: 'WEB',
      copies: 1,
      printType: 'REGISTER_RECEIPT',
      templateVersion: 1
    })
    expect(call?.body).not.toHaveProperty('encounterId')
  })

  it('C1 预交金退款按后端 DTO 发送，并忽略 0 元占位流水', async () => {
    await c1RefundInpatientPrepay({
      encounterId: 'ENC-1',
      inpatientNo: 'ZY-1',
      operatorId: 'OP-1',
      paymentDetails: [{ amount: 500, payChannelCode: 'cash' }],
      refundAmount: 500,
      remark: '测试退款'
    })
    expect(httpState.calls[0]).toMatchObject({
      method: 'POST',
      path: '/pay_manage/charge/refund-prepayment',
      body: {
        inpatientNo: 'ZY-1',
        operatorId: 'OP-1',
        paymentDetails: [{ amount: 500, payChannelCode: 'CASH' }],
        reason: '测试退款',
        remark: '测试退款'
      }
    })

    const page = await c1PagePrepayRecords('ZY-1', { pageIndex: 1, pageSize: 10 })
    expect(page.rows).toEqual([])
  })

  it('C1 现金预交金已入账但流水为 PENDING 时按已结算显示', async () => {
    const page = await c1PagePrepayRecords('ZY-CASH', { pageIndex: 1, pageSize: 10 })

    expect(page.rows).toHaveLength(1)
    expect(page.rows[0]).toMatchObject({
      amount: 1000,
      channel: 'cash',
      refundableAmount: 1000,
      status: 'settled'
    })
  })
})
