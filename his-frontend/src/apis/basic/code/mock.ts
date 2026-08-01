import type {
  CodeItemQuery,
  CodeItemRecord,
  CodeItemSavePayload,
  CodeTypeQuery,
  CodeTypeRecord,
  CodeTypeSavePayload
} from './type'

// ---------- 模拟数据 ----------

let typeSeq = 20
let itemSeq = 100

function now() {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

// 字典类型种子数据
const dictTypes: CodeTypeRecord[] = [
  seedType('dt-001', 'vital_signs_code', '生命体特征', '1.0', '生命体特征相关编码'),
  seedType('dt-002', 'diagnosis_code', '诊断编码', '1.0', '疾病诊断相关编码'),
  seedType('dt-003', 'medication_code', '药品编码', '2.0', '药品信息相关编码'),
  seedType('dt-004', 'surgery_code', '手术编码', '1.0', '手术操作相关编码'),
  seedType('dt-005', 'examination_code', '检查编码', '1.0', '医学检查相关编码'),
  seedType('dt-006', 'laboratory_code', '检验编码', '1.0', '检验项目相关编码'),
  seedType('dt-007', 'department_code', '科室编码', '1.0', '医院科室相关编码'),
  seedType('dt-008', 'charge_code', '收费编码', '1.0', '收费项目相关编码'),
  seedType('dt-009', 'material_code', '材料编码', '2.0', '医用材料相关编码'),
  seedType('dt-010', 'nursing_code', '护理编码', '1.0', '护理项目相关编码')
]

// 枚举类型种子数据
const enumTypes: CodeTypeRecord[] = [
  seedType('et-001', 'gender', '性别', '1.0', '患者性别枚举'),
  seedType('et-002', 'blood_type', '血型', '1.0', '血型分类枚举'),
  seedType('et-003', 'marital_status', '婚姻状况', '1.0', '婚姻状况枚举'),
  seedType('et-004', 'education_level', '学历', '1.0', '学历等级枚举'),
  seedType('et-005', 'nationality', '国籍', '1.0', '国籍枚举'),
  seedType('et-006', 'nation', '民族', '1.0', '民族枚举'),
  seedType('et-007', 'occupation', '职业', '1.0', '职业分类枚举'),
  seedType('et-008', 'payment_method', '支付方式', '1.0', '支付方式枚举'),
  seedType('et-009', 'patient_status', '患者状态', '1.0', '患者就诊状态枚举'),
  seedType('et-010', 'visit_type', '就诊类型', '1.0', '就诊类型枚举')
]

// 字典项
const dictItems: CodeItemRecord[] = [
  // dt-001 生命体特征
  seedItem('di-001', 'dt-001', 'heart_rate', '心率', '患者心率指标', 1),
  seedItem('di-002', 'dt-001', 'blood_pressure', '血压', '患者血压指标', 2),
  seedItem('di-003', 'dt-001', 'temperature', '体温', '患者体温指标', 3),
  seedItem('di-004', 'dt-001', 'respiratory_rate', '呼吸频率', '患者呼吸频率指标', 4),
  seedItem('di-005', 'dt-001', 'oxygen_saturation', '血氧饱和度', '患者血氧饱和度指标', 5),
  // dt-002 诊断编码
  seedItem('di-006', 'dt-002', 'hypertension', '高血压', '原发性高血压诊断', 1),
  seedItem('di-007', 'dt-002', 'diabetes', '糖尿病', '2型糖尿病诊断', 2),
  seedItem('di-008', 'dt-002', 'pneumonia', '肺炎', '细菌性肺炎诊断', 3),
  seedItem('di-009', 'dt-002', 'fracture', '骨折', '四肢骨折诊断', 4),
  seedItem('di-010', 'dt-002', 'gastritis', '胃炎', '慢性胃炎诊断', 5),
  seedItem('di-011', 'dt-002', 'anemia', '贫血', '缺铁性贫血诊断', 6),
  // dt-003 药品编码
  seedItem('di-012', 'dt-003', 'aspirin', '阿司匹林', '解热镇痛抗炎药', 1),
  seedItem('di-013', 'dt-003', 'metformin', '二甲双胍', '口服降糖药', 2),
  seedItem('di-014', 'dt-003', 'amoxicillin', '阿莫西林', '广谱青霉素类抗生素', 3),
  seedItem('di-015', 'dt-003', 'omeprazole', '奥美拉唑', '质子泵抑制剂', 4),
  seedItem('di-016', 'dt-003', 'atorvastatin', '阿托伐他汀', '降脂药', 5),
  // dt-004 手术编码
  seedItem('di-017', 'dt-004', 'appendectomy', '阑尾切除术', '阑尾切除手术', 1),
  seedItem('di-018', 'dt-004', 'cholecystectomy', '胆囊切除术', '胆囊切除手术', 2),
  seedItem('di-019', 'dt-004', 'craniotomy', '开颅手术', '开颅手术操作', 3),
  seedItem('di-020', 'dt-004', 'arthroplasty', '关节置换术', '人工关节置换手术', 4),
  seedItem('di-021', 'dt-004', 'cataract_surgery', '白内障手术', '白内障摘除手术', 5),
  // dt-005 检查编码
  seedItem('di-022', 'dt-005', 'chest_xray', '胸部X光', '胸部X射线检查', 1),
  seedItem('di-023', 'dt-005', 'ct_scan', 'CT扫描', '计算机断层扫描', 2),
  seedItem('di-024', 'dt-005', 'mri', '核磁共振', '磁共振成像检查', 3),
  seedItem('di-025', 'dt-005', 'ultrasound', '超声检查', 'B型超声检查', 4),
  seedItem('di-026', 'dt-005', 'ecg', '心电图', '心电图检查', 5),
  // dt-006 检验编码
  seedItem('di-027', 'dt-006', 'blood_routine', '血常规', '血液常规检验', 1),
  seedItem('di-028', 'dt-006', 'urine_routine', '尿常规', '尿液常规检验', 2),
  seedItem('di-029', 'dt-006', 'liver_function', '肝功能', '肝功能检验', 3),
  seedItem('di-030', 'dt-006', 'kidney_function', '肾功能', '肾功能检验', 4),
  seedItem('di-031', 'dt-006', 'blood_lipid', '血脂', '血脂检验', 5),
  // dt-007 科室编码
  seedItem('di-032', 'dt-007', 'internal_medicine', '内科', '内科科室', 1),
  seedItem('di-033', 'dt-007', 'surgery', '外科', '外科科室', 2),
  seedItem('di-034', 'dt-007', 'pediatrics', '儿科', '儿科科室', 3),
  seedItem('di-035', 'dt-007', 'obstetrics', '妇产科', '妇产科科室', 4),
  seedItem('di-036', 'dt-007', 'ophthalmology', '眼科', '眼科科室', 5),
  // dt-008 收费编码
  seedItem('di-037', 'dt-008', 'registration_fee', '挂号费', '门诊挂号收费', 1),
  seedItem('di-038', 'dt-008', 'consultation_fee', '诊查费', '医生诊查收费', 2),
  seedItem('di-039', 'dt-008', 'bed_fee', '床位费', '住院床位收费', 3),
  seedItem('di-040', 'dt-008', 'nursing_fee', '护理费', '护理服务收费', 4),
  seedItem('di-041', 'dt-008', 'treatment_fee', '治疗费', '治疗项目收费', 5),
  // dt-009 材料编码
  seedItem('di-042', 'dt-009', 'syringe', '注射器', '一次性注射器', 1),
  seedItem('di-043', 'dt-009', 'gauze', '纱布', '医用纱布', 2),
  seedItem('di-044', 'dt-009', 'gloves', '手套', '医用检查手套', 3),
  seedItem('di-045', 'dt-009', 'catheter', '导管', '医用导管', 4),
  seedItem('di-046', 'dt-009', 'suture', '缝合线', '手术缝合线', 5),
  // dt-010 护理编码
  seedItem('di-047', 'dt-010', 'wound_care', '伤口护理', '伤口换药护理', 1),
  seedItem('di-048', 'dt-010', 'iv_therapy', '静脉输液', '静脉输液护理', 2),
  seedItem('di-049', 'dt-010', 'catheter_care', '导管护理', '导管维护护理', 3),
  seedItem('di-050', 'dt-010', 'pressure_ulcer_care', '压疮护理', '压疮预防与护理', 4),
  seedItem('di-051', 'dt-010', 'health_education', '健康教育', '患者健康教育', 5)
]

// 枚举项
const enumItems: CodeItemRecord[] = [
  // et-001 性别
  seedItem('ei-001', 'et-001', 'male', '男', '男性', 1),
  seedItem('ei-002', 'et-001', 'female', '女', '女性', 2),
  seedItem('ei-003', 'et-001', 'unknown', '未知', '性别未知', 3),
  // et-002 血型
  seedItem('ei-004', 'et-002', 'A', 'A型', 'A型血', 1),
  seedItem('ei-005', 'et-002', 'B', 'B型', 'B型血', 2),
  seedItem('ei-006', 'et-002', 'AB', 'AB型', 'AB型血', 3),
  seedItem('ei-007', 'et-002', 'O', 'O型', 'O型血', 4),
  // et-003 婚姻状况
  seedItem('ei-008', 'et-003', 'unmarried', '未婚', '未婚状态', 1),
  seedItem('ei-009', 'et-003', 'married', '已婚', '已婚状态', 2),
  seedItem('ei-010', 'et-003', 'divorced', '离异', '离异状态', 3),
  seedItem('ei-011', 'et-003', 'widowed', '丧偶', '丧偶状态', 4),
  // et-004 学历
  seedItem('ei-012', 'et-004', 'doctor', '博士', '博士研究生', 1),
  seedItem('ei-013', 'et-004', 'master', '硕士', '硕士研究生', 2),
  seedItem('ei-014', 'et-004', 'bachelor', '本科', '大学本科', 3),
  seedItem('ei-015', 'et-004', 'college', '大专', '大学专科', 4),
  seedItem('ei-016', 'et-004', 'high_school', '高中', '高中及以下', 5),
  // et-005 国籍
  seedItem('ei-017', 'et-005', 'CN', '中国', '中华人民共和国', 1),
  seedItem('ei-018', 'et-005', 'US', '美国', '美利坚合众国', 2),
  seedItem('ei-019', 'et-005', 'JP', '日本', '日本国', 3),
  seedItem('ei-020', 'et-005', 'KR', '韩国', '大韩民国', 4),
  seedItem('ei-021', 'et-005', 'UK', '英国', '大不列颠及北爱尔兰联合王国', 5),
  // et-006 民族
  seedItem('ei-022', 'et-006', 'han', '汉族', '汉族', 1),
  seedItem('ei-023', 'et-006', 'zhuang', '壮族', '壮族', 2),
  seedItem('ei-024', 'et-006', 'hui', '回族', '回族', 3),
  seedItem('ei-025', 'et-006', 'manchu', '满族', '满族', 4),
  seedItem('ei-026', 'et-006', 'uighur', '维吾尔族', '维吾尔族', 5),
  // et-007 职业
  seedItem('ei-027', 'et-007', 'doctor', '医生', '执业医师', 1),
  seedItem('ei-028', 'et-007', 'nurse', '护士', '注册护士', 2),
  seedItem('ei-029', 'et-007', 'teacher', '教师', '教育工作者', 3),
  seedItem('ei-030', 'et-007', 'engineer', '工程师', '工程技术人员', 4),
  seedItem('ei-031', 'et-007', 'student', '学生', '在校学生', 5),
  // et-008 支付方式
  seedItem('ei-032', 'et-008', 'cash', '现金', '现金支付', 1),
  seedItem('ei-033', 'et-008', 'card', '银行卡', '银行卡支付', 2),
  seedItem('ei-034', 'et-008', 'wechat', '微信支付', '微信支付', 3),
  seedItem('ei-035', 'et-008', 'alipay', '支付宝', '支付宝支付', 4),
  seedItem('ei-036', 'et-008', 'insurance', '医保结算', '医疗保险结算', 5),
  // et-009 患者状态
  seedItem('ei-037', 'et-009', 'outpatient', '门诊', '门诊就诊', 1),
  seedItem('ei-038', 'et-009', 'inpatient', '住院', '住院治疗', 2),
  seedItem('ei-039', 'et-009', 'emergency', '急诊', '急诊就诊', 3),
  seedItem('ei-040', 'et-009', 'discharged', '出院', '已出院', 4),
  seedItem('ei-041', 'et-009', 'observation', '留观', '留院观察', 5),
  // et-010 就诊类型
  seedItem('ei-042', 'et-010', 'first_visit', '初诊', '首次就诊', 1),
  seedItem('ei-043', 'et-010', 'follow_up', '复诊', '复查就诊', 2),
  seedItem('ei-044', 'et-010', 'referral', '转诊', '转院就诊', 3),
  seedItem('ei-045', 'et-010', 'emergency_visit', '急诊', '急诊就诊', 4),
  seedItem('ei-046', 'et-010', 'consultation', '会诊', '多科室会诊', 5)
]

// ---------- 工具函数 ----------

function seedType(
  id: string,
  name: string,
  title: string,
  version: string,
  description: string
): CodeTypeRecord {
  return { id, name, title, version, description, createdAt: now(), updatedAt: now() }
}

function seedItem(
  id: string,
  typeId: string,
  code: string,
  name: string,
  description: string,
  sort: number
): CodeItemRecord {
  return { id, typeId, code, name, description, sort, createdAt: now(), updatedAt: now() }
}

function delay(ms = 80) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function typeStore(system: 'dict' | 'enum') {
  return system === 'dict' ? dictTypes : enumTypes
}

function itemStore(system: 'dict' | 'enum') {
  return system === 'dict' ? dictItems : enumItems
}

function clone<T>(value: T): T {
  return JSON.parse(JSON.stringify(value))
}

// ---------- 类型 CRUD ----------

export async function mockPageCodeTypes(query: CodeTypeQuery) {
  await delay()
  const store = typeStore(query.system)
  const keyword = query.keyword?.trim().toLowerCase()
  let list = store
  if (keyword) {
    list = store.filter(
      (t) => t.name.toLowerCase().includes(keyword) || t.title.toLowerCase().includes(keyword)
    )
  }
  return clone(list)
}

export async function mockCreateCodeType(
  system: 'dict' | 'enum',
  payload: CodeTypeSavePayload
) {
  await delay()
  if (!payload.name.trim()) throw new Error('名称不能为空')
  if (!payload.title.trim()) throw new Error('标题不能为空')
  if (!payload.version.trim()) throw new Error('版本不能为空')
  typeSeq += 1
  const store = typeStore(system)
  const record: CodeTypeRecord = {
    id: `${system === 'dict' ? 'dt' : 'et'}-${String(typeSeq).padStart(3, '0')}`,
    name: payload.name.trim(),
    title: payload.title.trim(),
    version: payload.version.trim(),
    description: payload.description.trim(),
    createdAt: now(),
    updatedAt: now()
  }
  store.push(record)
  return clone(record)
}

export async function mockUpdateCodeType(
  system: 'dict' | 'enum',
  id: string,
  payload: CodeTypeSavePayload
) {
  await delay()
  if (!payload.name.trim()) throw new Error('名称不能为空')
  if (!payload.title.trim()) throw new Error('标题不能为空')
  if (!payload.version.trim()) throw new Error('版本不能为空')
  const store = typeStore(system)
  const target = store.find((t) => t.id === id)
  if (!target) throw new Error('类型不存在')
  Object.assign(target, {
    name: payload.name.trim(),
    title: payload.title.trim(),
    version: payload.version.trim(),
    description: payload.description.trim(),
    updatedAt: now()
  })
  return clone(target)
}

export async function mockRemoveCodeType(system: 'dict' | 'enum', id: string) {
  await delay()
  const store = typeStore(system)
  const index = store.findIndex((t) => t.id === id)
  if (index === -1) throw new Error('类型不存在')
  store.splice(index, 1)
  // 同步删除关联的编码项
  const items = itemStore(system)
  for (let i = items.length - 1; i >= 0; i--) {
    const current = items[i]
    if (current?.typeId === id) items.splice(i, 1)
  }
}

// ---------- 编码项 CRUD ----------

export async function mockPageCodeItems(query: CodeItemQuery) {
  await delay()
  const store = itemStore(query.system)
  let list = store.filter((item) => item.typeId === query.typeId)
  const keyword = query.keyword?.trim().toLowerCase()
  const name = query.name?.trim().toLowerCase()
  if (keyword) {
    list = list.filter(
      (item) =>
        item.name.toLowerCase().includes(keyword) ||
        item.code.toLowerCase().includes(keyword) ||
        item.description.toLowerCase().includes(keyword)
    )
  }
  if (name) {
    list = list.filter((item) => item.name.toLowerCase().includes(name))
  }
  list.sort((a, b) => a.sort - b.sort)
  return clone(list)
}

export async function mockCreateCodeItem(
  system: 'dict' | 'enum',
  typeId: string,
  payload: CodeItemSavePayload
) {
  await delay()
  if (!payload.code.trim()) throw new Error('编码不能为空')
  if (!payload.name.trim()) throw new Error('名称不能为空')
  if (!payload.description.trim()) throw new Error('描述不能为空')
  if (payload.sort < 0) throw new Error('排序号不能为负数')
  itemSeq += 1
  const store = itemStore(system)
  const prefix = system === 'dict' ? 'di' : 'ei'
  const record: CodeItemRecord = {
    id: `${prefix}-${String(itemSeq).padStart(3, '0')}`,
    typeId,
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim(),
    sort: payload.sort,
    createdAt: now(),
    updatedAt: now()
  }
  store.push(record)
  return clone(record)
}

export async function mockUpdateCodeItem(
  system: 'dict' | 'enum',
  id: string,
  payload: CodeItemSavePayload
) {
  await delay()
  if (!payload.code.trim()) throw new Error('编码不能为空')
  if (!payload.name.trim()) throw new Error('名称不能为空')
  if (!payload.description.trim()) throw new Error('描述不能为空')
  if (payload.sort < 0) throw new Error('排序号不能为负数')
  const store = itemStore(system)
  const target = store.find((item) => item.id === id)
  if (!target) throw new Error('编码项不存在')
  Object.assign(target, {
    code: payload.code.trim(),
    name: payload.name.trim(),
    description: payload.description.trim(),
    sort: payload.sort,
    updatedAt: now()
  })
  return clone(target)
}

export async function mockRemoveCodeItem(system: 'dict' | 'enum', id: string) {
  await delay()
  const store = itemStore(system)
  const index = store.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('编码项不存在')
  store.splice(index, 1)
}

// ---------- 选项加载 ----------

export async function mockLoadCodeOptions(system: 'dict' | 'enum', keyword?: string) {
  await delay()
  const store = typeStore(system)
  const kw = keyword?.trim().toLowerCase()
  let list = store
  if (kw) {
    list = store.filter(
      (t) => t.name.toLowerCase().includes(kw) || t.title.toLowerCase().includes(kw)
    )
  }
  return clone(list.map((t) => ({ label: `${t.title}-${t.name}`, value: t.id })))
}
