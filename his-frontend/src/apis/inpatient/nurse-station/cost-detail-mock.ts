import dayjs from 'dayjs'

import { createPageDTO } from '../../type'
import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationCostCategoryTotalVO,
  NurseStationCostDetailDataVO,
  NurseStationCostDetailInitVO,
  NurseStationCostDetailLineVO,
  NurseStationCostDetailQueryDTO,
  NurseStationCostExpenseBlock,
  NurseStationCostSummaryLineVO,
  NurseStationCostTotalsVO
} from './cost-detail-type'

const today = dayjs().format('YYYY-MM-DD')
const yesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')

const costDetailLines: NurseStationCostDetailLineVO[] = [
  {
    chargeItemId: 'cost-001',
    productId: 'drug-ceftriaxone',
    productTableName: 'medication_definition',
    itemName: '注射用头孢曲松钠',
    itemSpec: '2g/瓶',
    quantity: 2,
    quantityUnit: 'VIAL',
    quantityUnitDisplay: '瓶',
    unitPrice: 18.5,
    totalPrice: 37,
    businessDateTime: `${today} 08:20:00`,
    status: 'CHARGED',
    statusDisplay: '已收费',
    generateSource: 'ORDER',
    generateSourceDisplay: '医嘱计费',
    refundHighlight: false,
    serviceTable: 'medication_request',
    expenseBlockCode: 'MEDICATION',
    expenseBlockDisplay: '药品费',
    categoryCode: 'WESTERN_MEDICINE',
    categoryDisplay: '西药费',
    packageUnitCode: 'BOX',
    packageUnitDisplay: '盒',
    splitUnitCode: 'VIAL',
    splitUnitDisplay: '瓶',
    splitRatio: 10
  },
  {
    chargeItemId: 'cost-002',
    productId: 'drug-ambroxol',
    productTableName: 'medication_definition',
    itemName: '盐酸氨溴索注射液',
    itemSpec: '2ml:15mg',
    quantity: 2,
    quantityUnit: 'PCS',
    quantityUnitDisplay: '支',
    unitPrice: 6.2,
    totalPrice: 12.4,
    businessDateTime: `${today} 08:22:00`,
    status: 'PENDING',
    statusDisplay: '待收费',
    generateSource: 'ORDER',
    generateSourceDisplay: '医嘱计费',
    refundHighlight: false,
    serviceTable: 'medication_request',
    expenseBlockCode: 'MEDICATION',
    expenseBlockDisplay: '药品费',
    categoryCode: 'WESTERN_MEDICINE',
    categoryDisplay: '西药费',
    packageUnitCode: 'BOX',
    packageUnitDisplay: '盒',
    splitUnitCode: 'PCS',
    splitUnitDisplay: '支',
    splitRatio: 5
  },
  {
    chargeItemId: 'cost-003',
    productId: 'service-nursing-level-2',
    productTableName: 'healthcare_service',
    itemName: 'Ⅱ级护理',
    quantity: 1,
    quantityUnit: 'DAY',
    quantityUnitDisplay: '日',
    unitPrice: 18,
    totalPrice: 18,
    businessDateTime: `${today} 00:05:00`,
    status: 'CHARGED',
    statusDisplay: '已收费',
    generateSource: 'AUTO',
    generateSourceDisplay: '自动计费',
    refundHighlight: false,
    serviceTable: 'service_request',
    expenseBlockCode: 'SERVICE',
    expenseBlockDisplay: '诊疗费',
    categoryCode: 'NURSING',
    categoryDisplay: '护理费'
  },
  {
    chargeItemId: 'cost-004',
    productId: 'service-oxygen',
    productTableName: 'healthcare_service',
    itemName: '氧气吸入',
    quantity: 2,
    quantityUnit: 'HOUR',
    quantityUnitDisplay: '小时',
    unitPrice: 5,
    totalPrice: 10,
    businessDateTime: `${today} 09:10:00`,
    status: 'PENDING',
    statusDisplay: '待收费',
    generateSource: 'DIRECT',
    generateSourceDisplay: '直接划价',
    refundHighlight: false,
    serviceTable: 'service_request',
    expenseBlockCode: 'SERVICE',
    expenseBlockDisplay: '诊疗费',
    categoryCode: 'TREATMENT',
    categoryDisplay: '治疗费'
  },
  {
    chargeItemId: 'cost-005',
    productId: 'device-infusion-set',
    productTableName: 'device_definition',
    itemName: '一次性使用输液器',
    itemSpec: '带针 0.7mm',
    quantity: 1,
    quantityUnit: 'SET',
    quantityUnitDisplay: '套',
    unitPrice: 6.8,
    totalPrice: 6.8,
    businessDateTime: `${today} 08:25:00`,
    status: 'PENDING',
    statusDisplay: '待收费',
    generateSource: 'ORDER',
    generateSourceDisplay: '医嘱计费',
    refundHighlight: false,
    serviceTable: 'device_request',
    expenseBlockCode: 'DEVICE',
    expenseBlockDisplay: '耗材费',
    categoryCode: 'MATERIAL',
    categoryDisplay: '卫生材料费'
  },
  {
    chargeItemId: 'cost-006',
    productId: 'device-syringe-20',
    productTableName: 'device_definition',
    itemName: '一次性使用无菌注射器',
    itemSpec: '20ml',
    quantity: 2,
    quantityUnit: 'PCS',
    quantityUnitDisplay: '支',
    unitPrice: 1.5,
    totalPrice: 3,
    businessDateTime: `${today} 08:30:00`,
    status: 'CHARGED',
    statusDisplay: '已收费',
    generateSource: 'DIRECT',
    generateSourceDisplay: '直接划价',
    refundHighlight: false,
    serviceTable: 'device_request',
    expenseBlockCode: 'DEVICE',
    expenseBlockDisplay: '耗材费',
    categoryCode: 'MATERIAL',
    categoryDisplay: '卫生材料费'
  },
  {
    chargeItemId: 'cost-007-refund',
    productId: 'device-syringe-20',
    productTableName: 'device_definition',
    itemName: '一次性使用无菌注射器',
    itemSpec: '20ml',
    quantity: -1,
    quantityUnit: 'PCS',
    quantityUnitDisplay: '支',
    unitPrice: 1.5,
    totalPrice: -1.5,
    businessDateTime: `${today} 11:05:00`,
    status: 'REFUNDED',
    statusDisplay: '已退费',
    generateSource: 'REFUND',
    generateSourceDisplay: '退费冲销',
    refundHighlight: true,
    serviceTable: 'device_request',
    expenseBlockCode: 'DEVICE',
    expenseBlockDisplay: '耗材费',
    categoryCode: 'MATERIAL',
    categoryDisplay: '卫生材料费'
  },
  {
    chargeItemId: 'cost-008-yesterday',
    productId: 'service-blood-routine',
    productTableName: 'healthcare_service',
    itemName: '血细胞分析',
    itemSpec: '五分类',
    quantity: 1,
    quantityUnit: 'TIME',
    quantityUnitDisplay: '次',
    unitPrice: 28,
    totalPrice: 28,
    businessDateTime: `${yesterday} 10:35:00`,
    status: 'CHARGED',
    statusDisplay: '已收费',
    generateSource: 'ORDER',
    generateSourceDisplay: '医嘱计费',
    refundHighlight: false,
    serviceTable: 'service_request',
    expenseBlockCode: 'SERVICE',
    expenseBlockDisplay: '诊疗费',
    categoryCode: 'LABORATORY',
    categoryDisplay: '检验费'
  },
  {
    chargeItemId: 'cost-101-surgery',
    productId: 'service-dressing',
    productTableName: 'healthcare_service',
    itemName: '切口换药',
    quantity: 1,
    quantityUnit: 'TIME',
    quantityUnitDisplay: '次',
    unitPrice: 22,
    totalPrice: 22,
    businessDateTime: `${today} 09:40:00`,
    status: 'PENDING',
    statusDisplay: '待收费',
    generateSource: 'ORDER',
    generateSourceDisplay: '医嘱计费',
    refundHighlight: false,
    serviceTable: 'service_request',
    expenseBlockCode: 'SERVICE',
    expenseBlockDisplay: '诊疗费',
    categoryCode: 'TREATMENT',
    categoryDisplay: '治疗费'
  }
]

