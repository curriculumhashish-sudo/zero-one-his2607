// src/apis/pharmacy/outpatient-dispense/mock.ts

import type {
  PageDispenseQueueParams,
  PendingDispenseEncounterQueueRowDto,
  EncounterMedicationDispenseSlipRowDto,
  SavePreparationParams,
  ConfirmDispenseParams,
  PractitionerOptionDto,
  PharmacyOptionDto,
} from './type'

/** ===== 模拟患者队列 ===== */
const mockQueueData: PendingDispenseEncounterQueueRowDto[] = [
  {
    encounterId: 'enc-001',
    patientId: 'P001',
    patientName: '张三',
    genderText: '男',
    ageText: '35岁',
    visitNo: 'MZ20260724001',
    prescriptionCount: 2,
    registerTime: '2026-07-24 09:00:00',
    dispenseStatus: 'PREPARATION',
  },
  {
    encounterId: 'enc-002',
    patientId: 'P002',
    patientName: '李四',
    genderText: '女',
    ageText: '28岁',
    visitNo: 'MZ20260724002',
    prescriptionCount: 1,
    registerTime: '2026-07-24 09:30:00',
    dispenseStatus: 'IN_PROGRESS',
  },
  {
    encounterId: 'enc-003',
    patientId: 'P003',
    patientName: '王五',
    genderText: '男',
    ageText: '42岁',
    visitNo: 'MZ20260724003',
    prescriptionCount: 3,
    registerTime: '2026-07-24 10:00:00',
    dispenseStatus: 'PREPARATION',
  },
]

/** ===== 模拟处方明细 ===== */
const mockSlipData: Record<string, EncounterMedicationDispenseSlipRowDto[]> = {
  'enc-001': [
    {
      dispenseId: 'disp-001-1',
      prescriptionNo: 'RX20260724001',
      medicationName: '阿莫西林胶囊',
      specText: '0.5g*20粒',
      quantity: 20,
      unitName: '粒',
      dosageText: '0.5g',
      frequencyText: 'tid',
      routeText: '口服',
      days: 3,
      totalPrice: 25.60,
      statusEnum: 'PREPARATION',
      pharmacyName: '门诊药房',
      lotNumber: '20260501',
      traceNo: '',
      factoryName: 'XX制药厂',
      inventoryDetailList: [
        {
          pharmacyOrganizationId: 'ph-001',
          pharmacyName: '门诊药房',
          lotNumber: '20260501',
          availableQuantity: 100,
        },
        {
          pharmacyOrganizationId: 'ph-002',
          pharmacyName: '住院药房',
          lotNumber: '20260510',
          availableQuantity: 50,
        },
      ],
    },
    {
      dispenseId: 'disp-001-2',
      prescriptionNo: 'RX20260724001',
      medicationName: '头孢克肟片',
      specText: '0.1g*12粒',
      quantity: 12,
      unitName: '粒',
      dosageText: '0.1g',
      frequencyText: 'bid',
      routeText: '口服',
      days: 3,
      totalPrice: 32.80,
      statusEnum: 'PREPARATION',
      pharmacyName: '门诊药房',
      lotNumber: '20260515',
      traceNo: '',
      factoryName: 'YY药业',
      inventoryDetailList: [
        {
          pharmacyOrganizationId: 'ph-001',
          pharmacyName: '门诊药房',
          lotNumber: '20260515',
          availableQuantity: 80,
        },
      ],
    },
  ],
  'enc-002': [
    {
      dispenseId: 'disp-002-1',
      prescriptionNo: 'RX20260724002',
      medicationName: '布洛芬缓释胶囊',
      specText: '0.3g*10粒',
      quantity: 10,
      unitName: '粒',
      dosageText: '0.3g',
      frequencyText: 'bid',
      routeText: '口服',
      days: 5,
      totalPrice: 18.50,
      statusEnum: 'IN_PROGRESS',
      pharmacyName: '门诊药房',
      lotNumber: '20260420',
      traceNo: 'TRACE2024001',
      factoryName: 'ZZ制药',
      inventoryDetailList: [],
    },
  ],
}

/** ===== 模拟药师列表 ===== */
const mockDispensers: PractitionerOptionDto[] = [
  { practitionerId: 'disp-01', displayName: '王药师' },
  { practitionerId: 'disp-02', displayName: '李药师' },
  { practitionerId: 'disp-03', displayName: '张药师' },
]

/** ===== 模拟药房列表 ===== */
const mockPharmacies: PharmacyOptionDto[] = [
  { pharmacyId: 'ph-001', pharmacyName: '门诊药房' },
  { pharmacyId: 'ph-002', pharmacyName: '住院药房' },
]

/** ===== 分页查询患者队列 ===== */
export function mockPageDispenseQueue(
  params: PageDispenseQueueParams,
): { rows: PendingDispenseEncounterQueueRowDto[]; total: number } {
  let result = [...mockQueueData]

  if (params.keyword) {
    result = result.filter(
      (p) =>
        p.patientName.includes(params.keyword!) ||
        p.visitNo.includes(params.keyword!),
    )
  }

  if (params.dispenseStatusEnumIn?.length) {
    result = result.filter((p) =>
      params.dispenseStatusEnumIn!.includes(p.dispenseStatus),
    )
  }

  return {
    rows: result,
    total: result.length,
  }
}

/** ===== 查询患者处方明细 ===== */
export function mockListDispenseSlips(
  encounterId: string,
): EncounterMedicationDispenseSlipRowDto[] {
  return mockSlipData[encounterId] || []
}

/** ===== 准备发药 ===== */
export function mockSavePreparation(params: SavePreparationParams): { success: boolean } {
  console.log('[mock] 准备发药:', params)
  return { success: true }
}

/** ===== 确认发药 ===== */
export function mockConfirmDispense(params: ConfirmDispenseParams): { message: string } {
  console.log('[mock] 确认发药:', params)
  return { message: '发药成功' }
}

/** ===== 加载药师选项 ===== */
export function mockLoadDispenserOptions(keyword: string): PractitionerOptionDto[] {
  if (!keyword) return mockDispensers
  return mockDispensers.filter((d) => d.displayName.includes(keyword))
}

/** ===== 加载药房选项 ===== */
export function mockLoadPharmacyOptions(keyword: string): PharmacyOptionDto[] {
  if (!keyword) return mockPharmacies
  return mockPharmacies.filter((p) => p.pharmacyName.includes(keyword))
}