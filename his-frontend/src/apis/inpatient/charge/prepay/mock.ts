import type {
  CollectPrepayPayload,
  InpatientPatient,
  InpatientQueueQuery,
  PageResult,
  PrepayRecord,
  PrepayRecordQuery,
  PrepaySummary,
  RefundPrepayPayload,
  RequestContext
} from './type'
const patients: InpatientPatient[] = [
  {
    id: 'ip-1',
    encounterId: 'inp-3001',
    patientNo: 'P000301',
    visitNo: 'ZY20260715001',
    name: '孙建军',
    gender: '男',
    age: 61,
    departmentName: '心血管内科',
    wardName: '心内科一病区',
    bedNo: '12床',
    admissionDate: '2026-07-15',
    status: 'in-hospital',
    paymentType: '医保',
    balance: 3200
  },
  {
    id: 'ip-2',
    encounterId: 'inp-3002',
    patientNo: 'P000302',
    visitNo: 'ZY20260721002',
    name: '吴梅',
    gender: '女',
    age: 47,
    departmentName: '呼吸内科',
    wardName: '呼吸科病区',
    bedNo: '08床',
    admissionDate: '2026-07-21',
    status: 'in-hospital',
    paymentType: '自费',
    balance: 1800
  }
]
const records = new Map<string, PrepayRecord[]>([
  [
    'inp-3001',
    [
      {
        id: 'prepay-1',
        encounterId: 'inp-3001',
        type: 'collect',
        amount: 5000,
        channel: 'card',
        status: 'settled',
        transactionNo: 'PP202607150001',
        createdAt: '2026-07-15 10:20',
        refundableAmount: 3200
      },
      {
        id: 'prepay-2',
        encounterId: 'inp-3001',
        type: 'refund',
        amount: 200,
        channel: 'card',
        status: 'settled',
        transactionNo: 'PR202607180001',
        createdAt: '2026-07-18 15:40',
        refundableAmount: 0
      }
    ]
  ],
  [
    'inp-3002',
    [
      {
        id: 'prepay-3',
        encounterId: 'inp-3002',
        type: 'collect',
        amount: 1800,
        channel: 'wechat',
        status: 'settled',
        transactionNo: 'PP202607210001',
        createdAt: '2026-07-21 09:10',
        refundableAmount: 1800
      }
    ]
  ]
])
const pendingPayloads = new Map<
  string,
  {
    type: 'collect' | 'refund'
    encounterId: string
    amount: number
    sourceRecordId?: string
  }
>()
let sequence = 4
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
  if (!row) throw new Error('住院患者不存在')
  return row
}

function channelText(channel: PrepayRecord['channel']) {
  return channel === 'cash'
    ? '现金'
    : channel === 'card'
      ? '银行卡'
      : channel === 'wechat'
        ? '微信'
        : '支付宝'
}

function syncPatientFields(row: InpatientPatient) {
  row.patientId = row.id
  row.encounterBusNo = row.visitNo
  row.patientName = row.name
  row.genderEnum = row.gender === '男' ? 'MALE' : 'FEMALE'
  row.genderEnumText = row.gender
  row.ageText = `${row.age}岁`
  row.organizationName = row.departmentName
  row.bedName = row.bedNo
  row.startTime = row.admissionDate
  row.statusEnum = 'IN_HOSPITAL'
  row.statusEnumText = '在院'
  row.contractDisplay = row.paymentType
  row.accountBalance = row.balance
  row.availableBalance = row.balance
  row.prepayNetBalance = row.balance
  row.pendingSettlementTotal = 0
  row.lowBalanceWarning = row.balance < 500
  return row
}

function syncRecordFields(row: PrepayRecord) {
  row.paymentId = row.id
  row.bizTypeText = row.type === 'collect' ? '预交金收款' : '预交金退款'
  row.paymentDetails = [{ amount: row.amount, payChannelCode: row.channel }]
  row.payMethodText = channelText(row.channel)
  row.paymentEnum = row.channel.toUpperCase()
  row.operatorName = '收费员'
  row.ticketNo = row.transactionNo
  row.statusEnum = row.status.toUpperCase()
  row.statusEnumText =
    row.status === 'processing'
      ? '处理中'
      : row.status === 'settled'
        ? '已结算'
        : row.status === 'refunded'
          ? '已退款'
          : '已取消'
  row.tradeTime = row.createdAt
  return row
}

