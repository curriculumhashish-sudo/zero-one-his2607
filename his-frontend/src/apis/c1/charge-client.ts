import HttpClient, { DataUpType } from '@/apis/http'

type Loose = Record<string, any>
type Envelope<T = any> = { code?: number; message?: string; data?: T }
type Context = { signal?: AbortSignal }

const client = new HttpClient(
  { baseURL: import.meta.env.VITE_C1_CHARGE_SERVER_URL || '/c1-charge' },
  {
    requestInterceptor: (config) => {
      if (typeof localStorage !== 'undefined') {
        const token = localStorage.getItem('token')
        const head = (localStorage.getItem('tokenHead') || 'Bearer').trim()
        if (token) config.headers.Authorization = head ? `${head} ${token}` : token
      }
      if (config.upType === DataUpType.json) config.headers['Content-Type'] = 'application/json;charset=UTF-8'
      return config
    }
  }
)

function cfg(context?: Context) {
  return context?.signal ? { signal: context.signal } : undefined
}

async function unwrap<T = any>(request: Promise<Envelope<T>>): Promise<T> {
  const response = await request
  if (response.code !== undefined && response.code !== 10000 && response.code !== 200) {
    if (response.code === 401) {
      throw new Error('C1 服务未接收到有效登录令牌，请先登录后再访问该模块')
    }
    throw new Error(response.message || 'C1收费工作站接口请求失败')
  }
  return (response.data === undefined ? (response as any) : response.data) as T
}

function rowsOf(value: any): any[] {
  if (Array.isArray(value)) return value
  if (!value || typeof value !== 'object') return []
  for (const key of ['records', 'rows', 'content', 'list', 'items', 'data']) {
    if (Array.isArray(value[key])) return value[key]
  }
  return []
}

function pageOf(value: any, fallback: any = {}): any {
  const rows = rowsOf(value)
  const pageIndex = Number(value?.pageIndex ?? value?.current ?? fallback.pageIndex ?? 1)
  const pageSize = Number(value?.pageSize ?? value?.size ?? fallback.pageSize ?? rows.length)
  const total = Number(value?.total ?? value?.totalCount ?? value?.totalElements ?? value?.totalRecords ?? rows.length)
  return { rows, records: rows, content: rows, total, totalCount: total, totalElements: total, pageIndex, pageSize }
}

function omitEmpty(value: Loose): Loose {
  return Object.fromEntries(
    Object.entries(value).filter(([, item]) => item !== undefined && item !== null && item !== '')
  )
}

const asString = (v: any) => (v === undefined || v === null ? '' : String(v))
const asNumber = (v: any, fallback = 0) => {
  const n = Number(v)
  return Number.isFinite(n) ? n : fallback
}
const dateRange = (q: any) => ({
  ...(q.startDate || q.dateFrom ? { startDate: q.startDate ?? q.dateFrom } : {}),
  ...(q.endDate || q.dateTo ? { endDate: q.endDate ?? q.dateTo } : {})
})

function mapPatient(row: Loose, kind: 'charge' | 'refund' | 'inpatient' = 'charge'): Loose {
  // C1 部分住院队列只返回 hospitalized_number/inpatient_no。保留真实主键优先，
  // 没有主键时至少使用住院号作为查询标识，避免患者卡片点击后 id 为空、页面看起来无响应。
  const encounterBusNo = asString(
    row.encounterBusNo ?? row.visitNumber ?? row.hospitalizedNumber ?? row.hospitalized_number ?? row.inpatient_no
  )
  const encounterId = asString(row.encounterId ?? row.encounter_id ?? row.id ?? encounterBusNo)
  const patientId = asString(row.patientId ?? row.patient_id)
  const name = asString(row.patientName ?? row.patient_name ?? row.name)
  const department = asString(row.organizationName ?? row.organization ?? row.dept_name ?? row.depart)
  const statusEnum = asString(row.statusEnum ?? row.status ?? (kind === 'refund' ? 'PENDING' : 'PENDING'))
  return {
    ...row,
    id: encounterId,
    encounterId,
    patientId,
    patientNo: asString(row.patientNo ?? row.hospitalizedNumber ?? row.hospitalized_number ?? patientId),
    encounterBusNo,
    visitNo: asString(row.visitNo ?? row.encounterBusNo ?? row.visitNumber ?? row.inpatient_no ?? encounterId),
    patientName: name,
    name,
    genderCode: asString(row.genderCode ?? row.sex),
    genderEnumText: asString(row.genderEnumText ?? row.sexText ?? row.genderText ?? row.gender ?? row.sex),
    gender: asString(row.gender ?? row.sexText ?? row.genderText ?? row.sex),
    age: asNumber(row.age),
    ageText: asString(row.ageText ?? row.age),
    organizationName: department,
    departmentName: department,
    doctorName: asString(row.doctorName ?? row.doctor),
    startTime: asString(row.startTime ?? row.hospitalizedTime ?? row.hospitalized_time ?? row.admissionDate ?? row.admit_date),
    visitTime: asString(row.visitTime ?? row.startTime ?? row.hospitalizedTime ?? row.hospitalized_time ?? row.admissionDate ?? row.admit_date),
    totalAmount: asNumber(row.totalAmount ?? row.amount ?? row.cost),
    paidAmount: asNumber(row.paidAmount ?? row.paid ?? row.settledAmount),
    paymentId: asString(row.paymentId),
    statusEnum,
    status: /REFUND|RETURN|CANCEL/i.test(statusEnum) ? 'refunded' : (/SETTLE|COMPLET|PAID/i.test(statusEnum) ? 'settled' : 'pending'),
    paymentType: asString(row.paymentType ?? row.paymentMethod ?? row.payWay ?? row.method),
    wardName: asString(row.wardName ?? row.ward ?? row.ward_name ?? row.admission_ward),
    bedName: asString(row.bedName ?? row.bed_number ?? row.bed_no)
  }
}

