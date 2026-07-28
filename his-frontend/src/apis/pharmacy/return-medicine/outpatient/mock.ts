import type {
  OutpatientReturnMedicinePatientQueueDTO,
  OutpatientReturnMedicineSlipDTO,
  OutpatientReturnMedicineInitDTO,
  PageOutpatientReturnMedicinePatientQueueParams,
  PageOutpatientReturnMedicinePatientQueueResult,
  OutpatientReturnMedicineRequest,
  OutpatientReturnMedicineResult
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

const mockPatients: OutpatientReturnMedicinePatientQueueDTO[] = [
  {
    id: 'RET001',
    encounterId: 'ENC001',
    patientId: 'PAT001',
    patientName: '张明',
    patientNo: 'PA2024001',
    visitNo: 'VIS20241201001',
    departmentName: '内科',
    doctorName: '李医生',
    status: 'pending',
    statusTone: 'warning',
    visitTime: formatDateTime(new Date(now.getTime() - 1800000)),
    serviceType: '普通门诊',
    paymentType: '医保'
  },
  {
    id: 'RET002',
    encounterId: 'ENC002',
    patientId: 'PAT002',
    patientName: '李华',
    patientNo: 'PA2024002',
    visitNo: 'VIS20241201002',
    departmentName: '皮肤科',
    doctorName: '王医生',
    status: 'pending',
    statusTone: 'warning',
    visitTime: formatDateTime(new Date(now.getTime() - 3600000)),
    serviceType: '普通门诊',
    paymentType: '自费'
  },
  {
    id: 'RET003',
    encounterId: 'ENC003',
    patientId: 'PAT003',
    patientName: '王强',
    patientNo: 'PA2024003',
    visitNo: 'VIS20241201003',
    departmentName: '心血管科',
    doctorName: '赵医生',
    status: 'partial',
    statusTone: 'info',
    visitTime: formatDateTime(new Date(now.getTime() - 5400000)),
    serviceType: '专家门诊',
    paymentType: '医保'
  },
  {
    id: 'RET004',
    encounterId: 'ENC004',
    patientId: 'PAT004',
    patientName: '刘芳',
    patientNo: 'PA2024004',
    visitNo: 'VIS20241201004',
    departmentName: '妇科',
    doctorName: '孙医生',
    status: 'completed',
    statusTone: 'success',
    visitTime: formatDateTime(new Date(now.getTime() - 7200000)),
    serviceType: '普通门诊',
    paymentType: '医保'
  },
  {
    id: 'RET005',
    encounterId: 'ENC005',
    patientId: 'PAT005',
    patientName: '陈明',
    patientNo: 'PA2024005',
    visitNo: 'VIS20241201005',
    departmentName: '眼科',
    doctorName: '周医生',
    status: 'pending',
    statusTone: 'warning',
    visitTime: formatDateTime(new Date(now.getTime() - 9000000)),
    serviceType: '普通门诊',
    paymentType: '自费'
  }
]

const mockSlips: Record<string, OutpatientReturnMedicineSlipDTO[]> = {
  RET001: [
    {
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
          itemName: '布洛芬缓释胶囊',
          spec: '0.3g*20粒',
          dosage: '0.3g',
          route: '口服',
          frequency: '每日2次',
          days: 7,
          quantity: 2,
          unit: '盒',
          price: 28.00,
          totalAmount: 56.00,
          status: 'pending',
          batchNo: 'B20240101',
          traceCode: '',
          factory: '上海制药厂'
        },
        {
          itemId: 'ITEM002',
          slipId: 'SLIP001',
          drugId: 'DRUG002',
          itemName: '氨溴索口服溶液',
          spec: '100ml:300mg',
          dosage: '10ml',
          route: '口服',
          frequency: '每日3次',
          days: 5,
          quantity: 1,
          unit: '瓶',
          price: 35.00,
          totalAmount: 35.00,
          status: 'pending',
          batchNo: 'B20240201',
          traceCode: '',
          factory: '北京制药厂'
        }
      ]
    }
  ],
  RET002: [
    {
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
          batchNo: 'B20240301',
          traceCode: '',
          factory: '广州制药厂'
        }
      ]
    }
  ],
  RET003: [
    {
      slipId: 'SLIP003',
      encounterId: 'ENC003',
      patientId: 'PAT003',
      patientName: '王强',
      patientNo: 'PA2024003',
      visitNo: 'VIS20241201003',
      doctorName: '赵医生',
      departmentName: '心血管科',
      createTime: formatDateTime(new Date(now.getTime() - 5400000)),
      status: 'partial',
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
          status: 'completed',
          batchNo: 'B20240401',
          traceCode: 'TR20240101',
          factory: '天津制药厂',
          reason: '患者过敏'
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
          batchNo: 'B20240501',
          traceCode: '',
          factory: '南京制药厂'
        }
      ]
    }
  ],
  RET004: [
    {
      slipId: 'SLIP004',
      encounterId: 'ENC004',
      patientId: 'PAT004',
      patientName: '刘芳',
      patientNo: 'PA2024004',
      visitNo: 'VIS20241201004',
      doctorName: '孙医生',
      departmentName: '妇科',
      createTime: formatDateTime(new Date(now.getTime() - 7200000)),
      status: 'completed',
      items: [
        {
          itemId: 'ITEM006',
          slipId: 'SLIP004',
          drugId: 'DRUG006',
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
          status: 'completed',
          batchNo: 'B20240601',
          traceCode: 'TR20240201',
          factory: '湖南制药厂',
          reason: '症状缓解，无需继续服用'
        }
      ]
    }
  ],
  RET005: [
    {
      slipId: 'SLIP005',
      encounterId: 'ENC005',
      patientId: 'PAT005',
      patientName: '陈明',
      patientNo: 'PA2024005',
      visitNo: 'VIS20241201005',
      doctorName: '周医生',
      departmentName: '眼科',
      createTime: formatDateTime(new Date(now.getTime() - 9000000)),
      status: 'pending',
      items: [
        {
          itemId: 'ITEM007',
          slipId: 'SLIP005',
          drugId: 'DRUG007',
          itemName: '珍珠明目滴眼液',
          spec: '8ml',
          dosage: '1滴',
          route: '外用',
          frequency: '每日3次',
          days: 14,
          quantity: 2,
          unit: '瓶',
          price: 12.00,
          totalAmount: 24.00,
          status: 'pending',
          batchNo: 'B20240701',
          traceCode: '',
          factory: '杭州制药厂'
        }
      ]
    }
  ]
}

