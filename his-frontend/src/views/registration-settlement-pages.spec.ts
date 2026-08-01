// @vitest-environment jsdom

import { beforeEach, describe, expect, expectTypeOf, it } from 'vitest'
import { createPinia, setActivePinia } from 'pinia'
import registrationSettlementRoutes from '@/router/main/registration-settlement'

import {
  cancelInpatientRegistration,
  getInpatientRegistrationContext,
  loadInpatientRegistrationInit,
  pageInpatientRegistrationQueue,
  pageRegistrationPatientOptions,
  submitInpatientRegistration
} from '@/apis/inpatient/charge/register'
import {
  confirmInpatientSettle,
  getInpatientSettleSummary,
  listInpatientSettleCharges,
  listInpatientSettlementHistory,
  pageInpatientSettleQueue,
  preSettleInpatientCharges,
  reverseInpatientSettle
} from '@/apis/inpatient/charge/settle'
import {
  cancelRegisterWithRefund,
  confirmOutpatientRegister,
  getRegisterTicket,
  getTodayRegisterSummary,
  listBookableSlotHints,
  listRegisterPractitioners,
  loadRegisterBenefitPreview,
  loadOutpatientRegisterInit,
  pageOutpatientRegisterHistory,
  pageRegisterPatientOptions,
  preSettleOutpatientRegister,
  validateRegisterCancellation
} from '@/apis/outpatient/charge/register'
import type {
  PreSettleOutpatientRegisterPayload,
  RegisterPaymentDetail
} from '@/apis/outpatient/charge/register'
import type {
  InpatientSettlePaymentDetail,
  PreSettleInpatientResult
} from '@/apis/inpatient/charge/settle'
import { useUserStore } from '@/stores/user'
import {
  calculateRegistrationAge,
  parseRegistrationIdCard
} from '@/views/inpatient/charge/register/adapter'
import { calculateAge, parseResidentIdCard } from '@/views/outpatient/charge/register/adapter'

