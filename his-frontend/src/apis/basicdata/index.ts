import type { ProfiledHttpClient } from './client'
import { useProfiledHttp } from './client'
import { getApiProfile, mockOrThrow, type ApiProfile } from './profile'
import { asNumber, asString, normalizePage, parseStatus, type BackendPage } from './shared'
import {
  mockCreateMedicationCatalog,
  mockGetMedicationCatalogDetail,
  mockListMedicationCatalog,
  mockLoadMedicationCatalogFormOptions,
  mockRemoveMedicationCatalog,
  mockSetMedicationCatalogEnabled,
  mockUpdateMedicationCatalog
} from './mock'
import type {
  MedicationCatalogDetail,
  MedicationCatalogFormOptions,
  MedicationCatalogQuery,
  MedicationCatalogRecord,
  MedicationCatalogSavePayload,
  MedicationStatus
} from './type'

interface BackendMedicationCatalog {
  id?: unknown
  version?: unknown
  busNo?: unknown
  tongyongname?: unknown
  name?: unknown
  merchandiseName?: unknown
  categoryCode?: unknown
  categoryName?: unknown
  kinds?: unknown
  kind?: unknown
  manufacturerText?: unknown
  manufacturerId?: unknown
  changjia?: unknown
  supplyId?: unknown
  gongyingshang?: unknown
  approvalNumber?: unknown
  wenhao?: unknown
  nationalDrugCode?: unknown
  guojiabianma?: unknown
  ybNo?: unknown
  yibaobianma?: unknown
  ybMatchFlag?: unknown
  yibaoduima?: unknown
  financialTypeCode?: unknown
  financeTypeCode?: unknown
  caiwuleixin?: unknown
  chrgitmLv?: unknown
  yibaodengji?: unknown
  settlementType?: unknown
  jiesuanleixin?: unknown
  ybCategory?: unknown
  insuranceCategoryCode?: unknown
  yibaoleibie?: unknown
  characteristic?: unknown
  xingzhi?: unknown
  totalVolume?: unknown
  guige?: unknown
  doseUnitCode?: unknown
  danwei?: unknown
  doseUnitRatio?: unknown
  beishu?: unknown
  doseFormCode?: unknown
  jixin?: unknown
  skinTestFlag?: unknown
  pishipanbie?: unknown
  injectFlag?: unknown
  zhusheyaopin?: unknown
  usageLimit?: unknown
  yongliangxianding?: unknown
  methodCode?: unknown
  yongfa?: unknown
  rateCode?: unknown
  pingci?: unknown
  dose?: unknown
  dancijiliang?: unknown
  maxUnit?: unknown
  dancizuida?: unknown
  usageInstruction?: unknown
  dosageInstruction?: unknown
  yongfashuoming?: unknown
  restrictedScope?: unknown
  shiyongfanwei?: unknown
  medicationDefinition?: unknown
  medicationdefinition?: unknown
  yaopingshuoming?: unknown
  dddCode?: unknown
  DDDzhi?: unknown
  dddUnitCode?: unknown
  DDDdanwei?: unknown
  purchaseUnitCode?: unknown
  caigoudanwei?: unknown
  purchaseUnitRatio?: unknown
  caigoubeishu?: unknown
  purchasePrice?: unknown
  gourujia?: unknown
  partAttributeEnum?: unknown
  menzhengshuxing?: unknown
  stockUnitCode?: unknown
  kucundanwei?: unknown
  stockUnitRatio?: unknown
  kucunbeishu?: unknown
  retailPrice?: unknown
  linghsoujia?: unknown
  thoPartAttributeEnum?: unknown
  linshishuxing?: unknown
  packageUnitCode?: unknown
  baozhuangdanwei?: unknown
  packageUnitRatio?: unknown
  baozhuangbeishu?: unknown
  maxRetailPrice?: unknown
  maximumRetailPrice?: unknown
  zuigaolinshoujia?: unknown
  splitUnitCode?: unknown
  chailingdanwei?: unknown
  itemMaxQuantity?: unknown
  jinjiezuigao?: unknown
  itemMinQuantity?: unknown
  jinjiezuidi?: unknown
  pyStr?: unknown
  yaopingyinma?: unknown
  wbStr?: unknown
  merchandisePyStr?: unknown
  shangpingyinma?: unknown
  merchandiseWbStr?: unknown
  shangwubima?: unknown
  drug69Code?: unknown
  liujiuma?: unknown
  statusEnum?: unknown
  yaopingstatus?: unknown
  domainEnum?: unknown
  lingyu?: unknown
  basicFlag?: unknown
  childrenFlag?: unknown
  selfFlag?: unknown
  activeFlag?: unknown
  restrictedFlag?: unknown
  antibioticFlag?: unknown
  rxFlag?: unknown
  updatedAt?: unknown
}

