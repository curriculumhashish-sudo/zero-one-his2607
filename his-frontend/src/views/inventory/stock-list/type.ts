/** 下拉选项 */
export interface SelectOption {
  label: string
  value: string
}

/** 表单字段配置：用数据描述每个查询字段，模板 v-for 渲染 */
export interface FormFieldConfig {
  label: string
  prop: string
  type: 'input' | 'select'
  placeholder?: string
  multiple?: boolean
  clearable?: boolean
  collapseTags?: boolean
  filterable?: boolean
  options?: SelectOption[]
}

/** 表格列配置 */
export interface TableColumnConfig {
  prop?: string
  label: string
  width?: number
  minWidth?: number
  align?: 'left' | 'center' | 'right'
  headerAlign?: 'left' | 'center' | 'right'
  fixed?: boolean | 'left' | 'right'
  type?: string
  showOverflowTooltip?: boolean
}

// ==================== 下拉选项 ====================

export const warehouseOptions: SelectOption[] = [
  { label: '耗材库', value: '耗材库' },
  { label: '西药库', value: '西药库' },
  { label: '中药库', value: '中药库' },
  { label: '门诊西药库', value: '门诊西药库' },
  { label: '门诊中药库', value: '门诊中药库' },
  { label: '住院西药库', value: '住院西药库' },
  { label: '住院中药库', value: '住院中药库' },
  { label: '门诊耗材库', value: '门诊耗材库' },
  { label: '住院耗材库', value: '住院耗材库' }
]

export const supplierOptions: SelectOption[] = [
  { label: '华东医疗器械制造有限公司', value: '华东医疗器械制造有限公司' },
  { label: '华南中药饮品供应公司', value: '华南中药饮品供应公司' },
  { label: '华中药品配送有限公司', value: '华中药品配送有限公司' },
  { label: '西南医用耗材制造有限公司', value: '西南医用耗材制造有限公司' },
  { label: '华北诊断试剂制造厂', value: '华北诊断试剂制造厂' },
  { label: '东部医疗物流供应链有限公司', value: '东部医疗物流供应链有限公司' },
  { label: '华南医疗设备制造有限公司', value: '华南医疗设备制造有限公司' },
  { label: '北方疫苗冷链供应有限公司', value: '北方疫苗冷链供应有限公司' },
  { label: '华东一次性耗材制造有限公司', value: '华东一次性耗材制造有限公司' },
  { label: '中原中药材供应有限公司', value: '中原中药材供应有限公司' },
  { label: '华西影像设备制造有限公司', value: '华西影像设备制造有限公司' }
]

export const medicationCategoryOptions: SelectOption[] = [
  { label: '西药', value: '西药' },
  { label: '中成药', value: '中成药' },
  { label: '中草药', value: '中草药' },
  { label: '外购药品', value: '外购药品' },
  { label: '其他', value: '其他' }
]

export const deviceCategoryOptions: SelectOption[] = [
  { label: '有源的', value: '有源的' },
  { label: '治疗设备', value: '治疗设备' },
  { label: '通讯类', value: '通讯类' },
  { label: '非院内使用', value: '非院内使用' },
  { label: '植入类', value: '植入类' },
  { label: '试管类', value: '试管类' },
  { label: '床旁类', value: '床旁类' },
  { label: '单次消耗类', value: '单次消耗类' },
  { label: '可重用类', value: '可重用类' },
  { label: '软件类', value: '软件类' },
  { label: '医用耗材', value: '医用耗材' },
  { label: '检验试剂', value: '检验试剂' }
]

export const inventoryStatusOptions: SelectOption[] = [
  { label: '已启用', value: '已启用' },
  { label: '已停用', value: '已停用' }
]

export const chrgitmLvOptions: SelectOption[] = [
  { label: '甲类', value: '甲类' },
  { label: '乙类', value: '乙类' },
  { label: '自费', value: '自费' }
]

export const inventoryRangeOptions: SelectOption[] = [
  { label: '数量等于0', value: '数量等于0' },
  { label: '数量大于0', value: '数量大于0' }
]

// ==================== 表单字段配置（按顺序，前8个默认可见，后2个折叠） ====================