function mapChargeItem(row: Loose): Loose {
  const amount = asNumber(row.amount ?? row.lineAmount ?? row.totalPrice ?? row.cost)
  const quantity = asNumber(row.quantityValue ?? row.quantity ?? 1, 1)
  return {
    ...row,
    id: asString(row.chargeItemId ?? row.id ?? row.itemId),
    chargeItemId: asString(row.chargeItemId ?? row.id ?? row.itemId),
    itemName: asString(row.itemName ?? row.name),
    category: asString(row.category ?? row.chargeCategoryDisplay ?? row.categoryDisplay),
    specification: asString(row.specification ?? row.specText),
    quantity,
    quantityValue: quantity,
    quantityUnit: asString(row.quantityUnit ?? row.unit),
    unitPrice: asNumber(row.unitPrice ?? (quantity ? amount / quantity : amount)),
    amount,
    totalPrice: asNumber(row.totalPrice ?? amount),
    insuranceAmount: asNumber(row.insuranceAmount ?? row.fundPayAmount),
    selfPayAmount: asNumber(row.selfPayAmount ?? row.personalPayAmount ?? amount),
    statusCode: asString(row.statusCode ?? row.statusEnum),
    statusText: asString(row.statusText ?? row.statusEnumText),
    status: /SETTLE|PAID|COMPLET|REFUND/i.test(asString(row.statusCode ?? row.statusEnum)) ? 'settled' : 'pending'
  }
}

// 门诊挂号
const c1RegisterTypeById = new Map<string, string>()

