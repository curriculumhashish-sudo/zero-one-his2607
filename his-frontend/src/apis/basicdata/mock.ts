import type {
  MedicationCatalogDetail,
  MedicationCatalogFormOptions,
  MedicationCatalogPageResult,
  MedicationCatalogQuery,
  MedicationCatalogRecord,
  MedicationCatalogSavePayload,
  MedicationStatus,
  SelectOption
} from './type'

// ==================== 下拉选项 ====================

const categoryOptions: SelectOption[] = [
  { label: '西药', value: 'WM' },
  { label: '中成药', value: 'TCM' },
  { label: '中草药', value: 'HM' },
  { label: '生物制品', value: 'BP' },
  { label: '原料药', value: 'RM' }
]

const supplierOptions: SelectOption[] = [
  { label: '国药控股', value: 'SUP-001' },
  { label: '上海医药', value: 'SUP-002' },
  { label: '华润医药', value: 'SUP-003' },
  { label: '九州通', value: 'SUP-004' }
]

const financialTypeOptions: SelectOption[] = [
  { label: '药品费', value: 'DRUG' },
  { label: '卫材费', value: 'MATERIAL' },
  { label: '检查费', value: 'EXAM' }
]

const propertyOptions: SelectOption[] = [
  { label: '普通药品', value: 'NORMAL' },
  { label: '特殊药品', value: 'SPECIAL' },
  { label: '贵重药品', value: 'PRECIOUS' }
]

const doseUnitOptions: SelectOption[] = [
  { label: 'mg', value: 'mg' },
  { label: 'g', value: 'g' },
  { label: 'ml', value: 'ml' },
  { label: 'μg', value: 'ug' },
  { label: 'IU', value: 'IU' }
]

const doseFormOptions: SelectOption[] = [
  { label: '片剂', value: 'TABLET' },
  { label: '胶囊', value: 'CAPSULE' },
  { label: '注射液', value: 'INJECTION' },
  { label: '口服液', value: 'ORAL_LIQUID' },
  { label: '颗粒剂', value: 'GRANULE' },
  { label: '软膏', value: 'OINTMENT' }
]

const methodOptions: SelectOption[] = [
  { label: '口服', value: 'PO' },
  { label: '静脉注射', value: 'IV' },
  { label: '肌肉注射', value: 'IM' },
  { label: '外用', value: 'TOPICAL' },
  { label: '皮下注射', value: 'SC' }
]

const rateOptions: SelectOption[] = [
  { label: '每日一次', value: 'QD' },
  { label: '每日两次', value: 'BID' },
  { label: '每日三次', value: 'TID' },
  { label: '每4小时一次', value: 'Q4H' },
  { label: '必要时', value: 'PRN' }
]

const purchaseUnitOptions: SelectOption[] = [
  { label: '盒', value: 'BOX' },
  { label: '瓶', value: 'BOTTLE' },
  { label: '支', value: 'AMP' },
  { label: '袋', value: 'BAG' }
]

const stockUnitOptions: SelectOption[] = [
  { label: '盒', value: 'BOX' },
  { label: '瓶', value: 'BOTTLE' },
  { label: '支', value: 'AMP' },
  { label: '袋', value: 'BAG' }
]

const packageUnitOptions: SelectOption[] = [
  { label: '盒', value: 'BOX' },
  { label: '瓶', value: 'BOTTLE' },
  { label: '支', value: 'AMP' },
  { label: '袋', value: 'BAG' }
]

const splitUnitOptions: SelectOption[] = [
  { label: '片', value: 'TAB' },
  { label: '粒', value: 'CAP' },
  { label: 'ml', value: 'ml' },
  { label: 'g', value: 'g' }
]

const partAttributeOptions: SelectOption[] = [
  { label: '可拆零', value: 'SPLITTABLE' },
  { label: '不可拆零', value: 'NON_SPLITTABLE' },
  { label: '整包装', value: 'WHOLE' }
]

const thoPartAttributeOptions: SelectOption[] = [
  { label: '可拆零', value: 'SPLITTABLE' },
  { label: '不可拆零', value: 'NON_SPLITTABLE' },
  { label: '整包装', value: 'WHOLE' }
]

