import type {
  ConfirmInpatientSettlePayload,
  ConfirmInpatientSettleResult,
  InpatientSettleChargeCategory,
  InpatientSettleChargeLine,
  InpatientSettleChargeQuery,
  InpatientSettleChargeResult,
  InpatientSettleInitResult,
  InpatientSettlePatient,
  InpatientSettleQueuePage,
  InpatientSettleQueueQuery,
  InpatientSettleSummary,
  InpatientSettleSummaryQuery,
  InpatientSettlementHistory,
  PreSettleInpatientPayload,
  PreSettleInpatientResult,
  RequestContext,
  ReverseInpatientSettlePayload,
  SettledInpatientPaymentQuery,
  ReverseInpatientSettleResult
} from './type'

const patients: InpatientSettlePatient[] = [
  {
    accountBalance: 1200,
    accountBalanceDisplayMode: 'NORMAL',
    accountBalanceSubText: '可用余额 ¥1200.00',
    accountBalanceSummaryText: '余额充足',
    availableBalance: 1200,
    ageText: '48岁',
    bedName: '12床',
    contractDisplay: '职工医保',
    contractId: 'CONTRACT-MI',
    encounterBusNo: 'IP20260725001',
    encounterId: '820000000000000001',
    genderEnum: 'MALE',
    genderEnumText: '男',
    organizationName: '心血管内科',
    patientId: '920000000000000001',
    patientName: '周明',
    pendingSettlementTotal: 486.5,
    startTime: '2026-07-25 09:10:00',
    statusEnum: 'PENDING_SETTLE',
    statusEnumText: '待结算',
    wardName: '心内一病区'
  },
  {
    accountBalance: 300,
    accountBalanceDisplayMode: 'LOW',
    accountBalanceSubText: '可用余额 ¥300.00',
    accountBalanceSummaryText: '余额可能不足',
    availableBalance: 300,
    ageText: '39岁',
    bedName: '08床',
    contractDisplay: '自费',
    contractId: 'CONTRACT-SELF',
    encounterBusNo: 'IP20260723002',
    encounterId: '820000000000000002',
    genderEnum: 'FEMALE',
    genderEnumText: '女',
    lastSettleTime: '2026-07-27 15:20:00',
    organizationName: '呼吸内科',
    patientId: '920000000000000002',
    patientName: '吴芳',
    pendingSettlementTotal: 0,
    startTime: '2026-07-23 14:20:00',
    statusEnum: 'SETTLED',
    statusEnumText: '已结算',
    wardName: '呼吸一病区'
  }
]

const chargeLines = new Map<string, InpatientSettleChargeLine[]>([
  [
    '820000000000000001',
    [
      {
        categoryCode: 'DRUG',
        categoryDisplay: '西药费',
        chargeItemId: 'CHG-10001',
        itemName: '阿司匹林肠溶片',
        lineAmount: 36.5,
        occurrenceTime: '2026-07-27 08:20:00',
        quantityUnit: '盒',
        quantityValue: 1,
        selected: true,
        sourceText: '长期医嘱',
        specText: '100mg*30片',
        statusEnum: 'PENDING',
        statusEnumText: '待结算',
        unitPrice: 36.5
      },
      {
        categoryCode: 'EXAM',
        categoryDisplay: '检查费',
        chargeItemId: 'CHG-10002',
        itemName: '心脏彩色多普勒超声',
        lineAmount: 260,
        occurrenceTime: '2026-07-27 10:00:00',
        quantityUnit: '次',
        quantityValue: 1,
        selected: true,
        sourceText: '检查申请',
        specText: '',
        statusEnum: 'PENDING',
        statusEnumText: '待结算',
        unitPrice: 260
      },
      {
        categoryCode: 'TREAT',
        categoryDisplay: '治疗费',
        chargeItemId: 'CHG-10003',
        itemName: '心电监护',
        lineAmount: 190,
        occurrenceTime: '2026-07-27 12:00:00',
        quantityUnit: '日',
        quantityValue: 1,
        selected: true,
        sourceText: '治疗执行',
        specText: '',
        statusEnum: 'PENDING',
        statusEnumText: '待结算',
        unitPrice: 190
      }
    ]
  ],
  [
    '820000000000000002',
    [
      {
        categoryCode: 'DRUG',
        categoryDisplay: '西药费',
        chargeItemId: 'CHG-20001',
        itemName: '布地奈德吸入剂',
        lineAmount: 128,
        occurrenceTime: '2026-07-26 09:00:00',
        quantityUnit: '支',
        quantityValue: 1,
        selected: false,
        sourceText: '长期医嘱',
        specText: '200吸',
        statusEnum: 'SETTLED',
        statusEnumText: '已结算',
        unitPrice: 128
      }
    ]
  ]
])

