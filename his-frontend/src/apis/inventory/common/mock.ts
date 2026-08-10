import type {
  FormConfig,
  PageResult,
  PurchaseInboundDetail,
  PurchaseInboundDetailResult,
  PurchaseInboundOrder,
  PurchaseInboundQuery,
  PurchaseInboundSavePayload,
  PurchaseInboundStatus,
  SupplierItem
} from './type'

const PHARMACIES = [
  '门诊西药房',
  '门诊中药房',
  '门诊耗材房',
  '住院西药房',
  '住院中药房',
  '住院耗材房'
]
const DRUG_SOURCE_DATABASES = ['西药库', '中药库', '耗材库']
const CATEGORIES = ['药品', '耗材']
const HANDLERS = ['张三', '李四', '王五', '赵六']
const APPROVERS = ['陈主任', '刘药师', '周科长']
const SUPPLIERS = ['九州通医药集团', '国药控股', '华润医药']

/** 状态常量 */
const STATUS = {
  EDITING: 0,
  PENDING: 1,
  REVIEWING: 2,
  WITHDRAWN: 3,
  APPROVED: 4,
  REJECTED: 5
} as const

const STATUS_TEXT: Record<number, string> = {
  [STATUS.EDITING]: '编辑中',
  [STATUS.PENDING]: '待审核',
  [STATUS.REVIEWING]: '审核中',
  [STATUS.WITHDRAWN]: '已撤回',
  [STATUS.APPROVED]: '同意',
  [STATUS.REJECTED]: '驳回'
}

function pad(n: number): string {
  return String(n).padStart(2, '0')
}

/** 生成随机金额（两位小数） */
function randomAmount(min: number, max: number): number {
  return Math.round((min + Math.random() * (max - min)) * 100) / 100
}

// 弹窗数据
const supplierData: SupplierItem[] = [
  {
    itemCode: 'MD0029',
    itemName: '盐酸氨溴索注射液',
    manufacturer: '湖北科伦药业有限公司',
    purchasePrice: 0.23,
    retailPrice: 0,
    packageUnit: '支',
    unitCode: '2',
    specification: '2ml:15mg',
    stockQuantity: 20,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323016347827048448'
  },
  {
    itemCode: 'MD0028',
    itemName: '注射用头孢呋辛钠',
    manufacturer: '山东润泽制药有限公司',
    purchasePrice: 7.97,
    retailPrice: 0,
    packageUnit: '瓶',
    unitCode: '3',
    specification: '0.75g',
    stockQuantity: 20,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323016347810271232'
  },
  {
    itemCode: 'MD0027',
    itemName: '氯化钠注射液',
    manufacturer: '四川科伦药业股份有限公司',
    purchasePrice: 2.99,
    retailPrice: 0,
    packageUnit: '瓶',
    unitCode: '3',
    specification: '250ml:2.25g',
    stockQuantity: 200,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323024481220038656'
  },
  {
    itemCode: 'MD0026',
    itemName: '氯化钠注射液',
    manufacturer: '四川科伦药业股份有限公司',
    purchasePrice: 2.66,
    retailPrice: 0,
    packageUnit: '瓶',
    unitCode: '3',
    specification: '100ml:0.9g',
    stockQuantity: 200,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323024481236815872'
  },
  {
    itemCode: 'MD0025',
    itemName: '布洛芬混悬液',
    manufacturer: '上海强生制药有限公司',
    purchasePrice: 13.17,
    retailPrice: 0,
    packageUnit: '盒',
    unitCode: '1',
    specification: '100ml:2g',
    stockQuantity: 10,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323024481249398784'
  },
  {
    itemCode: 'MD0024',
    itemName: '阿托伐他汀钙片',
    manufacturer: '晖致制药(大连)有限公司',
    purchasePrice: 38.49,
    retailPrice: 0,
    packageUnit: '盒',
    unitCode: '1',
    specification: '20mg',
    stockQuantity: 10,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323016347743162368'
  },
  {
    itemCode: 'MD0023',
    itemName: '瑞格列奈片',
    manufacturer: '江苏豪森药业集团有限公司',
    purchasePrice: 6.89,
    retailPrice: 0,
    packageUnit: '盒',
    unitCode: '1',
    specification: '1mg',
    stockQuantity: 10,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323024303205388288'
  },
  {
    itemCode: 'MD0022',
    itemName: '替米沙坦片',
    manufacturer: '上海信谊天平药业有限公司',
    purchasePrice: 13.36,
    retailPrice: 0,
    packageUnit: '盒',
    unitCode: '1',
    specification: '40mg',
    stockQuantity: 10,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323016347713802240'
  },
  {
    itemCode: 'MD0021',
    itemName: '头孢克肟胶囊',
    manufacturer: '华北制药河北华民药业有限责任公司',
    purchasePrice: 6.99,
    retailPrice: 0,
    packageUnit: '盒',
    unitCode: '1',
    specification: '0.1g',
    stockQuantity: 10,
    approvalNo: '',
    itemType: '西药',
    inventoryItemId: '323024481308119040'
  }
]

