import { createPageDTO, type PageDTO } from '../../type'
import type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  GroupInpatientMedicalOrdersDTO,
  InpatientDiagnosisDictionaryItemVO,
  InpatientDiagnosisDictionaryQuery,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDiagnosisUsageHistoryQuery,
  InpatientDiagnosisUsageHistoryVO,
  InpatientDischargeAdvicePreparationVO,
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
    patientNo: 'P10001',
    inpatientNo: 'ZY20260701001',
    admissionAt: '2026-07-21 09:20',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-08',
    bedName: '08床',
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '周敏',
    paymentTypeName: '职工医保',
    diagnosisSummary: '冠状动脉粥样硬化性心脏病',
    riskHint: '青霉素过敏',
    pinyinCode: 'WJG',
    version: 1
  },
  {
    encounterId: 'ipt-enc-20260702-002',
    patientId: 'patient-10002',
    patientName: '赵桂芳',
    genderName: '女',
    ageText: '74岁',
    patientNo: 'P10002',
    inpatientNo: 'ZY20260702006',
    admissionAt: '2026-07-22 14:05',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-12',
    bedName: '12床',
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '周敏',
    paymentTypeName: '居民医保',
    diagnosisSummary: '原发性高血压3级',
    pinyinCode: 'ZGF',
    version: 1
  },
  {
    encounterId: 'ipt-enc-20260703-003',
    patientId: 'patient-10003',
    patientName: '陈志强',
    genderName: '男',
    ageText: '56岁',
    patientNo: 'P10003',
    inpatientNo: 'ZY20260703003',
    admissionAt: '2026-07-23 10:32',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-15',
    bedName: '15床',
    encounterStatus: 'DISCHARGE_PENDING',
    encounterStatusName: '待出院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '郑洁',
    paymentTypeName: '自费',
    diagnosisSummary: '阵发性心房颤动',
    pinyinCode: 'CZQ',
    version: 2
  },
  {
    encounterId: 'ipt-enc-20260704-004',
    patientId: 'patient-10004',
    patientName: '刘春梅',
    genderName: '女',
    ageText: '49岁',
    patientNo: 'P10004',
    inpatientNo: 'ZY20260704009',
    admissionAt: '2026-07-24 08:46',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-18',
    bedName: '18床',
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '郑洁',
    paymentTypeName: '职工医保',
    diagnosisSummary: '急性心肌炎',
    riskHint: '账户余额不足',
    pinyinCode: 'LCM',
    version: 1
  },
  {
    encounterId: 'ipt-enc-20260705-005',
    patientId: 'patient-10005',
    patientName: '孙大勇',
    genderName: '男',
    ageText: '62岁',
    patientNo: 'P10005',
    inpatientNo: 'ZY20260705011',
    admissionAt: '2026-07-25 11:18',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-21',
    bedName: '21床',
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '黄娟',
    paymentTypeName: '居民医保',
    diagnosisSummary: '慢性心力衰竭',
    pinyinCode: 'SDY',
    version: 1
  },
  {
    encounterId: 'ipt-enc-20260706-006',
    patientId: 'patient-10006',
    patientName: '何秀兰',
    genderName: '女',
    ageText: '81岁',
    patientNo: 'P10006',
    inpatientNo: 'ZY20260706002',
    admissionAt: '2026-07-26 16:40',
    departmentName: '心血管内科',
    wardId: 'ward-cardiology-1',
    wardName: '心内一病区',
    bedId: 'bed-cardiology-23',
    bedName: '23床',
    encounterStatus: 'IN_HOSPITAL',
    encounterStatusName: '在院',
    attendingDoctorName: '李尔王',
    primaryNurseName: '黄娟',
    paymentTypeName: '职工医保',
    diagnosisSummary: '病态窦房结综合征',
    pinyinCode: 'HXL',
    version: 1
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
  ...[
    ['10003', 'ipt-enc-20260703-003', 'ZY20260703003', '阵发性心房颤动入院记录'],
    ['10004', 'ipt-enc-20260704-004', 'ZY20260704009', '急性心肌炎入院记录'],
    ['10005', 'ipt-enc-20260705-005', 'ZY20260705011', '慢性心力衰竭入院记录'],
    ['10006', 'ipt-enc-20260706-006', 'ZY20260706002', '病态窦房结综合征入院记录']
  ].map(
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

const inpatientDiagnosisDictionary: InpatientDiagnosisDictionaryItemVO[] = [
  {
    id: 'diag-I251',
    code: 'I25.1',
    name: '冠状动脉粥样硬化性心脏病',
    typeName: '西医诊断'
  },
  { id: 'diag-I10', code: 'I10', name: '原发性高血压', typeName: '西医诊断' },
  { id: 'diag-I489', code: 'I48.9', name: '心房颤动和心房扑动', typeName: '西医诊断' },
  { id: 'diag-I409', code: 'I40.9', name: '急性心肌炎', typeName: '西医诊断' },
  { id: 'diag-I509', code: 'I50.9', name: '心力衰竭', typeName: '西医诊断' },
  { id: 'diag-I495', code: 'I49.5', name: '病态窦房结综合征', typeName: '西医诊断' },
  { id: 'diag-E785', code: 'E78.5', name: '高脂血症', typeName: '西医诊断' },
  { id: 'diag-R074', code: 'R07.4', name: '胸痛', typeName: '症状诊断' },
  { id: 'diag-R002', code: 'R00.2', name: '心悸', typeName: '症状诊断' }
]

function createDiagnosisRecord(
  encounterId: string,
  patientId: string,
  dictionaryIndex: number,
  primary: boolean,
  orderNo: number
): InpatientDiagnosisRecordVO {
  const item = inpatientDiagnosisDictionary[dictionaryIndex]!
  return {
    id: `ipt-diagnosis-${encounterId}-${item.id}`,
    encounterId,
    patientId,
    catalogId: item.id,
    code: item.code,
    name: item.name,
    typeName: item.typeName,
    primary,
    orderNo,
    diagnosedAt: '2026-07-27 09:30:00',
    updatedAt: '2026-07-27 09:30:00',
    version: 1
  }
}

const inpatientDiagnosisRecords: InpatientDiagnosisRecordVO[] = [
  createDiagnosisRecord('ipt-enc-20260701-001', 'patient-10001', 0, true, 1),
  createDiagnosisRecord('ipt-enc-20260701-001', 'patient-10001', 6, false, 2),
  createDiagnosisRecord('ipt-enc-20260702-002', 'patient-10002', 1, true, 1),
  createDiagnosisRecord('ipt-enc-20260703-003', 'patient-10003', 2, true, 1),
  createDiagnosisRecord('ipt-enc-20260704-004', 'patient-10004', 3, true, 1),
  createDiagnosisRecord('ipt-enc-20260705-005', 'patient-10005', 4, true, 1),
  createDiagnosisRecord('ipt-enc-20260706-006', 'patient-10006', 5, true, 1)
]

const inpatientDiagnosisUsageHistory: InpatientDiagnosisUsageHistoryVO[] = [
  { ...inpatientDiagnosisDictionary[1]!, useCount: 36, lastUsedAt: '2026-07-27 09:30:00' },
  { ...inpatientDiagnosisDictionary[0]!, useCount: 28, lastUsedAt: '2026-07-27 09:25:00' },
  { ...inpatientDiagnosisDictionary[4]!, useCount: 21, lastUsedAt: '2026-07-26 16:40:00' },
  { ...inpatientDiagnosisDictionary[2]!, useCount: 18, lastUsedAt: '2026-07-26 15:10:00' },
  { ...inpatientDiagnosisDictionary[6]!, useCount: 15, lastUsedAt: '2026-07-25 11:20:00' },
  { ...inpatientDiagnosisDictionary[7]!, useCount: 12, lastUsedAt: '2026-07-24 14:05:00' }
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
  createMedicalOrderRecord('ipt-enc-20260703-003', 'patient-10003', 0, 'SIGNED', 1),
  createMedicalOrderRecord('ipt-enc-20260704-004', 'patient-10004', 4, 'DRAFT', 1),
  createMedicalOrderRecord('ipt-enc-20260705-005', 'patient-10005', 4, 'SIGNED', 1),
  createMedicalOrderRecord('ipt-enc-20260706-006', 'patient-10006', 6, 'DRAFT', 1)
]

const inpatientDischargeWayOptions = [
  { value: '1', label: '医嘱离院' },
  { value: '2', label: '医嘱转院' },
  { value: '3', label: '医嘱转社区卫生服务机构' },
  { value: '4', label: '非医嘱离院' },
  { value: '5', label: '死亡' },
  { value: '9', label: '其他' }
]

const inpatientDischargeAdviceRecords: InpatientDischargeAdviceVO[] = [
  {
    id: 'ipt-discharge-advice-001',
    encounterId: 'ipt-enc-20260703-003',
    patientId: 'patient-10003',
    plannedDischargeTime: '2026-07-28 10:00:00',
    outWayCode: '1',
    outWayName: '医嘱离院',
    reasonText: '病情稳定，计划出院',
    sourceOrgId: '心血管内科',
    sourceLocationId: 'bed-cardiology-15',
    createdAt: '2026-07-27 16:20:00',
    createdByName: '李尔王',
    version: 1
  }
]

let nextDiagnosisId = 100
let nextMedicalOrderId = 200
let nextMedicalOrderGroupId = 1
let nextDischargeAdviceId = 10
let nextMedicalRecordId = 100

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

function updateDiagnosisUsage(data: CreateInpatientDiagnosisDTO) {
  const history = inpatientDiagnosisUsageHistory.find((item) => item.id === data.catalogId)
  if (history) {
    history.useCount += 1
    history.lastUsedAt = '2026-07-27 10:00:00'
    return
  }

  inpatientDiagnosisUsageHistory.push({
    id: data.catalogId,
    code: data.code,
    name: data.name,
    typeName: data.typeName,
    useCount: 1,
    lastUsedAt: '2026-07-27 10:00:00'
  })
}

/** 模拟分页查询住院医生站在院患者队列。 */
export async function mockPageInpatientDoctorPatientQueue(
  query: InpatientDoctorPatientQueueQuery
): Promise<PageDTO<InpatientDoctorPatientQueueItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)

  const matched = inpatientPatientSeed.filter((patient) => {
    if (!keyword) return true

    return [
      patient.patientName,
      patient.patientNo,
      patient.inpatientNo,
      patient.bedName,
      patient.diagnosisSummary,
      patient.pinyinCode
    ].some((value) => value?.toLowerCase().includes(keyword))
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
      if (data.version !== 0) throw new Error('新建病历的版本号必须为 0')
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
        authoredByName: patient.attendingDoctorName ?? '当前医生',
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
export async function mockPageInpatientDiagnosisRecords(
  query: InpatientDiagnosisRecordQuery
): Promise<PageDTO<InpatientDiagnosisRecordVO>> {
  try {
    findInpatientPatientOrThrow(query.encounterId)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院患者不存在或已出院')
  }

  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const matched = inpatientDiagnosisRecords
    .filter((record) => record.encounterId === query.encounterId)
    .filter((record) => {
      if (!keyword) return true
      return [record.code, record.name, record.typeName].some((value) =>
        value.toLowerCase().includes(keyword)
      )
    })
    .slice()
    .sort((left, right) => left.orderNo - right.orderNo)

  const start = (pageIndex - 1) * pageSize
  const rows = matched.slice(start, start + pageSize).map((record) => ({ ...record }))

  return delay(
    createPageDTO({
      pageIndex,
      pageSize,
      total: matched.length,
      rows
    })
  )
}

/** 模拟分页查询住院诊断字典。 */
export function mockPageInpatientDiagnosisDictionary(
  query: InpatientDiagnosisDictionaryQuery
): Promise<PageDTO<InpatientDiagnosisDictionaryItemVO>> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const matched = inpatientDiagnosisDictionary.filter((item) => {
    if (!keyword) return true
    return [item.code, item.name, item.typeName].some((value) =>
      value.toLowerCase().includes(keyword)
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

/** 模拟获取当前医生最近使用的诊断。 */
export function mockGetInpatientDiagnosisUsageHistory(
  query: InpatientDiagnosisUsageHistoryQuery = {}
): Promise<InpatientDiagnosisUsageHistoryVO[]> {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const limit = Math.max(1, query.limit ?? 20)
  const rows = inpatientDiagnosisUsageHistory
    .filter((item) => {
      if (!keyword) return true
      return [item.code, item.name, item.typeName].some((value) =>
        value.toLowerCase().includes(keyword)
      )
    })
    .slice()
    .sort((left, right) => right.useCount - left.useCount)
    .slice(0, limit)
    .map((item) => ({ ...item }))

  return delay(rows)
}

/** 模拟新增一条住院诊断。 */
export function mockCreateInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  try {
    const patient = ensureDiagnosisEditable(data.encounterId)
    if (patient.patientId !== data.patientId) {
      return delayReject('患者档案与住院就诊不匹配')
    }
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院诊断新增失败')
  }

  const duplicated = inpatientDiagnosisRecords.some(
    (record) => record.encounterId === data.encounterId && record.catalogId === data.catalogId
  )
  if (duplicated) return delayReject('本次住院已经存在相同诊断')

  const record: InpatientDiagnosisRecordVO = {
    id: `ipt-diagnosis-${nextDiagnosisId++}`,
    ...data,
    diagnosedAt: '2026-07-27 10:00:00',
    updatedAt: '2026-07-27 10:00:00',
    version: 1
  }
  inpatientDiagnosisRecords.push(record)
  updateDiagnosisUsage(data)
  return delay({ ...record })
}

/** 模拟修改一条住院诊断。 */
export function mockUpdateInpatientDiagnosis(
  diagnosisId: string,
  data: UpdateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  const record = inpatientDiagnosisRecords.find((item) => item.id === diagnosisId)
  if (!record) return delayReject('住院诊断不存在或已被删除')

  try {
    ensureDiagnosisEditable(record.encounterId)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院诊断修改失败')
  }
  if (record.version !== data.version) return delayReject('住院诊断已被他人修改，请刷新后重试')

  Object.assign(record, {
    catalogId: data.catalogId,
    code: data.code,
    name: data.name,
    typeName: data.typeName,
    primary: data.primary,
    orderNo: data.orderNo,
    updatedAt: '2026-07-27 10:00:00',
    version: record.version + 1
  })
  return delay({ ...record })
}

/** 模拟删除一条住院诊断。 */
export function mockDeleteInpatientDiagnosis(diagnosisId: string, version: number): Promise<void> {
  const index = inpatientDiagnosisRecords.findIndex((item) => item.id === diagnosisId)
  if (index < 0) return delayReject('住院诊断不存在或已被删除')

  const record = inpatientDiagnosisRecords[index]!
  try {
    ensureDiagnosisEditable(record.encounterId)
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '住院诊断删除失败')
  }
  if (record.version !== version) return delayReject('住院诊断已被他人修改，请刷新后重试')

  inpatientDiagnosisRecords.splice(index, 1)
  return delay(undefined)
}

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

function buildDischargeAdvicePreparation(
  encounterId: string,
  patientId: string
): InpatientDischargeAdvicePreparationVO {
  const patient = findInpatientPatientOrThrow(encounterId)
  if (patient.patientId !== patientId) throw new Error('患者档案与住院就诊不匹配')

  const existingAdvice = inpatientDischargeAdviceRecords.find(
    (item) => item.encounterId === encounterId
  )
  const diagnosisCount = inpatientDiagnosisRecords.filter(
    (record) => record.encounterId === encounterId
  ).length
  const draftMedicalRecordCount = inpatientMedicalRecordSeed.filter(
    (record) => record.encounterId === encounterId && record.status === 'DRAFT'
  ).length
  const draftOrderCount = inpatientMedicalOrderRecords.filter(
    (record) => record.encounterId === encounterId && record.status === 'DRAFT'
  ).length
  const activeLongTermOrderCount = inpatientMedicalOrderRecords.filter(
    (record) =>
      record.encounterId === encounterId &&
      record.status === 'SIGNED' &&
      record.therapyType === 'LONG_TERM'
  ).length
  const checks: InpatientDischargeAdvicePreparationVO['checks'] = []

  if (patient.encounterStatus === 'DISCHARGE_PENDING' || existingAdvice) {
    checks.push({
      code: 'DISCHARGE_ALREADY_PENDING',
      level: 'BLOCKER',
      message: '该患者已经开立出院医嘱，不能重复开立'
    })
  }
  if (diagnosisCount === 0) {
    checks.push({
      code: 'DIAGNOSIS_MISSING',
      level: 'BLOCKER',
      message: '当前住院就诊尚未维护诊断，请先完成诊断录入',
      targetTab: 'diagnosis'
    })
  }
  if (draftMedicalRecordCount > 0) {
    checks.push({
      code: 'MEDICAL_RECORD_DRAFT',
      level: 'REMINDER',
      message: `存在 ${draftMedicalRecordCount} 份未签名病历，请确认后续补签安排`,
      count: draftMedicalRecordCount,
      targetTab: 'medicalRecord'
    })
  }
  if (draftOrderCount > 0) {
    checks.push({
      code: 'ORDER_DRAFT',
      level: 'REMINDER',
      message: `存在 ${draftOrderCount} 条待签发医嘱，请确认是否继续保留`,
      count: draftOrderCount,
      targetTab: 'orders'
    })
  }
  if (activeLongTermOrderCount > 0) {
    checks.push({
      code: 'LONG_TERM_ORDER_ACTIVE',
      level: 'REMINDER',
      message: `存在 ${activeLongTermOrderCount} 条执行中的长期医嘱，请安排后续停嘱`,
      count: activeLongTermOrderCount,
      targetTab: 'orders'
    })
  }

  return {
    encounterId,
    patientId,
    encounterVersion: patient.version,
    canCreate: !checks.some((check) => check.level === 'BLOCKER'),
    checks,
    outWayOptions: inpatientDischargeWayOptions.map((item) => ({ ...item })),
    existingAdvice: existingAdvice ? { ...existingAdvice } : undefined
  }
}

/** 模拟加载出院医嘱弹窗及前置校验信息。 */
export function mockGetInpatientDischargeAdvicePreparation(
  encounterId: string,
  patientId: string
): Promise<InpatientDischargeAdvicePreparationVO> {
  try {
    return delay(buildDischargeAdvicePreparation(encounterId, patientId))
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '出院医嘱前置校验失败')
  }
}

/** 模拟开立出院医嘱并将患者更新为待出院状态。 */
export function mockCreateInpatientDischargeAdvice(
  data: CreateInpatientDischargeAdviceDTO
): Promise<InpatientDischargeAdviceVO> {
  try {
    const preparation = buildDischargeAdvicePreparation(data.encounterId, data.patientId)
    const patient = findInpatientPatientOrThrow(data.encounterId)
    if (!preparation.canCreate) {
      const blocker = preparation.checks.find((check) => check.level === 'BLOCKER')
      throw new Error(blocker?.message ?? '当前患者不能开立出院医嘱')
    }
    if (patient.version !== data.encounterVersion) {
      throw new Error('患者住院状态已被他人修改，请刷新后重试')
    }
    if (!data.plannedDischargeTime.trim()) throw new Error('请选择计划出院时间')
    const outWay = inpatientDischargeWayOptions.find((item) => item.value === data.outWayCode)
    if (!outWay) throw new Error('请选择有效的出院方式')

    const now = '2026-07-28 09:30:00'
    const advice: InpatientDischargeAdviceVO = {
      id: `ipt-discharge-advice-${nextDischargeAdviceId++}`,
      encounterId: data.encounterId,
      patientId: data.patientId,
      plannedDischargeTime: data.plannedDischargeTime,
      outWayCode: outWay.value,
      outWayName: outWay.label,
      reasonText: data.reasonText?.trim() || undefined,
      sourceOrgId: data.sourceOrgId,
      sourceLocationId: data.sourceLocationId,
      createdAt: now,
      createdByName: patient.attendingDoctorName ?? '当前医生',
      version: 1
    }
    inpatientDischargeAdviceRecords.push(advice)

    const currentOrders = inpatientMedicalOrderRecords.filter(
      (record) => record.encounterId === data.encounterId
    )
    inpatientMedicalOrderRecords.push({
      id: `ipt-order-${nextMedicalOrderId++}`,
      encounterId: data.encounterId,
      patientId: data.patientId,
      catalogId: 'system-discharge-advice',
      code: 'DISCHARGE',
      name: `出院（${outWay.label}）`,
      category: 'TREATMENT',
      categoryName: '出院医嘱',
      quantity: 1,
      quantityUnit: '次',
      remark: [data.plannedDischargeTime, data.reasonText?.trim()].filter(Boolean).join(' · '),
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

    patient.encounterStatus = 'DISCHARGE_PENDING'
    patient.encounterStatusName = '待出院'
    patient.version += 1
    return delay({ ...advice })
  } catch (error) {
    return delayReject(error instanceof Error ? error.message : '开立出院医嘱失败')
  }
}
