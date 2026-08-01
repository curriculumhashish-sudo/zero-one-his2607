import type { StockListRow, StockListQuery, PageResult } from './type'

// ==================== 属性池（覆盖所有查询条件选项） ====================

const WAREHOUSES = ['耗材库', '西药库', '中药库', '门诊西药库', '门诊中药库', '住院西药库', '住院中药库', '门诊耗材库', '住院耗材库']

const SUPPLIERS = [
  '华东医疗器械制造有限公司', '华南中药饮品供应公司', '华中药品配送有限公司',
  '西南医用耗材制造有限公司', '华北诊断试剂制造厂', '东部医疗物流供应链有限公司',
  '华南医疗设备制造有限公司', '北方疫苗冷链供应有限公司', '华东一次性耗材制造有限公司',
  '中原中药材供应有限公司', '华西影像设备制造有限公司',
]

const CHRGITMLV = ['甲类', '乙类', '自费']
const STATUSES: { label: string; enum: string }[] = [
  { label: '已启用', enum: 'ACTIVE' },
  { label: '已停用', enum: 'INACTIVE' },
]

/** 按近似度分配供应商 */
function pickSupplier(i: number) { return SUPPLIERS[i % SUPPLIERS.length]! }
function pickWarehouse(i: number) { return WAREHOUSES[i % WAREHOUSES.length]! }
function pickChrgitmLv(i: number) { return CHRGITMLV[i % CHRGITMLV.length]! }
function pickStatus(i: number) { return STATUSES[i % 2]! }

// ==================== 物品模板 ====================

interface Template {
  name: string
  merchandiseName: string
  categoryLabel: string
  medicationCategoryCode?: string
  deviceCategoryCode?: string
  totalVolume: string
  doseFormLabel: string
  manufacturer: string
  approvalNumber: string | null
  ybNo: string | null
}

/** 确保每个药品类型至少覆盖 */
const MEDICINES: Template[] = [
  // 西药
  { name: '阿莫西林胶囊', merchandiseName: '阿莫西林胶囊', categoryLabel: '西药', medicationCategoryCode: '西药', totalVolume: '0.5g×24粒', doseFormLabel: '胶囊剂', manufacturer: '华北制药股份有限公司', approvalNumber: '国药准字H13020726', ybNo: 'YB0001' },
  { name: '头孢克肟片', merchandiseName: '头孢克肟片', categoryLabel: '西药', medicationCategoryCode: '西药', totalVolume: '0.1g×6片', doseFormLabel: '片剂', manufacturer: '广州白云山制药股份有限公司', approvalNumber: '国药准字H10930128', ybNo: 'YB0002' },
  { name: '布洛芬缓释胶囊', merchandiseName: '布洛芬缓释胶囊', categoryLabel: '西药', medicationCategoryCode: '西药', totalVolume: '0.3g×20粒', doseFormLabel: '胶囊剂', manufacturer: '中美天津史克制药有限公司', approvalNumber: '国药准字H10900089', ybNo: 'YB0003' },
  { name: '硝苯地平控释片', merchandiseName: '硝苯地平控释片', categoryLabel: '西药', medicationCategoryCode: '西药', totalVolume: '30mg×7片', doseFormLabel: '片剂', manufacturer: '拜耳医药保健有限公司', approvalNumber: '国药准字J20130118', ybNo: 'YB0004' },
  // 中成药
  { name: '板蓝根颗粒', merchandiseName: '板蓝根颗粒', categoryLabel: '中成药', medicationCategoryCode: '中成药', totalVolume: '10g×20袋', doseFormLabel: '颗粒剂', manufacturer: '广州白云山和记黄埔中药有限公司', approvalNumber: '国药准字Z44020072', ybNo: 'YB0101' },
  { name: '六味地黄丸', merchandiseName: '六味地黄丸', categoryLabel: '中成药', medicationCategoryCode: '中成药', totalVolume: '360丸/瓶', doseFormLabel: '丸剂', manufacturer: '北京同仁堂科技发展股份有限公司', approvalNumber: '国药准字Z11020056', ybNo: 'YB0102' },
  { name: '复方丹参片', merchandiseName: '复方丹参片', categoryLabel: '中成药', medicationCategoryCode: '中成药', totalVolume: '0.3g×60片', doseFormLabel: '片剂', manufacturer: '天士力医药集团股份有限公司', approvalNumber: '国药准字Z12020118', ybNo: 'YB0103' },
  // 中草药
  { name: '黄芪饮片', merchandiseName: '黄芪饮片', categoryLabel: '中草药', medicationCategoryCode: '中草药', totalVolume: '1kg/袋', doseFormLabel: '饮片', manufacturer: '康美药业股份有限公司', approvalNumber: null, ybNo: null },
  { name: '当归饮片', merchandiseName: '当归饮片', categoryLabel: '中草药', medicationCategoryCode: '中草药', totalVolume: '500g/袋', doseFormLabel: '饮片', manufacturer: '康美药业股份有限公司', approvalNumber: null, ybNo: null },
  // 外购药品
  { name: '注射用青霉素钠', merchandiseName: '注射用青霉素钠', categoryLabel: '外购药品', medicationCategoryCode: '外购药品', totalVolume: '80万单位/瓶', doseFormLabel: '注射剂', manufacturer: '华北制药股份有限公司', approvalNumber: '国药准字H13020635', ybNo: 'YB0201' },
  { name: '人血白蛋白', merchandiseName: '人血白蛋白', categoryLabel: '外购药品', medicationCategoryCode: '外购药品', totalVolume: '10g/瓶', doseFormLabel: '注射剂', manufacturer: '华兰生物工程股份有限公司', approvalNumber: '国药准字S10970066', ybNo: 'YB0202' },
  // 其他
  { name: '灵芝孢子粉', merchandiseName: '灵芝孢子粉', categoryLabel: '其他', medicationCategoryCode: '其他', totalVolume: '100g/盒', doseFormLabel: '颗粒剂', manufacturer: '浙江寿仙谷医药股份有限公司', approvalNumber: null, ybNo: null },
]

