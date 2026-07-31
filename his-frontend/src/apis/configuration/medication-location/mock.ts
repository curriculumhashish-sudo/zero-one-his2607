import type { MedicationLocationRuleRecord } from './type'

let sequence = 3
let rules: MedicationLocationRuleRecord[] = [
  {
    id: 'medication-location-001',
    departmentName: '呼吸内科',
    itemType: 'DRUG',
    itemName: '阿莫西林胶囊',
    pharmacyName: '门诊西药房',
    routeName: '口服',
    priority: 1,
    enabled: true
  },
  {
    id: 'medication-location-002',
    departmentName: '中医科',
    itemType: 'HERBAL',
    itemName: '感冒清热颗粒',
    pharmacyName: '中药房',
    routeName: '口服',
    priority: 1,
    enabled: true
  },
  {
    id: 'medication-location-003',
    departmentName: '骨科',
    itemType: 'DEVICE',
    itemName: '石膏绷带',
    pharmacyName: '耗材库房',
    routeName: '外用',
    priority: 2,
    enabled: true
  }
]

function clone<T>(value: T): T {
  return structuredClone(value)
}

export async function mockSaveMedicationLocationRules(nextRules: MedicationLocationRuleRecord[]) {
  const used = new Set<string>()
  rules = nextRules.map((rule) => {
    if (!rule.departmentName.trim() || !rule.itemName.trim() || !rule.pharmacyName.trim()) {
      throw new Error('科室、项目和发药地点不能为空')
    }
    let id = rule.id
    if (!id) {
      sequence += 1
      id = `medication-location-${String(sequence).padStart(3, '0')}`
    }
    if (used.has(id)) throw new Error('取药科室规则重复')
    used.add(id)
    return {
      ...clone(rule),
      id,
      departmentName: rule.departmentName.trim(),
      itemName: rule.itemName.trim(),
      pharmacyName: rule.pharmacyName.trim(),
      routeName: rule.routeName.trim(),
      priority: Math.max(1, Number(rule.priority) || 1)
    }
  })
  return clone(rules)
}

export async function mockPageMedicationLocationRules(query: {
  pageIndex: number
  pageSize: number
  keyword?: string
}) {
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const filtered = keyword
    ? rules.filter((item) =>
        [item.departmentName, item.itemName, item.pharmacyName, item.routeName].some((value) =>
          value.toLowerCase().includes(keyword)
        )
      )
    : rules
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  return {
    pageIndex,
    pageSize,
    total: filtered.length,
    pages: Math.ceil(filtered.length / pageSize),
    rows: clone(filtered.slice((pageIndex - 1) * pageSize, pageIndex * pageSize))
  }
}
