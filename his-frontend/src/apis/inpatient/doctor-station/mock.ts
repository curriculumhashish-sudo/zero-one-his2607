import { createPageDTO, type PageDTO } from '../../type'
import type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  DeleteInpatientDiagnosisDTO,
  GroupInpatientMedicalOrdersDTO,
  InpatientDiagnosisBriefVO,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDischargeAdviceVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientDoctorPatientQueueQuery,
  InpatientMedicalOrderBatchActionDTO,
  InpatientMedicalOrderBatchResultVO,
  InpatientMedicalOrderCatalogItemVO,
  InpatientMedicalOrderCatalogQuery,
  InpatientMedicalOrderQuery,
  InpatientMedicalOrderRecordVO,
  InpatientMedicalOrderStatus,
  InpatientMedicalRecordDocumentTypeOptionVO,
  InpatientMedicalRecordHistoryDetailVO,
  InpatientMedicalRecordHistoryItemVO,
  InpatientMedicalRecordHistoryQuery,
  InpatientMedicalRecordVO,
  SaveInpatientMedicalRecordDTO,
  SignInpatientMedicalRecordDTO,
  UpdateInpatientDiagnosisDTO,
  UpdateInpatientMedicalOrderDTO
} from './type'

const MOCK_DELAY = 80

const inpatientPatientSeed: InpatientDoctorPatientQueueItemVO[] = [
  {
    encounterId: 'ipt-enc-20260701-001',
    patientId: 'patient-10001',
    patientName: '王建国',
    genderName: '男',
    ageText: '68岁',
    inpatientNo: 'ZY20260701001',
    admissionAt: '2026-07-21 09:20',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-08',
    bedName: '08床',
    statusEnum: 'IN_PROGRESS',
    diagnosisSummary: '冠状动脉粥样硬化性心脏病'
  },
  {
    encounterId: 'ipt-enc-20260702-002',
    patientId: 'patient-10002',
    patientName: '赵桂芳',
    genderName: '女',
    ageText: '74岁',
    inpatientNo: 'ZY20260702006',
    admissionAt: '2026-07-22 14:05',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-12',
    bedName: '12床',
    statusEnum: 'IN_PROGRESS',
    diagnosisSummary: '原发性高血压3级'
  },
  {
    encounterId: 'ipt-enc-20260703-003',
    patientId: 'patient-10003',
    patientName: '陈志强',
    genderName: '男',
    ageText: '56岁',
    inpatientNo: 'ZY20260703003',
    admissionAt: '2026-07-23 10:32',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-15',
    bedName: '15床',
    statusEnum: 'ON_HOLD',
    diagnosisSummary: '阵发性心房颤动'
  }
]

