import { createPageDTO, type PageDTO } from '../../type'
import type {
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorDiagnosisVO,
  DoctorMedicalRecordTemplateNodeVO,
  DoctorMedicalRecordTemplateVO,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorDiagnosesDTO,
  SaveDoctorMedicalRecordDTO
} from './type'

/**
 * 模拟患者队列。
 *
 * 模拟数据只放在 API 模块中，页面仍然像调用真实后端一样调用 index.ts。
 * 将来接入后端时，只需要替换 index.ts 的实现，不需要修改页面调用方式。
 */
const patientQueue: DoctorPatientQueueItemVO[] = [
  {
    encounterId: 'enc-20260723-001',
    patientId: 'patient-1001',
    patientName: '张建国',
    genderName: '男',
    age: 56,
    patientNo: 'MR20260001',
    visitNo: 'OP202607230001',
    queueNumber: 1,
    registeredAt: '2026-07-23 08:05',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'WAITING',
    visitStatusName: '候诊',
    paymentTypeName: '职工医保',
    riskHint: '青霉素过敏'
  },
  {
    encounterId: 'enc-20260723-002',
    patientId: 'patient-1002',
    patientName: '李晓梅',
    genderName: '女',
    age: 42,
    patientNo: 'MR20260002',
    visitNo: 'OP202607230002',
    queueNumber: 2,
    registeredAt: '2026-07-23 08:18',
    departmentName: '心血管内科',
    visitTypeName: '专家门诊',
    visitStatus: 'WAITING',
    visitStatusName: '候诊',
    paymentTypeName: '居民医保',
    priorityName: '优先'
  },
  {
    encounterId: 'enc-20260723-003',
    patientId: 'patient-1003',
    patientName: '王志强',
    genderName: '男',
    age: 67,
    patientNo: 'MR20260003',
    visitNo: 'OP202607230003',
    queueNumber: 3,
    registeredAt: '2026-07-23 08:32',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'IN_PROGRESS',
    visitStatusName: '在诊',
    paymentTypeName: '自费',
    riskHint: '高血压病史'
  },
  {
    encounterId: 'enc-20260723-004',
    patientId: 'patient-1004',
    patientName: '赵敏',
    genderName: '女',
    age: 35,
    patientNo: 'MR20260004',
    visitNo: 'OP202607230004',
    queueNumber: 4,
    registeredAt: '2026-07-23 09:06',
    departmentName: '心血管内科',
    visitTypeName: '复诊',
    visitStatus: 'WAITING',
    visitStatusName: '候诊',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260723-005',
    patientId: 'patient-1005',
    patientName: '陈晨',
    genderName: '男',
    age: 29,
    patientNo: 'MR20260005',
    visitNo: 'OP202607230005',
    queueNumber: 5,
    registeredAt: '2026-07-23 09:25',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'COMPLETED',
    visitStatusName: '完诊',
    paymentTypeName: '居民医保'
  },
  {
    encounterId: 'enc-20260722-006',
    patientId: 'patient-1006',
    patientName: '周淑芬',
    genderName: '女',
    age: 61,
    patientNo: 'MR20260006',
    visitNo: 'OP202607220006',
    queueNumber: 6,
    registeredAt: '2026-07-22 15:10',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'COMPLETED',
    visitStatusName: '完诊',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260723-007',
    patientId: 'patient-1007',
    patientName: '孙丽华',
    genderName: '女',
    age: 48,
    patientNo: 'MR20260007',
    visitNo: 'OP202607230007',
    queueNumber: 7,
    registeredAt: '2026-07-23 09:40',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'WAITING',
    visitStatusName: '候诊',
    paymentTypeName: '居民医保',
    riskHint: '磺胺类药物过敏'
  },
  {
    encounterId: 'enc-20260723-008',
    patientId: 'patient-1008',
    patientName: '吴国平',
    genderName: '男',
    age: 53,
    patientNo: 'MR20260008',
    visitNo: 'OP202607230008',
    queueNumber: 8,
    registeredAt: '2026-07-23 09:50',
    departmentName: '心血管内科',
    visitTypeName: '专家门诊',
    visitStatus: 'IN_PROGRESS',
    visitStatusName: '在诊',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260723-009',
    patientId: 'patient-1009',
    patientName: '郑秀兰',
    genderName: '女',
    age: 64,
    patientNo: 'MR20260009',
    visitNo: 'OP202607230009',
    queueNumber: 9,
    registeredAt: '2026-07-23 10:05',
    departmentName: '心血管内科',
    visitTypeName: '复诊',
    visitStatus: 'IN_PROGRESS',
    visitStatusName: '在诊',
    paymentTypeName: '居民医保',
    riskHint: '冠心病复诊'
  },
  {
    encounterId: 'enc-20260723-010',
    patientId: 'patient-1010',
    patientName: '马海峰',
    genderName: '男',
    age: 45,
    patientNo: 'MR20260010',
    visitNo: 'OP202607230010',
    queueNumber: 10,
    registeredAt: '2026-07-23 10:15',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'IN_PROGRESS',
    visitStatusName: '在诊',
    paymentTypeName: '自费'
  },
  {
    encounterId: 'enc-20260723-011',
    patientId: 'patient-1011',
    patientName: '何芳',
    genderName: '女',
    age: 38,
    patientNo: 'MR20260011',
    visitNo: 'OP202607230011',
    queueNumber: 11,
    registeredAt: '2026-07-23 07:50',
    departmentName: '心血管内科',
    visitTypeName: '普通门诊',
    visitStatus: 'COMPLETED',
    visitStatusName: '完诊',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260722-012',
    patientId: 'patient-1012',
    patientName: '高玉明',
    genderName: '男',
    age: 70,
    patientNo: 'MR20260012',
    visitNo: 'OP202607220012',
    queueNumber: 12,
    registeredAt: '2026-07-22 14:20',
    departmentName: '心血管内科',
    visitTypeName: '专家门诊',
    visitStatus: 'COMPLETED',
    visitStatusName: '完诊',
    paymentTypeName: '居民医保',
    priorityName: '高龄'
  }
]