export async function c1LoadOutpatientRegisterInit(query: any = {}, context: Context = {}) {
  let rows: any[] = []
  try {
    rows = rowsOf(await unwrap(client.get('/charging/register/getseeing', {
      pageIndex: 1,
      pageSize: 1000
    }, cfg(context))))
  } catch {
    // 初始化选项失败不阻断页面，列表查询会再次报告真实错误。
  }
  const options = (key: string, labelKey = key, fallbackKey?: string) => Array.from(
    new Map<string, { value: string; label: string }>(
      rows
        .map((r): [string, { value: string; label: string }] => {
          const value = asString(r[key]) || (fallbackKey ? asString(r[fallbackKey]) : '')
          const label = asString(r[labelKey]) || asString(r[key]) || value
          return [value, { value, label }]
        })
        .filter(([value]) => value)
    ).values()
  )
  for (const row of rows) {
    const id = asString(row.serviceTypeId) || asString(row.register_type)
    const label = asString(row.register_type) || asString(row.serviceTypeName) || id
    if (id && label) c1RegisterTypeById.set(id, label)
  }
  return {
    departmentOptions: options('organization_id', 'organization', 'organization'),
    serviceTypeOptions: options('serviceTypeId', 'register_type', 'register_type'),
    contractOptions: options('contractNo', 'cost_type', 'cost_type').concat(
      rows.some((row) => row.cost_type === '自费') ? [] : [{ value: 'CONTRACT-SELF', label: '自费' }]
    ),
    paymentChannelOptions: [{ value: 'CASH', label: '现金' }, { value: 'BANK_CARD', label: '银行卡' }, { value: 'WECHAT', label: '微信' }, { value: 'ALIPAY', label: '支付宝' }],
    genderOptions: [{ value: 'M', label: '男' }, { value: 'F', label: '女' }],
    practitionerOptions: options('doctor_id', 'doctor', 'doctor')
  }
}
export async function c1PageRegisterPatientOptions(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/register/getseeing', {
    pageIndex: query.pageIndex ?? 1,
    pageSize: query.pageSize ?? 20,
    ...(query.keyword ? { name: query.keyword } : {})
  }, cfg(context)))
  const page = pageOf(data, query)
  const rows = page.rows.map((row: Loose) => ({
    patientId: asString(row.patient_id ?? row.patientId ?? row.id),
    patientBusNo: asString(row.patient_no ?? row.patientBusNo ?? row.number),
    name: asString(row.name),
    genderCode: asString(row.sex),
    genderText: asString(row.sex),
    age: row.age,
    birthDate: asString(row.birth_date),
    phoneMasked: asString(row.phone),
    idCardNoMasked: asString(row.id_number)
  }))
  return { ...page, rows, records: rows, content: rows }
}
export async function c1ListRegisterPractitioners(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/register/getseeing', { pageIndex: 1, pageSize: 1000 }, cfg(context)))
  const rows = rowsOf(data).filter((row: Loose) =>
    (!query.organizationId || asString(row.organization) === asString(query.organizationId))
    && (!query.keyword || asString(row.doctor).includes(asString(query.keyword)))
  )
  const options = new Map<string, Loose>()
  for (const row of rows) {
    const value = asString(row.doctorId) || asString(row.doctor_id) || asString(row.doctor)
    const label = asString(row.doctorName) || asString(row.doctor) || value
    if (value) options.set(value, { value, label, practitionerId: value, practitionerName: label })
  }
  return { practitionerOptions: [...options.values()] }
}
export async function c1ListBookableSlotHints(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/register/get_today_appoint', omitEmpty({
    pageIndex: 1,
    pageSize: 1000,
    doctor: query.practitionerId,
    register_type: c1RegisterTypeById.get(asString(query.healthcareServiceId)) ?? query.healthcareServiceId
  }), cfg(context)))
  return {
    hints: rowsOf(data).map((r) => ({
      slotId: r.slotId ?? r.id,
      slotStart: r.slotStart ?? r.startTime ?? r.start_time,
      slotEnd: r.slotEnd ?? r.endTime ?? r.end_time,
      remainingCount:
        r.remainingCount ??
        r.remaining ??
        Math.max(0, asNumber(r.capacity) - asNumber(r.booked_count))
    }))
  }
}
export async function c1LoadRegisterBenefitPreview(_payload: any, _context: Context = {}) { return { benefitKind: 'NONE' } }
export async function c1PreSettleOutpatientRegister(payload: any, context: Context = {}) {
  const body = { ...payload, contractNo: payload.contractId ?? payload.contractNo, doctor: payload.practitionerId ?? payload.doctor, doctor_id: payload.practitionerId ?? payload.doctor_id, organization: payload.organizationId ?? payload.organization, organization_id: payload.organizationId ?? payload.organization_id, patient_id: payload.patientId ?? payload.patient_id, birth_date: payload.birthDate ?? payload.birth_date, id_number: payload.idCardNo ?? payload.id_number, serviceTypeId: payload.serviceTypeId, slotId: payload.slotId }
  return unwrap(client.post('/charging/register/add_register', body, cfg(context)))
}
export async function c1ConfirmOutpatientRegister(payload: any, context: Context = {}) {
  const details = Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []
  const body = {
    ...payload,
    paymentDraft: payload.paymentDraft ?? payload.paymentDraftId,
    paymentMethod: payload.paymentMethod ?? details[0]?.payChannelCode,
    amount: payload.amount ?? details.reduce((sum: number, item: any) => sum + asNumber(item.amount ?? item.payAmount), 0),
    transactionId: payload.transactionId ?? details[0]?.transactionNo,
    paymentDetails: undefined
  }
  return unwrap(client.post('/charging/register/chargeconfirmcontroller', body, cfg(context)))
}
export async function c1PageOutpatientRegisterHistory(query: any, context: Context = {}) {
  // C1 当前 getseeing 对 startDate/endDate 会返回 unordered_map::at；先取可分页的全量结果，再在适配层按日期筛选。
  const data = await unwrap(client.get('/charging/register/getseeing', omitEmpty({ keyword: query.keyword, pageIndex: 1, pageSize: 1000 }), cfg(context)))
  let rows = rowsOf(data)
  const from = asString(query.dateFrom ?? query.startDate)
  const to = asString(query.dateTo ?? query.endDate)
  if (from || to) {
    rows = rows.filter((row: Loose) => {
      const date = asString(row.startTime ?? row.time).slice(0, 10)
      return (!from || date >= from) && (!to || date <= to)
    })
  }
  const pageIndex = Number(query.pageIndex ?? 1)
  const pageSize = Number(query.pageSize ?? 20)
  const pageRows = rows.slice(Math.max(0, pageIndex - 1) * pageSize, Math.max(0, pageIndex - 1) * pageSize + pageSize)
  const records = pageRows.map((r: Loose) => ({ ...mapPatient(r), amount: asNumber(r.cost ?? r.amount), contactPhone: asString(r.phone), doctorName: asString(r.doctorName ?? r.doctor), organizationName: asString(r.organizationName ?? r.organization), serviceTypeId: asString(r.serviceTypeId), serviceTypeName: asString(r.registerType ?? r.serviceTypeName ?? r.register_type), startTime: asString(r.startTime ?? r.time), statusEnum: asString(r.statusEnum ?? r.status), statusText: asString(r.statusText ?? r.statusEnumText) }))
  return { rows: records, records, content: records, total: rows.length, totalCount: rows.length, totalElements: rows.length, pageIndex, pageSize }
}
export async function c1GetTodayRegisterSummary(query: any, context: Context = {}) { const data = await c1PageOutpatientRegisterHistory({ dateFrom: query.date, dateTo: query.date, pageIndex: 1, pageSize: 1000 }, context); return { date: query.date, totalCount: data.total, activeCount: data.records.filter((r: any) => !/CANCEL/i.test(r.statusEnum)).length, cancelledCount: data.records.filter((r: any) => /CANCEL/i.test(r.statusEnum)).length } }
export async function c1GetRegisterAppointment(query: any, context: Context = {}) { const data = await unwrap(client.get('/charging/register/get_today_appoint', query, cfg(context))); return rowsOf(data)[0] ?? data }
export async function c1ValidateRegisterCancellation(payload: any, context: Context = {}) { const data = await unwrap(client.get('/charging/register/getseeing', payload, cfg(context))); return { cancelable: Boolean(rowsOf(data).length), reasonCode: undefined, reasonText: undefined } }
export async function c1CancelOutpatientRegister(payload: any, context: Context = {}) { return unwrap(client.post('/charging/register/quitnumber', payload, cfg(context))) }
export async function c1CancelRegisterWithRefund(payload: any, context: Context = {}) { return unwrap(client.post('/charging/register/quitnumber', payload, cfg(context))) }
export async function c1GetRegisterTicket(query: any, context: Context = {}) { return unwrap(client.post('/charging/register/printregisterreceipt', query, cfg(context))) }

