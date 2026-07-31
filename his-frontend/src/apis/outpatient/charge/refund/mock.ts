import type {
  PageResult,
  RefundDetail,
  RefundItem,
  RefundPatient,
  RefundPayload,
  RefundQueueQuery,
  RefundRecord,
  SwitchPreservedChargeItemsRequest,
  VerifyRefundableBeforeExecutionRequest,
  RequestContext
} from './type'
const patients: RefundPatient[] = [
  {
    id: 'refund-patient-1',
    encounterId: 'enc-2001',
    patientNo: 'P000201',
    visitNo: 'V20260725011',
    name: '赵琴',
    gender: '女',
    age: 29,
    departmentName: '妇科',
    visitTime: '2026-07-25 08:20',
    status: 'pending',
    paymentType: '医保'
  },
  {
    id: 'refund-patient-2',
    encounterId: 'enc-2002',
    patientNo: 'P000202',
    visitNo: 'V20260725012',
    name: '周凯',
    gender: '男',
    age: 50,
    departmentName: '骨科',
    visitTime: '2026-07-25 09:18',
    status: 'refunded',
    paymentType: '自费'
  }
]
const items = new Map<string, RefundItem[]>([
  [
    'enc-2001',
    [
      {
        id: 'refund-item-1',
        paymentRecordId: 'pay-9001',
        paymentNo: 'PAY202607250001',
        itemName: '腰椎 CT',
        category: '检查',
        quantity: 1,
        amount: 260,
        insuranceMode: 'insurance',
        insuranceAmount: 208,
        selfPayAmount: 52,
        refundableAmount: 260,
        status: 'refundable',
        paidAt: '2026-07-25 08:30',
        channel: 'card'
      },
      {
        id: 'refund-item-2',
        paymentRecordId: 'pay-9001',
        paymentNo: 'PAY202607250001',
        itemName: '一次性增强管',
        category: '耗材',
        quantity: 1,
        amount: 35,
        insuranceMode: 'insurance',
        insuranceAmount: 28,
        selfPayAmount: 7,
        refundableAmount: 35,
        status: 'refundable',
        paidAt: '2026-07-25 08:30',
        channel: 'card'
      },
      {
        id: 'refund-item-3',
        paymentRecordId: 'pay-9002',
        paymentNo: 'PAY202607250002',
        itemName: '注射费',
        category: '治疗',
        quantity: 1,
        amount: 18,
        insuranceMode: 'insurance',
        insuranceAmount: 14.4,
        selfPayAmount: 3.6,
        refundableAmount: 18,
        status: 'refundable',
        paidAt: '2026-07-25 08:40',
        channel: 'wechat'
      }
    ]
  ],
  [
    'enc-2002',
    [
      {
        id: 'refund-item-4',
        paymentRecordId: 'pay-9003',
        paymentNo: 'PAY202607250003',
        itemName: '清创换药',
        category: '治疗',
        quantity: 1,
        amount: 80,
        insuranceMode: 'self-pay',
        insuranceAmount: 0,
        selfPayAmount: 80,
        refundableAmount: 0,
        status: 'refunded',
        paidAt: '2026-07-24 15:30',
        channel: 'cash'
      }
    ]
  ]
])
const refundRecords: RefundRecord[] = [
  {
    id: 'refund-failed-1',
    encounterId: 'enc-2002',
    paymentRecordId: 'pay-9003',
    amount: 120,
    status: 'failed',
    failureReason: '原支付渠道拒绝退款，请核对原交易状态后重试',
    createdAt: '2026-07-25 11:30'
  }
]
const pendingPayloads = new Map<string, RefundPayload>()
const verifiedSelections = new Map<string, string[]>()
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
  if (!row) throw new Error('患者就诊信息不存在')
  return row
}

function syncPatientFields(row: RefundPatient) {
  const rows = items.get(row.encounterId) ?? []
  row.patientId = row.id
  row.encounterBusNo = row.visitNo
  row.patientName = row.name
  row.genderCode = row.gender === '男' ? 'M' : 'F'
  row.genderEnumText = row.gender
  row.ageText = `${row.age}岁`
  row.organizationName = row.departmentName
  row.doctorName = row.departmentName === '妇科' ? '李文静' : '赵海峰'
  row.startTime = row.visitTime
  row.queueLabelCode = row.status === 'pending' ? 'PENDING' : 'SETTLED'
  row.chargeItemStatusSummary = rows.some((item) => item.status === 'refundable')
    ? '存在可退项目'
    : '无可退项目'
  row.totalAmount = rows.reduce((sum, item) => sum + item.amount, 0)
  row.paidAmount = row.totalAmount
  return row
}

