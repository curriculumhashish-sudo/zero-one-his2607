import dayjs from 'dayjs'

import { createPageDTO, type PageDTO } from '../../type'
import type {
  DoctorAdviceApplicationFormVO,
  PrintDoctorAdviceApplicationFormDTO
} from './application-form/type'
import type {
  CreateDoctorAdviceOrderDTO,
  DeleteDoctorAdviceOrdersDTO,
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderHistoryDetailQuery,
  DoctorAdviceOrderHistoryEncounterVO,
  DoctorAdviceOrderHistoryQuery,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderPageVO,
  DoctorAdviceOrderQuery,
  DoctorAdviceOrderStatus,
  DoctorAdviceOrderVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorDiagnosisVO,
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordHistoryQuery,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientDiagnosisHistoryItemVO,
  DoctorPatientDiagnosisHistoryQuery,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  OpdoctorCreateDiagnosisDTO,
  DoctorVisitStatus,
  GroupDoctorAdviceOrdersDTO,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorMedicalRecordDTO,
  SignDoctorAdviceOrdersDTO,
  UngroupDoctorAdviceOrdersDTO,
  UpdateDoctorAdviceOrderDTO,
  WithdrawDoctorAdviceOrdersDTO
} from './type'

/**
 * 模拟患者队列。
 *
 * 模拟数据只放在 API 模块中，页面仍然像调用真实后端一样调用 index.ts。
 * 将来接入后端时，只需要替换 index.ts 的实现，不需要修改页面调用方式。
 */
const mockToday = dayjs().format('YYYY-MM-DD')
const mockYesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')
const fullDemoEncounterId = 'enc-demo-in-progress'
const fullDemoPatientId = 'patient-demo-001'
const fullDemoPatientNo = 'MR-DEMO-0001'

const basePatientQueue: DoctorPatientQueueItemVO[] = [
  {
    encounterId: fullDemoEncounterId,
    patientId: fullDemoPatientId,
    patientName: '全功能演示患者',
    gender: '男',
    age: 52,
    visitNo: fullDemoPatientNo,
    queueNumber: 0,
    visitTime: `${mockToday} 07:30`,
    departmentName: '心血管内科',
    registerType: '专家门诊',
    visitStatus: 'in-progress',
    paymentTypeName: '职工医保',
    priorityName: '全功能演示'
  },
  {
    encounterId: 'enc-20260723-001',
    patientId: 'patient-1001',
    patientName: '张建国',
    gender: '男',
    age: 56,
    visitNo: 'MR20260001',
    queueNumber: 1,
    visitTime: `${mockToday} 08:05`,
    departmentName: '心血管内科',
    registerType: '普通门诊',
    visitStatus: 'arrived',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260723-005',
    patientId: 'patient-1005',
    patientName: '陈晨',
    gender: '男',
    age: 29,
    visitNo: 'MR20260005',
    queueNumber: 5,
    visitTime: `${mockYesterday} 09:25`,
    departmentName: '心血管内科',
    registerType: '普通门诊',
    visitStatus: 'finished',
    paymentTypeName: '居民医保'
  }
]

/** 仅供接口单测使用的隐藏就诊夹具，不进入浏览器患者队列。 */
const hiddenPatientFixtures: DoctorPatientQueueItemVO[] = [
  {
    encounterId: 'enc-20260723-002',
    patientId: 'patient-1002',
    patientName: '李晓梅',
    gender: '女',
    age: 42,
    visitNo: 'MR20260002',
    queueNumber: 2,
    visitTime: `${mockToday} 08:18`,
    departmentName: '心血管内科',
    registerType: '专家门诊',
    visitStatus: 'arrived',
    paymentTypeName: '居民医保',
    priorityName: '优先'
  },
  {
    encounterId: 'enc-20260723-003',
    patientId: 'patient-1003',
    patientName: '王志强',
    gender: '男',
    age: 67,
    visitNo: 'MR20260003',
    queueNumber: 3,
    visitTime: `${mockToday} 08:32`,
    departmentName: '心血管内科',
    registerType: '普通门诊',
    visitStatus: 'in-progress',
    paymentTypeName: '自费'
  },
  {
    encounterId: 'enc-20260723-008',
    patientId: 'patient-1008',
    patientName: '吴国平',
    gender: '男',
    age: 53,
    visitNo: 'MR20260008',
    queueNumber: 8,
    visitTime: `${mockToday} 09:50`,
    departmentName: '心血管内科',
    registerType: '专家门诊',
    visitStatus: 'in-progress',
    paymentTypeName: '职工医保'
  },
  {
    encounterId: 'enc-20260723-009',
    patientId: 'patient-1009',
    patientName: '郑秀兰',
    gender: '女',
    age: 64,
    visitNo: 'MR20260009',
    queueNumber: 9,
    visitTime: `${mockToday} 10:05`,
    departmentName: '心血管内科',
    registerType: '复诊',
    visitStatus: 'in-progress',
    paymentTypeName: '居民医保'
  },
  {
    encounterId: 'enc-20260723-010',
    patientId: 'patient-1010',
    patientName: '马海峰',
    gender: '男',
    age: 45,
    visitNo: 'MR20260010',
    queueNumber: 10,
    visitTime: `${mockToday} 10:15`,
    departmentName: '心血管内科',
    registerType: '普通门诊',
    visitStatus: 'in-progress',
    paymentTypeName: '自费'
  }
]

/**
 * 仅供 CRUD 单元测试使用的空白在诊就诊。
 *
 * 它们不进入 patientQueue，因此浏览器里的医生患者队列不会显示测试患者；
 * 接口测试仍可在互不干扰的就诊上验证新增、删除和分组。
 */
const apiTestPatientFixtures: DoctorPatientQueueItemVO[] = [14, 17, 20, 30].map((sequence) => ({
  encounterId: `enc-extra-${sequence}`,
  patientId: `patient-extra-${sequence}`,
  patientName: `接口测试患者${sequence}`,
  gender: '男',
  age: 30 + sequence,
  visitNo: `MR-TEST-${sequence}`,
  queueNumber: sequence,
  visitTime: `${mockToday} 11:00`,
  departmentName: '心血管内科',
  registerType: '普通门诊',
  visitStatus: 'in-progress',
  paymentTypeName: '自费'
}))

/** 页面接口只返回真实场景患者；测试夹具仅进入内部就诊注册表。 */
const patientQueue: DoctorPatientQueueItemVO[] = [...basePatientQueue]
const currentEncounterRegistry: DoctorPatientQueueItemVO[] = [
  ...patientQueue,
  ...hiddenPatientFixtures,
  ...apiTestPatientFixtures
]

/**
 * 患者上下文中的模拟业务资料。
 * 未单独配置的患者会使用 buildPatientContext 中的默认内容。
 */
const patientContextDetails: Record<string, Pick<DoctorPatientContextVO, 'phone'>> = {
  [fullDemoEncounterId]: {
    phone: '138****0001'
  },
  'enc-20260723-001': {
    phone: '138****1001'
  },
  'enc-20260723-002': {
    phone: '138****1002'
  },
  'enc-20260723-003': {
    phone: '138****1003'
  },
  'enc-20260723-009': {
    phone: '138****1009'
  }
}

/**
 * 接诊时间单独保存，便于模拟接诊操作后更新上下文。
 */