const versions = new Map<string, number>()

function versionKey(profile: ApiProfile, id: string): string {
  return `${profile}:${id}`
}

function rememberVersion(profile: ApiProfile, item: BackendMedicationCatalog): void {
  const id = asString(item.id)
  const version = asNumber(item.version, Number.NaN)
  if (id && Number.isFinite(version)) versions.set(versionKey(profile, id), version)
}

async function ensureVersion(
  profile: ApiProfile,
  id: string,
  http: ProfiledHttpClient
): Promise<number> {
  const cached = versions.get(versionKey(profile, id))
  if (cached !== undefined) return cached

  const res =
    profile === 'c6'
      ? await http.get<BackendMedicationCatalog>('/drug/query-by-id', { id })
      : await http.get<BackendMedicationCatalog>('/medicine/query-medicine', { id })
  if (res.data) rememberVersion(profile, res.data)

  const version = versions.get(versionKey(profile, id))
  if (version === undefined) {
    throw new Error(`${profile.toUpperCase()} 药品数据未返回 version，无法安全执行写操作`)
  }
  return version
}

function catalogStatus(item: BackendMedicationCatalog): MedicationStatus {
  const raw = asString(item.statusEnum ?? item.yaopingstatus).toUpperCase()
  if (raw === 'DRAFT' || raw === 'DISCARDED' || raw === 'INACTIVE') return raw
  return parseStatus(item.activeFlag ?? raw) ? 'ACTIVE' : 'INACTIVE'
}

function mapCatalogRecord(
  profile: ApiProfile,
  item: BackendMedicationCatalog
): MedicationCatalogRecord {
  rememberVersion(profile, item)
  const categoryCode = asString(item.categoryCode ?? item.kinds ?? item.kind)
  const doseFormCode = asString(item.doseFormCode ?? item.jixin)
  return {
    id: asString(item.id),
    busNo: asString(item.busNo ?? item.id),
    name: asString(item.tongyongname ?? item.name),
    merchandiseName: asString(item.merchandiseName ?? item.name),
    categoryCode,
    categoryName: asString(item.categoryName ?? item.kinds ?? item.kind ?? categoryCode),
    totalVolume: asString(item.totalVolume ?? item.guige),
    activeFlag: parseStatus(item.activeFlag ?? item.statusEnum ?? item.yaopingstatus),
    manufacturerText: asString(item.manufacturerText ?? item.changjia),
    doseFormName: doseFormCode,
    updatedAt: asString(item.updatedAt)
  }
}