/**
 * 患者上下文中的模拟业务资料。
 * 未单独配置的患者会使用 buildPatientContext 中的默认内容。
 */
const patientContextDetails: Record<
  string,
  Pick<
    DoctorPatientContextVO,
    'chiefComplaint' | 'allergyHistory' | 'pastMedicalHistory' | 'phone' | 'emergencyContact'
  > &
    Pick<DoctorPatientContextVO, 'triageNote'>
> = {
  'enc-20260723-001': {
    chiefComplaint: '反复胸闷、心悸一周',
    allergyHistory: '青霉素过敏',
    pastMedicalHistory: '高血压 8 年',
    phone: '138****1001',
    emergencyContact: '张晓军 139****2001',
    triageNote: '静息后症状有所缓解'
  },
  'enc-20260723-002': {
    chiefComplaint: '活动后气短伴乏力三天',
    allergyHistory: '未发现明确药物过敏',
    pastMedicalHistory: '无特殊病史',
    phone: '138****1002',
    emergencyContact: '李明 139****2002',
    triageNote: '专家门诊预约患者'
  },
  'enc-20260723-003': {
    chiefComplaint: '血压控制不佳两周',
    allergyHistory: '未发现明确药物过敏',
    pastMedicalHistory: '高血压、冠心病',
    phone: '138****1003',
    emergencyContact: '王丽 139****2003',
    triageNote: '已完成分诊测压'
  },
  'enc-20260723-009': {
    chiefComplaint: '冠心病出院后复诊',
    allergyHistory: '头孢类药物过敏',
    pastMedicalHistory: '冠心病、血脂异常',
    phone: '138****1009',
    emergencyContact: '郑伟 139****2009',
    triageNote: '携带外院检查报告'
  }
}

/**
 * 接诊时间单独保存，便于模拟接诊操作后更新上下文。
 */
const acceptedAtByEncounterId: Record<string, string | undefined> = Object.fromEntries(
  patientQueue.map((patient) => [
    patient.encounterId,
    patient.visitStatus === 'WAITING' ? undefined : patient.registeredAt
  ])
)

/**
 * 已保存的病历 Mock。
 *
 * 候诊患者没有初始病历；在诊和完诊患者准备了少量示例内容，
 * 这样可以同时验证“新建病历”“继续编辑”和“完诊只读”三种状态。
 */