const acceptedAtByEncounterId: Record<string, string | undefined> = Object.fromEntries(
  currentEncounterRegistry.map((patient) => [
    patient.encounterId,
    patient.visitStatus === 'arrived' ? undefined : patient.visitTime
  ])
)

/**
 * 患者既往已完诊门诊的摘要数据。
 *
 * 这些记录故意不放入 patientQueue：患者队列只表示当前医生需要处理的就诊，
 * 历史病历则按 patientId 跨科室、跨医生查询。
 */
interface MockHistoricalMedicalRecordVisit extends DoctorMedicalRecordHistoryItemVO {
  patientId: string
  recordId: string
  visitNo: string
  visitTypeName: string
  updatedAt: string
}

const historicalMedicalRecordVisits: MockHistoricalMedicalRecordVisit[] = [
  ...Array.from({ length: 12 }, (_, index): MockHistoricalMedicalRecordVisit => {
    const sequence = index + 1
    const departments = ['心血管内科', '全科医学科', '呼吸内科']
    const doctors = ['李尔王', '周医生', '陈医生']
    const diagnoses = ['原发性高血压', '冠状动脉粥样硬化性心脏病', '心悸']
    const visitedAt = dayjs()
      .subtract(sequence, 'month')
      .hour(8 + (index % 4))
      .minute(10 + index)
      .format('YYYY-MM-DD HH:mm')

    return {
      recordId: `history-record-1003-${sequence}`,
      encounterId: `history-enc-1003-${sequence}`,
      patientId: 'patient-1003',
      visitNo: `HOP1003${String(sequence).padStart(3, '0')}`,
      visitTypeName: index % 3 === 0 ? '专家门诊' : '普通门诊',
      deptName: departments[index % departments.length] ?? '心血管内科',
      doctorName: doctors[index % doctors.length] ?? '李尔王',
      diagnosis: diagnoses[index % diagnoses.length],
      visitDate: visitedAt,
      updatedAt: `${visitedAt}:30`
    }
  }),
  {
    recordId: 'history-record-demo-1',
    encounterId: 'history-enc-demo-1',
    patientId: fullDemoPatientId,
    visitNo: 'HOP-DEMO-001',
    visitTypeName: '专家门诊',
    deptName: '心血管内科',
    doctorName: '周医生',
    diagnosis: '原发性高血压',
    visitDate: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:20'),
    updatedAt: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:50:00')
  },
  {
    recordId: 'history-record-demo-2',
    encounterId: 'history-enc-demo-2',
    patientId: fullDemoPatientId,
    visitNo: 'HOP-DEMO-002',
    visitTypeName: '普通门诊',
    deptName: '全科医学科',
    doctorName: '陈医生',
    diagnosis: '心悸',
    visitDate: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:15'),
    updatedAt: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:40:00')
  },
  {
    recordId: 'history-record-demo-3',
    encounterId: 'history-enc-demo-3',
    patientId: fullDemoPatientId,
    visitNo: 'HOP-DEMO-003',
    visitTypeName: '复诊',
    deptName: '内分泌科',
    doctorName: '赵医生',
    diagnosis: '高脂血症',
    visitDate: dayjs().subtract(6, 'month').format('YYYY-MM-DD 14:05'),
    updatedAt: dayjs().subtract(6, 'month').format('YYYY-MM-DD 14:30:00')
  },
  {
    recordId: 'history-record-1008-1',
    encounterId: 'history-enc-1008-1',
    patientId: 'patient-1008',
    visitNo: 'HOP1008001',
    visitTypeName: '普通门诊',
    deptName: '内分泌科',
    doctorName: '赵医生',
    diagnosis: '2 型糖尿病',
    visitDate: dayjs().subtract(45, 'day').format('YYYY-MM-DD 09:20'),
    updatedAt: dayjs().subtract(45, 'day').format('YYYY-MM-DD 09:45:00')
  },
  {
    recordId: 'history-record-1008-2',
    encounterId: 'history-enc-1008-2',
    patientId: 'patient-1008',
    visitNo: 'HOP1008002',
    visitTypeName: '专家门诊',
    deptName: '心血管内科',
    doctorName: '李尔王',
    diagnosis: '高血压',
    visitDate: dayjs().subtract(20, 'day').format('YYYY-MM-DD 10:10'),
    updatedAt: dayjs().subtract(20, 'day').format('YYYY-MM-DD 10:40:00')
  },
  {
    recordId: 'history-record-1009-1',
    encounterId: 'history-enc-1009-1',
    patientId: 'patient-1009',
    visitNo: 'HOP1009001',
    visitTypeName: '复诊',
    deptName: '心血管内科',
    doctorName: '孙医生',
    diagnosis: '冠心病',
    visitDate: dayjs().subtract(2, 'month').format('YYYY-MM-DD 14:15'),
    updatedAt: dayjs().subtract(2, 'month').format('YYYY-MM-DD 14:40:00')
  }
]

/** 历史摘要对应的只读富文本正文。 */
const historicalMedicalRecordByEncounterId: Record<string, DoctorMedicalRecordVO> =
  Object.fromEntries(
    historicalMedicalRecordVisits.map((visit) => [
      visit.encounterId,
      {
        id: visit.recordId,
        encounterId: visit.encounterId,
        content:
          `<h3>主诉</h3><p>${visit.diagnosis ?? '慢性病'}复诊。</p>` +
          `<h3>现病史</h3><p>患者于 ${visit.visitDate} 到 ${visit.deptName} 就诊，近期一般情况稳定。</p>` +
          `<h3>诊断</h3><p>${visit.diagnosis ?? '待明确诊断'}</p>` +
          '<h3>处理意见</h3><p>遵医嘱继续治疗，定期门诊随访。</p>',
        updatedAt: visit.updatedAt
      }
    ])
  )

function toMedicalRecordHistoryItem(
  item: MockHistoricalMedicalRecordVisit
): DoctorMedicalRecordHistoryItemVO {
  return {
    encounterId: item.encounterId,
    visitDate: item.visitDate,
    diagnosis: item.diagnosis,
    doctorName: item.doctorName,
    deptName: item.deptName
  }
}

/**
 * 已保存的病历 Mock。
 *
 * 候诊患者没有初始病历；在诊和完诊患者准备了少量示例内容，
 * 这样可以同时验证“新建病历”“继续编辑”和“完诊只读”三种状态。
 */