const statusOptions: SelectOption[] = [
  { label: '草稿', value: 'DRAFT' },
  { label: '正常', value: 'ACTIVE' },
  { label: '停用', value: 'INACTIVE' },
  { label: '作废', value: 'DISCARDED' }
]

const domainOptions: SelectOption[] = [
  { label: '门诊', value: 'OUTPATIENT' },
  { label: '住院', value: 'INPATIENT' },
  { label: '门诊+住院', value: 'BOTH' }
]

const ybLevelOptions: SelectOption[] = [
  { label: '甲类', value: 'A' },
  { label: '乙类', value: 'B' },
  { label: '丙类', value: 'C' },
  { label: '自费', value: 'SELF' }
]

const settlementTypeOptions: SelectOption[] = [
  { label: '普通结算', value: 'NORMAL' },
  { label: '特殊结算', value: 'SPECIAL' }
]

const ybCategoryOptions: SelectOption[] = [
  { label: '医保内', value: 'INSIDE' },
  { label: '医保外', value: 'OUTSIDE' },
  { label: '部分医保', value: 'PARTIAL' }
]

const dddUnitOptions: SelectOption[] = [
  { label: 'mg', value: 'mg' },
  { label: 'g', value: 'g' },
  { label: 'ml', value: 'ml' }
]

// ==================== 类别名称映射 ====================

const categoryNameMap = new Map(categoryOptions.map((item) => [item.value, item.label]))
const doseFormNameMap = new Map(doseFormOptions.map((item) => [item.value, item.label]))

// ==================== Mock 数据 ====================

let sequence = 6

function now(): string {
  return new Date().toISOString().replace('T', ' ').slice(0, 19)
}

function seedDetail(
  id: string,
  busNo: string,
  name: string,
  merchandiseName: string,
  categoryCode: string,
  manufacturerText: string,
  supplyId: string,
  totalVolume: string,
  doseFormCode: string,
  activeFlag: boolean,
  statusEnum: MedicationStatus,
  overrides: Partial<MedicationCatalogDetail> = {}
): MedicationCatalogDetail {
  return {
    id,
    busNo,
    name,
    merchandiseName,
    categoryCode,
    manufacturerText,
    supplyId,
    totalVolume,
    doseFormCode,
    doseUnitCode: 'mg',
    doseUnitRatio: 1,
    purchaseUnitCode: 'BOX',
    purchaseUnitRatio: 10,
    purchasePrice: 15.5,
    partAttributeEnum: 'SPLITTABLE',
    stockUnitCode: 'BOX',
    stockUnitRatio: 1,
    retailPrice: 18.6,
    thoPartAttributeEnum: 'SPLITTABLE',
    packageUnitCode: 'BOX',
    packageUnitRatio: 10,
    splitUnitCode: 'TAB',
    statusEnum,
    activeFlag,
    ...overrides
  }
}

