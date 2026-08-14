import type {
  InpatientReturnMedicineInitDto,
  PageInpatientReturnPatientQueueParams,
  PageInpatientReturnPatientQueueResult,
  InpatientReturnMedicineDetailDto,
  InpatientReturnMedicineResultDto,
  ConfirmInpatientMedicineReturnData,
  RejectInpatientMedicineReturnData
} from './type'

export const mockInitData: InpatientReturnMedicineInitDto = {
  insuranceContractOptions: [
    { label: '市医保（H220104024...）', value: 'H220104024' }
  ],
  wardOptions: [
    { label: '内科病区', value: 'ward-1' },
    { label: '外科病区', value: 'ward-2' },
    { label: '心内科病区', value: 'ward-3' },
    { label: '妇产科病区', value: 'ward-4' }
  ]
}

export const mockPatientQueueData: PageInpatientReturnPatientQueueResult = {
  records: [
    {
      encounterId: 'INP-RETURN-001',
      patientName: '张三',
      gender: '男',
      age: '45',
      serviceType: '住院',
      patientNo: 'P20260001',
      visitNo: 'V20260726001',
      visitTime: '2026-07-26 09:00:00',
      departmentName: '内科',
      status: 'pending',
      paymentType: '医保',
      bedNo: '101-01'
    },
    {
      encounterId: 'INP-RETURN-002',
      patientName: '李四',
      gender: '女',
      age: '32',
      serviceType: '住院',
      patientNo: 'P20260002',
      visitNo: 'V20260726002',
      visitTime: '2026-07-26 08:30:00',
      departmentName: '外科',
      status: 'pending',
      paymentType: '自费',
      bedNo: '201-03'
    },
    {
      encounterId: 'INP-RETURN-003',
      patientName: '王五',
      gender: '男',
      age: '58',
      serviceType: '住院',
      patientNo: 'P20260003',
      visitNo: 'V20260726003',
      visitTime: '2026-07-26 10:00:00',
      departmentName: '心内科',
      status: 'completed',
      paymentType: '医保',
      bedNo: '301-02'
    },
    {
      encounterId: 'INP-RETURN-004',
      patientName: '赵六',
      gender: '女',
      age: '28',
      serviceType: '住院',
      patientNo: 'P20260004',
      visitNo: 'V20260726004',
      visitTime: '2026-07-26 11:00:00',
      departmentName: '妇产科',
      status: 'pending',
      paymentType: '医保',
      bedNo: '401-05'
    }
  ],
  total: 4,
  pageNum: 1,
  pageSize: 10
}

export const mockReturnDetailData: Record<string, InpatientReturnMedicineDetailDto> = {
  'INP-RETURN-001': {
    encounterId: 'INP-RETURN-001',
    patientName: '张三',
    patientNo: 'P20260001',
    bedNo: '101-01',
    items: [
      {
        itemId: 'INP-RETURN-ITEM-001',
        itemName: '头孢克肟胶囊',
        spec: '0.1g*6粒',
        quantity: 2,
        unit: '盒',
        batchNo: 'LOT20240101',
        totalPrice: 45.00,
        pharmacy: '西药房-A库',
        traceCode: '-',
        factory: '广州白云山制药',
        status: 'refundable',
        refundQuantity: 0,
        originalQuantity: 2
      },
      {
        itemId: 'INP-RETURN-ITEM-002',
        itemName: '氯化钠注射液',
        spec: '250ml:2.25g',
        quantity: 3,
        unit: '瓶',
        batchNo: 'LOT20240201',
        totalPrice: 15.00,
        pharmacy: '西药房-A库',
        traceCode: '-',
        factory: '四川科伦药业',
        status: 'partially-refundable',
        refundQuantity: 1,
        originalQuantity: 3
      }
    ]
  },
  'INP-RETURN-002': {
    encounterId: 'INP-RETURN-002',
    patientName: '李四',
    patientNo: 'P20260002',
    bedNo: '201-03',
    items: [
      {
        itemId: 'INP-RETURN-ITEM-003',
        itemName: '布洛芬缓释胶囊',
        spec: '0.3g*20粒',
        quantity: 1,
        unit: '盒',
        batchNo: 'LOT20240401',
        totalPrice: 18.00,
        pharmacy: '西药房-A库',
        traceCode: '-',
        factory: '中美史克',
        status: 'non-refundable',
        refundQuantity: 0,
        originalQuantity: 1
      }
    ]
  },
  'INP-RETURN-003': {
    encounterId: 'INP-RETURN-003',
    patientName: '王五',
    patientNo: 'P20260003',
    bedNo: '301-02',
    items: [
      {
        itemId: 'INP-RETURN-ITEM-004',
        itemName: '阿莫西林胶囊',
        spec: '0.5g*20粒',
        quantity: 2,
        unit: '盒',
        batchNo: 'LOT20240501',
        totalPrice: 25.00,
        pharmacy: '西药房-A库',
        traceCode: 'TRAC001',
        factory: '珠海联邦制药',
        status: 'refunded',
        refundQuantity: 2,
        originalQuantity: 2
      }
    ]
  },
  'INP-RETURN-004': {
    encounterId: 'INP-RETURN-004',
    patientName: '赵六',
    patientNo: 'P20260004',
    bedNo: '401-05',
    items: [
      {
        itemId: 'INP-RETURN-ITEM-005',
        itemName: '维生素C片',
        spec: '100mg*100片',
        quantity: 1,
        unit: '瓶',
        batchNo: 'LOT20240601',
        totalPrice: 8.00,
        pharmacy: '西药房-A库',
        traceCode: '-',
        factory: '华北制药',
        status: 'refundable',
        refundQuantity: 0,
        originalQuantity: 1
      },
      {
        itemId: 'INP-RETURN-ITEM-006',
        itemName: '葡萄糖注射液',
        spec: '500ml:25g',
        quantity: 2,
        unit: '瓶',
        batchNo: 'LOT20240701',
        totalPrice: 12.00,
        pharmacy: '西药房-A库',
        traceCode: '-',
        factory: '四川科伦药业',
        status: 'refundable',
        refundQuantity: 0,
        originalQuantity: 2
      }
    ]
  }
}

