import HttpClient, { DataUpType } from '@/apis/http'
import { getActivePinia } from 'pinia'
import { useUserStore } from '@/stores/user'

type Loose = Record<string, any>
type Envelope<T = any> = { code?: number; message?: string; data?: T }
type Context = { signal?: AbortSignal }

const client = new HttpClient(
  { baseURL: import.meta.env.VITE_C1_CHARGE_SERVER_URL || '/c1-charge' },
  {
    requestInterceptor: (config) => {
      const storedToken =
        typeof localStorage === 'undefined' ? '' : localStorage.getItem('token') || ''
      const sessionToken = getActivePinia() ? useUserStore().getToken : ''
      const token = String(sessionToken || storedToken).trim()
      const head = (
        typeof localStorage === 'undefined'
          ? 'Bearer'
          : localStorage.getItem('tokenHead') || 'Bearer'
      ).trim()
      if (token) config.headers.Authorization = head ? `${head} ${token}` : token
      if (config.upType === DataUpType.json)
        config.headers['Content-Type'] = 'application/json;charset=UTF-8'
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
  const total = Number(
    value?.total ?? value?.totalCount ?? value?.totalElements ?? value?.totalRecords ?? rows.length
  )
  return {
    rows,
    records: rows,
    content: rows,
    total,
    totalCount: total,
    totalElements: total,
    pageIndex,
    pageSize
  }
}

function omitEmpty(value: Loose): Loose {
  return Object.fromEntries(
    Object.entries(value).filter(([, item]) => item !== undefined && item !== null && item !== '')
  )
}

function currentOperator() {
  if (typeof localStorage === 'undefined') return { id: '', name: '' }
  const storedId = asString(
    localStorage.getItem('operatorId') ?? localStorage.getItem('userId')
  ).trim()
  const storedName = asString(
    localStorage.getItem('operatorName') ?? localStorage.getItem('userName')
  ).trim()
  if (storedId) return { id: storedId, name: storedName }
  try {
    const encoded = asString(localStorage.getItem('token')).split('.')[1]
    if (!encoded || typeof atob !== 'function') return { id: '', name: storedName }
    const base64 = encoded.replaceAll('-', '+').replaceAll('_', '/')
    const bytes = atob(base64.padEnd(Math.ceil(base64.length / 4) * 4, '='))
    const json = decodeURIComponent(
      Array.from(bytes, (char) => `%${char.charCodeAt(0).toString(16).padStart(2, '0')}`).join('')
    )
    const claims = JSON.parse(json) as Loose
    return {
      id: asString(
        claims.practitionerId ?? claims.practitioner_id ?? claims.id ?? claims.userId ?? claims.sub
      ).trim(),
      name: asString(
        claims.practitionerName ?? claims.name ?? claims.user_name ?? claims.sub
      ).trim()
    }
  } catch {
    return { id: '', name: storedName }
  }
}

const asString = (v: any) => (v === undefined || v === null ? '' : String(v))
const asNumber = (v: any, fallback = 0) => {
  const n = Number(v)
  return Number.isFinite(n) ? n : fallback
}

const admissionDateOnly = (value: any) => {
  const text = asString(value).trim()
  if (!text) return ''
  const match = text.match(/^(\d{4}-\d{2}-\d{2})/)
  return match?.[1] ?? text
}

function c1DateTime(value?: any): string {
  const text = asString(value).trim()
  if (/^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/.test(text)) return text
  const now = new Date().toLocaleString('sv-SE').replace('T', ' ').slice(0, 19)
  if (/^\d{4}-\d{2}-\d{2}$/.test(text)) return `${text} ${now.slice(11)}`
  return now
}

function c1PaymentChannel(value: any): string {
  const channel = asString(value).trim().toUpperCase()
  return ({ CARD: 'BANK_CARD', BANKCARD: 'BANK_CARD' } as Loose)[channel] ?? channel
}

function isUsableRegisterReference(row: Loose): boolean {
  const serviceTypeId = asString(row.serviceTypeId).trim()
  const serviceTypeName = asString(row.register_type ?? row.serviceTypeName).trim()
  const organization = asString(row.organization_id ?? row.organization).trim()
  if (!serviceTypeId || !serviceTypeName || !organization) return false
  return !/^(string|hello|test|你好)$/i.test(serviceTypeId)
}

function ageFromBirthDate(value: any): number {
  const date = new Date(asString(value))
  if (Number.isNaN(date.getTime())) return 0
  const now = new Date()
  let age = now.getFullYear() - date.getFullYear()
  if (
    now.getMonth() < date.getMonth() ||
    (now.getMonth() === date.getMonth() && now.getDate() < date.getDate())
  )
    age -= 1
  return Math.max(0, age)
}

const c1GenderText = (value: any) => {
  const gender = asString(value).trim().toUpperCase()
  if (gender === 'MALE' || gender === 'M') return '男'
  if (gender === 'FEMALE' || gender === 'F') return '女'
  return asString(value)
}

const c1GenderCode = (value: any) => {
  const gender = asString(value).trim().toUpperCase()
  if (gender === 'MALE' || gender === 'M' || gender === '男') return 'MALE'
  if (gender === 'FEMALE' || gender === 'F' || gender === '女') return 'FEMALE'
  return gender
}

const c1PatientConditionText = (value: any) =>
  ({ GENERAL: '一般', URGENT: '急', CRITICAL: '危' })[asString(value).toUpperCase()] ??
  asString(value)
const c1AdmissionTypeText = (value: any) => ({ '6': '普通' })[asString(value)] ?? asString(value)
const c1AdmissionModeText = (value: any) =>
  ({ '1': '普通入院' })[asString(value)] ?? asString(value)
const c1FeeNatureText = (value: any) =>
  ({ 'CONTRACT-SELF': '自费', 'CONTRACT-MI': '医保' })[asString(value)] ?? asString(value)

function normalizeSettleQueueStatus(value: any): string {
  const status = asString(value).trim().toUpperCase()
  if (
    !status ||
    /UNSETTLED|IN_PROGRESS|REGISTERED|INHOSPITAL|WAIT_SETTLE|PENDING_SETTLE/.test(status)
  ) {
    return 'PENDING_SETTLE'
  }
  if (/SETTLED|COMPLETED|COMPLETED_SETTLE|DISCHARGED/.test(status)) return 'SETTLED'
  return status
}

function normalizeSettleLineStatus(value: any): string {
  const status = asString(value).trim().toUpperCase()
  if (!status || /UNSETTLED|IN_PROGRESS|REGISTERED|INHOSPITAL|PENDING/.test(status))
    return 'PENDING'
  if (/SETTLED|COMPLETED|PAID/.test(status)) return 'SETTLED'
  return status
}
const dateRange = (q: any) => ({
  ...(q.startDate || q.dateFrom ? { startDate: q.startDate ?? q.dateFrom } : {}),
  ...(q.endDate || q.dateTo ? { endDate: q.endDate ?? q.dateTo } : {})
})

function mapPatient(row: Loose, kind: 'charge' | 'refund' | 'inpatient' = 'charge'): Loose {
  // C1 部分住院队列只返回 hospitalized_number/inpatient_no。保留真实主键优先，
  // 没有主键时至少使用住院号作为查询标识，避免患者卡片点击后 id 为空、页面看起来无响应。
  const encounterBusNo = asString(
    row.encounterBusNo ??
      row.visitNumber ??
      row.hospitalizedNumber ??
      row.hospitalized_number ??
      row.inpatient_no
  )
  const encounterId = asString(row.encounterId ?? row.encounter_id ?? row.id ?? encounterBusNo)
  const patientId = asString(row.patientId ?? row.patient_id)
  const name = asString(row.patientName ?? row.patient_name ?? row.name)
  const department = asString(
    row.organizationName ?? row.organization ?? row.dept_name ?? row.depart
  )
  const totalAmount = asNumber(row.totalAmount ?? row.amount ?? row.cost)
  const paidAmount = asNumber(row.paidAmount ?? row.paid ?? row.settledAmount)
  const statusEnum = asString(
    row.queueLabelCode ??
      row.cashierChargeRollupStatusCode ??
      row.statusEnum ??
      row.status ??
      'PENDING'
  ).toUpperCase()
  // C1 的队列服务只把 DRAFT 当作待收费，但其他收费代码还会生成 UNPAID。
  // 当总费用大于已支付金额时，以实际未付余额为准，避免患者被错误归入“已收费”。
  const hasOutstandingBalance = totalAmount > paidAmount + 0.000001
  const settled =
    !hasOutstandingBalance && /^(SETTLED|COMPLETED|PAID|SUCCESS)$/.test(statusEnum)
  return {
    ...row,
    id: encounterId,
    encounterId,
    patientId,
    patientNo: asString(
      row.patientNo ?? row.hospitalizedNumber ?? row.hospitalized_number ?? patientId
    ),
    encounterBusNo,
    visitNo: asString(
      row.visitNo ?? row.encounterBusNo ?? row.visitNumber ?? row.inpatient_no ?? encounterId
    ),
    patientName: name,
    name,
    genderCode: asString(row.genderCode ?? row.sex),
    genderEnumText: asString(
      row.genderEnumText ?? row.sexText ?? row.genderText ?? row.gender ?? row.sex
    ),
    gender: asString(row.gender ?? row.sexText ?? row.genderText ?? row.sex),
    age: asNumber(row.age),
    ageText: asString(row.ageText ?? row.age),
    organizationName: department,
    departmentName: department,
    doctorName: asString(row.doctorName ?? row.doctor),
    startTime: asString(
      row.startTime ??
        row.hospitalizedTime ??
        row.hospitalized_time ??
        row.admissionDate ??
        row.admit_date
    ),
    visitTime: asString(
      row.visitTime ??
        row.startTime ??
        row.hospitalizedTime ??
        row.hospitalized_time ??
        row.admissionDate ??
        row.admit_date
    ),
    totalAmount,
    paidAmount,
    paymentId: asString(row.paymentId),
    queueLabelCode: asString(row.queueLabelCode ?? row.cashierChargeRollupStatusCode ?? statusEnum),
    statusEnum,
    status: /REFUND|RETURN|CANCEL/i.test(statusEnum)
      ? 'refunded'
      : settled
        ? 'settled'
        : 'pending',
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
    status: /SETTLE|PAID|COMPLET|REFUND/i.test(asString(row.statusCode ?? row.statusEnum))
      ? 'settled'
      : 'pending'
  }
}

// 门诊挂号
const c1RegisterTypeById = new Map<string, string>()
const c1RegisterCostById = new Map<string, number>()
const c1RegisterOrganizationNameById = new Map<string, string>()
const c1RegisterDoctorNameById = new Map<string, string>()
const c1RegisterSlotById = new Map<string, Loose>()
const c1RegisterDrafts = new Map<string, Loose>()
const c1RegisterHistoryById = new Map<string, Loose>()

function c1LegacyRegisterSlot(): Loose | undefined {
  if (import.meta.env.VITE_C1_ALLOW_LEGACY_REGISTER_SLOT !== 'true') return undefined
  return {
    id: '329701000000000070',
    status: 'free',
    capacity: 30,
    booked_count: 0,
    register_type: '普通号',
    // C1 联调测试数据中的普通号诊察费为 5 元；后端恢复返回 registerFee 后优先使用后端值。
    fee: 5,
    organization: '',
    start_time: '2026-08-10 08:00:00',
    end_time: '2026-08-10 18:00:00'
  }
}

function c1RegisterOrganizationId(value: any): string {
  const organizationId = asString(value).trim()
  if (/^\d{10,}$/.test(organizationId)) return organizationId
  return asString(import.meta.env.VITE_C1_DEFAULT_ORGANIZATION_ID).trim() || organizationId
}

function c1RegisterStatus(value: any): string {
  const status = asString(value).trim().toUpperCase()
  if (status === 'REGISTERED') return 'PLANNED'
  if (/CANCEL|REFUND|RETURN/.test(status)) return 'CANCELLED'
  return status
}

function isC1OutpatientRegisterRecord(row: Loose): boolean {
  const classEnum = asString(row.classEnum ?? row.class_enum)
    .trim()
    .toUpperCase()
  if (classEnum && /^(IMP|IPD|INPATIENT)$/.test(classEnum)) return false
  const number = asString(row.number ?? row.encounterBusNo ?? row.bus_no)
    .trim()
    .toUpperCase()
  if (number.startsWith('ZY')) return false
  if (number.startsWith('EN')) return true
  const status = asString(row.statusEnum ?? row.status)
    .trim()
    .toUpperCase()
  if (status === 'INHOSPITAL') return false
  const registerType = asString(row.register_type ?? row.registerType ?? row.serviceTypeName).trim()
  const organization = asString(row.organizationName ?? row.organization).trim()
  return Boolean(registerType) && !organization.includes('住院')
}

function registerResultId(value: any): string {
  if (typeof value === 'string' || typeof value === 'number') return asString(value)
  if (!value || typeof value !== 'object') return ''
  for (const key of ['encounterId', 'encounter_id', 'visitNumber', 'visit_number', 'id', 'data']) {
    const result = registerResultId(value[key])
    if (result) return result
  }
  return ''
}

export async function c1LoadOutpatientRegisterInit(query: any = {}, context: Context = {}) {
  c1RegisterCostById.clear()
  c1RegisterSlotById.clear()
  let rows: any[] = []
  try {
    rows = rowsOf(
      await unwrap(
        client.get(
          '/charging/register/getseeing',
          {
            pageIndex: 1,
            pageSize: 1000
          },
          cfg(context)
        )
      )
    )
  } catch {
    // 初始化选项失败不阻断页面，列表查询会再次报告真实错误。
  }
  rows = rows.filter(isUsableRegisterReference)
  const options = (key: string, labelKey = key, fallbackKey?: string) =>
    Array.from(
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
    if (id && label) {
      c1RegisterTypeById.set(id, label)
      // getseeing 返回的 cost 是历史记录总费用，不一定是本次挂号费，不能直接用于试算。
      const fee = row.registerFee ?? row.register_fee ?? row.fee ?? row.price ?? row.unitPrice
      if (fee !== undefined && fee !== null && fee !== '') {
        c1RegisterCostById.set(id, asNumber(fee))
      }
    }
    const organizationId = asString(row.organization_id) || asString(row.organization)
    const organizationName = asString(row.organization) || organizationId
    if (organizationId) c1RegisterOrganizationNameById.set(organizationId, organizationName)
    const doctorId = asString(row.doctor_id) || asString(row.doctor)
    const doctorName = asString(row.doctor) || doctorId
    if (doctorId) c1RegisterDoctorNameById.set(doctorId, doctorName)
  }
  return {
    departmentOptions: options('organization_id', 'organization', 'organization'),
    serviceTypeOptions: options('serviceTypeId', 'register_type', 'register_type'),
    contractOptions: options('contractNo', 'cost_type', 'cost_type').concat(
      rows.some((row) => row.cost_type === '自费')
        ? []
        : [{ value: 'CONTRACT-SELF', label: '自费' }]
    ),
    paymentChannelOptions: [
      { value: 'CASH', label: '现金' },
      { value: 'BANK_CARD', label: '银行卡' },
      { value: 'WECHAT', label: '微信' },
      { value: 'ALIPAY', label: '支付宝' }
    ],
    genderOptions: [
      { value: 'MALE', label: '男' },
      { value: 'FEMALE', label: '女' }
    ],
    practitionerOptions: options('doctor_id', 'doctor', 'doctor')
  }
}
export async function c1PageRegisterPatientOptions(query: any, context: Context = {}) {
  const data = await unwrap(
    client.get(
      '/charging/register/getseeing',
      {
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 20,
        ...(query.keyword ? { name: query.keyword } : {})
      },
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  const rows = page.rows.map((row: Loose) => ({
    patientId: asString(row.patient_id ?? row.patientId ?? row.id),
    patientBusNo: asString(row.patient_no ?? row.patientBusNo ?? row.number),
    name: asString(row.name),
    genderCode: c1GenderCode(row.genderCode ?? row.sex),
    genderText: c1GenderText(row.genderCode ?? row.sex),
    age: row.age,
    birthDate: asString(row.birth_date),
    phoneMasked: asString(row.phone),
    idCardNoMasked: asString(row.id_number)
  }))
  return { ...page, rows, records: rows, content: rows }
}
export async function c1ListRegisterPractitioners(query: any, context: Context = {}) {
  const data = await unwrap(
    client.get('/charging/register/getseeing', { pageIndex: 1, pageSize: 1000 }, cfg(context))
  )
  const rows = rowsOf(data).filter(
    (row: Loose) =>
      isUsableRegisterReference(row) &&
      asString(row.doctor).trim() &&
      (!query.organizationId ||
        [row.organization_id, row.organization]
          .map(asString)
          .includes(asString(query.organizationId))) &&
      (!query.keyword || asString(row.doctor).includes(asString(query.keyword)))
  )
  const options = new Map<string, Loose>()
  for (const row of rows) {
    const value = asString(row.doctorId) || asString(row.doctor_id) || asString(row.doctor)
    const label = asString(row.doctorName) || asString(row.doctor) || value
    if (value) options.set(value, { value, label, practitionerId: value, practitionerName: label })
  }
  return { practitionerOptions: [...options.values()] }
}
export async function c1ListBookableSlotHints(_query: any, context: Context = {}) {
  // C1 当前带 doctor/register_type 会错误返回空列表，先读取全部当日号源供页面使用。
  const data = await unwrap(
    client.get(
      '/charging/register/get_today_appoint',
      {
        pageIndex: 1,
        pageSize: 1000
      },
      cfg(context)
    )
  )
  const serverRows = rowsOf(data)
  const legacySlot = c1LegacyRegisterSlot()
  const slotRows = serverRows.length > 0 ? serverRows : legacySlot ? [legacySlot] : []
  return {
    hints: slotRows
      .map((r) => ({
        slotId: r.slotId ?? r.id,
        slotStart: r.slotStart ?? r.startTime ?? r.start_time,
        slotEnd: r.slotEnd ?? r.endTime ?? r.end_time,
        remainingCount:
          r.remainingCount ??
          r.remaining ??
          Math.max(0, asNumber(r.capacity) - asNumber(r.booked_count))
      }))
      .map((hint: Loose, index: number) => {
        const row = slotRows[index] ?? {}
        const slotId = asString(hint.slotId)
        const fee = row.registerFee ?? row.register_fee ?? row.fee ?? row.price ?? row.unitPrice
        if (slotId) c1RegisterSlotById.set(slotId, { ...row, fee })
        return {
          ...hint,
          ...(fee !== undefined && fee !== null && fee !== '' ? { fee: asNumber(fee) } : {})
        }
      })
  }
}
export async function c1LoadRegisterBenefitPreview(_payload: any, _context: Context = {}) {
  return { benefitKind: 'NONE' }
}
export async function c1PreSettleOutpatientRegister(payload: any, _context: Context = {}) {
  const serviceTypeId = asString(payload.serviceTypeId)
  const slot = c1RegisterSlotById.get(asString(payload.slotId))
  const slotFee = slot?.fee
  const totalAmount = asNumber(
    payload.registerFee ??
      payload.register_fee ??
      (slotFee !== undefined && slotFee !== '' ? slotFee : c1RegisterCostById.get(serviceTypeId)),
    0
  )
  const paymentDraftId = `C1-REGISTER-${Date.now()}`
  c1RegisterDrafts.set(paymentDraftId, { payload: { ...payload }, registered: false })
  return {
    appointmentId: payload.appointmentId,
    chargeItems: [
      {
        amount: totalAmount,
        chargeItemCode: serviceTypeId,
        chargeItemName: c1RegisterTypeById.get(serviceTypeId) ?? '挂号费',
        quantity: 1,
        unitPrice: totalAmount
      }
    ],
    paymentDraftId,
    slotId: payload.slotId,
    totalAmount,
    insurancePreSettled: false,
    ybFundPaySumamt: 0,
    ybAcctPay: 0,
    ybPsnCashPay: totalAmount
  }
}
export async function c1ConfirmOutpatientRegister(payload: any, context: Context = {}) {
  const draftId = asString(payload.paymentDraftId)
  const draft = c1RegisterDrafts.get(draftId)
  if (!draft) throw new Error('挂号费用试算已失效，请重新试算')
  const source = draft.payload ?? {}
  const details = Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []
  const amount = details.reduce(
    (sum: number, item: Loose) => sum + asNumber(item.amount ?? item.payAmount),
    0
  )
  if (amount <= 0) {
    throw new Error('C1 当日号源未返回有效挂号费，当前无法确认缴费')
  }
  if (!draft.registered) {
    const operator = currentOperator()
    const organizationId = c1RegisterOrganizationId(source.organizationId)
    const practitionerId = asString(source.practitionerId).trim() || operator.id
    const contractId = asString(source.contractId).trim()
    const registerBody = omitEmpty({
      name: asString(source.name),
      id_number: asString(source.idCardNo ?? source.id_number),
      phone: asString(source.phone),
      sex: c1GenderText(source.gender ?? source.sex),
      birth_date: admissionDateOnly(source.birthDate ?? source.birth_date),
      age: asNumber(source.age, ageFromBirthDate(source.birthDate ?? source.birth_date)),
      organization:
        c1RegisterOrganizationNameById.get(asString(source.organizationId)) ??
        asString(source.organizationId),
      doctor:
        (c1RegisterDoctorNameById.get(asString(source.practitionerId)) ??
          asString(source.practitionerId)) ||
        operator.name,
      patient_id: asString(source.patientId),
      doctor_id: practitionerId,
      organization_id: organizationId,
      operator_id: operator.id,
      tenant_id: asString(import.meta.env.VITE_C1_DEFAULT_TENANT_ID).trim() || '1',
      serviceTypeId: asString(source.serviceTypeId),
      slotId: asString(source.slotId),
      contractNo: contractId === 'CONTRACT-SELF' ? '1' : contractId,
      role_code: 'ATTENDING',
      balance: 0,
      type_code: 'PERSONAL_CASH_ACCOUNT'
    })
    try {
      draft.registerResult = await unwrap(
        client.post('/charging/register/add_register', registerBody, cfg(context))
      )
    } catch (error) {
      const message = error instanceof Error ? error.message : ''
      if (/operation failure|接口请求失败/i.test(message)) {
        throw new Error('新增挂号记录失败：C1 后端未完成挂号记录、号源和收费账户的关联写入')
      }
      throw error
    }
    const encounterId = registerResultId(draft.registerResult)
    if (!encounterId) {
      throw new Error('C1 挂号接口未返回有效就诊标识，无法继续确认缴费')
    }
    draft.registerResult = encounterId
    draft.registered = true
  }
  const paymentChannel = asString(details[0]?.payChannelCode).toUpperCase()
  const paymentMethod =
    ({ CASH: 1, WECHAT: 2, ALIPAY: 3, BANK_CARD: 4 } as Loose)[paymentChannel] ?? 1
  let paymentResult: any
  try {
    paymentResult = await unwrap(
      client.post(
        '/charging/register/chargeconfirmcontroller',
        omitEmpty({
          visitNumber: asString(draft.registerResult),
          patientId: asString(source.patientId),
          name: asString(source.name),
          phone: asString(source.phone),
          amount,
          paymentDraft: draftId,
          registrationType:
            c1RegisterTypeById.get(asString(source.serviceTypeId)) ??
            asString(source.serviceTypeId),
          paymentMethod,
          transactionId: asString(details[0]?.transactionNo),
          thirdPartyTransactionId: asString(payload.clientScanResult?.tradeNo),
          paymentTime: new Date().toLocaleString('sv-SE').replace('T', ' ').slice(0, 19),
          receiptData: payload.clientScanResult ? JSON.stringify(payload.clientScanResult) : '',
          paymentStatus: 1
        }),
        cfg(context)
      )
    )
  } catch (error) {
    const message = error instanceof Error ? error.message : ''
    if (/operation failure|接口请求失败/i.test(message)) {
      throw new Error(
        '挂号记录已创建，但确认缴费失败：后端未找到对应的支付草稿或挂号收费项目，请后端补齐挂号收费初始化'
      )
    }
    throw error
  }
  c1RegisterDrafts.delete(draftId)
  return {
    encounterId: asString(draft.registerResult),
    invoiceId: '',
    invoiceStatus: 'NOT_PROVIDED',
    paymentId: asString(paymentResult) || draftId,
    status: 'SUCCESS'
  }
}
export async function c1PageOutpatientRegisterHistory(query: any, context: Context = {}) {
  // C1 当前 getseeing 对 startDate/endDate 会返回 unordered_map::at；先取可分页的全量结果，再在适配层按日期筛选。
  const data = await unwrap(
    client.get(
      '/charging/register/getseeing',
      omitEmpty({ keyword: query.keyword, pageIndex: 1, pageSize: 1000 }),
      cfg(context)
    )
  )
  let rows = rowsOf(data).filter(isC1OutpatientRegisterRecord)
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
  const pageRows = rows.slice(
    Math.max(0, pageIndex - 1) * pageSize,
    Math.max(0, pageIndex - 1) * pageSize + pageSize
  )
  const records = pageRows.map((r: Loose) => {
    const backendStatusEnum = asString(r.statusEnum ?? r.status)
      .trim()
      .toUpperCase()
    const record = {
      ...mapPatient(r),
      amount: asNumber(r.cost ?? r.amount),
      backendStatusEnum,
      canCancel: backendStatusEnum === 'REGISTERED' || backendStatusEnum === 'PLANNED',
      contactPhone: asString(r.phone),
      doctorName: asString(r.doctorName ?? r.doctor),
      organizationName: asString(r.organizationName ?? r.organization),
      serviceTypeId: asString(r.serviceTypeId),
      serviceTypeName: asString(r.registerType ?? r.serviceTypeName ?? r.register_type),
      startTime: asString(r.startTime ?? r.time),
      statusEnum: c1RegisterStatus(backendStatusEnum),
      statusText: asString(r.statusText ?? r.statusEnumText)
    }
    c1RegisterHistoryById.set(asString((record as Loose).encounterId), record)
    return record
  })
  return {
    rows: records,
    records,
    content: records,
    total: rows.length,
    totalCount: rows.length,
    totalElements: rows.length,
    pageIndex,
    pageSize
  }
}
export async function c1GetTodayRegisterSummary(query: any, context: Context = {}) {
  const data = await c1PageOutpatientRegisterHistory(
    { dateFrom: query.date, dateTo: query.date, pageIndex: 1, pageSize: 1000 },
    context
  )
  return {
    date: query.date,
    totalCount: data.total,
    activeCount: data.records.filter((r: any) => !/CANCEL/i.test(r.statusEnum)).length,
    cancelledCount: data.records.filter((r: any) => /CANCEL/i.test(r.statusEnum)).length
  }
}
export async function c1GetRegisterAppointment(query: any, context: Context = {}) {
  const data = await unwrap(client.get('/charging/register/get_today_appoint', query, cfg(context)))
  return rowsOf(data)[0] ?? data
}
export async function c1ValidateRegisterCancellation(payload: any, context: Context = {}) {
  const record = c1RegisterHistoryById.get(asString(payload.encounterId))
  if (!record) {
    return {
      cancelable: false,
      reasonCode: 'REGISTER_NOT_FOUND',
      reasonText: '当前挂号记录未加载，请刷新就诊记录后重试'
    }
  }
  if (!record.canCancel) {
    return {
      cancelable: false,
      reasonCode: 'STATUS_NOT_CANCELABLE',
      reasonText: `当前状态“${record.backendStatusEnum || record.statusEnum || '未知'}”不允许退号`
    }
  }
  return { cancelable: true, reasonCode: undefined, reasonText: undefined }
}

function c1RegisterCancellationBody(payload: any): Loose {
  const record = c1RegisterHistoryById.get(asString(payload.encounterId))
  if (!record) throw new Error('当前挂号记录未加载，请刷新就诊记录后重试')
  const refundAmount = asNumber(record.amount)
  if (refundAmount <= 0) {
    throw new Error('当前挂号金额为 0，C1 退号接口暂不支持无费用退号，请后端允许退款金额为 0')
  }
  const requiredLinks = {
    accountId: asString(record.accountId ?? record.account_id),
    chargeItemId: asString(record.chargeItemId ?? record.charge_item_id),
    detailId: asString(record.detailId ?? record.detail_id),
    invoiceId: asString(record.invoiceId ?? record.invoice_id),
    patientId: asString(record.patientId ?? record.patient_id),
    paymentNo: asString(record.paymentNo ?? record.payment_no ?? record.paymentId),
    slotId: asString(record.slotId ?? record.slot_id)
  }
  const missing = Object.entries(requiredLinks)
    .filter(([, value]) => !value)
    .map(([key]) => key)
  if (missing.length) {
    throw new Error(`C1 挂号记录未返回退号所需字段：${missing.join('、')}`)
  }
  return omitEmpty({
    visitNumber: asString(record.encounterId),
    name: asString(record.patientName),
    phone: asString(record.contactPhone),
    department: asString(record.organizationName),
    doctor: asString(record.doctorName),
    registrationType: asString(record.serviceTypeName),
    registrationTime: asString(record.startTime),
    refundAmount,
    status: asString(record.backendStatusEnum || 'REGISTERED'),
    encounterId: asString(record.encounterId),
    version: asNumber(record.version, 1),
    ...requiredLinks,
    payEnum: asString(record.payEnum ?? record.pay_enum),
    refundingChargeItemIds: asString(
      record.refundingChargeItemIds ??
        record.refunding_charge_item_ids ??
        requiredLinks.chargeItemId
    ),
    refundRequestId: asString(payload.refundRequestId)
  })
}

export async function c1CancelOutpatientRegister(payload: any, context: Context = {}) {
  return unwrap(
    client.post('/charging/register/quitnumber', c1RegisterCancellationBody(payload), cfg(context))
  )
}
export async function c1CancelRegisterWithRefund(payload: any, context: Context = {}) {
  return unwrap(
    client.post('/charging/register/quitnumber', c1RegisterCancellationBody(payload), cfg(context))
  )
}
export async function c1GetRegisterTicket(query: any, context: Context = {}) {
  const encounterId = asString(query.encounterId)
  if (!encounterId) throw new Error('缺少就诊标识，无法加载挂号小票')
  const operator = currentOperator()
  const historyData = await unwrap(
    client.get('/charging/register/getseeing', { pageIndex: 1, pageSize: 1000 }, cfg(context))
  )
  const row =
    rowsOf(historyData).find((item: Loose) =>
      [item.encounterId, item.encounter_id, item.id, item.number]
        .map(asString)
        .includes(encounterId)
    ) ?? {}
  const printBody = {
    printType: 'REGISTER_RECEIPT',
    isDefault: true,
    traceId: `C1-REGISTER-${Date.now()}`,
    bindingCode: 'op-reg-print',
    templateVersion: 1,
    printerName: 'local-printer',
    copies: 1,
    operatorId: operator.id || 'WEB',
    operatorName: operator.name || 'Web端',
    clientType: 'WEB',
    bizRefJson: JSON.stringify({ encounterId })
  }
  const printResult = await unwrap<Loose>(
    client.post('/charging/register/printregisterreceipt', printBody, cfg(context))
  )
  const amount = asNumber(row.cost ?? row.amount)
  const serviceTypeName = asString(row.registerType ?? row.serviceTypeName ?? row.register_type)
  return {
    printCount: asNumber(row.printCount, 1),
    slip: {
      slipNo: asString(row.receiptNo ?? row.number ?? row.encounterBusNo ?? encounterId),
      encounterId,
      patientName: asString(row.patientName ?? row.name),
      genderEnumText: c1GenderText(row.genderEnum ?? row.sex),
      ageText: asString(row.age),
      organizationName: asString(row.organizationName ?? row.organization),
      registerDepartmentName: asString(row.organizationName ?? row.organization),
      doctorName: asString(row.doctorName ?? row.doctor),
      serviceTypeName,
      feeNatureText: asString(row.feeNatureText ?? row.cost_type),
      totalAmountText: `¥${amount.toFixed(2)}`,
      chargeTimeText: asString(row.chargeTime ?? row.time),
      cashierName: operator.name || 'Web端',
      statusText: asString(row.statusText ?? row.statusEnumText ?? row.statusEnum),
      chargeItemLines: serviceTypeName
        ? [{ itemName: serviceTypeName, amountText: `¥${amount.toFixed(2)}` }]
        : []
    },
    printSuccess: printResult.printSuccess !== false,
    printerStatus: asString(printResult.printerStatus)
  }
}

// 门诊收费
export async function c1PageChargePatientQueue(query: any, context: Context = {}) {
  const pageIndex = query.pageIndex ?? 1
  const pageSize = query.pageSize ?? 20
  const data = await unwrap(
    client.get(
      '/charging/charge/get-patient-queue',
      {
        ...query,
        pageIndex,
        pageSize
      },
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  const rows = page.rows.map((row: Loose) => mapPatient(row, 'charge'))
  const expectedStatus =
    query.queueTabCode === 'PENDING' ? 'pending' : query.queueTabCode === 'SETTLED' ? 'settled' : ''
  if (!expectedStatus || rows.length) return { ...page, rows, records: rows, content: rows }

  // C1 当前页签查询可能返回空列表；读取 ALL 后按后端返回的 queueLabelCode 做前端回退筛选。
  const allData = await unwrap(
    client.get(
      '/charging/charge/get-patient-queue',
      {
        ...query,
        queueTabCode: 'ALL',
        pageIndex: 1,
        pageSize: 1000
      },
      cfg(context)
    )
  )
  const filtered = rowsOf(allData)
    .map((row: Loose) => mapPatient(row, 'charge'))
    .filter((row: Loose) => row.status === expectedStatus)
  const start = (pageIndex - 1) * pageSize
  const current = filtered.slice(start, start + pageSize)
  return {
    rows: current,
    records: current,
    content: current,
    total: filtered.length,
    totalCount: filtered.length,
    totalElements: filtered.length,
    pageIndex,
    pageSize
  }
}
export async function c1GetPendingChargeDetail(encounterId: string, context: Context = {}) {
  const data = await unwrap<Loose>(
    client.get('/charging/charge/get-charge-items', { encounterId }, cfg(context))
  )
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
      insuranceAmount: pendingItems.reduce(
        (s: number, x: Loose) => s + asNumber(x.insuranceAmount),
        0
      ),
      selfPayAmount: pendingItems.reduce((s: number, x: Loose) => s + asNumber(x.selfPayAmount), 0)
    }
  }
}
export async function c1PreSettleOutpatientCharge(payload: any, context: Context = {}) {
  const body = omitEmpty({
    encounterId: asString(payload.encounterId),
    chargeItemIds: (payload.chargeItemIds ?? []).map(asString),
    contractId: asString(payload.contractId),
    insuranceTypeCode: asString(payload.insuranceTypeCode),
    personalInsuranceNo: asString(payload.personalInsuranceNo),
    insuranceCardNo: asString(payload.insuranceCardNo)
  })
  return unwrap(client.post('/charging/charge/pre-settle-charge', body, cfg(context)))
}
export async function c1SettleOutpatientCharge(payload: any, context: Context = {}) {
  const body = omitEmpty({
    paymentDraftId: asString(payload.paymentDraftId),
    encounterId: asString(payload.encounterId),
    chargeItemIds: (payload.chargeItemIds ?? []).map(asString),
    paymentDetails: (Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []).map(
      (item: Loose) => ({
        payChannelCode: c1PaymentChannel(item.payChannelCode),
        amount: asNumber(item.amount)
      })
    ),
    billDate: c1DateTime(payload.billDate),
    outcomeEnum: asString(payload.outcomeEnum) || 'COMPLETED',
    ybChrgBchno: asString(payload.ybChrgBchno),
    ybChrgBchnos: payload.ybChrgBchnos
  })
  return unwrap(client.post('/charging/charge/confirm-settle-charge', body, cfg(context)))
}
export async function c1SwitchChargeInsuranceMode(data: any, context: Context = {}) {
  return c1GetPendingChargeDetail(data.encounterId, context)
}
export async function c1QueryPaymentResult(paymentId: string, context: Context = {}) {
  return unwrap(client.get('/charging/charge/confirm-settle-charge', { paymentId }, cfg(context)))
}
export async function c1RecoverSettlement(paymentId: string, context: Context = {}) {
  return c1QueryPaymentResult(paymentId, context)
}
export async function c1GetChargeReceipt(params: any, context: Context = {}) {
  return unwrap(client.post('/charging/charge/print-charge-receipt', params, cfg(context)))
}

// 门诊退费
export async function c1PageRefundPatientQueue(query: any, context: Context = {}) {
  const pageIndex = query.pageIndex ?? 1
  const pageSize = query.pageSize ?? 20
  const pendingOnly = query.queueTabCode === 'PENDING'
  const data = await unwrap(
    client.get(
      '/charging/refund/get-patient-list',
      {
        ...query,
        pageIndex: pendingOnly ? 1 : pageIndex,
        pageSize: pendingOnly ? 1000 : pageSize
      },
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  if (pendingOnly) {
    const usableRows = (
      await Promise.all(
        page.rows.map(async (row: Loose) => {
          const encounterId = asString(row.encounterId ?? row.encounter_id ?? row.id)
          if (!encounterId) return undefined
          try {
            const detail = await unwrap<Loose>(
              client.get('/charging/refund/get-item-list', { encounterId }, cfg(context))
            )
            const groups = Array.isArray(detail.paymentGroups) ? detail.paymentGroups : []
            const usable = groups.some((group: Loose) => {
              const lines = [
                ...(Array.isArray(group.refundingLines) ? group.refundingLines : []),
                ...(Array.isArray(group.preservedLines) ? group.preservedLines : [])
              ].filter(
                (line: Loose) =>
                  !/REFUNDED/i.test(
                    asString(line.chargeItemStatusEnum ?? line.statusEnum ?? line.status)
                  ) &&
                  asNumber(line.lineTotalAmount ?? line.amount ?? line.totalPrice) > 0
              )
              return asNumber(group.refundableAmount) > 0 && lines.length > 0
            })
            return usable ? row : undefined
          } catch {
            return undefined
          }
        })
      )
    ).filter((row): row is Loose => Boolean(row))
    const start = (pageIndex - 1) * pageSize
    const currentRows = usableRows
      .slice(start, start + pageSize)
      .map((row: Loose) => mapPatient({ ...row, queueLabelCode: 'PENDING' }, 'refund'))
    return {
      rows: currentRows,
      records: currentRows,
      content: currentRows,
      total: usableRows.length,
      totalCount: usableRows.length,
      totalElements: usableRows.length,
      pageIndex,
      pageSize
    }
  }
  const mappedRows = page.rows.map((r: Loose) => mapPatient(r, 'refund'))
  return {
    ...page,
    rows: mappedRows,
    records: mappedRows,
    content: mappedRows
  }
}
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
    channel: String(
      group.originalPayChannelCodes?.[0] ?? group.payChannelCode ?? 'cash'
    ).toLowerCase()
  }
}
function mapRefundPaymentGroup(group: Loose): Loose {
  const refundingLines = Array.isArray(group.refundingLines) ? group.refundingLines : []
  const preservedLines = Array.isArray(group.preservedLines) ? group.preservedLines : []
  const allLines = [...refundingLines, ...preservedLines]
  const refundableLines = allLines.filter(
    (line: Loose) => !/REFUNDED/i.test(asString(line.chargeItemStatusEnum ?? line.statusEnum))
  )
  const unavailableLines = allLines.filter((line: Loose) =>
    /REFUNDED/i.test(asString(line.chargeItemStatusEnum ?? line.statusEnum))
  )
  return {
    ...group,
    paymentId: asString(group.paymentRecordId ?? group.paymentId),
    paymentBusNo: asString(group.paymentNo ?? group.paymentBusNo),
    paymentStatusEnum: asString(group.paymentStatusEnum),
    halfRefundOnPayment: Boolean(group.halfRefundOnPayment),
    originalPayChannelCodes: (Array.isArray(group.originalPayChannelCodes)
      ? group.originalPayChannelCodes
      : []
    ).map((v: any) => String(v).toLowerCase()),
    refundableAmount: asNumber(group.refundableAmount),
    refundingLines: refundableLines.map((line: Loose) => mapRefundPaymentLine(line, group, true)),
    preservedLines: unavailableLines.map((line: Loose) => mapRefundPaymentLine(line, group, false))
  }
}
export async function c1GetRefundableExecutionDetail(encounterId: string, context: Context = {}) {
  const data = await unwrap<Loose>(
    client.get('/charging/refund/get-item-list', { encounterId }, cfg(context))
  )
  const patient = mapPatient(data.patient ?? {}, 'refund')
  const paymentGroups = Array.isArray(data.paymentGroups)
    ? data.paymentGroups.map(mapRefundPaymentGroup)
    : []
  const items = paymentGroups.flatMap((group: Loose) => [
    ...group.refundingLines,
    ...group.preservedLines
  ])
  return { patient, items, paymentGroups }
}
export async function c1VerifyOutpatientRefund(payload: any, context: Context = {}) {
  return unwrap(
    client.get(
      '/charging/refund/verify-refund',
      {
        encounterId: asString(payload.encounterId),
        refundingChargeItemIds: (payload.refundingChargeItemIds ?? []).map(asString).join(',')
      },
      cfg(context)
    )
  )
}
export async function c1RefundOutpatientCharge(payload: any, context: Context = {}) {
  const clientRefundResult = payload.clientRefundResult
  const body = omitEmpty({
    billDate: c1DateTime(payload.billDate),
    encounterId: asString(payload.encounterId),
    originalPaymentId: asString(payload.originalPaymentId ?? payload.paymentId),
    refundingChargeItemIds: (payload.refundingChargeItemIds ?? []).map(asString),
    refundPaymentDetails: (payload.refundPaymentDetails ?? payload.paymentDetails ?? []).map(
      (item: Loose) => ({
        amount: asNumber(item.amount),
        payChannelCode: c1PaymentChannel(item.payChannelCode)
      })
    ),
    clientRefundResult:
      typeof clientRefundResult === 'string'
        ? clientRefundResult
        : clientRefundResult
          ? JSON.stringify(clientRefundResult)
          : '',
    refundRequestId: asString(payload.refundRequestId ?? payload.refundId),
    preservedNewChargeItemIds: (payload.preservedNewChargeItemIds ?? []).map(asString)
  })
  return unwrap(client.post('/charging/refund/execute-refund', body, cfg(context)))
}
export async function c1SwitchRetainedItemsInsuranceMode(data: any, context: Context = {}) {
  return c1GetRefundableExecutionDetail(data.encounterId, context)
}
export async function c1QueryRefundResult(refundId: string, context: Context = {}) {
  return unwrap(client.get('/charging/refund/verify-refund', { refundId }, cfg(context)))
}
export async function c1RecoverRefund(refundId: string, context: Context = {}) {
  return c1QueryRefundResult(refundId, context)
}

// 住院登记
export async function c1LoadInpatientRegistrationInit(_context: Context = {}) {
  return { paymentChannelOptions: [] }
}
function mapRegistrationStatus(value: any): string {
  const status = asString(value).toUpperCase()
  if (status === 'IN_PROGRESS' || status === 'REGISTERED' || status === 'INHOSPITAL')
    return 'ADMITTED'
  if (status === 'WAITING' || status === 'PENDING') return 'WAIT_ADMISSION'
  if (status === 'CANCELED') return 'CANCELLED'
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
    organizationName: asString(
      row.organizationName ?? row.dept_name ?? row.organization ?? row.depart
    ),
    wardId: asString(row.wardId ?? row.ward_id),
    wardName: asString(row.wardName ?? row.ward_name),
    bedId: asString(row.bedId ?? row.bed_id),
    bedName: asString(row.bedName ?? row.bed_no ?? row.bed_number),
    startTime: asString(row.startTime ?? row.hospitalized_time ?? row.admit_date),
    statusEnum,
    statusEnumText:
      asString(row.statusEnumText ?? row.status_text ?? row.statusText) ||
      (statusEnum === 'CANCELLED' ? '已取消' : statusEnum === 'ADMITTED' ? '已登记' : '待登记'),
    cancelable: Boolean(row.cancelable)
  }
}

export async function c1PageInpatientRegistrationQueue(
  query: any,
  context: Context = {},
  _mockFilter: any = {}
) {
  const requestQuery = {
    ...query,
    registrationListStatuses: Array.isArray(query.registrationListStatuses)
      ? query.registrationListStatuses.join(',')
      : query.registrationListStatuses
  }
  const data = await unwrap(
    client.get(
      '/charging/enroll/query-all',
      { ...requestQuery, pageIndex: query.pageIndex ?? 1, pageSize: query.pageSize ?? 20 },
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  const content = page.rows.map(mapRegistrationQueueRow)
  return { ...page, rows: content, records: content, content }
}
export async function c1GetInpatientRegistrationContext(
  encounterId?: string,
  context: Context = {}
) {
  if (encounterId) {
    const data = await unwrap<Loose>(
      client.get('/charging/enroll/query-by-id', { encounterId }, cfg(context))
    )
    if (data && (data.patient || data.encounter)) return data
    const row = data as Loose
    return {
      contractOptions: [],
      organizationOptions: row.organizationId
        ? [
            {
              value: asString(row.organizationId),
              label: asString(row.organizationName ?? row.organizationId)
            }
          ]
        : [],
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
        primaryConditionDefinitionName: asString(
          row.conditionDefinitionName ?? row.primaryConditionDefinitionName
        ),
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
  const data = await unwrap(
    client.get('/charging/enroll/query-all', { pageIndex: 1, pageSize: 1000 }, cfg(context))
  )
  const organizations = new Map<string, { value: string; label: string }>()
  for (const row of rowsOf(data)) {
    const value = asString(row.organizationId ?? row.organization_id)
    const label = asString(row.organizationName ?? row.organization ?? row.dept_name)
    if (value) organizations.set(value, { value, label: label || value })
  }
  return {
    contractOptions: [],
    organizationOptions: [...organizations.values()],
    inWayCodeOptions: [],
    admitSourceCodeOptions: [],
    encounter: null,
    patient: null,
    readOnly: false
  }
}
export async function c1PageRegistrationPatientOptions(query: any, context: Context = {}) {
  const data = await unwrap(
    client.get(
      '/charging/enroll/query-all',
      {
        ...query,
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 20
      },
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  const rows = page.rows.map((row: Loose) => ({
    ...row,
    patientId: asString(
      row.patientId ?? row.patient_id ?? row.patient?.id ?? row.patient?.patientId
    ),
    patientBusNo: asString(
      row.patientBusNo ?? row.patient_no ?? row.patientBusNumber ?? row.number ?? row.inpatient_no
    ),
    name: asString(row.name ?? row.patientName ?? row.patient_name ?? row.patient?.name),
    genderCode: asString(row.genderCode ?? row.genderEnum ?? row.gender ?? row.sex),
    genderText: asString(
      row.genderText ?? row.genderEnumText ?? row.gender ?? row.sexText ?? row.sex
    ),
    birthDate: asString(
      row.birthDate ?? row.birth_date ?? row.patient?.birthDate ?? row.patient?.birth_date
    ),
    age: row.age ?? row.patientAge ?? row.patient?.age,
    phoneMasked: asString(row.phoneMasked ?? row.phone ?? row.phone_number ?? row.patient?.phone),
    idCardNoMasked: asString(
      row.idCardNoMasked ?? row.idCard ?? row.id_card ?? row.id_number ?? row.patient?.idCard
    )
  }))
  return { ...page, rows, records: rows, content: rows }
}
export async function c1ListRegistrationWards(departmentId: string, context: Context = {}) {
  // C1 对 query-all 携带 organizationId 会返回 HTTP 500；读取可用列表后在前端按科室筛选。
  const data = await unwrap(
    client.get('/charging/enroll/query-all', { pageIndex: 1, pageSize: 1000 }, cfg(context))
  )
  const rows = rowsOf(data).filter(
    (row: Loose) => asString(row.organizationId ?? row.organization_id) === asString(departmentId)
  )
  const unique = new Map<string, Loose>()
  for (const row of rows) {
    const wardId = asString(row.wardId ?? row.ward_id ?? row.id)
    const wardName = asString(row.wardName ?? row.ward_name ?? row.name)
    if (wardId && wardName)
      unique.set(wardId, { wardId, wardName, wardCode: asString(row.wardCode ?? row.ward_code) })
  }
  return { wards: [...unique.values()] }
}
export async function c1GetWardCapacity(wardId: string, _context: Context = {}) {
  // 当前 C1 没有稳定的病区床位容量接口；避免向 query-all 发送 wardId 触发 HTTP 500。
  return { totalBedCount: '—', vacantBedCount: '后端未提供', wardId }
}
export async function c1ListAttendingDoctors(query: any, context: Context = {}) {
  return { doctorOptions: [], primaryNurseOptions: [] }
}
export async function c1SubmitInpatientRegistration(payload: any, context: Context = {}) {
  const diagnosis = asString(
    payload.admissionDiagnosis ??
      payload.diagnosisDesc ??
      payload.admissionDiagnoses?.find((item: Loose) => item.primaryFlag)?.description ??
      payload.admissionDiagnoses?.[0]?.description
  )
  const paymentDetails = (Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []).map(
    (item: Loose) => ({
      payWay: asString(item.payWay ?? item.payChannelCode),
      amount: asString(item.amount ?? item.payAmount)
    })
  )
  const birthtime = admissionDateOnly(payload.birthtime ?? payload.birthDate)
  const body = {
    name: asString(payload.name ?? payload.patientName),
    idCard: asString(payload.idCard ?? payload.id_number),
    birthtime,
    age: asNumber(payload.age, ageFromBirthDate(birthtime)),
    sex: c1GenderText(payload.sex ?? payload.genderEnum),
    phone: asString(payload.phone),
    admissionDept: asString(payload.admissionDept ?? payload.organizationId),
    admission_ward: asString(payload.admission_ward ?? payload.wardId),
    attending_doctor: asString(payload.attending_doctor ?? payload.attendingDoctorId),
    admissionDiagnosis: diagnosis,
    diagnosisDesc: diagnosis,
    patientCondition: c1PatientConditionText(payload.patientCondition ?? payload.priorityEnum),
    admissionType: c1AdmissionTypeText(payload.admissionType ?? payload.admitSourceCode),
    admissionMode: c1AdmissionModeText(payload.admissionMode ?? payload.inWayCode),
    feeNature: c1FeeNatureText(payload.feeNature ?? payload.contractId),
    admissionDate: admissionDateOnly(payload.startTime ?? payload.admissionDate),
    prepayAmount: asString(payload.prepayAmount ?? payload.advanceDepositAmount),
    payWay: asString(payload.payWay ?? paymentDetails[0]?.payWay ?? 'CASH'),
    organizationId: asString(payload.organizationId),
    wardId: asString(payload.wardId),
    attendingDoctorId: asString(payload.attendingDoctorId),
    contractId: asString(payload.contractId),
    paymentDetails
  }
  return unwrap(client.post('/charging/admission/add', body, cfg(context)))
}
export async function c1CancelInpatientRegistration(payload: any, context: Context = {}) {
  const encounterId = asString(payload.encounterId)
  await unwrap(
    client.delete(
      '/charging/admission/delete',
      {
        encounterId,
        cancelReason: asString(payload.cancelReason),
        insuranceRevoked: false
      },
      cfg(context)
    )
  )
  return { encounterId, statusEnum: 'CANCELLED', success: true }
}

export async function c1RevokeInsuranceAdmission(payload: any, context: Context = {}) {
  const encounterId = asString(payload.encounterId)
  await unwrap(
    client.delete(
      '/charging/admission/delete',
      {
        encounterId,
        cancelReason: asString(payload.cancelReason),
        insuranceRevoked: true
      },
      cfg(context)
    )
  )
  return { encounterStatus: 'CANCELLED', insurancePlatformRevoked: true }
}

// 预交金管理
export async function c1PageInpatientQueue(query: any, context: Context = {}) {
  const data = await unwrap(
    client.get(
      '/charging/pay_manage/getpatientqueue',
      omitEmpty({
        name: query.keyword,
        hospitalized_time: query.admissionDate,
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 20
      }),
      cfg(context)
    )
  )
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
  const type = /REFUND|退/.test(`${row.service_type ?? row.serviceType ?? ''}${statusEnum}`)
    ? 'refund'
    : 'collect'
  const channel = mapPrepayChannel(row.channel ?? row.pay_type ?? row.payWay)
  // C1 的预交金已经计入账户余额后，流水接口仍可能返回 PENDING/处理中，
  // 同时后端没有提供按 paymentId 查询异步结果的接口。金额大于 0 的收款流水
  // 因此按已结算展示，避免前端调用错误的交易结果查询并触发 9994/HTTP 500。
  const c1RecordedCollectionSettled =
    type === 'collect' && amount > 0 && /PENDING|PROCESSING|处理中/.test(statusEnum)
  const status =
    /SUCCESS|SETTLED|COMPLETED/.test(statusEnum) || c1RecordedCollectionSettled
      ? 'settled'
      : /REFUND/.test(statusEnum)
        ? 'refunded'
        : /CANCEL/.test(statusEnum)
          ? 'cancelled'
          : 'processing'
  const transactionNo = asString(
    row.transactionNo ??
      row.transaction_no ??
      row.paymentNo ??
      row.payment_id ??
      row.id ??
      `${inpatientNo}-${row.pay_time ?? ''}`
  )
  return {
    ...row,
    id: asString(row.paymentId ?? row.payment_id ?? row.id ?? transactionNo),
    paymentId: asString(row.paymentId ?? row.payment_id ?? row.id),
    encounterId: inpatientNo,
    type,
    amount,
    channel,
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
  const data = await unwrap<Loose>(
    client.get(
      '/charging/pay_manage/getpatientdetails',
      { hospitalized_number: encounterId },
      cfg(context)
    )
  )
  const patient = mapPatient(
    {
      ...data,
      hospitalizedNumber: data.hospitalized_number,
      patientName: data.name,
      depart: data.depart,
      wardName: data.ward,
      bed_number: data.bed_number,
      encounterId
    },
    'inpatient'
  )
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
  const params = {
    hospitalized_number: encounterId,
    pageIndex: query.pageIndex ?? 1,
    pageSize: query.pageSize ?? 20
  }
  const data = await unwrap<Loose>(
    client.get('/charging/pay_manage/getpatientpayrecords', params, cfg(context))
  )
  const page = pageOf(data, query)
  const mappedRows = page.rows.map((row: Loose) => mapPrepayRecord(row, encounterId))
  // C1 偶尔会返回金额为 0、状态为 PENDING 的占位流水。它没有发生真实收退款，
  // 也没有可供前端查询结果的支付标识，不能将其当成“扫码交易处理中”。
  const rows = mappedRows.filter((row: Loose) => row.amount > 0)
  const total = Math.max(0, page.total - (mappedRows.length - rows.length))
  return {
    ...page,
    total,
    totalCount: total,
    totalElements: total,
    rows,
    records: rows,
    content: rows
  }
}

export async function c1CollectInpatientPrepay(payload: any, context: Context = {}) {
  const operator = currentOperator()
  const operatorId = asString(payload.operatorId).trim() || operator.id
  if (!operatorId) throw new Error('当前登录信息缺少操作员标识，无法办理预交金收款')
  const inpatientNo = asString(
    payload.inpatientNo ?? payload.encounterBusNo ?? payload.encounterId
  ).trim()
  if (!inpatientNo) throw new Error('当前患者缺少住院号，无法办理预交金收款')
  const body = omitEmpty({
    inpatientNo,
    paymentDetails: (Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []).map(
      (item: Loose) => ({
        payChannelCode:
          ({ card: 'BANK_CARD', wechat: 'WECHAT', alipay: 'ALIPAY', cash: 'CASH' } as Loose)[
            asString(item.payChannelCode).toLowerCase()
          ] ?? asString(item.payChannelCode).toUpperCase(),
        amount: asNumber(item.amount)
      })
    ),
    paymentDraftId: asString(payload.paymentDraftId),
    operatorId,
    operatorName: asString(payload.operatorName).trim() || operator.name,
    remark: asString(payload.remark)
  })
  try {
    return await unwrap(client.post('/pay_manage/charge/add-prepayment', body, cfg(context)))
  } catch (error) {
    const status = (error as Loose)?.response?.status
    if (status === 500) {
      throw new Error('预交金收款失败：C1 后端收款接口返回 500，请刷新流水确认是否已经入账')
    }
    if (error instanceof Error && /operation failure|接口请求失败/i.test(error.message)) {
      throw new Error('预交金收款失败：后端未找到该住院号对应的收费账户')
    }
    throw error
  }
}
export async function c1RefundInpatientPrepay(payload: any, context: Context = {}) {
  const operator = currentOperator()
  const operatorId = asString(payload.operatorId).trim() || operator.id
  if (!operatorId) throw new Error('当前登录信息缺少操作员标识，无法办理预交金退款')
  const inpatientNo = asString(
    payload.inpatientNo ?? payload.encounterBusNo ?? payload.encounterId
  ).trim()
  if (!inpatientNo) throw new Error('当前患者缺少住院号，无法办理预交金退款')
  const body = omitEmpty({
    inpatientNo,
    paymentDetails: (Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []).map(
      (item: Loose) => ({
        payChannelCode:
          ({ card: 'BANK_CARD', wechat: 'WECHAT', alipay: 'ALIPAY', cash: 'CASH' } as Loose)[
            asString(item.payChannelCode).toLowerCase()
          ] ?? asString(item.payChannelCode).toUpperCase(),
        amount: asNumber(item.amount)
      })
    ),
    paymentDraftId: asString(payload.paymentDraftId),
    operatorId,
    operatorName: asString(payload.operatorName).trim() || operator.name,
    reason: asString(payload.reason ?? payload.remark),
    remark: asString(payload.remark)
  })
  try {
    const result = await unwrap<Loose>(
      client.post('/pay_manage/charge/refund-prepayment', body, cfg(context))
    )
    return {
      ...result,
      accountBalance: asNumber(result.accountBalance ?? result.balance),
      paymentId: asString(result.paymentId)
    }
  } catch (error) {
    const status = (error as Loose)?.response?.status
    if (status === 500) {
      throw new Error(
        '预交金退款失败：C1 后端退款接口返回 500，请后端核对该住院号的预交金账户和可退余额'
      )
    }
    if (error instanceof Error && /operation failure|接口请求失败/i.test(error.message)) {
      throw new Error('预交金退款失败：后端未找到该住院号对应的收费账户或可退余额')
    }
    throw error
  }
}
export async function c1QueryPrepayPaymentResult(paymentId: string, context: Context = {}) {
  return unwrap(
    client.get('/charging/pay_manage/getpatientpayrecords', { paymentId }, cfg(context))
  )
}
export async function c1RecoverPrepaySettlement(paymentId: string, context: Context = {}) {
  return c1QueryPrepayPaymentResult(paymentId, context)
}
export async function c1CancelPrepayPayment(paymentId: string, context: Context = {}) {
  return c1QueryPrepayPaymentResult(paymentId, context)
}

// 住院结算
export async function c1LoadInpatientSettleInit(_context: Context = {}) {
  return {
    defaultPresentationMode: 'DETAIL',
    defaultSettleQueueTab: 'PENDING_SETTLE',
    paymentChannelOptions: [
      { value: 'CASH', label: '现金' },
      { value: 'BANK_CARD', label: '银行卡' },
      { value: 'WECHAT', label: '微信' },
      { value: 'ALIPAY', label: '支付宝' },
      { value: 'ACCOUNT', label: '账户余额' }
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
}
export async function c1PageInpatientSettleQueue(query: any, context: Context = {}) {
  const data = await unwrap(
    client.get(
      '/charging/settle/getpatientlist',
      omitEmpty({
        patient_name: query.keyword,
        pageIndex: query.pageIndex ?? 1,
        pageSize: query.pageSize ?? 20
      }),
      cfg(context)
    )
  )
  const page = pageOf(data, query)
  let registrationRows: Loose[] = []
  try {
    const registrationData = await unwrap(
      client.get(
        '/charging/enroll/query-all',
        {
          pageIndex: 1,
          pageSize: 1000,
          registrationListStatuses: 'PLANNED,REGISTERED,IN_PROGRESS,INHOSPITAL'
        },
        cfg(context)
      )
    )
    registrationRows = rowsOf(registrationData)
  } catch {
    // 登记资料仅用于补齐结算队列缺失的患者主键，不阻断患者列表。
  }
  const registrationByEncounter = new Map(
    registrationRows.map((row: Loose) => [asString(row.encounterId ?? row.encounter_id), row])
  )
  const registrationByNo = new Map(
    registrationRows.map((row: Loose) => [
      asString(row.encounterBusNo ?? row.inpatient_no ?? row.bus_no ?? row.number),
      row
    ])
  )
  const rows = page.rows.map((row: Loose) => {
    const encounterBusNo = asString(
      row.encounterBusNo ?? row.inpatient_no ?? row.bus_no ?? row.number
    )
    const registered =
      registrationByEncounter.get(asString(row.encounterId ?? row.encounter_id)) ??
      registrationByNo.get(encounterBusNo) ??
      {}
    return mapPatient(
      {
        ...row,
        accountId: row.accountId ?? row.account_id ?? registered.accountId ?? registered.account_id,
        contractId:
          row.contractId ?? row.contract_id ?? registered.contractId ?? registered.contract_id,
        contractNo:
          row.contractNo ?? row.contract_no ?? registered.contractNo ?? registered.contract_no,
        encounterBusNo,
        encounterId:
          registered.encounterId ??
          registered.encounter_id ??
          row.encounterId ??
          row.encounter_id ??
          row.id,
        patientId: row.patientId ?? row.patient_id ?? registered.patientId ?? registered.patient_id,
        patientName:
          row.patientName ??
          row.patient_name ??
          row.name ??
          registered.patientName ??
          registered.patient_name,
        organizationName:
          row.organizationName ??
          row.organization ??
          row.org_name ??
          registered.organizationName ??
          registered.organization ??
          registered.org_name ??
          row.dept_name,
        wardName: row.wardName ?? registered.wardName ?? row.ward_name,
        bedName: row.bedName ?? registered.bedName ?? row.bed_no,
        startTime: row.startTime ?? registered.startTime ?? row.admit_date,
        statusEnum: normalizeSettleQueueStatus(row.statusEnum ?? row.status),
        statusEnumText: row.statusEnumText ?? '待结算',
        gender: row.genderText ?? row.gender
      },
      'inpatient'
    )
  })
  return { ...page, rows, records: rows, content: rows }
}
export async function c1GetInpatientSettleSummary(
  encounterId: string,
  patientId: string,
  selectedChargeItemIds: string[] = [],
  context: Context = {}
) {
  const data =
    (await unwrap<Loose>(
      client.get('/charging/settle/getTotalFees', { encounter_id: encounterId }, cfg(context))
    )) ?? {}
  const rows = rowsOf(data)
  const rowTotal = rows.reduce(
    (sum: number, row: Loose) =>
      sum + asNumber(row.amount ?? row.totalAmount ?? row.total_price ?? row.total_fee),
    0
  )
  const pendingSettlementTotal =
    asNumber(
      data.unsettledAmount ??
        data.pendingSettlementTotal ??
        data.totalAmount ??
        data.total_amount ??
        data.totalFee ??
        data.total_fee ??
        data.personal_pay_fee ??
        data.total_hospital_fee ??
        data.amount,
      rowTotal
    ) || rowTotal
  const prepayNetBalance = asNumber(data.account_balance ?? data.prepay_total_fee)
  return {
    encounterId,
    patientId,
    pendingSettlementTotal,
    selectedTotal: selectedChargeItemIds.length ? pendingSettlementTotal : 0,
    prepayNetBalance,
    displayAccountBalance: prepayNetBalance,
    balanceHint: asNumber(data.final_balance_fee, prepayNetBalance - pendingSettlementTotal),
    statusEnum: 'PENDING_SETTLE',
    statusEnumText: '待结算'
  }
}
export async function c1ListInpatientSettleCharges(
  encounterId: string,
  patientId: string,
  presentationMode = 'DETAIL',
  context: Context = {}
) {
  const data = await unwrap<Loose>(
    client.get(
      '/charging/settle/getCostDetail',
      { encounterId, patientId, presentationMode },
      cfg(context)
    )
  )
  const rawItems = Array.isArray(data.items) ? data.items : rowsOf(data)
  const lines = rawItems.map((row: Loose) => ({
    ...row,
    categoryCode: asString(row.categoryCode ?? row.category_code ?? row.chargeCategoryCode),
    categoryDisplay: asString(
      row.categoryDisplay ?? row.category_display ?? row.category ?? row.chargeCategoryName
    ),
    chargeItemId: asString(row.chargeItemId ?? row.charge_item_id ?? row.id),
    itemName: asString(row.itemName ?? row.item_name ?? row.name),
    lineAmount: asNumber(row.lineAmount ?? row.line_amount ?? row.amount ?? row.totalPrice),
    occurrenceTime: asString(row.occurrenceTime ?? row.occurrence_time),
    quantityUnit: asString(row.quantityUnit ?? row.quantity_unit ?? row.unit),
    quantityValue: row.quantityValue ?? row.quantity_value ?? row.quantity,
    selected: row.selected !== false,
    sourceText: asString(row.sourceText ?? row.source_text),
    specText: asString(row.specText ?? row.spec_text ?? row.specification),
    statusEnum: normalizeSettleLineStatus(row.statusEnum ?? row.status_enum ?? row.status),
    statusEnumText: asString(row.statusEnumText ?? row.status_enum_text ?? row.statusText),
    unitPrice: row.unitPrice ?? row.unit_price
  }))
  const categoryMap = new Map<string, Loose>()
  for (const line of lines) {
    const key = line.categoryCode || line.categoryDisplay || 'OTHER'
    const category = categoryMap.get(key) ?? {
      categoryCode: line.categoryCode || key,
      categoryDisplay: line.categoryDisplay || '其他',
      amount: 0,
      lineCount: 0
    }
    category.amount = asNumber(category.amount) + asNumber(line.lineAmount)
    category.lineCount += 1
    categoryMap.set(key, category)
  }
  return {
    ...data,
    encounterId,
    presentationMode,
    defaultSelectAll: true,
    pendingSettlementTotal:
      asNumber(data.unsettledAmount ?? data.pendingSettlementTotal ?? data.totalAmount) ||
      lines.reduce((sum: number, line: Loose) => sum + asNumber(line.lineAmount), 0),
    categories: Array.isArray(data.categories) ? data.categories : [...categoryMap.values()],
    lines
  }
}
export async function c1ListInpatientSettleChargeCategories(query: any, context: Context = {}) {
  return unwrap(client.get('/charging/settle/getCategoryDetails', query, cfg(context)))
}
export async function c1ListInpatientSettleChargeDetails(query: any, context: Context = {}) {
  return unwrap(client.get('/charging/settle/getCategoryDetails', query, cfg(context)))
}
export async function c1PreSettleInpatientCharges(payload: any, context: Context = {}) {
  if (!payload.chargeItemIds?.length) throw new Error('请至少选择一项待结算费用')
  const data =
    (await unwrap<Loose>(
      client.get(
        '/charging/settle/getCostDetail',
        {
          encounterId: payload.encounterId,
          patientId: payload.patientId,
          presentationMode: 'DETAIL'
        },
        cfg(context)
      )
    )) ?? {}
  const selectedIds = new Set(payload.chargeItemIds.map(asString))
  const selected = rowsOf(data).filter((row: Loose) =>
    selectedIds.has(asString(row.chargeItemId ?? row.charge_item_id ?? row.id))
  )
  if (selected.length !== selectedIds.size) throw new Error('所选费用已变化，请刷新后重试')
  const receivableTotal = Number(
    selected
      .reduce(
        (sum: number, row: Loose) =>
          sum +
          asNumber(
            row.totalPrice ?? row.total_price ?? row.lineAmount ?? row.line_amount ?? row.amount
          ),
        0
      )
      .toFixed(2)
  )
  const first = selected[0] ?? {}
  return {
    accountId: asString(
      payload.accountId ?? data.accountId ?? data.account_id ?? first.accountId ?? first.account_id
    ),
    contractNo: asString(
      payload.contractNo ??
        data.contractNo ??
        data.contract_no ??
        first.contractNo ??
        first.contract_no
    ),
    displayAmount: receivableTotal,
    insurancePreSettled: false,
    paymentDraftId: `C1-LOCAL-${Date.now()}`,
    previewText: `${payload.settleMode === 'DISCHARGE' ? '出院结算' : '中途结算'}，应收 ¥${receivableTotal.toFixed(2)}`,
    receivableTotal,
    settlementComposition: 'SELF_PAY',
    settleMode: payload.settleMode ?? 'INTERIM',
    ybAcctPay: 0,
    ybFundPaySumamt: 0,
    ybMedfeeSumamt: 0,
    ybPsnCashPay: receivableTotal
  }
}

export async function c1ConfirmInpatientSettle(payload: any, context: Context = {}) {
  const accountId = asString(payload.accountId)
  if (!accountId) throw new Error('后端未返回收费账户标识 accountId，当前患者无法执行住院结算')
  const paymentDetails = (Array.isArray(payload.paymentDetails) ? payload.paymentDetails : []).map(
    (item: Loose) => ({
      payEnum: asString(item.payEnum ?? item.payChannelCode),
      amount: asNumber(item.amount),
      ...(item.payTransNo ? { payTransNo: asString(item.payTransNo) } : {})
    })
  )
  const displayAmount = asNumber(
    payload.displayAmount,
    paymentDetails.reduce((sum: number, item: Loose) => sum + asNumber(item.amount), 0)
  )
  const body = omitEmpty({
    patientId: asString(payload.patientId),
    encounterId: asString(payload.encounterId),
    accountId,
    chargeItemIds: (payload.chargeItemIds ?? []).map(asString),
    paymentDetails,
    displayAmount,
    billDate:
      asString(payload.billDate) ||
      new Date().toLocaleString('sv-SE').replace('T', ' ').slice(0, 19)
  })
  return unwrap(client.post('/charging/settle/settlement', body, cfg(context)))
}
export async function c1ListInpatientSettlementHistory(
  encounterId: string,
  patientId: string,
  context: Context = {}
) {
  const data = await unwrap<Loose>(
    client.get(
      '/charging/settle/getSettleHistory',
      { encounterId, patientId, pageIndex: 1, pageSize: 20 },
      cfg(context)
    )
  )
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
export async function c1GetSettledInpatientPayment(
  encounterId: string,
  patientId: string,
  settlementId: string,
  context: Context = {}
) {
  return unwrap(
    client.get(
      '/charging/settle/getSettleHistory',
      { encounterId, patientId, paymentId: settlementId },
      cfg(context)
    )
  )
}
export async function c1ReverseInpatientSettle(
  encounterId: string,
  patientId: string,
  settlementId: string,
  reason: string,
  context: Context = {}
) {
  return unwrap(
    client.post(
      '/charging/settle/settlement',
      { encounterId, patientId, paymentId: settlementId, reason },
      cfg(context)
    )
  )
}