/** 确保每个耗材类型至少覆盖 */
const DEVICES: Template[] = [
  { name: '一次性静脉留置针', merchandiseName: '一次性静脉留置针', categoryLabel: '医用耗材', deviceCategoryCode: '医用耗材', totalVolume: '22G×1支', doseFormLabel: '—', manufacturer: '山东威高集团医用高分子制品股份有限公司', approvalNumber: '国械注准20173141075', ybNo: 'YB1001' },
  { name: '血糖试纸', merchandiseName: '血糖试纸', categoryLabel: '检验试剂', deviceCategoryCode: '检验试剂', totalVolume: '50条/盒', doseFormLabel: '—', manufacturer: '罗氏诊断产品(上海)有限公司', approvalNumber: '国械注准20172401046', ybNo: 'YB1002' },
  { name: '心脏起搏器', merchandiseName: '心脏起搏器', categoryLabel: '植入类', deviceCategoryCode: '植入类', totalVolume: 'DDDR型', doseFormLabel: '—', manufacturer: '美敦力(上海)管理有限公司', approvalNumber: '国械注准20173121013', ybNo: 'YB1003' },
  { name: '心电图纸', merchandiseName: '心电图纸', categoryLabel: '通讯类', deviceCategoryCode: '通讯类', totalVolume: '210mm×30m', doseFormLabel: '—', manufacturer: '天津广大纸业股份有限公司', approvalNumber: null, ybNo: null },
  { name: '无菌注射器', merchandiseName: '无菌注射器', categoryLabel: '单次消耗类', deviceCategoryCode: '单次消耗类', totalVolume: '5ml×100支', doseFormLabel: '—', manufacturer: '山东新华医疗器械股份有限公司', approvalNumber: '国械注准20173151038', ybNo: 'YB1005' },
  { name: '医用纱布绷带', merchandiseName: '医用纱布绷带', categoryLabel: '有源的', deviceCategoryCode: '有源的', totalVolume: '8cm×600cm', doseFormLabel: '—', manufacturer: '绍兴福清卫生用品有限公司', approvalNumber: '国械注准20173641082', ybNo: 'YB1006' },
  { name: '骨科钢板', merchandiseName: '骨科钢板', categoryLabel: '治疗设备', deviceCategoryCode: '治疗设备', totalVolume: '4孔直型', doseFormLabel: '—', manufacturer: '常州华森医疗器械有限公司', approvalNumber: '国械注准20173461019', ybNo: null },
  { name: '一次性手术刀片', merchandiseName: '一次性手术刀片', categoryLabel: '非院内使用', deviceCategoryCode: '非院内使用', totalVolume: '11#50片/盒', doseFormLabel: '—', manufacturer: '上海浦东金环医疗用品股份有限公司', approvalNumber: '国械注准20172011057', ybNo: 'YB1008' },
  { name: '检验用采血管', merchandiseName: '检验用采血管', categoryLabel: '试管类', deviceCategoryCode: '试管类', totalVolume: '5ml×100支', doseFormLabel: '—', manufacturer: 'BD医疗器械(上海)有限公司', approvalNumber: '国械注准20172411032', ybNo: 'YB1009' },
  { name: '血液透析管路', merchandiseName: '血液透析管路', categoryLabel: '床旁类', deviceCategoryCode: '床旁类', totalVolume: '成人型', doseFormLabel: '—', manufacturer: '费森尤斯医药用品(上海)有限公司', approvalNumber: '国械注准20173451075', ybNo: 'YB1010' },
  { name: '医用缝合线', merchandiseName: '医用缝合线', categoryLabel: '可重用类', deviceCategoryCode: '可重用类', totalVolume: '3-0×75cm', doseFormLabel: '—', manufacturer: '强生(上海)医疗器材有限公司', approvalNumber: '国械注准20173021125', ybNo: null },
  { name: 'CT造影剂', merchandiseName: 'CT造影剂', categoryLabel: '软件类', deviceCategoryCode: '软件类', totalVolume: '100ml/瓶', doseFormLabel: '—', manufacturer: '拜耳医药保健有限公司', approvalNumber: '国药准字H10970101', ybNo: 'YB1012' },
]