export const formFieldConfigs: FormFieldConfig[] = [
  {
    label: '备份单号',
    prop: 'backupBusNo',
    type: 'select',
    placeholder: '不选则查现库存',
    clearable: true
  },
  {
    label: '关键字',
    prop: 'keyword',
    type: 'input',
    placeholder: '名称、编码、商品名、拼音、五笔、批号',
    clearable: true
  },
  {
    label: '库房',
    prop: 'warehouseIds',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: warehouseOptions
  },
  {
    label: '供应商',
    prop: 'supplierIds',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: supplierOptions
  },
  {
    label: '近效期(天)',
    prop: 'expireWithinDays',
    type: 'input',
    placeholder: '剩余天数 ≤',
    clearable: true
  },
  {
    label: '药品类型',
    prop: 'medicationCategoryCodes',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: medicationCategoryOptions
  },
  {
    label: '耗材类型',
    prop: 'deviceCategoryCodes',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: deviceCategoryOptions
  },
  {
    label: '供应状态',
    prop: 'inventoryStatusEnums',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: inventoryStatusOptions
  },
  // ↓ 以下默认折叠
  {
    label: '医保等级',
    prop: 'chrgitmLvList',
    type: 'select',
    placeholder: '可多选',
    multiple: true,
    clearable: true,
    collapseTags: true,
    options: chrgitmLvOptions
  },
  {
    label: '库存范围',
    prop: 'zeroStockFilter',
    type: 'select',
    placeholder: '库存范围',
    clearable: true,
    options: inventoryRangeOptions
  }
]

// ==================== 表格列配置 ====================

export const tableColumnConfigs: TableColumnConfig[] = [
  {
    type: 'index',
    label: '序号',
    width: 56,
    align: 'center',
    headerAlign: 'center',
    fixed: 'left'
  },
  {
    prop: 'productBusNo',
    label: '编码',
    width: 110,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true,
    fixed: 'left'
  },
  {
    prop: 'name',
    label: '项目名称',
    minWidth: 140,
    align: 'left',
    headerAlign: 'center',
    showOverflowTooltip: true,
    fixed: 'left'
  },
  {
    prop: 'merchandiseName',
    label: '商品名称',
    minWidth: 120,
    align: 'left',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'categoryLabel',
    label: '项目类别',
    width: 100,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'lotNumber',
    label: '批号',
    minWidth: 110,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'packageStockText',
    label: '库存(包装)',
    minWidth: 100,
    align: 'right',
    headerAlign: 'center'
  },
  {
    prop: 'splitStockText',
    label: '库存(最小)',
    minWidth: 100,
    align: 'right',
    headerAlign: 'center'
  },
  {
    prop: 'packageUnitRatio',
    label: '拆零换算比',
    width: 80,
    align: 'center',
    headerAlign: 'center'
  },
  {
    prop: 'warehouseName',
    label: '库房',
    minWidth: 100,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'totalVolume',
    label: '规格',
    minWidth: 100,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'manufacturer',
    label: '厂家',
    minWidth: 120,
    align: 'left',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'supplierName',
    label: '供应商',
    minWidth: 140,
    align: 'left',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'doseFormLabel',
    label: '剂型',
    width: 90,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'approvalNumber',
    label: '国药准字',
    minWidth: 120,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  {
    prop: 'ybNo',
    label: '医保码',
    minWidth: 120,
    align: 'center',
    headerAlign: 'center',
    showOverflowTooltip: true
  },
  { prop: 'expirationDate', label: '效期', width: 120, align: 'center', headerAlign: 'center' },
  { prop: 'productionDate', label: '生产日期', width: 120, align: 'center', headerAlign: 'center' },
  { prop: 'remainingDays', label: '剩余天数', width: 90, align: 'center', headerAlign: 'center' },
  {
    prop: 'inventoryStatusLabel',
    label: '供应状态',
    width: 100,
    align: 'center',
    headerAlign: 'center'
  },
  { prop: 'chrgitmLvLabel', label: '医保等级', width: 100, align: 'center', headerAlign: 'center' },
  { prop: 'lotCostPrice', label: '进价', width: 130, align: 'right', headerAlign: 'center' },
  { prop: 'lotSalePrice', label: '售价', width: 130, align: 'right', headerAlign: 'center' },
  { prop: 'lineCostTotal', label: '进价总额', width: 120, align: 'right', headerAlign: 'center' },
  { prop: 'linePriceTotal', label: '售价总额', width: 120, align: 'right', headerAlign: 'center' },
]

// ==================== 计算函数 ====================

/** 进价总额合计 */
export function sumLineCostTotal(rows: { lineCostTotal?: number | null }[]): number {
  return rows.reduce((s, r) => s + (r.lineCostTotal ?? 0), 0)
}

/** 售价总额合计 */
export function sumLinePriceTotal(rows: { linePriceTotal?: number | null }[]): number {
  return rows.reduce((s, r) => s + (r.linePriceTotal ?? 0), 0)
}
