import type {
  PageDispenseQueueParams,
  PageDispenseQueueResult,
  PagePatientQueueResult,
  DispenseSlipDto,
  InventoryLotDto,
  PharmacyDispenserDto,
  TraceCodeConfirmResult
} from './type'

function formatDateTime(date: Date): string {
  return date.toLocaleString('zh-CN', {
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

const now = new Date()

const mockPatients: PagePatientQueueResult['records'] = [
  {
    id: 'ENC001',
    name: '张明',
    gender: '男',
    age: '35',
    serviceType: '普通门诊',
    patientNo: 'PA2024001',
    visitNo: 'VIS20241201001',
    visitTime: formatDateTime(new Date(now.getTime() - 1800000)),
    departmentName: '内科',
    status: '待发药',
    statusTone: 'warning',
    paymentType: '医保'
  },
  {
    id: 'ENC002',
    name: '李华',
    gender: '女',
    age: '28',
    serviceType: '普通门诊',
    patientNo: 'PA2024002',
    visitNo: 'VIS20241201002',
    visitTime: formatDateTime(new Date(now.getTime() - 3600000)),
    departmentName: '皮肤科',
    status: '待发药',
    statusTone: 'warning',
    paymentType: '自费'
  },
  {
    id: 'ENC003',
    name: '王强',
    gender: '男',
    age: '55',
    serviceType: '专家门诊',
    patientNo: 'PA2024003',
    visitNo: 'VIS20241201003',
    visitTime: formatDateTime(new Date(now.getTime() - 5400000)),
    departmentName: '心血管科',
    status: '待发药',
    statusTone: 'warning',
    paymentType: '医保'
  },
  {
    id: 'ENC004',
    name: '刘芳',
    gender: '女',
    age: '42',
    serviceType: '普通门诊',
    patientNo: 'PA2024004',
    visitNo: 'VIS20241201004',
    visitTime: formatDateTime(new Date(now.getTime() - 7200000)),
    departmentName: '妇科',
    status: '待发药',
    statusTone: 'warning',
    paymentType: '医保'
  },
  {
    id: 'ENC005',
    name: '陈磊',
    gender: '男',
    age: '18',
    serviceType: '急诊',
    patientNo: 'PA2024005',
    visitNo: 'VIS20241201005',
    visitTime: formatDateTime(new Date(now.getTime() - 900000)),
    departmentName: '急诊科',
    status: '待发药',
    statusTone: 'warning',
    paymentType: '自费'
  },
  {
    id: 'ENC006',
    name: '赵敏',
    gender: '女',
    age: '65',
    serviceType: '普通门诊',
    patientNo: 'PA2024006',
    visitNo: 'VIS20241201006',
    visitTime: formatDateTime(new Date(now.getTime() - 10800000)),
    departmentName: '骨科',
    status: '已发药',
    statusTone: 'success',
    paymentType: '医保'
  },
  {
    id: 'ENC007',
    name: '孙伟',
    gender: '男',
    age: '30',
    serviceType: '普通门诊',
    patientNo: 'PA2024007',
    visitNo: 'VIS20241201007',
    visitTime: formatDateTime(new Date(now.getTime() - 12600000)),
    departmentName: '消化内科',
    status: '已发药',
    statusTone: 'success',
    paymentType: '自费'
  },
  {
    id: 'ENC008',
    name: '周静',
    gender: '女',
    age: '25',
    serviceType: '普通门诊',
    patientNo: 'PA2024008',
    visitNo: 'VIS20241201008',
    visitTime: formatDateTime(new Date(now.getTime() - 14400000)),
    departmentName: '眼科',
    status: '已发药',
    statusTone: 'success',
    paymentType: '医保'
  }
]

const mockInventoryLots: InventoryLotDto[] = [
  {
    inventoryId: 'INV001',
    inventoryName: '西药房-A库',
    lotNo: 'LOT20240101',
    batchNo: 'B20240101',
    expiryDate: '2026-12-31',
    availableQty: 100,
    unit: '盒',
    price: 25.50
  },
  {
    inventoryId: 'INV002',
    inventoryName: '西药房-B库',
    lotNo: 'LOT20240601',
    batchNo: 'B20240601',
    expiryDate: '2025-12-31',
    availableQty: 50,
    unit: '盒',
    price: 24.80
  },
  {
    inventoryId: 'INV003',
    inventoryName: '中药房',
    lotNo: 'LOT20241001',
    batchNo: 'B20241001',
    expiryDate: '2026-10-01',
    availableQty: 200,
    unit: '克',
    price: 0.50
  },
  {
    inventoryId: 'INV004',
    inventoryName: '西药房-A库',
    lotNo: 'LOT20240301',
    batchNo: 'B20240301',
    expiryDate: '2025-06-30',
    availableQty: 30,
    unit: '盒',
    price: 18.00
  }
]

const mockDispenserList: PharmacyDispenserDto[] = [
  { dispenserId: 'DISP001', name: '张药师', code: 'Y001' },
  { dispenserId: 'DISP002', name: '李药师', code: 'Y002' },
  { dispenserId: 'DISP003', name: '王药师', code: 'Y003' }
]

function buildSlipsForEncounter(encounterId: string): DispenseSlipDto[] {
  const slipMap: Record<string, DispenseSlipDto> = {
    ENC001: {
      slipId: 'SLIP001',
      encounterId: 'ENC001',
      patientId: 'PAT001',
      patientName: '张明',
      patientNo: 'PA2024001',
      visitNo: 'VIS20241201001',
      doctorName: '李医生',
      departmentName: '内科',
      createTime: formatDateTime(new Date(now.getTime() - 1800000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM001',
          slipId: 'SLIP001',
          drugId: 'DRUG001',
          itemName: '阿莫西林胶囊',
          spec: '0.5g*20粒',
          dosage: '0.5g',
          route: '口服',
          frequency: '每日3次',
          days: 7,
          quantity: 2,
          unit: '盒',
          price: 25.50,
          totalAmount: 51.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        },
        {
          itemId: 'ITEM002',
          slipId: 'SLIP001',
          drugId: 'DRUG002',
          itemName: '布洛芬缓释胶囊',
          spec: '0.3g*20粒',
          dosage: '0.3g',
          route: '口服',
          frequency: '每日2次',
          days: 3,
          quantity: 1,
          unit: '盒',
          price: 18.00,
          totalAmount: 18.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[3]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        }
      ]
    },
    ENC002: {
      slipId: 'SLIP002',
      encounterId: 'ENC002',
      patientId: 'PAT002',
      patientName: '李华',
      patientNo: 'PA2024002',
      visitNo: 'VIS20241201002',
      doctorName: '王医生',
      departmentName: '皮肤科',
      createTime: formatDateTime(new Date(now.getTime() - 3600000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM003',
          slipId: 'SLIP002',
          drugId: 'DRUG003',
          itemName: '氯雷他定片',
          spec: '10mg*14片',
          dosage: '10mg',
          route: '口服',
          frequency: '每日1次',
          days: 7,
          quantity: 1,
          unit: '盒',
          price: 22.00,
          totalAmount: 22.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        }
      ]
    },
    ENC003: {
      slipId: 'SLIP003',
      encounterId: 'ENC003',
      patientId: 'PAT003',
      patientName: '王强',
      patientNo: 'PA2024003',
      visitNo: 'VIS20241201003',
      doctorName: '赵医生',
      departmentName: '心血管科',
      createTime: formatDateTime(new Date(now.getTime() - 5400000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM004',
          slipId: 'SLIP003',
          drugId: 'DRUG004',
          itemName: '硝苯地平缓释片',
          spec: '20mg*30片',
          dosage: '20mg',
          route: '口服',
          frequency: '每日2次',
          days: 14,
          quantity: 2,
          unit: '盒',
          price: 35.00,
          totalAmount: 70.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        },
        {
          itemId: 'ITEM005',
          slipId: 'SLIP003',
          drugId: 'DRUG005',
          itemName: '阿司匹林肠溶片',
          spec: '100mg*30片',
          dosage: '100mg',
          route: '口服',
          frequency: '每日1次',
          days: 30,
          quantity: 1,
          unit: '盒',
          price: 15.00,
          totalAmount: 15.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[3]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        },
        {
          itemId: 'ITEM006',
          slipId: 'SLIP003',
          drugId: 'DRUG006',
          itemName: '丹参滴丸',
          spec: '27mg*150丸',
          dosage: '10丸',
          route: '口服',
          frequency: '每日3次',
          days: 14,
          quantity: 2,
          unit: '瓶',
          price: 45.00,
          totalAmount: 90.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[2]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        }
      ]
    },
    ENC004: {
      slipId: 'SLIP004',
      encounterId: 'ENC004',
      patientId: 'PAT004',
      patientName: '刘芳',
      patientNo: 'PA2024004',
      visitNo: 'VIS20241201004',
      doctorName: '孙医生',
      departmentName: '妇科',
      createTime: formatDateTime(new Date(now.getTime() - 7200000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM007',
          slipId: 'SLIP004',
          drugId: 'DRUG007',
          itemName: '妇科千金片',
          spec: '0.32g*108片',
          dosage: '6片',
          route: '口服',
          frequency: '每日3次',
          days: 7,
          quantity: 1,
          unit: '盒',
          price: 38.00,
          totalAmount: 38.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        },
        {
          itemId: 'ITEM008',
          slipId: 'SLIP004',
          drugId: 'DRUG008',
          itemName: '益母草颗粒',
          spec: '15g*10袋',
          dosage: '1袋',
          route: '口服',
          frequency: '每日2次',
          days: 10,
          quantity: 2,
          unit: '盒',
          price: 25.00,
          totalAmount: 50.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[2]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        }
      ]
    },
    ENC005: {
      slipId: 'SLIP005',
      encounterId: 'ENC005',
      patientId: 'PAT005',
      patientName: '陈磊',
      patientNo: 'PA2024005',
      visitNo: 'VIS20241201005',
      doctorName: '周医生',
      departmentName: '急诊科',
      createTime: formatDateTime(new Date(now.getTime() - 900000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM009',
          slipId: 'SLIP005',
          drugId: 'DRUG009',
          itemName: '对乙酰氨基酚片',
          spec: '0.5g*20片',
          dosage: '0.5g',
          route: '口服',
          frequency: '每日3次',
          days: 3,
          quantity: 1,
          unit: '盒',
          price: 12.00,
          totalAmount: 12.00,
          status: 'pending',
          inventoryLots: [mockInventoryLots[3]!],
          selectedInventoryId: '',
          selectedLotNo: '',
          pickedQty: 0
        }
      ]
    },
    ENC006: {
      slipId: 'SLIP006',
      encounterId: 'ENC006',
      patientId: 'PAT006',
      patientName: '赵敏',
      patientNo: 'PA2024006',
      visitNo: 'VIS20241201006',
      doctorName: '吴医生',
      departmentName: '骨科',
      createTime: formatDateTime(new Date(now.getTime() - 10800000)),
      status: 'completed',
      items: [
        {
          itemId: 'ITEM010',
          slipId: 'SLIP006',
          drugId: 'DRUG010',
          itemName: '骨痛贴膏',
          spec: '7cm*10cm*5贴',
          dosage: '1贴',
          route: '外用',
          frequency: '每日1次',
          days: 5,
          quantity: 2,
          unit: '盒',
          price: 42.00,
          totalAmount: 84.00,
          status: 'completed',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: 'INV001',
          selectedLotNo: 'LOT20240101',
          pickedQty: 2
        }
      ]
    },
    ENC007: {
      slipId: 'SLIP007',
      encounterId: 'ENC007',
      patientId: 'PAT007',
      patientName: '孙伟',
      patientNo: 'PA2024007',
      visitNo: 'VIS20241201007',
      doctorName: '郑医生',
      departmentName: '消化内科',
      createTime: formatDateTime(new Date(now.getTime() - 12600000)),
      status: 'completed',
      items: [
        {
          itemId: 'ITEM011',
          slipId: 'SLIP007',
          drugId: 'DRUG011',
          itemName: '奥美拉唑肠溶胶囊',
          spec: '20mg*14粒',
          dosage: '20mg',
          route: '口服',
          frequency: '每日2次',
          days: 7,
          quantity: 1,
          unit: '盒',
          price: 30.00,
          totalAmount: 30.00,
          status: 'completed',
          inventoryLots: [mockInventoryLots[3]!],
          selectedInventoryId: 'INV004',
          selectedLotNo: 'LOT20240301',
          pickedQty: 1
        }
      ]
    },
    ENC008: {
      slipId: 'SLIP008',
      encounterId: 'ENC008',
      patientId: 'PAT008',
      patientName: '周静',
      patientNo: 'PA2024008',
      visitNo: 'VIS20241201008',
      doctorName: '黄医生',
      departmentName: '眼科',
      createTime: formatDateTime(new Date(now.getTime() - 14400000)),
      status: 'completed',
      items: [
        {
          itemId: 'ITEM012',
          slipId: 'SLIP008',
          drugId: 'DRUG012',
          itemName: '左氧氟沙星滴眼液',
          spec: '5ml:15mg',
          dosage: '1滴',
          route: '滴眼',
          frequency: '每日4次',
          days: 7,
          quantity: 1,
          unit: '瓶',
          price: 28.00,
          totalAmount: 28.00,
          status: 'completed',
          inventoryLots: [mockInventoryLots[0]!, mockInventoryLots[1]!],
          selectedInventoryId: 'INV001',
          selectedLotNo: 'LOT20240101',
          pickedQty: 1
        },
        {
          itemId: 'ITEM013',
          slipId: 'SLIP008',
          drugId: 'DRUG013',
          itemName: '珍珠明目滴眼液',
          spec: '8ml',
          dosage: '1滴',
          route: '滴眼',
          frequency: '每日3次',
          days: 7,
          quantity: 1,
          unit: '瓶',
          price: 15.00,
          totalAmount: 15.00,
          status: 'completed',
          inventoryLots: [mockInventoryLots[2]!],
          selectedInventoryId: 'INV003',
          selectedLotNo: 'LOT20241001',
          pickedQty: 1
        }
      ]
    }
  }

  return slipMap[encounterId] ? [slipMap[encounterId]!] : []
}

export function mockPageDispenseQueue(params: PageDispenseQueueParams): PageDispenseQueueResult {
  const filtered = mockPatients.filter((p) => {
    if (params.keyword) {
      const kw = params.keyword.toLowerCase()
      return (
        (p as unknown as { name?: string }).name?.toLowerCase().includes(kw) ||
        p.patientNo.toLowerCase().includes(kw) ||
        p.visitNo.toLowerCase().includes(kw)
      )
    }
    return true
  })

  const total = filtered.length
  const pageSize = params.pageSize || 10
  const pageNum = params.pageNum || 1
  const start = (pageNum - 1) * pageSize
  const end = start + pageSize

  return {
    total,
    pageNum,
    pageSize,
    records: filtered.slice(start, end) as unknown as PageDispenseQueueResult['records']
  }
}

export function mockListDispenseSlips(encounterId: string): DispenseSlipDto[] {
  return buildSlipsForEncounter(encounterId)
}

export function mockConfirmMedicationDispense() {
  return {
    success: true,
    dispenseNo: `DISP${Date.now()}`,
    dispenseTime: formatDateTime(now),
    dispensedCount: 0
  }
}

export function mockGetDispenserList(): PharmacyDispenserDto[] {
  return mockDispenserList
}

export function mockListInventoryLots(): InventoryLotDto[] {
  return mockInventoryLots
}

export function mockConfirmTraceCode(): TraceCodeConfirmResult {
  return {
    success: true,
    drugName: '阿莫西林胶囊',
    lotNo: 'LOT20240101',
    expiryDate: '2026-12-31'
  }
}

export function mockPrintDispenseSlip() {
  return { success: true, printNo: `PRINT${Date.now()}` }
}

export function mockSavePreparation() {
  return { success: true }
}

export function mockConfirmDispense() {
  return { message: '发药成功' }
}

export function mockLoadDispenserOptions(_keyword: string) {
  return mockDispenserList.map((d) => ({
    practitionerId: d.dispenserId,
    displayName: d.name
  }))
}

export function mockLoadPharmacyOptions(_keyword: string) {
  const pharmacies = [
    { pharmacyId: 'PHAR001', pharmacyName: '西药房' },
    { pharmacyId: 'PHAR002', pharmacyName: '中药房' },
    { pharmacyId: 'PHAR003', pharmacyName: '急诊药房' }
  ]
  return pharmacies
}

export function mockPagePatientQueue(params: PageDispenseQueueParams): PagePatientQueueResult {
  const filtered = mockPatients.filter((p) => {
    if (params.status) {
      const statusMap: Record<string, string> = {
        pending: '待发药',
        completed: '已发药'
      }
      const targetStatus = statusMap[params.status] || params.status
      if (p.status !== targetStatus) return false
    }
    if (params.keyword) {
      const kw = params.keyword.toLowerCase()
      return (
        p.name.toLowerCase().includes(kw) ||
        p.patientNo.toLowerCase().includes(kw) ||
        p.visitNo.toLowerCase().includes(kw)
      )
    }
    return true
  })

  const total = filtered.length
  const pageSize = params.pageSize || 10
  const pageNum = params.pageNum || 1
  const start = (pageNum - 1) * pageSize
  const end = start + pageSize

  return {
    total,
    pageNum,
    pageSize,
    records: filtered.slice(start, end)
  }
}
