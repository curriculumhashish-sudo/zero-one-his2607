import type { PageQuery } from '../../type'

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
  /** 后端就诊状态码。 */
  statusEnum: string
  /** 当前诊断摘要。 */
  diagnosisSummary?: string
}

/** 住院医生站患者队列查询参数。 */
export interface InpatientDoctorPatientQueueQuery extends PageQuery {
  /** 姓名、住院号或床号。 */
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
  id: string
  documentTypeName: string
  title: string
  status: InpatientMedicalRecordStatus
  patientId: string
  encounterId: string
  inpatientNo: string
  documentTypeCode: InpatientMedicalRecordDocumentTypeCode
  statusName: string
  departmentName: string
  authoredByName: string
  authoredAt: string
  updatedAt: string
}

/** 住院病历历史分页查询参数。 */
export interface InpatientMedicalRecordHistoryQuery extends PageQuery {
  /** 患者档案 ID，用于查询患者跨住院历史。 */
  patientId: string
  /** Backend currently supports title filtering only. */
  keyword?: string
  currentEncounterId?: string
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
  version?: number
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
  id: string
  conditionId: string
  /** Diagnosis definition code returned by the diagnosis-entry detail API. */
  definitionCode: string
  description: string
  /** Medical category name, for example Chinese medicine or Western medicine. */
  kindName: string
  /** Diagnosis type code used by the diagnosis modify API. */
  diagnosisTypeCode: string
  /** Diagnosis type display name. */
  diagnosisTypeName: string
  primaryFlag: number
  rank: number
  tcmFlag: number
}

export interface InpatientDiagnosisRecordQuery extends PageQuery {
  encounterId: string
  patientId: string
}

export interface InpatientDiagnosisBriefVO {
  definitionName: string
}

export interface CreateInpatientDiagnosisDTO {
  encounterId: string
  patientId: string
  definitionCode?: string
  description: string
  diagnosisTypeCode?: string
  primaryFlag: number
  rank: number
  tcmFlag: number
}

export interface UpdateInpatientDiagnosisDTO {
  id: string
  diagnosisTypeCode: string
}

export interface DeleteInpatientDiagnosisDTO {
  id: string
  conditionId: string
}

export interface InpatientDiagnosisAddResultVO {
  diagnosisId: string
  conditionId: string
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
  /** 记录来源；Real 模式记录使用真实写接口，Mock 模式记录使用本地接口。 */
  source?: 'mock' | 'real'
  /** 后端生命周期动作所需的产品表名；当前后端列表未返回。 */
  productTableName?: string
  /** 是否为历史住院记录或其他只读记录。 */
  readonly?: boolean
  /** 是否与当前工作区 encounterId 不一致。 */
  encounterMismatch?: boolean
  /** 后端原始状态值，用于兼容未纳入前端枚举的状态。 */
  rawStatus?: string
  /** 后端返回的开立医生标识。 */
  authoredByName?: string
  /** 后端返回的总价文本。 */
  totalPrice?: string
  /** 后端返回的皮试标志。 */
  skinTest?: boolean
}

/** 当前住院医嘱分页查询条件。 */
export interface InpatientMedicalOrderQuery extends PageQuery {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID；Real 查询接口使用该字段，Mock 查询可不传。 */
  patientId?: string
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
  /** 后端生命周期接口所需的产品定义表名。 */
  productTableName?: string
  /** 当前医嘱分组 ID，用于并组和拆组。 */
  groupId?: string
}

/** 住院医嘱批量状态动作参数。 */
export interface InpatientMedicalOrderBatchActionDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 患者档案 ID。 */
  patientId: string
  /** 目标医嘱。 */
  items: InpatientMedicalOrderBatchItemDTO[]
  /** Real 停嘱接口要求的原因码。Mock 模式不使用。 */
  stopReason?: number
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
  /** 部分成功操作的失败数量。 */
  failedCount?: number
  /** 部分成功操作的失败原因。 */
  failureMessages?: string[]
  /** 并组成功后后端返回的分组 ID。 */
  groupId?: string
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

/** 已开立的出院医嘱。 */
export interface InpatientDischargeAdviceVO {
  /** 出院医嘱 ID。 */
  id: string
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 医嘱状态。 */
  statusEnum: string
  /** 计划出院时间。 */
  plannedDischargeTime: string
  /** 出院方式编码。 */
  outWayCode: string
  /** 出院方式名称。 */
  outWayName: string
  /** 创建医嘱后的就诊状态。 */
  encounterNewStatus: string
}

/** 开立出院医嘱参数。 */
export interface CreateInpatientDischargeAdviceDTO {
  /** 本次住院就诊 ID。 */
  encounterId: string
  /** 计划出院时间。 */
  plannedDischargeTime: string
  /** 出院方式编码。 */
  outWayCode: string
}