// 门诊收费
export async function c1PageChargePatientQueue(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/charge/get-patient-queue', { ...query, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 }, cfg(context)))
  return { ...pageOf(data, query), rows: pageOf(data, query).rows.map((r: Loose) => mapPatient(r, 'charge')) }
}
export async function c1GetPendingChargeDetail(encounterId: string, context: Context = {}) {
  const data = await unwrap<Loose>(client.get('/charging/charge/get-charge-items', { encounterId }, cfg(context)))
  const items = rowsOf(data).map(mapChargeItem)
  const patient = mapPatient(data.patient ?? {}, 'charge')
  const pendingItems = items.filter((item: Loose) => item.status === 'pending')
  return {
    encounterId,
    patient,
    items,
    insuranceMode: 'self-pay',
    summary: {
      totalAmount: pendingItems.reduce((s: number, x: Loose) => s + asNumber(x.amount), 0),
      insuranceAmount: pendingItems.reduce((s: number, x: Loose) => s + asNumber(x.insuranceAmount), 0),
      selfPayAmount: pendingItems.reduce((s: number, x: Loose) => s + asNumber(x.selfPayAmount), 0)
    }
  }
}
export async function c1PreSettleOutpatientCharge(payload: any, context: Context = {}) { return unwrap(client.post('/charging/charge/pre-settle-charge', payload, cfg(context))) }
export async function c1SettleOutpatientCharge(payload: any, context: Context = {}) { return unwrap(client.post('/charging/charge/confirm-settle-charge', payload, cfg(context))) }
export async function c1SwitchChargeInsuranceMode(data: any, context: Context = {}) { return c1GetPendingChargeDetail(data.encounterId, context) }
export async function c1QueryPaymentResult(paymentId: string, context: Context = {}) { return unwrap(client.get('/charging/charge/confirm-settle-charge', { paymentId }, cfg(context))) }
export async function c1RecoverSettlement(paymentId: string, context: Context = {}) { return c1QueryPaymentResult(paymentId, context) }
export async function c1GetChargeReceipt(params: any, context: Context = {}) { return unwrap(client.post('/charging/charge/print-charge-receipt', params, cfg(context))) }

// 门诊退费
export async function c1PageRefundPatientQueue(query: any, context: Context = {}) { const data = await unwrap(client.get('/charging/refund/get-patient-list', { ...query, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 }, cfg(context))); const page = pageOf(data, query); return { ...page, rows: page.rows.map((r: Loose) => mapPatient(r, 'refund')), records: page.rows.map((r: Loose) => mapPatient(r, 'refund')), content: page.rows.map((r: Loose) => mapPatient(r, 'refund')) } }
function mapRefundPaymentLine(line: Loose, group: Loose, refundable: boolean): Loose {
  const amount = asNumber(line.lineTotalAmount ?? line.amount ?? line.totalPrice)
  const itemName = asString(line.itemName ?? line.itemDisplayName ?? line.name)
  const chargeItemId = asString(line.chargeItemId ?? line.id)
  const paymentRecordId = asString(group.paymentRecordId ?? group.paymentId)
  const paymentNo = asString(group.paymentNo ?? group.paymentBusNo)
  return {
    ...line,
    id: chargeItemId,
    chargeItemId,
    paymentRecordId,
    paymentNo,
    paymentBusNo: paymentNo,
    itemName,
    itemDisplayName: itemName,
    category: asString(line.category ?? line.chargeItemStatusEnumText),
    quantity: asNumber(line.quantity ?? 1, 1),
    lineTotalAmount: amount,
    amount,
    insuranceMode: 'self-pay',
    insuranceAmount: 0,
    selfPayAmount: amount,
    refundableAmount: refundable ? amount : 0,
    chargeItemStatusEnum: asString(line.chargeItemStatusEnum ?? line.statusEnum),
    status: refundable ? 'refundable' : 'unavailable',
    paidAt: asString(group.billDate ?? group.payTime ?? group.settleTime),
    channel: String(group.originalPayChannelCodes?.[0] ?? group.payChannelCode ?? 'cash').toLowerCase()
  }
}
function mapRefundPaymentGroup(group: Loose): Loose {
  const refundingLines = Array.isArray(group.refundingLines) ? group.refundingLines : []
  const preservedLines = Array.isArray(group.preservedLines) ? group.preservedLines : []
  return {
    ...group,
    paymentId: asString(group.paymentRecordId ?? group.paymentId),
    paymentBusNo: asString(group.paymentNo ?? group.paymentBusNo),
    paymentStatusEnum: asString(group.paymentStatusEnum),
    halfRefundOnPayment: Boolean(group.halfRefundOnPayment),
    originalPayChannelCodes: (Array.isArray(group.originalPayChannelCodes) ? group.originalPayChannelCodes : []).map((v: any) => String(v).toLowerCase()),
    refundableAmount: asNumber(group.refundableAmount),
    refundingLines: refundingLines.map((line: Loose) => mapRefundPaymentLine(line, group, true)),
    preservedLines: preservedLines.map((line: Loose) => mapRefundPaymentLine(line, group, false))
  }
}
export async function c1GetRefundableExecutionDetail(encounterId: string, context: Context = {}) {
  const data = await unwrap<Loose>(client.get('/charging/refund/get-item-list', { encounterId }, cfg(context)))
  const patient = mapPatient(data.patient ?? {}, 'refund')
  const paymentGroups = Array.isArray(data.paymentGroups) ? data.paymentGroups.map(mapRefundPaymentGroup) : []
  const items = paymentGroups.flatMap((group: Loose) => [...group.refundingLines, ...group.preservedLines])
  return { patient, items, paymentGroups }
}
export async function c1VerifyOutpatientRefund(payload: any, context: Context = {}) { return unwrap(client.get('/charging/refund/verify-refund', payload, cfg(context))) }
export async function c1RefundOutpatientCharge(payload: any, context: Context = {}) {
  const body = {
    ...payload,
    paymentId: payload.paymentId ?? payload.originalPaymentId,
    refundRequestId: payload.refundRequestId ?? payload.refundId,
    paymentDetails: payload.paymentDetails ?? payload.refundPaymentDetails
  }
  return unwrap(client.post('/charging/refund/execute-refund', body, cfg(context)))
}
export async function c1SwitchRetainedItemsInsuranceMode(data: any, context: Context = {}) { return c1GetRefundableExecutionDetail(data.encounterId, context) }
export async function c1QueryRefundResult(refundId: string, context: Context = {}) { return unwrap(client.get('/charging/refund/verify-refund', { refundId }, cfg(context))) }
export async function c1RecoverRefund(refundId: string, context: Context = {}) { return c1QueryRefundResult(refundId, context) }

