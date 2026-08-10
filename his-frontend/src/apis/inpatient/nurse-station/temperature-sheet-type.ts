/** 体温单支持的时间窗口。 */
export type NurseStationTemperatureWindowRange = '14d' | '3d' | '7d'

/** 体温单每天固定的六个测量时次。 */
export type NurseStationTemperatureTimeSlot = '02' | '06' | '10' | '14' | '18' | '22'

/** 体温单特殊事件编码。 */
export type NurseStationTemperatureSpecialEventCode =
  | 'ADMISSION'
  | 'DEATH'
  | 'DELIVERY'
  | 'DISCHARGE'
  | 'MEDICATION_COOLING'
  | 'OUT_FOR_EXAM'
  | 'PHYSICAL_COOLING'
  | 'PULSE_DEFICIT'
  | 'REFUSE_TEMP'
  | 'SURGERY'
  | 'TEMP_NOT_RISING'
  | 'TRANSFER'

/** 特殊事件。 */
export interface NurseStationTemperatureSpecialEventVO {
  /** 事件编码。 */
  code: NurseStationTemperatureSpecialEventCode
  /** 事件显示名称。 */
  displayText: string
  /** 事件发生时间，部分事件必填。 */
  eventTime?: string
  /** 物理或药物降温后的体温。 */
  cooledTemperature?: string
}

/** 单个测量时次的数据。 */
export interface NurseStationTemperatureSlotVO {
  /** 每天固定时次。 */
  timeSlot: NurseStationTemperatureTimeSlot
  /** 已保存记录 ID。 */
  vitalSignId?: string
  /** 乐观锁版本。 */
  version?: number
  /** 体温，单位摄氏度。 */
  temperature?: string
  /** 脉搏，单位次/分。 */
  pulse?: string
  /** 心率，单位次/分。 */
  heartRate?: string
  /** 呼吸，单位次/分。 */
  respiratory?: string
  /** 收缩压，单位 mmHg。 */
  systolicPressure?: string
  /** 舒张压，单位 mmHg。 */
  diastolicPressure?: string
  /** 当前时次记录的特殊事件。 */
  specialEvents: NurseStationTemperatureSpecialEventVO[]
}

/** 按天记录的补充统计。本项目不包含皮试字段。 */
export interface NurseStationTemperatureDailySupplementVO {
  /** 大便次数。 */
  stoolCount?: string
  /** 小便次数。 */
  urineCount?: string
  /** 总入量，单位 ml。 */
  intake?: string
  /** 尿量，单位 ml。 */
  urine?: string
  /** 其他出量，单位 ml。 */
  other?: string
  /** 体重，单位 kg。 */
  weight?: string
  /** 身高，单位 cm。 */
  height?: string
  /** 腹围，单位 cm。 */
  abdominalCircumference?: string
}

/** 体温单一天的完整网格。 */
export interface NurseStationTemperatureGridDayVO {
  /** 日期，格式 yyyy-MM-dd。 */
  recordDate: string
  /** 六个固定时次。 */
  slots: NurseStationTemperatureSlotVO[]
  /** 当日补充统计。 */
  dailySupplement: NurseStationTemperatureDailySupplementVO
}

/** 体温单患者抬头。 */
export interface NurseStationTemperatureSheetHeaderVO {
  patientName: string
  genderText?: string
  ageText?: string
  wardName?: string
  bedName?: string
  encounterBusNo?: string
  admissionDate?: string
}

/** 查询体温单窗口。 */
export interface NurseStationTemperatureSheetQueryDTO {
  encounterId: string
  patientId: string
  windowRange: NurseStationTemperatureWindowRange
  /** 0 表示最新窗口，数值越大表示越早的窗口。 */
  pageIndex?: number
  /** 打开录入抽屉时聚焦的日期。 */
  focusRecordDate?: string
  /** 后端旧接口未返回纸质单抬头时，页面提供的患者上下文。 */
  patientContext?: NurseStationTemperatureSheetHeaderVO
}

/** 体温单查询结果。 */
export interface NurseStationTemperatureSheetDataVO {
  paperHeader: NurseStationTemperatureSheetHeaderVO
  gridDays: NurseStationTemperatureGridDayVO[]
  hospitalizationDaysByDate: Record<string, number>
  postoperativeDaysByDate: Record<string, number>
  pagination: {
    dateFrom: string
    dateTo: string
    pageIndex: number
    totalPages: number
  }
}

/** 保存单个时次。 */
export interface NurseStationTemperatureSaveSlotDTO extends Omit<
  NurseStationTemperatureSlotVO,
  'version'
> {
  /** 更新已有记录时传查询所得版本。 */
  expectedVersion?: number
}

/** 批量保存同一天体温单。 */
export interface NurseStationSaveTemperatureSheetDTO {
  idempotencyKey: string
  encounterId: string
  patientId: string
  recordDate: string
  /** 有内容的时次，单批最多六条。 */
  slots: NurseStationTemperatureSaveSlotDTO[]
  /** 当天补充统计，不包含皮试阳性。 */
  dailySupplement?: NurseStationTemperatureDailySupplementVO
}

/** 单个时次保存结果。 */
export interface NurseStationTemperatureSaveSlotResultVO {
  timeSlot: NurseStationTemperatureTimeSlot
  success: boolean
  message: string
  slot?: NurseStationTemperatureSlotVO
}

/** 批量保存体温单结果。 */
export interface NurseStationSaveTemperatureSheetResultVO {
  success: boolean
  savedCount: number
  nextTimeSlot?: NurseStationTemperatureTimeSlot
  results: NurseStationTemperatureSaveSlotResultVO[]
}