const ALL_TEMPLATES = [...MEDICINES, ...DEVICES]
const UNITS = ['盒', '支', '瓶', '袋', '箱', '包']

// ==================== 数据生成 ====================

let idSeq = 0
const rng = ((seed: number) => () => { seed = (seed * 1103515245 + 12345) & 0x7fffffff; return seed / 0x7fffffff })(42)

function randInt(min: number, max: number) { return Math.floor(rng() * (max - min + 1)) + min }
function pick<T>(arr: T[], i: number) { return arr[i % arr.length]! }

function randomDate(startYear: number, endYear: number): string {
  const y = randInt(startYear, endYear)
  const m = String(randInt(1, 12)).padStart(2, '0')
  const d = String(randInt(1, 28)).padStart(2, '0')
  return `${y}-${m}-${d} 00:00:00`
}

function buildLotNumber(i: number) {
  return `L${String(2026).slice(2)}${String(randInt(1, 12)).padStart(2, '0')}${String(i).padStart(3, '0')}`
}

function buildStockTexts(i: number, zeroStock: boolean) {
  if (zeroStock) return { packageStockText: '0盒', splitStockText: '0支' }
  const pkgQty = randInt(5, 200)
  const ratio = [1, 4, 10, 12, 20, 24, 100][i % 7]!
  const unit = UNITS[i % UNITS.length]!
  const splitUnit = UNITS[(i + 1) % UNITS.length]!
  return {
    packageStockText: `${pkgQty}${unit}`,
    splitStockText: `${pkgQty * ratio}${splitUnit}`,
    packageUnitRatio: ratio,
  }
}

function parseQty(text: string): number {
  const m = text.match(/^([\d.]+)/)
  return m ? parseFloat(m[1]) : 0
}