// 住院登记
export async function c1LoadInpatientRegistrationInit(_context: Context = {}) { return { paymentChannelOptions: [] } }
function mapRegistrationStatus(value: any): string {
  const status = asString(value).toUpperCase()
  if (status === 'IN_PROGRESS' || status === 'REGISTERED') return 'ADMITTED'
  if (status === 'WAITING' || status === 'PENDING') return 'WAIT_ADMISSION'
  return status || 'WAIT_ADMISSION'
}

function mapRegistrationQueueRow(row: Loose): Loose {
  const statusEnum = mapRegistrationStatus(row.statusEnum ?? row.status)
  return {
    ...row,
    encounterId: asString(row.encounterId ?? row.encounter_id ?? row.id),
    encounterBusNo: asString(row.encounterBusNo ?? row.inpatient_no ?? row.hospitalized_number),
    patientId: asString(row.patientId ?? row.patient_id),
    patientName: asString(row.patientName ?? row.patient_name ?? row.name),
    organizationId: asString(row.organizationId ?? row.organization_id),
    organizationName: asString(row.organizationName ?? row.dept_name ?? row.organization ?? row.depart),
    wardId: asString(row.wardId ?? row.ward_id),
    wardName: asString(row.wardName ?? row.ward_name),
    bedId: asString(row.bedId ?? row.bed_id),
    bedName: asString(row.bedName ?? row.bed_no ?? row.bed_number),
    startTime: asString(row.startTime ?? row.hospitalized_time ?? row.admit_date),
    statusEnum,
    statusEnumText: asString(row.statusEnumText ?? row.status_text ?? row.statusText) || (statusEnum === 'CANCELLED' ? '已取消' : statusEnum === 'ADMITTED' ? '已登记' : '待登记'),
    cancelable: Boolean(row.cancelable)
  }
}

export async function c1PageInpatientRegistrationQueue(query: any, context: Context = {}, _mockFilter: any = {}) { const data = await unwrap(client.get('/charging/enroll/query-all', { ...query, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 }, cfg(context))); const page = pageOf(data, query); const content = page.rows.map(mapRegistrationQueueRow); return { ...page, rows: content, records: content, content } }
export async function c1GetInpatientRegistrationContext(encounterId?: string, context: Context = {}) {
  if (encounterId) {
    const data = await unwrap<Loose>(client.get('/charging/enroll/query-by-id', { encounterId }, cfg(context)))
    if (data && (data.patient || data.encounter)) return data
    const row = data as Loose
    return {
      contractOptions: [],
      organizationOptions: row.organizationId ? [{ value: asString(row.organizationId), label: asString(row.organizationName ?? row.organizationId) }] : [],
      inWayCodeOptions: [],
      admitSourceCodeOptions: [],
      encounter: {
        encounterId: asString(row.encounterId ?? row.encounter_id),
        encounterBusNo: asString(row.encounterBusNo ?? row.inpatient_no),
        organizationId: asString(row.organizationId ?? row.organization_id),
        organizationName: asString(row.organizationName ?? row.dept_name),
        wardId: asString(row.wardId ?? row.ward_id),
        wardName: asString(row.wardName ?? row.ward_name),
        bedId: asString(row.bedId ?? row.bed_id),
        bedName: asString(row.bedName ?? row.bed_no),
        startTime: asString(row.startTime ?? row.admit_date),
        statusEnum: mapRegistrationStatus(row.statusEnum ?? row.status),
        inWayCode: asString(row.inWayCode ?? row.in_way_code),
        inWayCodeText: asString(row.inWayCodeText ?? row.in_way_code_text),
        admitSourceCode: asString(row.admitSourceCode ?? row.admit_source_code),
        admitSourceCodeText: asString(row.admitSourceCodeText ?? row.admit_source_code_text),
        outpatientDoctorName: asString(row.outpatientDoctorName ?? row.outpatient_doctor_name),
        attendingDoctorId: asString(row.attendingDoctorId ?? row.attending_doctor_id),
        attendingDoctorName: asString(row.attendingDoctorName ?? row.attending_doctor_name),
        primaryConditionDefinitionName: asString(row.conditionDefinitionName ?? row.primaryConditionDefinitionName),
        priorityEnum: asString(row.priorityEnum ?? row.priority)
      },
      patient: {
        patientId: asString(row.patientId ?? row.patient_id),
        name: asString(row.patientName ?? row.patient_name ?? row.name),
        genderEnum: asString(row.genderEnum ?? row.gender),
        birthDate: asString(row.birthDate ?? row.birth_date),
        idCard: asString(row.idCard ?? row.id_card),
        phone: asString(row.phone ?? row.phone_number)
      },
      readOnly: mapRegistrationStatus(row.statusEnum ?? row.status) !== 'WAIT_ADMISSION'
    }
  }
  const data = await unwrap(client.get('/charging/enroll/query-all', { pageIndex: 1, pageSize: 1000 }, cfg(context)))
  const organizations = new Map<string, { value: string; label: string }>()
  for (const row of rowsOf(data)) {
    const value = asString(row.organizationId ?? row.organization_id)
    const label = asString(row.organizationName ?? row.organization ?? row.dept_name)
    if (value) organizations.set(value, { value, label: label || value })
  }
  return { contractOptions: [], organizationOptions: [...organizations.values()], inWayCodeOptions: [], admitSourceCodeOptions: [], encounter: null, patient: null, readOnly: false }
}
export async function c1PageRegistrationPatientOptions(query: any, context: Context = {}) { const data = await unwrap(client.get('/charging/enroll/query-all', { ...query, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 }, cfg(context))); return pageOf(data, query) }
export async function c1ListRegistrationWards(departmentId: string, context: Context = {}) {
  // C1 对 query-all 携带 organizationId 会返回 HTTP 500；读取可用列表后在前端按科室筛选。
  const data = await unwrap(client.get('/charging/enroll/query-all', { pageIndex: 1, pageSize: 1000 }, cfg(context)))
  const rows = rowsOf(data).filter((row: Loose) => asString(row.organizationId ?? row.organization_id) === asString(departmentId))
  const unique = new Map<string, Loose>()
  for (const row of rows) {
    const wardId = asString(row.wardId ?? row.ward_id ?? row.id)
    const wardName = asString(row.wardName ?? row.ward_name ?? row.name)
    if (wardId && wardName) unique.set(wardId, { wardId, wardName, wardCode: asString(row.wardCode ?? row.ward_code) })
  }
  return { wards: [...unique.values()] }
}
export async function c1GetWardCapacity(wardId: string, _context: Context = {}) {
  // 当前 C1 没有稳定的病区床位容量接口；避免向 query-all 发送 wardId 触发 HTTP 500。
  return { totalBedCount: '—', vacantBedCount: '后端未提供', wardId }
}
export async function c1ListAttendingDoctors(query: any, context: Context = {}) { return { doctorOptions: [], primaryNurseOptions: [] } }
export async function c1SubmitInpatientRegistration(payload: any, context: Context = {}) { const body = { ...payload, admissionDate: payload.startTime ?? payload.admissionDate, admissionDept: payload.organizationId ?? payload.admissionDept, admission_ward: payload.wardId ?? payload.admission_ward, attending_doctor: payload.attendingDoctorId ?? payload.attending_doctor, name: payload.patientName ?? payload.name, idCard: payload.idCard ?? payload.id_number, prepayAmount: payload.advanceDepositAmount ?? payload.prepayAmount, payWay: payload.paymentDetails ?? payload.payWay }; return unwrap(client.post('/charging/admission/add', body, cfg(context))) }
export async function c1CancelInpatientRegistration(payload: any, context: Context = {}) { return unwrap(client.delete('/charging/admission/delete', payload, cfg(context))) }
export async function c1RevokeInsuranceAdmission(payload: any, context: Context = {}) { return c1CancelInpatientRegistration(payload, context) }

