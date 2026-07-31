import type {
  BookableSlotHint,
  BookableSlotQuery,
  BookableSlotResult,
  CancelPlannedRegistrationPayload,
  CancelPlannedRegistrationResult,
  CancelOutpatientRegisterPayload,
  CancelOutpatientRegisterResult,
  ConfirmOutpatientRegisterPayload,
  ConfirmOutpatientRegisterResult,
  LoadRegisterBenefitPreviewParams,
  LoadRegisterBenefitPreviewResult,
  LoadOutpatientRegisterInitQuery,
  PreSettleOutpatientRegisterPayload,
  PreSettleOutpatientRegisterResult,
  RegisterAppointmentContext,
  RegisterAppointmentQuery,
  RegisterCancellationValidation,
  RegisterHistoryPage,
  RegisterHistoryQuery,
  RegisterHistoryRecord,
  RegisterInitResult,
  RegisterOption,
  RegisterPatient,
  RegisterPatientPage,
  RegisterPatientQuery,
  RegisterPractitionerQuery,
  RegisterPractitionerResult,
  RegisterTicketQuery,
  RegisterTicketResult,
  RequestContext,
  TodayRegisterSummary,
  TodayRegisterSummaryQuery,
  ValidateRegisterCancellationPayload
} from './type'

const departments: RegisterOption[] = [
  { value: '1001001', label: '心血管内科' },
  { value: '1001002', label: '呼吸内科' },
  { value: '1001003', label: '神经内科' }
]

type MockPractitionerOption = RegisterOption & { organizationId: string }

const practitioners: MockPractitionerOption[] = [
  { value: '3001001', label: '赵主任', organizationId: '1001001' },
  { value: '3001002', label: '钱医生', organizationId: '1001001' },
  { value: '3002001', label: '孙医生', organizationId: '1001002' },
  { value: '3003001', label: '李医生', organizationId: '1001003' }
]

const patients: RegisterPatient[] = [
  {
    patientId: '930000000000000001',
    patientBusNo: 'MR202607101',
    name: '陈晨',
    genderCode: 'FEMALE',
    genderText: '女',
    birthDate: '1991-03-14',
    age: 35,
    idCardNoMasked: '110101199103140044',
    phoneMasked: '13800000044'
  },
  {
    patientId: '930000000000000002',
    patientBusNo: 'MR202607102',
    name: '林强',
    genderCode: 'MALE',
    genderText: '男',
    birthDate: '1980-11-02',
    age: 45,
    idCardNoMasked: '110101198011020055',
    phoneMasked: '13800000055'
  }
]

const now = new Date()
const today = `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(2, '0')}-${String(
  now.getDate()
).padStart(2, '0')}`
type MockBookableSlot = BookableSlotHint & {
  healthcareServiceId: string
  healthcareServiceName: string
  organizationId: string
  organizationName: string
  practitionerId: string
  practitionerName: string
  remainingCount: number
  slotEnd: string
  slotId: string
  slotStart: string
}

const slots: MockBookableSlot[] = [
  {
    slotId: 'SLOT-OP-001',
    slotStart: `${today} 08:00:00`,
    slotEnd: `${today} 08:30:00`,
    remainingCount: 5,
    organizationId: '1001001',
    organizationName: '心血管内科',
    practitionerId: '3001001',
    practitionerName: '赵主任',
    healthcareServiceId: 'SERVICE-NORMAL',
    healthcareServiceName: '普通门诊'
  },
  {
    slotId: 'SLOT-OP-002',
    slotStart: `${today} 09:00:00`,
    slotEnd: `${today} 09:30:00`,
    remainingCount: 2,
    organizationId: '1001001',
    organizationName: '心血管内科',
    practitionerId: '3001002',
    practitionerName: '钱医生',
    healthcareServiceId: 'SERVICE-EXPERT',
    healthcareServiceName: '专家门诊'
  },
  {
    slotId: 'SLOT-OP-003',
    slotStart: `${today} 14:00:00`,
    slotEnd: `${today} 14:30:00`,
    remainingCount: 4,
    organizationId: '1001002',
    organizationName: '呼吸内科',
    practitionerId: '3002001',
    practitionerName: '孙医生',
    healthcareServiceId: 'SERVICE-NORMAL',
    healthcareServiceName: '普通门诊'
  }
]

