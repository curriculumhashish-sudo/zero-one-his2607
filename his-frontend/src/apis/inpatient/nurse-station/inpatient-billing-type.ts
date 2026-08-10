import type { PageQuery } from '../../type'

/** 住院记账允许直接选择的项目类别；本项目不在护士站直接划价药品。 */
export type NurseStationBillingCatalogCategory = 'DEVICE' | 'SERVICE'

/** 划价来源，用于区分直接划价和关联医嘱的补费。 */
export type NurseStationBillingPricingMode = 'DIRECT' | 'SUPPLEMENT'

/** 收费状态。 */
export type NurseStationBillingChargeStatus = 'CANCELLED' | 'CHARGED' | 'PENDING'

/** 自动计费规则状态。 */
export type NurseStationAutoBillingRuleStatus = 'ACTIVE' | 'INACTIVE'

/** 划价选品目录分页条件。 */
export interface NurseStationBillingCatalogQueryDTO extends PageQuery {
  /** 当前住院就诊 ID，用于服务端校验病区和收费范围。 */
  encounterId: string
  /** 患者主档 ID。 */
  patientId: string
  /** 诊疗项目或耗材。 */
  category: NurseStationBillingCatalogCategory
  /** 编码、名称或检索码。 */
  keyword?: string
}

/** 项目可用计价单位。 */
export interface NurseStationBillingUnitBandVO {
  /** 单位编码。 */
  unitCode: string
  /** 单位名称。 */
  unitName: string
  /** 相对最小单位的换算比例。 */
  ratioToSplit: number
}

/** 划价选品目录行。 */
export interface NurseStationBillingCatalogItemVO {
  /** 产品来源物理表，真实后端据此选择正确写入路径。 */
  productTableName: string
  /** 产品定义 ID。 */
  productDefinitionId: string
  /** 收费项目定义 ID。 */
  chargeItemDefinitionId: string
  /** 项目业务编码。 */
  busNo: string
  /** 项目名称。 */
  name: string
  /** 项目类别。 */
  category: NurseStationBillingCatalogCategory
  /** 类别显示名称。 */
  categoryText: string
  /** 规格。 */
  specificationText?: string
  /** 默认计价单位编码。 */
  unitCode: string
  /** 默认计价单位名称。 */
  unitName: string
  /** 可选单位。 */
  unitBands: NurseStationBillingUnitBandVO[]
  /** 当前有效单价。 */
  unitPrice: number
  /** 耗材默认发放库房 ID。 */
  defaultDispenseLocationId?: string
  /** 耗材默认发放库房名称。 */
  defaultDispenseLocationName?: string
  /** 后端原始收费类别编码，Real 模式用于保留接口信息。 */
  backendCategoryCode?: string
  /** 后端组织/执行科室 ID。 */
  organizationUnitId?: string
  /** 后端组织/执行科室名称。 */
  organizationUnitName?: string
  /** 当前行是否具备真实划价所需的完整字段。 */
  writable?: boolean
  /** 不可划价时缺失的后端字段。 */
  missingFields?: string[]
}

/** 医嘱补费父医嘱查询条件。 */
export interface NurseStationSupplementOrderQueryDTO {
  encounterId: string
  patientId: string
  /** 全部、长期或临时。 */
  therapyType?: 'ALL' | 'LONG' | 'TEMP'
  /** 医嘱内容关键字。 */
  keyword?: string
}

/** 医嘱下已经补收的费用子项。 */
export interface NurseStationSupplementChargeItemVO {
  requestId: string
  productName: string
  quantity: number
  unitName: string
  unitPrice: number
  totalPrice: number
  pricedTime: string
  chargeStatusText: string
}

/** 可补费父医嘱。 */
export interface NurseStationSupplementOrderVO {
  requestId: string
  orderType: 'MED_REQUEST' | 'SERVICE_REQUEST'
  encounterId: string
  patientId: string
  therapyType: 'LONG' | 'TEMP'
  therapyTypeText: string
  adviceDisplayName: string
  specificationText?: string
  dosageDisplay?: string
  usageDisplay?: string
  frequencyDisplay?: string
  authoredTime: string
  stopTime?: string
  requesterName?: string
  performerLocationDisplay?: string
  statusText: string
  chargeItems: NurseStationSupplementChargeItemVO[]
}

/** 结构化父医嘱锚点，避免字符串拼接无法区分医嘱类型。 */
export interface NurseStationBillingParentOrderAnchorDTO {
  orderType: 'MED_REQUEST' | 'SERVICE_REQUEST'
  requestId: string
}

