/** 通用下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 药品状态枚举 */
export type MedicationStatus = 'DRAFT' | 'ACTIVE' | 'INACTIVE' | 'DISCARDED'

/** 药品目录查询参数 */
export interface MedicationCatalogQuery {
  /** 关键词（名称或编码） */
  keyword?: string
  /** 药品名称 */
  name?: string
  /** 药品分类编码 */
  categoryCode?: string
  /** 启用状态 */
  enabled?: boolean | null
  /** 页码 */
  pageIndex: number
  /** 每页条数 */
  pageSize: number
}

/** 药品目录记录（列表展示用） */
export interface MedicationCatalogRecord {
  id: string
  /** 药品编码（如 MD0035） */
  busNo: string
  /** 通用名称 */
  name: string
  /** 商品名称 */
  merchandiseName: string
  /** 药品分类编码 */
  categoryCode: string
  /** 药品分类名称（展示用） */
  categoryName: string
  /** 规格 */
  totalVolume: string
  /** 启用状态 */
  activeFlag: boolean
  /** 生产厂家 */
  manufacturerText: string
  /** 剂型 */
  doseFormName: string
  /** 更新时间 */
  updatedAt: string
}

/** 药品目录详情（编辑表单用，含全部字段） */
export interface MedicationCatalogDetail {
  id?: string
  busNo?: string
  /** 通用名称 */
  name: string
  /** 商品名称 */
  merchandiseName: string
  /** 药品分类编码 */
  categoryCode: string
  /** 生产厂家文本 */
  manufacturerText: string
  /** 生产厂家标识 */
  manufacturerId?: string
  /** 供应商标识 */
  supplyId: string
  /** 批准文号 */
  approvalNumber?: string
  /** 国家药品贯标编码 */
  nationalDrugCode?: string
  /** 医保编码 */
  ybNo?: string
  /** 医保是否已对码 */
  ybMatchFlag?: boolean
  /** 财务类型编码 */
  financialTypeCode?: string
  /** 医保等级 */
  chrgitmLv?: string
  /** 病案结算类型 */
  settlementType?: string
  /** 医保类别 */
  ybCategory?: string
  /** 药品性质编码 */
  characteristic?: string
  /** 规格 */
  totalVolume: string
  /** 剂量单位编码 */
  doseUnitCode: string
  /** 剂量单位→拆零单位换算倍数 */
  doseUnitRatio: number
  /** 剂型编码 */
  doseFormCode: string
  /** 是否皮试药 */
  skinTestFlag?: boolean
  /** 是否注射药 */
  injectFlag?: boolean
  /** 用量限定 */
  usageLimit?: string
  /** 默认用法编码 */
  methodCode?: string
  /** 默认用药频次编码 */
  rateCode?: string
  /** 单次剂量 */
  dose?: number
  /** 单次最大剂量 */
  maxUnit?: string
  /** 用法说明 */
  usageInstruction?: string
  /** 限制使用范围 */
  restrictedScope?: string
  /** 药品说明 */
  medicationDefinition?: string
  /** DDD值编码 */
  dddCode?: string
  /** DDD单位编码 */
  dddUnitCode?: string
  /** 采购单位编码 */
  purchaseUnitCode: string
  /** 采购单位→拆零单位换算倍数 */
  purchaseUnitRatio: number
  /** 购入价（包装单位） */
  purchasePrice: number
  /** 门诊拆分属性编码 */
  partAttributeEnum: string
  /** 库存单位编码 */
  stockUnitCode: string
  /** 库存单位→拆零单位换算倍数 */
  stockUnitRatio: number
  /** 零售价（包装单位） */
  retailPrice: number
  /** 住院临时医嘱拆分属性编码 */
  thoPartAttributeEnum: string
  /** 包装单位编码 */
  packageUnitCode: string
  /** 包装单位→拆零单位换算倍数 */
  packageUnitRatio: number
  /** 最高零售价（包装单位） */
  maxRetailPrice?: number
  /** 拆零单位编码 */
  splitUnitCode: string
  /** 警戒线最高值（库存单位） */
  itemMaxQuantity?: number
  /** 警戒线最低值（库存单位） */
  itemMinQuantity?: number
  /** 药品拼音码 */
  pyStr?: string
  /** 药品五笔码 */
  wbStr?: string
  /** 商品拼音码 */
  merchandisePyStr?: string
  /** 商品五笔码 */
  merchandiseWbStr?: string
  /** 69码 */
  drug69Code?: string
  /** 生命周期状态编码 */
  statusEnum: MedicationStatus
  /** 领域编码 */
  domainEnum?: string
  /** 是否基本药物 */
  basicFlag?: boolean
  /** 儿童用药标志 */
  childrenFlag?: boolean
  /** 是否自制 */
  selfFlag?: boolean
  /** 是否活性 */
  activeFlag: boolean
  /** 是否限制使用 */
  restrictedFlag?: boolean
  /** 是否抗生素 */
  antibioticFlag?: boolean
  /** 处方药标志 */
  rxFlag?: boolean
}