const inpatientMedicalRecordSeed: InpatientMedicalRecordVO[] = [
  {
    id: 'ipt-record-10001-admission',
    patientId: 'patient-10001',
    encounterId: 'ipt-enc-20260701-001',
    inpatientNo: 'ZY20260701001',
    documentTypeCode: 'ADMISSION_RECORD',
    documentTypeName: '入院记录',
    title: '心血管内科入院记录',
    status: 'SIGNED',
    statusName: '已签名',
    departmentName: '心血管内科',
    authoredByName: '李尔王',
    authoredAt: '2026-07-21 11:10:00',
    updatedAt: '2026-07-21 11:26:00',
    content:
      '<h3>入院记录</h3><p><strong>主诉：</strong>反复胸闷、胸痛2年，加重3天。</p><p><strong>现病史：</strong>患者近3天活动后胸痛频率增加，休息后可缓解。</p><p><strong>初步诊断：</strong>冠状动脉粥样硬化性心脏病。</p>',
    version: 2,
    readonly: true
  },
  {
    id: 'ipt-record-10001-progress-01',
    patientId: 'patient-10001',
    encounterId: 'ipt-enc-20260701-001',
    inpatientNo: 'ZY20260701001',
    documentTypeCode: 'PROGRESS_NOTE',
    documentTypeName: '病程记录',
    title: '首次病程记录',
    status: 'SIGNED',
    statusName: '已签名',
    departmentName: '心血管内科',
    authoredByName: '李尔王',
    authoredAt: '2026-07-21 15:20:00',
    updatedAt: '2026-07-21 15:38:00',
    content:
      '<h3>首次病程记录</h3><p>患者老年男性，因反复胸闷、胸痛入院。结合既往史及辅助检查，冠心病诊断依据充分。</p><p><strong>诊疗计划：</strong>完善心电图、心肌酶及冠脉评估，给予抗血小板、调脂治疗。</p>',
    version: 1,
    readonly: true
  },
  {
    id: 'ipt-record-10001-history-discharge',
    patientId: 'patient-10001',
    encounterId: 'ipt-enc-20260112-021',
    inpatientNo: 'ZY20260112021',
    documentTypeCode: 'DISCHARGE_RECORD',
    documentTypeName: '出院记录',
    title: '既往住院出院记录',
    status: 'ARCHIVED',
    statusName: '已归档',
    departmentName: '心血管内科',
    authoredByName: '张医生',
    authoredAt: '2026-01-20 09:15:00',
    updatedAt: '2026-01-20 10:02:00',
    content:
      '<h3>出院记录</h3><p><strong>入院诊断：</strong>冠状动脉粥样硬化性心脏病。</p><p><strong>出院情况：</strong>胸痛症状缓解，生命体征平稳。</p>',
    version: 3,
    readonly: true
  },
  {
    id: 'ipt-record-10002-admission',
    patientId: 'patient-10002',
    encounterId: 'ipt-enc-20260702-002',
    inpatientNo: 'ZY20260702006',
    documentTypeCode: 'ADMISSION_RECORD',
    documentTypeName: '入院记录',
    title: '高血压入院记录',
    status: 'SIGNED',
    statusName: '已签名',
    departmentName: '心血管内科',
    authoredByName: '李尔王',
    authoredAt: '2026-07-22 16:30:00',
    updatedAt: '2026-07-22 16:42:00',
    content:
      '<h3>入院记录</h3><p><strong>主诉：</strong>发现血压升高20余年，头晕1周。</p><p><strong>初步诊断：</strong>原发性高血压3级。</p>',
    version: 1,
    readonly: true
  },
  {
    id: 'ipt-record-10002-history',
    patientId: 'patient-10002',
    encounterId: 'ipt-enc-20250308-032',
    inpatientNo: 'ZY20250308032',
    documentTypeCode: 'DISCHARGE_RECORD',
    documentTypeName: '出院记录',
    title: '既往高血压住院记录',
    status: 'ARCHIVED',
    statusName: '已归档',
    departmentName: '老年医学科',
    authoredByName: '王医生',
    authoredAt: '2025-03-18 10:00:00',
    updatedAt: '2025-03-18 10:18:00',
    content: '<h3>出院记录</h3><p>住院期间调整降压方案，血压控制平稳后出院。</p>',
    version: 2,
    readonly: true
  },
  ...[['10003', 'ipt-enc-20260703-003', 'ZY20260703003', '阵发性心房颤动入院记录']].map(
    ([patientSuffix, encounterId, inpatientNo, title]): InpatientMedicalRecordHistoryDetailVO => ({
      id: `ipt-record-${patientSuffix}-admission`,
      patientId: `patient-${patientSuffix}`,
      encounterId: encounterId!,
      inpatientNo: inpatientNo!,
      documentTypeCode: 'ADMISSION_RECORD',
      documentTypeName: '入院记录',
      title: title!,
      status: 'SIGNED',
      statusName: '已签名',
      departmentName: '心血管内科',
      authoredByName: '李尔王',
      authoredAt: '2026-07-26 18:00:00',
      updatedAt: '2026-07-26 18:20:00',
      content: `<h3>入院记录</h3><p>${title}，具体内容请结合患者本次住院情况查看。</p>`,
      version: 1,
      readonly: true
    })
  )
]

const inpatientMedicalRecordDocumentTypeOptions: InpatientMedicalRecordDocumentTypeOptionVO[] = [
  {
    value: 'ADMISSION_RECORD',
    label: '入院记录',
    defaultTitle: '入院记录',
    defaultContent:
      '<h3>入院记录</h3><p><strong>主诉：</strong></p><p><strong>现病史：</strong></p><p><strong>既往史：</strong></p><p><strong>专科检查：</strong></p><p><strong>初步诊断：</strong></p>'
  },
  {
    value: 'FIRST_COURSE_RECORD',
    label: '首次病程记录',
    defaultTitle: '首次病程记录',
    defaultContent:
      '<h3>首次病程记录</h3><p><strong>病例特点：</strong></p><p><strong>诊断依据：</strong></p><p><strong>鉴别诊断：</strong></p><p><strong>诊疗计划：</strong></p>'
  },
  {
    value: 'DAILY_COURSE_RECORD',
    label: '日常病程记录',
    defaultTitle: '日常病程记录',
    defaultContent:
      '<h3>日常病程记录</h3><p><strong>病情变化：</strong></p><p><strong>查房意见：</strong></p><p><strong>诊疗计划：</strong></p>'
  },
  {
    value: 'DISCHARGE_RECORD',
    label: '出院记录',
    defaultTitle: '出院记录',
    defaultContent:
      '<h3>出院记录</h3><p><strong>入院诊断：</strong></p><p><strong>诊疗经过：</strong></p><p><strong>出院诊断：</strong></p><p><strong>出院情况：</strong></p><p><strong>出院医嘱：</strong></p>'
  }
]

const inpatientDiagnosisRecords: Array<
  InpatientDiagnosisRecordVO & {
    encounterId: string
    patientId: string
  }
