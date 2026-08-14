import { useProfiledHttp, type ProfiledHttpClient } from '../client'
import { getApiProfile, mockOrThrow } from '../profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from '../shared'
import {
  mockCreateMedication,
  mockGetMedicationDetail,
  mockListMedication,
  mockLoadMedicationFormOptions,
  mockRemoveMedication,
  mockSetMedicationEnabled,
  mockUpdateMedication
} from './mock'
import type {
  MedicationDetail,
  MedicationFormOptions,
  MedicationQuery,
  MedicationRecord,
  MedicationSavePayload
} from './type'

interface BackendMedication {
  id?: unknown
  version?: unknown
  busNo?: unknown
  code?: unknown
  name?: unknown
  genericName?: unknown
  tongyongname?: unknown
  categoryCode?: unknown
  categoryName?: unknown
  kinds?: unknown
  kind?: unknown
  totalVolume?: unknown
  specification?: unknown
  guige?: unknown
  doseFormCode?: unknown
  dosageForm?: unknown
  jixin?: unknown
  doseUnitCode?: unknown
  packageUnitCode?: unknown
  unit?: unknown
  danwei?: unknown
  manufacturerText?: unknown
  manufacturer?: unknown
  changjia?: unknown
  /** C6 供应商 */
  gongyingshang?: unknown
  /** C6 性质 */
  xingzhi?: unknown
  retailPrice?: unknown
  linghsoujia?: unknown
  /** C6 DrugListDTO 价格（string 类型） */
  price?: unknown
  /** C6 DrugListDTO 状态（integer：1 启用 / 0 禁用） */
  status?: unknown
  activeFlag?: unknown
  statusEnum?: unknown
  yaopingstatus?: unknown
  sortNo?: unknown
  sort?: unknown
  description?: unknown
  medicationdefinition?: unknown
  yaopingshuoming?: unknown
  updatedAt?: unknown
  /** C6 采购单位 */
  caigoudanwei?: unknown
  /** C6 采购倍数 */
  caigoubeishu?: unknown
  /** C6 购入价 */
  gourujia?: unknown
  /** C6 门诊属性 */
  menzhengshuxing?: unknown
  /** C6 库存单位 */
  kucundanwei?: unknown
  /** C6 库存倍数 */
  kucunbeishu?: unknown
  /** C6 临时属性 */
  linshishuxing?: unknown
  /** C6 包装倍数 */
  baozhuangbeishu?: unknown
  /** C6 包装单位 */
  baozhuangdanwei?: unknown
  /** C6 拆零单位 */
  chailingdanwei?: unknown
  /** C6 倍数 */
  beishu?: unknown
  /** C6 财务类型 */
  caiwuleixin?: unknown
  /** C6 批准文号 */
  wenhao?: unknown
  /** C6 国家编码 */
  guojiabianma?: unknown
}

const versions = new Map<string, number>()

function rememberVersion(item: BackendMedication): void {
  const id = asString(item.id)
  const version = asNumber(item.version, Number.NaN)
  if (id && Number.isFinite(version)) versions.set(id, version)
}

async function ensureMedicationVersion(
  profile: 'c6' | 'j4',
  id: string,
  http: ProfiledHttpClient
): Promise<number> {
  const cached = versions.get(id)
  if (cached !== undefined) return cached

  const res =
    profile === 'c6'
      ? await http.get<BackendMedication>('/drug/query-by-id', { id })
      : await http.get<BackendMedication>('/medicine/query-medicine', { id })
  if (res.data) rememberVersion(res.data)

  const version = versions.get(id)
  if (version === undefined) {
    throw new Error(`${profile.toUpperCase()} 药品数据未返回 version，无法安全执行写操作`)
  }
  return version
}