export async function mockLoadOutpatientReturnMedicineInit(): Promise<OutpatientReturnMedicineInitDTO> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return {
    dispenserOptions: [
      { label: '张药师', value: 'DISP001' },
      { label: '李药师', value: 'DISP002' },
      { label: '王药师', value: 'DISP003' }
    ]
  }
}

export async function mockPageOutpatientReturnMedicinePatientQueue(
  params: PageOutpatientReturnMedicinePatientQueueParams
): Promise<PageOutpatientReturnMedicinePatientQueueResult> {
  await new Promise(resolve => setTimeout(resolve, 300))
  
  let filtered = [...mockPatients]
  
  if (params.status && params.status !== 'all') {
    filtered = filtered.filter(p => p.status === params.status)
  }
  
  if (params.keyword) {
    const keyword = params.keyword.toLowerCase()
    filtered = filtered.filter(p =>
      p.patientName.toLowerCase().includes(keyword) ||
      p.patientNo.toLowerCase().includes(keyword) ||
      p.visitNo.toLowerCase().includes(keyword)
    )
  }
  
  return {
    records: filtered,
    total: filtered.length,
    pageIndex: 1,
    pageSize: 50
  }
}

export async function mockListOutpatientReturnMedicineSlips(
  encounterId: string
): Promise<OutpatientReturnMedicineSlipDTO[]> {
  await new Promise(resolve => setTimeout(resolve, 300))
  return mockSlips[encounterId] || []
}

export async function mockReturnOutpatientMedicine(
  request: OutpatientReturnMedicineRequest
): Promise<OutpatientReturnMedicineResult> {
  await new Promise(resolve => setTimeout(resolve, 500))
  return {
    success: true,
    message: '退药成功'
  }
}