function todayText() {
  const date = new Date()
  return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

describe('B组三轮登记结算接口', () => {
  beforeEach(() => {
    setActivePinia(createPinia())
    useUserStore().user = { id: 'OPERATOR-B', username: '收费员B' }
  })

  it('注册第三轮三个独占路由', () => {
    expect(registrationSettlementRoutes.map((route) => route.path)).toEqual([
      '/inpatient/charge/register',
      '/inpatient/charge/settle',
      '/outpatient/charge/register'
    ])
  })

  it('接口关键字段类型和返回结构与原型一致', async () => {
    expectTypeOf<PreSettleOutpatientRegisterPayload['ybRegisterFee']>().toEqualTypeOf<
      boolean | undefined
    >()
    expectTypeOf<RegisterPaymentDetail['payAmount']>().toEqualTypeOf<number | string>()
    expectTypeOf<InpatientSettlePaymentDetail['amount']>().toEqualTypeOf<number | string>()
    expectTypeOf<PreSettleInpatientResult['receivableTotal']>().toEqualTypeOf<number | string>()

    const registrationInit = await loadInpatientRegistrationInit()
    expect(Object.keys(registrationInit)).toEqual(['paymentChannelOptions'])

    const registrationByDate = await pageInpatientRegistrationQueue(
      { PageIndex: 1, PageSize: 20 },
      {},
      { dateFrom: '2026-07-28', dateTo: '2026-07-28' }
    )
    expect(registrationByDate.content.length).toBeGreaterThan(0)
    expect(
      registrationByDate.content.every((record) => record.startTime?.startsWith('2026-07-28'))
    ).toBe(true)

    const outpatientInit = await loadOutpatientRegisterInit()
    expect(outpatientInit).not.toHaveProperty('appointmentModeOptions')
    expect(outpatientInit).toHaveProperty('practitionerOptions')

    const departmentId = outpatientInit.departmentOptions[0]!.value
    const slotResult = await listBookableSlotHints({ organizationId: departmentId })
    const firstSlot = slotResult.hints[0]!
    expect(Object.keys(firstSlot).sort()).toEqual([
      'remainingCount',
      'slotEnd',
      'slotId',
      'slotStart'
    ])
  })

  it('患者选择表格可直接加载数据，身份证自动识别出生日期、年龄和性别', async () => {
    const [inpatientPatients, outpatientPatients] = await Promise.all([
      pageRegistrationPatientOptions({ PageIndex: 1, PageSize: 20 }),
      pageRegisterPatientOptions({ PageIndex: 1, PageSize: 20 })
    ])
    expect(inpatientPatients.records.length).toBeGreaterThan(0)
    expect(outpatientPatients.records.length).toBeGreaterThan(0)

    expect(parseRegistrationIdCard('110101197804120011')).toMatchObject({
      birthDate: '1978-04-12',
      genderEnum: 'MALE'
    })
    expect(parseResidentIdCard('110101199103140044')).toMatchObject({
      birthDate: '1991-03-14',
      gender: 'FEMALE'
    })
    expect(calculateRegistrationAge('2000-08-01', new Date(2026, 6, 30))).toBe(25)
    expect(calculateAge('2000-07-01', new Date(2026, 6, 30))).toBe(26)
  })

  it('住院登记支持上下文、登记和取消状态流转', async () => {
    const queue = await pageInpatientRegistrationQueue({ PageIndex: 1, PageSize: 20 })
    const source = queue.content.find((item) => item.statusEnum === 'WAIT_ADMISSION')!
    const context = await getInpatientRegistrationContext(source.encounterId)
    expect(context.patient?.patientId).toBe(source.patientId)
    const result = await submitInpatientRegistration({
      admissionDiagnoses: [{ description: '测试诊断', primaryFlag: true }],
      contractId: 'CONTRACT-SELF',
      encounterId: source.encounterId,
      operatorId: 'OPERATOR-B',
      organizationId: '1001001',
      patientId: source.patientId,
      patientName: source.patientName,
      startTime: '2026-07-28 10:00:00',
      wardId: '2001001'
    })
    expect(result).toMatchObject({ encounterId: source.encounterId, statusEnum: 'ADMITTED' })
    await expect(
      cancelInpatientRegistration(source.encounterId, '测试取消')
    ).resolves.toMatchObject({ success: true })
  })

  it('住院结算支持费用明细、预结算、确认和反结', async () => {
    const queue = await pageInpatientSettleQueue({
      settleQueueTab: 'PENDING_SETTLE',
      PageIndex: 1,
      PageSize: 20
    })
    const patient = queue.content[0]!
    const detail = await listInpatientSettleCharges(
      patient.encounterId,
      patient.patientId,
      'DETAIL'
    )
    const pending = (detail.lines ?? []).filter((line) => line.statusEnum === 'PENDING')
    expect(pending.length).toBeGreaterThan(0)
    const preview = await preSettleInpatientCharges({
      chargeItemIds: pending.map((line) => line.chargeItemId),
      contractId: patient.contractId,
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      settleMode: 'INTERIM'
    })
    expect(Number(preview.receivableTotal)).toBeGreaterThan(0)
    const result = await confirmInpatientSettle({
      chargeItemIds: pending.map((line) => line.chargeItemId),
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      paymentDetails: [
        {
          amount: preview.insurancePreSettled
            ? (preview.displayAmount ?? 0)
            : preview.receivableTotal,
          payChannelCode: 'CASH'
        }
      ],
      paymentDraftId: preview.paymentDraftId
    })
    expect(result.paymentId).toBeTruthy()
    const history = await listInpatientSettlementHistory(patient.encounterId, patient.patientId)
    expect(history.payments[0]?.paymentId).toBe(result.paymentId)
    await expect(
      reverseInpatientSettle('settle-enc-1', 'settle-patient-1', result.paymentId, '测试反结')
    ).resolves.toMatchObject({
      succeeded: true
    })
    await expect(
      getInpatientSettleSummary(patient.encounterId, patient.patientId)
    ).resolves.toHaveProperty('pendingSettlementTotal')
  })

  it('门诊挂号支持号源回填、缴费、票据和退款取消', async () => {
    const init = await loadOutpatientRegisterInit()
    const departmentId = init.departmentOptions[0]!.value
    const practitionerId = (await listRegisterPractitioners({ organizationId: departmentId }))
      .practitionerOptions[0]!.value
    const hints = await listBookableSlotHints({
      organizationId: departmentId,
      registrationDate: todayText(),
      practitionerId,
      healthcareServiceId: init.serviceTypeOptions[0]!.value
    })
    const slot = hints.hints.find((item) => (item.remainingCount ?? 0) > 0)
    expect(slot).toBeTruthy()
    const preview = await preSettleOutpatientRegister({
      birthDate: '1991-03-14',
      contractId: 'CONTRACT-SELF',
      gender: 'FEMALE',
      idCardNo: '110101199103140044',
      name: '测试患者',
      organizationId: departmentId,
      phone: '13800000044',
      practitionerId,
      serviceTypeId: init.serviceTypeOptions[0]!.value,
      slotId: slot!.slotId
    })
    const confirmed = await confirmOutpatientRegister({
      paymentDetails: [{ payAmount: preview.totalAmount, payChannelCode: 'CASH' }],
      paymentDraftId: preview.paymentDraftId
    })
    expect(confirmed.status).toBe('SUCCESS')
    await expect(getRegisterTicket(String(confirmed.encounterId))).resolves.toHaveProperty('slip')
    const history = await pageOutpatientRegisterHistory({
      dateFrom: todayText(),
      dateTo: todayText(),
      PageIndex: 1,
      PageSize: 20
    })
    const record = history.records.find((item) => item.encounterId === confirmed.encounterId)!
    expect((await validateRegisterCancellation(record.encounterId)).cancelable).toBe(true)
    await expect(
      cancelRegisterWithRefund({
        encounterId: record.encounterId,
        cancelReason: '测试退号',
        refundRequestId: crypto.randomUUID()
      })
    ).resolves.toMatchObject({ encounterStatus: 'CANCELLED' })
    expect((await getTodayRegisterSummary()).cancelledCount).toBeGreaterThan(0)
  })

  it('门诊医保待遇返回原型字段并提示凭证过期', async () => {
    const result = await loadRegisterBenefitPreview({
      contractId: 'CONTRACT-MI',
      patientId: '930000000000000001',
      runTreatmentCheck: true,
      ybMediumType: '02',
      ybPsnName: '陈晨'
    })
    expect(result).toMatchObject({
      benefitKind: 'INSURANCE',
      contractMatch: {
        matchStatus: 'MATCH',
        selectedContractId: 'CONTRACT-MI'
      },
      ybPreview: {
        insuplcAdmdvs: '229900',
        treatmentBlocked: false
      }
    })
    await expect(
      loadRegisterBenefitPreview({
        contractId: 'CONTRACT-MI',
        ybCardInfo: 'EXPIRED-CREDENTIAL',
        ybMediumType: '01'
      })
    ).rejects.toThrow('医保凭证已过期')
  })

  it('住院结算扫码结果可使用同一支付草稿补结算且不会重复扣款', async () => {
    const queue = await pageInpatientSettleQueue({
      settleQueueTab: 'PENDING_SETTLE',
      PageIndex: 1,
      PageSize: 20
    })
    const patient = queue.content[0]!
    const detail = await listInpatientSettleCharges(
      patient.encounterId,
      patient.patientId,
      'DETAIL'
    )
    const pending = (detail.lines ?? []).filter((line) => line.statusEnum === 'PENDING')
    const preview = await preSettleInpatientCharges({
      chargeItemIds: pending.map((line) => line.chargeItemId),
      contractId: patient.contractId,
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      settleMode: 'INTERIM'
    })
    const payable = preview.insurancePreSettled
      ? (preview.displayAmount ?? 0)
      : preview.receivableTotal
    const payload = {
      chargeItemIds: pending.map((line) => line.chargeItemId),
      clientScanResult: {
        merTradeNo: `MER-${preview.paymentDraftId}`,
        tradeNo: `RECOVER-${preview.paymentDraftId}`,
        payType: 'SCAN_PAY',
        respCode: '00',
        respMsg: '支付成功',
        txnAmtFen: Math.round(Number(payable) * 100)
      },
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      paymentDetails: [{ amount: payable, payChannelCode: 'SCAN_PAY' }],
      paymentDraftId: preview.paymentDraftId,
      ybPreSettleId: preview.ybPreSettleId
    }
    await expect(confirmInpatientSettle(payload)).rejects.toThrow('本地住院结算尚未完成')
    const result = await confirmInpatientSettle(payload)
    expect(result.paymentId).toBeTruthy()
    await reverseInpatientSettle(
      'settle-enc-1',
      'settle-patient-1',
      result.paymentId,
      '恢复测试数据'
    )
  })

  it('门诊挂号扫码结果可使用同一支付草稿补结算', async () => {
    const init = await loadOutpatientRegisterInit()
    const organizationId = init.departmentOptions[0]!.value
    const practitionerId = (await listRegisterPractitioners({ organizationId }))
      .practitionerOptions[0]!.value
    const slot = (
      await listBookableSlotHints({
        healthcareServiceId: init.serviceTypeOptions[0]!.value,
        organizationId,
        practitionerId,
        registrationDate: todayText()
      })
    ).hints.find((item) => (item.remainingCount ?? 0) > 0)!
    const preview = await preSettleOutpatientRegister({
      birthDate: '1991-03-14',
      contractId: 'CONTRACT-SELF',
      gender: 'FEMALE',
      idCardNo: '110101199103140044',
      name: '扫码测试患者',
      organizationId,
      phone: '13800000044',
      practitionerId,
      serviceTypeId: init.serviceTypeOptions[0]!.value,
      slotId: slot.slotId
    })
    const payload = {
      clientScanResult: {
        merTradeNo: `MER-${preview.paymentDraftId}`,
        tradeNo: `RECOVER-${preview.paymentDraftId}`,
        payType: 'SCAN_PAY',
        respCode: '00',
        respMsg: '支付成功',
        txnAmtFen: Math.round(Number(preview.totalAmount) * 100)
      },
      paymentDetails: [{ payAmount: preview.totalAmount, payChannelCode: 'SCAN_PAY' }],
      paymentDraftId: preview.paymentDraftId
    }
    await expect(confirmOutpatientRegister(payload)).rejects.toThrow('本地挂号结算尚未完成')
    await expect(confirmOutpatientRegister(payload)).resolves.toMatchObject({ status: 'SUCCESS' })
  })
})