> = [
  {
    id: 'ipt-diagnosis-10001-1',
    conditionId: 'condition-i25-10',
    encounterId: 'ipt-enc-20260701-001',
    patientId: 'patient-10001',
    definitionCode: 'I25.10',
    description: '冠状动脉粥样硬化性心脏病',
    kindName: '西医',
    diagnosisTypeCode: '入院初步诊断',
    diagnosisTypeName: '入院初步诊断',
    primaryFlag: 1,
    rank: 1,
    tcmFlag: 0
  },
  {
    id: 'ipt-diagnosis-10001-2',
    conditionId: 'condition-i10',
    encounterId: 'ipt-enc-20260701-001',
    patientId: 'patient-10001',
    definitionCode: 'I10',
    description: '原发性高血压',
    kindName: '西医',
    diagnosisTypeCode: '入院初步诊断',
    diagnosisTypeName: '入院初步诊断',
    primaryFlag: 0,
    rank: 2,
    tcmFlag: 0
  },
  ...inpatientPatientSeed.slice(1).map((patient, index) => ({
    id: `ipt-diagnosis-${index + 2}-1`,
    conditionId: `condition-mock-${index + 2}`,
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    definitionCode: `MOCK-${String(index + 2).padStart(4, '0')}`,
    description: patient.diagnosisSummary || '待明确诊断',
    kindName: '西医',
    diagnosisTypeCode: '入院初步诊断',
    diagnosisTypeName: '入院初步诊断',
    primaryFlag: 1,
    rank: 1,
    tcmFlag: 0
  }))
]

const inpatientMedicalOrderCatalog: InpatientMedicalOrderCatalogItemVO[] = [
  {
    id: 'order-drug-aspirin',
    code: 'YP0001',
    name: '阿司匹林肠溶片',
    category: 'DRUG',
    categoryName: '西药',
    specification: '100mg×30片',
    defaultDose: 100,
    defaultUnit: 'mg',
    defaultFrequency: '每日一次',
    defaultRoute: '口服',
    defaultDays: 7,
    defaultQuantity: 7,
    quantityUnit: '片',
    defaultTherapyType: 'LONG_TERM'
  },
  {
    id: 'order-drug-metoprolol',
    code: 'YP0002',
    name: '酒石酸美托洛尔片',
    category: 'DRUG',
    categoryName: '西药',
    specification: '25mg×20片',
    defaultDose: 25,
    defaultUnit: 'mg',
    defaultFrequency: '每日两次',
    defaultRoute: '口服',
    defaultDays: 7,
    defaultQuantity: 14,
    quantityUnit: '片',
    defaultTherapyType: 'LONG_TERM'
  },
  {
    id: 'order-drug-atorvastatin',
    code: 'YP0003',
    name: '阿托伐他汀钙片',
    category: 'DRUG',
    categoryName: '西药',
    specification: '20mg×14片',
    defaultDose: 20,
    defaultUnit: 'mg',
    defaultFrequency: '每晚一次',
    defaultRoute: '口服',
    defaultDays: 7,
    defaultQuantity: 7,
    quantityUnit: '片',
    defaultTherapyType: 'LONG_TERM'
  },
  {
    id: 'order-drug-furosemide',
    code: 'YP0004',
    name: '呋塞米注射液',
    category: 'DRUG',
    categoryName: '西药',
    specification: '2ml:20mg',
    defaultDose: 20,
    defaultUnit: 'mg',
    defaultFrequency: '每日一次',
    defaultRoute: '静脉注射',
    defaultDays: 1,
    defaultQuantity: 1,
    quantityUnit: '支',
    defaultTherapyType: 'LONG_TERM'
  },
  {
    id: 'order-lab-blood',
    code: 'JY0001',
    name: '血常规',
    category: 'LAB',
    categoryName: '检验',
    defaultQuantity: 1,
    quantityUnit: '次',
    defaultTherapyType: 'TEMPORARY'
  },
  {
    id: 'order-lab-biochemical',
    code: 'JY0002',
    name: '肝肾功能及电解质',
    category: 'LAB',
    categoryName: '检验',
    defaultQuantity: 1,
    quantityUnit: '次',
    defaultTherapyType: 'TEMPORARY'
  },
  {
    id: 'order-exam-ecg',
    code: 'JC0001',
    name: '十二导联心电图',
    category: 'EXAM',
    categoryName: '检查',
    defaultQuantity: 1,
    quantityUnit: '次',
    defaultTherapyType: 'TEMPORARY'
  },
  {
    id: 'order-exam-echo',
    code: 'JC0002',
    name: '心脏彩色多普勒超声',
    category: 'EXAM',
    categoryName: '检查',
    defaultQuantity: 1,
    quantityUnit: '次',
    defaultTherapyType: 'TEMPORARY'
  },
  {
    id: 'order-treatment-oxygen',
    code: 'ZL0001',
    name: '低流量吸氧',
    category: 'TREATMENT',
    categoryName: '治疗',
    defaultQuantity: 1,
    quantityUnit: '小时',
    defaultTherapyType: 'LONG_TERM'
  }
]

const inpatientMedicalOrderStatusNames: Record<InpatientMedicalOrderStatus, string> = {
  DRAFT: '待签发',
  SIGNED: '已签发',
  STOPPED: '已停嘱',
  WITHDRAWN: '已撤回'
}