/** 列表映射：C6 DrugListDTO（8 字段）与 J4 MedicineListVO（9 字段）双端兼容。 */
function mapMedication(item: BackendMedication): MedicationRecord {
  rememberVersion(item)
  return {
    id: asString(item.id),
    // J4: busNo, C6: code
    code: asString(item.busNo ?? item.code),
    name: asString(item.name),
    // 列表接口均不返回详情字段，空值占位
    genericName: '',
    // J4: categoryCode, C6: kinds
    category: asString(item.categoryCode ?? item.kinds),
    categoryName: asString(item.categoryCode ?? item.kinds),
    // J4: totalVolume, C6: guige
    specification: asString(item.totalVolume ?? item.guige),
    dosageForm: '',
    dosageFormName: '',
    unit: '',
    // J4: manufacturerText, C6: changjia
    manufacturer: asString(item.manufacturerText ?? item.changjia),
    // J4: retailPrice (number), C6: price (string)
    retailPrice: asNumber(item.retailPrice ?? item.price),
    // J4: activeFlag (boolean), C6: status (integer)
    enabled: parseStatus(item.activeFlag ?? item.status),
    sort: 0,
    remark: '',
    updatedAt: ''
  }
}

/** 详情映射：C6 DrugDetailDTO 与 J4 MedicineDetailVO 双端兼容。 */
function mapDetail(item: BackendMedication): MedicationDetail {
  rememberVersion(item)
  return {
    id: asString(item.id),
    code: asString(item.busNo ?? item.id),
    name: asString(item.name),
    // J4: genericName, C6: tongyongname
    genericName: asString(item.genericName ?? item.tongyongname),
    // J4: categoryCode, C6: kinds
    category: asString(item.categoryCode ?? item.kinds),
    // J4: totalVolume, C6: guige
    specification: asString(item.totalVolume ?? item.guige),
    // J4: doseFormCode, C6: jixin
    dosageForm: asString(item.doseFormCode ?? item.jixin),
    // J4: doseUnitCode/packageUnitCode, C6: danwei
    unit: asString(item.doseUnitCode ?? item.packageUnitCode ?? item.danwei),
    // J4: manufacturerText, C6: changjia
    manufacturer: asString(item.manufacturerText ?? item.changjia),
    // J4: retailPrice (number), C6: linghsoujia (string)
    retailPrice: asNumber(item.retailPrice ?? item.linghsoujia),
    // J4: statusEnum, C6: yaopingstatus/activeFlag
    enabled: parseStatus(item.statusEnum ?? item.yaopingstatus ?? item.activeFlag),
    sort: asNumber(item.sortNo ?? item.sort),
    // J4: medicationdefinition, C6: yaopingshuoming
    remark: asString(item.medicationdefinition ?? item.yaopingshuoming)
  }
}

/**
 * C6 DrugAddDTO — 18 个必填字段（中文拼音）+ 常用可选字段。
 * 注意：DrugAddDTO 不包含 id 字段，id 由后端自动生成。
 */
function toC6CreateDTO(data: MedicationSavePayload) {
  return {
    // --- 18 个必填字段 ---
    /** 通用名称（必填） */
    tongyongname: data.genericName || data.name || '未命名',
    /** 药品名称（必填） */
    name: data.name || '未命名药品',
    /** 药品分类（必填） */
    kinds: data.category || '1',
    /** 生产厂家（必填） */
    changjia: data.manufacturer || '未知厂家',
    /** 供应商（必填，默认取生产厂家） */
    gongyingshang: data.manufacturer || '未知供应商',
    /** 性质：1=西药 2=中成药 3=中草药（必填） */
    xingzhi: 1,
    /** 规格（必填） */
    guige: data.specification || '1',
    /** 单位（必填） */
    danwei: data.unit || '1',
    /** 采购单位（必填，默认取单位） */
    caigoudanwei: data.unit || '1',
    /** 采购倍数（必填，>0） */
    caigoubeishu: 1,
    /** 购入价（必填，string 类型） */
    gourujia: String(Number.isFinite(data.retailPrice) ? data.retailPrice : 0),
    /** 门诊属性（必填，默认 "1"） */
    menzhengshuxing: '1',
    /** 库存单位（必填，默认取单位） */
    kucundanwei: data.unit || '1',
    /** 库存倍数（必填，>0） */
    kucunbeishu: 1,
    /** 临时属性（必填，默认 "1"） */
    linshishuxing: '1',
    /** 包装倍数（必填，>0） */
    baozhuangbeishu: 1,
    /** 包装单位（必填，默认取单位） */
    baozhuangdanwei: data.unit || '1',
    /** 拆零单位（必填，默认取单位） */
    chailingdanwei: data.unit || '1',
    // --- 常用可选字段 ---
    /** 剂型 */
    jixin: data.dosageForm || '',
    /** 零售价（string 类型，注意：Swagger 字段名为 lingsoujia 不是 linghsoujia） */
    lingsoujia: String(Number.isFinite(data.retailPrice) ? data.retailPrice : 0),
    /** 药品说明 */
    yaopingshuoming: data.remark || '',
    /** 启用状态 */
    activeFlag: data.enabled,
    /** 倍数 */
    beishu: 1,
    /** 财务类型 */
    caiwuleixin: '1'
  }
}