const medicalRecordByEncounterId: Record<string, DoctorMedicalRecordVO> = {
  'enc-20260723-003': {
    id: 'record-003',
    encounterId: 'enc-20260723-003',
    content:
      '<h3>主诉</h3><p>血压控制不佳两周。</p><h3>现病史</h3><p>近两周家庭监测血压偏高，偶有头晕，无胸痛、呼吸困难。</p><h3>查体</h3><p>神志清楚，心肺听诊未见明显异常。</p>',
    version: 1,
    updatedAt: '2026-07-23 08:45:00'
  },
  'enc-20260723-008': {
    id: 'record-008',
    encounterId: 'enc-20260723-008',
    content:
      '<h3>主诉</h3><p>患者到诊，主诉信息待医生补充。</p><h3>现病史</h3><p>请在此补充本次发病经过、伴随症状及诊疗经过。</p><h3>查体</h3><p>请在此补充查体结果。</p>',
    version: 1,
    updatedAt: '2026-07-23 09:55:00'
  },
  'enc-20260723-009': {
    id: 'record-009',
    encounterId: 'enc-20260723-009',
    content:
      '<h3>主诉</h3><p>冠心病出院后复诊。</p><h3>现病史</h3><p>出院后规律服药，无明显胸痛，活动耐量较前改善。</p><h3>处理意见</h3><p>继续随访血压、心率和血脂。</p>',
    version: 2,
    updatedAt: '2026-07-23 10:12:00'
  },
  'enc-20260723-005': {
    id: 'record-005',
    encounterId: 'enc-20260723-005',
    content:
      '<h3>主诉</h3><p>间断心悸三天。</p><h3>现病史</h3><p>无晕厥及胸痛，休息后可缓解。</p><h3>处理意见</h3><p>已完成本次诊疗，按医嘱复诊。</p>',
    version: 3,
    updatedAt: '2026-07-23 09:40:00'
  }
}

/**
 * 病历模板树只返回目录和模板摘要，模板正文通过详情接口按需加载。
 */
const medicalRecordTemplateTree: DoctorMedicalRecordTemplateNodeVO[] = [
  {
    id: 'category-common',
    name: '通用模板',
    type: 'CATEGORY',
    isLeaf: false,
    children: [
      {
        id: 'template-first-visit',
        name: '初诊病历',
        type: 'TEMPLATE',
        isLeaf: true
      },
      {
        id: 'template-follow-up',
        name: '复诊病历',
        type: 'TEMPLATE',
        isLeaf: true
      },
      {
        id: 'template-simple',
        name: '简要病历',
        type: 'TEMPLATE',
        isLeaf: true
      }
    ]
  },
  {
    id: 'category-cardiology',
    name: '心血管内科',
    type: 'CATEGORY',
    isLeaf: false,
    children: [
      {
        id: 'template-hypertension',
        name: '高血压随访',
        type: 'TEMPLATE',
        isLeaf: true
      },
      {
        id: 'template-chest-pain',
        name: '胸痛就诊',
        type: 'TEMPLATE',
        isLeaf: true
      },
      {
        id: 'template-palpitations',
        name: '心悸就诊',
        type: 'TEMPLATE',
        isLeaf: true
      }
    ]
  }
]

/** 模板正文，key 与模板树叶子节点的 id 保持一致。 */
const medicalRecordTemplateDetails: Record<string, DoctorMedicalRecordTemplateVO> = {
  'template-first-visit': {
    id: 'template-first-visit',
    name: '初诊病历',
    content:
      '<h3>主诉</h3><p><br></p><h3>现病史</h3><p><br></p><h3>既往史</h3><p><br></p><h3>个人史</h3><p><br></p><h3>查体</h3><p><br></p><h3>处理意见</h3><p><br></p>'
  },
  'template-follow-up': {
    id: 'template-follow-up',
    name: '复诊病历',
    content:
      '<h3>复诊原因</h3><p><br></p><h3>治疗后变化</h3><p><br></p><h3>复查结果</h3><p><br></p><h3>后续计划</h3><p><br></p>'
  },
  'template-simple': {
    id: 'template-simple',
    name: '简要病历',
    content: '<h3>主诉</h3><p><br></p><h3>主要检查</h3><p><br></p><h3>诊疗意见</h3><p><br></p>'
  },
  'template-hypertension': {
    id: 'template-hypertension',
    name: '高血压随访',
    content:
      '<h3>主诉</h3><p>高血压随访。</p><h3>用药情况</h3><p><br></p><h3>家庭血压</h3><p>最高：　mmHg；最低：　mmHg。</p><h3>危险因素评估</h3><p><br></p><h3>调整方案</h3><p><br></p>'
  },
  'template-chest-pain': {
    id: 'template-chest-pain',
    name: '胸痛就诊',
    content:
      '<h3>主诉</h3><p>胸痛待查。</p><h3>疼痛特点</h3><p>部位：　性质：　持续时间：　诱因：</p><h3>伴随症状</h3><p><br></p><h3>心电图及检查</h3><p><br></p><h3>处理意见</h3><p><br></p>'
  },
  'template-palpitations': {
    id: 'template-palpitations',
    name: '心悸就诊',
    content:
      '<h3>主诉</h3><p>反复心悸。</p><h3>发作特点</h3><p>频率：　持续时间：　诱因：</p><h3>伴随症状</h3><p><br></p><h3>既往检查</h3><p><br></p><h3>处理意见</h3><p><br></p>'
  }
}

