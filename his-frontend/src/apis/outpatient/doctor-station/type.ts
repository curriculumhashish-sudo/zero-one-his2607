import type { PageDTO, PageQuery } from '../../type'

/**
 * 门诊患者当前的就诊状态。
 *
 * 这里保留后端风格的状态码，页面不会直接拿状态码做展示，
 * 而是通过 adapter.ts 转换为中文和对应的语义色。
 */
export type DoctorVisitStatus = 'arrived' | 'in-progress' | 'finished'

/**
 * 医生工作站患者队列中的单条后端数据。
 */
export interface DoctorPatientQueueItemVO {
  /** 本次就诊记录 ID，也是医生站选择患者时使用的唯一标识 */
  encounterId: string
  /** 患者档案 ID */
  patientId: string
  /** 患者姓名 */
  patientName: string
  /** 性别名称 */
  gender: string
  /** 年龄 */
  age: number
  /** 就诊号（后端唯一返回的号码，与后端字段对齐） */
  visitNo: string
  /** 当前排队号 */
  queueNumber: number
  /** 挂号或到诊时间，格式为 YYYY-MM-DD HH:mm */
  visitTime: string
  /** 接诊科室名称 */
  departmentName: string
  /** 门诊服务类型，例如普通门诊、专家门诊 */
  registerType: string
  /** 就诊状态码 */
  visitStatus: DoctorVisitStatus
  /** 支付类型名称 */
  paymentTypeName: string
  /** 优先级提示，仅演示数据展示，页面未使用 */
  priorityName?: string
}

/**
 * 患者队列单日查询参数。
 *
 * 医生站只查询某一天属于当前医生的患者，不做分页；患者较多时由左侧列表
 * 在浏览器内滚动，避免翻页后正在处理的患者从操作视野中消失。
 */
export interface DoctorPatientQueueQuery {
  /** 患者姓名，仅支持按姓名模糊查询 */
  patientName?: string
  /** 查询日期，格式为 YYYY-MM-DD */
  visitDate: string
  /** 就诊状态；不传表示查询全部状态 */
  visitStatus?: DoctorVisitStatus
}

/**
 * GET /cycleCtrl/queue 返回的患者队列数据。
 *
 * 该类型保持后端字段名称，API 层会将 payType、visitStatus 等字段转换为
 * 页面使用的 DoctorPatientQueueItemVO。
 */
export interface PatientQueueDTO {
  /** 患者档案 ID */
  patientId?: string
  /** 本次就诊记录 ID */
  encounterId?: string
  /** 患者姓名 */
  patientName: string
  /** 性别 */
  gender: string
  /** 年龄 */
  age: number
  /** 挂号类型 */
  registerType: string
  /** 就诊时间 */
  visitTime: string
  /** 病历号 */
  visitNo: string
  /** 支付类型 */
  payType: string
  /** 后端就诊状态 */
  visitStatus: string
  /** 排队号 */
  queueNumber?: number
  /** 科室名称 */
  departmentName?: string
  /** 优先级名称 */
  priorityName?: string
}

/**
 * 医生选择患者后加载的就诊上下文。
 *
 * 队列 VO 只提供卡片摘要；上下文提供医生开展诊疗所需的详细信息。
 */
export interface DoctorPatientContextVO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 患者档案 ID */
  patientId: string
  /** 当前就诊状态码 */
  visitStatus: DoctorVisitStatus
  /** 当前就诊状态名称 */
  visitStatusName: string
  /** 接诊医生姓名 */
  attendingDoctorName?: string
  /** 接诊时间 */
  acceptedAt?: string
  /** 联系电话 */
  phone: string
}

/**
 * 门诊病历详情。
 *
 * content 使用 HTML 字符串，是因为公共 RichTextEditorField 组件通过 HTML 读写内容。
 */
export interface DoctorMedicalRecordVO {
  /** 病历记录 ID；尚未首次保存时可能为空 */
  id?: string
  /** 本次就诊记录 ID */
  encounterId: string
  /** 富文本病历内容 */
  content: string
  /** 最近保存时间，格式为 YYYY-MM-DD HH:mm:ss */
  updatedAt?: string
}

/**
 * 保存门诊病历时提交的数据。
 */
export interface SaveDoctorMedicalRecordDTO {
  /** 富文本病历内容 */
  content: string
}

/**
 * 患者历史门诊病历分页查询参数。
 *
 * 历史病历按患者档案查询，而不是按当前医生的患者队列查询，
 * 因此能够查看患者在其他科室、其他医生处已经完成的门诊病历。
 */