const medicalRecordByEncounterId: Record<string, DoctorMedicalRecordVO> = {
  ...historicalMedicalRecordByEncounterId,
  'enc-20260723-001': {
    id: 'record-001',
    encounterId: 'enc-20260723-001',
    content:
      '<h3>主诉</h3><p>近期血压波动，前来复诊。</p>' +
      '<h3>现病史</h3><p>患者自述偶有头晕，无胸痛、呼吸困难。</p>' +
      '<h3>查体</h3><p>神志清楚，心肺听诊未见明显异常。</p>',
    updatedAt: `${mockToday} 08:10:00`
  },
  [fullDemoEncounterId]: {
    id: 'record-demo-current',
    encounterId: fullDemoEncounterId,
    content:
      '<h3>主诉</h3><p>反复胸闷、心悸伴活动后气短两周。</p>' +
      '<h3>现病史</h3><p>患者近两周活动后出现胸闷、心悸，休息后可缓解，无晕厥。家庭血压最高 165/98mmHg。</p>' +
      '<h3>既往史</h3><p>高血压 8 年、高脂血症 3 年；青霉素过敏。</p>' +
      '<h3>查体</h3><p>神志清楚，心率 86 次/分，律齐，双肺呼吸音清。</p>' +
      '<h3>处理意见</h3><p>完善检验及心电图检查，调整降压治疗，必要时办理住院进一步诊治。</p>',
    updatedAt: `${mockToday} 08:05:00`
  },
  'enc-20260723-003': {
    id: 'record-003',
    encounterId: 'enc-20260723-003',
    content:
      '<h3>主诉</h3><p>血压控制不佳两周。</p><h3>现病史</h3><p>近两周家庭监测血压偏高，偶有头晕，无胸痛、呼吸困难。</p><h3>查体</h3><p>神志清楚，心肺听诊未见明显异常。</p>',
    updatedAt: '2026-07-23 08:45:00'
  },
  'enc-20260723-008': {
    id: 'record-008',
    encounterId: 'enc-20260723-008',
    content:
      '<h3>主诉</h3><p>患者到诊，主诉信息待医生补充。</p><h3>现病史</h3><p>请在此补充本次发病经过、伴随症状及诊疗经过。</p><h3>查体</h3><p>请在此补充查体结果。</p>',
    updatedAt: '2026-07-23 09:55:00'
  },
  'enc-20260723-009': {
    id: 'record-009',
    encounterId: 'enc-20260723-009',
    content:
      '<h3>主诉</h3><p>冠心病出院后复诊。</p><h3>现病史</h3><p>出院后规律服药，无明显胸痛，活动耐量较前改善。</p><h3>处理意见</h3><p>继续随访血压、心率和血脂。</p>',
    updatedAt: '2026-07-23 10:12:00'
  },
  'enc-20260723-005': {
    id: 'record-005',
    encounterId: 'enc-20260723-005',
    content:
      '<h3>主诉</h3><p>间断心悸三天。</p><h3>现病史</h3><p>无晕厥及胸痛，休息后可缓解。</p><h3>处理意见</h3><p>已完成本次诊疗，按医嘱复诊。</p>',
    updatedAt: '2026-07-23 09:40:00'
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
 * 已保存诊断 Mock。
 * 分别覆盖在诊多诊断、在诊单诊断和完诊只读场景。
 */
const diagnosisListByEncounterId: Record<string, DoctorDiagnosisListVO> = {
  [fullDemoEncounterId]: {
    encounterId: fullDemoEncounterId,
    rows: [
      createSavedDiagnosis(fullDemoEncounterId, diagnosisDictionary[0]!, true, 1),
      createSavedDiagnosis(fullDemoEncounterId, diagnosisDictionary[7]!, false, 2)
    ],
    updatedAt: `${mockToday} 08:10:00`
  },
  'enc-20260723-003': {
    encounterId: 'enc-20260723-003',
    rows: [createSavedDiagnosis('enc-20260723-003', diagnosisDictionary[0]!, true, 1)],
    updatedAt: '2026-07-23 08:50:00'
  },
  'enc-20260723-008': {
    encounterId: 'enc-20260723-008',
    rows: [
      createSavedDiagnosis('enc-20260723-008', diagnosisDictionary[1]!, true, 1),
      createSavedDiagnosis('enc-20260723-008', diagnosisDictionary[7]!, false, 2)
    ],
    updatedAt: '2026-07-23 10:20:00'
  },
  'enc-20260723-009': {
    encounterId: 'enc-20260723-009',
    rows: [createSavedDiagnosis('enc-20260723-009', diagnosisDictionary[1]!, true, 1)],
    updatedAt: '2026-07-23 10:15:00'
  },
  'enc-20260723-005': {
    encounterId: 'enc-20260723-005',
    rows: [createSavedDiagnosis('enc-20260723-005', diagnosisDictionary[2]!, true, 1)],
    updatedAt: '2026-07-23 09:38:00'
  }
}

/** 根据历史病历生成跨科室、跨医生的患者历史诊断 Mock。 */
type MockPatientDiagnosisHistoryItem = DoctorPatientDiagnosisHistoryItemVO & {
  patientId: string
  visitNo: string
  departmentName: string
  attendingDoctorName: string
  visitedAt: string
}

const historicalPatientDiagnoses: MockPatientDiagnosisHistoryItem[] =
  historicalMedicalRecordVisits.flatMap((visit, visitIndex) => {
    const primaryCatalog =
      diagnosisDictionary.find((item) => item.name === visit.diagnosis) ??
      diagnosisDictionary[visitIndex % diagnosisDictionary.length]!
    const primary: MockPatientDiagnosisHistoryItem = {
      ...createSavedDiagnosis(visit.encounterId, primaryCatalog, true, 1),
      id: `history-diagnosis-${visit.encounterId}-1`,
      patientId: visit.patientId,
      visitNo: visit.visitNo,
      departmentName: visit.deptName ?? '',
      attendingDoctorName: visit.doctorName ?? '',
      visitedAt: visit.visitDate,
      diagnosedAt: `${visit.visitDate}:00`
    }
    if (visitIndex % 2 !== 0) return [primary]

    const secondaryCatalog = diagnosisDictionary[(visitIndex + 5) % diagnosisDictionary.length]!
    return [
      primary,
      {
        ...createSavedDiagnosis(visit.encounterId, secondaryCatalog, false, 2),
        id: `history-diagnosis-${visit.encounterId}-2`,
        patientId: visit.patientId,
        visitNo: visit.visitNo,
        departmentName: visit.deptName ?? '',
        attendingDoctorName: visit.doctorName ?? '',
        visitedAt: visit.visitDate,
        diagnosedAt: `${visit.visitDate}:00`
      }
    ]
  })

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
    quantityUnit: '片',
    dosePerQuantityUnit: 5
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
    dosePerQuantityUnit: 25
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
    quantityUnit: '片',
    dosePerQuantityUnit: 20
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
    quantityUnit: '片',
    dosePerQuantityUnit: 100
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
  orderNo: number,
  status: DoctorAdviceOrderStatus = 'DRAFT',
  groupId?: string
): DoctorAdviceOrderVO {
  const isFormal = status !== 'DRAFT'
  const isDrug = item.category === 'DRUG'
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
    dosePerQuantityUnit: item.dosePerQuantityUnit,
    status,
    groupId,
    prescriptionNo: isFormal && isDrug ? `RX-${encounterId}-${orderNo}` : undefined,
    serviceRequestId: isFormal && !isDrug ? `SR-${encounterId}-${orderNo}` : undefined,
    orderNo
  }
}