/**
 * ICD 诊断字典 Mock。
 *
 * 公共诊断组件不会直接导入这些数据，而是通过页面注入的分页加载函数访问。
 */
const diagnosisDictionary: DoctorDiagnosisDictionaryItemVO[] = [
  { id: 'diag-I10', code: 'I10', name: '原发性高血压', typeName: '西医诊断' },
  {
    id: 'diag-I251',
    code: 'I25.1',
    name: '冠状动脉粥样硬化性心脏病',
    typeName: '西医诊断'
  },
  { id: 'diag-I499', code: 'I49.9', name: '心律失常', typeName: '西医诊断' },
  { id: 'diag-I489', code: 'I48.9', name: '心房颤动和心房扑动', typeName: '西医诊断' },
  { id: 'diag-R074', code: 'R07.4', name: '胸痛', typeName: '症状诊断' },
  { id: 'diag-R002', code: 'R00.2', name: '心悸', typeName: '症状诊断' },
  { id: 'diag-I509', code: 'I50.9', name: '心力衰竭', typeName: '西医诊断' },
  { id: 'diag-E785', code: 'E78.5', name: '高脂血症', typeName: '西医诊断' },
  { id: 'diag-I209', code: 'I20.9', name: '心绞痛', typeName: '西医诊断' },
  { id: 'diag-I471', code: 'I47.1', name: '室上性心动过速', typeName: '西医诊断' },
  { id: 'diag-I340', code: 'I34.0', name: '二尖瓣关闭不全', typeName: '西医诊断' },
  { id: 'diag-I429', code: 'I42.9', name: '心肌病', typeName: '西医诊断' }
]

/** 构造一条已保存诊断，减少下面 Mock 初始化的重复字段。 */
function createSavedDiagnosis(
  encounterId: string,
  item: DoctorDiagnosisDictionaryItemVO,
  primary: boolean,
  orderNo: number
): DoctorDiagnosisVO {
  return {
    id: 'record-' + encounterId + '-' + item.id,
    encounterId,
    catalogId: item.id,
    code: item.code,
    name: item.name,
    typeName: item.typeName,
    primary,
    diagnosedAt: '2026-07-23 10:20:00',
    orderNo
  }
}

/**
 * 已保存诊断及版本 Mock。
 * 分别覆盖在诊多诊断、在诊单诊断和完诊只读场景。
 */
const diagnosisListByEncounterId: Record<string, DoctorDiagnosisListVO> = {
  'enc-20260723-003': {
    encounterId: 'enc-20260723-003',
    rows: [createSavedDiagnosis('enc-20260723-003', diagnosisDictionary[0]!, true, 1)],
    version: 1,
    updatedAt: '2026-07-23 08:50:00'
  },
  'enc-20260723-008': {
    encounterId: 'enc-20260723-008',
    rows: [
      createSavedDiagnosis('enc-20260723-008', diagnosisDictionary[1]!, true, 1),
      createSavedDiagnosis('enc-20260723-008', diagnosisDictionary[7]!, false, 2)
    ],
    version: 2,
    updatedAt: '2026-07-23 10:20:00'
  },
  'enc-20260723-009': {
    encounterId: 'enc-20260723-009',
    rows: [createSavedDiagnosis('enc-20260723-009', diagnosisDictionary[1]!, true, 1)],
    version: 1,
    updatedAt: '2026-07-23 10:15:00'
  },
  'enc-20260723-005': {
    encounterId: 'enc-20260723-005',
    rows: [createSavedDiagnosis('enc-20260723-005', diagnosisDictionary[2]!, true, 1)],
    version: 1,
    updatedAt: '2026-07-23 09:38:00'
  }
}