const lineOwnership: Record<string, { encounterId: string; patientId: string }> = {
  'cost-001': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-002': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-003': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-004': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-005': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-006': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-007-refund': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-008-yesterday': { encounterId: 'encounter-001', patientId: 'patient-001' },
  'cost-101-surgery': { encounterId: 'encounter-004', patientId: 'patient-004' }
}

function assertPatient(encounterId: string, patientId: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient || patient.patientId !== patientId) throw new Error('患者与住院记录不匹配')
}

function roundMoney(value: number) {
  return Number(value.toFixed(2))
}

function buildSummaryLines(lines: NurseStationCostDetailLineVO[]) {
  const groups = new Map<string, NurseStationCostDetailLineVO[]>()
  for (const line of lines) {
    const key = [line.productTableName, line.productId, line.expenseBlockCode, line.unitPrice].join(
      '|'
    )
    const group = groups.get(key) ?? []
    group.push(line)
    groups.set(key, group)
  }

  return [...groups.entries()]
    .map(([summaryKey, rows]): NurseStationCostSummaryLineVO => {
      const first = rows[0]!
      const netQuantity = rows.reduce((sum, row) => sum + row.quantity, 0)
      const splitRatio = first.splitRatio ?? 1
      const packageQuantity = splitRatio > 1 ? Math.trunc(netQuantity / splitRatio) : undefined
      const splitUnitQuantity =
        splitRatio > 1 ? roundMoney(netQuantity - (packageQuantity ?? 0) * splitRatio) : netQuantity
      return {
        summaryKey,
        productId: first.productId,
        productTableName: first.productTableName,
        itemName: first.itemName,
        itemSpec: first.itemSpec,
        categoryCode: first.categoryCode,
        categoryDisplay: first.categoryDisplay,
        unitPrice: first.unitPrice,
        lineTotalAmount: roundMoney(rows.reduce((sum, row) => sum + row.totalPrice, 0)),
        packageQuantity,
        splitUnitQuantity,
        packageUnitCode: first.packageUnitCode,
        packageUnitDisplay: first.packageUnitDisplay,
        splitUnitCode: first.splitUnitCode ?? first.quantityUnit,
        splitUnitDisplay: first.splitUnitDisplay ?? first.quantityUnitDisplay,
        expenseBlockCode: first.expenseBlockCode,
        expenseBlockDisplay: first.expenseBlockDisplay
      }
    })
    .sort(
      (a, b) =>
        a.expenseBlockCode.localeCompare(b.expenseBlockCode) ||
        a.itemName.localeCompare(b.itemName, 'zh-CN')
    )
}