type MockRegisterHistoryRecord = RegisterHistoryRecord & {
  patientId?: string
  printCount: number
  slotId?: string
}

const records: MockRegisterHistoryRecord[] = []
const drafts = new Map<
  string,
  { payload: PreSettleOutpatientRegisterPayload; result: PreSettleOutpatientRegisterResult }
>()
const scanRecoveryDrafts = new Set<string>()

function clone<T>(value: T): T {
  return structuredClone(value)
}

function amount(value: number | string | undefined) {
  return Number(value ?? 0)
}

function wait(context: RequestContext = {}, duration = 160) {
  return new Promise<void>((resolve, reject) => {
    if (context.signal?.aborted) {
      reject(new DOMException('请求已取消', 'AbortError'))
      return
    }
    const timer = setTimeout(resolve, duration)
    context.signal?.addEventListener(
      'abort',
      () => {
        clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      },
      { once: true }
    )
  })
}

export async function mockLoadOutpatientRegisterInit(
  _query: LoadOutpatientRegisterInitQuery = {},
  context: RequestContext = {}
) {
  await wait(context)
  const result: RegisterInitResult = {
    departmentOptions: departments,
    serviceTypeOptions: [
      { value: 'SERVICE-NORMAL', label: '普通门诊' },
      { value: 'SERVICE-EXPERT', label: '专家门诊' }
    ],
    contractOptions: [
      { value: 'CONTRACT-SELF', label: '自费' },
      { value: 'CONTRACT-MI', label: '职工医保' }
    ],
    paymentChannelOptions: [
      { value: 'CASH', label: '现金' },
      { value: 'BANK_CARD', label: '银行卡' },
      { value: 'SCAN_PAY', label: '扫码支付' }
    ],
    genderOptions: [
      { value: 'MALE', label: '男' },
      { value: 'FEMALE', label: '女' },
      { value: 'UNKNOWN', label: '未知' }
    ],
    practitionerOptions: practitioners.map(({ disabled, label, value }) => ({
      disabled,
      label,
      value
    })),
    contractCategoryById: { 'CONTRACT-SELF': '1', 'CONTRACT-MI': '3' },
    contractInsuranceProfileById: {
      'CONTRACT-MI': {
        busNo: 'YB-CONTRACT-01',
        contractId: 'CONTRACT-MI',
        categoryCode: '3',
        treatmentAreaCode: '229900',
        ybFlag: true
      }
    },
    serviceTypeFeeById: { 'SERVICE-NORMAL': 20, 'SERVICE-EXPERT': 50 }
  }
  return clone(result)
}

export async function mockPageRegisterPatientOptions(
  query: RegisterPatientQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const filtered = patients.filter((patient) =>
    `${patient.name}${patient.patientBusNo}${patient.idCardNoMasked}${patient.phoneMasked}`
      .toLowerCase()
      .includes(keyword)
  )
  const start = (query.pageNo - 1) * query.pageSize
  const result: RegisterPatientPage = {
    records: clone(filtered.slice(start, start + query.pageSize)),
    total: filtered.length
  }
  return result
}

export async function mockListRegisterPractitioners(
  query: RegisterPractitionerQuery,
  context: RequestContext = {}
) {
  await wait(context, 100)
  const normalized = query.keyword?.trim().toLowerCase() ?? ''
  const result: RegisterPractitionerResult = {
    practitionerOptions: clone(
      practitioners
        .filter(
          (item) =>
            (!query.organizationId || item.organizationId === String(query.organizationId)) &&
            (!normalized || item.label.toLowerCase().includes(normalized))
        )
        .map(({ disabled, label, value }) => ({ disabled, label, value }))
    )
  }
  return result
}