/**
 * C6 MedicineUpdateDTO — 2 个必填字段（id, version）+ 常用可选字段（英文驼峰）。
 * 对应 PUT /drug/modify 接口。
 */
function toC6UpdateDTO(id: string, data: MedicationSavePayload, version: number) {
  return {
    /** 药品 ID（必填） */
    id,
    /** 乐观锁版本号（必填） */
    version,
    /** 药品名称 */
    name: data.name,
    /** 药品分类编码 */
    categoryCode: data.category,
    /** 规格（MedicineUpdateDTO 字段名为 totalVolume） */
    totalVolume: data.specification,
    /** 剂型编码 */
    doseFormCode: data.dosageForm,
    /** 剂量单位编码 */
    doseUnitCode: data.unit,
    /** 包装单位编码 */
    packageUnitCode: data.unit,
    /** 库存单位编码 */
    stockUnitCode: data.unit,
    /** 生产厂家 */
    manufacturerText: data.manufacturer,
    /** 零售价 */
    retailPrice: Number.isFinite(data.retailPrice) ? data.retailPrice : 0,
    /** 药品说明 */
    definition: data.remark || ''
  }
}

/**
 * J4 MedicineAddDTO / MedicineDetailDTO 双端 DTO。
 * 前端表单仅采集核心字段，其余必填字段使用业务默认值补全。
 * MedicineAddDTO 共 22 个必填字段，均有防御性默认值兜底。
 */
function toJ4DTO(data: MedicationSavePayload, id?: string) {
  // 防御性默认值：表单字段为空时使用合理默认值，避免 9995 参数验证失败
  const safeCode = data.code || ''
  const safeName = data.name || '未命名药品'
  const safeCategory = data.category || '1'
  const safeSpec = data.specification || '1'
  const safeDoseForm = data.dosageForm || '1'
  const safeUnit = data.unit || '1'
  const safeManufacturer = data.manufacturer || '未知厂家'
  const safePrice = Number.isFinite(data.retailPrice) ? data.retailPrice : 0

  return {
    id,
    busNo: safeCode,
    /** 通用名称（必填） */
    name: safeName,
    /** 商品名称（必填，默认取通用名称） */
    merchandiseName: safeName,
    /** 药品分类编码（必填） */
    categoryCode: safeCategory,
    /** 生产厂家（必填） */
    manufacturerText: safeManufacturer,
    /** 供应商 ID（必填，默认取生产厂家） */
    supplyId: safeManufacturer,
    /** 财务类型编码（必填，默认 "1"） */
    typeCode: '1',
    /** 药理作用分类编码（必填） */
    pharmacologyCategoryCode: 'general',
    /** 规格（必填） */
    totalVolume: safeSpec,
    /** 剂型编码（必填） */
    doseFormCode: safeDoseForm,
    /** 剂量单位编码（必填） */
    doseUnitCode: safeUnit,
    /** 拆零/剂量倍数（必填，>0） */
    doseUnitRatio: 1,
    /** 门诊拆分属性编码（必填） */
    partAttributeEnum: '1',
    /** 住院临时医嘱拆分属性编码（必填） */
    thoPartAttributeEnum: '1',
    /** 购入价（必填，>=0） */
    purchasePrice: safePrice,
    /** 零售价（必填，>=0） */
    retailPrice: safePrice,
    /** 采购单位编码（必填，默认取剂量单位） */
    purchaseUnitCode: safeUnit,
    /** 采购/拆零倍数（必填，>0） */
    purchaseUnitRatio: 1,
    /** 库存单位编码（必填，默认取剂量单位） */
    stockUnitCode: safeUnit,
    /** 库存/拆零倍数（必填，>0） */
    stockUnitRatio: 1,
    /** 包装单位编码（必填，默认取剂量单位） */
    packageUnitCode: safeUnit,
    /** 包装/拆零倍数（必填，>0） */
    packageUnitRatio: 1,
    /** 拆零单位编码（必填，默认取剂量单位） */
    splitUnitCode: safeUnit,
    // --- 可选字段 ---
    genericName: data.genericName || safeName,
    statusEnum: data.enabled ? 'ACTIVE' : 'INACTIVE',
    activeFlag: data.enabled,
    sortNo: data.sort || 0,
    medicationdefinition: data.remark || ''
  }
}

