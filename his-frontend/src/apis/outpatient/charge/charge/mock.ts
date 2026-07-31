import type {
  ChargeDetail,
  ChargeItem,
  ChargePatient,
  ChargeQueueQuery,
  ChargeRecord,
  ConfirmOutpatientChargeSettlementResponse,
  InsuranceMode,
  PageResult,
  PaymentStatus,
  PreSettleOutpatientChargesRequest,
  PreSettlement,
  ReceiptData,
  PrintChargeReceiptParams,
  RequestContext,
  SettleChargePayload
} from './type'

const patients: ChargePatient[] = [
  {
    id: 'patient-1',
    encounterId: 'enc-1001',
    patientNo: 'P000101',
    visitNo: 'V20260725001',
    name: '张明',
    gender: '男',
    age: 42,
    departmentName: '心血管内科',
    visitTime: '2026-07-25 08:32',
    status: 'pending',
    paymentType: '医保'
  },
  {
    id: 'patient-2',
    encounterId: 'enc-1002',
    patientNo: 'P000102',
    visitNo: 'V20260725002',
    name: '刘芳',
    gender: '女',
    age: 36,
    departmentName: '呼吸内科',
    visitTime: '2026-07-25 09:05',
    status: 'pending',
    paymentType: '自费'
  },
  {
    id: 'patient-3',
    encounterId: 'enc-1003',
    patientNo: 'P000103',
    visitNo: 'V20260725003',
    name: '陈勇',
    gender: '男',
    age: 58,
    departmentName: '普通外科',
    visitTime: '2026-07-25 09:46',
    status: 'settled',
    paymentType: '医保'
  }
]
const items = new Map<string, ChargeItem[]>([
  [
    'enc-1001',
    [
      {
        id: 'charge-1',
        category: '检查',
        itemName: '心电图检查',
        specification: '十二导联',
        quantity: 1,
        unitPrice: 35,
        amount: 35,
        insuranceAmount: 28,
        selfPayAmount: 7,
        status: 'pending'
      },
      {
        id: 'charge-2',
        category: '药品',
        itemName: '阿托伐他汀钙片',
        specification: '20mg*14片',
        quantity: 2,
        unitPrice: 38.5,
        amount: 77,
        insuranceAmount: 61.6,
        selfPayAmount: 15.4,
        status: 'pending'
      }
    ]
  ],
  [
    'enc-1002',
    [
      {
        id: 'charge-3',
        category: '检验',
        itemName: '血常规',
        specification: '五分类',
        quantity: 1,
        unitPrice: 25,
        amount: 25,
        insuranceAmount: 0,
        selfPayAmount: 25,
        status: 'pending'
      },
      {
        id: 'charge-4',
        category: '药品',
        itemName: '盐酸氨溴索口服液',
        specification: '100ml',
        quantity: 1,
        unitPrice: 22,
        amount: 22,
        insuranceAmount: 0,
        selfPayAmount: 22,
        status: 'pending'
      }
    ]
  ],
  [
    'enc-1003',
    [
      {
        id: 'charge-5',
        category: '治疗',
        itemName: '换药',
        specification: '小换药',
        quantity: 1,
        unitPrice: 20,
        amount: 20,
        insuranceAmount: 16,
        selfPayAmount: 4,
        status: 'settled',
        recordId: 'record-1'
      }
    ]
  ]
])
const records: ChargeRecord[] = [
  {
    id: 'record-1',
    encounterId: 'enc-1003',
    itemIds: ['charge-5'],
    receiptNo: 'RC202607250001',
    amount: 20,
    channel: 'card',
    status: 'success',
    receiptStatus: 'failed',
    printStatus: 'scheduled',
    receiptFailureReason: '电子票据平台返回状态不一致，请核对后重试开票',
    createdAt: '2026-07-25 10:10'
  }
]
const payments = new Map<string, PaymentStatus>()
let sequence = 10