/** 已保存门诊医嘱，覆盖在诊和完诊只读场景。 */
const adviceOrderListByEncounterId: Record<string, DoctorAdviceOrderListVO> = {
  [fullDemoEncounterId]: {
    encounterId: fullDemoEncounterId,
    rows: [
      createSavedAdviceOrder(
        fullDemoEncounterId,
        adviceOrderCatalog[0]!,
        1,
        'DRAFT',
        'group-demo-draft'
      ),
      createSavedAdviceOrder(
        fullDemoEncounterId,
        adviceOrderCatalog[8]!,
        2,
        'DRAFT',
        'group-demo-draft'
      ),
      createSavedAdviceOrder(fullDemoEncounterId, adviceOrderCatalog[3]!, 3, 'SIGNED'),
      createSavedAdviceOrder(fullDemoEncounterId, adviceOrderCatalog[4]!, 4, 'SIGNED'),
      createSavedAdviceOrder(fullDemoEncounterId, adviceOrderCatalog[6]!, 5, 'SIGNED'),
      createSavedAdviceOrder(fullDemoEncounterId, adviceOrderCatalog[9]!, 6, 'WITHDRAWN')
    ],
    updatedAt: `${mockToday} 08:15:00`
  },
  'enc-20260723-003': {
    encounterId: 'enc-20260723-003',
    rows: [
      createSavedAdviceOrder(
        'enc-20260723-003',
        adviceOrderCatalog[0]!,
        1,
        'DRAFT',
        'group-mixed-003'
      ),
      createSavedAdviceOrder(
        'enc-20260723-003',
        adviceOrderCatalog[4]!,
        2,
        'DRAFT',
        'group-mixed-003'
      )
    ],
    updatedAt: '2026-07-23 08:55:00'
  },
  'enc-20260723-008': {
    encounterId: 'enc-20260723-008',
    rows: [
      createSavedAdviceOrder('enc-20260723-008', adviceOrderCatalog[3]!, 1, 'SIGNED'),
      createSavedAdviceOrder('enc-20260723-008', adviceOrderCatalog[6]!, 2, 'DRAFT')
    ],
    updatedAt: '2026-07-23 10:25:00'
  },
  'enc-20260723-009': {
    encounterId: 'enc-20260723-009',
    rows: [
      createSavedAdviceOrder('enc-20260723-009', adviceOrderCatalog[2]!, 1, 'SIGNED'),
      createSavedAdviceOrder('enc-20260723-009', adviceOrderCatalog[5]!, 2, 'WITHDRAWN')
    ],
    updatedAt: '2026-07-23 10:18:00'
  },
  'enc-20260723-005': {
    encounterId: 'enc-20260723-005',
    rows: [createSavedAdviceOrder('enc-20260723-005', adviceOrderCatalog[6]!, 1, 'SIGNED')],
    updatedAt: '2026-07-23 09:42:00'
  }
}

/** 历史医嘱就诊摘要，不进入当前医生患者队列。 */
const historicalAdviceEncounters: (DoctorAdviceOrderHistoryEncounterVO & {
  patientId: string
})[] = [
  {
    encounterId: 'history-advice-enc-demo-1',
    patientId: fullDemoPatientId,
    visitNo: 'HOP-DEMO-001',
    departmentName: '心血管内科',
    attendingDoctorName: '周医生',
    visitedAt: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:20'),
    signedCount: 3,
    withdrawnCount: 1
  },
  {
    encounterId: 'history-advice-enc-demo-2',
    patientId: fullDemoPatientId,
    visitNo: 'HOP-DEMO-002',
    departmentName: '全科医学科',
    attendingDoctorName: '陈医生',
    visitedAt: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:15'),
    signedCount: 2,
    withdrawnCount: 0
  },
  {
    encounterId: 'history-advice-enc-1003-1',
    patientId: 'patient-1003',
    visitNo: 'HOP-1003-001',
    departmentName: '心血管内科',
    attendingDoctorName: '周医生',
    visitedAt: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:10'),
    signedCount: 2,
    withdrawnCount: 1
  },
  {
    encounterId: 'history-advice-enc-1003-2',
    patientId: 'patient-1003',
    visitNo: 'HOP-1003-002',
    departmentName: '全科医学科',
    attendingDoctorName: '陈医生',
    visitedAt: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:20'),
    signedCount: 2,
    withdrawnCount: 0
  },
  {
    encounterId: 'history-advice-enc-1008-1',
    patientId: 'patient-1008',
    visitNo: 'HOP-1008-001',
    departmentName: '内分泌科',
    attendingDoctorName: '赵医生',
    visitedAt: dayjs().subtract(2, 'month').format('YYYY-MM-DD 14:30'),
    signedCount: 1,
    withdrawnCount: 1
  }
]

/** 历史详情只保存已经签发或撤回的医嘱。 */
const historicalAdviceOrderListByEncounterId: Record<string, DoctorAdviceOrderVO[]> = {
  'history-advice-enc-demo-1': [
    createSavedAdviceOrder(
      'history-advice-enc-demo-1',
      adviceOrderCatalog[0]!,
      1,
      'SIGNED',
      'history-group-demo-1'
    ),
    createSavedAdviceOrder(
      'history-advice-enc-demo-1',
      adviceOrderCatalog[1]!,
      2,
      'SIGNED',
      'history-group-demo-1'
    ),
    createSavedAdviceOrder('history-advice-enc-demo-1', adviceOrderCatalog[4]!, 3, 'SIGNED'),
    createSavedAdviceOrder('history-advice-enc-demo-1', adviceOrderCatalog[8]!, 4, 'WITHDRAWN')
  ],
  'history-advice-enc-demo-2': [
    createSavedAdviceOrder('history-advice-enc-demo-2', adviceOrderCatalog[2]!, 1, 'SIGNED'),
    createSavedAdviceOrder('history-advice-enc-demo-2', adviceOrderCatalog[6]!, 2, 'SIGNED')
  ],
  'history-advice-enc-1003-1': [
    createSavedAdviceOrder(
      'history-advice-enc-1003-1',
      adviceOrderCatalog[0]!,
      1,
      'SIGNED',
      'history-group-1003-1'
    ),
    createSavedAdviceOrder(
      'history-advice-enc-1003-1',
      adviceOrderCatalog[4]!,
      2,
      'SIGNED',
      'history-group-1003-1'
    ),
    createSavedAdviceOrder('history-advice-enc-1003-1', adviceOrderCatalog[6]!, 3, 'WITHDRAWN')
  ],
  'history-advice-enc-1003-2': [
    createSavedAdviceOrder('history-advice-enc-1003-2', adviceOrderCatalog[1]!, 1, 'SIGNED'),
    createSavedAdviceOrder('history-advice-enc-1003-2', adviceOrderCatalog[7]!, 2, 'SIGNED')
  ],
  'history-advice-enc-1008-1': [
    createSavedAdviceOrder('history-advice-enc-1008-1', adviceOrderCatalog[3]!, 1, 'SIGNED'),
    createSavedAdviceOrder('history-advice-enc-1008-1', adviceOrderCatalog[5]!, 2, 'WITHDRAWN')
  ]
}

/**
 * 制造接近真实网络请求的短暂延迟。
 * 搜索词越短延迟越长，便于验证“旧请求不能覆盖新请求”的保护逻辑。
 */
function waitForMockRequest(keyword = '') {
  const delay = Math.max(180, 620 - keyword.length * 80)
  return new Promise<void>((resolve) => window.setTimeout(resolve, delay))
}

/** 就诊状态码对应的中文名；状态名不再由接口返回，由前端根据 visitStatus 判断。 */
const visitStatusNameByCode: Record<DoctorVisitStatus, string> = {
  arrived: '候诊',
  'in-progress': '在诊',
  finished: '完诊'
}

function findPatientOrThrow(encounterId: string) {
  const patient = currentEncounterRegistry.find((item) => item.encounterId === encounterId)
  if (!patient) throw new Error('未找到当前患者的就诊记录')
  return patient
}