// 预交金管理
export async function c1PageInpatientQueue(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/pay_manage/getpatientqueue', omitEmpty({
    name: query.keyword,
    hospitalized_time: query.admissionDate,
    pageIndex: query.pageIndex ?? 1,
    pageSize: query.pageSize ?? 20
  }), cfg(context)))
  const page = pageOf(data, query)
  return { ...page, rows: page.rows.map((r: Loose) => mapPatient(r, 'inpatient')) }
}
function mapPrepayChannel(value: any): string {
  const text = asString(value).toUpperCase()
  if (/微信|WECHAT/.test(text)) return 'wechat'
  if (/支付宝|ALIPAY/.test(text)) return 'alipay'
  if (/银行卡|BANK_CARD|CARD/.test(text)) return 'card'
  return 'cash'
}

function mapPrepayRecord(row: Loose, inpatientNo: string): Loose {
  const amount = asNumber(row.amount ?? row.cost ?? row.refundAmount)
  const statusEnum = asString(row.statusEnum ?? row.status).toUpperCase()
  const type = /REFUND|退/.test(`${row.service_type ?? row.serviceType ?? ''}${statusEnum}`) ? 'refund' : 'collect'
  const status = /SUCCESS|SETTLED|COMPLETED/.test(statusEnum)
    ? 'settled'
    : /REFUND/.test(statusEnum)
      ? 'refunded'
      : /CANCEL/.test(statusEnum)
        ? 'cancelled'
        : 'processing'
  const transactionNo = asString(row.transactionNo ?? row.transaction_no ?? row.paymentNo ?? row.payment_id ?? row.id ?? `${inpatientNo}-${row.pay_time ?? ''}`)
  return {
    ...row,
    id: asString(row.paymentId ?? row.payment_id ?? row.id ?? transactionNo),
    paymentId: asString(row.paymentId ?? row.payment_id ?? row.id),
    encounterId: inpatientNo,
    type,
    amount,
    channel: mapPrepayChannel(row.channel ?? row.pay_type ?? row.payWay),
    transactionNo,
    createdAt: asString(row.createdAt ?? row.pay_time ?? row.createTime),
    operatorName: asString(row.operatorName ?? row.operateman),
    statusEnum,
    status,
    refundableAmount: type === 'collect' && status === 'settled' ? amount : 0,
    paymentDetails: row.paymentDetails
  }
}

export async function c1GetInpatientPrepaySummary(encounterId: string, context: Context = {}) {
  const data = await unwrap<Loose>(client.get('/charging/pay_manage/getpatientdetails', { hospitalized_number: encounterId }, cfg(context)))
  const patient = mapPatient({
    ...data,
    hospitalizedNumber: data.hospitalized_number,
    patientName: data.name,
    depart: data.depart,
    wardName: data.ward,
    bed_number: data.bed_number,
    encounterId
  }, 'inpatient')
  patient.id = encounterId
  patient.encounterId = encounterId
  patient.patientNo = encounterId
  patient.visitNo = encounterId
  patient.bedNo = asString(data.bed_number)
  if (data.age === undefined || data.age === null || data.age === '') {
    patient.age = undefined
    patient.ageText = ''
  }
  const balance = asNumber(data.rest ?? data.balance)
  return {
    patient,
    balance,
    totalCollected: 0,
    totalRefunded: 0,
    refundableBalance: balance
  }
}

