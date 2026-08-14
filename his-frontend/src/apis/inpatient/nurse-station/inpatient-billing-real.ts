import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import { realApiBaseURL } from './patient-real'
import type {
  NurseStationAutoBillingDataVO,
  NurseStationAutoBillingMutationResultVO,
  NurseStationAutoBillingQueryDTO,
  NurseStationAutoBillingRuleVO,
  NurseStationBillingCatalogItemVO,
  NurseStationBillingCatalogQueryDTO,
  NurseStationBillingWriteResultVO,
  NurseStationCancelBillingDTO,
  NurseStationCancelBillingResultVO,
  NurseStationConfirmBillingDTO,
  NurseStationDeleteAutoBillingRuleDTO,
  NurseStationPricedOrderQueryDTO,
  NurseStationPricedOrderVO,
  NurseStationSaveAutoBillingRuleDTO,
  NurseStationSupplementOrderQueryDTO,
  NurseStationSupplementOrderVO
} from './inpatient-billing-type'

interface JsonResponse<T> {
  data?: T
  message?: string
}

interface BackendCatalogRow {
  productDefinitionId?: string
  name?: string
  unitPrice?: number
  categoryCode?: string
  categoryCodeText?: string
  unitCode?: string
  unitCodeText?: string
  specificationText?: string
  organizationUnitId?: string
  organizationUnitName?: string
  productTableName?: string
  chargeItemDefinitionId?: string
}

interface BackendCatalogPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: BackendCatalogRow[]
}

interface BackendPricedOrder {
  serviceRequestId?: string
  requestId?: string
  productDefinitionId?: string
  productName?: string
  quantity?: number
  unitCode?: string
  unitPrice?: number
  totalPrice?: number
  pricedTime?: string
  orderType?: string
  chargeStatus?: string
  chargeStatusText?: string
  dispenseStatusText?: string
}

interface BackendPricedGroup {
  encounterId?: string
  pricedOrders?: BackendPricedOrder[]
}

interface BackendSupplementOrder {
  orderId?: string
  orderName?: string
  therapyType?: string
  therapyTypeText?: string
  dosageInstruction?: string
  frequencyCode?: string
  frequencyCodeText?: string
}

interface BackendSupplementGroup {
  encounterId?: string
  orders?: BackendSupplementOrder[]
}

interface BackendWriteResult {
  success?: boolean
  processedCount?: number
  createdServiceRequestIds?: string[]
  cancelledServiceRequestIds?: string[]
}

interface BackendAutoRule {
  ruleId?: string
  ruleName?: string
  activityId?: string
  activityName?: string
  chargeDefinitionId?: string
  quantityValue?: number
  isActive?: boolean
}

interface BackendAutoRuleList {
  configs?: BackendAutoRule[]
}

interface BackendAutoMutation {
  success?: boolean
  ruleId?: string
}

function unwrapData<T>(response: JsonResponse<T>, fallbackMessage: string): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || fallbackMessage)
  }
  return response.data
}

function http() {
  return useHttp()
}

function categoryCode(category: NurseStationBillingCatalogQueryDTO['category']) {
  // 当前后端未公布完整枚举，先使用与前端业务类别同名的编码。
  return category
}

function categoryFromBackend(
  code: string | undefined
): NurseStationBillingCatalogItemVO['category'] | null {
  const normalized = String(code || '').toUpperCase()
  if (!normalized || normalized === 'DRUG' || normalized === 'MEDICINE') return null
  if (['DEVICE', 'MATERIAL', 'CONSUMABLE', 'HC'].some((item) => normalized.includes(item))) {
    return 'DEVICE'
  }
  if (['SERVICE', 'TREATMENT', 'CARE'].some((item) => normalized.includes(item))) {
    return 'SERVICE'
  }
  return null
}