const mockOrders: PurchaseInboundOrder[] = Array.from({ length: 5 }, (_, index) => {
  const status = ([0, 1, 2, 3, 4, 5] as PurchaseInboundStatus[])[Math.floor(Math.random() * 6)] ?? 0
  const day = pad(Math.floor(Math.random() * 28) + 1) // 1~28日随机
  const h = 8 + (index % 10)
  const m = (index * 7) % 60
  const sourceDb = DRUG_SOURCE_DATABASES[index % DRUG_SOURCE_DATABASES.length] ?? '西药库'
  const targetPharmacy = PHARMACIES[(index + 1) % PHARMACIES.length] ?? PHARMACIES[0]!
  return {
    // -------- 主键与工作流字段 --------

    /** 单据唯一标识（API 主键，用于详情查询、状态变更等） 例: "pi-001" */
    id: `pi-${String(index + 1).padStart(3, '0')}`,
    /** 工作流状态  例: 0 */
    status,
    /** 项目类型（下拉筛选用） 例: "药品" | "耗材" */
    category: CATEGORIES[index % CATEGORIES.length] ?? '药品',

    // -------- 卡片队列展示字段 --------

    /** 单据号（卡片头部主标识，多字段 fallback 的首选） 例: "PD-2026-0001" */
    busNo: `PD-2026-${String(index + 1).padStart(4, '0')}`,
    /** 状态枚举值（与 status 同值，用于审批判定逻辑） 例: 0 */
    statusEnum: status,
    /** 状态展示文本（卡片状态徽章直接渲染） 例: "编辑中" */
    statusEnum_enumText: STATUS_TEXT[status] ?? '编辑中',
    /** 制单人展示名（卡片"制单人"行） 例: "张三" */
    applicantId_dictText: HANDLERS[index % HANDLERS.length] ?? '系统用户',
    /** 制单时间（卡片右上时间栏第一行，格式 YYYY-MM-DD HH:mm） 例: "2026-07-01 08:00" */
    applyTime: `2026-08-${day} ${pad(h)}:${pad(m)}`,
    /** 审批人展示名（仅待审核/审核中时有值；卡片"审批人"行） 例: "陈主任" */
    approverId_dictText:
      status === STATUS.PENDING || status === STATUS.REVIEWING
        ? (APPROVERS[index % APPROVERS.length] ?? '')
        : '',
    /** 审批时间（仅同意/驳回时有值；卡片右上时间栏第二行） 例: "2026-07-01 09:15" */
    approvalTime:
      status === STATUS.APPROVED || status === STATUS.REJECTED
        ? `2026-08-${day} ${pad(h + 1)}:${pad((m + 15) % 60)}`
        : '',
    /** 供应商展示名（showSupplier=true 时在卡片供应商行展示） 例: "九州通医药集团" */
    supplierId_dictText: SUPPLIERS[index % SUPPLIERS.length],
    /** 药源库名称（卡片票尾行左侧） 例: "西药库" */
    sourceLocationId_dictText: sourceDb,
    /** 目的药房名称（卡片票尾行 → 右侧） 例: "门诊中药房" */
    purposeLocationId_dictText: targetPharmacy,
    /** 总金额（卡片票尾行右侧，两位小数 + "元"；50~5000 随机） 例: 1234.56 */
    totalAmount: randomAmount(50, 5000)
  }
})

const detailByOrderId = new Map<string, PurchaseInboundDetail[]>()
let sequence = mockOrders.length

for (const order of mockOrders) {
  detailByOrderId.set(
    order.id,
    supplierData.slice(0, 3).map((item, index) =>
      normalizeDetail({
        id: `${order.id}-line-${index + 1}`,
        itemCode: item.itemCode,
        itemName: item.itemName,
        specification: item.specification,
        manufacturer: item.manufacturer,
        batchNo: `B2026070${index + 1}`,
        bookQuantity: item.stockQuantity,
        actualQuantity: item.stockQuantity,
        actualUnit: item.packageUnit,
        profitLoss: 0,
        unitPrice: item.purchasePrice,
        actualAmount: 0,
        profitLossAmount: 0,
        remark: ''
      })
    )
  )
}