export interface DoctorMedicalRecordHistoryQuery extends PageQuery {
  /** 患者档案 ID */
  patientId: string
  /** 就诊号、科室、医生或主诊断关键词 */
  keyword?: string
}

/**
 * 患者历史门诊病历摘要。
 *
 * 列表只返回轻量摘要；完整富文本仍通过 getMedicalRecord 按就诊 ID 加载，
 * 避免分页列表一次传输大量 HTML。
 */
export interface DoctorMedicalRecordHistoryItemVO {
  /** 历史就诊记录 ID */
  encounterId: string
  /** Visit time returned by the outpatient encounter service. */
  visitDate: string
  /** Primary diagnosis description, when available. */
  diagnosis?: string
  /** Attending doctor name, when available. */
  doctorName?: string
  /** Department name, when available. */
  deptName?: string
}

/**
 * 门诊诊断记录。
 */
export interface DoctorDiagnosisVO {
  /** 诊断记录 ID */
  id: string
  /** 本次就诊记录 ID */
  encounterId: string
  /** 诊断字典项目 ID */
  catalogId: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称 */
  typeName: string
  /** 是否为主诊断 */
  primary: boolean
  /** 诊断时间，格式为 YYYY-MM-DD HH:mm:ss */
  diagnosedAt: string
  /** 显示顺序，从 1 开始 */
  orderNo: number
}

/**
 * 当前就诊的诊断列表。
 */
export interface DoctorDiagnosisListVO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 诊断列表 */
  rows: DoctorDiagnosisVO[]
  /** 最近保存时间 */
  updatedAt?: string
}

/**
 * 诊断字典查询参数。
 */
export interface DoctorDiagnosisDictionaryQuery extends PageQuery {
  /** ICD 编码或诊断名称关键词 */
  keyword?: string
}

/**
 * 诊断字典候选项。
 *
 * extends Record 便于在通用选择表格中直接作为行数据使用。
 */
export interface DoctorDiagnosisDictionaryItemVO extends Record<string, unknown> {
  /** 诊断字典项目 ID */
  id: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称 */
  typeName: string
}

/** 诊断使用历史或常用诊断候选项。 */
export interface DoctorDiagnosisUsageHistoryItemVO extends Record<string, unknown> {
  /** 诊断字典项目 ID */
  catalogId: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称 */
  typeName: string
  /** 使用次数 */
  useCount: number
  /** 最近使用时间 */
  lastUsedAt: string
}

/** 分页查询诊断使用历史。 */
export interface DoctorDiagnosisUsageHistoryQuery extends PageQuery {
  /** 当前患者 ID；用于生成患者历史候选 */
  patientId?: string
  /** ICD 编码或诊断名称 */
  keyword?: string
}

/** 患者跨就诊历史诊断记录。 */
export interface DoctorPatientDiagnosisHistoryItemVO extends DoctorDiagnosisVO {
  /** 门诊就诊号 */
  visitNo: string
  /** 就诊科室 */
  departmentName: string
  /** 接诊医生 */
  attendingDoctorName: string
  /** 就诊时间 */
  visitedAt: string
}

/** 患者历史诊断条件分页查询。 */
export interface DoctorPatientDiagnosisHistoryQuery extends PageQuery {
  /** 患者档案 ID */
  patientId: string
  /** 就诊号、科室、医生、ICD 编码或诊断名称 */
  keyword?: string
  /** 诊断开始日期，YYYY-MM-DD */
  startDate?: string
  /** 诊断结束日期，YYYY-MM-DD */
  endDate?: string
}

/**
 * 保存时提交的单条诊断。
 */
export interface SaveDoctorDiagnosisItemDTO {
  /** 已有诊断记录 ID；新诊断为空 */
  id?: string
  /** 诊断字典项目 ID */
  catalogId: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断分类名称 */
  typeName: string
  /** 是否为主诊断 */
  primary: boolean
  /** 显示顺序，从 1 开始 */
  orderNo: number
}

/**
 * 后端单条新增诊断请求体。
 *
 * 字段名与后端 /opdoctor/diagnose 对齐；conditionId、diagCode 已确认，
 * diagTypeCode 的分类编码值待后端提供编码表。
 */
export interface OpdoctorCreateDiagnosisDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 病情/诊断字典 ID（对应前端 catalogId，语义待确认） */
  conditionId: string
  /** ICD 编码（后端为整数类型，类型待确认） */
  diagCode: string
  /** 诊断描述 */
  description: string
  /** 诊断分类编码 */
  diagTypeCode: string
  /** 是否主诊断：1 是、0 否 */
  primaryFlag: 0 | 1
  /** 排序 */
  rank: number
}