records.forEach((rows) => rows.forEach(syncRecordFields))
patients.forEach(syncPatientFields)

export async function mockPageInpatientQueue(
  query: InpatientQueueQuery,
  context: RequestContext = {}
): Promise<PageResult<InpatientPatient>> {
  await delay(context)
  const key = query.keyword?.trim().toLowerCase()
  const rows = patients.filter(
    (item) =>
      (!query.admissionDate || item.admissionDate === query.admissionDate) &&
      (!query.admissionStartDate || item.admissionDate >= query.admissionStartDate) &&
      (!query.admissionEndDate || item.admissionDate <= query.admissionEndDate) &&
      (!key ||
        `${item.name}${item.patientNo}${item.visitNo}${item.bedNo}`.toLowerCase().includes(key))
  )
  patients.forEach(syncPatientFields)
  const pageNo = query.PageIndex ?? query.pageNo ?? query.page ?? 1
  const pageSize = query.PageSize ?? query.pageSize ?? 20
  const start = (pageNo - 1) * pageSize
  const pageRows = clone(rows.slice(start, start + pageSize))
  return {
    rows: pageRows,
    content: clone(pageRows),
    total: rows.length,
    totalCount: rows.length,
    pageNo,
    pageSize
  }
}
export async function mockGetInpatientPrepaySummary(
  encounterId: string,
  context: RequestContext = {}
): Promise<PrepaySummary> {
  await delay(context)
  const patient = patientOf(encounterId)
  const rows = records.get(encounterId) ?? []
  const totalCollected = rows
    .filter((item) => item.type === 'collect' && item.status === 'settled')
    .reduce((sum, item) => sum + item.amount, 0)
  const totalRefunded = rows
    .filter((item) => item.type === 'refund' && item.status === 'settled')
    .reduce((sum, item) => sum + item.amount, 0)
  return clone({
    patient,
    balance: patient.balance,
    totalCollected,
    totalRefunded,
    refundableBalance: Math.max(0, patient.balance)
  })
}
export async function mockPagePrepayRecords(
  encounterId: string,
  query: PrepayRecordQuery,
  context: RequestContext = {}
): Promise<PageResult<PrepayRecord>> {
  await delay(context)
  patientOf(encounterId)
  const rows = records.get(encounterId) ?? []
  const pageNo = query.PageIndex ?? query.pageNo ?? query.page ?? 1
  const pageSize = query.PageSize ?? query.pageSize ?? 20
  const start = (pageNo - 1) * pageSize
  const pageRows = clone(rows.slice(start, start + pageSize))
  return {
    rows: pageRows,
    records: clone(pageRows),
    total: rows.length,
    pageNo,
    pageSize
  }
}
export async function mockCollectInpatientPrepay(
  data: CollectPrepayPayload,
  context: RequestContext = {}
) {
  await delay(context, 260)
  const amount = Number(data.amount)
  if (amount <= 0) throw new Error('收款金额必须大于 0')
  const patient = patientOf(data.encounterId)
  if (patient.patientId !== data.patientId) throw new Error('患者与本次住院就诊不匹配')
  if ((records.get(data.encounterId) ?? []).some((item) => item.status === 'processing'))
    throw new Error('当前患者存在处理中的扫码交易，请先确认或取消')
  const channel = String(data.paymentDetails[0]?.payChannelCode ?? '') as PrepayRecord['channel']
  if (!['cash', 'card', 'wechat', 'alipay'].includes(channel)) throw new Error('支付渠道无效')
  const processing = Boolean(data.clientScanResult)
  const row: PrepayRecord = {
    id: `prepay-${sequence++}`,
    encounterId: data.encounterId,
    type: 'collect',
    amount,
    channel,
    status: processing ? 'processing' : 'settled',
    transactionNo: `PP20260725${String(sequence).padStart(4, '0')}`,
    createdAt: '2026-07-25 13:00',
    refundableAmount: processing ? 0 : amount
  }
  syncRecordFields(row)
  ;(records.get(data.encounterId) ?? []).unshift(row)
  if (processing)
    pendingPayloads.set(row.id, {
      type: 'collect',
      encounterId: data.encounterId,
      amount
    })
  else {
    patient.balance += amount
    syncPatientFields(patient)
  }
  row.accountBalance = patient.balance
  return { accountBalance: patient.balance, paymentId: row.id }
}
export async function mockRefundInpatientPrepay(
  data: RefundPrepayPayload,
  context: RequestContext = {}
) {
  await delay(context, 260)
  const patient = patientOf(data.encounterId)
  const source = (records.get(data.encounterId) ?? []).find(
    (item) => item.id === data.originalPaymentId && item.type === 'collect'
  )
  if (patient.patientId !== data.patientId) throw new Error('患者与本次住院就诊不匹配')
  if (!source || source.status !== 'settled') throw new Error('当前记录状态不允许退款')
  const channel = String(data.paymentDetails[0]?.payChannelCode ?? '') as PrepayRecord['channel']
  if (channel !== source.channel) throw new Error('预交金退款必须退回原支付渠道')
  if ((records.get(data.encounterId) ?? []).some((item) => item.status === 'processing'))
    throw new Error('当前患者存在处理中的扫码交易，请先确认或取消')
  const max = Math.min(source.refundableAmount, patient.balance)
  const amount = Number(data.refundAmount)
  if (amount <= 0 || amount > max)
    throw new Error(`退款金额不能超过可退余额 ¥${max.toFixed(2)}`)
  const processing = Boolean(data.clientRefundResult)
  const row: PrepayRecord = {
    id: `prepay-${sequence++}`,
    encounterId: data.encounterId,
    type: 'refund',
    amount,
    channel,
    status: processing ? 'processing' : 'settled',
    transactionNo: `PR20260725${String(sequence).padStart(4, '0')}`,
    createdAt: '2026-07-25 13:10',
    refundableAmount: 0
  }
  syncRecordFields(row)
  ;(records.get(data.encounterId) ?? []).unshift(row)
  if (processing) {
    pendingPayloads.set(row.id, {
      type: 'refund',
      encounterId: data.encounterId,
      amount,
      sourceRecordId: source.id
    })
    return { accountBalance: patient.balance, paymentId: row.id }
  }
  source.refundableAmount -= amount
  if (source.refundableAmount <= 0) source.status = 'refunded'
  patient.balance -= amount
  syncRecordFields(source)
  syncPatientFields(patient)
  row.accountBalance = patient.balance
  return { accountBalance: patient.balance, paymentId: row.id }
}
export async function mockQueryPrepayPaymentResult(
  paymentId: string,
  context: RequestContext = {}
) {
  await delay(context)
  for (const [encounterId, rows] of records) {
    const row = rows.find((item) => item.id === paymentId)
    if (!row) continue
    if (row.status === 'processing') {
      const pending = pendingPayloads.get(paymentId)
      if (pending?.type === 'refund') {
        const source = rows.find((item) => item.id === pending.sourceRecordId)
        if (!source || source.status !== 'settled')
          throw new Error('原收款记录状态已变化，退款失败')
        const patient = patientOf(encounterId)
        const max = Math.min(source.refundableAmount, patient.balance)
        if (pending.amount > max) throw new Error(`退款金额不能超过可退余额 ¥${max.toFixed(2)}`)
        source.refundableAmount -= pending.amount
        if (source.refundableAmount <= 0) source.status = 'refunded'
        patient.balance -= pending.amount
        syncRecordFields(source)
        syncPatientFields(patient)
      } else {
        row.refundableAmount = row.amount
        const patient = patientOf(encounterId)
        patient.balance += row.amount
        syncPatientFields(patient)
      }
      row.status = 'settled'
      syncRecordFields(row)
      pendingPayloads.delete(paymentId)
    }
    return clone(row)
  }
  throw new Error('支付记录不存在')
}
export async function mockRecoverPrepaySettlement(paymentId: string, context: RequestContext = {}) {
  return mockQueryPrepayPaymentResult(paymentId, context)
}
export async function mockCancelPrepayPayment(paymentId: string, context: RequestContext = {}) {
  await delay(context)
  for (const rows of records.values()) {
    const row = rows.find((item) => item.id === paymentId)
    if (!row) continue
    if (row.status !== 'processing') throw new Error('当前交易状态不允许取消')
    row.status = 'cancelled'
    syncRecordFields(row)
    pendingPayloads.delete(paymentId)
    return clone(row)
  }
  throw new Error('支付记录不存在')
}