export async function mockLoadRegisterBenefitPreview(
  payload: LoadRegisterBenefitPreviewParams,
  context: RequestContext = {}
) {
  await wait(context)
  const insurance = String(payload.contractId) === 'CONTRACT-MI'
  if (insurance && !payload.patientId && !payload.ybCardInfo) {
    throw new Error('医保身份信息不足，请选择患者或重新读取医保凭证')
  }
  if (insurance && payload.ybCardInfo?.toUpperCase().includes('EXPIRED')) {
    throw new Error('医保凭证已过期，请重新读取')
  }
  const result: LoadRegisterBenefitPreviewResult = insurance
    ? {
        benefitKind: 'INSURANCE',
        contractMatch: {
          blockRegister: false,
          hintMessage: '费用性质与当前参保地匹配',
          matchStatus: 'MATCH',
          patientInsuplcAdmdvs: payload.insuplcAdmdvs ?? '229900',
          recommendedCategoryCode: '3',
          recommendedContractId: 'CONTRACT-MI',
          recommendedContractName: '职工医保',
          selectedContractId: String(payload.contractId),
          selectedTreatmentAreaCode: '229900'
        },
        ybPreview: {
          areaHint: {
            areaKind: 'LOCAL',
            hintMessage: '参保地与就医地一致',
            insuplcAdmdvs: payload.insuplcAdmdvs ?? '229900',
            mdtrtareaAdmdvs: '229900'
          },
          balc: 2680.5,
          certno: payload.ybCertno,
          insuinfoList: [
            {
              balc: 2680.5,
              insuplcAdmdvs: payload.insuplcAdmdvs ?? '229900',
              insutype: payload.insutype ?? '310',
              psnInsuStas: '1',
              psnType: '11'
            }
          ],
          insuplcAdmdvs: payload.insuplcAdmdvs ?? '229900',
          insutype: payload.insutype ?? '310',
          psnName: payload.ybPsnName,
          psnNo: payload.psnNo ?? `PSN-${String(payload.patientId ?? 'TEMP')}`,
          treatmentBlocked: false,
          treatmentInfoList: payload.runTreatmentCheck
            ? [
                {
                  psnNo: payload.psnNo ?? `PSN-${String(payload.patientId ?? 'TEMP')}`,
                  trtChkRslt: '1',
                  trtChkType: '1',
                  trtEnjymntFlag: '1'
                }
              ]
            : []
        }
      }
    : {
        benefitKind: 'NONE',
        contractMatch: {
          blockRegister: false,
          matchStatus: 'MATCH',
          selectedContractId: String(payload.contractId)
        }
      }
  return clone(result)
}

export async function mockListBookableSlotHints(
  query: BookableSlotQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const result: BookableSlotResult = {
    hints: clone(
      slots
        .filter(
          (slot) =>
            slot.organizationId === query.organizationId &&
            (!query.registrationDate || slot.slotStart.startsWith(query.registrationDate)) &&
            (!query.practitionerId || slot.practitionerId === query.practitionerId) &&
            (!query.healthcareServiceId || slot.healthcareServiceId === query.healthcareServiceId)
        )
        .map(({ remainingCount, slotEnd, slotId, slotStart }) => ({
          remainingCount,
          slotEnd,
          slotId,
          slotStart
        }))
    )
  }
  return result
}

export async function mockGetRegisterAppointment(
  query: RegisterAppointmentQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const slot = slots[0]
  if (!slot) throw new Error('预约记录不存在')
  const result: RegisterAppointmentContext = {
    appointmentBusNo: query.appointmentBusNo ?? 'APT-20260728001',
    appointmentId: query.appointmentBusNo ?? 'APT-20260728001',
    healthcareServiceId: slot.healthcareServiceId ?? 'SERVICE-NORMAL',
    organizationId: slot.organizationId ?? '',
    organizationName: slot.organizationName,
    practitionerId: slot.practitionerId,
    practitionerName: slot.practitionerName,
    serviceTypeName: slot.healthcareServiceName,
    slotEnd: slot.slotEnd,
    slotId: slot.slotId,
    slotStart: slot.slotStart,
    statusEnum: 'BOOKED'
  }
  return result
}