function createMedicalOrderRecord(
  encounterId: string,
  patientId: string,
  catalogIndex: number,
  status: InpatientMedicalOrderStatus,
  orderNo: number,
  groupId?: string
): InpatientMedicalOrderRecordVO {
  const item = inpatientMedicalOrderCatalog[catalogIndex]!
  const therapyType = item.defaultTherapyType
  return {
    id: `ipt-order-${encounterId}-${item.id}`,
    encounterId,
    patientId,
    catalogId: item.id,
    code: item.code,
    name: item.name,
    category: item.category,
    categoryName: item.categoryName,
    specification: item.specification,
    dose: item.defaultDose,
    unit: item.defaultUnit,
    frequency: item.defaultFrequency,
    route: item.defaultRoute,
    days: item.defaultDays,
    quantity: item.defaultQuantity ?? 1,
    quantityUnit: item.quantityUnit ?? '项',
    therapyType,
    therapyTypeName: therapyType === 'LONG_TERM' ? '长期' : '临时',
    groupId,
    status,
    statusName: inpatientMedicalOrderStatusNames[status],
    orderNo,
    authoredAt: '2026-07-27 08:30:00',
    signedAt: status === 'DRAFT' ? undefined : '2026-07-27 08:45:00',
    stoppedAt: status === 'STOPPED' ? '2026-07-27 09:40:00' : undefined,
    updatedAt: '2026-07-27 09:40:00',
    version: 1
  }
}

const inpatientMedicalOrderRecords: InpatientMedicalOrderRecordVO[] = [
  createMedicalOrderRecord('ipt-enc-20260701-001', 'patient-10001', 0, 'DRAFT', 1, 'group-10001-1'),
  createMedicalOrderRecord('ipt-enc-20260701-001', 'patient-10001', 1, 'DRAFT', 2, 'group-10001-1'),
  createMedicalOrderRecord('ipt-enc-20260701-001', 'patient-10001', 2, 'SIGNED', 3),
  createMedicalOrderRecord('ipt-enc-20260701-001', 'patient-10001', 3, 'STOPPED', 4),
  createMedicalOrderRecord('ipt-enc-20260701-001', 'patient-10001', 4, 'SIGNED', 5),
  createMedicalOrderRecord('ipt-enc-20260702-002', 'patient-10002', 1, 'DRAFT', 1),
  createMedicalOrderRecord('ipt-enc-20260702-002', 'patient-10002', 5, 'SIGNED', 2),
  createMedicalOrderRecord('ipt-enc-20260703-003', 'patient-10003', 0, 'SIGNED', 1)
]

const inpatientDischargeWayOptions = [
  { value: '1', label: '医嘱离院' },
  { value: '2', label: '医嘱转院' },
  { value: '3', label: '医嘱转社区卫生服务机构' },
  { value: '4', label: '非医嘱离院' },
  { value: '5', label: '死亡' },
  { value: '6', label: '其他' }
]

const inpatientDischargeAdviceRecords: InpatientDischargeAdviceVO[] = []

let nextMedicalOrderId = 200
let nextMedicalOrderGroupId = 1
let nextDischargeAdviceId = 10
let nextMedicalRecordId = 100
let nextDiagnosisId = 100

function delay<T>(value: T): Promise<T> {
  return new Promise((resolve) => {
    window.setTimeout(() => resolve(value), MOCK_DELAY)
  })
}

function delayReject(message: string): Promise<never> {
  return new Promise((_, reject) => {
    window.setTimeout(() => reject(new Error(message)), MOCK_DELAY)
  })
}

function findInpatientPatientOrThrow(encounterId: string) {
  const patient = inpatientPatientSeed.find((item) => item.encounterId === encounterId)
  if (!patient) throw new Error('住院患者不存在或已出院')
  return patient
}

function ensureDiagnosisEditable(encounterId: string) {
  return findInpatientPatientOrThrow(encounterId)
}

function toMockDiagnosisVO(
  record: InpatientDiagnosisRecordVO & { encounterId: string; patientId: string }
): InpatientDiagnosisRecordVO {
  return {
    id: record.id,
    conditionId: record.conditionId,
    definitionCode: record.definitionCode,
    description: record.description,
    kindName: record.kindName,
    diagnosisTypeCode: record.diagnosisTypeCode,
    diagnosisTypeName: record.diagnosisTypeName,
    primaryFlag: record.primaryFlag,
    rank: record.rank,
    tcmFlag: record.tcmFlag
  }
}

/** 模拟分页查询住院医生站在院患者队列。 */
export async function mockPageInpatientDoctorPatientQueue(
  query: InpatientDoctorPatientQueueQuery
): Promise<PageDTO<InpatientDoctorPatientQueueItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)

  const matched = inpatientPatientSeed.filter((patient) => {
    if (inpatientDischargeAdviceRecords.some((item) => item.encounterId === patient.encounterId)) {
      return false
    }
    if (!keyword) return true

    return [patient.patientName, patient.inpatientNo, patient.bedName].some((value) =>
      value?.toLowerCase().includes(keyword)
    )
  })

  const start = (pageIndex - 1) * pageSize
  const rows = matched.slice(start, start + pageSize).map((patient) => ({ ...patient }))

  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows
    })
  )
}

