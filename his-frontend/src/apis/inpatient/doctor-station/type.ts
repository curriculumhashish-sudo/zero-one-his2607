import type { PageQuery } from '../../type'

/**
 * 住院医生站患者状态。
 *
 * 医生站患者队列只展示仍处于住院流程中的患者；已经开立出院医嘱的患者
 * 仍可能停留在队列中，直至护士站完成正式出院。
 */
export type InpatientEncounterStatus = 'IN_HOSPITAL' | 'DISCHARGE_PENDING'

/** 住院医生站患者队列中的单条后端数据。 */
export interface InpatientDoctorPatientQueueItemVO {
  /** 本次住院就诊 ID，住院医生站业务的主键。 */
  encounterId: string
  /** 患者档案 ID，用于查询患者跨就诊历史。 */
  patientId: string
  /** 患者姓名。 */
  patientName: string
  /** 性别名称。 */
  genderName: string
  /** 已格式化年龄，例如“46岁”。 */
  ageText: string
  /** 患者号。 */
  patientNo: string
  /** 住院号。 */
  inpatientNo: string
  /** 入院时间，格式为 YYYY-MM-DD HH:mm。 */
  admissionAt: string
  /** 当前科室名称。 */
  departmentName: string
  /** 当前病区 ID。 */
  wardId: string
  /** 当前病区名称。 */
  wardName: string
  /** 当前床位 ID。 */
  bedId: string
  /** 当前床位名称。 */
  bedName: string
  /** 住院状态码。 */
  encounterStatus: InpatientEncounterStatus
  /** 住院状态名称。 */
  encounterStatusName: string
  /** 主管医生姓名。 */
  attendingDoctorName?: string
  /** 责任护士姓名。 */
  primaryNurseName?: string
  /** 费用性质或医保类型名称。 */
  paymentTypeName?: string
  /** 当前诊断摘要。 */
  diagnosisSummary?: string
  /** 余额不足等需要突出显示的风险提示。 */
  riskHint?: string
  /** 拼音简码，用于队列关键字检索。 */
  pinyinCode?: string
  /** 就诊记录版本号，用于出院医嘱等状态操作的并发控制。 */
  version: number
}

/** 住院医生站患者队列查询参数。 */
export interface InpatientDoctorPatientQueueQuery extends PageQuery {
  /** 姓名、患者号、住院号、床号、诊断或拼音简码。 */
  keyword?: string
}

/** 住院病历文书状态。 */
export type InpatientMedicalRecordStatus = 'DRAFT' | 'SIGNED' | 'ARCHIVED'

/** 住院病历文书类型编码。 */
export type InpatientMedicalRecordDocumentTypeCode =
  | 'ADMISSION_RECORD'
  | 'FIRST_COURSE_RECORD'
  | 'DAILY_COURSE_RECORD'
  | 'DISCHARGE_RECORD'
  | 'PROGRESS_NOTE'

/** 可新建的住院病历文书类型。 */
export interface InpatientMedicalRecordDocumentTypeOptionVO {
  /** 文书类型编码。 */
  value: Exclude<InpatientMedicalRecordDocumentTypeCode, 'PROGRESS_NOTE'>
  /** 文书类型名称。 */
  label: string
  /** 新建文书时使用的默认标题。 */
  defaultTitle: string
  /** 新建文书时使用的初始正文。 */
  defaultContent: string
}

/**
 * 住院病历历史列表摘要。
 *
 * 历史查询按 patientId 跨住院就诊返回；encounterId 用于区分文书所属的
 * 具体住院记录，不能使用住院号代替。
 */
export interface InpatientMedicalRecordHistoryItemVO {
  /** 病历文书 ID。 */
  id: string
  /** 患者档案 ID。 */
  patientId: string
  /** 文书所属住院就诊 ID。 */
  encounterId: string
  /** 文书所属住院号。 */
  inpatientNo: string
  /** 文书类型编码。 */
  documentTypeCode: InpatientMedicalRecordDocumentTypeCode
  /** 文书类型名称。 */
  documentTypeName: string
  /** 文书标题。 */
  title: string
  /** 文书状态。 */
  status: InpatientMedicalRecordStatus
  /** 文书状态名称。 */
  statusName: string
  /** 文书所属科室。 */
  departmentName: string
  /** 书写医生姓名。 */
  authoredByName: string
  /** 书写时间，格式为 YYYY-MM-DD HH:mm:ss。 */
  authoredAt: string
  /** 最近更新时间，格式为 YYYY-MM-DD HH:mm:ss。 */
  updatedAt: string
}