/** 门诊基础版支持的医嘱分类。 */
export type DoctorAdviceOrderCategory = 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'

/** 门诊医嘱生命周期状态。 */
export type DoctorAdviceOrderStatus = 'DRAFT' | 'SIGNED' | 'WITHDRAWN'

/** 已保存的门诊医嘱。 */
export interface DoctorAdviceOrderVO {
  /** 医嘱记录 ID */
  id: string
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱项目目录 ID */
  catalogId: string
  /** 项目编码 */
  code: string
  /** 项目名称 */
  name: string
  /** 项目分类 */
  category: DoctorAdviceOrderCategory
  /** 项目分类名称 */
  categoryName: string
  /** 规格 */
  specification?: string
  /** 单次剂量 */
  dose?: number
  /** 剂量单位 */
  unit?: string
  /** 执行频次 */
  frequency?: string
  /** 给药途径 */
  route?: string
  /** 疗程天数 */
  days?: number
  /** 开立数量 */
  quantity: number
  /** 数量单位 */
  quantityUnit: string
  /** 一个开立数量单位包含的剂量 */
  dosePerQuantityUnit?: number
  /** 医嘱备注 */
  remark?: string
  /** 医嘱状态 */
  status: DoctorAdviceOrderStatus
  /** 任意医嘱分组 ID；为空表示未分组 */
  groupId?: string
  /** 签发后生成的处方号 */
  prescriptionNo?: string
  /** 非药品医嘱签发后生成的服务申请 ID，用于打印申请单 */
  serviceRequestId?: string
  /** 显示顺序 */
  orderNo: number
  /** Single-order optimistic-lock version returned by the backend. */
  version?: number
  /** History-only fields returned by /historyOrder/detail. */
  executedCount?: number
  sampleType?: string
  executeHospital?: string
  createTime?: string
  diagnosis?: string
  doctorName?: string
  isSkinTest?: boolean
}

/**
 * 当前就诊的医嘱列表。
 *
 * version 为医嘱列表整体版本，由 API 层维护缓存并自动携带，页面无需感知。
 */
export interface DoctorAdviceOrderListVO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表 */
  rows: DoctorAdviceOrderVO[]
  /** 最近保存时间 */
  updatedAt?: string
  /** 医嘱列表整体版本；取最近一次查询或操作响应中的值 */
  version?: number
}

/**
 * 当前就诊医嘱分页结果。
 *
 * version 语义同上，供 API 层缓存后传给变更接口。
 */
export interface DoctorAdviceOrderPageVO extends PageDTO<DoctorAdviceOrderVO> {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 最近变更时间 */
  updatedAt?: string
  /** 医嘱列表整体版本 */
  version?: number
}

/** 当前就诊医嘱分页查询参数。 */
export interface DoctorAdviceOrderQuery extends PageQuery {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 项目编码、名称或备注关键词 */
  keyword?: string
  /** 医嘱分类 */
  category?: DoctorAdviceOrderCategory
  /** 医嘱状态 */
  status?: DoctorAdviceOrderStatus
}

/** 医嘱项目目录查询参数。 */
export interface DoctorAdviceOrderCatalogQuery extends PageQuery {
  /** 项目编码或名称关键词 */
  keyword?: string
  /** 可选项目分类 */
  category?: DoctorAdviceOrderCategory
}

/** 医嘱项目目录候选项。 */
export interface DoctorAdviceOrderCatalogItemVO extends Record<string, unknown> {
  /** 项目目录 ID */
  id: string
  /** 项目编码 */
  code: string
  /** 项目名称 */
  name: string
  /** 项目分类 */
  category: DoctorAdviceOrderCategory
  /** 项目分类名称 */
  categoryName: string
  /** 规格 */
  specification?: string
  /** 默认单次剂量 */
  defaultDose?: number
  /** 默认剂量单位 */
  defaultUnit?: string
  /** 默认执行频次 */
  defaultFrequency?: string
  /** 默认给药途径 */
  defaultRoute?: string
  /** 默认疗程天数 */
  defaultDays?: number
  /** 默认开立数量 */
  defaultQuantity: number
  /** 数量单位 */
  quantityUnit: string
  /** 一个开立数量单位包含的剂量，例如每片 5mg */
  dosePerQuantityUnit?: number
}