/** 门诊医嘱项目目录 Mock，覆盖药品、检验、检查和治疗。 */
const adviceOrderCatalog: DoctorAdviceOrderCatalogItemVO[] = [
  {
    id: 'order-drug-amlodipine',
    code: 'YP0001',
    name: '苯磺酸氨氯地平片',
    category: 'DRUG',
    categoryName: '西药',
    specification: '5mg×14片',
    defaultDose: 5,
    defaultUnit: 'mg',
    defaultFrequency: '每日一次',
    defaultRoute: '口服',
    defaultDays: 7,
    defaultQuantity: 7,
    quantityUnit: '片'
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
    quantityUnit: '片'
  },
  {
    id: 'order-drug-atorvastatin',
    code: 'YP0003',
    name: '阿托伐他汀钙片',
    category: 'DRUG',
    categoryName: '西药',
    specification: '20mg×7片',
    defaultDose: 20,
    defaultUnit: 'mg',
    defaultFrequency: '每晚一次',
    defaultRoute: '口服',
    defaultDays: 7,
    defaultQuantity: 7,
    quantityUnit: '片'
  },
  {
    id: 'order-drug-aspirin',
    code: 'YP0004',
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
    quantityUnit: '片'
  },
  {
    id: 'order-lab-blood',
    code: 'JY0001',
    name: '血常规',
    category: 'LAB',
    categoryName: '检验',
    defaultQuantity: 1,
    quantityUnit: '次'
  },
  {
    id: 'order-lab-lipid',
    code: 'JY0002',
    name: '血脂四项',
    category: 'LAB',
    categoryName: '检验',
    defaultQuantity: 1,
    quantityUnit: '次'
  },
  {
    id: 'order-exam-ecg',
    code: 'JC0001',
    name: '十二导联心电图',
    category: 'EXAM',
    categoryName: '检查',
    defaultQuantity: 1,
    quantityUnit: '次'
  },
  {
    id: 'order-exam-echo',
    code: 'JC0002',
    name: '心脏彩色多普勒超声',
    category: 'EXAM',
    categoryName: '检查',
    defaultQuantity: 1,
    quantityUnit: '次'
  },
  {
    id: 'order-treatment-oxygen',
    code: 'ZL0001',
    name: '低流量吸氧',
    category: 'TREATMENT',
    categoryName: '治疗',
    defaultQuantity: 1,
    quantityUnit: '次'
  },
  {
    id: 'order-treatment-monitor',
    code: 'ZL0002',
    name: '动态血压监测',
    category: 'TREATMENT',
    categoryName: '治疗',
    defaultQuantity: 1,
    quantityUnit: '次'
  }
]

/** 将目录项目转换成一条已保存医嘱。 */
function createSavedAdviceOrder(
  encounterId: string,
  item: DoctorAdviceOrderCatalogItemVO,
  orderNo: number
): DoctorAdviceOrderVO {
  return {
    id: 'advice-' + encounterId + '-' + item.id,
    encounterId,
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
    quantity: item.defaultQuantity,
    quantityUnit: item.quantityUnit,
    status: 'SAVED',
    orderNo
  }
}

/** 已保存门诊医嘱，覆盖在诊和完诊只读场景。 */
const adviceOrderListByEncounterId: Record<string, DoctorAdviceOrderListVO> = {
  'enc-20260723-003': {
    encounterId: 'enc-20260723-003',
    rows: [
      createSavedAdviceOrder('enc-20260723-003', adviceOrderCatalog[0]!, 1),
      createSavedAdviceOrder('enc-20260723-003', adviceOrderCatalog[4]!, 2)
    ],
    version: 1,
    updatedAt: '2026-07-23 08:55:00'
  },
  'enc-20260723-008': {
    encounterId: 'enc-20260723-008',
    rows: [
      createSavedAdviceOrder('enc-20260723-008', adviceOrderCatalog[3]!, 1),
      createSavedAdviceOrder('enc-20260723-008', adviceOrderCatalog[6]!, 2)
    ],
    version: 2,
    updatedAt: '2026-07-23 10:25:00'
  },
  'enc-20260723-009': {
    encounterId: 'enc-20260723-009',
    rows: [
      createSavedAdviceOrder('enc-20260723-009', adviceOrderCatalog[2]!, 1),
      createSavedAdviceOrder('enc-20260723-009', adviceOrderCatalog[5]!, 2)
    ],
    version: 1,
    updatedAt: '2026-07-23 10:18:00'
  },
  'enc-20260723-005': {
    encounterId: 'enc-20260723-005',
    rows: [createSavedAdviceOrder('enc-20260723-005', adviceOrderCatalog[6]!, 1)],
    version: 1,
    updatedAt: '2026-07-23 09:42:00'
  }
}