function buildTotals(lines: NurseStationCostDetailLineVO[]): NurseStationCostTotalsVO {
  const blockTotal = (block: NurseStationCostExpenseBlock) =>
    roundMoney(
      lines
        .filter((line) => line.expenseBlockCode === block)
        .reduce((sum, line) => sum + line.totalPrice, 0)
    )
  const categoryMap = new Map<string, NurseStationCostCategoryTotalVO>()
  for (const line of lines) {
    const current = categoryMap.get(line.categoryCode) ?? {
      categoryCode: line.categoryCode,
      categoryDisplay: line.categoryDisplay,
      amount: 0
    }
    current.amount = roundMoney(current.amount + line.totalPrice)
    categoryMap.set(line.categoryCode, current)
  }
  const medicationTotal = blockTotal('MEDICATION')
  const serviceTotal = blockTotal('SERVICE')
  const deviceTotal = blockTotal('DEVICE')
  return {
    medicationTotal,
    serviceTotal,
    deviceTotal,
    categoryTotals: [...categoryMap.values()],
    grandTotal: roundMoney(medicationTotal + serviceTotal + deviceTotal)
  }
}

/** Mock：返回费用明细页面默认日期和支持的展示模式。 */
export async function mockLoadNurseStationCostDetailInit(): Promise<NurseStationCostDetailInitVO> {
  await Promise.resolve()
  return {
    dateFrom: today,
    dateTo: today,
    presentationOptions: [
      { code: 'DETAIL', display: '明细' },
      { code: 'SUMMARY', display: '汇总' }
    ],
    expenseBlockOrder: ['MEDICATION', 'SERVICE', 'DEVICE']
  }
}

/** Mock：按单患者、日期和展示模式返回同一口径的明细、汇总与合计。 */
export async function mockQueryNurseStationCostDetail(
  query: NurseStationCostDetailQueryDTO
): Promise<NurseStationCostDetailDataVO> {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  if (!dayjs(query.dateFrom).isValid() || !dayjs(query.dateTo).isValid()) {
    throw new Error('查询日期格式不正确')
  }
  if (dayjs(query.dateFrom).isAfter(dayjs(query.dateTo))) {
    throw new Error('开始日期不能晚于结束日期')
  }

  const filtered = costDetailLines
    .filter((line) => {
      const owner = lineOwnership[line.chargeItemId]
      const businessDate = line.businessDateTime.slice(0, 10)
      return (
        owner?.encounterId === query.encounterId &&
        owner.patientId === query.patientId &&
        businessDate >= query.dateFrom &&
        businessDate <= query.dateTo
      )
    })
    .sort((a, b) => b.businessDateTime.localeCompare(a.businessDateTime))
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, Math.min(query.pageSize, 100))
  const start = (pageIndex - 1) * pageSize
  const summaryLines = buildSummaryLines(filtered)

  return {
    presentationMode: query.presentationMode,
    detailPage: createPageDTO({
      pageIndex,
      pageSize,
      total: filtered.length,
      rows: filtered.slice(start, start + pageSize).map((line) => ({ ...line }))
    }),
    summaryLines,
    totals: buildTotals(filtered),
    totalDetailCount: filtered.length,
    summaryItemCount: summaryLines.length
  }
}