export async function mockPreSettleOutpatientRegister(payload: PreSettleOutpatientRegisterPayload) {
  await wait()
  if (!payload.name || !payload.idCardNo || !payload.phone) throw new Error('患者身份信息不完整')
  if (!payload.organizationId || !payload.practitionerId || !payload.serviceTypeId) {
    throw new Error('科室、医生和号源类型不能为空')
  }
  const slot = payload.slotId
    ? slots.find((item) => item.slotId === String(payload.slotId))
    : undefined
  if (payload.slotId && (!slot || slot.remainingCount <= 0)) throw new Error('所选号源已不可用')
  const normalFee = payload.serviceTypeId === 'SERVICE-EXPERT' ? 50 : 10
  const treatmentFee = payload.serviceTypeId === 'SERVICE-EXPERT' ? 30 : 8
  const totalAmount = normalFee + treatmentFee
  const insuranceContract = payload.contractId === 'CONTRACT-MI'
  if (insuranceContract && !payload.psnNo) {
    throw new Error('医保挂号缺少人员编号，请先读取医保待遇信息')
  }
  const insurance = insuranceContract && Boolean(payload.ybRegisterFee)
  const paymentDraftId = `DRAFT-OP-${Date.now()}`
  const result: PreSettleOutpatientRegisterResult = {
    appointmentId: payload.appointmentId,
    chargeItems: [
      {
        amount: normalFee,
        chargeItemCode: 'REG-FEE',
        chargeItemName: '挂号费',
        quantity: 1,
        unitPrice: normalFee
      },
      {
        amount: treatmentFee,
        chargeItemCode: 'TREAT-FEE',
        chargeItemName: '诊查费',
        quantity: 1,
        unitPrice: treatmentFee
      }
    ],
    encounterId: `OP-${Date.now()}`,
    paymentDraftId,
    slotId: payload.slotId,
    totalAmount,
    insurancePreSettled: insurance,
    ybFundPaySumamt: insurance ? Number((totalAmount * 0.6).toFixed(2)) : 0,
    ybAcctPay: insurance ? Number((totalAmount * 0.1).toFixed(2)) : 0,
    ybPsnCashPay: insurance ? Number((totalAmount * 0.3).toFixed(2)) : totalAmount
  }
  drafts.set(paymentDraftId, { payload: clone(payload), result: clone(result) })
  return result
}