function buildPatientContext(patient: DoctorPatientQueueItemVO): DoctorPatientContextVO {
  const details = patientContextDetails[patient.encounterId]

  return {
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    visitStatus: patient.visitStatus,
    visitStatusName: visitStatusNameByCode[patient.visitStatus],
    attendingDoctorName: patient.visitStatus === 'arrived' ? undefined : '李尔王',
    // 暂离后重新进入候诊，不应继续展示上一轮接诊时间。
    acceptedAt:
      patient.visitStatus === 'arrived' ? undefined : acceptedAtByEncounterId[patient.encounterId],
    phone: details?.phone ?? '138****0000'
  }
}

/**
 * 模拟按单日查询医生患者队列，不做分页截断。
 *
 * 输入关键词“失败”会主动抛出错误，方便开发阶段检查失败提示和刷新重试。
 */
export async function mockListDoctorPatientQueue(
  query: DoctorPatientQueueQuery
): Promise<DoctorPatientQueueItemVO[]> {
  const patientName = query.patientName?.trim() ?? ''
  await waitForMockRequest(patientName)

  if (patientName === '失败') {
    throw new Error('模拟患者队列加载失败')
  }
  if (!/^\d{4}-\d{2}-\d{2}$/.test(query.visitDate)) {
    throw new Error('患者队列查询日期格式应为 YYYY-MM-DD')
  }

  const normalizedPatientName = patientName.toLocaleLowerCase()
  const filteredRows = patientQueue.filter((patient) => {
    const visitDate = patient.visitTime.slice(0, 10)
    const matchesKeyword =
      !normalizedPatientName ||
      patient.patientName.toLocaleLowerCase().includes(normalizedPatientName)
    const matchesVisitDate = visitDate === query.visitDate
    const matchesVisitStatus = !query.visitStatus || patient.visitStatus === query.visitStatus

    return matchesKeyword && matchesVisitDate && matchesVisitStatus
  })
  return filteredRows.map((patient) => ({ ...patient }))
}

/**
 * 模拟分页查询患者历史门诊病历。
 *
 * 历史数据只包含已完诊且有有效正文的病历，并按就诊时间从新到旧返回。
 */
export async function mockPagePatientMedicalRecordHistory(
  query: DoctorMedicalRecordHistoryQuery
): Promise<PageDTO<DoctorMedicalRecordHistoryItemVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword || query.patientId)

  if (keyword === '失败') {
    throw new Error('模拟历史病历加载失败')
  }

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = historicalMedicalRecordVisits
    .filter((item) => {
      const record = medicalRecordByEncounterId[item.encounterId]
      const matchesPatient = item.patientId === query.patientId
      const matchesKeyword =
        !normalizedKeyword ||
        [item.visitNo, item.deptName, item.doctorName, item.diagnosis]
          .filter(Boolean)
          .join(' ')
          .toLocaleLowerCase()
          .includes(normalizedKeyword)
      const hasSavedRecord =
        Boolean(record) &&
        Boolean(record?.id) &&
        hasMeaningfulMedicalRecordContent(record?.content ?? '')

      return matchesPatient && matchesKeyword && hasSavedRecord
    })
    .sort((left, right) => right.visitDate.localeCompare(left.visitDate))

  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize

  return createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize).map(toMedicalRecordHistoryItem)
  })
}

/**
 * 模拟接诊或加载已有就诊。
 *
 * 待诊患者进入在诊状态；已经在诊或完诊的患者保持原状态并返回上下文。
 */
export async function mockAcceptPatient(encounterId: string): Promise<DoctorPatientContextVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus === 'arrived') {
    patient.visitStatus = 'in-progress'
    acceptedAtByEncounterId[encounterId] = '2026-07-23 10:30'
  }

  return buildPatientContext(patient)
}

/**
 * 模拟暂离。
 *
 * 当前项目只有候诊、在诊、完诊三种状态，因此“暂离”表示结束本轮接诊占用，
 * 让患者重新回到候诊队列。重复暂离按幂等操作处理。
 */
export async function mockLeavePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus === 'finished') {
    throw new Error('该患者已经完诊，不能执行暂离')
  }

  if (patient.visitStatus === 'in-progress') {
    patient.visitStatus = 'arrived'
  }

  return buildPatientContext(patient)
}

/**
 * 判断富文本是否包含医生真正录入的内容。
 *
 * 富文本编辑器可能产生只有标签、空格或 &nbsp; 的 HTML，这种内容不能算作
 * 已完成病历。真实后端接入后，这条规则仍应在后端保留。
 */
function hasMeaningfulMedicalRecordContent(content: string) {
  return content
    .replace(/<[^>]*>/g, '')
    .replace(/&nbsp;|&#160;/gi, '')
    .trim()
}

/**
 * 模拟完诊，并在 Mock“服务端”再次校验病历和诊断。
 *
 * 页面校验负责及时提醒医生；这里的校验负责防止绕过页面直接调用接口。
 */
export async function mockCompletePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  // 完诊接口按幂等方式返回，避免网络重试造成重复业务结果。
  if (patient.visitStatus === 'finished') return buildPatientContext(patient)
  if (patient.visitStatus !== 'in-progress') {
    throw new Error('请先接诊患者再执行完诊')
  }

  const medicalRecord = medicalRecordByEncounterId[encounterId]
  if (
    !medicalRecord ||
    !medicalRecord.id ||
    !hasMeaningfulMedicalRecordContent(medicalRecord.content)
  ) {
    throw new Error('请先填写并保存门诊病历')
  }

  const diagnosisList = diagnosisListByEncounterId[encounterId]
  if (
    !diagnosisList ||
    diagnosisList.rows.length === 0 ||
    diagnosisList.rows.filter((item) => item.primary).length !== 1
  ) {
    throw new Error('请先保存诊断，并且只能设置一个主诊断')
  }

  patient.visitStatus = 'finished'
  return buildPatientContext(patient)
}

/**
 * 模拟获取当前就诊病历。
 *
 * 后端没有记录时仍返回统一结构，页面不需要用 null 区分“新建”和“编辑”。
 */
export async function mockGetMedicalRecord(encounterId: string): Promise<DoctorMedicalRecordVO> {
  await waitForMockRequest(encounterId)
  const isCurrentEncounter = currentEncounterRegistry.some(
    (item) => item.encounterId === encounterId
  )
  const isHistoricalEncounter = historicalMedicalRecordVisits.some(
    (item) => item.encounterId === encounterId
  )
  if (!isCurrentEncounter && !isHistoricalEncounter) {
    throw new Error('未找到指定就诊的门诊病历')
  }

  const savedRecord = medicalRecordByEncounterId[encounterId]
  return savedRecord
    ? { ...savedRecord }
    : {
        encounterId,
        content: ''
      }
}

/**
 * 模拟保存病历。
 *
 * 只有在诊患者允许保存；数据更新一致性由后端负责。
 */
export async function mockSaveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  await waitForMockRequest(encounterId)
  const patient = findPatientOrThrow(encounterId)

  if (patient.visitStatus !== 'in-progress') {
    throw new Error(
      patient.visitStatus === 'arrived' ? '请先接诊患者再保存病历' : '患者已完诊，病历不可修改'
    )
  }

  const currentRecord = medicalRecordByEncounterId[encounterId]
  const savedRecord: DoctorMedicalRecordVO = {
    id: currentRecord?.id ?? `record-${encounterId}`,
    encounterId,
    content: data.content,
    updatedAt: '2026-07-23 10:35:00'
  }
  medicalRecordByEncounterId[encounterId] = savedRecord

  return { ...savedRecord }
}

