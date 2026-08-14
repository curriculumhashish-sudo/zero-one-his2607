import dayjs from 'dayjs'

import type {
  DoctorPrescriptionScope,
  DoctorPrescriptionSheetQuery,
  DoctorPrescriptionSheetVO
} from './type'

const today = dayjs().format('YYYY-MM-DD')

const currentPrescriptionSheets: Record<string, DoctorPrescriptionSheetVO[]> = {
  'MR-DEMO-0001': [
    {
      patientName: '全功能演示患者',
      age: 52,
      gender: '男',
      department: '心血管内科',
      patientId: 'OP-DEMO-0001',
      prescriptionNo: 'RX-enc-demo-in-progress-3',
      costType: '职工医保',
      date: `${today} 08:15`,
      doctor: '李尔王',
      diagnosis: '原发性高血压；高脂血症',
      totalPrice: 18.6,
      pharmacist: '待调配',
      dispenser: '待发药',
      priceChecker: '系统划价',
      items: '1. 阿司匹林肠溶片　100mg×30片\n　用法：100mg　口服　每日一次　共7天\n　数量：7片'
    }
  ],
  MR20260008: [
    {
      patientName: '吴国平',
      age: 53,
      gender: '男',
      department: '心血管内科',
      patientId: 'OP202607230008',
      prescriptionNo: 'RX-enc-20260723-008-1',
      costType: '职工医保',
      date: `${today} 10:30`,
      doctor: '李尔王',
      diagnosis: '高血压',
      totalPrice: 18.6,
      pharmacist: '待调配',
      dispenser: '待发药',
      priceChecker: '系统划价',
      items: '1. 阿司匹林肠溶片　100mg×30片\n　用法：100mg　口服　每日一次　共7天\n　数量：7片'
    }
  ],
  MR20260009: [
    {
      patientName: '郑秀兰',
      age: 64,
      gender: '女',
      department: '心血管内科',
      patientId: 'OP202607230009',
      prescriptionNo: 'RX-enc-20260723-009-1',
      costType: '居民医保',
      date: `${today} 10:30`,
      doctor: '李尔王',
      diagnosis: '冠状动脉粥样硬化性心脏病',
      totalPrice: 35.2,
      pharmacist: '王药师',
      dispenser: '待发药',
      priceChecker: '系统划价',
      items: '1. 阿托伐他汀钙片　20mg×7片\n　用法：20mg　口服　每晚一次　共7天\n　数量：7片'
    }
  ]
}

const historicalPrescriptionSheets: Record<string, DoctorPrescriptionSheetVO[]> = {
  'MR-DEMO-0001': [
    {
      patientName: '全功能演示患者',
      age: 52,
      gender: '男',
      department: '心血管内科',
      patientId: 'HOP-DEMO-001',
      prescriptionNo: 'RX-HISTORY-DEMO-001',
      costType: '职工医保',
      date: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:35'),
      doctor: '周医生',
      diagnosis: '原发性高血压',
      totalPrice: 72.4,
      pharmacist: '赵药师',
      dispenser: '钱药师',
      priceChecker: '系统划价',
      items:
        '1. 苯磺酸氨氯地平片　5mg×14片\n　用法：5mg　口服　每日一次　共7天\n　数量：7片\n\n2. 酒石酸美托洛尔片　25mg×20片\n　用法：25mg　口服　每日两次　共7天\n　数量：14片'
    },
    {
      patientName: '全功能演示患者',
      age: 52,
      gender: '男',
      department: '全科医学科',
      patientId: 'HOP-DEMO-002',
      prescriptionNo: 'RX-HISTORY-DEMO-002',
      costType: '职工医保',
      date: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:30'),
      doctor: '陈医生',
      diagnosis: '心悸',
      totalPrice: 35.2,
      pharmacist: '赵药师',
      dispenser: '钱药师',
      priceChecker: '系统划价',
      items: '1. 阿托伐他汀钙片　20mg×7片\n　用法：20mg　口服　每晚一次　共7天\n　数量：7片'
    }
  ],
  MR20260003: [
    {
      patientName: '王志强',
      age: 67,
      gender: '男',
      department: '心血管内科',
      patientId: 'HOP-1003-001',
      prescriptionNo: 'RX-HISTORY-1003-001',
      costType: '自费',
      date: dayjs().subtract(1, 'month').format('YYYY-MM-DD 09:30'),
      doctor: '周医生',
      diagnosis: '原发性高血压',
      totalPrice: 53.8,
      pharmacist: '赵药师',
      dispenser: '钱药师',
      priceChecker: '系统划价',
      items:
        '1. 苯磺酸氨氯地平片　5mg×14片\n　用法：5mg　口服　每日一次　共7天\n　数量：7片\n\n2. 酒石酸美托洛尔片　25mg×20片\n　用法：25mg　口服　每日两次　共7天\n　数量：14片'
    },
    {
      patientName: '王志强',
      age: 67,
      gender: '男',
      department: '全科医学科',
      patientId: 'HOP-1003-002',
      prescriptionNo: 'RX-HISTORY-1003-002',
      costType: '自费',
      date: dayjs().subtract(3, 'month').format('YYYY-MM-DD 10:40'),
      doctor: '陈医生',
      diagnosis: '高脂血症',
      totalPrice: 35.2,
      pharmacist: '赵药师',
      dispenser: '钱药师',
      priceChecker: '系统划价',
      items: '1. 阿托伐他汀钙片　20mg×7片\n　用法：20mg　口服　每晚一次　共7天\n　数量：7片'
    }
  ],
  MR20260008: [
    {
      patientName: '吴国平',
      age: 53,
      gender: '男',
      department: '心血管内科',
      patientId: 'HOP-1008-001',
      prescriptionNo: 'RX-HISTORY-1008-001',
      costType: '职工医保',
      date: dayjs().subtract(2, 'month').format('YYYY-MM-DD 08:50'),
      doctor: '周医生',
      diagnosis: '原发性高血压',
      totalPrice: 18.6,
      pharmacist: '赵药师',
      dispenser: '钱药师',
      priceChecker: '系统划价',
      items: '1. 阿司匹林肠溶片　100mg×30片\n　用法：100mg　口服　每日一次　共7天\n　数量：7片'
    }
  ]
}

function delay(ms = 160) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function normalizeScope(scope?: DoctorPrescriptionScope) {
  return scope ?? 'CURRENT_ENCOUNTER'
}

/** 模拟 GET /cyclectrl/prescription_query。 */
export async function mockQueryDoctorPrescriptionSheets(
  query: DoctorPrescriptionSheetQuery
): Promise<DoctorPrescriptionSheetVO[]> {
  await delay()
  const patientNo = query.num?.trim()
  if (!patientNo) throw new Error('缺少患者病历号')

  const scope = normalizeScope(query.type)
  if (!['CURRENT_ENCOUNTER', 'HISTORY'].includes(scope)) {
    throw new Error('处方单查询范围无效')
  }

  const source = scope === 'HISTORY' ? historicalPrescriptionSheets : currentPrescriptionSheets
  return (source[patientNo] ?? []).map((sheet) => ({ ...sheet }))
}