function buildRow(template: Template, i: number): StockListRow {
  idSeq++
  const id = String(idSeq)
  const zeroStock = i % 7 === 0 // 约1/7为零库存
  const status = pick(STATUSES, i)
  const stocks = buildStockTexts(i, zeroStock)
  const costPrice = parseFloat((rng() * 500 + 0.1).toFixed(4))
  const salePrice = parseFloat((costPrice * (1 + rng() * 0.5)).toFixed(4))
  const remain = zeroStock ? null : randInt(1, 1095)
  const prodDate = randomDate(2024, 2026)
  const expDate = remain != null
    ? (() => { const d = new Date(); d.setDate(d.getDate() + remain); return d.toISOString().slice(0, 10) + ' 00:00:00' })()
    : randomDate(2027, 2029)
  const hasTrace = rng() > 0.3

  return {
    id,
    productBusNo: `DD${String(id).padStart(4, '0')}`,
    name: template.name,
    merchandiseName: template.merchandiseName,
    categoryLabel: template.categoryLabel,
    lotNumber: buildLotNumber(i),
    ...stocks,
    warehouseName: pickWarehouse(i),
    totalVolume: template.totalVolume,
    manufacturer: template.manufacturer,
    supplierName: pickSupplier(i),
    doseFormLabel: template.doseFormLabel,
    approvalNumber: template.approvalNumber,
    ybNo: template.ybNo,
    expirationDate: expDate,
    productionDate: prodDate,
    remainingDays: remain,
    inventoryStatusLabel: status.label,
    inventoryStatusEnum: status.enum,
    chrgitmLvLabel: template.medicationCategoryCode === '中草药' ? null : pickChrgitmLv(i),
    lotCostPrice: costPrice,
    lotSalePrice: salePrice,
    lineCostTotal: parseFloat((costPrice * randInt(5, 50)).toFixed(4)),
    linePriceTotal: parseFloat((salePrice * randInt(5, 50)).toFixed(4)),
    traceNo: hasTrace ? `TRACE${String(id).padStart(6, '0')}` : null,
    medicationCategoryCode: template.medicationCategoryCode,
    deviceCategoryCode: template.deviceCategoryCode,
  }
}

// 模块加载时一次性生成（session 内稳定）
const MOCK_DATA: StockListRow[] = Array.from({ length: 48 }, (_, i) => {
  const tpl = ALL_TEMPLATES[i % ALL_TEMPLATES.length]!
  return buildRow(tpl, i)
})

// ==================== 过滤逻辑 ====================

function matchKeyword(row: StockListRow, kw: string): boolean {
  const fields = [row.productBusNo, row.name, row.merchandiseName, row.categoryLabel, row.lotNumber]
  return fields.some((f) => f.toLowerCase().includes(kw))
}

function filterData(query: StockListQuery): StockListRow[] {
  const kw = query.keyword?.trim().toLowerCase() || ''
  return MOCK_DATA.filter((row) => {
    if (kw && !matchKeyword(row, kw)) return false
    if (query.warehouseNames?.length && !query.warehouseNames.includes(row.warehouseName)) return false
    if (query.supplierNames?.length && !query.supplierNames.includes(row.supplierName)) return false
    if (query.medicationCategoryCodes?.length && !query.medicationCategoryCodes.includes(row.medicationCategoryCode ?? '')) return false
    if (query.deviceCategoryCodes?.length && !query.deviceCategoryCodes.includes(row.deviceCategoryCode ?? '')) return false
    if (query.inventoryStatusLabels?.length && !query.inventoryStatusLabels.includes(row.inventoryStatusLabel)) return false
    if (query.chrgitmLvLabels?.length && !query.chrgitmLvLabels.includes(row.chrgitmLvLabel ?? '')) return false
    if (query.expireWithinDays != null && (row.remainingDays == null || row.remainingDays > query.expireWithinDays)) return false
    if (query.zeroStockOnly === true) {
      if (parseQty(row.packageStockText) !== 0 || parseQty(row.splitStockText) !== 0) return false
    } else if (query.zeroStockOnly === false) {
      if (parseQty(row.packageStockText) === 0 && parseQty(row.splitStockText) === 0) return false
    }
    return true
  })
}

// ==================== 导出函数 ====================

function delay(ms = 120): Promise<void> {
  return new Promise((r) => setTimeout(r, ms))
}

export async function mockQueryStockList(query: StockListQuery): Promise<PageResult<StockListRow>> {
  await delay()
  const filtered = filterData(query)
  const start = (query.pageNo - 1) * query.pageSize
  const rows = filtered.slice(start, start + query.pageSize)
  return { rows, total: filtered.length }
}

export async function mockRetireItem(id: string): Promise<void> {
  await delay()
  const row = MOCK_DATA.find((r) => r.id === id)
  if (row) {
    row.inventoryStatusEnum = 'INACTIVE'
    row.inventoryStatusLabel = '已停用'
  }
}

export async function mockResumeItem(id: string): Promise<void> {
  await delay()
  const row = MOCK_DATA.find((r) => r.id === id)
  if (row) {
    row.inventoryStatusEnum = 'ACTIVE'
    row.inventoryStatusLabel = '已启用'
  }
}