/** 住院病历历史分页查询参数。 */
export interface InpatientMedicalRecordHistoryQuery extends PageQuery {
  /** 患者档案 ID，用于查询患者跨住院历史。 */
  patientId: string
  /** 当前住院就诊 ID；传入后当前住院文书优先展示。 */
  currentEncounterId?: string
  /** 标题、文书类型、住院号、科室或医生关键词。 */
  keyword?: string
  /** 可选文书类型编码。 */
  documentTypeCode?: string
}

/** 住院病历文书详情。 */
export interface InpatientMedicalRecordVO extends InpatientMedicalRecordHistoryItemVO {
  /** 富文本病历正文。 */
  content: string
  /** 文书版本号。 */
  version: number
  /** 当前登录医生是否只能查看该文书。 */
  readonly: boolean
}

/** 住院病历历史详情。 */
export interface InpatientMedicalRecordHistoryDetailVO extends InpatientMedicalRecordVO {
  /** 历史详情固定只读。 */
  readonly: true
}

/** 保存住院病历草稿参数。 */
export interface SaveInpatientMedicalRecordDTO {
  /** 文书 ID；首次保存时不传。 */
  id?: string
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 文书类型编码。 */
  documentTypeCode: Exclude<InpatientMedicalRecordDocumentTypeCode, 'PROGRESS_NOTE'>
  /** 文书标题。 */
  title: string
  /** 富文本病历正文。 */
  content: string
  /** 页面加载时取得的版本号；首次保存为 0。 */
  version: number
}

/** 签名住院病历参数。 */
export interface SignInpatientMedicalRecordDTO {
  /** 文书 ID。 */
  id: string
  /** 页面加载时取得的版本号。 */
  version: number
}

/** 住院诊断记录。 */
export interface InpatientDiagnosisRecordVO {
  /** 诊断记录 ID。 */
  id: string
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 诊断字典项目 ID。 */
  catalogId: string
  /** ICD 编码。 */
  code: string
  /** 诊断名称。 */
  name: string
  /** 诊断分类名称。 */
  typeName: string
  /** 是否为主诊断。 */
  primary: boolean
  /** 诊断显示顺序。 */
  orderNo: number
  /** 诊断时间，格式为 YYYY-MM-DD HH:mm:ss。 */
  diagnosedAt: string
  /** 最近更新时间，格式为 YYYY-MM-DD HH:mm:ss。 */
  updatedAt: string
  /** 记录版本号，用于修改和删除时的并发控制。 */
  version: number
}

/** 当前住院诊断记录分页查询条件。 */
export interface InpatientDiagnosisRecordQuery extends PageQuery {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** ICD 编码、诊断名称或诊断分类关键词。 */
  keyword?: string
}

/** 诊断字典候选项。 */
export interface InpatientDiagnosisDictionaryItemVO extends Record<string, unknown> {
  /** 诊断字典项目 ID。 */
  id: string
  /** ICD 编码。 */
  code: string
  /** 诊断名称。 */
  name: string
  /** 诊断分类名称。 */
  typeName: string
}

/** 诊断字典分页查询条件。 */
export interface InpatientDiagnosisDictionaryQuery extends PageQuery {
  /** ICD 编码、诊断名称或诊断分类关键词。 */
  keyword?: string
}

/** 医生诊断使用历史。 */
export interface InpatientDiagnosisUsageHistoryVO extends InpatientDiagnosisDictionaryItemVO {
  /** 历史使用次数。 */
  useCount: number
  /** 最近使用时间，格式为 YYYY-MM-DD HH:mm:ss。 */
  lastUsedAt: string
}

/** 诊断使用历史查询条件。 */
export interface InpatientDiagnosisUsageHistoryQuery {
  /** ICD 编码或诊断名称关键词。 */
  keyword?: string
  /** 最多返回条数。 */
  limit?: number
}

/** 新增住院诊断参数。 */
export interface CreateInpatientDiagnosisDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 诊断字典项目 ID。 */
  catalogId: string
  /** ICD 编码。 */
  code: string
  /** 诊断名称。 */
  name: string
  /** 诊断分类名称。 */
  typeName: string
  /** 是否为主诊断。 */
  primary: boolean
  /** 诊断显示顺序。 */
  orderNo: number
}

/** 修改住院诊断参数。 */
export interface UpdateInpatientDiagnosisDTO {
  /** 诊断字典项目 ID。 */
  catalogId: string
  /** ICD 编码。 */
  code: string
  /** 诊断名称。 */
  name: string
  /** 诊断分类名称。 */
  typeName: string
  /** 是否为主诊断。 */
  primary: boolean
  /** 诊断显示顺序。 */
  orderNo: number
  /** 当前记录版本号。 */
  version: number
}