export async function mockLoadInpatientReturnMedicineInit(): Promise<InpatientReturnMedicineInitDto> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockInitData
}

export async function mockPageInpatientReturnPatientQueue(params: PageInpatientReturnPatientQueueParams): Promise<PageInpatientReturnPatientQueueResult> {
  await new Promise(resolve => setTimeout(resolve, 300))
  let records = [...mockPatientQueueData.records]
  if (params.status) {
    records = records.filter(r => r.status === params.status)
  }
  if (params.keyword) {
    const kw = params.keyword.toLowerCase()
    records = records.filter(r =>
      r.patientName.toLowerCase().includes(kw) ||
      r.patientNo.toLowerCase().includes(kw) ||
      r.visitNo.toLowerCase().includes(kw)
    )
  }
  return {
    records,
    total: records.length,
    pageNum: params.pageNum || 1,
    pageSize: params.pageSize || 10
  }
}

export async function mockGetInpatientReturnMedicineDetail(encounterId: string): Promise<InpatientReturnMedicineDetailDto | undefined> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockReturnDetailData[encounterId]
}

export async function mockConfirmInpatientMedicineReturn(
  data: ConfirmInpatientMedicineReturnData
): Promise<InpatientReturnMedicineResultDto> {
  await new Promise(resolve => setTimeout(resolve, 500))

  const detail = mockReturnDetailData[data.summaryNo]
  if (detail) {
    for (const item of data.items) {
      const found = detail.items.find(i => i.itemId === item.dispenseId)
      if (found) {
        found.status = 'refunded'
        found.refundQuantity = item.dispenseQuantity
      }
    }

    const allRefunded = detail.items.every(i => i.status === 'refunded')
    const patient = mockPatientQueueData.records.find(p => p.encounterId === data.summaryNo)
    if (patient && allRefunded) {
      patient.status = 'completed'
    }
  }

  const results = data.items.map(item => ({
    itemId: item.dispenseId,
    success: true,
    message: '成功'
  }))

  return {
    success: true,
    message: '退药成功',
    results
  }
}

export async function mockRejectInpatientMedicineReturn(
  data: RejectInpatientMedicineReturnData
): Promise<InpatientReturnMedicineResultDto> {
  await new Promise(resolve => setTimeout(resolve, 300))

  const detail = mockReturnDetailData[data.returnId]
  if (detail && data.itemIds) {
    for (const itemId of data.itemIds) {
      const found = detail.items.find(i => i.itemId === itemId)
      if (found) {
        found.status = 'refundable'
        found.refundQuantity = 0
      }
    }
  }

  return {
    success: true,
    message: '驳回成功',
    results: []
  }
}