/** 模拟分页查询患者跨住院就诊的历史病历。 */
export async function mockPageInpatientMedicalRecordHistory(
  query: InpatientMedicalRecordHistoryQuery
): Promise<PageDTO<InpatientMedicalRecordHistoryItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const documentTypeCode = query.documentTypeCode?.trim() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)

  const matched = inpatientMedicalRecordSeed
    .filter((record) => record.patientId === query.patientId)
    .filter((record) => !documentTypeCode || record.documentTypeCode === documentTypeCode)
    .filter((record) => {
      if (!keyword) return true
      return [
        record.title,
        record.documentTypeName,
        record.inpatientNo,
        record.departmentName,
        record.authoredByName
      ].some((value) => value.toLowerCase().includes(keyword))
    })
    .slice()
    .sort((left, right) => {
      const leftCurrent = left.encounterId === query.currentEncounterId ? 1 : 0
      const rightCurrent = right.encounterId === query.currentEncounterId ? 1 : 0
      if (leftCurrent !== rightCurrent) return rightCurrent - leftCurrent
      return right.authoredAt.localeCompare(left.authoredAt)
    })

  const start = (pageIndex - 1) * pageSize
  const rows = matched
    .slice(start, start + pageSize)
    .map(({ content: _content, version: _version, readonly: _readonly, ...summary }) => ({
      ...summary
    }))

  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows
    })
  )
}

/** 模拟获取单份历史病历详情。 */
export function mockGetInpatientMedicalRecordHistoryDetail(
  recordId: string
): Promise<InpatientMedicalRecordHistoryDetailVO> {
  const record = inpatientMedicalRecordSeed.find((item) => item.id === recordId)
  if (!record) return delayReject('历史病历不存在或已被删除')
  return delay({ ...record, readonly: true as const })
}

/** 模拟获取住院病历文书类型。 */
export function mockListInpatientMedicalRecordDocumentTypes(): Promise<
  InpatientMedicalRecordDocumentTypeOptionVO[]
> {
  return delay(inpatientMedicalRecordDocumentTypeOptions.map((item) => ({ ...item })))
}

/** 模拟获取一份住院病历；当前住院的草稿允许继续编辑。 */
export function mockGetInpatientMedicalRecord(recordId: string): Promise<InpatientMedicalRecordVO> {
  const record = inpatientMedicalRecordSeed.find((item) => item.id === recordId)
  if (!record) return delayReject('住院病历不存在或已被删除')
  const isCurrentEncounter = inpatientPatientSeed.some(
    (patient) => patient.encounterId === record.encounterId
  )
  return delay({
    ...record,
    readonly: record.status !== 'DRAFT' || !isCurrentEncounter
  })
}

/** 模拟保存住院病历草稿。 */
export function mockSaveInpatientMedicalRecord(
  data: SaveInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  try {
    const patient = findInpatientPatientOrThrow(data.encounterId)
    if (patient.patientId !== data.patientId) throw new Error('患者档案与住院就诊不匹配')

    const documentType = inpatientMedicalRecordDocumentTypeOptions.find(
      (item) => item.value === data.documentTypeCode
    )
    if (!documentType) throw new Error('请选择有效的病历文书类型')
    const title = data.title.trim()
    if (!title) throw new Error('请填写病历标题')

    const now = '2026-07-28 10:15:00'
    if (!data.id) {
      // Version is no longer part of the frontend create payload; keep zero as the Mock default.
      if ((data.version ?? 0) !== 0) throw new Error('新建病历的版本号必须为 0')
      const created: InpatientMedicalRecordVO = {
        id: `ipt-record-${nextMedicalRecordId++}`,
        patientId: data.patientId,
        encounterId: data.encounterId,
        inpatientNo: patient.inpatientNo,
        documentTypeCode: documentType.value,
        documentTypeName: documentType.label,
        title,
        status: 'DRAFT',
        statusName: '草稿',
        departmentName: patient.departmentName,
        authoredByName: '当前医生',
        authoredAt: now,
        updatedAt: now,
        content: data.content,
        version: 1,
        readonly: false
      }
      inpatientMedicalRecordSeed.push(created)
      return delay({ ...created })
    }

    const record = inpatientMedicalRecordSeed.find((item) => item.id === data.id)
    if (!record) throw new Error('住院病历不存在或已被删除')
    if (record.encounterId !== data.encounterId || record.patientId !== data.patientId) {
      throw new Error('病历与当前住院患者不匹配')
    }
    if (record.status !== 'DRAFT') throw new Error('已签名或归档病历不能修改')
    if (record.version !== data.version) throw new Error('病历已被他人修改，请刷新后重试')

    record.documentTypeCode = documentType.value
    record.documentTypeName = documentType.label
    record.title = title
    record.content = data.content
    record.updatedAt = now
    record.version += 1
    record.readonly = false
    return delay({ ...record })
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '保存住院病历失败')
  }
}

/** 模拟签名住院病历。 */
export function mockSignInpatientMedicalRecord(
  data: SignInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  try {
    const record = inpatientMedicalRecordSeed.find((item) => item.id === data.id)
    if (!record) throw new Error('住院病历不存在或已被删除')
    findInpatientPatientOrThrow(record.encounterId)
    if (record.status !== 'DRAFT') throw new Error('只有草稿病历可以签名')
    if (record.version !== data.version) throw new Error('病历已被他人修改，请刷新后重试')
    const plainText = record.content
      .replace(/<[^>]*>/g, '')
      .replace(/&nbsp;|&#160;/gi, '')
      .trim()
    if (!plainText) throw new Error('病历正文不能为空')

    record.status = 'SIGNED'
    record.statusName = '已签名'
    record.updatedAt = '2026-07-28 10:20:00'
    record.version += 1
    record.readonly = true
    return delay({ ...record })
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '签名住院病历失败')
  }
}