/** 批量划价中的单个项目。 */
export interface NurseStationBillingWriteItemDTO {
  /** 前端临时行 ID，用于逐项对应保存结果。 */
  clientItemId: string
  encounterId: string
  patientId: string
  accountId: string
  productTableName: string
  productDefinitionId: string
  chargeItemDefinitionId: string
  quantity: number
  unitCode: string
  /** 页面展示的单价，服务端仍需按最新价复核。 */
  previewUnitPrice: number
  /** 耗材发放库房或诊疗项目执行科室。 */
  performLocationId?: string
  /** 医嘱补费时必填，直接划价时禁止传入。 */
  parentOrderAnchor?: NurseStationBillingParentOrderAnchorDTO
}

/** 直接划价或医嘱补费批量请求。 */
export interface NurseStationConfirmBillingDTO {
  idempotencyKey: string
  mode: NurseStationBillingPricingMode
  items: NurseStationBillingWriteItemDTO[]
}

/** 单个划价项目处理结果。 */
export interface NurseStationBillingWriteResultItemVO {
  clientItemId: string
  success: boolean
  message: string
  requestId?: string
  finalUnitPrice?: number
  finalTotalPrice?: number
}

/** 批量划价处理结果。 */
export interface NurseStationBillingWriteResultVO {
  success: boolean
  processedCount: number
  results: NurseStationBillingWriteResultItemVO[]
}

/** 已划价列表查询条件。 */
export interface NurseStationPricedOrderQueryDTO {
  encounterId: string
  patientId: string
  pricedTimeFrom?: string
  pricedTimeTo?: string
  parentRequestId?: string
}

/** 已划价列表行。 */
export interface NurseStationPricedOrderVO {
  requestId: string
  orderType: 'DEVICE_REQUEST' | 'SERVICE_REQUEST'
  encounterId: string
  patientId: string
  productDefinitionId: string
  productName: string
  quantity: number
  unitCode: string
  unitName: string
  unitPrice: number
  totalPrice: number
  pricedTime: string
  pricingMode: NurseStationBillingPricingMode
  pricingModeText: string
  parentOrderAnchor?: NurseStationBillingParentOrderAnchorDTO
  chargeStatus: NurseStationBillingChargeStatus
  chargeStatusText: string
  dispenseStatusText?: string
  /** 页面只有在此字段为 true 时才允许勾选取消。 */
  cancellable: boolean
  cancelReasonDisabled?: string
  /** Real 模式后端缺少状态字段时，用于页面提示。 */
  backendFieldsIncomplete?: boolean
}

/** 取消划价单项。 */
export interface NurseStationCancelBillingItemDTO {
  requestId: string
  orderType: 'DEVICE_REQUEST' | 'SERVICE_REQUEST'
  encounterId: string
}

/** 批量取消划价。 */
export interface NurseStationCancelBillingDTO {
  idempotencyKey: string
  reason: string
  items: NurseStationCancelBillingItemDTO[]
}

/** 单项取消结果。 */
export interface NurseStationCancelBillingResultItemVO {
  requestId: string
  success: boolean
  message: string
}

/** 批量取消结果。 */
export interface NurseStationCancelBillingResultVO {
  success: boolean
  processedCount: number
  results: NurseStationCancelBillingResultItemVO[]
}

/** 自动计费规则查询条件。 */
export interface NurseStationAutoBillingQueryDTO {
  encounterId: string
  patientId: string
  billDate?: string
}

/** 自动计费规则。 */
export interface NurseStationAutoBillingRuleVO {
  ruleId: string
  ruleName: string
  status: NurseStationAutoBillingRuleStatus
  statusText: string
  activityId: string
  activityName: string
  chargeDefinitionId: string
  quantity: number
}

/** 自动计费列表结果。 */
export interface NurseStationAutoBillingDataVO {
  rules: NurseStationAutoBillingRuleVO[]
}

/** 新增或修改自动计费规则。 */
export interface NurseStationSaveAutoBillingRuleDTO {
  encounterId: string
  patientId: string
  ruleId?: string
  ruleName: string
  activityId: string
  chargeDefinitionId: string
  quantity: number
}

/** 删除自动计费规则。 */
export interface NurseStationDeleteAutoBillingRuleDTO {
  encounterId: string
  patientId: string
  ruleId: string
}

/** 自动计费规则写操作结果。 */
export interface NurseStationAutoBillingMutationResultVO {
  success: boolean
  message: string
}