function delay(ms = 80): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function cloneOrder(order: PurchaseInboundOrder): PurchaseInboundOrder {
  return { ...order }
}

function normalizeDetail(detail: PurchaseInboundDetail): PurchaseInboundDetail {
  const actualQuantity = Number(detail.actualQuantity) || 0
  const bookQuantity = Number(detail.bookQuantity) || 0
  const unitPrice = Number(detail.unitPrice) || 0
  const actualAmount = actualQuantity * unitPrice
  return {
    ...detail,
    actualQuantity,
    bookQuantity,
    unitPrice,
    profitLoss: actualQuantity - bookQuantity,
    actualAmount,
    profitLossAmount: actualAmount - bookQuantity * unitPrice
  }
}

function cloneDetails(details: PurchaseInboundDetail[]): PurchaseInboundDetail[] {
  return details.map((detail) => ({ ...detail }))
}

function requireOrder(id: string): PurchaseInboundOrder {
  const order = mockOrders.find((item) => item.id === id)
  if (!order) throw new Error('单据不存在')
  return order
}

function requireStatus(
  order: PurchaseInboundOrder,
  allowed: PurchaseInboundStatus[],
  action: string
) {
  if (!allowed.includes(order.status)) throw new Error(`当前状态不能${action}`)
}

function buildResult(order: PurchaseInboundOrder): PurchaseInboundDetailResult {
  return {
    order: cloneOrder(order),
    details: cloneDetails(detailByOrderId.get(order.id) ?? [])
  }
}

// 药房盘点列表分页查询
export async function mockPagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  await delay()
  const keyword = query.checkNo?.trim().toLowerCase()
  const filtered = mockOrders.filter((order) => {
    if (keyword) {
      // 卡片单据号
      const busNo = (order.busNo ?? '').toLowerCase()
      // 卡片库房名称
      const locationText = (order.sourceLocationId_dictText ?? '').toLowerCase()
      if (!busNo.includes(keyword) && !locationText.includes(keyword)) return false
    }
    if (query.startDate && query.endDate) {
      // 制单时间
      const applyDate = (order.applyTime ?? '').slice(0, 10)
      if (applyDate < query.startDate || applyDate > query.endDate) return false
    }
    if (query.checkType && order.sourceLocationId_dictText !== query.checkType) return false
    return true
  })
  return { rows: filtered.map(cloneOrder), total: filtered.length }
}

export async function mockLoadPurchaseInboundInit() {
  await delay()
  return {
    pharmacyOptions: PHARMACIES.map((value) => ({ label: value, value })),
    categoryOptions: CATEGORIES.map((value) => ({ label: value, value }))
  }
}

export async function mockGetPurchaseInboundDetail(
  id: string
): Promise<PurchaseInboundDetailResult> {
  await delay()
  return buildResult(requireOrder(id))
}

export async function mockCreatePurchaseInbound(
  payload: PurchaseInboundSavePayload
): Promise<PurchaseInboundDetailResult> {
  await delay()
  sequence += 1
  const now = new Date()
  const dateStr = now.toISOString().slice(0, 10)
  const id = `pi-${String(sequence).padStart(3, '0')}`
  const targetStatus = payload.targetStatus ?? 0
  const order: PurchaseInboundOrder = {
    id,
    status: targetStatus,
    category: payload.itemType,
    // 卡片展示字段
    busNo: `PD-2026-${String(sequence).padStart(4, '0')}`,
    statusEnum: targetStatus,
    statusEnum_enumText: STATUS_TEXT[targetStatus] ?? '编辑中',
    applicantId_dictText: (payload as any).applicantId_dictText || '当前用户',
    applyTime: `${dateStr} ${pad(now.getHours())}:${pad(now.getMinutes())}`,
    approverId_dictText: '',
    approvalTime: '',
    supplierId_dictText: '',
    sourceLocationId_dictText: (payload as any).sourceLocationId_dictText || payload.pharmacy,
    purposeLocationId_dictText:
      (payload as any).purposeLocationId_dictText || (payload as any).targetPharmacy || '',
    totalAmount: payload.details.reduce(
      (sum, d) => sum + (Number(d.actualQuantity) || 0) * (Number(d.unitPrice) || 0),
      0
    )
  }
  mockOrders.unshift(order)
  detailByOrderId.set(id, payload.details.map(normalizeDetail))
  return buildResult(order)
}