export async function c1PagePrepayRecords(encounterId: string, query: any, context: Context = {}) {
  const params = { inpatientNo: encounterId, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 }
  let data = await unwrap<Loose>(client.get('/pay_manage/charge/query-prepayment', params, cfg(context)))
  let page = pageOf(data, query)
  // 服务器当前历史数据仍由兼容查询接口提供；官方路径无记录时回退读取同一住院号。
  if (!page.rows.length) {
    data = await unwrap<Loose>(client.get('/charging/pay_manage/getpatientpayrecords', {
      hospitalized_number: encounterId,
      pageIndex: params.pageIndex,
      pageSize: params.pageSize
    }, cfg(context)))
    page = pageOf(data, query)
  }
  const rows = page.rows.map((row: Loose) => mapPrepayRecord(row, encounterId))
  return { ...page, rows, records: rows, content: rows }
}

export async function c1CollectInpatientPrepay(payload: any, context: Context = {}) {
  return unwrap(client.post('/pay_manage/charge/add-prepayment', {
    ...payload,
    inpatientNo: payload.inpatientNo ?? payload.encounterBusNo ?? payload.encounterId,
    operatorId: payload.operatorId ?? (typeof localStorage === 'undefined' ? '' : localStorage.getItem('operatorId')) ?? ''
  }, cfg(context)))
}
export async function c1RefundInpatientPrepay(payload: any, context: Context = {}) {
  return unwrap(client.post('/pay_manage/charge/refund-prepayment', {
    ...payload,
    inpatientNo: payload.inpatientNo ?? payload.encounterBusNo ?? payload.encounterId,
    originalPaymentId: payload.originalPaymentId ?? payload.paymentId,
    operatorId: payload.operatorId ?? (typeof localStorage === 'undefined' ? '' : localStorage.getItem('operatorId')) ?? ''
  }, cfg(context)))
}
export async function c1QueryPrepayPaymentResult(paymentId: string, context: Context = {}) { return unwrap(client.get('/charging/pay_manage/getpatientpayrecords', { paymentId }, cfg(context))) }
export async function c1RecoverPrepaySettlement(paymentId: string, context: Context = {}) { return c1QueryPrepayPaymentResult(paymentId, context) }
export async function c1CancelPrepayPayment(paymentId: string, context: Context = {}) { return c1QueryPrepayPaymentResult(paymentId, context) }