/** 模拟获取当前就诊诊断；没有保存记录时返回空列表。 */
export async function mockGetDiagnoses(encounterId: string): Promise<DoctorDiagnosisListVO> {
  await waitForMockRequest(encounterId)
  findPatientOrThrow(encounterId)

  const saved = diagnosisListByEncounterId[encounterId]
  return saved
    ? { ...saved, rows: saved.rows.map((item) => ({ ...item })) }
    : { encounterId, rows: [] }
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

/** 模拟患者跨就诊历史诊断的条件分页查询。 */
export async function mockPagePatientDiagnosisHistory(
  query: DoctorPatientDiagnosisHistoryQuery
): Promise<PageDTO<DoctorPatientDiagnosisHistoryItemVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword || query.patientId)
  if (keyword === '失败') throw new Error('模拟患者历史诊断加载失败')

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = historicalPatientDiagnoses
    .filter((item) => {
      const matchesKeyword =
        !normalizedKeyword ||
        [
          item.visitNo,
          item.departmentName,
          item.attendingDoctorName,
          item.code,
          item.name,
          item.typeName
        ]
          .join(' ')
          .toLocaleLowerCase()
          .includes(normalizedKeyword)
      const matchesStart = !query.startDate || item.visitedAt.slice(0, 10) >= query.startDate
      const matchesEnd = !query.endDate || item.visitedAt.slice(0, 10) <= query.endDate
      return item.patientId === query.patientId && matchesKeyword && matchesStart && matchesEnd
    })
    .sort(
      (left, right) => right.visitedAt.localeCompare(left.visitedAt) || left.orderNo - right.orderNo
    )
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize
  return createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize).map((item) => ({
      id: item.id,
      encounterId: item.encounterId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      typeName: item.typeName,
      primary: item.primary,
      diagnosedAt: item.diagnosedAt,
      orderNo: item.orderNo
    }))
  })
}

let diagnosisMutationSequence = 1000

function ensureDiagnosisList(encounterId: string): DoctorDiagnosisListVO {
  const existing = diagnosisListByEncounterId[encounterId]
  if (existing) return existing
  const created: DoctorDiagnosisListVO = { encounterId, rows: [] }
  diagnosisListByEncounterId[encounterId] = created
  return created
}

function assertDiagnosisEditable(encounterId: string) {
  const patient = findPatientOrThrow(encounterId)
  if (patient.visitStatus !== 'in-progress') {
    throw new Error(
      patient.visitStatus === 'arrived' ? '请先接诊患者再操作诊断' : '患者已完诊，诊断不可修改'
    )
  }
}

function finishDiagnosisMutation(
  list: DoctorDiagnosisListVO,
  rows: DoctorDiagnosisVO[]
): DoctorDiagnosisListVO {
  list.rows = rows.map((item, index) => ({ ...item, orderNo: index + 1 }))
  list.updatedAt = dayjs().format('YYYY-MM-DD HH:mm:ss')
  return { ...list, rows: list.rows.map((item) => ({ ...item })) }
}

/** 模拟单条新增诊断接口。 */
export async function mockCreateDiagnosis(
  data: OpdoctorCreateDiagnosisDTO
): Promise<DoctorDiagnosisListVO> {
  await waitForMockRequest(data.encounterId)
  assertDiagnosisEditable(data.encounterId)
  const list = ensureDiagnosisList(data.encounterId)
  if (list.rows.some((item) => item.catalogId === data.conditionId)) {
    throw new Error('当前就诊已存在相同诊断')
  }
  const primary = list.rows.length === 0 ? true : data.primaryFlag === 1
  if (primary && list.rows.some((item) => item.primary)) {
    throw new Error('只能设置一个主诊断')
  }
  const diagnosedAt = dayjs().format('YYYY-MM-DD HH:mm:ss')
  return finishDiagnosisMutation(list, [
    ...list.rows,
    {
      id: `diagnosis-${data.encounterId}-${++diagnosisMutationSequence}`,
      encounterId: data.encounterId,
      catalogId: data.conditionId,
      code: data.diagCode,
      name: data.description,
      typeName: data.diagTypeCode,
      primary,
      diagnosedAt,
      orderNo: data.rank
    }
  ])
}

/** 模拟单条删除诊断接口；删除主诊断后自动补位。 */
export async function mockDeleteDiagnosis(id: string): Promise<DoctorDiagnosisListVO> {
  await waitForMockRequest(id)
  const entry = Object.entries(diagnosisListByEncounterId).find(([, list]) =>
    list.rows.some((item) => item.id === id)
  )
  if (!entry) throw new Error('未找到需要删除的诊断')
  const [encounterId, list] = entry
  assertDiagnosisEditable(encounterId)
  const remaining = list.rows.filter((item) => item.id !== id).map((item) => ({ ...item }))
  if (remaining.length === 0) throw new Error('至少需要保留一条诊断')
  if (!remaining.some((item) => item.primary)) remaining[0]!.primary = true
  return finishDiagnosisMutation(list, remaining)
}

let adviceMutationSequence = 1000

function cloneAdviceList(list: DoctorAdviceOrderListVO): DoctorAdviceOrderListVO {
  return { ...list, rows: list.rows.map((item) => ({ ...item })) }
}

function ensureAdviceList(encounterId: string): DoctorAdviceOrderListVO {
  const existing = adviceOrderListByEncounterId[encounterId]
  if (existing) {
    if (existing.version === undefined) existing.version = 0
    return existing
  }

  const created: DoctorAdviceOrderListVO = { encounterId, rows: [], version: 0 }
  adviceOrderListByEncounterId[encounterId] = created
  return created
}

/**
 * 校验变更请求携带的列表版本。
 *
 * 模拟后端的乐观锁：请求 version 与当前列表 version 不一致时拒绝，
 * 页面提交前应通过查询拿到最新 version。为兼容单测直接调用变更接口，
 * 未携带 version 时不校验。
 */
function assertAdviceVersion(encounterId: string, version?: number) {
  if (version === undefined) return
  const list = ensureAdviceList(encounterId)
  if (version !== list.version) {
    throw new Error('医嘱列表已被其他操作更新，请刷新后重试')
  }
}

/** 医嘱列表每次变更后版本递增，并刷新更新时间。 */
function finishAdviceMutation(
  list: DoctorAdviceOrderListVO,
  rows: DoctorAdviceOrderVO[]
): DoctorAdviceOrderListVO {
  list.rows = normalizeOrphanAdviceGroups(rows).sort((left, right) => left.orderNo - right.orderNo)
  list.updatedAt = dayjs().format('YYYY-MM-DD HH:mm:ss')
  list.version = (list.version ?? 0) + 1
  return cloneAdviceList(list)
}

function assertAdviceEncounterEditable(encounterId: string) {
  const patient = findPatientOrThrow(encounterId)
  if (patient.visitStatus !== 'in-progress') {
    throw new Error(
      patient.visitStatus === 'arrived' ? '请先接诊患者再操作医嘱' : '患者已完诊，医嘱不可修改'
    )
  }
}