export async function mockUpdatePurchaseInbound(
  id: string,
  payload: PurchaseInboundSavePayload
): Promise<PurchaseInboundDetailResult> {
  await delay()
  const order = requireOrder(id)
  requireStatus(order, [0, 1], '修改')
  order.sourceLocationId_dictText = (payload as any).sourceLocationId_dictText || payload.pharmacy
  order.purposeLocationId_dictText =
    (payload as any).purposeLocationId_dictText || (payload as any).targetPharmacy || ''
  order.category = payload.itemType
  order.rejectReason = undefined
  if (payload.targetStatus != null) {
    order.status = payload.targetStatus
    order.statusEnum = payload.targetStatus
    order.statusEnum_enumText = STATUS_TEXT[payload.targetStatus] ?? ''
  }
  detailByOrderId.set(id, payload.details.map(normalizeDetail))
  return buildResult(order)
}

export async function mockRemovePurchaseInbound(id: string): Promise<void> {
  await delay()
  const order = requireOrder(id)
  requireStatus(order, [0, 1], '删除')
  const index = mockOrders.findIndex((item) => item.id === id)
  mockOrders.splice(index, 1)
  detailByOrderId.delete(id)
}

async function changeStatus(
  id: string,
  from: PurchaseInboundStatus[],
  to: PurchaseInboundStatus,
  action: string
): Promise<PurchaseInboundOrder> {
  await delay()
  const order = requireOrder(id)
  requireStatus(order, from, action)
  order.status = to
  order.statusEnum = to
  order.statusEnum_enumText = STATUS_TEXT[to] ?? ''
  return cloneOrder(order)
}

export const mockSubmitPurchaseInbound = (id: string) => changeStatus(id, [0, 1, 3], 2, '提交')
export const mockWithdrawPurchaseInbound = (id: string) => changeStatus(id, [2], 3, '撤回')
export const mockApprovePurchaseInbound = (id: string) => changeStatus(id, [2], 4, '审批')

export async function mockRejectPurchaseInbound(
  id: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  const order = requireOrder(id)
  requireStatus(order, [2], '驳回')
  if (!reason.trim()) throw new Error('请填写驳回原因')
  await delay()
  order.status = 5
  order.statusEnum = 5
  order.statusEnum_enumText = '驳回'
  order.rejectReason = reason.trim()
  return cloneOrder(order)
}

export async function mockLoadSupplierOptions(query: {
  keyword: string
  pageIndex: number
  pageSize: number
}): Promise<{ rows: SupplierItem[]; total: number }> {
  await delay()
  const keyword = query.keyword.trim().toLowerCase()
  const filtered = supplierData.filter(
    (item) =>
      !keyword ||
      item.itemName.toLowerCase().includes(keyword) ||
      item.itemCode.toLowerCase().includes(keyword) ||
      item.manufacturer.toLowerCase().includes(keyword)
  )
  const start = (query.pageIndex - 1) * query.pageSize
  return { rows: filtered.slice(start, start + query.pageSize), total: filtered.length }
}

/** 模拟获取表单配置 */
export async function mockGetFormConfig(): Promise<FormConfig> {
  await delay(100)
  return {
    columns: [
      { prop: 'itemName', label: '项目名称', width: 200, fixed: 'left' as const },
      { prop: 'specification', label: '规格', width: 120 },
      { prop: 'manufacturer', label: '厂家', minWidth: 160 },
      { prop: 'batchNo', label: '批号', width: 120 },
      {
        prop: 'bookQuantity',
        label: '账面数量',
        editable: true,
        editorType: 'number' as const,
        width: 100
      },
      { prop: 'actualQuantity', label: '实盘数量', width: 180 },
      { prop: 'profitLoss', label: '盈亏数量', width: 90 },
      { prop: 'unitPrice', label: '单价', width: 100 },
      { prop: 'actualAmount', label: '实盘金额', width: 100 },
      { prop: 'profitLossAmount', label: '盈亏金额', width: 100 },
      { prop: 'remark', label: '备注', width: 140 }
    ],
    rowDefaults: {
      itemName: '',
      specification: '',
      manufacturer: '',
      batchNo: '',
      bookQuantity: 0,
      actualQuantity: 0,
      actualUnit: '单位',
      profitLoss: 0,
      unitPrice: 0,
      actualAmount: 0,
      profitLossAmount: 0,
      remark: ''
    },
    selectTableColumns: [
      { prop: 'itemCode', label: '项目编码', width: 100 },
      { prop: 'itemName', label: '项目名称', minWidth: 140 },
      { prop: 'manufacturer', label: '厂家/产地', minWidth: 160 },
      { prop: 'specification', label: '规格', width: 120 },
      { prop: 'stockQuantity', label: '库存数量', width: 90 },
      { prop: 'packageUnit', label: '包装单位', width: 90 }
    ]
  }
}