const details: MedicationCatalogDetail[] = [
  seedDetail(
    'med-001', 'MD0001', '阿莫西林胶囊', '阿莫仙', 'WM', '珠海联邦制药', 'SUP-001',
    '0.25g*50粒', 'CAPSULE', true, 'ACTIVE',
    { approvalNumber: '国药准字H44023521', nationalDrugCode: '86900453000451', ybNo: 'XB006A', ybMatchFlag: true, chrgitmLv: 'A', purchasePrice: 12.5, retailPrice: 14.8, pyStr: 'amxljn', rxFlag: true, basicFlag: true }
  ),
  seedDetail(
    'med-002', 'MD0002', '布洛芬缓释胶囊', '芬必得', 'WM', '中美天津史克制药', 'SUP-002',
    '0.3g*20粒', 'CAPSULE', true, 'ACTIVE',
    { approvalNumber: '国药准字H10983131', nationalDrugCode: '86900987000326', ybNo: 'XB041B', ybMatchFlag: true, chrgitmLv: 'B', purchasePrice: 18.0, retailPrice: 21.6, pyStr: 'blfhsmjn', rxFlag: false }
  ),
  seedDetail(
    'med-003', 'MD0003', '氯化钠注射液', '生理盐水', 'WM', '石家庄四药', 'SUP-003',
    '0.9%*250ml', 'INJECTION', true, 'ACTIVE',
    { approvalNumber: '国药准字H13023270', nationalDrugCode: '86902884000487', ybNo: 'XB055A', ybMatchFlag: true, chrgitmLv: 'A', purchasePrice: 3.5, retailPrice: 4.2, pyStr: 'lhnzsy', injectFlag: true, doseUnitCode: 'ml', doseUnitRatio: 1, purchaseUnitCode: 'BOTTLE', purchaseUnitRatio: 1, stockUnitCode: 'BOTTLE', packageUnitCode: 'BOTTLE', splitUnitCode: 'ml' }
  ),
  seedDetail(
    'med-004', 'MD0004', '板蓝根颗粒', '板蓝根', 'TCM', '广州白云山制药', 'SUP-001',
    '10g*20袋', 'GRANULE', true, 'ACTIVE',
    { approvalNumber: '国药准字Z44023683', ybNo: 'ZB001A', ybMatchFlag: true, chrgitmLv: 'A', purchasePrice: 8.5, retailPrice: 10.2, pyStr: 'blgkl', purchaseUnitCode: 'BAG', purchaseUnitRatio: 20, stockUnitCode: 'BAG', packageUnitCode: 'BAG', splitUnitCode: 'g' }
  ),
  seedDetail(
    'med-005', 'MD0005', '头孢克肟分散片', '世福素', 'WM', '广州白云山制药', 'SUP-004',
    '0.1g*6片', 'TABLET', false, 'INACTIVE',
    { approvalNumber: '国药准字H20050870', nationalDrugCode: '86900388000812', ybNo: 'XB021B', ybMatchFlag: true, chrgitmLv: 'B', purchasePrice: 22.0, retailPrice: 26.4, pyStr: 'tbkwfs', rxFlag: true, antibioticFlag: true }
  ),
  seedDetail(
    'med-006', 'MD0006', '双黄连口服液', '双黄连', 'TCM', '哈药集团三精制药', 'SUP-002',
    '10ml*10支', 'ORAL_LIQUID', true, 'ACTIVE',
    { approvalNumber: '国药准字Z10920057', ybNo: 'ZB003A', ybMatchFlag: true, chrgitmLv: 'A', purchasePrice: 11.8, retailPrice: 14.2, pyStr: 'shlkfy', purchaseUnitCode: 'BOTTLE', purchaseUnitRatio: 10, stockUnitCode: 'BOTTLE', packageUnitCode: 'BOTTLE', splitUnitCode: 'ml', doseUnitCode: 'ml', doseUnitRatio: 1 }
  )
]

// ==================== 工具函数 ====================

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function toRecord(detail: MedicationCatalogDetail): MedicationCatalogRecord {
  return {
    id: detail.id ?? '',
    busNo: detail.busNo ?? '',
    name: detail.name,
    merchandiseName: detail.merchandiseName,
    categoryCode: detail.categoryCode,
    categoryName: categoryNameMap.get(detail.categoryCode) ?? detail.categoryCode,
    totalVolume: detail.totalVolume,
    activeFlag: detail.activeFlag,
    manufacturerText: detail.manufacturerText,
    doseFormName: doseFormNameMap.get(detail.doseFormCode) ?? detail.doseFormCode,
    updatedAt: (detail as MedicationCatalogRecord).updatedAt ?? now()
  }
}

/** 为每条记录附加 updatedAt（详情中不含该字段，首次转换时补上） */
const recordTimestamps = new Map<string, string>()
for (const d of details) {
  recordTimestamps.set(d.id ?? '', '2026-07-15 09:30:00')
}

function getRecordTimestamp(id: string): string {
  return recordTimestamps.get(id) ?? now()
}

function requireDetail(id: string): MedicationCatalogDetail {
  const detail = details.find((item) => item.id === id)
  if (!detail) throw new Error('药品目录不存在')
  return detail
}

