import type {
  InpatientDispenseInitDto,
  PageInpatientDispenseQueueParams,
  PageInpatientDispenseQueueResult,
  InpatientMedicationDispenseSlipDto,
  InpatientDispenseResultDto,
  PageSummarySlipParams,
  PageSummarySlipResult,
  MedicationSummarySlipDto,
  SummaryDispenseResultDto
} from './type'

export const mockInitData: InpatientDispenseInitDto = {
  dispenserOptions: [
    { label: '张药师', value: 'dispenser-001' },
    { label: '李药师', value: 'dispenser-002' },
    { label: '王药师', value: 'dispenser-003' }
  ],
  receiverOptions: [
    { label: '护士甲', value: 'receiver-001' },
    { label: '护士乙', value: 'receiver-002' },
    { label: '护士丙', value: 'receiver-003' }
  ]
}

export const mockPatientQueueData: PageInpatientDispenseQueueResult = {
  records: [
    {
      encounterId: 'INPATIENT-001',
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
      encounterId: 'INPATIENT-002',
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
      encounterId: 'INPATIENT-003',
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
      encounterId: 'INPATIENT-004',
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

export const mockDispenseSlipData: Record<string, InpatientMedicationDispenseSlipDto[]> = {
  'INPATIENT-001': [
    {
      slipId: 'SLIP-001',
      rxNo: 'CP2026-07-INP001',
      rxType: 'western',
      status: 'pending',
      items: [
        {
          dispenseId: 'ITEM-001',
          itemName: '头孢克肟胶囊',
          spec: '0.1g*6粒',
          quantity: 2,
          unit: '盒',
          batchNo: 'LOT20240101',
          totalPrice: 45.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '广州白云山制药',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        },
        {
          dispenseId: 'ITEM-002',
          itemName: '氯化钠注射液',
          spec: '250ml:2.25g',
          quantity: 3,
          unit: '瓶',
          batchNo: 'LOT20240201',
          totalPrice: 15.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '四川科伦药业',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    },
    {
      slipId: 'SLIP-002',
      rxNo: 'CP2026-07-INP002',
      rxType: 'herbal',
      status: 'pending',
      items: [
        {
          dispenseId: 'ITEM-003',
          itemName: '丹参滴丸',
          spec: '27mg*150丸',
          quantity: 1,
          unit: '盒',
          batchNo: 'LOT20240301',
          totalPrice: 35.00,
          pharmacy: '中药房',
          traceCode: '-',
          factory: '天津天士力',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    }
  ],
  'INPATIENT-002': [
    {
      slipId: 'SLIP-003',
      rxNo: 'CP2026-07-INP003',
      rxType: 'western',
      status: 'pending',
      items: [
        {
          dispenseId: 'ITEM-004',
          itemName: '布洛芬缓释胶囊',
          spec: '0.3g*20粒',
          quantity: 1,
          unit: '盒',
          batchNo: 'LOT20240401',
          totalPrice: 18.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '中美史克',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    }
  ],
  'INPATIENT-003': [
    {
      slipId: 'SLIP-004',
      rxNo: 'CP2026-07-INP004',
      rxType: 'western',
      status: 'completed',
      items: [
        {
          dispenseId: 'ITEM-005',
          itemName: '阿莫西林胶囊',
          spec: '0.5g*20粒',
          quantity: 2,
          unit: '盒',
          batchNo: 'LOT20240501',
          totalPrice: 25.00,
          pharmacy: '西药房-A库',
          traceCode: 'TRAC001',
          factory: '珠海联邦制药',
          sourceStatus: 'COMPLETED',
          status: 'completed'
        }
      ]
    }
  ],
  'INPATIENT-004': [
    {
      slipId: 'SLIP-005',
      rxNo: 'CP2026-07-INP005',
      rxType: 'western',
      status: 'pending',
      items: [
        {
          dispenseId: 'ITEM-006',
          itemName: '维生素C片',
          spec: '100mg*100片',
          quantity: 1,
          unit: '瓶',
          batchNo: 'LOT20240601',
          totalPrice: 8.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '华北制药',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        },
        {
          dispenseId: 'ITEM-007',
          itemName: '葡萄糖注射液',
          spec: '500ml:25g',
          quantity: 2,
          unit: '瓶',
          batchNo: 'LOT20240701',
          totalPrice: 12.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '四川科伦药业',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    }
  ]
}

export const mockSummarySlipData: PageSummarySlipResult = {
  records: [
    {
      summaryId: 'SUMMARY-001',
      summaryNo: 'HS2026-07-001',
      status: 'pending',
      receiverName: '护士甲',
      receiverId: 'receiver-001',
      createTime: '2026-07-26 09:00:00',
      items: [
        {
          dispenseId: 'SUM-ITEM-001',
          itemName: '头孢克肟胶囊',
          spec: '0.1g*6粒',
          quantity: 5,
          unit: '盒',
          batchNo: 'LOT20240101',
          totalPrice: 112.50,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '广州白云山制药',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        },
        {
          dispenseId: 'SUM-ITEM-002',
          itemName: '氯化钠注射液',
          spec: '250ml:2.25g',
          quantity: 10,
          unit: '瓶',
          batchNo: 'LOT20240201',
          totalPrice: 50.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '四川科伦药业',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    },
    {
      summaryId: 'SUMMARY-002',
      summaryNo: 'HS2026-07-002',
      status: 'pending',
      receiverName: '护士乙',
      receiverId: 'receiver-002',
      createTime: '2026-07-26 10:00:00',
      items: [
        {
          dispenseId: 'SUM-ITEM-003',
          itemName: '阿莫西林胶囊',
          spec: '0.5g*20粒',
          quantity: 8,
          unit: '盒',
          batchNo: 'LOT20240501',
          totalPrice: 100.00,
          pharmacy: '西药房-A库',
          traceCode: '-',
          factory: '珠海联邦制药',
          sourceStatus: 'COMPLETED',
          status: 'pending'
        }
      ]
    },
    {
      summaryId: 'SUMMARY-003',
      summaryNo: 'HS2026-07-003',
      status: 'completed',
      receiverName: '护士丙',
      receiverId: 'receiver-003',
      createTime: '2026-07-26 08:00:00',
      items: [
        {
          dispenseId: 'SUM-ITEM-004',
          itemName: '布洛芬缓释胶囊',
          spec: '0.3g*20粒',
          quantity: 3,
          unit: '盒',
          batchNo: 'LOT20240401',
          totalPrice: 54.00,
          pharmacy: '西药房-A库',
          traceCode: 'TRAC002',
          factory: '中美史克',
          sourceStatus: 'COMPLETED',
          status: 'completed'
        }
      ]
    }
  ],
  total: 3,
  pageNum: 1,
  pageSize: 10
}

export async function mockLoadInpatientDispenseInit(): Promise<InpatientDispenseInitDto> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockInitData
}

export async function mockPageInpatientDispenseQueue(params: PageInpatientDispenseQueueParams): Promise<PageInpatientDispenseQueueResult> {
  await new Promise(resolve => setTimeout(resolve, 300))
  let records = [...mockPatientQueueData.records]
  if (params.status) {
    const targetStatus = params.status === 'pending' ? 'pending' : 'completed'
    records = records.filter(r => r.status === targetStatus)
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

export async function mockPageInpatientMedicationDispenseSlips(encounterId: string): Promise<InpatientMedicationDispenseSlipDto[]> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockDispenseSlipData[encounterId] || []
}

export async function mockSaveInpatientDispensePreparation(): Promise<void> {
  await new Promise(resolve => setTimeout(resolve, 300))
}

export async function mockDispenseInpatientMedications(): Promise<InpatientDispenseResultDto> {
  await new Promise(resolve => setTimeout(resolve, 500))
  return {
    success: true,
    message: '发药成功',
    results: [
      { dispenseId: 'ITEM-001', success: true, message: '成功' },
      { dispenseId: 'ITEM-002', success: true, message: '成功' }
    ]
  }
}

export async function mockPagePendingMedicationSummarySlips(params: PageSummarySlipParams): Promise<PageSummarySlipResult> {
  await new Promise(resolve => setTimeout(resolve, 300))
  let records = [...mockSummarySlipData.records]
  if (params.status) {
    records = records.filter(r => r.status === params.status)
  }
  if (params.keyword) {
    const kw = params.keyword.toLowerCase()
    records = records.filter(r =>
      r.summaryNo.toLowerCase().includes(kw) ||
      r.receiverName.toLowerCase().includes(kw)
    )
  }
  return {
    records,
    total: records.length,
    pageNum: params.pageNum || 1,
    pageSize: params.pageSize || 10
  }
}

export async function mockGetMedicationSummaryDispenseDetail(summaryId: string): Promise<MedicationSummarySlipDto | undefined> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockSummarySlipData.records.find(r => r.summaryId === summaryId)
}

export async function mockDispenseSummaryMedications(): Promise<SummaryDispenseResultDto> {
  await new Promise(resolve => setTimeout(resolve, 500))
  return { success: true, message: '汇总发药成功' }
}

export async function mockRejectSummaryDispense(): Promise<SummaryDispenseResultDto> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return { success: true, message: '驳回成功' }
}
