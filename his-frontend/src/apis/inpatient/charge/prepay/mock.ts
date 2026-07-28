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
export async function mockPageInpatientQueue(
  query: InpatientQueueQuery,
  context: RequestContext = {}
): Promise<PageResult<InpatientPatient>> {
  await delay(context)
  const key = query.keyword?.trim().toLowerCase()
  const rows = patients.filter(
    (item) =>
      !key ||
      `${item.name}${item.patientNo}${item.visitNo}${item.bedNo}`.toLowerCase().includes(key)
  )
  const start = (query.page - 1) * query.pageSize
  return { rows: clone(rows.slice(start, start + query.pageSize)), total: rows.length }
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
  const start = (query.page - 1) * query.pageSize
  return { rows: clone(rows.slice(start, start + query.pageSize)), total: rows.length }
}
export async function mockCollectInpatientPrepay(
  data: CollectPrepayPayload,
  context: RequestContext = {}
) {
  await delay(context, 260)
  if (data.amount <= 0) throw new Error('收款金额必须大于 0')
  const patient = patientOf(data.encounterId)
  if ((records.get(data.encounterId) ?? []).some((item) => item.status === 'processing'))
    throw new Error('当前患者存在处理中的扫码交易，请先确认或取消')
  const processing = ['wechat', 'alipay'].includes(data.channel)
  const row: PrepayRecord = {
    id: `prepay-${sequence++}`,
    encounterId: data.encounterId,
    type: 'collect',
    amount: data.amount,
    channel: data.channel,
    status: processing ? 'processing' : 'settled',
    transactionNo: `PP20260725${String(sequence).padStart(4, '0')}`,
    createdAt: '2026-07-25 13:00',
    refundableAmount: processing ? 0 : data.amount
  }
  ;(records.get(data.encounterId) ?? []).unshift(row)
  if (processing)
    pendingPayloads.set(row.id, {
      type: 'collect',
      encounterId: data.encounterId,
      amount: data.amount
    })
  else patient.balance += data.amount
  return clone(row)
}
export async function mockRefundInpatientPrepay(
  data: RefundPrepayPayload,
  context: RequestContext = {}
) {
  await delay(context, 260)
  const patient = patientOf(data.encounterId)
  const source = (records.get(data.encounterId) ?? []).find(
    (item) => item.id === data.recordId && item.type === 'collect'
  )
  if (!source || source.status !== 'settled') throw new Error('当前记录状态不允许退款')
  if (data.channel !== source.channel) throw new Error('预交金退款必须退回原支付渠道')
  if ((records.get(data.encounterId) ?? []).some((item) => item.status === 'processing'))
    throw new Error('当前患者存在处理中的扫码交易，请先确认或取消')
  const max = Math.min(source.refundableAmount, patient.balance)
  if (data.amount <= 0 || data.amount > max)
    throw new Error(`退款金额不能超过可退余额 ¥${max.toFixed(2)}`)
  const processing = ['wechat', 'alipay'].includes(data.channel)
  const row: PrepayRecord = {
    id: `prepay-${sequence++}`,
    encounterId: data.encounterId,
    type: 'refund',
    amount: data.amount,
    channel: data.channel,
    status: processing ? 'processing' : 'settled',
    transactionNo: `PR20260725${String(sequence).padStart(4, '0')}`,
    createdAt: '2026-07-25 13:10',
    refundableAmount: 0
  }
  ;(records.get(data.encounterId) ?? []).unshift(row)
  if (processing) {
    pendingPayloads.set(row.id, {
      type: 'refund',
      encounterId: data.encounterId,
      amount: data.amount,
      sourceRecordId: source.id
    })
    return clone(row)
  }
  source.refundableAmount -= data.amount
  if (source.refundableAmount <= 0) source.status = 'refunded'
  patient.balance -= data.amount
  return clone(row)
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
      } else {
        row.refundableAmount = row.amount
        patientOf(encounterId).balance += row.amount
      }
      row.status = 'settled'
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
    pendingPayloads.delete(paymentId)
    return clone(row)
  }
  throw new Error('支付记录不存在')
}
