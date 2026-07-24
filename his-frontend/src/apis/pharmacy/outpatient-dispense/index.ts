// src/apis/pharmacy/outpatient-dispense/index.ts

import type {
  PageDispenseQueueParams,
  PendingDispenseEncounterQueueRowDto,
  EncounterMedicationDispenseSlipRowDto,
  SavePreparationParams,
  ConfirmDispenseParams,
  PractitionerOptionDto,
  PharmacyOptionDto,
  PrintPrescriptionParams
} from './type'

import {
  mockPageDispenseQueue,
  mockListDispenseSlips,
  mockSavePreparation,
  mockConfirmDispense,
  mockLoadDispenserOptions,
  mockLoadPharmacyOptions
} from './mock'

/**
 * 加载发药页面初始化数据（药师候选等）
 */
export async function loadDispensePageInit(): Promise<{
  dispensers: PractitionerOptionDto[]
  pharmacies: PharmacyOptionDto[]
}> {
  // TODO: 替换为真实接口
  // const res = await httpLoadPharmacyDispensePageInit()
  // return { dispensers: res.preparerCandidatePage.content, pharmacies: res.pharmacyList }
  return {
    dispensers: await mockLoadDispenserOptions(''),
    pharmacies: await mockLoadPharmacyOptions('')
  }
}

/**
 * 分页查询发药患者队列
 */
export async function pageDispensePatientQueue(
  params: PageDispenseQueueParams
): Promise<{ rows: PendingDispenseEncounterQueueRowDto[]; total: number }> {
  // TODO: 替换为真实接口
  // return httpPagePendingDispenseEncounterQueue(params)
  return mockPageDispenseQueue(params)
}

/**
 * 查询患者处方明细（发放行列表）
 */
export async function listPatientPrescriptions(
  encounterId: string,
  status?: 'pending' | 'completed'
): Promise<EncounterMedicationDispenseSlipRowDto[]> {
  // TODO: 替换为真实接口
  // const statusEnum = status === 'pending' ? ['PREPARATION', 'IN_PROGRESS'] : ['COMPLETED']
  // return httpPageEncounterMedicationDispenseSlips({ encounterId, dispenseStatusEnumIn: statusEnum })
  return mockListDispenseSlips(encounterId)
}

/**
 * 保存配药准备（锁定库存）
 */
export async function saveDispensePreparation(
  params: SavePreparationParams
): Promise<{ success: boolean }> {
  // TODO: 替换为真实接口
  // return httpSavePrepareOutpatientMedicationDispense(params)
  return mockSavePreparation(params)
}

/**
 * 确认发药
 */
export async function confirmMedicationDispense(
  params: ConfirmDispenseParams
): Promise<{ message: string }> {
  // TODO: 替换为真实接口
  // return httpDispenseOutpatientMedications(params)
  return mockConfirmDispense(params)
}

/**
 * 加载调配药师选项
 */
export async function loadDispenserOptions(
  keyword: string
): Promise<{ label: string; value: string }[]> {
  // TODO: 替换为真实接口
  const list = await mockLoadDispenserOptions(keyword)
  return list.map((item) => ({
    label: item.displayName,
    value: String(item.practitionerId)
  }))
}

/**
 * 加载药房选项
 */
export async function loadPharmacyOptions(
  keyword: string
): Promise<{ label: string; value: string }[]> {
  // TODO: 替换为真实接口
  const list = await mockLoadPharmacyOptions(keyword)
  return list.map((item) => ({
    label: item.pharmacyName,
    value: item.pharmacyId
  }))
}

/**
 * 预览处方（打印预览）
 */
export async function previewPrescription(
  context: Record<string, unknown>
): Promise<{ previewUrl: string }> {
  // TODO: 替换为真实接口
  console.log('[print] 预览处方:', context)
  await new Promise((resolve) => setTimeout(resolve, 800))
  return { previewUrl: '/preview/prescription' }
}

/**
 * 打印处方
 */
export async function printPrescription(
  params: PrintPrescriptionParams
): Promise<{ success: boolean }> {
  // TODO: 替换为真实接口
  void params
  await new Promise((resolve) => setTimeout(resolve, 1500))
  return { success: true }
}
