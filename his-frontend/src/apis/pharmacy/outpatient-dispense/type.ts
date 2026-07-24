// src/apis/pharmacy/outpatient-dispense/type.ts

/** ===== 发药队列查询参数 ===== */
export interface PageDispenseQueueParams {
  startTime?: string
  endTime?: string
  keyword?: string
  dispenseStatusEnumIn?: ('PREPARATION' | 'IN_PROGRESS' | 'COMPLETED')[]
  pageNo?: number
  pageSize?: number
}

/** ===== 发药队列返回行 ===== */
export interface PendingDispenseEncounterQueueRowDto {
  encounterId: string
  patientId: string
  patientName: string
  genderText: string
  ageText: string
  visitNo: string
  prescriptionCount: number
  registerTime: string
  dispenseStatus: 'PREPARATION' | 'IN_PROGRESS' | 'COMPLETED'
}

/** ===== 发放明细查询参数 ===== */
export interface PageDispenseSlipsParams {
  encounterId: string
  dispenseStatusEnumIn?: ('PREPARATION' | 'IN_PROGRESS' | 'COMPLETED')[]
  pageNo?: number
  pageSize?: number
}

/** ===== 发放明细返回行 ===== */
export interface EncounterMedicationDispenseSlipRowDto {
  dispenseId: string
  prescriptionNo: string
  /** 前端页签使用的处方类型；真实接口对接时在 API 层完成枚举转换。 */
  rxType?: 'western' | 'herbal'
  medicationName: string
  specText: string
  quantity: number
  unitName: string
  dosageText: string
  frequencyText: string
  routeText: string
  days: number
  totalPrice: number
  statusEnum:
    | 'PENDING'
    | 'PREPARATION'
    | 'IN_PROGRESS'
    | 'PARTIAL'
    | 'COMPLETED'
    | 'BLOCKED'
    | 'ERROR'
  pharmacyName: string
  lotNumber?: string
  traceNo?: string
  factoryName?: string
  inventoryDetailList?: {
    pharmacyOrganizationId: string
    pharmacyName: string
    lotNumber: string
    availableQuantity: number
  }[]
}

/** 处方直接打印参数，不在发药页面重复选择模板和打印机。 */
export interface PrintPrescriptionParams {
  encounterId: string
  prescriptionIds: string[]
}

/** ===== 发药准备参数 ===== */
export interface SavePreparationParams {
  encounterId: string
  preparerPractitionerId: string
  lines: {
    dispenseId: string
    dispenseLocationId: string
    lotNumber?: string | null
    quantity: string
    traceNo?: string | null
  }[]
}

/** ===== 发药确认参数 ===== */
export interface ConfirmDispenseParams {
  encounterId: string
  contractBusNo?: string
  lines: {
    dispenseId: string
  }[]
}

/** ===== 药师选项 ===== */
export interface PractitionerOptionDto {
  practitionerId: number | string
  displayName: string
}

/** ===== 药房选项 ===== */
export interface PharmacyOptionDto {
  pharmacyId: string
  pharmacyName: string
}