/** 模拟分页查询当前住院就诊的诊断记录。 */
export function mockPageInpatientDiagnosisRecords(
  query: InpatientDiagnosisRecordQuery
): Promise<PageDTO<InpatientDiagnosisRecordVO>> {
  try {
    const patient = findInpatientPatientOrThrow(query.encounterId)
    if (patient.patientId !== query.patientId) throw new Error('患者档案与住院就诊不匹配')

    const pageIndex = Math.max(1, query.pageIndex)
    const pageSize = Math.max(1, query.pageSize)
    const matched = inpatientDiagnosisRecords
      .filter(
        (record) => record.encounterId === query.encounterId && record.patientId === query.patientId
      )
      .slice()
      .sort((left, right) => left.rank - right.rank)
    const start = (pageIndex - 1) * pageSize

    return delay(
      createPageDTO({
        pageIndex,
        pageSize,
        total: matched.length,
        rows: matched.slice(start, start + pageSize).map(toMockDiagnosisVO)
      })
    )
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院诊断加载失败')
  }
}

/** 模拟租户范围内的历史诊断候选。 */
export function mockListInpatientDiagnosisBrief(): Promise<InpatientDiagnosisBriefVO[]> {
  const names = new Set(
    inpatientDiagnosisRecords.map((record) => record.description.trim()).filter(Boolean)
  )
  return delay(Array.from(names, (definitionName) => ({ definitionName })))
}

/** 模拟新增住院诊断。 */
export function mockCreateInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  try {
    const patient = ensureDiagnosisEditable(data.encounterId)
    if (patient.patientId !== data.patientId) throw new Error('患者档案与住院就诊不匹配')

    const description = data.description.trim()
    if (!description) throw new Error('诊断描述不能为空')
    if (
      inpatientDiagnosisRecords.some(
        (record) =>
          record.encounterId === data.encounterId && record.description.trim() === description
      )
    ) {
      throw new Error('该诊断已存在')
    }

    const sequence = nextDiagnosisId++
    const record = {
      id: `ipt-diagnosis-${sequence}`,
      conditionId: `condition-mock-${sequence}`,
      encounterId: data.encounterId,
      patientId: data.patientId,
      definitionCode: data.definitionCode ?? '',
      description,
      kindName: data.tcmFlag === 1 ? '中医' : '西医',
      diagnosisTypeCode: data.diagnosisTypeCode ?? '入院初步诊断',
      diagnosisTypeName: data.diagnosisTypeCode ?? '入院初步诊断',
      primaryFlag: data.primaryFlag,
      rank: data.rank,
      tcmFlag: data.tcmFlag
    }
    inpatientDiagnosisRecords.push(record)
    return delay(toMockDiagnosisVO(record))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '新增住院诊断失败')
  }
}

/** 模拟修改住院诊断类型。 */
export function mockUpdateInpatientDiagnosis(data: UpdateInpatientDiagnosisDTO): Promise<void> {
  try {
    const record = inpatientDiagnosisRecords.find((item) => item.id === data.id)
    if (!record) throw new Error('住院诊断不存在或已被删除')
    ensureDiagnosisEditable(record.encounterId)
    record.diagnosisTypeCode = data.diagnosisTypeCode
    record.diagnosisTypeName = data.diagnosisTypeCode
    return delay(undefined)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '修改住院诊断失败')
  }
}

/** 模拟删除住院诊断。 */
export function mockDeleteInpatientDiagnosis(data: DeleteInpatientDiagnosisDTO): Promise<void> {
  try {
    const index = inpatientDiagnosisRecords.findIndex((item) => item.id === data.id)
    const record = inpatientDiagnosisRecords[index]
    if (!record || record.conditionId !== data.conditionId) {
      throw new Error('住院诊断不存在或已被删除')
    }
    ensureDiagnosisEditable(record.encounterId)
    inpatientDiagnosisRecords.splice(index, 1)
    return delay(undefined)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '删除住院诊断失败')
  }
}

/** 校验批量医嘱操作并返回对应医嘱。 */
function medicalOrderBatchRows(
  data: InpatientMedicalOrderBatchActionDTO,
  allowedStatuses: InpatientMedicalOrderStatus[],
  actionName: string
) {
  const patient = ensureDiagnosisEditable(data.encounterId)
  if (patient.patientId !== data.patientId) throw new Error('患者档案与住院就诊不匹配')
  if (data.items.length === 0) throw new Error(`请至少选择一条可${actionName}医嘱`)

  const uniqueIds = new Set(data.items.map((item) => item.id))
  if (uniqueIds.size !== data.items.length) throw new Error('批量医嘱中存在重复记录')

  return data.items.map((item) => {
    const record = inpatientMedicalOrderRecords.find((order) => order.id === item.id)
    if (!record || record.encounterId !== data.encounterId || record.patientId !== data.patientId) {
      throw new Error('医嘱不存在或不属于当前患者')
    }
    if (record.version !== item.version) throw new Error('医嘱已被他人修改，请刷新后重试')
    if (!allowedStatuses.includes(record.status)) {
      throw new Error(`${record.name}当前状态不允许${actionName}`)
    }
    return record
  })
}