function mapCatalogRow(row: BackendCatalogRow): NurseStationBillingCatalogItemVO | null {
  const category = categoryFromBackend(row.categoryCode)
  if (!category || !row.productDefinitionId || !row.name) return null
  const missingFields: string[] = []
  const productTableName =
    row.productTableName || (category === 'DEVICE' ? 'device_definition' : 'healthcare_service')
  if (!row.productTableName) missingFields.push('productTableName')
  if (!row.chargeItemDefinitionId) missingFields.push('chargeItemDefinitionId')
  const unitCode = row.unitCode || ''
  if (!unitCode) missingFields.push('unitCode')
  return {
    productTableName,
    productDefinitionId: row.productDefinitionId,
    chargeItemDefinitionId: row.chargeItemDefinitionId || '',
    busNo: row.productDefinitionId,
    name: row.name,
    category,
    categoryText: row.categoryCodeText || (category === 'DEVICE' ? '卫生材料' : '诊疗项目'),
    specificationText: row.specificationText,
    unitCode,
    unitName: row.unitCodeText || unitCode,
    unitBands: unitCode
      ? [{ unitCode, unitName: row.unitCodeText || unitCode, ratioToSplit: 1 }]
      : [],
    unitPrice: Number(row.unitPrice || 0),
    defaultDispenseLocationId: category === 'DEVICE' ? row.organizationUnitId : undefined,
    defaultDispenseLocationName: category === 'DEVICE' ? row.organizationUnitName : undefined,
    backendCategoryCode: row.categoryCode,
    organizationUnitId: row.organizationUnitId,
    organizationUnitName: row.organizationUnitName,
    writable: missingFields.length === 0,
    missingFields
  }
}

function mapWriteResult(
  data: BackendWriteResult,
  items: NurseStationConfirmBillingDTO['items']
): NurseStationBillingWriteResultVO {
  const success = data.success === true
  const processedCount = Number(data.processedCount || 0)
  const requestIds = data.createdServiceRequestIds || []
  return {
    success,
    processedCount,
    results: items.map((item, index) => ({
      clientItemId: item.clientItemId,
      success: index < processedCount && success,
      message:
        index < processedCount && success ? '划价成功' : success ? '未处理' : '后端未处理该项目',
      requestId: requestIds[index],
      finalUnitPrice: item.previewUnitPrice,
      finalTotalPrice: item.previewUnitPrice * item.quantity
    }))
  }
}

/** 查询真实划价目录，并过滤无法明确归类的药品或未知项目。 */
export async function realQueryNurseStationBillingCatalog(
  query: NurseStationBillingCatalogQueryDTO
): Promise<PageDTO<NurseStationBillingCatalogItemVO>> {
  const response = await http().get<BackendCatalogPage>(
    '/inpatientbilling/page-pricing-catalog',
    {
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      encounterId: query.encounterId,
      patientId: query.patientId,
      keyword: query.keyword,
      categoryCode: categoryCode(query.category)
    },
    { baseURL: realApiBaseURL }
  )
  const page = unwrapData(response, '后端未返回划价目录')
  const rows = (page.rows || [])
    .map(mapCatalogRow)
    .filter((row): row is NurseStationBillingCatalogItemVO => Boolean(row))
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? rows.length,
    pages: page.pages ?? Math.ceil((page.total ?? rows.length) / Math.max(query.pageSize, 1)),
    rows
  }
}

function mapWriteItems(data: NurseStationConfirmBillingDTO) {
  return data.items.map((item) => ({
    encounterId: item.encounterId,
    patientId: item.patientId,
    enabled: true,
    accountId: item.accountId,
    productTableName: item.productTableName,
    productDefinitionId: item.productDefinitionId,
    quantity: item.quantity,
    unitCode: item.unitCode,
    chargeItemDefinitionId: item.chargeItemDefinitionId || undefined,
    unitPrice: item.previewUnitPrice,
    totalPrice: item.previewUnitPrice * item.quantity,
    remark: data.mode === 'SUPPLEMENT' ? '护士站医嘱补费' : '护士站直接划价',
    ...(item.parentOrderAnchor
      ? { parentRequestAnchor: `ORDER_${item.parentOrderAnchor.requestId}` }
      : {})
  }))
}