function mapCatalogDetail(
  profile: ApiProfile,
  item: BackendMedicationCatalog
): MedicationCatalogDetail {
  rememberVersion(profile, item)
  return {
    id: asString(item.id),
    busNo: asString(item.busNo ?? item.id),
    name: asString(item.tongyongname ?? item.name),
    merchandiseName: asString(item.merchandiseName ?? item.name),
    categoryCode: asString(item.categoryCode ?? item.kinds ?? item.kind),
    manufacturerText: asString(item.manufacturerText ?? item.changjia),
    manufacturerId: asString(item.manufacturerId),
    supplyId: asString(item.supplyId ?? item.gongyingshang),
    approvalNumber: asString(item.approvalNumber ?? item.wenhao),
    nationalDrugCode: asString(item.nationalDrugCode ?? item.guojiabianma),
    ybNo: asString(item.ybNo ?? item.yibaobianma),
    ybMatchFlag: parseStatus(item.ybMatchFlag ?? item.yibaoduima),
    financialTypeCode: asString(item.financialTypeCode ?? item.financeTypeCode ?? item.caiwuleixin),
    chrgitmLv: asString(item.chrgitmLv ?? item.yibaodengji),
    settlementType: asString(item.settlementType ?? item.jiesuanleixin),
    ybCategory: asString(item.ybCategory ?? item.insuranceCategoryCode ?? item.yibaoleibie),
    characteristic: asString(item.characteristic ?? item.xingzhi),
    totalVolume: asString(item.totalVolume ?? item.guige),
    doseUnitCode: asString(item.doseUnitCode ?? item.danwei),
    doseUnitRatio: asNumber(item.doseUnitRatio ?? item.beishu, 1),
    doseFormCode: asString(item.doseFormCode ?? item.jixin),
    skinTestFlag: parseStatus(item.skinTestFlag ?? item.pishipanbie),
    injectFlag: parseStatus(item.injectFlag ?? item.zhusheyaopin),
    usageLimit: asString(item.usageLimit ?? item.yongliangxianding),
    methodCode: asString(item.methodCode ?? item.yongfa),
    rateCode: asString(item.rateCode ?? item.pingci),
    dose: asNumber(item.dose ?? item.dancijiliang),
    maxUnit: asString(item.maxUnit ?? item.dancizuida),
    usageInstruction: asString(
      item.usageInstruction ?? item.dosageInstruction ?? item.yongfashuoming
    ),
    restrictedScope: asString(item.restrictedScope ?? item.shiyongfanwei),
    medicationDefinition: asString(
      item.medicationDefinition ?? item.medicationdefinition ?? item.yaopingshuoming
    ),
    dddCode: asString(item.dddCode ?? item.DDDzhi),
    dddUnitCode: asString(item.dddUnitCode ?? item.DDDdanwei),
    purchaseUnitCode: asString(item.purchaseUnitCode ?? item.caigoudanwei),
    purchaseUnitRatio: asNumber(item.purchaseUnitRatio ?? item.caigoubeishu, 1),
    purchasePrice: asNumber(item.purchasePrice ?? item.gourujia),
    partAttributeEnum: asString(item.partAttributeEnum ?? item.menzhengshuxing),
    stockUnitCode: asString(item.stockUnitCode ?? item.kucundanwei),
    stockUnitRatio: asNumber(item.stockUnitRatio ?? item.kucunbeishu, 1),
    retailPrice: asNumber(item.retailPrice ?? item.linghsoujia),
    thoPartAttributeEnum: asString(item.thoPartAttributeEnum ?? item.linshishuxing),
    packageUnitCode: asString(item.packageUnitCode ?? item.baozhuangdanwei),
    packageUnitRatio: asNumber(item.packageUnitRatio ?? item.baozhuangbeishu, 1),
    maxRetailPrice: asNumber(
      item.maxRetailPrice ?? item.maximumRetailPrice ?? item.zuigaolinshoujia
    ),
    splitUnitCode: asString(item.splitUnitCode ?? item.chailingdanwei),
    itemMaxQuantity: asNumber(item.itemMaxQuantity ?? item.jinjiezuigao),
    itemMinQuantity: asNumber(item.itemMinQuantity ?? item.jinjiezuidi),
    pyStr: asString(item.pyStr ?? item.yaopingyinma),
    wbStr: asString(item.wbStr),
    merchandisePyStr: asString(item.merchandisePyStr ?? item.shangpingyinma),
    merchandiseWbStr: asString(item.merchandiseWbStr ?? item.shangwubima),
    drug69Code: asString(item.drug69Code ?? item.liujiuma),
    statusEnum: catalogStatus(item),
    domainEnum: asString(item.domainEnum ?? item.lingyu),
    basicFlag: parseStatus(item.basicFlag),
    childrenFlag: parseStatus(item.childrenFlag),
    selfFlag: parseStatus(item.selfFlag),
    activeFlag: parseStatus(item.activeFlag ?? item.statusEnum ?? item.yaopingstatus),
    restrictedFlag: parseStatus(item.restrictedFlag),
    antibioticFlag: parseStatus(item.antibioticFlag),
    rxFlag: parseStatus(item.rxFlag)
  }
}