/** 新增/保存参数 */
export interface MedicationCatalogSavePayload {
  id?: string
  name: string
  merchandiseName: string
  categoryCode: string
  manufacturerText: string
  manufacturerId?: string
  supplyId: string
  approvalNumber?: string
  nationalDrugCode?: string
  ybNo?: string
  ybMatchFlag?: boolean
  financialTypeCode?: string
  chrgitmLv?: string
  settlementType?: string
  ybCategory?: string
  characteristic?: string
  totalVolume: string
  doseUnitCode: string
  doseUnitRatio: number
  doseFormCode: string
  skinTestFlag?: boolean
  injectFlag?: boolean
  usageLimit?: string
  methodCode?: string
  rateCode?: string
  dose?: number
  maxUnit?: string
  usageInstruction?: string
  restrictedScope?: string
  medicationDefinition?: string
  dddCode?: string
  dddUnitCode?: string
  purchaseUnitCode: string
  purchaseUnitRatio: number
  purchasePrice: number
  partAttributeEnum: string
  stockUnitCode: string
  stockUnitRatio: number
  retailPrice: number
  thoPartAttributeEnum: string
  packageUnitCode: string
  packageUnitRatio: number
  maxRetailPrice?: number
  splitUnitCode: string
  itemMaxQuantity?: number
  itemMinQuantity?: number
  pyStr?: string
  wbStr?: string
  merchandisePyStr?: string
  merchandiseWbStr?: string
  drug69Code?: string
  statusEnum: MedicationStatus
  domainEnum?: string
  basicFlag?: boolean
  childrenFlag?: boolean
  selfFlag?: boolean
  activeFlag: boolean
  restrictedFlag?: boolean
  antibioticFlag?: boolean
  rxFlag?: boolean
}

/** 分页结果 */
export interface MedicationCatalogPageResult {
  rows: MedicationCatalogRecord[]
  total: number
}

/** 表单选项数据 */
export interface MedicationCatalogFormOptions {
  categoryOptions: SelectOption[]
  supplierOptions: SelectOption[]
  financialTypeOptions: SelectOption[]
  propertyOptions: SelectOption[]
  doseUnitOptions: SelectOption[]
  doseFormOptions: SelectOption[]
  methodOptions: SelectOption[]
  rateOptions: SelectOption[]
  purchaseUnitOptions: SelectOption[]
  stockUnitOptions: SelectOption[]
  packageUnitOptions: SelectOption[]
  splitUnitOptions: SelectOption[]
  partAttributeOptions: SelectOption[]
  thoPartAttributeOptions: SelectOption[]
  statusOptions: SelectOption[]
  domainOptions: SelectOption[]
  ybLevelOptions: SelectOption[]
  settlementTypeOptions: SelectOption[]
  ybCategoryOptions: SelectOption[]
  dddUnitOptions: SelectOption[]
}