/**
 * 制造接近真实网络请求的短暂延迟。
 * 搜索词越短延迟越长，便于验证“旧请求不能覆盖新请求”的保护逻辑。
 */
function waitForMockRequest(keyword = '') {
  const delay = Math.max(180, 620 - keyword.length * 80)
  return new Promise<void>((resolve) => window.setTimeout(resolve, delay))
}

function findPatientOrThrow(encounterId: string) {
  const patient = patientQueue.find((item) => item.encounterId === encounterId)
  if (!patient) throw new Error('未找到当前患者的就诊记录')
  return patient
}

function buildPatientContext(patient: DoctorPatientQueueItemVO): DoctorPatientContextVO {
  const details = patientContextDetails[patient.encounterId]

  return {
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    visitStatus: patient.visitStatus,
    visitStatusName: patient.visitStatusName,
    attendingDoctorName: patient.visitStatus === 'WAITING' ? undefined : '李尔王',
    acceptedAt: acceptedAtByEncounterId[patient.encounterId],
    chiefComplaint: details?.chiefComplaint ?? '患者到诊，主诉信息待医生补充',
    allergyHistory: details?.allergyHistory ?? patient.riskHint ?? '未发现明确药物过敏',
    pastMedicalHistory: details?.pastMedicalHistory ?? '既往病史待进一步询问',
    phone: details?.phone ?? '138****0000',
    emergencyContact: details?.emergencyContact ?? '家属 139****0000',
    triageNote: details?.triageNote
  }
}

/**
 * 模拟分页查询医生患者队列。
 *
 * 输入关键词“失败”会主动抛出错误，方便开发阶段检查失败提示和刷新重试。
 */
export async function mockPageDoctorPatientQueue(
  query: DoctorPatientQueueQuery
): Promise<PageDTO<DoctorPatientQueueItemVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword)

  if (keyword === '失败') {
    throw new Error('模拟患者队列加载失败')
  }

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = patientQueue.filter((patient) => {
    const visitDate = patient.registeredAt.slice(0, 10)
    const matchesKeyword =
      !normalizedKeyword ||
      [patient.patientName, patient.patientNo, patient.visitNo]
        .join(' ')
        .toLocaleLowerCase()
        .includes(normalizedKeyword)
    const matchesStartDate = !query.startDate || visitDate >= query.startDate
    const matchesEndDate = !query.endDate || visitDate <= query.endDate
    const matchesVisitStatus = !query.visitStatus || patient.visitStatus === query.visitStatus

    return matchesKeyword && matchesStartDate && matchesEndDate && matchesVisitStatus
  })

  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize

  return createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize)
  })
}

/**
 * 模拟加载单个患者的就诊上下文。
 */
export async function mockGetPatientContext(encounterId: string): Promise<DoctorPatientContextVO> {
  await waitForMockRequest(encounterId)
  return buildPatientContext(findPatientOrThrow(encounterId))
}

/**
 * 模拟接诊。
 *
 * 接诊操作具备幂等性：已经在诊的患者再次提交接诊时直接返回当前上下文；
 * 已完诊患者不允许重新接诊。
 */
export async function mockAcceptPatient(encounterId: string): Promise<DoctorPatientContextVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus === 'COMPLETED') {
    throw new Error('该患者已经完诊，不能重新接诊')
  }

  if (patient.visitStatus === 'WAITING') {
    patient.visitStatus = 'IN_PROGRESS'
    patient.visitStatusName = '在诊'
    acceptedAtByEncounterId[encounterId] = '2026-07-23 10:30'
  }

  return buildPatientContext(patient)
}