export async function mockConfirmOutpatientRegister(payload: ConfirmOutpatientRegisterPayload) {
  await wait()
  const draft = drafts.get(String(payload.paymentDraftId))
  if (!draft) throw new Error('挂号预结算已失效，请重新确认')
  if (payload.clientScanResult && payload.clientScanResult.respCode !== '00') {
    throw new Error(payload.clientScanResult.respMsg || '扫码支付失败')
  }
  const draftId = String(payload.paymentDraftId)
  if (payload.clientScanResult?.tradeNo.includes('RECOVER') && !scanRecoveryDrafts.has(draftId)) {
    scanRecoveryDrafts.add(draftId)
    throw new Error('扫码扣款已确认，本地挂号结算尚未完成')
  }
  const paidAmount = payload.paymentDetails.reduce((sum, item) => sum + amount(item.payAmount), 0)
  const expectedAmount = draft.result.insurancePreSettled
    ? amount(draft.result.ybPsnCashPay)
    : amount(draft.result.totalAmount)
  if (Math.abs(paidAmount - expectedAmount) > 0.001) throw new Error('缴费金额与应收金额不一致')
  const slot = draft.result.slotId
    ? slots.find((item) => item.slotId === String(draft.result.slotId))
    : undefined
  if (slot && slot.remainingCount <= 0) throw new Error('所选号源已被占用')
  if (slot) slot.remainingCount -= 1
  const paymentId = `PAY-OP-${Date.now()}`
  const doctor = practitioners.find((item) => item.value === String(draft.payload.practitionerId))
  const department = departments.find((item) => item.value === String(draft.payload.organizationId))
  const service = (await mockLoadOutpatientRegisterInit()).serviceTypeOptions.find(
    (item) => item.value === String(draft.payload.serviceTypeId)
  )
  records.unshift({
    amount: amount(draft.result.totalAmount),
    canCancel: true,
    contactPhone: draft.payload.phone,
    doctorName: doctor?.label ?? String(draft.payload.practitionerId),
    encounterBusNo: `OP2026${String(records.length + 1).padStart(5, '0')}`,
    encounterId: String(draft.result.encounterId),
    organizationName: department?.label ?? String(draft.payload.organizationId),
    patientGenderCode: draft.payload.gender,
    patientGenderText: draft.payload.gender === 'FEMALE' ? '女' : '男',
    patientName: draft.payload.name,
    patientId: draft.payload.patientId == null ? undefined : String(draft.payload.patientId),
    paymentDraftId: String(payload.paymentDraftId),
    paymentId,
    printCount: 0,
    serviceTypeId: String(draft.payload.serviceTypeId),
    serviceTypeName: service?.label ?? String(draft.payload.serviceTypeId),
    startTime: slot?.slotStart ?? `${today} 10:00:00`,
    statusEnum: 'PLANNED',
    statusText: '已挂号',
    hasLinkedAppointment: Boolean(draft.payload.appointmentId),
    contractName: draft.payload.contractId === 'CONTRACT-MI' ? '职工医保' : '自费',
    insurancePlatformRegistered: draft.payload.contractId === 'CONTRACT-MI',
    insurancePreSettled: draft.result.insurancePreSettled,
    ybFundPaySumamt: amount(draft.result.ybFundPaySumamt),
    ybAcctPay: amount(draft.result.ybAcctPay),
    ybPsnCashPay: amount(draft.result.ybPsnCashPay),
    slotId: draft.result.slotId == null ? undefined : String(draft.result.slotId)
  })
  drafts.delete(String(payload.paymentDraftId))
  scanRecoveryDrafts.delete(draftId)
  const result: ConfirmOutpatientRegisterResult = {
    encounterId: draft.result.encounterId,
    invoiceId: `INV-OP-${Date.now()}`,
    invoiceStatus: 'ISSUED',
    paymentId,
    status: 'SUCCESS'
  }
  return result
}

export async function mockPageOutpatientRegisterHistory(
  query: RegisterHistoryQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const filtered = records.filter((record) => {
    const date = record.startTime.slice(0, 10)
    if (date < query.dateFrom || date > query.dateTo) return false
    if (query.patientId && record.patientId !== query.patientId) return false
    return (
      !keyword ||
      `${record.patientName}${record.encounterBusNo}${record.doctorName}`
        .toLowerCase()
        .includes(keyword)
    )
  })
  const start = (query.pageNo - 1) * query.pageSize
  const result: RegisterHistoryPage = {
    records: clone(filtered.slice(start, start + query.pageSize)),
    total: filtered.length
  }
  return result
}

export async function mockGetTodayRegisterSummary(
  query: TodayRegisterSummaryQuery = {},
  context: RequestContext = {}
) {
  await wait(context, 100)
  const date = query.date ?? today
  const todayRecords = records.filter((record) => record.startTime.startsWith(date))
  const result: TodayRegisterSummary = {
    date,
    totalCount: todayRecords.length,
    activeCount: todayRecords.filter((record) => record.statusEnum !== 'CANCELLED').length,
    cancelledCount: todayRecords.filter((record) => record.statusEnum === 'CANCELLED').length
  }
  return result
}

export async function mockValidateRegisterCancellation(
  payload: ValidateRegisterCancellationPayload,
  context: RequestContext = {}
) {
  await wait(context)
  const record = records.find((item) => item.encounterId === String(payload.encounterId))
  if (!record) throw new Error('挂号记录不存在')
  const result: RegisterCancellationValidation = {
    cancelable: Boolean(record.canCancel && record.statusEnum === 'PLANNED'),
    reasonCode: record.statusEnum === 'IN_PROGRESS' ? 'IN_TREATMENT' : undefined,
    reasonText: record.statusEnum === 'IN_PROGRESS' ? '患者已进入诊疗流程' : undefined
  }
  return result
}