function medicalOrderBatchResult(
  rows: InpatientMedicalOrderRecordVO[]
): InpatientMedicalOrderBatchResultVO {
  return {
    updatedCount: rows.length,
    rows: rows.map((row) => ({ ...row }))
  }
}

function touchMedicalOrder(
  record: InpatientMedicalOrderRecordVO,
  patch: Partial<InpatientMedicalOrderRecordVO>
) {
  Object.assign(record, patch, {
    updatedAt: '2026-07-27 10:30:00',
    version: record.version + 1
  })
}

/** 模拟分页查询当前住院医嘱。 */
export function mockPageInpatientMedicalOrders(
  query: InpatientMedicalOrderQuery
): Promise<PageDTO<InpatientMedicalOrderRecordVO>> {
  try {
    findInpatientPatientOrThrow(query.encounterId)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院患者不存在或已出院')
  }

  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const matched = inpatientMedicalOrderRecords
    .filter((record) => record.encounterId === query.encounterId)
    .filter((record) => !query.status || record.status === query.status)
    .filter((record) => !query.therapyType || record.therapyType === query.therapyType)
    .filter((record) => {
      if (!keyword) return true
      return [record.code, record.name, record.categoryName, record.remark, record.statusName].some(
        (value) => value?.toLowerCase().includes(keyword)
      )
    })
    .slice()
    .sort((left, right) => left.orderNo - right.orderNo)
  const start = (pageIndex - 1) * pageSize

  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows: matched.slice(start, start + pageSize).map((record) => ({ ...record }))
    })
  )
}

/** 模拟分页查询住院医嘱项目目录。 */
export function mockPageInpatientMedicalOrderCatalog(
  query: InpatientMedicalOrderCatalogQuery
): Promise<PageDTO<InpatientMedicalOrderCatalogItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const matched = inpatientMedicalOrderCatalog
    .filter((item) => !query.category || item.category === query.category)
    .filter((item) => {
      if (!keyword) return true
      return [item.code, item.name, item.categoryName, item.specification].some((value) =>
        value?.toLowerCase().includes(keyword)
      )
    })
  const start = (pageIndex - 1) * pageSize

  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows: matched.slice(start, start + pageSize).map((item) => ({ ...item }))
    })
  )
}

/** 模拟新增一条住院医嘱草稿。 */
export function mockCreateInpatientMedicalOrder(
  data: CreateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  try {
    const patient = ensureDiagnosisEditable(data.encounterId)
    if (patient.patientId !== data.patientId) {
      return delayReject('患者档案与住院就诊不匹配')
    }
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱新增失败')
  }

  const record: InpatientMedicalOrderRecordVO = {
    id: `ipt-order-${nextMedicalOrderId++}`,
    ...data,
    therapyTypeName: data.therapyType === 'LONG_TERM' ? '长期' : '临时',
    status: 'DRAFT',
    statusName: inpatientMedicalOrderStatusNames.DRAFT,
    authoredAt: '2026-07-27 10:30:00',
    updatedAt: '2026-07-27 10:30:00',
    version: 1
  }
  inpatientMedicalOrderRecords.push(record)
  return delay({ ...record })
}

/** 模拟修改一条住院医嘱草稿。 */
export function mockUpdateInpatientMedicalOrder(
  orderId: string,
  data: UpdateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  const record = inpatientMedicalOrderRecords.find((item) => item.id === orderId)
  if (!record) return delayReject('住院医嘱不存在或已被删除')
  try {
    ensureDiagnosisEditable(record.encounterId)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱修改失败')
  }
  if (record.status !== 'DRAFT') return delayReject('只有待签发医嘱可以修改')
  if (record.version !== data.version) return delayReject('医嘱已被他人修改，请刷新后重试')

  touchMedicalOrder(record, {
    catalogId: data.catalogId,
    code: data.code,
    name: data.name,
    category: data.category,
    categoryName: data.categoryName,
    specification: data.specification,
    dose: data.dose,
    unit: data.unit,
    frequency: data.frequency,
    route: data.route,
    days: data.days,
    quantity: data.quantity,
    quantityUnit: data.quantityUnit,
    remark: data.remark,
    therapyType: data.therapyType,
    therapyTypeName: data.therapyType === 'LONG_TERM' ? '长期' : '临时',
    orderNo: data.orderNo
  })
  return delay({ ...record })
}

/** 模拟批量删除待签发住院医嘱。 */
export function mockDeleteInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['DRAFT'], '删除')
    const ids = new Set(rows.map((row) => row.id))
    for (let index = inpatientMedicalOrderRecords.length - 1; index >= 0; index -= 1) {
      if (ids.has(inpatientMedicalOrderRecords[index]!.id)) {
        inpatientMedicalOrderRecords.splice(index, 1)
      }
    }
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱删除失败')
  }
}