/**
 * 模拟获取当前就诊病历。
 *
 * 后端没有记录时仍返回统一结构，页面不需要用 null 区分“新建”和“编辑”。
 */
export async function mockGetMedicalRecord(encounterId: string): Promise<DoctorMedicalRecordVO> {
  await waitForMockRequest(encounterId)
  findPatientOrThrow(encounterId)

  const savedRecord = medicalRecordByEncounterId[encounterId]
  return savedRecord
    ? { ...savedRecord }
    : {
        encounterId,
        content: '',
        version: 0
      }
}

/**
 * 模拟保存病历。
 *
 * 只有在诊患者允许保存；version 不一致时拒绝覆盖，模拟真实后端的乐观锁。
 */
export async function mockSaveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus !== 'IN_PROGRESS') {
    throw new Error(
      patient.visitStatus === 'WAITING' ? '请先接诊患者再保存病历' : '患者已完诊，病历不可修改'
    )
  }

  const currentRecord = medicalRecordByEncounterId[encounterId]
  const currentVersion = currentRecord?.version ?? 0
  if (data.version !== currentVersion) {
    throw new Error('病历已被其他操作修改，请刷新后重试')
  }

  const savedRecord: DoctorMedicalRecordVO = {
    id: currentRecord?.id ?? `record-${encounterId}`,
    encounterId,
    content: data.content,
    version: currentVersion + 1,
    updatedAt: '2026-07-23 10:35:00'
  }
  medicalRecordByEncounterId[encounterId] = savedRecord

  return { ...savedRecord }
}

/** 模拟加载病历模板树，并复制数据以避免页面意外修改 Mock 源数据。 */
export async function mockListMedicalRecordTemplates(): Promise<
  DoctorMedicalRecordTemplateNodeVO[]
> {
  await waitForMockRequest('templates')

  const cloneNode = (
    node: DoctorMedicalRecordTemplateNodeVO
  ): DoctorMedicalRecordTemplateNodeVO => ({
    ...node,
    children: node.children?.map(cloneNode)
  })

  return medicalRecordTemplateTree.map(cloneNode)
}

/** 模拟按模板 ID 加载完整正文。 */
export async function mockGetMedicalRecordTemplate(
  templateId: string
): Promise<DoctorMedicalRecordTemplateVO> {
  await waitForMockRequest(templateId)
  const template = medicalRecordTemplateDetails[templateId]
  if (!template) throw new Error('未找到指定的病历模板')
  return { ...template }
}

/** 模拟获取当前就诊诊断；没有保存记录时返回 version 为 0 的空列表。 */
export async function mockGetDiagnoses(encounterId: string): Promise<DoctorDiagnosisListVO> {
  await waitForMockRequest(encounterId)
  findPatientOrThrow(encounterId)

  const saved = diagnosisListByEncounterId[encounterId]
  return saved
    ? { ...saved, rows: saved.rows.map((item) => ({ ...item })) }
    : { encounterId, rows: [], version: 0 }
}

/** 模拟按 ICD 编码或诊断名称分页搜索诊断字典。 */
export async function mockPageDiagnosisDictionary(
  query: DoctorDiagnosisDictionaryQuery
): Promise<PageDTO<DoctorDiagnosisDictionaryItemVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword)

  if (keyword === '失败') throw new Error('模拟诊断字典加载失败')

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = diagnosisDictionary.filter((item) =>
    [item.code, item.name, item.typeName].join(' ').toLocaleLowerCase().includes(normalizedKeyword)
  )
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize

  return createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize).map((item) => ({ ...item }))
  })
}

/**
 * 模拟保存完整诊断列表。
 *
 * 只有在诊患者可以保存；同时校验版本号和唯一主诊断，
 * 这些规则以后可以直接迁移到真实后端。
 */