/** 住院医嘱状态。 */
export type InpatientMedicalOrderStatus = 'DRAFT' | 'SIGNED' | 'STOPPED' | 'WITHDRAWN'

/** 住院医嘱长临类型。 */
export type InpatientMedicalOrderTherapyType = 'LONG_TERM' | 'TEMPORARY'

/** 住院医嘱记录。 */
export interface InpatientMedicalOrderRecordVO {
  /** 医嘱记录 ID。 */
  id: string
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 医嘱项目目录 ID。 */
  catalogId: string
  /** 医嘱项目编码。 */
  code: string
  /** 医嘱项目名称。 */
  name: string
  /** 医嘱项目分类。 */
  category: 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'
  /** 医嘱项目分类名称。 */
  categoryName: string
  /** 规格。 */
  specification?: string
  /** 单次剂量。 */
  dose?: number
  /** 剂量单位。 */
  unit?: string
  /** 执行频次。 */
  frequency?: string
  /** 给药途径。 */
  route?: string
  /** 疗程天数。 */
  days?: number
  /** 开立数量。 */
  quantity: number
  /** 数量单位。 */
  quantityUnit: string
  /** 医嘱备注。 */
  remark?: string
  /** 长期或临时医嘱。 */
  therapyType: InpatientMedicalOrderTherapyType
  /** 长临类型名称。 */
  therapyTypeName: string
  /** 成组 ID；未成组时为空。 */
  groupId?: string
  /** 医嘱状态。 */
  status: InpatientMedicalOrderStatus
  /** 医嘱状态名称。 */
  statusName: string
  /** 显示顺序。 */
  orderNo: number
  /** 开立时间。 */
  authoredAt: string
  /** 签发时间。 */
  signedAt?: string
  /** 停嘱时间。 */
  stoppedAt?: string
  /** 最近更新时间。 */
  updatedAt: string
  /** 记录版本号，用于并发控制。 */
  version: number
}

/** 当前住院医嘱分页查询条件。 */
export interface InpatientMedicalOrderQuery extends PageQuery {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 医嘱编码、名称、分类或备注关键词。 */
  keyword?: string
  /** 可选医嘱状态。 */
  status?: InpatientMedicalOrderStatus
  /** 可选长临类型。 */
  therapyType?: InpatientMedicalOrderTherapyType
}

/** 住院医嘱项目目录候选项。 */
export interface InpatientMedicalOrderCatalogItemVO extends Record<string, unknown> {
  /** 医嘱项目目录 ID。 */
  id: string
  /** 项目编码。 */
  code: string
  /** 项目名称。 */
  name: string
  /** 项目分类。 */
  category: 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'
  /** 项目分类名称。 */
  categoryName: string
  /** 规格。 */
  specification?: string
  /** 默认单次剂量。 */
  defaultDose?: number
  /** 默认剂量单位。 */
  defaultUnit?: string
  /** 默认执行频次。 */
  defaultFrequency?: string
  /** 默认给药途径。 */
  defaultRoute?: string
  /** 默认疗程天数。 */
  defaultDays?: number
  /** 默认开立数量。 */
  defaultQuantity?: number
  /** 数量单位。 */
  quantityUnit?: string
  /** 默认长临类型。 */
  defaultTherapyType: InpatientMedicalOrderTherapyType
}

/** 住院医嘱项目目录分页查询条件。 */
export interface InpatientMedicalOrderCatalogQuery extends PageQuery {
  /** 项目编码、名称或分类关键词。 */
  keyword?: string
  /** 可选项目分类。 */
  category?: 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'
}

/** 新增住院医嘱参数。 */
export interface CreateInpatientMedicalOrderDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 医嘱项目目录 ID。 */
  catalogId: string
  /** 医嘱项目编码。 */
  code: string
  /** 医嘱项目名称。 */
  name: string
  /** 医嘱项目分类。 */
  category: 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'
  /** 医嘱项目分类名称。 */
  categoryName: string
  /** 规格。 */
  specification?: string
  /** 单次剂量。 */
  dose?: number
  /** 剂量单位。 */
  unit?: string
  /** 执行频次。 */
  frequency?: string
  /** 给药途径。 */
  route?: string
  /** 疗程天数。 */
  days?: number
  /** 开立数量。 */
  quantity: number
  /** 数量单位。 */
  quantityUnit: string
  /** 医嘱备注。 */
  remark?: string
  /** 长期或临时医嘱。 */
  therapyType: InpatientMedicalOrderTherapyType
  /** 显示顺序。 */
  orderNo: number
}