function validateAdviceItem(item: SaveDoctorAdviceOrdersDTO['items'][number]) {
  if (!item.name.trim()) throw new Error('医嘱项目名称不能为空')
  if (!item.quantity || item.quantity <= 0) throw new Error(item.name + '：请填写开立数量')
  if (
    item.category === 'DRUG' &&
    (!item.dose || !item.unit || !item.frequency || !item.route || !item.days)
  ) {
    throw new Error(item.name + '：药品剂量、单位、用法、频次和天数必须完整')
  }
}

function toDraftAdviceOrder(
  encounterId: string,
  item: SaveDoctorAdviceOrdersDTO['items'][number],
  id: string
): DoctorAdviceOrderVO {
  return {
    id,
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
    dosePerQuantityUnit: item.dosePerQuantityUnit,
    remark: item.remark,
    status: 'DRAFT',
    groupId: item.groupId,
    orderNo: item.orderNo
  }
}

/** 任意组只保留至少两条成员，拆组或删除后的孤立行自动解组。 */
function normalizeOrphanAdviceGroups(rows: DoctorAdviceOrderVO[]) {
  const groupCounts = new Map<string, number>()
  for (const row of rows) {
    if (row.groupId) groupCounts.set(row.groupId, (groupCounts.get(row.groupId) ?? 0) + 1)
  }
  return rows.map((row) =>
    row.groupId && (groupCounts.get(row.groupId) ?? 0) < 2
      ? { ...row, groupId: undefined }
      : { ...row }
  )
}

function assertCompleteAdviceGroupsSelected(rows: DoctorAdviceOrderVO[], adviceIds: string[]) {
  const selected = new Set(adviceIds)
  const selectedGroups = new Set(
    rows.filter((row) => selected.has(row.id) && row.groupId).map((row) => row.groupId!)
  )

  for (const groupId of selectedGroups) {
    const groupRows = rows.filter((row) => row.groupId === groupId)
    if (groupRows.some((row) => !selected.has(row.id))) {
      throw new Error('成组医嘱必须整组选择后再操作')
    }
  }
}

/** 模拟获取当前就诊医嘱；没有保存记录时返回空列表。 */
export async function mockGetAdviceOrders(encounterId: string): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(encounterId)
  findPatientOrThrow(encounterId)
  const list = ensureAdviceList(encounterId)
  if (list.version === undefined) list.version = 0
  return cloneAdviceList(list)
}

/** 模拟按条件分页查询本次就诊医嘱。 */
export async function mockPageAdviceOrders(
  query: DoctorAdviceOrderQuery
): Promise<DoctorAdviceOrderPageVO> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword || query.encounterId)
  findPatientOrThrow(query.encounterId)
  if (keyword === '失败') throw new Error('模拟医嘱列表加载失败')

  const list = ensureAdviceList(query.encounterId)
  const normalizedKeyword = keyword.toLocaleLowerCase()
  const filteredRows = list.rows.filter((item) => {
    const matchesKeyword =
      !normalizedKeyword ||
      [item.code, item.name, item.categoryName, item.remark]
        .filter(Boolean)
        .join(' ')
        .toLocaleLowerCase()
        .includes(normalizedKeyword)
    const matchesCategory = !query.category || item.category === query.category
    const matchesStatus = !query.status || item.status === query.status
    return matchesKeyword && matchesCategory && matchesStatus
  })
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize
  const page = createPageDTO({
    pageIndex,
    pageSize,
    total: filteredRows.length,
    rows: filteredRows.slice(startIndex, startIndex + pageSize).map((item) => ({ ...item }))
  })

  return {
    ...page,
    encounterId: query.encounterId,
    updatedAt: list.updatedAt,
    version: list.version ?? 0
  }
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

/** 模拟新增单条草稿医嘱。 */
export async function mockCreateAdviceOrder(
  data: CreateDoctorAdviceOrderDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  const list = ensureAdviceList(data.encounterId)
  validateAdviceItem(data.item)
  if (
    list.rows.some((row) => row.catalogId === data.item.catalogId && row.status !== 'WITHDRAWN')
  ) {
    throw new Error('不能重复添加相同医嘱项目')
  }

  const id = `advice-${data.encounterId}-${++adviceMutationSequence}`
  return finishAdviceMutation(list, [
    ...list.rows,
    toDraftAdviceOrder(data.encounterId, data.item, id)
  ])
}

/** 模拟修改单条草稿医嘱。 */
export async function mockUpdateAdviceOrder(
  data: UpdateDoctorAdviceOrderDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  validateAdviceItem(data.item)
  const current = list.rows.find((row) => row.id === data.adviceId)
  if (!current) throw new Error('未找到待修改医嘱')
  if (current.status !== 'DRAFT') throw new Error('只有未签发医嘱可以修改')

  return finishAdviceMutation(
    list,
    list.rows.map((row) =>
      row.id === data.adviceId
        ? {
            ...toDraftAdviceOrder(data.encounterId, data.item, data.adviceId),
            groupId: row.groupId
          }
        : row
    )
  )
}

/** 模拟批量删除草稿医嘱。 */
export async function mockDeleteAdviceOrders(
  data: DeleteDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  if (data.adviceIds.length === 0) throw new Error('请选择需要删除的医嘱')
  const selected = list.rows.filter((row) => data.adviceIds.includes(row.id))
  if (selected.length !== data.adviceIds.length) throw new Error('部分待删除医嘱不存在')
  if (selected.some((row) => row.status !== 'DRAFT')) throw new Error('只有未签发医嘱可以删除')

  return finishAdviceMutation(
    list,
    list.rows.filter((row) => !data.adviceIds.includes(row.id))
  )
}

/** 模拟任意草稿医嘱并组。 */
export async function mockGroupAdviceOrders(
  data: GroupDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  const selectedIds = [...new Set(data.adviceIds)]
  if (selectedIds.length < 2) throw new Error('并组至少需要选择两条医嘱')
  const selected = list.rows.filter((row) => selectedIds.includes(row.id))
  if (selected.length !== selectedIds.length) throw new Error('部分待并组医嘱不存在')
  if (selected.some((row) => row.status !== 'DRAFT')) throw new Error('只有未签发医嘱可以并组')

  return finishAdviceMutation(
    list,
    list.rows.map((row) => (selectedIds.includes(row.id) ? { ...row, groupId: data.groupId } : row))
  )
}

/** 模拟草稿医嘱拆组。 */
export async function mockUngroupAdviceOrders(
  data: UngroupDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  if (data.adviceIds.length === 0) throw new Error('请选择需要拆组的医嘱')
  const selected = list.rows.filter((row) => data.adviceIds.includes(row.id))
  if (selected.length !== new Set(data.adviceIds).size) throw new Error('部分待拆组医嘱不存在')
  if (selected.some((row) => row.status !== 'DRAFT')) throw new Error('只有未签发医嘱可以拆组')

  return finishAdviceMutation(
    list,
    list.rows.map((row) => (data.adviceIds.includes(row.id) ? { ...row, groupId: undefined } : row))
  )
}

/** 模拟批量签发草稿医嘱。 */
export async function mockSignAdviceOrders(
  data: SignDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  if (data.adviceIds.length === 0) throw new Error('请选择需要签发的医嘱')
  const selected = list.rows.filter((row) => data.adviceIds.includes(row.id))
  if (selected.length !== data.adviceIds.length) throw new Error('部分待签发医嘱不存在')
  if (selected.some((row) => row.status !== 'DRAFT')) throw new Error('只能签发未签发医嘱')
  assertCompleteAdviceGroupsSelected(list.rows, data.adviceIds)

  return finishAdviceMutation(
    list,
    list.rows.map((row) =>
      data.adviceIds.includes(row.id)
        ? {
            ...row,
            status: 'SIGNED',
            prescriptionNo:
              row.category === 'DRUG' ? `RX-${data.encounterId}-${row.id}` : undefined,
            serviceRequestId:
              row.category === 'DRUG' ? undefined : `SR-${data.encounterId}-${row.id}`
          }
        : row
    )
  )
}