export async function mockSaveDiagnoses(
  encounterId: string,
  data: SaveDoctorDiagnosesDTO
): Promise<DoctorDiagnosisListVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus !== 'IN_PROGRESS') {
    throw new Error(
      patient.visitStatus === 'WAITING' ? '请先接诊患者再保存诊断' : '患者已完诊，诊断不可修改'
    )
  }

  const current = diagnosisListByEncounterId[encounterId]
  const currentVersion = current?.version ?? 0
  if (data.version !== currentVersion) {
    throw new Error('诊断已被其他操作修改，请刷新后重试')
  }

  if (data.items.length === 0 || data.items.filter((item) => item.primary).length !== 1) {
    throw new Error('请至少添加一条诊断，并且只能设置一个主诊断')
  }

  const savedAt = '2026-07-23 10:40:00'
  const saved: DoctorDiagnosisListVO = {
    encounterId,
    rows: data.items.map((item, index) => ({
      id: item.id ?? 'diagnosis-' + encounterId + '-' + item.catalogId,
      encounterId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      typeName: item.typeName,
      primary: item.primary,
      diagnosedAt:
        current?.rows.find((oldItem) => oldItem.catalogId === item.catalogId)?.diagnosedAt ??
        savedAt,
      orderNo: index + 1
    })),
    version: currentVersion + 1,
    updatedAt: savedAt
  }

  diagnosisListByEncounterId[encounterId] = saved
  return { ...saved, rows: saved.rows.map((item) => ({ ...item })) }
}

/** 模拟获取当前就诊医嘱；没有保存记录时返回 version 为 0 的空列表。 */
export async function mockGetAdviceOrders(encounterId: string): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(encounterId)
  findPatientOrThrow(encounterId)

  const saved = adviceOrderListByEncounterId[encounterId]
  return saved
    ? { ...saved, rows: saved.rows.map((item) => ({ ...item })) }
    : { encounterId, rows: [], version: 0 }
}

/** 模拟按项目编码、名称或分类分页搜索医嘱项目目录。 */
export async function mockPageAdviceOrderCatalog(
  query: DoctorAdviceOrderCatalogQuery
): Promise<PageDTO<DoctorAdviceOrderCatalogItemVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword)
  if (keyword === '失败') throw new Error('模拟医嘱项目加载失败')

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = adviceOrderCatalog.filter((item) => {
    const matchesKeyword =
      !normalizedKeyword ||
      [item.code, item.name, item.categoryName, item.specification]
        .join(' ')
        .toLocaleLowerCase()
        .includes(normalizedKeyword)
    const matchesCategory = !query.category || item.category === query.category
    return matchesKeyword && matchesCategory
  })
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize

  return createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize).map((item) => ({ ...item }))
  })
}

/** 模拟保存完整门诊医嘱，并执行状态、版本和必填字段校验。 */
export async function mockSaveAdviceOrders(
  encounterId: string,
  data: SaveDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus !== 'IN_PROGRESS') {
    throw new Error(
      patient.visitStatus === 'WAITING' ? '请先接诊患者再保存医嘱' : '患者已完诊，医嘱不可修改'
    )
  }

  const current = adviceOrderListByEncounterId[encounterId]
  const currentVersion = current?.version ?? 0
  if (data.version !== currentVersion) {
    throw new Error('医嘱已被其他操作修改，请刷新后重试')
  }
  if (data.items.length === 0) throw new Error('请至少添加一条医嘱')

  const catalogIds = data.items.map((item) => item.catalogId)
  if (new Set(catalogIds).size !== catalogIds.length) {
    throw new Error('不能重复添加相同医嘱项目')
  }
  for (const item of data.items) {
    if (!item.quantity || item.quantity <= 0) throw new Error(item.name + '：请填写开立数量')
    if (
      item.category === 'DRUG' &&
      (!item.dose || !item.unit || !item.frequency || !item.route || !item.days)
    ) {
      throw new Error(item.name + '：药品剂量、单位、用法、频次和天数必须完整')
    }
  }

  const savedAt = '2026-07-23 10:45:00'
  const saved: DoctorAdviceOrderListVO = {
    encounterId,
    rows: data.items.map((item, index) => ({
      id: item.id ?? 'advice-' + encounterId + '-' + item.catalogId,
      encounterId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      category: item.category,
      categoryName: item.categoryName,
      specification: item.specification,
      dose: item.dose,
      unit: item.unit,
      frequency: item.frequency,
      route: item.route,
      days: item.days,
      quantity: item.quantity,
      quantityUnit: item.quantityUnit,
      remark: item.remark,
      status: 'SAVED',
      orderNo: index + 1
    })),
    version: currentVersion + 1,
    updatedAt: savedAt
  }

  adviceOrderListByEncounterId[encounterId] = saved
  return { ...saved, rows: saved.rows.map((item) => ({ ...item })) }
}
