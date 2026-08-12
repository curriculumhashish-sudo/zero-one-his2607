import type {
  PageDispenseQueueParams,
  PendingDispenseEncounterQueueRowDto,
  EncounterMedicationDispenseSlipRowDto,
  SavePreparationParams,
  ConfirmDispenseParams,
  BatchDispenseResultDto,
  PractitionerOptionDto,
  PharmacyOptionDto,
  DispenseSlipDto
} from './type'
import type { PrintActionContext } from '@/components/common/PrintActionPanel.vue'
import type { PatientSummary } from '@/components/patient/type'

import {
  mockPageDispenseQueue,
  mockListDispenseSlips,
  mockSavePreparation,
  mockConfirmDispense,
  mockLoadDispenserOptions,
  mockLoadPharmacyOptions,
  mockPagePatientQueue
} from './mock'

import { adaptPatientSummary } from './adapter'

export async function loadDispensePageInit(): Promise<{
  dispensers: PractitionerOptionDto[]
  pharmacies: PharmacyOptionDto[]
}> {
  return {
    dispensers: await mockLoadDispenserOptions(''),
    pharmacies: await mockLoadPharmacyOptions(''),
  }
}

export async function pageDispensePatientQueue(
  params: PageDispenseQueueParams,
): Promise<{ rows: PendingDispenseEncounterQueueRowDto[]; total: number }> {
  const result = mockPageDispenseQueue(params)
  return {
    rows: result.records,
    total: result.total
  }
}

export async function pagePatientQueue(
  params: PageDispenseQueueParams,
): Promise<{ rows: PatientSummary[]; total: number }> {
  const result = mockPagePatientQueue(params)
  return {
    rows: result.records as PatientSummary[],
    total: result.total
  }
}

const prescriptionNoMap: Record<string, string> = {
  ENC001: 'CP2026-07-230004',
  ENC002: 'CP2026-07-230005',
  ENC003: 'CP2026-07-230006',
  ENC004: 'CP2026-07-230007',
  ENC005: 'CP2026-07-230008',
  ENC006: 'CP2026-07-230001',
  ENC007: 'CP2026-07-230002',
  ENC008: 'CP2026-07-230003',
}

export async function listPatientPrescriptions(
  encounterId: string,
): Promise<EncounterMedicationDispenseSlipRowDto[]> {
  const slips = mockListDispenseSlips(encounterId)
  return slips.flatMap((slip) =>
    slip.items.map((item) => {
      const firstLot = item.inventoryLots?.[0]
      const rxNo = prescriptionNoMap[encounterId] || `CP${new Date().getFullYear()}-${String(new Date().getMonth() + 1).padStart(2, '0')}-${String(slip.slipId).padStart(8, '0')}`
      return {
        dispenseId: item.itemId,
        prescriptionNo: rxNo,
        medicationName: item.itemName,
        rxType: inferRxType(item.itemName),
        specText: item.spec,
        quantity: item.quantity,
        unitName: item.unit,
        dosageText: item.dosage,
        frequencyText: item.frequency,
        routeText: item.route,
        days: item.days,
        totalPrice: item.totalAmount,
        statusEnum: (item.status === 'completed' ? 'COMPLETED' : 'PENDING') as EncounterMedicationDispenseSlipRowDto['statusEnum'],
        pharmacyName: firstLot?.inventoryName || '',
        lotNumber: firstLot?.lotNo || '',
        factoryName: getFactoryName(item.itemName),
        traceNo: '',
        inventoryDetailList: item.inventoryLots?.map((lot) => ({
          pharmacyOrganizationId: lot.inventoryId,
          pharmacyName: lot.inventoryName,
          lotNumber: lot.lotNo,
          availableQuantity: lot.availableQty
        })) || []
      }
    })
  )
}

function getFactoryName(drugName: string): string {
  const factoryMap: Record<string, string> = {
    '阿莫西林胶囊': '湖北科伦药业有限公司',
    '布洛芬缓释胶囊': '四川科伦药业股份有限公司',
    '氯雷他定片': '湖北科伦药业有限公司',
    '硝苯地平缓释片': '四川科伦药业股份有限公司',
    '阿司匹林肠溶片': '湖北科伦药业有限公司',
    '丹参滴丸': '四川科伦药业股份有限公司',
    '妇科千金片': '湖北科伦药业有限公司',
    '益母草颗粒': '四川科伦药业股份有限公司',
    '对乙酰氨基酚片': '湖北科伦药业有限公司',
    '骨痛贴膏': '四川科伦药业股份有限公司',
    '奥美拉唑肠溶胶囊': '湖北科伦药业有限公司',
    '左氧氟沙星滴眼液': '四川科伦药业股份有限公司',
    '珍珠明目滴眼液': '湖北科伦药业有限公司',
    '盐酸氨溴索注射液': '湖北科伦药业有限公司',
    '氯化钠注射液': '四川科伦药业股份有限公司'
  }
  return factoryMap[drugName] || '未知厂家'
}

/** 根据药品名推断处方类型（西药/成药 vs 中药饮片） */
function inferRxType(itemName: string): 'western' | 'herbal' {
  const herbalDrugs = new Set([
    '丹参滴丸',
    '妇科千金片',
    '益母草颗粒',
    '骨痛贴膏',
    '珍珠明目滴眼液',
  ])
  return herbalDrugs.has(itemName) ? 'herbal' : 'western'
}

export async function listDispenseSlips(encounterId: string): Promise<DispenseSlipDto[]> {
  return mockListDispenseSlips(encounterId)
}

export async function saveDispensePreparation(
  _params: SavePreparationParams,
): Promise<{ success: boolean }> {
  return mockSavePreparation()
}

export async function confirmMedicationDispense(
  params: ConfirmDispenseParams,
): Promise<BatchDispenseResultDto> {
  return mockConfirmDispense(params)
}

export async function loadDispenserOptions(
  keyword: string,
): Promise<{ label: string; value: string }[]> {
  const list = await mockLoadDispenserOptions(keyword)
  return list.map((item) => ({
    label: item.displayName,
    value: String(item.practitionerId),
  }))
}

export async function loadPharmacyOptions(
  keyword: string,
): Promise<{ label: string; value: string }[]> {
  const list = await mockLoadPharmacyOptions(keyword)
  return list.map((item) => ({
    label: item.pharmacyName,
    value: item.pharmacyId,
  }))
}

export async function previewPrescription(
  context: Record<string, unknown>,
): Promise<{ previewUrl: string }> {
  console.log('[print] 预览处方:', context)
  await new Promise((resolve) => setTimeout(resolve, 800))
  return { previewUrl: '/preview/prescription.html' }
}

export async function printPrescription(
  context: PrintActionContext,
): Promise<{ success: boolean }> {
  console.log('[print] 打印处方:', context)
  await new Promise((resolve) => setTimeout(resolve, 1500))
  return { success: true }
}

export { adaptPatientSummary }
