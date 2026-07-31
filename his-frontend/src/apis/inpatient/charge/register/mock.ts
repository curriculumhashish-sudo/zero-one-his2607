import type {
  AttendingDoctorQuery,
  AttendingDoctorResult,
  CancelRegistrationPayload,
  CancelRegistrationResult,
  RegistrationContext,
  RegistrationPageInitResult,
  RegistrationOption,
  RegistrationPageResult,
  RegistrationPatientOption,
  RegistrationPatientPage,
  RegistrationPatientQuery,
  RegistrationQueueQuery,
  RegistrationQueueMockFilter,
  RegistrationQueueRecord,
  RegistrationContextQuery,
  RegistrationWardQuery,
  RequestContext,
  RevokeInsuranceAdmissionResult,
  SubmitRegistrationPayload,
  SubmitRegistrationResult,
  WardCapacity,
  WardCapacityQuery,
  WardOption
} from './type'

const organizations: RegistrationOption[] = [
  { value: '1001001', label: '心血管内科' },
  { value: '1001002', label: '呼吸内科' },
  { value: '1001003', label: '神经内科' }
]

const contractCategoryById: Record<string, string> = {
  'CONTRACT-SELF': '1',
  'CONTRACT-MI': '3'
}

const wards: Record<string, WardOption[]> = {
  '1001001': [
    { wardId: '2001001', wardCode: 'CARD-A', wardName: '心内一病区' },
    { wardId: '2001002', wardCode: 'CARD-B', wardName: '心内监护病区' }
  ],
  '1001002': [{ wardId: '2002001', wardCode: 'RESP-A', wardName: '呼吸一病区' }],
  '1001003': [{ wardId: '2003001', wardCode: 'NEUR-A', wardName: '神经一病区' }]
}

type MockDoctorOption = RegistrationOption & { organizationId: string; wardId: string }

const doctors: MockDoctorOption[] = [
  { value: '3001001', label: '赵主任', organizationId: '1001001', wardId: '2001001' },
  { value: '3001002', label: '钱医生', organizationId: '1001001', wardId: '2001002' },
  { value: '3002001', label: '孙医生', organizationId: '1001002', wardId: '2002001' },
  { value: '3003001', label: '李医生', organizationId: '1001003', wardId: '2003001' }
]

const patients: RegistrationPatientOption[] = [
  {
    patientId: '900000000000000001',
    patientBusNo: 'MR202607001',
    name: '周明',
    genderCode: 'MALE',
    genderText: '男',
    birthDate: '1978-04-12',
    age: 48,
    idCardNoMasked: '110101197804120011',
    phoneMasked: '13800000011'
  },
  {
    patientId: '900000000000000002',
    patientBusNo: 'MR202607002',
    name: '吴芳',
    genderCode: 'FEMALE',
    genderText: '女',
    birthDate: '1986-09-20',
    age: 39,
    idCardNoMasked: '110101198609200022',
    phoneMasked: '13800000022'
  },
  {
    patientId: '900000000000000003',
    patientBusNo: 'MR202607003',
    name: '郑华',
    genderCode: 'MALE',
    genderText: '男',
    birthDate: '1959-01-05',
    age: 67,
    idCardNoMasked: '110101195901050033',
    phoneMasked: '13800000033'
  }
]

type MockRegistrationQueueRecord = RegistrationQueueRecord & { idCard?: string }

