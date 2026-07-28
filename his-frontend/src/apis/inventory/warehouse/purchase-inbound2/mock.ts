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
const CATEGORIES = ['药品', '耗材']
const HANDLERS = ['张三', '李四', '王五', '赵六']

const supplierData: SupplierItem[] = [
  {
    itemCode: 'MD0029',
    itemName: '盐酸氨溴索注射液',
    manufacturer: '湖北科伦药业有限公司',
    purchasePrice: 0.23,
    retailPrice: 0,
    packageUnit: '支',
    specification: '2ml:15mg',
    stockQuantity: 20,
    approvalNo: ''
  },
  {
    itemCode: 'MD0028',
    itemName: '注射用头孢呋辛钠',
    manufacturer: '山东润泽制药有限公司',
    purchasePrice: 7.97,
    retailPrice: 0,
    packageUnit: '瓶',
    specification: '0.75g',
    stockQuantity: 20,
    approvalNo: ''
  },
  {
    itemCode: 'MD0027',
    itemName: '氯化钠注射液',
    manufacturer: '四川科伦药业股份有限公司',
    purchasePrice: 2.99,
    retailPrice: 0,
    packageUnit: '瓶',
    specification: '250ml:2.25g',
    stockQuantity: 200,
    approvalNo: ''
  },
  {
    itemCode: 'MD0026',
    itemName: '氯化钠注射液',
    manufacturer: '四川科伦药业股份有限公司',
    purchasePrice: 2.66,
    retailPrice: 0,
    packageUnit: '瓶',
    specification: '100ml:0.9g',
    stockQuantity: 200,
    approvalNo: ''
  },
  {
    itemCode: 'MD0025',
    itemName: '布洛芬混悬液',
    manufacturer: '上海强生制药有限公司',
    purchasePrice: 13.17,
    retailPrice: 0,
    packageUnit: '盒',
    specification: '100ml:2g',
    stockQuantity: 10,
    approvalNo: ''
  },
  {
    itemCode: 'MD0024',
    itemName: '阿托伐他汀钙片',
    manufacturer: '晖致制药(大连)有限公司',
    purchasePrice: 38.49,
    retailPrice: 0,
    packageUnit: '盒',
    specification: '20mg',
    stockQuantity: 10,
    approvalNo: ''
  },
  {
    itemCode: 'MD0023',
    itemName: '瑞格列奈片',
    manufacturer: '江苏豪森药业集团有限公司',
    purchasePrice: 6.89,
    retailPrice: 0,
    packageUnit: '盒',
    specification: '1mg',
    stockQuantity: 10,
    approvalNo: ''
  },
  {
    itemCode: 'MD0022',
    itemName: '替米沙坦片',
    manufacturer: '上海信谊天平药业有限公司',
    purchasePrice: 13.36,
    retailPrice: 0,
    packageUnit: '盒',
    specification: '40mg',
    stockQuantity: 10,
    approvalNo: ''
  },
  {
    itemCode: 'MD0021',
    itemName: '头孢克肟胶囊',
    manufacturer: '华北制药河北华民药业有限责任公司',
    purchasePrice: 6.99,
    retailPrice: 0,
    packageUnit: '盒',
    specification: '0.1g',
    stockQuantity: 10,
    approvalNo: ''
  }
]

const mockOrders: PurchaseInboundOrder[] = Array.from({ length: 12 }, (_, index) => {
  const pharmacy = PHARMACIES[index % PHARMACIES.length] ?? PHARMACIES[0]!
  return {
    id: `pi-${String(index + 1).padStart(3, '0')}`,
    docNo: `PD-2026-${String(index + 1).padStart(4, '0')}`,
    date: `2026-07-${String((index % 20) + 1).padStart(2, '0')}`,
    handler: HANDLERS[index % HANDLERS.length] ?? '系统用户',
    counterparty: pharmacy,
    status: ([0, 1, 2, 3] as PurchaseInboundStatus[])[index % 4] ?? 0,
    type: pharmacy,
    category: CATEGORIES[index % CATEGORIES.length] ?? '药品'
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

export async function mockPagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  await delay()
  const keyword = query.keyword?.trim().toLowerCase()
  const filtered = mockOrders.filter((order) => {
    if (keyword && !order.docNo.toLowerCase().includes(keyword) && !order.type.includes(keyword)) {
      return false
    }
    if (query.dateRange) {
      const [start, end] = query.dateRange
      if (order.date < start || order.date > end) return false
    }
    if (query.type && order.type !== query.type) return false
    if (query.category && order.category !== query.category) return false
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
  const id = `pi-${String(sequence).padStart(3, '0')}`
  const order: PurchaseInboundOrder = {
    id,
    docNo: `PD-2026-${String(sequence).padStart(4, '0')}`,
    date: new Date().toISOString().slice(0, 10),
    handler: '当前用户',
    counterparty: payload.pharmacy,
    status: 0,
    type: payload.pharmacy,
    category: payload.itemType
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
  requireStatus(order, [0, 3], '修改')
  order.type = payload.pharmacy
  order.category = payload.itemType
  order.counterparty = payload.pharmacy
  order.rejectReason = undefined
  detailByOrderId.set(id, payload.details.map(normalizeDetail))
  return buildResult(order)
}

export async function mockRemovePurchaseInbound(id: string): Promise<void> {
  await delay()
  const order = requireOrder(id)
  requireStatus(order, [0, 3], '删除')
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
  return cloneOrder(order)
}

export const mockSubmitPurchaseInbound = (id: string) => changeStatus(id, [0, 3], 1, '提交')
export const mockWithdrawPurchaseInbound = (id: string) => changeStatus(id, [1], 0, '撤回')
export const mockApprovePurchaseInbound = (id: string) => changeStatus(id, [1], 2, '审批')

export async function mockRejectPurchaseInbound(
  id: string,
  reason: string
): Promise<PurchaseInboundOrder> {
  const order = requireOrder(id)
  requireStatus(order, [1], '驳回')
  if (!reason.trim()) throw new Error('请填写驳回原因')
  await delay()
  order.status = 3
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
      { prop: 'manufacturer', label: '厂家/产地', minWidth: 160 },
      { prop: 'quantity', label: '数量', editable: true, editorType: 'number' as const, width: 100 },
      { prop: 'unitPrice', label: '单价', editable: true, editorType: 'number' as const, width: 100 },
      { prop: 'totalAmount', label: '总金额', width: 100 },
      { prop: 'batchNo', label: '批号', width: 120 },
      { prop: 'remark', label: '备注', width: 140 }
    ],
    rowDefaults: {
      itemName: '', specification: '', manufacturer: '',
      quantity: 0, unitPrice: 0, totalAmount: 0,
      batchNo: '', remark: ''
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
