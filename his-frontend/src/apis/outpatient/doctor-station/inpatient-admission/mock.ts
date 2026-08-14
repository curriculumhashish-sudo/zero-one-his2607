import type {
  CheckInpatientAdmissionVO,
  InpatientAdmissionDepartmentVO,
  InpatientAdmissionFormOptionsVO,
  InpatientAdmissionOptionVO,
  InpatientAdmissionWardVO,
  RequestInpatientAdmissionDTO,
  RequestInpatientAdmissionVO
} from './type'

const departmentOptions: InpatientAdmissionDepartmentVO[] = [
  { value: 'inpatient-dept-cardio', label: '心血管内科', code: 'I-CARDIO' },
  { value: 'inpatient-dept-neuro', label: '神经内科', code: 'I-NEURO' },
  { value: 'inpatient-dept-respiratory', label: '呼吸与危重症医学科', code: 'I-RESP' }
]

const wardOptions: InpatientAdmissionWardVO[] = [
  { value: 'ward-cardio-1', label: '心内一病区', departmentId: 'inpatient-dept-cardio' },
  { value: 'ward-cardio-2', label: '心内二病区', departmentId: 'inpatient-dept-cardio' },
  { value: 'ward-neuro-1', label: '神经内科病区', departmentId: 'inpatient-dept-neuro' },
  {
    value: 'ward-respiratory-1',
    label: '呼吸与危重症病区',
    departmentId: 'inpatient-dept-respiratory'
  }
]

const admissionTypeOptions: InpatientAdmissionOptionVO[] = [
  { value: 'ELECTIVE', label: '择期住院' },
  { value: 'EMERGENCY', label: '急诊住院' }
]

const admissionWayOptions: InpatientAdmissionOptionVO[] = [
  { value: 'OUTPATIENT_TRANSFER', label: '门诊转住院' },
  { value: 'EMERGENCY_TRANSFER', label: '急诊转住院' },
  { value: 'TRANSFER_IN', label: '外院转入' }
]

const priorityOptions: InpatientAdmissionOptionVO[] = [
  { value: 'NORMAL', label: '普通' },
  { value: 'URGENT', label: '紧急' },
  { value: 'CRITICAL', label: '危重' }
]

/** 模拟服务端保存的住院申请；同一门诊就诊只能存在一条未取消记录。 */
const applications = new Map<string, RequestInpatientAdmissionVO>([
  [
    'enc-20260723-009',
    {
      success: true,
      applicationId: 'admission-application-seeded',
      inpatientEncounterId: 'inpatient-encounter-seeded',
      inpatientEncounterNo: 'IP202607260001',
      status: 'PLANNED',
      statusName: '待住院登记'
    }
  ]
])

let applicationSequence = 1

function delay(ms = 120) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function cloneOption<T extends Record<string, unknown>>(option: T): T {
  return { ...option }
}

function requireOption(options: InpatientAdmissionOptionVO[], value: string, fieldName: string) {
  if (!options.some((option) => option.value === value)) {
    throw new Error(`${fieldName}不存在或已停用`)
  }
}

/** 模拟查询防重复状态。 */
export async function mockCheckInpatientAdmission(
  encounterId: string
): Promise<CheckInpatientAdmissionVO> {
  await delay()
  const application = applications.get(encounterId)
  if (!application || application.status === 'CANCELLED') return { exists: false }

  return {
    exists: true,
    applicationId: application.applicationId,
    inpatientEncounterId: application.inpatientEncounterId,
    inpatientEncounterNo: application.inpatientEncounterNo,
    status: application.status,
    statusName: application.statusName
  }
}

/** 模拟加载住院办理固定选项。 */
export async function mockLoadInpatientAdmissionOptions(): Promise<InpatientAdmissionFormOptionsVO> {
  await delay()
  return {
    departmentOptions: departmentOptions.map(cloneOption),
    admissionTypeOptions: admissionTypeOptions.map(cloneOption),
    admissionWayOptions: admissionWayOptions.map(cloneOption),
    priorityOptions: priorityOptions.map(cloneOption)
  }
}

/** 模拟按住院科室加载病区。 */
export async function mockListInpatientAdmissionWards(
  departmentId: string
): Promise<InpatientAdmissionWardVO[]> {
  await delay()
  requireOption(departmentOptions, departmentId, '入院科室')
  return wardOptions.filter((ward) => ward.departmentId === departmentId).map(cloneOption)
}

/** 模拟提交住院申请，并在“服务端”再次完成防重复与字段校验。 */
export async function mockRequestInpatientAdmission(
  data: RequestInpatientAdmissionDTO
): Promise<RequestInpatientAdmissionVO> {
  await delay()

  if (!data.encounterId.trim()) throw new Error('缺少门诊就诊标识')
  if (!data.patientId.trim()) throw new Error('缺少患者标识')
  const existingApplication = applications.get(data.encounterId)
  if (existingApplication && existingApplication.status !== 'CANCELLED') {
    throw new Error('当前门诊就诊已办理住院，请勿重复提交')
  }

  requireOption(departmentOptions, data.targetDepartmentId, '入院科室')
  if (data.wardId) {
    const ward = wardOptions.find((item) => item.value === data.wardId)
    if (!ward || ward.departmentId !== data.targetDepartmentId) {
      throw new Error('入院病区与入院科室不匹配')
    }
  }

  if (
    data.admissionDiagnoses.length !== 1 ||
    data.admissionDiagnoses.filter((item) => item.primary).length !== 1
  ) {
    throw new Error('请设置唯一的入院主诊断')
  }
  const diagnosis = data.admissionDiagnoses[0]!
  if (!diagnosis.catalogId || !diagnosis.code || !diagnosis.name.trim()) {
    throw new Error('入院主诊断信息不完整')
  }

  if (data.admissionTypeCode) {
    requireOption(admissionTypeOptions, data.admissionTypeCode, '入院类型')
  }
  if (data.admissionWayCode) {
    requireOption(admissionWayOptions, data.admissionWayCode, '入院方式')
  }
  if (data.priorityCode) {
    requireOption(priorityOptions, data.priorityCode, '病情优先级')
  }

  applicationSequence += 1
  const suffix = String(applicationSequence).padStart(4, '0')
  const result: RequestInpatientAdmissionVO = {
    success: true,
    applicationId: `admission-application-${suffix}`,
    inpatientEncounterId: `inpatient-encounter-${suffix}`,
    inpatientEncounterNo: `IP20260726${suffix}`,
    status: 'PLANNED',
    statusName: '待住院登记'
  }
  applications.set(data.encounterId, result)
  return { ...result }
}