async function cancelRecord(
  payload: CancelOutpatientRegisterPayload | CancelPlannedRegistrationPayload,
  refund: boolean
) {
  await wait()
  if (!(payload.cancelReason ?? '').trim()) throw new Error('请填写退号原因')
  const record = records.find((item) => item.encounterId === String(payload.encounterId))
  if (!record) throw new Error('挂号记录不存在')
  if (!record.canCancel || record.statusEnum !== 'PLANNED') throw new Error('当前记录不允许退号')
  if (refund && !record.paymentId) throw new Error('当前记录无需退款')
  record.statusEnum = 'CANCELLED'
  record.statusText = '已退号'
  record.canCancel = false
  const slot = slots.find((item) => item.slotId === record.slotId)
  if (slot) slot.remainingCount += 1
  const result = {
    encounterStatus: 'CANCELLED',
    redInvoiceId: refund ? `RED-INV-${Date.now()}` : undefined,
    refundStatus: refund ? 'SUCCESS' : undefined,
    appointmentReleaseMessage: slot ? '关联号源已释放' : '挂号记录已取消'
  }
  return result
}

export function mockCancelOutpatientRegister(payload: CancelPlannedRegistrationPayload) {
  return cancelRecord(payload, false).then<CancelPlannedRegistrationResult>((result) => ({
    encounterStatus: result.encounterStatus,
    insurancePlatformRevoked: false
  }))
}

export function mockCancelRegisterWithRefund(payload: CancelOutpatientRegisterPayload) {
  return cancelRecord(payload, true).then<CancelOutpatientRegisterResult>((result) => ({
    appointmentReleaseMessage: result.appointmentReleaseMessage,
    encounterStatus: result.encounterStatus,
    redInvoiceId: result.redInvoiceId,
    refundStatus: result.refundStatus ?? 'SUCCESS'
  }))
}

export async function mockGetRegisterTicket(
  query: RegisterTicketQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const record = records.find(
    (item) =>
      (!query.encounterId || item.encounterId === String(query.encounterId)) &&
      (!query.paymentId || item.paymentId === String(query.paymentId))
  )
  if (!record || record.statusEnum === 'CANCELLED') throw new Error('当前记录没有可打印的挂号小票')
  record.printCount += 1
  const result: RegisterTicketResult = {
    printCount: record.printCount,
    slip: {
      accountBalanceText: '¥0.00',
      accountPayText: `¥${Number(record.ybAcctPay ?? 0).toFixed(2)}`,
      ageText: '',
      alipayPayText: '¥0.00',
      amount: record.amount,
      cashierName: '收费员01',
      cashPayText: `¥${record.amount.toFixed(2)}`,
      chargeItemLines: [
        {
          amountText: `¥${record.amount.toFixed(2)}`,
          chargeCategoryText: '挂号费',
          itemName: record.serviceTypeName ?? '门诊挂号'
        }
      ],
      chargeTimeText: new Date().toISOString().replace('T', ' ').slice(0, 19),
      deductibleLineText: '¥0.00',
      doctorName: record.doctorName,
      encounterId: record.encounterId,
      feeNatureText: record.contractName,
      fundPayText: `¥${Number(record.ybFundPaySumamt ?? 0).toFixed(2)}`,
      genderEnumText: record.patientGenderText,
      hospitalName: '零壹医院',
      organizationName: record.organizationName,
      patientName: record.patientName,
      poolingPayText: '¥0.00',
      printCount: record.printCount,
      registerDepartmentName: record.organizationName,
      serviceTypeName: record.serviceTypeName,
      slipNo: `REG-${record.encounterBusNo}`,
      statusText: record.statusText,
      totalAmountText: `¥${record.amount.toFixed(2)}`,
      wechatPayText: '¥0.00'
    }
  }
  return result
}

export function getOutpatientRegisterSnapshot() {
  return { records: clone(records), slots: clone(slots) }
}