/** 模拟批量撤回已签发医嘱，撤回后保留记录。 */
export async function mockWithdrawAdviceOrders(
  data: WithdrawDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(data.encounterId)
  assertAdviceEncounterEditable(data.encounterId)
  assertAdviceVersion(data.encounterId, data.version)
  const list = ensureAdviceList(data.encounterId)
  if (data.adviceIds.length === 0) throw new Error('请选择需要撤回的医嘱')
  const selected = list.rows.filter((row) => data.adviceIds.includes(row.id))
  if (selected.length !== data.adviceIds.length) throw new Error('部分待撤回医嘱不存在')
  if (selected.some((row) => row.status !== 'SIGNED')) throw new Error('只能撤回已签发医嘱')
  assertCompleteAdviceGroupsSelected(list.rows, data.adviceIds)

  return finishAdviceMutation(
    list,
    list.rows.map((row) =>
      data.adviceIds.includes(row.id) ? { ...row, status: 'WITHDRAWN' } : row
    )
  )
}

/**
 * 模拟生成单张检验、检查或治疗申请单。
 *
 * 打印只读取已签发医嘱，不改变医嘱状态；真实后端也应保持这个特性，
 * 这样浏览器打印失败后可以安全重试。
 */
export async function mockPrintAdviceApplicationForm(
  data: PrintDoctorAdviceApplicationFormDTO
): Promise<DoctorAdviceApplicationFormVO> {
  const serviceRequestId = data.serviceRequestId.trim()
  await waitForMockRequest(serviceRequestId)
  if (!serviceRequestId) throw new Error('缺少服务申请 ID')

  let matchedEncounterId = ''
  let matchedOrder: DoctorAdviceOrderVO | undefined
  for (const [encounterId, list] of Object.entries(adviceOrderListByEncounterId)) {
    const order = list.rows.find((item) => item.serviceRequestId === serviceRequestId)
    if (!order) continue
    matchedEncounterId = encounterId
    matchedOrder = order
    break
  }

  if (!matchedOrder || !matchedEncounterId) throw new Error('未找到对应的医嘱申请单')
  if (matchedOrder.status !== 'SIGNED') throw new Error('只有已签发医嘱可以打印申请单')
  if (matchedOrder.category === 'DRUG') throw new Error('药品医嘱请使用处方单功能')

  const patient = findPatientOrThrow(matchedEncounterId)
  const context = buildPatientContext(patient)
  const diagnosisNames = (diagnosisListByEncounterId[matchedEncounterId]?.rows ?? [])
    .slice()
    .sort((left, right) => left.orderNo - right.orderNo)
    .map((item) => item.name)
    .join('、')

  return {
    hospitalName: '零壹医院',
    applicationNo: `AF-${serviceRequestId}`,
    serviceRequestId,
    categoryName: matchedOrder.categoryName,
    patientName: patient.patientName,
    genderName: patient.gender,
    age: patient.age,
    patientNo: patient.visitNo,
    visitNo: patient.visitNo,
    phone: context.phone,
    departmentName: patient.departmentName,
    attendingDoctorName: context.attendingDoctorName ?? '门诊医生',
    requestedAt: dayjs().format('YYYY-MM-DD HH:mm:ss'),
    diagnosisNames: diagnosisNames || '待补充',
    chiefComplaint: '（演示数据）',
    adviceId: matchedOrder.id,
    itemCode: matchedOrder.code,
    itemName: matchedOrder.name,
    specification: matchedOrder.specification,
    quantity: matchedOrder.quantity,
    quantityUnit: matchedOrder.quantityUnit,
    remark: matchedOrder.remark
  }
}

/** 模拟患者历史医嘱就诊分页查询。 */
export async function mockPagePatientAdviceOrderHistory(
  query: DoctorAdviceOrderHistoryQuery
): Promise<PageDTO<DoctorAdviceOrderHistoryEncounterVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword || query.patientId)
  const normalizedKeyword = keyword.toLocaleLowerCase()
  const rows = historicalAdviceEncounters
    .filter((item) => {
      const matchesKeyword =
        !normalizedKeyword ||
        [item.visitNo, item.departmentName, item.attendingDoctorName]
          .join(' ')
          .toLocaleLowerCase()
          .includes(normalizedKeyword)
      return (
        item.patientId === query.patientId &&
        (!query.visitType || item.visitType === query.visitType) &&
        matchesKeyword
      )
    })
    .sort((left, right) => right.visitedAt.localeCompare(left.visitedAt))
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const startIndex = (pageIndex - 1) * pageSize
  return createPageDTO({
    pageIndex,
    pageSize,
    total: rows.length,
    rows: rows.slice(startIndex, startIndex + pageSize).map((item) => ({ ...item }))
  })
}

/** 模拟历史医嘱详情分页查询。 */
export async function mockPagePatientAdviceOrderHistoryDetails(
  query: DoctorAdviceOrderHistoryDetailQuery
): Promise<PageDTO<DoctorAdviceOrderVO>> {
  const keyword = query.keyword?.trim() ?? ''
  await waitForMockRequest(keyword || query.encounterId)
  const encounter = historicalAdviceEncounters.find(
    (item) => item.encounterId === query.encounterId && item.patientId === query.patientId
  )
  if (!encounter) throw new Error('未找到患者历史医嘱就诊')

  const normalizedKeyword = keyword.toLocaleLowerCase()
  const rows = (historicalAdviceOrderListByEncounterId[query.encounterId] ?? []).filter((item) => {
    const matchesKeyword =
      !normalizedKeyword ||
      [item.code, item.name, item.categoryName, item.remark]
        .filter(Boolean)
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
    total: rows.length,
    rows: rows.slice(startIndex, startIndex + pageSize).map((item) => ({ ...item }))
  })
}

/**
 * 兼容旧页面的全量草稿保存。
 *
 * 已签发和已撤回行不会被覆盖；新页面迁移到差异 CRUD 后不再调用此入口。
 */
export async function mockSaveAdviceOrders(
  encounterId: string,
  data: SaveDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  await waitForMockRequest(encounterId)
  assertAdviceEncounterEditable(encounterId)
  const list = ensureAdviceList(encounterId)
  const draftItems = data.items.filter(
    (item) => !item.id || !list.rows.some((row) => row.id === item.id && row.status !== 'DRAFT')
  )
  const catalogIds = draftItems.map((item) => item.catalogId)
  if (new Set(catalogIds).size !== catalogIds.length) {
    throw new Error('不能重复添加相同医嘱项目')
  }
  draftItems.forEach(validateAdviceItem)
  const immutableRows = list.rows.filter((row) => row.status !== 'DRAFT')
  const drafts = draftItems.map((item) =>
    toDraftAdviceOrder(
      encounterId,
      item,
      item.id ?? `advice-${encounterId}-${++adviceMutationSequence}`
    )
  )
  return finishAdviceMutation(list, [...immutableRows, ...drafts])
}