/** 保存时提交的单条门诊医嘱。 */
export interface SaveDoctorAdviceOrderItemDTO {
  /** 已有医嘱记录 ID；新草稿为空 */
  id?: string
  /** 医嘱项目目录 ID */
  catalogId: string
  /** 项目编码 */
  code: string
  /** 项目名称 */
  name: string
  /** 项目分类 */
  category: DoctorAdviceOrderCategory
  /** 项目分类名称 */
  categoryName: string
  /** 规格 */
  specification?: string
  /** 单次剂量 */
  dose?: number
  /** 剂量单位 */
  unit?: string
  /** 执行频次 */
  frequency?: string
  /** 给药途径 */
  route?: string
  /** 疗程天数 */
  days?: number
  /** 开立数量 */
  quantity: number
  /** 数量单位 */
  quantityUnit: string
  /** 一个开立数量单位包含的剂量 */
  dosePerQuantityUnit?: number
  /** 医嘱备注 */
  remark?: string
  /** 医嘱分组 ID */
  groupId?: string
  /** 显示顺序 */
  orderNo: number
}

/** 保存当前就诊完整医嘱列表的请求。 */
export interface SaveDoctorAdviceOrdersDTO {
  /** 当前完整医嘱列表 */
  items: SaveDoctorAdviceOrderItemDTO[]
}

/** 单条新增医嘱请求。 */
export interface CreateDoctorAdviceOrderDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 新增草稿 */
  item: SaveDoctorAdviceOrderItemDTO
}

/** 单条修改医嘱请求。 */
export interface UpdateDoctorAdviceOrderDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 待修改医嘱 ID */
  adviceId: string
  /** 医嘱列表整体版本；API 层自动填充最近一次查询/操作的版本，页面可不传 */
  version?: number
  /** 修改后的完整草稿 */
  item: SaveDoctorAdviceOrderItemDTO
}

/** 批量删除草稿医嘱请求。 */
export interface DeleteDoctorAdviceOrdersDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表整体版本；API 层自动填充，页面可不传 */
  version?: number
  /** 待删除医嘱 ID */
  adviceIds: string[]
}

/** 任意医嘱并组请求。 */
export interface GroupDoctorAdviceOrdersDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表整体版本；API 层自动填充，页面可不传 */
  version?: number
  /** 至少两条草稿医嘱 ID */
  adviceIds: string[]
  /** 目标组 ID；契约注明由后端生成，联调后前端不再传 */
  groupId?: string
}

/** 医嘱拆组请求。 */
export interface UngroupDoctorAdviceOrdersDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表整体版本；API 层自动填充，页面可不传 */
  version?: number
  /** 需要清除分组的草稿医嘱 ID */
  adviceIds: string[]
}

/** 批量签发医嘱请求。 */
export interface SignDoctorAdviceOrdersDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表整体版本；API 层自动填充，页面可不传 */
  version?: number
  /** 待签发草稿医嘱 ID */
  adviceIds: string[]
}

/** 批量撤回医嘱请求。 */
export interface WithdrawDoctorAdviceOrdersDTO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表整体版本；API 层自动填充，页面可不传 */
  version?: number
  /** 待撤回已签发医嘱 ID */
  adviceIds: string[]
}

/** 患者历史医嘱就诊摘要。 */
export interface DoctorAdviceOrderHistoryEncounterVO {
  /** Patient archive identifier used by the history query. */
  patientId?: string
  /** 历史就诊记录 ID */
  encounterId: string
  /** 门诊就诊号 */
  visitNo: string
  /** 就诊科室 */
  departmentName: string
  /** 接诊医生 */
  attendingDoctorName: string
  /** 就诊时间 */
  visitedAt: string
  /** 已签发医嘱数量 */
  signedCount: number
  /** 已撤回医嘱数量 */
  withdrawnCount: number
  /** Backend history-order fields. Legacy aliases above remain for Mock mode. */
  visitTime?: string
  visitType?: string
  mainDiagnosis?: string
  doctorName?: string
  drugCount?: number
  examCount?: number
  materialCount?: number
}

/** 患者历史医嘱就诊列表查询参数。 */
export interface DoctorAdviceOrderHistoryQuery extends PageQuery {
  /** 患者档案 ID */
  patientId: string
  /** 排除当前就诊 */
  excludeEncounterId?: string
  /** 就诊号、科室或医生关键词 */
  keyword?: string
}

/** 历史医嘱详情分页查询参数。 */
export interface DoctorAdviceOrderHistoryDetailQuery extends PageQuery {
  /** 患者档案 ID，用于越权校验 */
  patientId: string
  /** 历史就诊记录 ID */
  encounterId: string
  /** 医嘱名称、编码或备注关键词 */
  keyword?: string
  /** 医嘱分类 */
  category?: DoctorAdviceOrderCategory
  /** 已签发或已撤回状态 */
  status?: Extract<DoctorAdviceOrderStatus, 'SIGNED' | 'WITHDRAWN'>
}