function validatePayload(payload: MedicationCatalogSavePayload) {
  if (!payload.name?.trim()) throw new Error('通用名称不能为空')
  if (!payload.merchandiseName?.trim()) throw new Error('商品名称不能为空')
  if (!payload.categoryCode) throw new Error('请选择药品分类')
  if (!payload.supplyId) throw new Error('请选择供应商')
  if (!payload.totalVolume?.trim()) throw new Error('规格不能为空')
  if (!payload.doseFormCode) throw new Error('请选择剂型')
  if (payload.purchasePrice < 0) throw new Error('购入价不能为负数')
  if (payload.retailPrice < 0) throw new Error('零售价不能为负数')
}

// ==================== Mock API ====================

export async function mockListMedicationCatalog(
  query: MedicationCatalogQuery
): Promise<MedicationCatalogPageResult> {
  await delay()
  let filtered = details.slice()

  const keyword = query.keyword?.trim().toLowerCase()
  if (keyword) {
    filtered = filtered.filter(
      (item) =>
        item.name.toLowerCase().includes(keyword) ||
        item.merchandiseName.toLowerCase().includes(keyword) ||
        (item.busNo ?? '').toLowerCase().includes(keyword)
    )
  }
  if (query.name?.trim()) {
    const name = query.name.trim().toLowerCase()
    filtered = filtered.filter((item) => item.name.toLowerCase().includes(name))
  }
  if (query.categoryCode) {
    filtered = filtered.filter((item) => item.categoryCode === query.categoryCode)
  }
  if (query.enabled != null) {
    filtered = filtered.filter((item) => item.activeFlag === query.enabled)
  }

  const total = filtered.length
  const start = (query.pageIndex - 1) * query.pageSize
  const pageRows = filtered.slice(start, start + query.pageSize).map((item) => {
    const record = toRecord(item)
    record.updatedAt = getRecordTimestamp(item.id ?? '')
    return record
  })

  return { rows: pageRows, total }
}

export async function mockGetMedicationCatalogDetail(
  id: string
): Promise<MedicationCatalogDetail> {
  await delay()
  return { ...requireDetail(id) }
}

export async function mockCreateMedicationCatalog(
  payload: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> {
  await delay()
  validatePayload(payload)
  sequence += 1
  const id = `med-${String(sequence).padStart(3, '0')}`
  const busNo = `MD${String(sequence).padStart(4, '0')}`
  const detail: MedicationCatalogDetail = {
    ...payload,
    id,
    busNo
  }
  details.push(detail)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockUpdateMedicationCatalog(
  id: string,
  payload: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> {
  await delay()
  validatePayload(payload)
  const detail = requireDetail(id)
  Object.assign(detail, payload)
  recordTimestamps.set(id, now())
  return { ...detail }
}

export async function mockSetMedicationCatalogEnabled(
  id: string,
  enabled: boolean
): Promise<void> {
  await delay()
  const detail = requireDetail(id)
  detail.activeFlag = enabled
  detail.statusEnum = enabled ? 'ACTIVE' : 'INACTIVE'
  recordTimestamps.set(id, now())
}

export async function mockRemoveMedicationCatalog(id: string): Promise<void> {
  await delay()
  const index = details.findIndex((item) => item.id === id)
  if (index === -1) throw new Error('药品目录不存在')
  details.splice(index, 1)
  recordTimestamps.delete(id)
}

export async function mockLoadMedicationCatalogFormOptions(): Promise<MedicationCatalogFormOptions> {
  await delay()
  return {
    categoryOptions,
    supplierOptions,
    financialTypeOptions,
    propertyOptions,
    doseUnitOptions,
    doseFormOptions,
    methodOptions,
    rateOptions,
    purchaseUnitOptions,
    stockUnitOptions,
    packageUnitOptions,
    splitUnitOptions,
    partAttributeOptions,
    thoPartAttributeOptions,
    statusOptions,
    domainOptions,
    ybLevelOptions,
    settlementTypeOptions,
    ybCategoryOptions,
    dddUnitOptions
  }
}