const records: MockRegistrationQueueRecord[] = [
  {
    ambEncounterId: '810000000000000001',
    cancelable: true,
    encounterBusNo: 'IP-APPLY-26072801',
    encounterId: '800000000000000001',
    idCard: patients[0]!.idCardNoMasked,
    insurancePlatformRegistered: false,
    inWayCode: '1',
    inWayCodeText: '门诊转入',
    organizationId: '1001001',
    organizationName: '心血管内科',
    patientId: patients[0]!.patientId!,
    patientName: patients[0]!.name!,
    startTime: '2026-07-28 09:10:00',
    statusEnum: 'WAIT_ADMISSION',
    statusEnumText: '待登记'
  },
  {
    ambEncounterId: '810000000000000002',
    bedId: 'BED-1208',
    bedName: '12床',
    cancelable: true,
    encounterBusNo: 'IP20260728002',
    encounterId: '800000000000000002',
    idCard: patients[1]!.idCardNoMasked,
    insurancePlatformRegistered: true,
    inWayCode: '2',
    inWayCodeText: '急诊入院',
    organizationId: '1001002',
    organizationName: '呼吸内科',
    patientId: patients[1]!.patientId!,
    patientName: patients[1]!.name!,
    startTime: '2026-07-27 16:20:00',
    statusEnum: 'ADMITTED',
    statusEnumText: '已登记',
    wardId: '2002001',
    wardName: '呼吸一病区'
  },
  {
    ambEncounterId: '810000000000000003',
    cancelable: false,
    encounterBusNo: 'IP-APPLY-26072603',
    encounterId: '800000000000000003',
    idCard: patients[2]!.idCardNoMasked,
    insurancePlatformRegistered: false,
    inWayCode: '5',
    inWayCodeText: '其他',
    organizationId: '1001003',
    organizationName: '神经内科',
    patientId: patients[2]!.patientId!,
    patientName: patients[2]!.name!,
    startTime: '2026-07-26 11:00:00',
    statusEnum: 'CANCELLED',
    statusEnumText: '已取消'
  }
]