function toJ4DTO(data: MedicationCatalogSavePayload, id?: string, version?: number) {
  return {
    id,
    version,
    ...data,
    financeTypeCode: data.financialTypeCode,
    dosageInstruction: data.usageInstruction,
    medicationdefinition: data.medicationDefinition,
    maximumRetailPrice: data.maxRetailPrice
  }
}

function toC6CreateDTO(data: MedicationCatalogSavePayload) {
  return {
    id: data.id,
    tongyongname: data.name,
    name: data.merchandiseName || data.name,
    kinds: data.categoryCode,
    changjia: data.manufacturerText,
    gongyingshang: data.supplyId,
    wenhao: data.approvalNumber,
    guojiabianma: data.nationalDrugCode,
    yibaobianma: data.ybNo,
    caiwuleixin: data.financialTypeCode,
    yibaodengji: data.chrgitmLv,
    jiesuanleixin: data.settlementType,
    yibaoduima: data.ybMatchFlag,
    yibaoleibie: data.ybCategory,
    xingzhi: data.characteristic,
    guige: data.totalVolume,
    danwei: data.doseUnitCode,
    beishu: data.doseUnitRatio,
    jixin: data.doseFormCode,
    pishipanbie: data.skinTestFlag,
    zhusheyaopin: data.injectFlag,
    yongliangxianding: data.usageLimit,
    yongfa: data.methodCode,
    pingci: data.rateCode,
    dancijiliang: data.dose,
    dancizuida: data.maxUnit,
    yongfashuoming: data.usageInstruction,
    shiyongfanwei: data.restrictedScope,
    yaopingshuoming: data.medicationDefinition,
    DDDzhi: data.dddCode,
    DDDdanwei: data.dddUnitCode,
    caigoudanwei: data.purchaseUnitCode,
    caigoubeishu: data.purchaseUnitRatio,
    gourujia: String(data.purchasePrice),
    menzhengshuxing: data.partAttributeEnum,
    kucundanwei: data.stockUnitCode,
    kucunbeishu: data.stockUnitRatio,
    linghsoujia: String(data.retailPrice),
    linshishuxing: data.thoPartAttributeEnum,
    baozhuangbeishu: data.packageUnitRatio,
    baozhuangdanwei: data.packageUnitCode,
    zuigaolinshoujia: data.maxRetailPrice == null ? undefined : String(data.maxRetailPrice),
    chailingdanwei: data.splitUnitCode,
    jinjiezuigao: data.itemMaxQuantity == null ? undefined : String(data.itemMaxQuantity),
    jinjiezuidi: data.itemMinQuantity == null ? undefined : String(data.itemMinQuantity),
    yaopingyinma: data.pyStr,
    shangpingyinma: data.merchandisePyStr,
    shangwubima: data.merchandiseWbStr,
    liujiuma: data.drug69Code,
    yaopingstatus: data.statusEnum,
    lingyu: data.domainEnum,
    selfFlag: data.selfFlag,
    antibioticFlag: data.antibioticFlag,
    basicFlag: data.basicFlag,
    activeFlag: data.activeFlag,
    childrenFlag: data.childrenFlag,
    restrictedFlag: data.restrictedFlag,
    rxFlag: data.rxFlag
  }
}

function localDetail(
  id: string | undefined,
  data: MedicationCatalogSavePayload
): MedicationCatalogDetail {
  return { ...data, id }
}

