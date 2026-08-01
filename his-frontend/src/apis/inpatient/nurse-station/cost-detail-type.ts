import type { PageDTO, PageQuery } from '../../type'

/** 费用明细支持的两种展示方式。 */
export type NurseStationCostPresentationMode = 'DETAIL' | 'SUMMARY'

/** 费用大类，用于计算顶部药品、诊疗和耗材合计。 */
export type NurseStationCostExpenseBlock = 'DEVICE' | 'MEDICATION' | 'SERVICE'

/** 页面初始化中的展示模式选项。 */
export interface NurseStationCostPresentationOptionVO {
  /** 稳定模式编码。 */
  code: NurseStationCostPresentationMode
  /** 页面显示名称。 */
  display: string
}

/** 费用明细页面初始化结果。 */
export interface NurseStationCostDetailInitVO {
  /** 默认开始日期，格式 yyyy-MM-dd。 */
  dateFrom: string
  /** 默认结束日期，格式 yyyy-MM-dd。 */
  dateTo: string
  /** 后端允许使用的展示方式。 */
  presentationOptions: NurseStationCostPresentationOptionVO[]
  /** 费用大类显示顺序。 */
  expenseBlockOrder: NurseStationCostExpenseBlock[]
}

/** 单患者费用明细查询条件。 */
export interface NurseStationCostDetailQueryDTO extends PageQuery {
  /** 当前住院就诊 ID。 */
  encounterId: string
  /** 患者主档 ID，服务端需与就诊 ID 交叉校验。 */
  patientId: string
  /** 查询开始日期，包含当天。 */
  dateFrom: string
  /** 查询结束日期，包含当天。 */
  dateTo: string
  /** 明细或汇总。 */
  presentationMode: NurseStationCostPresentationMode
}

/** 单条收费或退费明细。 */
export interface NurseStationCostDetailLineVO {
  /** 收费明细唯一标识，也是表格 row-key。 */
  chargeItemId: string
  /** 产品定义 ID。 */
  productId: string
  /** 产品来源表。 */
  productTableName: string
  /** 项目名称。 */
  itemName: string
  /** 项目规格。 */
  itemSpec?: string
  /** 数量；退费使用负数。 */
  quantity: number
  /** 数量单位编码。 */
  quantityUnit: string
  /** 数量单位显示名。 */
  quantityUnitDisplay: string
  /** 单价。 */
  unitPrice: number
  /** 行金额；退费使用负数。 */
  totalPrice: number
  /** 费用发生时间。 */
  businessDateTime: string
  /** 费用状态编码。 */
  status: 'CHARGED' | 'PENDING' | 'REFUNDED'
  /** 费用状态显示名。 */
  statusDisplay: string
  /** 费用生成来源编码。 */
  generateSource: 'AUTO' | 'DIRECT' | 'ORDER' | 'REFUND'
  /** 费用生成来源显示名。 */
  generateSourceDisplay: string
  /** 是否按退费行高亮。 */
  refundHighlight: boolean
  /** 产生费用的服务表。 */
  serviceTable: string
  /** 药品、诊疗或耗材大类。 */
  expenseBlockCode: NurseStationCostExpenseBlock
  /** 大类显示名称。 */
  expenseBlockDisplay: string
  /** 更细的收费分类编码。 */
  categoryCode: string
  /** 更细的收费分类名称。 */
  categoryDisplay: string
  /** 包装单位编码，药品汇总时使用。 */
  packageUnitCode?: string
  /** 包装单位显示名称。 */
  packageUnitDisplay?: string
  /** 拆零单位编码。 */
  splitUnitCode?: string
  /** 拆零单位显示名称。 */
  splitUnitDisplay?: string
  /** 一包装含多少拆零单位。 */
  splitRatio?: number
}

/** 按相同项目、费用大类和单价合并后的汇总行。 */
export interface NurseStationCostSummaryLineVO {
  /** 稳定汇总键。 */
  summaryKey: string
  productId: string
  productTableName: string
  itemName: string
  itemSpec?: string
  categoryCode: string
  categoryDisplay: string
  unitPrice: number
  /** 收费与退费相抵后的行净额。 */
  lineTotalAmount: number
  /** 净数量折算出的整包装数。 */
  packageQuantity?: number
  /** 不足一包装的拆零数量。 */
  splitUnitQuantity?: number
  packageUnitCode?: string
  packageUnitDisplay?: string
  splitUnitCode?: string
  splitUnitDisplay?: string
  expenseBlockCode: NurseStationCostExpenseBlock
  expenseBlockDisplay: string
}

/** 动态收费分类小计。 */
export interface NurseStationCostCategoryTotalVO {
  categoryCode: string
  categoryDisplay: string
  amount: number
}

/** 全部筛选结果的费用合计，不受当前分页影响。 */
export interface NurseStationCostTotalsVO {
  medicationTotal: number
  serviceTotal: number
  deviceTotal: number
  categoryTotals: NurseStationCostCategoryTotalVO[]
  grandTotal: number
}

/** 费用明细或汇总统一查询结果。 */
export interface NurseStationCostDetailDataVO {
  presentationMode: NurseStationCostPresentationMode
  /** 明细模式使用的分页数据；汇总模式返回空页。 */
  detailPage: PageDTO<NurseStationCostDetailLineVO>
  /** 汇总模式使用的项目集合；明细模式也返回，便于快速切换。 */
  summaryLines: NurseStationCostSummaryLineVO[]
  totals: NurseStationCostTotalsVO
  /** 筛选范围内原始明细总条数。 */
  totalDetailCount: number
  /** 筛选范围内合并后的项目总数。 */
  summaryItemCount: number
}