function localDetail(id: string | undefined, data: MedicationSavePayload): MedicationDetail {
  return { ...data, id }
}

export const listMedication = async (
  query: MedicationQuery
): Promise<{ rows: MedicationRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendMedication>>('/drug', {
            keywords: query.keyword ?? '',
            kind: query.category ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendMedication>>('/medicine/query-medicine-list', {
            keyword: query.keyword ?? '',
            categoryCode: query.category ?? '',
            activeFlag: query.enabled == null ? undefined : String(query.enabled),
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    const rows = page.rows
      .map(mapMedication)
      .filter((row) => (query.enabled == null ? true : row.enabled === query.enabled))
    return { rows, total: query.enabled == null ? page.total : rows.length }
  } catch (error) {
    return mockOrThrow(error, () => mockListMedication(query))
  }
}

export const getMedicationDetail = async (id: string): Promise<MedicationDetail> => {
  try {
    const http = useProfiledHttp()
    const res =
      getApiProfile() === 'c6'
        ? await http.get<BackendMedication>('/drug/query-by-id', { id })
        : await http.get<BackendMedication>('/medicine/query-medicine', { id })
    if (!res.data) throw new Error('获取药品详情失败')
    return mapDetail(res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetMedicationDetail(id))
  }
}

export const createMedication = async (data: MedicationSavePayload): Promise<MedicationDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendMedication | string>('/drug/add', toC6CreateDTO(data))
        : await http.post<BackendMedication | string>('/medicine/add-medicine', toJ4DTO(data))
    return typeof res.data === 'object' && res.data
      ? mapDetail(res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateMedication(data))
  }
}

export const updateMedication = async (
  id: string,
  data: MedicationSavePayload
): Promise<MedicationDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const version = await ensureMedicationVersion(profile, id, http)
    if (profile === 'c6') {
      const res = await http.put<BackendMedication>(
        '/drug/modify',
        toC6UpdateDTO(id, data, version)
      )
      if (res.data) rememberVersion(res.data)
      return res.data ? mapDetail(res.data) : localDetail(id, data)
    }
    const res = await http.put<BackendMedication>('/medicine/update-medicine', {
      ...toJ4DTO(data, id),
      version
    })
    return res.data ? mapDetail(res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateMedication(id, data))
  }
}

export const setMedicationEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.put('/drug/modify-status', {
        items: [{ id, version: await ensureMedicationVersion('c6', id, http) }],
        status: enabled ? 'ACTIVE' : 'INACTIVE'
      })
    } else {
      await http.put(`/medicine/change-status/${enabled ? 1 : 0}`, { ids: [id], size: 1 })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetMedicationEnabled(id, enabled))
  }
}

export const removeMedication = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    if (getApiProfile() === 'c6') {
      await http.delete('/drug/remove-drug', {
        items: [{ id, version: await ensureMedicationVersion('c6', id, http) }]
      })
    } else {
      await http.delete('/medicine/delete-medicine', { ids: [id] })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveMedication(id))
  }
}

/** 药品分类和剂型选项暂无两端统一接口，明确使用本地配置。 */
export const loadMedicationFormOptions = async (): Promise<MedicationFormOptions> =>
  mockLoadMedicationFormOptions()

export type * from './type'