/** 修改住院医嘱参数。 */
export interface UpdateInpatientMedicalOrderDTO extends Omit<
  CreateInpatientMedicalOrderDTO,
  'encounterId' | 'patientId'
> {
  /** 当前记录版本号。 */
  version: number
}

/** 批量医嘱动作中的单条并发控制信息。 */
export interface InpatientMedicalOrderBatchItemDTO {
  /** 医嘱记录 ID。 */
  id: string
  /** 当前记录版本号。 */
  version: number
}

/** 住院医嘱批量状态动作参数。 */
export interface InpatientMedicalOrderBatchActionDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 目标医嘱。 */
  items: InpatientMedicalOrderBatchItemDTO[]
}

/** 住院医嘱批量并组参数。 */
export interface GroupInpatientMedicalOrdersDTO extends InpatientMedicalOrderBatchActionDTO {
  /** 可选成组 ID；未传时由服务端生成。 */
  groupId?: string
}

/** 住院医嘱批量操作结果。 */
export interface InpatientMedicalOrderBatchResultVO {
  /** 成功更新条数。 */
  updatedCount: number
  /** 操作完成后的目标医嘱。 */
  rows: InpatientMedicalOrderRecordVO[]
}

/** 公共医嘱适配器使用的草稿保存行。 */
export interface SaveInpatientMedicalOrderDraftItemDTO extends Omit<
  CreateInpatientMedicalOrderDTO,
  'encounterId' | 'patientId'
> {
  /** 已保存医嘱 ID；新增草稿为空。 */
  id?: string
  /** 已保存医嘱版本号；新增草稿为空。 */
  version?: number
}

/** 公共医嘱适配器生成的住院草稿保存参数。 */
export interface SaveInpatientMedicalOrderDraftsDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 草稿行。 */
  items: SaveInpatientMedicalOrderDraftItemDTO[]
}

/** 出院医嘱前置校验提示级别。 */
export type InpatientDischargeAdviceCheckLevel = 'BLOCKER' | 'REMINDER'

/** 出院医嘱前置校验项。 */
export interface InpatientDischargeAdviceCheckItemVO {
  /** 稳定校验编码。 */
  code:
    | 'DISCHARGE_ALREADY_PENDING'
    | 'DIAGNOSIS_MISSING'
    | 'MEDICAL_RECORD_DRAFT'
    | 'ORDER_DRAFT'
    | 'LONG_TERM_ORDER_ACTIVE'
  /** 阻断或提醒。 */
  level: InpatientDischargeAdviceCheckLevel
  /** 给医生展示的说明。 */
  message: string
  /** 涉及记录数。 */
  count?: number
  /** 建议跳转的工作区。 */
  targetTab?: 'medicalRecord' | 'diagnosis' | 'orders'
}

/** 出院方式选项。 */
export interface InpatientDischargeWayOptionVO {
  /** 后端字典编码。 */
  value: string
  /** 展示名称。 */
  label: string
}

/** 已开立的出院医嘱。 */
export interface InpatientDischargeAdviceVO {
  /** 出院医嘱 ID。 */
  id: string
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 计划出院时间。 */
  plannedDischargeTime: string
  /** 出院方式编码。 */
  outWayCode: string
  /** 出院方式名称。 */
  outWayName: string
  /** 出院原因。 */
  reasonText?: string
  /** 原科室 ID。 */
  sourceOrgId?: string
  /** 原病区或床位 ID。 */
  sourceLocationId?: string
  /** 开立时间。 */
  createdAt: string
  /** 开立医生。 */
  createdByName: string
  /** 记录版本号。 */
  version: number
}

/** 打开出院医嘱弹窗前加载的数据。 */
export interface InpatientDischargeAdvicePreparationVO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 当前就诊版本。 */
  encounterVersion: number
  /** 是否允许开立。 */
  canCreate: boolean
  /** 前置校验结果。 */
  checks: InpatientDischargeAdviceCheckItemVO[]
  /** 出院方式字典。 */
  outWayOptions: InpatientDischargeWayOptionVO[]
  /** 已存在的出院医嘱。 */
  existingAdvice?: InpatientDischargeAdviceVO
}

/** 开立出院医嘱参数。 */
export interface CreateInpatientDischargeAdviceDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 当前就诊版本。 */
  encounterVersion: number
  /** 计划出院时间。 */
  plannedDischargeTime: string
  /** 出院方式编码。 */
  outWayCode: string
  /** 出院原因。 */
  reasonText?: string
  /** 原科室 ID。 */
  sourceOrgId?: string
  /** 原病区或床位 ID。 */
  sourceLocationId?: string
}