/** 直接划价和医嘱补费分别请求后端的两个 DTO。 */
export async function realConfirmNurseStationBilling(
  data: NurseStationConfirmBillingDTO
): Promise<NurseStationBillingWriteResultVO> {
  const items = mapWriteItems(data)
  const response =
    data.mode === 'DIRECT'
      ? await http().post<BackendWriteResult>(
          '/inpatientbilling/confirm-direct-pricing',
          { items },
          { baseURL: realApiBaseURL }
        )
      : await http().post<BackendWriteResult>(
          '/inpatientbilling/confirm-supplement-pricing',
          { items },
          { baseURL: realApiBaseURL }
        )
  return mapWriteResult(unwrapData(response, '后端未返回划价结果'), data.items)
}

/** 查询真实医嘱补费父医嘱；后端未提供补费子项时保留空明细。 */
export async function realQueryNurseStationSupplementOrders(
  query: NurseStationSupplementOrderQueryDTO
): Promise<NurseStationSupplementOrderVO[]> {
  const response = await http().get<{ groups?: BackendSupplementGroup[] }>(
    '/inpatientbilling/list-supplement-parent-orders',
    {
      encounterIds: [query.encounterId],
      therapyType:
        query.therapyType === 'ALL'
          ? undefined
          : query.therapyType === 'LONG'
            ? 'LONG_TERM'
            : 'TEMPORARY',
      keyword: query.keyword
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '后端未返回医嘱补费列表')
  const group = (data.groups || []).find(
    (item) => !item.encounterId || item.encounterId === query.encounterId
  )
  return (group?.orders || []).map((order) => ({
    requestId: order.orderId || '',
    orderType: 'SERVICE_REQUEST',
    encounterId: query.encounterId,
    patientId: query.patientId,
    therapyType: String(order.therapyType).toUpperCase().includes('LONG') ? 'LONG' : 'TEMP',
    therapyTypeText:
      order.therapyTypeText ||
      (String(order.therapyType).toUpperCase().includes('LONG') ? '长期' : '临时'),
    adviceDisplayName: order.orderName || '未提供医嘱名称',
    dosageDisplay: order.dosageInstruction,
    frequencyDisplay: order.frequencyCodeText || order.frequencyCode,
    authoredTime: '',
    statusText: '后端未提供状态',
    chargeItems: []
  }))
}

/** 查询真实已划价列表。后端字段不足时只读展示并禁用取消。 */
export async function realQueryNurseStationPricedOrders(
  query: NurseStationPricedOrderQueryDTO
): Promise<NurseStationPricedOrderVO[]> {
  const response = await http().get<{ groups?: BackendPricedGroup[] }>(
    '/inpatientbilling/list-priced-orders',
    {
      encounterIds: [query.encounterId],
      pricedTimeFrom: query.pricedTimeFrom,
      pricedTimeTo: query.pricedTimeTo
    },
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '后端未返回已划价列表')
  const group = (data.groups || []).find(
    (item) => !item.encounterId || item.encounterId === query.encounterId
  )
  return (group?.pricedOrders || []).map((row) => ({
    requestId: row.serviceRequestId || row.requestId || '',
    orderType: 'SERVICE_REQUEST',
    encounterId: query.encounterId,
    patientId: query.patientId,
    productDefinitionId: row.productDefinitionId || '',
    productName: row.productName || '未提供项目名称',
    quantity: Number(row.quantity || 0),
    unitCode: row.unitCode || '',
    unitName: row.unitCode || '',
    unitPrice: Number(row.unitPrice || 0),
    totalPrice: Number(row.totalPrice || 0),
    pricedTime: row.pricedTime || '',
    pricingMode: 'DIRECT',
    pricingModeText: '后端未提供来源',
    chargeStatus: 'PENDING',
    chargeStatusText: row.chargeStatusText || '后端未提供状态',
    dispenseStatusText: row.dispenseStatusText || '后端未提供状态',
    cancellable: false,
    cancelReasonDisabled: '后端未返回完整状态，暂不支持取消',
    backendFieldsIncomplete: true
  }))
}

/** 当前 Swagger 取消请求不支持幂等键和逐项原因，页面仅会对完整状态行调用。 */
export async function realCancelNurseStationBilling(
  data: NurseStationCancelBillingDTO
): Promise<NurseStationCancelBillingResultVO> {
  const response = await http().post<BackendWriteResult>(
    '/inpatientbilling/cancel-pricing',
    {
      items: data.items.map((item) => ({
        orderType: 'DIRECT',
        requestId: item.requestId,
        encounterId: item.encounterId
      }))
    },
    { baseURL: realApiBaseURL }
  )
  const result = unwrapData(response, '后端未返回取消结果')
  const ids = result.cancelledServiceRequestIds || []
  return {
    success: result.success === true,
    processedCount: Number(result.processedCount || 0),
    results: data.items.map((item, index) => ({
      requestId: item.requestId,
      success: ids.includes(item.requestId) || index < Number(result.processedCount || 0),
      message:
        ids.includes(item.requestId) || index < Number(result.processedCount || 0)
          ? '取消成功'
          : '后端未处理该项目'
    }))
  }
}

function mapAutoRule(rule: BackendAutoRule): NurseStationAutoBillingRuleVO {
  const active = rule.isActive === true
  return {
    ruleId: rule.ruleId || '',
    ruleName: rule.ruleName || '未提供规则名称',
    status: active ? 'ACTIVE' : 'INACTIVE',
    statusText: active ? '启用' : '停用',
    activityId: rule.activityId || '',
    activityName: rule.activityName || '未提供触发项目',
    chargeDefinitionId: rule.chargeDefinitionId || '',
    quantity: Number(rule.quantityValue || 0)
  }
}

/** 查询自动计费规则；护理级别布尔值不伪造成可编辑明细。 */
export async function realQueryNurseStationAutoBillingRules(
  query: NurseStationAutoBillingQueryDTO
): Promise<NurseStationAutoBillingDataVO> {
  const response = await http().get<BackendAutoRuleList>(
    '/inpatientbilling/list-auto-roll-configs',
    query,
    { baseURL: realApiBaseURL }
  )
  const data = unwrapData(response, '后端未返回自动计费规则')
  return {
    rules: (data.configs || []).map(mapAutoRule)
  }
}

function autoRulePayload(data: NurseStationSaveAutoBillingRuleDTO) {
  return {
    encounterId: data.encounterId,
    patientId: data.patientId,
    ruleName: data.ruleName,
    activityId: data.activityId,
    chargeDefinitionId: data.chargeDefinitionId,
    quantityValue: data.quantity,
    ...(data.ruleId ? { ruleId: data.ruleId } : {})
  }
}

/** 新增或修改自动计费规则；幂等键和版本号仅在前端保留。 */
export async function realSaveNurseStationAutoBillingRule(
  data: NurseStationSaveAutoBillingRuleDTO
): Promise<NurseStationAutoBillingMutationResultVO> {
  const path = data.ruleId
    ? '/inpatientbilling/update-auto-gen-rule'
    : '/inpatientbilling/create-auto-gen-rule'
  const response = await http().post<BackendAutoMutation>(path, autoRulePayload(data), {
    baseURL: realApiBaseURL
  })
  const result = unwrapData(response, '后端未返回自动计费规则保存结果')
  const success = data.ruleId ? result.success === true : Boolean(result.ruleId)
  return {
    success,
    message: success ? '自动计费规则保存成功' : '自动计费规则保存失败'
  }
}

/** 删除自动计费规则。 */
export async function realDeleteNurseStationAutoBillingRule(
  data: NurseStationDeleteAutoBillingRuleDTO
): Promise<NurseStationAutoBillingMutationResultVO> {
  const response = await http().post<BackendAutoMutation>(
    '/inpatientbilling/delete-auto-gen-rule',
    {
      ruleId: data.ruleId,
      encounterId: data.encounterId,
      patientId: data.patientId
    },
    { baseURL: realApiBaseURL }
  )
  const result = unwrapData(response, '后端未返回自动计费规则删除结果')
  return {
    success: result.success !== false,
    message: result.success === false ? '自动计费规则删除失败' : '自动计费规则已删除'
  }
}