function clone<T>(value: T): T {
  return structuredClone(value)
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

function statusText(status: RegistrationQueueRecord['statusEnum']) {
  return status === 'WAIT_ADMISSION' ? '待登记' : status === 'ADMITTED' ? '已登记' : '已取消'
}

export async function mockLoadInpatientRegistrationInit(context: RequestContext = {}) {
  await wait(context)
  const result: RegistrationPageInitResult = {
    paymentChannelOptions: [
      { value: 'CASH', label: '现金' },
      { value: 'BANK_CARD', label: '银行卡' },
      { value: 'WECHAT', label: '微信' },
      { value: 'ALIPAY', label: '支付宝' }
    ]
  }
  return clone(result)
}

function registrationContextOptions() {
  return {
    contractOptions: [
      { value: 'CONTRACT-SELF', label: '自费' },
      { value: 'CONTRACT-MI', label: '职工医保' }
    ],
    organizationOptions: organizations,
    inWayCodeOptions: [
      { value: '1', label: '门诊转住院' },
      { value: '2', label: '急诊入院' },
      { value: '3', label: '转入入院' },
      { value: '4', label: '其它医疗机构转入' },
      { value: '5', label: '其它' }
    ],
    admitSourceCodeOptions: [
      { value: '1', label: '事故' },
      { value: '2', label: '随意选择的' },
      { value: '3', label: '紧急' },
      { value: '4', label: '分娩' },
      { value: '5', label: '新生儿' },
      { value: '6', label: '常规' },
      { value: '7', label: '危重' }
    ]
  }
}

export async function mockPageInpatientRegistrationQueue(
  query: RegistrationQueueQuery,
  context: RequestContext = {},
  mockFilter: RegistrationQueueMockFilter = {}
) {
  await wait(context)
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const statuses = query.registrationListStatuses?.length
    ? query.registrationListStatuses
    : query.registrationListStatus
      ? [query.registrationListStatus]
      : []
  const filtered = records.filter((record) => {
    if (
      keyword &&
      !`${record.patientName}${record.encounterBusNo}${record.idCard ?? ''}`
        .toLowerCase()
        .includes(keyword)
    )
      return false
    if (query.idCard && !record.idCard?.includes(query.idCard.trim())) return false
    if (statuses.length && !statuses.includes(record.statusEnum)) return false
    const date = record.startTime?.slice(0, 10) ?? ''
    if (mockFilter.dateFrom && date < mockFilter.dateFrom) return false
    if (mockFilter.dateTo && date > mockFilter.dateTo) return false
    return true
  })
  const start = (query.pageNo - 1) * query.pageSize
  const result: RegistrationPageResult = {
    content: clone(
      filtered.slice(start, start + query.pageSize).map(({ idCard: _idCard, ...record }) => record)
    ),
    pageNo: query.pageNo,
    pageSize: query.pageSize,
    totalCount: filtered.length,
    totalElements: filtered.length
  }
  return result
}

export async function mockGetInpatientRegistrationContext(
  query: RegistrationContextQuery = {},
  context: RequestContext = {}
) {
  await wait(context)
  const { encounterId } = query
  const record = records.find((item) => item.encounterId === encounterId)
  const patient = record ? patients.find((item) => item.patientId === record.patientId) : null
  const options = registrationContextOptions()
  const result: RegistrationContext = {
    admitSourceCodeOptions: options.admitSourceCodeOptions,
    contractOptions: options.contractOptions,
    organizationOptions: organizations,
    inWayCodeOptions: options.inWayCodeOptions,
    encounter: record
      ? {
          ambEncounterId: record.ambEncounterId,
          encounterBusNo: record.encounterBusNo,
          encounterId: record.encounterId,
          inWayCode: record.inWayCode,
          inWayCodeText: record.inWayCodeText,
          organizationId: record.organizationId,
          organizationName: record.organizationName,
          outpatientDoctorName: '王医生',
          primaryConditionDefinitionId: 'COND-001',
          primaryConditionDefinitionName: '冠状动脉粥样硬化性心脏病',
          priorityEnum: 'GENERAL',
          startTime: record.startTime,
          statusEnum: record.statusEnum,
          wardId: record.wardId,
          wardName: record.wardName,
          bedId: record.bedId,
          bedName: record.bedName,
          attendingDoctorId: doctors.find((item) => item.wardId === record.wardId)?.value,
          attendingDoctorName: doctors.find((item) => item.wardId === record.wardId)?.label
        }
      : null,
    patient: patient
      ? {
          birthDate: patient.birthDate,
          genderEnum: patient.genderCode,
          idCard: patient.idCardNoMasked,
          name: patient.name,
          patientId: patient.patientId,
          phone: patient.phoneMasked
        }
      : null,
    readOnly: Boolean(record && record.statusEnum !== 'WAIT_ADMISSION')
  }
  return clone(result)
}

export async function mockPageRegistrationPatientOptions(
  query: RegistrationPatientQuery,
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
  const result: RegistrationPatientPage = {
    records: clone(filtered.slice(start, start + query.pageSize)),
    total: filtered.length
  }
  return result
}

export async function mockListRegistrationWards(
  query: RegistrationWardQuery,
  context: RequestContext = {}
) {
  await wait(context, 120)
  return { wards: clone(wards[query.organizationId] ?? []) }
}

export async function mockGetWardCapacity(query: WardCapacityQuery, context: RequestContext = {}) {
  await wait(context, 120)
  const { wardId } = query
  const result: WardCapacity = {
    wardId,
    totalBedCount: wardId === '2001002' ? 12 : 36,
    vacantBedCount: wardId === '2001002' ? 2 : 8
  }
  return result
}

export async function mockListAttendingDoctors(
  query: AttendingDoctorQuery,
  context: RequestContext = {}
) {
  await wait(context, 120)
  const result: AttendingDoctorResult = {
    doctorOptions: clone(
      doctors
        .filter(
          (doctor) =>
            (!query.organizationId || doctor.organizationId === query.organizationId) &&
            (!query.wardLocationId || doctor.wardId === query.wardLocationId)
        )
        .map(({ disabled, label, value }) => ({ disabled, label, value }))
    ),
    primaryNurseOptions: []
  }
  return result
}

export async function mockSubmitInpatientRegistration(payload: SubmitRegistrationPayload) {
  await wait()
  if (!payload.patientName || !payload.organizationId || !payload.wardId || !payload.startTime) {
    throw new Error('患者、入院科室、病区和入院时间不能为空')
  }
  const insurance = ['3', '4'].includes(contractCategoryById[payload.contractId] ?? '')
  if (insurance && !payload.psnNo) {
    throw new Error('医保登记缺少人员编号，请先读取医保待遇信息')
  }
  if (insurance && !payload.attendingDoctorId) {
    throw new Error('医保登记业务校验未通过：请选择主治医生')
  }
  const encounterId = payload.encounterId ?? `8000000000${Date.now()}`
  const patientId = payload.patientId ?? `9000000000${Date.now()}`
  let patient = patients.find((item) => item.patientId === patientId)
  if (!patient) {
    patient = {
      patientId,
      patientBusNo: `MR${Date.now()}`,
      name: payload.patientName,
      genderCode: payload.genderEnum,
      genderText: payload.genderEnum === 'FEMALE' ? '女' : '男',
      birthDate: payload.birthDate,
      idCardNoMasked: payload.idCard,
      phoneMasked: payload.phone
    }
    patients.push(patient)
  }
  const organization = organizations.find((item) => item.value === payload.organizationId)
  const ward = (wards[payload.organizationId] ?? []).find((item) => item.wardId === payload.wardId)
  let record = records.find((item) => item.encounterId === encounterId)
  if (!record) {
    record = {
      cancelable: true,
      encounterBusNo: `IP${new Date().toISOString().slice(0, 10).replaceAll('-', '')}${records.length + 1}`,
      encounterId,
      idCard: payload.idCard,
      insurancePlatformRegistered: insurance,
      organizationId: payload.organizationId,
      organizationName: organization?.label,
      patientId,
      patientName: payload.patientName,
      startTime: payload.startTime,
      statusEnum: 'ADMITTED',
      statusEnumText: '已登记',
      wardId: payload.wardId,
      wardName: ward?.wardName
    }
    records.unshift(record)
  } else {
    Object.assign(record, {
      cancelable: true,
      insurancePlatformRegistered: insurance,
      organizationId: payload.organizationId,
      organizationName: organization?.label,
      patientId,
      patientName: payload.patientName,
      startTime: payload.startTime,
      statusEnum: 'ADMITTED' as const,
      statusEnumText: '已登记',
      wardId: payload.wardId,
      wardName: ward?.wardName
    })
  }
  const result: SubmitRegistrationResult = {
    accountOpened: true,
    encounterBusNo: record.encounterBusNo,
    encounterId,
    paymentId: payload.advanceDepositAmount ? `PAY-${Date.now()}` : undefined,
    statusEnum: 'ADMITTED',
    ybMdtrtId: insurance ? `YB-${Date.now()}` : undefined
  }
  return result
}

export async function mockCancelInpatientRegistration(
  payload: CancelRegistrationPayload,
  context: RequestContext = {}
) {
  await wait(context)
  const { encounterId } = payload
  const record = records.find((item) => item.encounterId === encounterId)
  if (!record) throw new Error('登记记录不存在')
  if (!record.cancelable) throw new Error('当前状态不允许取消登记')
  record.statusEnum = 'CANCELLED'
  record.statusEnumText = statusText(record.statusEnum)
  record.cancelable = false
  const result: CancelRegistrationResult = { encounterId, statusEnum: 'CANCELLED', success: true }
  return result
}

export async function mockRevokeInsuranceAdmission(
  payload: CancelRegistrationPayload,
  context: RequestContext = {}
) {
  await wait(context)
  const { encounterId } = payload
  const record = records.find((item) => item.encounterId === encounterId)
  if (!record) throw new Error('登记记录不存在')
  if (!record.insurancePlatformRegistered) throw new Error('当前记录未办理医保平台入院')
  record.insurancePlatformRegistered = false
  record.statusEnum = 'CANCELLED'
  record.statusEnumText = '已取消'
  record.cancelable = false
  const result: RevokeInsuranceAdmissionResult = {
    encounterStatus: 'CANCELLED',
    insurancePlatformRevoked: true
  }
  return result
}

export function getInpatientRegistrationSnapshot() {
  return clone(records)
}