export const listMedicationCatalog = async (
  query: MedicationCatalogQuery
): Promise<{ rows: MedicationCatalogRecord[]; total: number }> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendPage<BackendMedicationCatalog>>('/Drug', {
            keywords: query.keyword ?? '',
            name: query.name ?? '',
            kind: query.categoryCode ?? '',
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
        : await http.get<BackendPage<BackendMedicationCatalog>>('/medicine/query-medicine-list', {
            keyword: query.keyword ?? query.name ?? '',
            categoryCode: query.categoryCode ?? '',
            activeFlag: query.enabled == null ? undefined : String(query.enabled),
            pageIndex: query.pageIndex,
            pageSize: query.pageSize
          })
    const page = normalizePage(res.data)
    const rows = page.rows
      .map((item) => mapCatalogRecord(profile, item))
      .filter((row) => (query.enabled == null ? true : row.activeFlag === query.enabled))
    return { rows, total: query.enabled == null ? page.total : rows.length }
  } catch (error) {
    return mockOrThrow(error, () => mockListMedicationCatalog(query))
  }
}

export const getMedicationCatalogDetail = async (id: string): Promise<MedicationCatalogDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.get<BackendMedicationCatalog>('/drug/query-by-id', { id })
        : await http.get<BackendMedicationCatalog>('/medicine/query-medicine', { id })
    if (!res.data) throw new Error('获取药品目录详情失败')
    return mapCatalogDetail(profile, res.data)
  } catch (error) {
    return mockOrThrow(error, () => mockGetMedicationCatalogDetail(id))
  }
}

export const createMedicationCatalog = async (
  data: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const res =
      profile === 'c6'
        ? await http.post<BackendMedicationCatalog | string>('/Drug/add', toC6CreateDTO(data))
        : await http.post<BackendMedicationCatalog | string>(
            '/medicine/add-medicine',
            toJ4DTO(data)
          )
    return typeof res.data === 'object' && res.data
      ? mapCatalogDetail(profile, res.data)
      : localDetail(asString(res.data), data)
  } catch (error) {
    return mockOrThrow(error, () => mockCreateMedicationCatalog(data))
  }
}

export const updateMedicationCatalog = async (
  id: string,
  data: MedicationCatalogSavePayload
): Promise<MedicationCatalogDetail> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    const version = await ensureVersion(profile, id, http)
    const res =
      profile === 'c6'
        ? await http.patch<BackendMedicationCatalog>(
            `/api/v2/medicines/${encodeURIComponent(id)}`,
            toJ4DTO(data, undefined, version)
          )
        : await http.put<BackendMedicationCatalog>(
            '/medicine/update-medicine',
            toJ4DTO(data, id, version)
          )
    return res.data ? mapCatalogDetail(profile, res.data) : localDetail(id, data)
  } catch (error) {
    return mockOrThrow(error, () => mockUpdateMedicationCatalog(id, data))
  }
}

export const setMedicationCatalogEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    if (profile === 'c6') {
      await http.patch('/api/v2/medicines/status', {
        items: [{ id, version: await ensureVersion(profile, id, http) }],
        status: enabled ? 'ACTIVE' : 'INACTIVE'
      })
    } else {
      await http.put(`/medicine/change-status/${enabled ? 1 : 0}`, { ids: [id], size: 1 })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockSetMedicationCatalogEnabled(id, enabled))
  }
}

export const removeMedicationCatalog = async (id: string): Promise<void> => {
  try {
    const http = useProfiledHttp()
    const profile = getApiProfile()
    if (profile === 'c6') {
      await http.post('/api/v2/medicines/batch-delete', {
        items: [{ id, version: await ensureVersion(profile, id, http) }]
      })
    } else {
      await http.delete('/medicine/delete-medicine', { ids: [id] })
    }
  } catch (error) {
    await mockOrThrow(error, () => mockRemoveMedicationCatalog(id))
  }
}

/** 两端均未提供完整的药品目录表单选项接口，继续使用本地静态选项。 */
export const loadMedicationCatalogFormOptions = async (): Promise<MedicationCatalogFormOptions> =>
  mockLoadMedicationCatalogFormOptions()

export type * from './type'