function syncItemFields(row: RefundItem) {
  row.chargeItemId = row.id
  row.paymentId = row.paymentRecordId
  row.paymentBusNo = row.paymentNo
  row.itemDisplayName = row.itemName
  row.lineTotalAmount = row.amount
  row.chargeItemStatusEnum = row.status.toUpperCase()
  return row
}

function syncRecordFields(row: RefundRecord) {
  row.reversePaymentId = row.id
  return row
}

items.forEach((rows) => rows.forEach(syncItemFields))
refundRecords.forEach(syncRecordFields)
patients.forEach(syncPatientFields)

export async function mockPageRefundPatientQueue(
  query: RefundQueueQuery,
  context: RequestContext = {}
): Promise<PageResult<RefundPatient>> {
  await delay(context)
  const key = query.keyword?.trim().toLowerCase()
  const status =
    query.queueTabCode === 'PENDING'
      ? 'pending'
      : query.queueTabCode === 'SETTLED'
        ? 'refunded'
        : query.queueTabCode === 'ALL'
          ? 'all'
          : query.status
  const rows = patients.filter(
    (item) =>
      (status === 'all' || item.status === status) &&
      (!query.startDate || item.visitTime.slice(0, 10) >= query.startDate) &&
      (!query.endDate || item.visitTime.slice(0, 10) <= query.endDate) &&
      (!key || `${item.name}${item.patientNo}${item.visitNo}`.toLowerCase().includes(key))
  )
  const start = ((query.pageNo ?? query.page) - 1) * query.pageSize
  const pageRows = clone(rows.slice(start, start + query.pageSize))
  const pageNo = query.pageNo ?? query.page
  return {
    rows: pageRows,
    records: clone(pageRows),
    content: clone(pageRows),
    total: rows.length,
    totalCount: rows.length,
    pageNo,
    pageSize: query.pageSize
  }
}
export async function mockGetRefundableExecutionDetail(
  encounterId: string,
  context: RequestContext = {}
): Promise<RefundDetail> {
  await delay(context)
  const interrupted = refundRecords.find(
    (record) => record.encounterId === encounterId && record.status === 'pending'
  )
  const failed = [...refundRecords]
    .reverse()
    .find((record) => record.encounterId === encounterId && record.status === 'failed')
  const detailRows = items.get(encounterId) ?? []
  const grouped = new Map<string, RefundItem[]>()
  detailRows.forEach((item) =>
    grouped.set(item.paymentRecordId, [...(grouped.get(item.paymentRecordId) ?? []), item])
  )
  return clone({
    patient: patientOf(encounterId),
    items: detailRows,
    paymentGroups: [...grouped.entries()].map(([paymentId, groupRows]) => ({
      halfRefundOnPayment:
        groupRows.some((item) => item.status === 'refunded') &&
        groupRows.some((item) => item.status === 'refundable'),
      originalPayChannelCodes: [...new Set(groupRows.map((item) => item.channel))],
      paymentBusNo: groupRows[0]?.paymentNo ?? paymentId,
      paymentId,
      paymentStatusEnum: 'SUCCESS',
      preservedLines: groupRows
        .filter((item) => item.status !== 'refundable')
        .map((item) => ({
          chargeItemId: item.id,
          chargeItemStatusEnum: item.chargeItemStatusEnum ?? item.status,
          itemDisplayName: item.itemName,
          lineTotalAmount: item.amount,
          paymentId
        })),
      refundableAmount: groupRows.reduce((sum, item) => sum + item.refundableAmount, 0),
      refundingLines: groupRows
        .filter((item) => item.status === 'refundable')
        .map((item) => ({
          chargeItemId: item.id,
          chargeItemStatusEnum: item.chargeItemStatusEnum ?? item.status,
          itemDisplayName: item.itemName,
          lineTotalAmount: item.amount,
          paymentId
        }))
    })),
    interruptedRefundId: interrupted?.id,
    refundFailureReason: failed?.failureReason
  })
}
export async function mockVerifyOutpatientRefund(
  data: VerifyRefundableBeforeExecutionRequest,
  context: RequestContext = {}
): Promise<{ allowed: boolean; rejectReasonCode?: string | null }> {
  await delay(context)
  if (!data.refundingChargeItemIds.length) throw new Error('请至少选择一条可退项目')
  const rows = items.get(data.encounterId) ?? []
  const selected = rows.filter((item) => data.refundingChargeItemIds.includes(item.id))
  if (
    !selected.length ||
    selected.some((item) => item.status !== 'refundable' || item.refundableAmount <= 0)
  )
    throw new Error('存在不可退项目，请刷新后重新选择')
  const groups = new Set(selected.map((item) => item.paymentRecordId))
  if (groups.size !== 1) throw new Error('一次退费只能选择同一笔原支付记录')
  const paymentRecordId = selected[0]!.paymentRecordId
  if (
    [...pendingPayloads.values()].some(
      (payload) =>
        payload.encounterId === data.encounterId && payload.originalPaymentId === paymentRecordId
    )
  )
    throw new Error('该原支付记录存在处理中的退款，请先查询或恢复退款结果')
  verifiedSelections.set(data.encounterId, [...data.refundingChargeItemIds])
  return { allowed: true }
}
export async function mockRefundOutpatientCharge(
  data: RefundPayload,
  context: RequestContext = {}
) {
  await delay(context, 300)
  const rows = items.get(data.encounterId) ?? []
  const selected = rows.filter((item) => data.refundingChargeItemIds.includes(item.id))
  if (
    selected.length !== data.refundingChargeItemIds.length ||
    selected.some((item) => item.status !== 'refundable')
  )
    throw new Error('退款项目状态已变化，请重新校验')
  if (
    new Set(selected.map((item) => item.paymentRecordId)).size !== 1 ||
    selected[0]?.paymentRecordId !== data.originalPaymentId
  )
    throw new Error('退款项目必须属于同一原支付记录')
  const channel = String(data.refundPaymentDetails[0]?.payChannelCode ?? '') as RefundItem['channel']
  if (selected.some((item) => item.channel !== channel))
    throw new Error('退款必须退回原支付渠道')
  const status = ['wechat', 'alipay'].includes(channel) ? 'pending' : 'success'
  const record: RefundRecord = {
    id: `refund-${sequence++}`,
    encounterId: data.encounterId,
    paymentRecordId: data.originalPaymentId,
    amount: selected.reduce((sum, item) => sum + item.refundableAmount, 0),
    halfRefundExecuted:
      selected.length < rows.filter((item) => item.paymentRecordId === data.originalPaymentId).length,
    insuranceRevoked: selected.some((item) => item.insuranceMode === 'insurance'),
    originalPaymentId: data.originalPaymentId,
    success: status === 'success',
    status,
    createdAt: '2026-07-25 12:30'
  }
  syncRecordFields(record)
  refundRecords.push(record)
  if (status === 'pending') pendingPayloads.set(record.id, clone(data))
  else completeRefund(data)
  return {
    halfRefundExecuted: record.halfRefundExecuted,
    insuranceRevoked: record.insuranceRevoked,
    originalPaymentId: record.originalPaymentId,
    reversePaymentId: record.id,
    success: record.success ?? false
  }
}
export async function mockSwitchRetainedItemsInsuranceMode(
  data: SwitchPreservedChargeItemsRequest,
  context: RequestContext = {}
) {
  await delay(context)
  const rows = items.get(data.encounterId) ?? []
  const selectedIds = verifiedSelections.get(data.encounterId) ?? []
  const selectedPaymentId = rows.find((item) => selectedIds.includes(item.id))?.paymentRecordId
  const retained = rows.filter(
    (item) =>
      item.paymentRecordId === selectedPaymentId &&
      item.status === 'refundable' &&
      !selectedIds.includes(item.id)
  )
  retained.forEach((item) => {
    item.insuranceMode = 'self-pay'
    item.insuranceAmount = 0
    item.selfPayAmount = Number((item.amount - item.insuranceAmount).toFixed(2))
    syncItemFields(item)
  })
  return { success: true }
}
function completeRefund(data: RefundPayload) {
  const rows = items.get(data.encounterId) ?? []
  rows
    .filter((item) => data.refundingChargeItemIds.includes(item.id))
    .forEach((item) => {
      item.status = 'refunded'
      item.refundableAmount = 0
      syncItemFields(item)
    })
  if (rows.every((item) => item.status !== 'refundable' && item.status !== 'partial'))
    patientOf(data.encounterId).status = 'refunded'
  syncPatientFields(patientOf(data.encounterId))
}
export async function mockQueryRefundResult(refundId: string, context: RequestContext = {}) {
  await delay(context)
  const record = refundRecords.find((item) => item.id === refundId)
  if (!record) throw new Error('退款记录不存在')
  const payload = pendingPayloads.get(refundId)
  if (record.status === 'failed') throw new Error(record.failureReason ?? '退款失败')
  if (payload) {
    completeRefund(payload)
    pendingPayloads.delete(refundId)
    record.status = 'success'
    syncRecordFields(record)
  }
  return clone(record)
}
export async function mockRecoverRefund(refundId: string, context: RequestContext = {}) {
  return mockQueryRefundResult(refundId, context)
}