// 住院结算
export async function c1LoadInpatientSettleInit(_context: Context = {}) { return { defaultPresentationMode: 'DETAIL', defaultSettleQueueTab: 'PENDING_SETTLE', paymentChannelOptions: [], presentationModeOptions: [], settleQueueTabOptions: [] } }
export async function c1PageInpatientSettleQueue(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/settle/getpatientlist', omitEmpty({
    patient_name: query.keyword,
    pageIndex: query.pageIndex ?? 1,
    pageSize: query.pageSize ?? 20
  }), cfg(context)))
  const page = pageOf(data, query)
  let registrationRows: Loose[] = []
  try {
    const registrationData = await unwrap(client.get('/charging/enroll/query-all', { pageIndex: 1, pageSize: 1000 }, cfg(context)))
    registrationRows = rowsOf(registrationData)
  } catch {
    // 登记资料仅用于补齐结算队列缺失的患者主键，不阻断患者列表。
  }
  const registrationByEncounter = new Map(
    registrationRows.map((row: Loose) => [asString(row.encounterId ?? row.encounter_id), row])
  )
  const registrationByNo = new Map(
    registrationRows.map((row: Loose) => [asString(row.encounterBusNo ?? row.inpatient_no), row])
  )
  const rows = page.rows.map((row: Loose) => {
    const registered = registrationByEncounter.get(asString(row.encounterId ?? row.encounter_id))
      ?? registrationByNo.get(asString(row.encounterBusNo ?? row.inpatient_no))
      ?? {}
    return mapPatient({
      ...row,
      encounterBusNo: row.encounterBusNo ?? row.inpatient_no,
      encounterId: registered.encounterId ?? registered.encounter_id ?? row.encounterId ?? row.encounter_id ?? row.id,
      patientId: row.patientId ?? row.patient_id ?? registered.patientId ?? registered.patient_id,
      patientName: row.patientName ?? row.patient_name ?? registered.patientName ?? registered.patient_name,
      organizationName: row.organizationName ?? registered.organizationName ?? row.dept_name,
      wardName: row.wardName ?? registered.wardName ?? row.ward_name,
      bedName: row.bedName ?? registered.bedName ?? row.bed_no,
      startTime: row.startTime ?? registered.startTime ?? row.admit_date,
      statusEnum: row.statusEnum ?? 'PENDING_SETTLE',
      statusEnumText: row.statusEnumText ?? '待结算',
      gender: row.genderText ?? row.gender,
    }, 'inpatient')
  })
  return { ...page, rows, records: rows, content: rows }
}
export async function c1GetInpatientSettleSummary(encounterId: string, patientId: string, selectedChargeItemIds: string[] = [], context: Context = {}) {
  const data = await unwrap(client.get('/charging/settle/getTotalFees', { encounter_id: encounterId }, cfg(context)))
  const rows = rowsOf(data)
  const pendingSettlementTotal = rows.reduce((sum: number, row: Loose) => sum + asNumber(row.amount ?? row.totalAmount ?? row.total_price), 0)
  return {
    encounterId,
    patientId,
    pendingSettlementTotal,
    selectedTotal: selectedChargeItemIds.length ? pendingSettlementTotal : 0,
    prepayNetBalance: 0,
    displayAccountBalance: 0,
    balanceHint: -pendingSettlementTotal,
    statusEnum: 'PENDING_SETTLE',
    statusEnumText: '待结算'
  }
}
export async function c1ListInpatientSettleCharges(encounterId: string, patientId: string, presentationMode = 'DETAIL', context: Context = {}) {
  const data = await unwrap<Loose>(client.get('/charging/settle/getCostDetail', { encounterId, patientId, presentationMode }, cfg(context)))
  const rawItems = Array.isArray(data.items) ? data.items : rowsOf(data)
  const lines = rawItems.map((row: Loose) => ({
    ...row,
    categoryCode: asString(row.categoryCode ?? row.category_code),
    categoryDisplay: asString(row.categoryDisplay ?? row.category_display ?? row.category),
    chargeItemId: asString(row.chargeItemId ?? row.charge_item_id ?? row.id),
    itemName: asString(row.itemName ?? row.item_name ?? row.name),
    lineAmount: asNumber(row.lineAmount ?? row.line_amount ?? row.amount ?? row.totalPrice),
    occurrenceTime: asString(row.occurrenceTime ?? row.occurrence_time),
    quantityUnit: asString(row.quantityUnit ?? row.quantity_unit ?? row.unit),
    quantityValue: row.quantityValue ?? row.quantity_value ?? row.quantity,
    selected: row.selected !== false,
    sourceText: asString(row.sourceText ?? row.source_text),
    specText: asString(row.specText ?? row.spec_text ?? row.specification),
    statusEnum: asString(row.statusEnum ?? row.status_enum ?? row.status ?? 'PENDING'),
    statusEnumText: asString(row.statusEnumText ?? row.status_enum_text ?? row.statusText),
    unitPrice: row.unitPrice ?? row.unit_price
  }))
  const categoryMap = new Map<string, Loose>()
  for (const line of lines) {
    const key = line.categoryCode || line.categoryDisplay || 'OTHER'
    const category = categoryMap.get(key) ?? { categoryCode: line.categoryCode || key, categoryDisplay: line.categoryDisplay || '其他', amount: 0, lineCount: 0 }
    category.amount = asNumber(category.amount) + asNumber(line.lineAmount)
    category.lineCount += 1
    categoryMap.set(key, category)
  }
  return {
    ...data,
    encounterId,
    presentationMode,
    defaultSelectAll: true,
    pendingSettlementTotal: asNumber(data.unsettledAmount ?? data.pendingSettlementTotal ?? data.totalAmount),
    categories: Array.isArray(data.categories) ? data.categories : [...categoryMap.values()],
    lines
  }
}
export async function c1ListInpatientSettleChargeCategories(query: any, context: Context = {}) { return unwrap(client.get('/charging/settle/getCategoryDetails', query, cfg(context))) }
export async function c1ListInpatientSettleChargeDetails(query: any, context: Context = {}) { return unwrap(client.get('/charging/settle/getCategoryDetails', query, cfg(context))) }
export async function c1PreSettleInpatientCharges(payload: any, context: Context = {}) { return unwrap(client.post('/charging/settle/settlement', { ...payload, accountId: payload.accountId ?? payload.contractId, contractNo: payload.contractNo ?? payload.contractId }, cfg(context))) }
export async function c1ConfirmInpatientSettle(payload: any, context: Context = {}) { return unwrap(client.post('/charging/settle/settlement', { ...payload, accountId: payload.accountId ?? payload.contractId, contractNo: payload.contractNo ?? payload.contractId }, cfg(context))) }
export async function c1ListInpatientSettlementHistory(encounterId: string, patientId: string, context: Context = {}) {
  const data = await unwrap<Loose>(client.get('/charging/settle/getSettleHistory', { encounterId, patientId, pageIndex: 1, pageSize: 20 }, cfg(context)))
  const rows = rowsOf(data)
  const first = rows[0] ?? {}
  return {
    ...data,
    activePaymentId: asString(data.activePaymentId ?? first.activePaymentId ?? first.settlementId),
    encounterId: asString(data.encounterId ?? first.encounterId ?? encounterId),
    operatorName: asString(data.operatorName ?? first.operatorName),
    payMethodText: asString(data.payMethodText ?? first.payMethodText),
    payments: rows.map((row: Loose) => ({
      ...row,
      paymentId: asString(row.paymentId ?? row.settlementId ?? row.activePaymentId),
      receivableTotal: row.receivableTotal ?? row.displayAmount ?? 0,
      recordType: row.recordType ?? 'FORWARD',
      relationId: asString(row.relationId),
      reverseDisabledReason: asString(row.reverseDisabledReason),
      reverseEnabled: Boolean(row.reverseEnabled),
      signedAmount: row.signedAmount ?? row.netSettleTotal ?? row.displayAmount ?? 0,
      settleTime: asString(row.settleTime ?? row.billDate),
      statusEnum: asString(row.statusEnum ?? row.status_enum)
    })),
    categories: data.categories ?? first.categories ?? [],
    lines: data.lines ?? first.lines ?? [],
    netSettleTotal: data.netSettleTotal ?? first.netSettleTotal ?? 0,
    receivableTotal: data.receivableTotal ?? first.receivableTotal ?? 0,
    settleTime: asString(data.settleTime ?? first.settleTime ?? first.billDate)
  }
}
export async function c1GetSettledInpatientPayment(encounterId: string, patientId: string, settlementId: string, context: Context = {}) { return unwrap(client.get('/charging/settle/getSettleHistory', { encounterId, patientId, paymentId: settlementId }, cfg(context))) }
export async function c1ReverseInpatientSettle(encounterId: string, patientId: string, settlementId: string, reason: string, context: Context = {}) { return unwrap(client.post('/charging/settle/settlement', { encounterId, patientId, paymentId: settlementId, reason }, cfg(context))) }