/** 模拟批量并组待签发药品医嘱。 */
export function mockGroupInpatientMedicalOrders(
  data: GroupInpatientMedicalOrdersDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['DRAFT'], '并组')
    if (rows.length < 2) throw new Error('并组至少需要选择两条待签发药品医嘱')
    if (rows.some((row) => row.category !== 'DRUG')) {
      throw new Error('只有药品医嘱可以并组')
    }
    const groupId = data.groupId?.trim() || `order-group-${nextMedicalOrderGroupId++}`
    rows.forEach((row) => touchMedicalOrder(row, { groupId }))
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱并组失败')
  }
}

/** 模拟批量拆组待签发药品医嘱。 */
export function mockUngroupInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['DRAFT'], '拆组')
    if (rows.some((row) => !row.groupId)) throw new Error('所选医嘱中存在未成组记录')
    rows.forEach((row) => touchMedicalOrder(row, { groupId: undefined }))
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱拆组失败')
  }
}

/** 模拟批量签发待签发住院医嘱。 */
export function mockSignInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['DRAFT'], '签发')
    rows.forEach((row) =>
      touchMedicalOrder(row, {
        status: 'SIGNED',
        statusName: inpatientMedicalOrderStatusNames.SIGNED,
        signedAt: '2026-07-27 10:30:00'
      })
    )
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱签发失败')
  }
}

/** 模拟批量撤回已签发住院医嘱。 */
export function mockWithdrawInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['SIGNED'], '撤回')
    rows.forEach((row) =>
      touchMedicalOrder(row, {
        status: 'WITHDRAWN',
        statusName: inpatientMedicalOrderStatusNames.WITHDRAWN
      })
    )
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱撤回失败')
  }
}

/** 模拟批量停止已签发长期医嘱。 */
export function mockStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['SIGNED'], '停嘱')
    if (rows.some((row) => row.therapyType !== 'LONG_TERM')) {
      throw new Error('只有已签发的长期医嘱可以停嘱')
    }
    rows.forEach((row) =>
      touchMedicalOrder(row, {
        status: 'STOPPED',
        statusName: inpatientMedicalOrderStatusNames.STOPPED,
        stoppedAt: '2026-07-27 10:30:00'
      })
    )
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱停嘱失败')
  }
}

/** 模拟批量取消长期医嘱停嘱。 */
export function mockCancelStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  try {
    const rows = medicalOrderBatchRows(data, ['STOPPED'], '取消停嘱')
    if (rows.some((row) => row.therapyType !== 'LONG_TERM')) {
      throw new Error('只有已停嘱的长期医嘱可以取消停嘱')
    }
    rows.forEach((row) =>
      touchMedicalOrder(row, {
        status: 'SIGNED',
        statusName: inpatientMedicalOrderStatusNames.SIGNED,
        stoppedAt: undefined
      })
    )
    return delay(medicalOrderBatchResult(rows))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院医嘱取消停嘱失败')
  }
}

/** 模拟开立出院医嘱并将患者移出在院队列。 */
export function mockCreateInpatientDischargeAdvice(
  data: CreateInpatientDischargeAdviceDTO
): Promise<InpatientDischargeAdviceVO> {
  try {
    const patient = findInpatientPatientOrThrow(data.encounterId)
    if (inpatientDischargeAdviceRecords.some((item) => item.encounterId === data.encounterId)) {
      throw new Error('该患者已经开立出院医嘱，不能重复开立')
    }
    if (!data.plannedDischargeTime.trim()) throw new Error('请选择计划出院时间')
    const outWay = inpatientDischargeWayOptions.find((item) => item.value === data.outWayCode)
    if (!outWay) throw new Error('请选择有效的出院方式')

    const now = '2026-07-28 09:30:00'
    const advice: InpatientDischargeAdviceVO = {
      id: `ipt-discharge-advice-${nextDischargeAdviceId++}`,
      encounterId: data.encounterId,
      statusEnum: 'ACTIVE',
      plannedDischargeTime: data.plannedDischargeTime,
      outWayCode: outWay.value,
      outWayName: outWay.label,
      encounterNewStatus: 'ON_HOLD'
    }
    inpatientDischargeAdviceRecords.push(advice)

    const currentOrders = inpatientMedicalOrderRecords.filter(
      (record) => record.encounterId === data.encounterId
    )
    inpatientMedicalOrderRecords.push({
      id: `ipt-order-${nextMedicalOrderId++}`,
      encounterId: data.encounterId,
      patientId: patient.patientId,
      catalogId: 'system-discharge-advice',
      code: 'DISCHARGE',
      name: `出院（${outWay.label}）`,
      category: 'TREATMENT',
      categoryName: '出院医嘱',
      quantity: 1,
      quantityUnit: '次',
      remark: data.plannedDischargeTime,
      therapyType: 'TEMPORARY',
      therapyTypeName: '临时',
      status: 'SIGNED',
      statusName: inpatientMedicalOrderStatusNames.SIGNED,
      orderNo: Math.max(0, ...currentOrders.map((record) => record.orderNo)) + 1,
      authoredAt: now,
      signedAt: now,
      updatedAt: now,
      version: 1
    })

    patient.statusEnum = 'ON_HOLD'
    return delay({ ...advice })
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '开立出院医嘱失败')
  }
}
