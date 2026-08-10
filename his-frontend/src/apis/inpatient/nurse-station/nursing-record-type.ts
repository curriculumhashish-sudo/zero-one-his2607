/** 护理记录纸质单抬头。 */
export interface NurseStationNursingRecordPaperHeaderVO {
  /** 患者姓名。 */
  patientName: string
  /** 性别显示文本。 */
  genderText?: string
  /** 年龄显示文本。 */
  ageText?: string
  /** 病区或病室名称。 */
  wardName?: string
  /** 床号。 */
  bedName?: string
  /** 住院号。 */
  encounterBusNo?: string
  /** 入院日期。 */
  admissionDate?: string
  /** 出院日期。 */
  dischargeDate?: string
}

/** 已保存的一条护理记录。 */
export interface NurseStationNursingRecordRowVO {
  /** 生命体征记录 ID，也是更新和删除时使用的业务主键。 */
  vitalSignId: string
  /** 记录日期，格式 yyyy-MM-dd。 */
  recordDate: string
  /** 记录时间，格式 HH:mm。 */
  recordTime: string
  /** 体温，单位摄氏度。 */
  temperature?: string
  /** 脉搏，单位次/分。 */
  pulse?: string
  /** 心率，单位次/分。 */
  heartRate?: string
  /** 呼吸，单位次/分。 */
  respiratory?: string
  /** 血压，例如 120/80。 */
  bloodPressure?: string
  /** 血氧饱和度，单位百分比。 */
  oxygenSaturation?: string
  /** 氧流量。 */
  oxygenFlow?: string
  /** 给氧方式。 */
  oxygenMode?: string
  /** 氧浓度或吸氧速率。 */
  oxygenRate?: string
  /** 意识状态。 */
  consciousness?: string
  /** 左侧瞳孔观察结果。 */
  pupilLeft?: string
  /** 右侧瞳孔观察结果。 */
  pupilRight?: string
  /** 皮肤状况。 */
  skinStatus?: string
  /** 管路护理。 */
  tubeCare?: string
  /** 病情观察及护理措施。 */
  conditionMeasure?: string
  /** 入量名称。 */
  intakeName?: string
  /** 入量数值，单位 ml。 */
  intakeAmount?: string
  /** 入量途径。 */
  intakeRoute?: string
  /** 出量名称。 */
  outputName?: string
  /** 出量数值，单位 ml。 */
  outputAmount?: string
  /** 出量颜色或性状。 */
  outputColorCharacter?: string
  /** 护士签名显示文本。 */
  nurseSignature?: string
  /** 乐观锁版本；每次更新后递增。 */
  version: number
  /** 创建时间，供审计和问题追踪使用。 */
  createdAt?: string
  /** 最后修改时间。 */
  updatedAt?: string
}

/** 查询患者护理记录的条件。 */
export interface NurseStationNursingRecordQueryDTO {
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 查询开始时间，格式 yyyy-MM-dd HH:mm:ss。 */
  recordedAtFrom?: string
  /** 查询结束时间，格式 yyyy-MM-dd HH:mm:ss。 */
  recordedAtTo?: string
  /** 是否按记录时间升序排列。 */
  sortAsc?: boolean
  /** 后端旧接口可能不返回完整抬头时，页面提供的患者上下文。 */
  patientContext?: NurseStationNursingRecordPaperHeaderVO
}

/** 护理记录纸质单查询结果。 */
export interface NurseStationNursingRecordListVO {
  /** 纸质单患者抬头。 */
  paperHeader: NurseStationNursingRecordPaperHeaderVO
  /** 查询范围内已经保存的记录。 */
  rows: NurseStationNursingRecordRowVO[]
}

/** 新增或更新一条护理记录的数据。 */
export interface NurseStationNursingRecordSaveItemDTO {
  /** 前端临时行 ID，用于把批量响应准确合并回原行。 */
  clientRowId: string
  /** 更新时传已有记录 ID；新增时不传。 */
  vitalSignId?: string
  /** 更新时传查询所得版本；新增时不传。 */
  expectedVersion?: number
  /** 记录日期，格式 yyyy-MM-dd。 */
  recordDate: string
  /** 记录时间，格式 HH:mm。 */
  recordTime: string
  /** 体温。 */
  temperature?: string
  /** 脉搏。 */
  pulse?: string
  /** 心率。 */
  heartRate?: string
  /** 呼吸。 */
  respiratory?: string
  /** 血压。 */
  bloodPressure?: string
  /** 血氧饱和度。 */
  oxygenSaturation?: string
  /** 氧流量。 */
  oxygenFlow?: string
  /** 给氧方式。 */
  oxygenMode?: string
  /** 氧浓度或吸氧速率。 */
  oxygenRate?: string
  /** 意识状态。 */
  consciousness?: string
  /** 左侧瞳孔。 */
  pupilLeft?: string
  /** 右侧瞳孔。 */
  pupilRight?: string
  /** 皮肤状况。 */
  skinStatus?: string
  /** 管路护理。 */
  tubeCare?: string
  /** 病情观察及护理措施。 */
  conditionMeasure?: string
  /** 入量名称。 */
  intakeName?: string
  /** 入量。 */
  intakeAmount?: string
  /** 入量途径。 */
  intakeRoute?: string
  /** 出量名称。 */
  outputName?: string
  /** 出量。 */
  outputAmount?: string
  /** 出量颜色或性状。 */
  outputColorCharacter?: string
  /** 护士签名。真实后端仍应以登录身份校验签名权限。 */
  nurseSignature?: string
  /** extra 表示护士主动添加的额外时点。 */
  rowType?: 'extra'
}

/** 批量保存护理记录请求。 */
export interface NurseStationSaveNursingRecordsDTO {
  /** 防止网络重试重复新增记录的幂等键。 */
  idempotencyKey: string
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 待保存记录，单批 1～50 条。 */
  records: NurseStationNursingRecordSaveItemDTO[]
}

/** 批量保存中单条记录的结果。 */
export interface NurseStationNursingRecordSaveResultVO {
  /** 对应请求中的前端临时行 ID。 */
  clientRowId: string
  /** 当前条目是否成功。 */
  success: boolean
  /** 处理结果说明。 */
  message: string
  /** 保存成功后的完整记录。 */
  row?: NurseStationNursingRecordRowVO
}

/** 批量保存护理记录响应。 */
export interface NurseStationSaveNursingRecordsResultVO {
  /** 是否全部保存成功。 */
  success: boolean
  /** 成功保存数量。 */
  savedCount: number
  /** 每条记录的处理结果。 */
  results: NurseStationNursingRecordSaveResultVO[]
}

/** 删除一条已保存护理记录的请求。 */
export interface NurseStationDeleteNursingRecordDTO {
  /** 防止重复删除的幂等键。 */
  idempotencyKey: string
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 待删除生命体征记录 ID。 */
  vitalSignId: string
  /** 查询时取得的版本，用于避免删除别人刚修改的记录。 */
  expectedVersion: number
  /** 可选删除原因。 */
  reason?: string
}

/** 删除护理记录响应。 */
export interface NurseStationDeleteNursingRecordResultVO {
  /** 是否删除成功。 */
  success: boolean
  /** 被删除的记录 ID。 */
  vitalSignId: string
  /** 处理结果说明。 */
  message: string
}
