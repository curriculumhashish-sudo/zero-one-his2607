import type { PageQuery } from '../../type'

/**
 * 门诊患者当前的就诊状态。
 *
 * 这里保留后端风格的状态码，页面不会直接拿状态码做展示，
 * 而是通过 adapter.ts 转换为中文和对应的语义色。
 */
export type DoctorVisitStatus = 'WAITING' | 'IN_PROGRESS' | 'COMPLETED'

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
  genderName: string
  /** 年龄 */
  age: number
  /** 门诊病历号 */
  patientNo: string
  /** 本次就诊号 */
  visitNo: string
  /** 当前排队号 */
  queueNumber: number
  /** 挂号或到诊时间，格式为 YYYY-MM-DD HH:mm */
  registeredAt: string
  /** 接诊科室名称 */
  departmentName: string
  /** 门诊服务类型，例如普通门诊、专家门诊 */
  visitTypeName: string
  /** 就诊状态码 */
  visitStatus: DoctorVisitStatus
  /** 就诊状态名称 */
  visitStatusName: string
  /** 支付类型名称 */
  paymentTypeName: string
  /** 优先级提示 */
  priorityName?: string
  /** 风险或过敏提示 */
  riskHint?: string
}

/**
 * 患者队列查询参数。
 *
 * 继承 PageQuery 后自动拥有 pageIndex 和 pageSize；
 * 虽然当前队列页面不展示分页器，接口仍保留分页能力，方便接入真实后端。
 */
export interface DoctorPatientQueueQuery extends PageQuery {
  /** 姓名、患者号或就诊号关键词 */
  keyword?: string
  /** 查询开始日期，格式为 YYYY-MM-DD */
  startDate?: string
  /** 查询结束日期，格式为 YYYY-MM-DD */
  endDate?: string
  /** 就诊状态；不传表示查询全部状态 */
  visitStatus?: DoctorVisitStatus
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
  /** 分诊主诉 */
  chiefComplaint: string
  /** 过敏史 */
  allergyHistory: string
  /** 既往病史 */
  pastMedicalHistory: string
  /** 联系电话 */
  phone: string
  /** 紧急联系人 */
  emergencyContact: string
  /** 分诊备注 */
  triageNote?: string
}

/**
 * 门诊病历详情。
 *
 * content 使用 HTML 字符串，是因为公共 RichTextEditorField 组件通过 HTML 读写内容。
 * version 用于乐观锁：保存时把当前版本传回后端，可避免覆盖其他人刚刚保存的内容。
 */
export interface DoctorMedicalRecordVO {
  /** 病历记录 ID；尚未首次保存时可能为空 */
  id?: string
  /** 本次就诊记录 ID */
  encounterId: string
  /** 富文本病历内容 */
  content: string
  /** 当前数据版本号；首次保存前为 0 */
  version: number
  /** 最近保存时间，格式为 YYYY-MM-DD HH:mm:ss */
  updatedAt?: string
}

/**
 * 保存门诊病历时提交的数据。
 */
export interface SaveDoctorMedicalRecordDTO {
  /** 富文本病历内容 */
  content: string
  /** 页面加载病历时取得的版本号 */
  version: number
}

/**
 * 病历模板树节点类型。
 *
 * CATEGORY 只是目录，TEMPLATE 才能被套用到编辑器。
 */
export type DoctorMedicalRecordTemplateNodeType = 'CATEGORY' | 'TEMPLATE'

/**
 * 病历模板树节点。
 */
export interface DoctorMedicalRecordTemplateNodeVO {
  /** 节点唯一 ID */
  id: string
  /** 节点显示名称 */
  name: string
  /** 目录或可套用模板 */
  type: DoctorMedicalRecordTemplateNodeType
  /** 是否为叶子节点，供树组件判断是否显示展开箭头 */
  isLeaf: boolean
  /** 子节点；模板节点没有子节点 */
  children?: DoctorMedicalRecordTemplateNodeVO[]
}

/**
 * 单个病历模板的完整内容。
 */
export interface DoctorMedicalRecordTemplateVO {
  /** 模板唯一 ID */
  id: string
  /** 模板名称 */
  name: string
  /** 套用到富文本编辑器的 HTML 内容 */
  content: string
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
 * 当前就诊的诊断列表及版本信息。
 */
export interface DoctorDiagnosisListVO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 诊断列表 */
  rows: DoctorDiagnosisVO[]
  /** 当前数据版本号 */
  version: number
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
 * 保存当前就诊全部诊断的请求数据。
 */
export interface SaveDoctorDiagnosesDTO {
  /** 页面加载诊断时取得的版本号 */
  version: number
  /** 当前完整诊断列表 */
  items: SaveDoctorDiagnosisItemDTO[]
}

/** 门诊基础版支持的医嘱分类。 */
export type DoctorAdviceOrderCategory = 'DRUG' | 'LAB' | 'EXAM' | 'TREATMENT'

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
  /** 医嘱备注 */
  remark?: string
  /** 医嘱状态 */
  status: 'SAVED'
  /** 显示顺序 */
  orderNo: number
}

/** 当前就诊的医嘱列表及版本。 */
export interface DoctorAdviceOrderListVO {
  /** 本次就诊记录 ID */
  encounterId: string
  /** 医嘱列表 */
  rows: DoctorAdviceOrderVO[]
  /** 当前数据版本号 */
  version: number
  /** 最近保存时间 */
  updatedAt?: string
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
  /** 医嘱备注 */
  remark?: string
  /** 显示顺序 */
  orderNo: number
}

/** 保存当前就诊完整医嘱列表的请求。 */
export interface SaveDoctorAdviceOrdersDTO {
  /** 页面加载医嘱时取得的版本号 */
  version: number
  /** 当前完整医嘱列表 */
  items: SaveDoctorAdviceOrderItemDTO[]
}
