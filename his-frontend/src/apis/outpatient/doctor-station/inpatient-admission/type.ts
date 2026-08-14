/**
 * 办理住院下拉选项。
 *
 * value 传给后端，label 只负责页面展示。
 */
export interface InpatientAdmissionOptionVO extends Record<string, unknown> {
  /** 选项值 */
  value: string
  /** 选项名称 */
  label: string
}

/** 可接收入院申请的住院科室。 */
export interface InpatientAdmissionDepartmentVO extends InpatientAdmissionOptionVO {
  /** 科室编码 */
  code: string
}

/** 入院病区。 */
export interface InpatientAdmissionWardVO extends InpatientAdmissionOptionVO {
  /** 所属住院科室 ID */
  departmentId: string
}

/** 办理住院弹窗初始化时需要的固定选项。 */
export interface InpatientAdmissionFormOptionsVO {
  /** 可接收入院申请的科室 */
  departmentOptions: InpatientAdmissionDepartmentVO[]
  /** 入院类型 */
  admissionTypeOptions: InpatientAdmissionOptionVO[]
  /** 入院方式 */
  admissionWayOptions: InpatientAdmissionOptionVO[]
  /** 病情优先级 */
  priorityOptions: InpatientAdmissionOptionVO[]
}

/** 住院申请状态。 */
export type InpatientAdmissionStatus = 'PLANNED' | 'ADMITTED' | 'CANCELLED'

/** 查询当前门诊就诊是否已有住院申请的结果。 */
export interface CheckInpatientAdmissionVO {
  /** 是否存在未取消的住院申请 */
  exists: boolean
  /** 已有住院申请 ID */
  applicationId?: string
  /** 已创建的住院就诊 ID */
  inpatientEncounterId?: string
  /** 已创建的住院号 */
  inpatientEncounterNo?: string
  /** 申请状态 */
  status?: InpatientAdmissionStatus
  /** 申请状态名称 */
  statusName?: string
}

/** 入院诊断提交项。 */
export interface InpatientAdmissionDiagnosisDTO {
  /** 诊断字典项目 ID */
  catalogId: string
  /** 当前门诊已保存的诊断记录 ID */
  diagnosisRecordId?: string
  /** ICD 编码 */
  code: string
  /** 诊断名称 */
  name: string
  /** 诊断说明 */
  description?: string
  /** 是否为入院主诊断；首版只能有一个主诊断 */
  primary: true
}

/** 提交住院申请的数据。 */
export interface RequestInpatientAdmissionDTO {
  /** 当前门诊就诊 ID */
  encounterId: string
  /** 患者档案 ID */
  patientId: string
  /** 目标入院科室 ID */
  targetDepartmentId: string
  /** 意向病区 ID */
  wardId?: string
  /** 入院诊断列表 */
  admissionDiagnoses: InpatientAdmissionDiagnosisDTO[]
  /** 入院类型编码 */
  admissionTypeCode?: string
  /** 入院方式编码 */
  admissionWayCode?: string
  /** 病情优先级编码 */
  priorityCode?: string
}

/** 住院申请提交结果。 */
export interface RequestInpatientAdmissionVO {
  /** 是否提交成功 */
  success: boolean
  /** 住院申请 ID */
  applicationId: string
  /** 新建的计划住院就诊 ID */
  inpatientEncounterId: string
  /** 新建的住院号 */
  inpatientEncounterNo: string
  /** 当前状态 */
  status: InpatientAdmissionStatus
  /** 当前状态名称 */
  statusName: string
}