function clone<T>(value: T): T {
  return structuredClone(value)
}
function delay(context: RequestContext = {}, ms = 120) {
  return new Promise<void>((resolve, reject) => {
    if (context.signal?.aborted) {
      reject(new DOMException('请求已取消', 'AbortError'))
      return
    }
    const timer = setTimeout(resolve, ms)
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
function patientOf(encounterId: string) {
  const row = patients.find((item) => item.encounterId === encounterId)
  if (!row) throw new Error('患者就诊信息不存在')
  return row
}
function summaryOf(rows: ChargeItem[]) {
  return rows.reduce(
    (sum, item) => ({
      totalAmount: sum.totalAmount + item.amount,
      insuranceAmount: sum.insuranceAmount + item.insuranceAmount,
      selfPayAmount: sum.selfPayAmount + item.selfPayAmount
    }),
    { totalAmount: 0, insuranceAmount: 0, selfPayAmount: 0 }
  )
}

function syncPatientFields(row: ChargePatient) {
  const patientItems = items.get(row.encounterId) ?? []
  row.patientId = row.id
  row.encounterBusNo = row.visitNo
  row.patientName = row.name
  row.genderCode = row.gender === '男' ? 'M' : 'F'
  row.genderEnumText = row.gender
  row.ageText = `${row.age}岁`
  row.organizationName = row.departmentName
  row.doctorName =
    row.departmentName === '心血管内科'
      ? '王建国'
      : row.departmentName === '呼吸内科'
        ? '李文静'
        : '赵海峰'
  row.startTime = row.visitTime
  row.queueLabelCode = row.status === 'pending' ? 'PENDING' : 'SETTLED'
  row.totalAmount = summaryOf(patientItems).totalAmount
  row.paidAmount = summaryOf(patientItems.filter((item) => item.status === 'settled')).totalAmount
  row.paymentId = [...records]
    .reverse()
    .find((record) => record.encounterId === row.encounterId)?.id
  return row
}

function syncItemFields(row: ChargeItem) {
  row.accountId = `ACCOUNT-${row.id}`
  row.chargeItemId = row.id
  row.busNo = row.id
  row.cashierName = '收费员'
  row.cashierUserId = 'OPERATOR-B'
  row.chargeCategoryCode = row.category
  row.chargeCategoryDisplay = row.category
  row.contractId = row.insuranceAmount > 0 ? 'CONTRACT-MI' : 'CONTRACT-SELF'
  row.discountRateDisplay = '100%'
  row.medicalInsuranceCode = `MI-${row.id}`
  row.medTypeCode = '11'
  row.feeNatureDisplay = row.insuranceAmount > 0 ? '医保' : '自费'
  row.quantityDisplay = `${row.quantity}${row.quantity === 1 ? '次' : '份'}`
  row.quantityValue = row.quantity
  row.quantityUnit = row.quantity === 1 ? '次' : '份'
  row.serviceId = row.id
  row.serviceTable = 'outpatient_charge_item'
  row.totalPrice = row.amount
  row.statusCode = row.status === 'pending' ? 'PENDING' : 'SETTLED'
  row.statusText = row.status === 'pending' ? '待收费' : '已收费'
  return row
}

function syncRecordFields(row: ChargeRecord) {
  row.paymentId = row.id
  row.paymentDetails = [{ amount: row.amount, payChannelCode: row.channel }]
  return row
}

items.forEach((rows) => rows.forEach(syncItemFields))
records.forEach(syncRecordFields)
patients.forEach(syncPatientFields)

export async function mockPageChargePatientQueue(
  query: ChargeQueueQuery,
  context: RequestContext = {}
): Promise<PageResult<ChargePatient>> {
  await delay(context)
  const keyword = query.keyword?.trim().toLowerCase()
  const status =
    query.queueTabCode === 'PENDING'
      ? 'pending'
      : query.queueTabCode === 'SETTLED'
        ? 'settled'
        : query.queueTabCode === 'ALL'
          ? 'all'
          : query.status
  const filtered = patients.filter(
    (item) =>
      (status === 'all' || item.status === status) &&
      (!query.startDate || item.visitTime.slice(0, 10) >= query.startDate) &&
      (!query.endDate || item.visitTime.slice(0, 10) <= query.endDate) &&
      (!keyword || `${item.name}${item.patientNo}${item.visitNo}`.toLowerCase().includes(keyword))
  )
  patients.forEach(syncPatientFields)
  const start = ((query.pageNo ?? query.page) - 1) * query.pageSize
  const rows = clone(filtered.slice(start, start + query.pageSize))
  return { rows, records: clone(rows), total: filtered.length }
}
export async function mockGetPendingChargeDetail(
  encounterId: string,
  context: RequestContext = {}
): Promise<ChargeDetail> {
  await delay(context)
  const patient = patientOf(encounterId)
  const rows = items.get(encounterId) ?? []
  const mode: InsuranceMode = patient.paymentType === '医保' ? 'insurance' : 'self-pay'
  const latestSettledRecord = [...records]
    .reverse()
    .find((record) => record.encounterId === encounterId && record.status === 'success')
  return clone({
    encounterId,
    patient,
    items: rows,
    insuranceMode: mode,
    summary: summaryOf(rows),
    interruptedPaymentId: records.find(
      (record) => record.encounterId === encounterId && record.status === 'pending'
    )?.id,
    latestSettledRecordId: latestSettledRecord?.id,
    receiptWarning:
      latestSettledRecord?.receiptStatus === 'failed'
        ? latestSettledRecord.receiptFailureReason
        : undefined
  })
}
export async function mockPreSettleOutpatientCharge(
  data: PreSettleOutpatientChargesRequest,
  context: RequestContext = {}
): Promise<PreSettlement> {
  await delay(context)
  if (!data.chargeItemIds.length) throw new Error('请至少选择一个收费项目')
  if (
    records.some((record) => record.encounterId === data.encounterId && record.status === 'pending')
  )
    throw new Error('当前就诊存在未完成支付，请先查询或恢复结算')
  const patient = patientOf(data.encounterId)
  const selected = (items.get(data.encounterId) ?? []).filter(
    (item) => data.chargeItemIds.includes(item.id) && item.status === 'pending'
  )
  if (selected.length !== data.chargeItemIds.length) throw new Error('部分项目状态已变化，请刷新后重试')
  const settlementId = `settlement-${sequence++}`
  const summary = summaryOf(selected)
  return {
    paymentDraftId: settlementId,
    displayAmount: summary.totalAmount,
    previews: selected.map((item) => ({
      chargeItemId: item.id,
      itemLabel: item.itemName,
      lineAmount: item.amount,
      quantityDisplay: item.quantityDisplay ?? String(item.quantity),
      quantityUnit: item.quantityUnit ?? '',
      feeNatureDisplay: item.feeNatureDisplay ?? ''
    })),
    insuranceGroups:
      patient.paymentType === '医保'
        ? [
            {
              accountId: `ACCOUNT-${patient.id}`,
              chargeItemIds: selected.map((item) => item.id),
              chrgBchno: `CHRG-${settlementId}`,
              contractId: 'CONTRACT-MI',
              diseCodg: 'I10',
              diseName: '门诊诊断',
              groupIndex: 0,
              mdtrtId: patient.encounterId,
              medType: '11',
              medTypeDisplay: '普通门诊',
              medfeeSumamt: summary.totalAmount,
              preSettleId: `YB-PRE-${settlementId}`,
              fundPaySumamt: summary.insuranceAmount,
              acctPay: 0,
              psnCashPay: summary.selfPayAmount
            }
          ]
        : [],
    selfPayRollup:
      patient.paymentType === '医保'
        ? null
        : {
            accountId: `ACCOUNT-${patient.id}`,
            chargeItemIds: selected.map((item) => item.id),
            totalAmount: summary.totalAmount
          },
    ybFundPaySumamt: summary.insuranceAmount,
    ybAcctPay: 0,
    ybPsnCashPay: summary.selfPayAmount,
    ybMedfeeSumamt: summary.totalAmount,
    ybChrgBchno: patient.paymentType === '医保' ? `CHRG-${settlementId}` : undefined,
    insurancePreSettled: patient.paymentType === '医保',
    settlementComposition: patient.paymentType === '医保' ? 'INSURANCE' : 'SELF_PAY',
  }
}
export async function mockSettleOutpatientCharge(
  data: SettleChargePayload,
  context: RequestContext = {}
): Promise<ConfirmOutpatientChargeSettlementResponse> {
  await delay(context, 300)
  const selected = (items.get(data.encounterId) ?? []).filter((item) =>
    data.chargeItemIds.includes(item.id)
  )
  if (selected.length !== data.chargeItemIds.length || selected.some((item) => item.status !== 'pending'))
    throw new Error('项目已经结算，请勿重复收费')
  const channel = String(data.paymentDetails[0]?.payChannelCode ?? '') as ChargeRecord['channel']
  if (!['cash', 'card', 'wechat', 'alipay'].includes(channel)) throw new Error('支付渠道无效')
  const paymentStatus: PaymentStatus = ['wechat', 'alipay'].includes(channel)
    ? 'pending'
    : 'success'
  const record: ChargeRecord = {
    id: `record-${sequence++}`,
    invoiceId: `INV-${sequence}`,
    invoiceStatus: paymentStatus === 'success' ? 'ISSUED' : 'PENDING',
    successMessageCode: paymentStatus === 'success' ? 'OUTPATIENT_CHARGE_SUCCESS' : undefined,
    ybSetlId:
      patientOf(data.encounterId).paymentType === '医保' ? `YB-SETL-${sequence}` : undefined,
    encounterId: data.encounterId,
    itemIds: [...data.chargeItemIds],
    receiptNo: `RC20260725${String(sequence).padStart(4, '0')}`,
    amount: summaryOf(selected).totalAmount,
    channel,
    status: paymentStatus,
    receiptStatus: paymentStatus === 'success' ? 'issued' : 'pending',
    printStatus: paymentStatus === 'success' ? 'auto-queued' : 'scheduled',
    createdAt: '2026-07-25 12:00'
  }
  syncRecordFields(record)
  records.push(record)
  payments.set(record.id, paymentStatus)
  if (paymentStatus === 'success') {
    selected.forEach((item) => {
      item.status = 'settled'
      item.recordId = record.id
      syncItemFields(item)
    })
    patientOf(data.encounterId).status = (items.get(data.encounterId) ?? []).some(
      (item) => item.status === 'pending'
    )
      ? 'pending'
      : 'settled'
    syncPatientFields(patientOf(data.encounterId))
  }
  return {
    invoiceId: record.invoiceId!,
    invoiceStatus: record.invoiceStatus,
    paymentId: record.id,
    successMessageCode: record.successMessageCode,
    ybSetlId: record.ybSetlId
  }
}
export async function mockSwitchChargeInsuranceMode(
  data: { encounterId: string; mode: InsuranceMode },
  context: RequestContext = {}
): Promise<ChargeDetail> {
  await delay(context)
  const patient = patientOf(data.encounterId)
  patient.paymentType = data.mode === 'insurance' ? '医保' : '自费'
  const rows = items.get(data.encounterId) ?? []
  rows
    .filter((item) => item.status === 'pending')
    .forEach((item) => {
      item.insuranceAmount = data.mode === 'insurance' ? Number((item.amount * 0.8).toFixed(2)) : 0
      item.selfPayAmount = Number((item.amount - item.insuranceAmount).toFixed(2))
      syncItemFields(item)
    })
  return mockGetPendingChargeDetail(data.encounterId, context)
}
export async function mockQueryPaymentResult(paymentId: string, context: RequestContext = {}) {
  await delay(context, 180)
  if (!payments.has(paymentId)) throw new Error('支付记录不存在')
  payments.set(paymentId, 'success')
  const record = records.find((item) => item.id === paymentId)!
  record.status = 'success'
  record.receiptStatus = 'issued'
  record.printStatus = 'scheduled'
  const rows = items.get(record.encounterId) ?? []
  rows
    .filter((item) => record.itemIds.includes(item.id))
    .forEach((item) => {
      item.status = 'settled'
      item.recordId = paymentId
      syncItemFields(item)
    })
  patientOf(record.encounterId).status = rows.some((item) => item.status === 'pending')
    ? 'pending'
    : 'settled'
  syncPatientFields(patientOf(record.encounterId))
  syncRecordFields(record)
  return clone(record)
}
export async function mockRecoverSettlement(paymentId: string, context: RequestContext = {}) {
  return mockQueryPaymentResult(paymentId, context)
}
export async function mockGetChargeReceipt(
  params: PrintChargeReceiptParams,
  context: RequestContext = {}
): Promise<ReceiptData> {
  await delay(context)
  const record = records.find(
    (item) =>
      item.status === 'success' &&
      (!params.paymentId || (item.paymentId ?? item.id) === params.paymentId) &&
      (!params.encounterId || item.encounterId === params.encounterId)
  )
  if (!record) throw new Error('只有收费成功的记录可以打印票据')
  if (record.receiptStatus !== 'issued')
    throw new Error(record.receiptFailureReason ?? '电子票据尚未开具，暂不能打印')
  record.printStatus = 'printed'
  const patient = patientOf(record.encounterId)
  const chargeItems = (items.get(record.encounterId) ?? []).filter((item) =>
    record.itemIds.includes(item.id)
  )
  return {
    recordId: record.id,
    receiptNo: record.receiptNo,
    patientName: patient.name,
    amount: record.amount,
    issuedAt: record.createdAt,
    printStatus: record.printStatus,
    previewText: `${record.receiptNo} · ${patient.name} · ¥${record.amount.toFixed(2)}`,
    slip: {
      encounterId: record.encounterId,
      patientName: patient.name,
      hospitalName: '零壹医院',
      genderEnumText: patient.genderEnumText ?? patient.gender,
      ageText: patient.ageText ?? `${patient.age}岁`,
      feeNatureText: patient.paymentType,
      chargeItemLines: chargeItems.map((item) => ({
        itemName: item.itemName,
        quantityText: item.quantityDisplay ?? String(item.quantity),
        amountText: `¥${item.amount.toFixed(2)}`,
        chargeCategoryText: item.chargeCategoryDisplay ?? item.category
      })),
      totalAmountText: `¥${record.amount.toFixed(2)}`,
      poolingPayText: `¥${chargeItems.reduce((sum, item) => sum + item.insuranceAmount, 0).toFixed(2)}`,
      fundPayText: `¥${chargeItems.reduce((sum, item) => sum + item.insuranceAmount, 0).toFixed(2)}`,
      accountPayText: '¥0.00',
      deductibleLineText: '¥0.00',
      cashPayText: record.channel === 'cash' ? `¥${record.amount.toFixed(2)}` : '¥0.00',
      wechatPayText: record.channel === 'wechat' ? `¥${record.amount.toFixed(2)}` : '¥0.00',
      alipayPayText: record.channel === 'alipay' ? `¥${record.amount.toFixed(2)}` : '¥0.00',
      accountBalanceText: '¥0.00',
      cashierName: '收费员01',
      chargeTimeText: record.createdAt
    }
  }
}

export function getChargeSnapshot() {
  return clone({ patients, items: [...items], records, payments: [...payments] })
}