const histories = new Map<string, InpatientSettlementHistory>([
  [
    '820000000000000002',
    {
      activePaymentId: 'PAY-IP-20001',
      categories: [{ amount: 128, categoryCode: 'DRUG', categoryDisplay: '西药费', lineCount: 1 }],
      encounterId: '820000000000000002',
      lines: structuredClone(chargeLines.get('820000000000000002') ?? []),
      operatorName: '收费员01',
      payMethodText: '现金',
      payments: [
        {
          payMethodText: '现金',
          paymentId: 'PAY-IP-20001',
          receivableTotal: 128,
          recordType: 'FORWARD',
          reverseEnabled: true,
          signedAmount: 128,
          settleTime: '2026-07-27 15:20:00',
          statusEnum: 'SUCCESS'
        }
      ],
      netSettleTotal: 128,
      receivableTotal: 128,
      settleTime: '2026-07-27 15:20:00'
    }
  ]
])

const preSettlements = new Map<
  string,
  { payload: PreSettleInpatientPayload; result: PreSettleInpatientResult }
>()
const scanRecoveryDrafts = new Set<string>()

function clone<T>(value: T): T {
  return structuredClone(value)
}

function amount(value: null | number | string | undefined) {
  return Number(value ?? 0)
}

function wait(context: RequestContext = {}, duration = 180) {
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

function categoriesOf(lines: InpatientSettleChargeLine[]) {
  const map = new Map<string, InpatientSettleChargeCategory>()
  for (const line of lines) {
    const code = line.categoryCode ?? 'OTHER'
    const item = map.get(code) ?? {
      amount: 0,
      categoryCode: code,
      categoryDisplay: line.categoryDisplay ?? '其他',
      lineCount: 0
    }
    item.amount = Number((amount(item.amount) + amount(line.lineAmount)).toFixed(2))
    item.lineCount += 1
    map.set(code, item)
  }
  return [...map.values()]
}

export async function mockLoadInpatientSettleInit(context: RequestContext = {}) {
  await wait(context)
  const result: InpatientSettleInitResult = {
    defaultPresentationMode: 'DETAIL',
    defaultSettleQueueTab: 'PENDING_SETTLE',
    paymentChannelOptions: [
      { value: 'CASH', label: '现金' },
      { value: 'BANK_CARD', label: '银行卡' },
      { value: 'SCAN_PAY', label: '扫码支付' }
    ],
    presentationModeOptions: [
      { value: 'SUMMARY', label: '费用大类' },
      { value: 'DETAIL', label: '费用明细' }
    ],
    settleQueueTabOptions: [
      { value: 'PENDING_SETTLE', label: '待结算' },
      { value: 'SETTLED', label: '已结算' }
    ]
  }
  return result
}

export async function mockPageInpatientSettleQueue(
  query: InpatientSettleQueueQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const filtered = patients.filter(
    (patient) =>
      patient.statusEnum === query.settleQueueTab &&
      (!keyword ||
        `${patient.patientName}${patient.encounterBusNo}`.toLowerCase().includes(keyword)) &&
      (!query.admissionDate || patient.startTime?.startsWith(query.admissionDate))
  )
  const start = (query.pageNo - 1) * query.pageSize
  const result: InpatientSettleQueuePage = {
    content: clone(filtered.slice(start, start + query.pageSize)),
    pageNo: query.pageNo,
    pageSize: query.pageSize,
    totalCount: filtered.length,
    totalElements: filtered.length
  }
  return result
}

export async function mockGetInpatientSettleSummary(
  query: InpatientSettleSummaryQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const { encounterId, patientId, selectedChargeItemIds = [] } = query
  const patient = patients.find(
    (item) => item.encounterId === encounterId && item.patientId === patientId
  )
  if (!patient) throw new Error('住院患者不存在')
  const lines = chargeLines.get(encounterId) ?? []
  const selected = selectedChargeItemIds.length
    ? lines.filter((line) => selectedChargeItemIds.includes(line.chargeItemId))
    : lines.filter((line) => line.statusEnum === 'PENDING')
  const selectedTotal = selected.reduce((sum, line) => sum + amount(line.lineAmount), 0)
  const result: InpatientSettleSummary = {
    accountBalanceDisplayMode: patient.accountBalanceDisplayMode,
    accountBalanceSubText: patient.accountBalanceSubText,
    accountBalanceSummaryText: patient.accountBalanceSummaryText,
    availableBalance: patient.availableBalance ?? undefined,
    balanceHint: Number((amount(patient.availableBalance) - selectedTotal).toFixed(2)),
    bedName: patient.bedName,
    displayAccountBalance: amount(patient.accountBalance),
    lowBalanceWarning: amount(patient.availableBalance) < selectedTotal,
    encounterBusNo: patient.encounterBusNo,
    encounterId,
    organizationName: patient.organizationName,
    patientId,
    patientName: patient.patientName,
    pendingSettlementTotal: patient.pendingSettlementTotal ?? 0,
    prepayNetBalance: amount(patient.accountBalance),
    selectedTotal: Number(selectedTotal.toFixed(2)),
    statusEnum: patient.statusEnum,
    statusEnumText: patient.statusEnumText,
    wardName: patient.wardName
  }
  return result
}

export async function mockListInpatientSettleCharges(
  query: InpatientSettleChargeQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const allLines = clone(chargeLines.get(query.encounterId) ?? [])
  const lines = allLines.filter((line) => {
    const date = line.occurrenceTime?.slice(0, 10) ?? ''
    if (query.chargeDateFrom && date < query.chargeDateFrom) return false
    if (query.chargeDateTo && date > query.chargeDateTo) return false
    return true
  })
  const result: InpatientSettleChargeResult = {
    categories: categoriesOf(lines),
    defaultSelectAll: query.presentationMode === 'DETAIL',
    encounterId: query.encounterId,
    lines,
    pendingSettlementTotal: Number(
      lines
        .filter((line) => line.statusEnum === 'PENDING')
        .reduce((sum, line) => sum + amount(line.lineAmount), 0)
        .toFixed(2)
    ),
    presentationMode: query.presentationMode
  }
  return result
}

export async function mockListInpatientSettleChargeCategories(
  query: Omit<InpatientSettleChargeQuery, 'presentationMode'>,
  context: RequestContext = {}
) {
  return mockListInpatientSettleCharges({ ...query, presentationMode: 'SUMMARY' }, context)
}

export async function mockListInpatientSettleChargeDetails(
  query: Omit<InpatientSettleChargeQuery, 'presentationMode'>,
  context: RequestContext = {}
) {
  return mockListInpatientSettleCharges({ ...query, presentationMode: 'DETAIL' }, context)
}

export async function mockPreSettleInpatientCharges(payload: PreSettleInpatientPayload) {
  await wait()
  if (!payload.chargeItemIds.length) throw new Error('请至少选择一项待结算费用')
  const lines = chargeLines.get(payload.encounterId) ?? []
  const selected = lines.filter(
    (line) => payload.chargeItemIds.includes(line.chargeItemId) && line.statusEnum === 'PENDING'
  )
  if (selected.length !== payload.chargeItemIds.length)
    throw new Error('所选费用已变化，请刷新后重试')
  const total = Number(selected.reduce((sum, line) => sum + amount(line.lineAmount), 0).toFixed(2))
  const paymentDraftId = `DRAFT-IP-${Date.now()}`
  const insurance = payload.contractId === 'CONTRACT-MI'
  const patientCash = insurance ? Number((total * 0.3).toFixed(2)) : total
  const result: PreSettleInpatientResult = {
    displayAmount: patientCash,
    insurancePreSettled: insurance,
    paymentDraftId,
    previewText: `${payload.settleMode === 'DISCHARGE' ? '出院结算' : '中途结算'}，应收 ¥${total.toFixed(2)}`,
    receivableTotal: total,
    settlementComposition: insurance ? 'INSURANCE' : 'SELF_PAY',
    settleMode: payload.settleMode ?? 'INTERIM',
    ybAcctPay: insurance ? Number((total * 0.1).toFixed(2)) : 0,
    ybFundPaySumamt: insurance ? Number((total * 0.6).toFixed(2)) : 0,
    ybMedfeeSumamt: insurance ? total : 0,
    ybPreSettleId: insurance ? `YB-PRE-${Date.now()}` : undefined,
    ybPsnCashPay: patientCash
  }
  preSettlements.set(paymentDraftId, { payload: clone(payload), result: clone(result) })
  return result
}

export async function mockConfirmInpatientSettle(payload: ConfirmInpatientSettlePayload) {
  await wait()
  const draft = preSettlements.get(payload.paymentDraftId)
  if (!draft) throw new Error('预结算结果已失效，请重新预结算')
  if (draft.payload.encounterId !== payload.encounterId)
    throw new Error('预结算患者与当前患者不一致')
  if (payload.clientScanResult && payload.clientScanResult.respCode !== '00') {
    throw new Error(payload.clientScanResult.respMsg || '扫码支付失败')
  }
  if (
    payload.clientScanResult?.tradeNo.includes('RECOVER') &&
    !scanRecoveryDrafts.has(payload.paymentDraftId)
  ) {
    scanRecoveryDrafts.add(payload.paymentDraftId)
    throw new Error('扫码扣款已确认，本地住院结算尚未完成')
  }
  const paymentTotal = payload.paymentDetails.reduce((sum, item) => sum + amount(item.amount), 0)
  const payableAmount = draft.result.insurancePreSettled
    ? amount(draft.result.displayAmount)
    : amount(draft.result.receivableTotal)
  if (Math.abs(paymentTotal - payableAmount) > 0.001) throw new Error('支付金额与应收金额不一致')
  const lines = chargeLines.get(payload.encounterId) ?? []
  const settledLines = lines.filter((line) => payload.chargeItemIds.includes(line.chargeItemId))
  settledLines.forEach((line) => {
    line.statusEnum = 'SETTLED'
    line.statusEnumText = '已结算'
    line.selected = false
  })
  const patient = patients.find((item) => item.encounterId === payload.encounterId)
  if (!patient) throw new Error('住院患者不存在')
  patient.pendingSettlementTotal = Number(
    lines
      .filter((line) => line.statusEnum === 'PENDING')
      .reduce((sum, line) => sum + amount(line.lineAmount), 0)
      .toFixed(2)
  )
  const paymentId = `PAY-IP-${Date.now()}`
  const settleTime = new Date().toISOString().replace('T', ' ').slice(0, 19)
  const history = histories.get(payload.encounterId) ?? {
    activePaymentId: paymentId,
    categories: [],
    encounterId: payload.encounterId,
    lines: [],
    operatorName: '收费员01',
    payMethodText: payload.paymentDetails[0]?.payChannelCode,
    payments: [],
    netSettleTotal: 0,
    receivableTotal: 0,
    settleTime
  }
  history.activePaymentId = paymentId
  history.categories = categoriesOf(settledLines)
  history.lines = clone(settledLines)
  history.payments.unshift({
    payMethodText: payload.paymentDetails[0]?.payChannelCode,
    paymentId,
    receivableTotal: draft.result.receivableTotal,
    recordType: 'FORWARD',
    reverseEnabled: true,
    signedAmount: draft.result.receivableTotal,
    settleTime,
    statusEnum: 'SUCCESS'
  })
  history.netSettleTotal = Number(
    history.payments.reduce((sum, payment) => sum + amount(payment.signedAmount), 0).toFixed(2)
  )
  history.receivableTotal = draft.result.receivableTotal
  history.settleTime = settleTime
  histories.set(payload.encounterId, history)
  if ((patient.pendingSettlementTotal ?? 0) === 0) {
    patient.statusEnum = 'SETTLED'
    patient.statusEnumText = '已结算'
    patient.lastSettleTime = settleTime
  }
  preSettlements.delete(payload.paymentDraftId)
  scanRecoveryDrafts.delete(payload.paymentDraftId)
  const result: ConfirmInpatientSettleResult = {
    advanceRefundPaymentId: patient.accountBalance ? `ADV-REF-${Date.now()}` : undefined,
    dischargeFullSettle: (patient.pendingSettlementTotal ?? 0) === 0,
    insuranceSettled: Boolean(draft.result.insurancePreSettled),
    invoiceDraftId: `INV-DRAFT-${Date.now()}`,
    invoiceStatus: 'PENDING',
    paymentId,
    successMessageCode: 'INPATIENT_SETTLE_SUCCESS',
    ybSetlId: draft.result.insurancePreSettled ? `YB-SETL-${Date.now()}` : undefined
  }
  return result
}

export async function mockListInpatientSettlementHistory(
  query: SettledInpatientPaymentQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const { encounterId } = query
  return clone(
    histories.get(encounterId) ?? {
      activePaymentId: '',
      categories: [],
      encounterId,
      lines: [],
      payments: [],
      netSettleTotal: 0,
      receivableTotal: 0
    }
  )
}

export async function mockGetSettledInpatientPayment(
  query: SettledInpatientPaymentQuery,
  context: RequestContext = {}
) {
  await wait(context)
  const { paymentId } = query
  const history = [...histories.values()].find((item) =>
    item.payments.some((payment) => payment.paymentId === paymentId)
  )
  if (!history) throw new Error('结算单不存在')
  return clone(history)
}

export async function mockReverseInpatientSettle(
  payload: ReverseInpatientSettlePayload,
  context: RequestContext = {}
) {
  await wait(context)
  const { paymentId: settlementId } = payload
  const history = [...histories.values()].find((item) =>
    item.payments.some((payment) => payment.paymentId === settlementId)
  )
  if (!history) throw new Error('结算单不存在')
  const forward = history.payments.find((payment) => payment.paymentId === settlementId)
  if (!forward?.reverseEnabled)
    throw new Error(forward?.reverseDisabledReason || '当前结算单不可反结')
  forward.reverseEnabled = false
  forward.reverseDisabledReason = '已完成反结'
  const refundPaymentId = `REV-IP-${Date.now()}`
  history.payments.unshift({
    payMethodText: forward.payMethodText,
    paymentId: refundPaymentId,
    receivableTotal: forward.receivableTotal,
    recordType: 'REVERSE',
    relationId: settlementId,
    reverseEnabled: false,
    reverseDisabledReason: '逆向记录不可再次反结',
    signedAmount: -forward.receivableTotal,
    settleTime: new Date().toISOString().replace('T', ' ').slice(0, 19),
    statusEnum: 'SUCCESS'
  })
  history.netSettleTotal = Number(
    history.payments.reduce((sum, payment) => sum + amount(payment.signedAmount), 0).toFixed(2)
  )
  const lines = chargeLines.get(history.encounterId) ?? []
  ;(history.lines ?? []).forEach((settledLine) => {
    const line = lines.find((item) => item.chargeItemId === settledLine.chargeItemId)
    if (line) {
      line.statusEnum = 'PENDING'
      line.statusEnumText = '待结算'
      line.selected = true
    }
  })
  const patient = patients.find((item) => item.encounterId === history.encounterId)
  if (patient) {
    patient.statusEnum = 'PENDING_SETTLE'
    patient.statusEnumText = '待结算'
    patient.pendingSettlementTotal = Number(
      lines
        .filter((line) => line.statusEnum === 'PENDING')
        .reduce((sum, line) => sum + amount(line.lineAmount), 0)
        .toFixed(2)
    )
  }
  const result: ReverseInpatientSettleResult = { refundPaymentId, succeeded: true }
  return result
}

export function getInpatientSettleSnapshot() {
  return { patients: clone(patients), histories: clone([...histories.values()]) }
}
