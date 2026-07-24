import type {
  PurchaseInboundOrder,
  PurchaseInboundQuery,
  PurchaseInboundDetail,
  SupplierItem,
  PageResult
} from './type'

const PHARMACIES = ['门诊西药房', '门诊中药房', '门诊耗材房', '住院西药房', '住院中药房', '住院耗材房']
const CATEGORIES = ['药品', '耗材']

/** 模拟采购入库单数据 */
const mockOrders: PurchaseInboundOrder[] = Array.from({ length: 12 }, (_, i) => ({
  id: `pi-${String(i + 1).padStart(3, '0')}`,
  docNo: `CG-2026-${String(i + 1).padStart(4, '0')}`,
  date: `2026-07-${String((i % 20) + 1).padStart(2, '0')}`,
  handler: ['张三', '李四', '王五', '赵六'][i % 4],
  counterparty: ['华东医药有限公司', '九州通医药集团', '国药控股股份有限公司', '华润医药商业集团'][i % 4],
  status: [0, 1, 2, 3][i % 4],
  type: PHARMACIES[i % PHARMACIES.length],
  category: CATEGORIES[i % 2]
}))

/** 模拟网络延迟 */
function delay(ms = 300): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

/** 模拟分页查询 */
export async function mockPagePurchaseInboundOrders(
  query: PurchaseInboundQuery
): Promise<PageResult<PurchaseInboundOrder>> {
  await delay()

  let filtered = [...mockOrders]

  if (query.keyword) {
    const kw = query.keyword.toLowerCase()
    filtered = filtered.filter(
      (o) => o.docNo.toLowerCase().includes(kw) || o.counterparty.toLowerCase().includes(kw)
    )
  }
  if (query.dateRange) {
    const [start, end] = query.dateRange
    filtered = filtered.filter((o) => o.date >= start && o.date <= end)
  }
  if (query.type) {
    filtered = filtered.filter((o) => o.type === query.type)
  }
  if (query.category) {
    filtered = filtered.filter((o) => o.category === query.category)
  }

  return { rows: filtered, total: filtered.length }
}

/** 模拟加载初始化数据 */
export async function mockLoadPurchaseInboundInit(): Promise<{
  pharmacyOptions: { label: string; value: string }[]
  categoryOptions: { label: string; value: string }[]
}> {
  await delay(200)
  return {
    pharmacyOptions: PHARMACIES.map((p) => ({ label: p, value: p })),
    categoryOptions: CATEGORIES.map((c) => ({ label: c, value: c }))
  }
}

/** 模拟获取采购入库详情 */
export async function mockGetPurchaseInboundDetail(id: string): Promise<{
  order: PurchaseInboundOrder
  details: PurchaseInboundDetail[]
}> {
  await delay()
  const order = mockOrders.find((o) => o.id === id) ?? mockOrders[0]
  const details: PurchaseInboundDetail[] = Array.from({ length: 3 }, (_, i) => ({
    id: `pid-${i + 1}`,
    orderId: id,
    drugCode: `YP-${String(1001 + i)}`,
    drugName: `测试药品${i + 1}`,
    specification: '10mg×20片',
    unit: '盒',
    quantity: (i + 1) * 10,
    unitPrice: (i + 1) * 15.5,
    amount: (i + 1) * 10 * (i + 1) * 15.5,
    batchNo: `B2026070${i + 1}`,
    expiryDate: `2028-0${i + 1}-01`
  }))
  return { order, details }
}

// ==================== 药品选项 ====================

const supplierData: SupplierItem[] = [
  { itemCode: 'MD0029', itemName: '盐酸氨溴索注射液', manufacturer: '湖北科伦药业有限公司', purchasePrice: 0.23, retailPrice: 0, packageUnit: '支', specification: '2ml:15mg', stockQuantity: 20, approvalNo: '' },
  { itemCode: 'MD0028', itemName: '注射用头孢呋辛钠', manufacturer: '山东润泽制药有限公司', purchasePrice: 7.97, retailPrice: 0, packageUnit: '瓶', specification: '0.75g', stockQuantity: 20, approvalNo: '' },
  { itemCode: 'MD0027', itemName: '氯化钠注射液', manufacturer: '四川科伦药业股份有限公司', purchasePrice: 2.99, retailPrice: 0, packageUnit: '瓶', specification: '250ml:2.25g', stockQuantity: 200, approvalNo: '' },
  { itemCode: 'MD0026', itemName: '氯化钠注射液', manufacturer: '四川科伦药业股份有限公司', purchasePrice: 2.66, retailPrice: 0, packageUnit: '瓶', specification: '100ml:0.9g', stockQuantity: 200, approvalNo: '' },
  { itemCode: 'MD0025', itemName: '布洛芬混悬液', manufacturer: '上海强生制药有限公司', purchasePrice: 13.17, retailPrice: 0, packageUnit: '盒', specification: '100ml:2g', stockQuantity: 10, approvalNo: '' },
  { itemCode: 'MD0024', itemName: '阿托伐他汀钙片', manufacturer: '晖致制药(大连)有限公司', purchasePrice: 38.49, retailPrice: 0, packageUnit: '盒', specification: '20mg', stockQuantity: 10, approvalNo: '' },
  { itemCode: 'MD0023', itemName: '瑞格列奈片', manufacturer: '江苏豪森药业集团有限公司', purchasePrice: 6.89, retailPrice: 0, packageUnit: '盒', specification: '1mg', stockQuantity: 10, approvalNo: '' },
  { itemCode: 'MD0022', itemName: '替米沙坦片', manufacturer: '上海信谊天平药业有限公司', purchasePrice: 13.36, retailPrice: 0, packageUnit: '盒', specification: '40mg', stockQuantity: 10, approvalNo: '' },
  { itemCode: 'MD0021', itemName: '头孢克肟胶囊', manufacturer: '华北制药河北华民药业有限责任公司', purchasePrice: 6.99, retailPrice: 0, packageUnit: '盒', specification: '0.1g', stockQuantity: 10, approvalNo: '' }
]

/** 模拟加载药品供应商选项（SelectTable 分页接口） */
export async function mockLoadSupplierOptions(query: {
  keyword: string
  pageIndex: number
  pageSize: number
}): Promise<{ rows: SupplierItem[]; total: number }> {
  await delay(200)
  let filtered = [...supplierData]
  if (query.keyword) {
    const kw = query.keyword.toLowerCase()
    filtered = filtered.filter(
      (item) =>
        item.itemName.toLowerCase().includes(kw) ||
        item.itemCode.toLowerCase().includes(kw) ||
        item.manufacturer.toLowerCase().includes(kw)
    )
  }
  const start = (query.pageIndex - 1) * query.pageSize
  return {
    rows: filtered.slice(start, start + query.pageSize),
    total: filtered.length
  }
}
