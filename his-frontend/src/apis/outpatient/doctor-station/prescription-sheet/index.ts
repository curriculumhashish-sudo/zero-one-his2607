import { useHttp } from '@/plugins/http'

import {
  isOutpatientDoctorStationRealApiEnabled,
  outpatientDoctorStationApiBaseURL
} from '../mode'
import { mockQueryDoctorPrescriptionSheets } from './mock'
import type { DoctorPrescriptionSheetQuery, DoctorPrescriptionSheetVO } from './type'

interface BackendJsonVO<T> {
  code?: number
  message?: string
  data?: T | null
}

interface BackendPrescription {
  patientName?: string
  age?: number | string
  gender?: string
  department?: string
  patientId?: string
  rxId?: string
  encounterId?: string
  prescriptionNo?: string
  costType?: string
  date?: string
  issuedTime?: string
  status?: string
  statusName?: string
  rxType?: string
  doctor?: string
  diagnosis?: string
  totalPrice?: number | string
  pharmacist?: string
  dispenser?: string
  priceChecker?: string
  items?: string
}

function unwrap<T>(response: BackendJsonVO<T> | T): T {
  const body = response as BackendJsonVO<T>
  if (typeof body.code === 'number' && body.code !== 10000) {
    throw new Error(body.message || 'Prescription loading failed')
  }
  if (body.data === null) throw new Error(body.message || 'Prescription loading failed')
  return body.data === undefined ? (response as T) : body.data
}

function number(value: unknown): number | undefined {
  const result = Number(value)
  return Number.isFinite(result) ? result : undefined
}

function mapPrescription(
  row: BackendPrescription,
  query: DoctorPrescriptionSheetQuery
): DoctorPrescriptionSheetVO {
  return {
    patientName: row.patientName,
    age: number(row.age),
    gender: row.gender,
    department: row.department,
    patientId: row.patientId || query.num,
    prescriptionNo: row.prescriptionNo || row.rxId,
    costType: query.paymentTypeName || row.costType,
    date: row.date || row.issuedTime,
    doctor: row.doctor,
    diagnosis: row.diagnosis,
    totalPrice: number(row.totalPrice),
    pharmacist: row.pharmacist,
    dispenser: row.dispenser,
    priceChecker: row.priceChecker,
    items: row.items
  }
}

async function realQueryDoctorPrescriptionSheets(
  query: DoctorPrescriptionSheetQuery
): Promise<DoctorPrescriptionSheetVO[]> {
  const response = await useHttp().get<BackendJsonVO<BackendPrescription[]> | BackendPrescription[]>(
    '/c2-opdoctor/prescription_query',
    {
      num: query.num?.trim() || undefined,
      encounterId: query.encounterId || undefined,
      type: query.type === 'CURRENT_ENCOUNTER' ? 'today' : 'history',
      page: 1,
      pageSize: 100
    },
    { baseURL: outpatientDoctorStationApiBaseURL }
  )
  const data = unwrap(response)
  const rows = Array.isArray(data)
    ? data
    : ((data as unknown as { rows?: BackendPrescription[]; data?: BackendPrescription[] }).rows ??
      (data as unknown as { data?: BackendPrescription[] }).data ??
      [])
  return rows.map((row) => mapPrescription(row, query))
}

/**
 * 查询患者处方单。
 *
 * 真实后端接口：GET /cyclectrl/prescription_query。
 * 当前阶段沿用医生站 Mock-first 结构，联调时只需替换本函数内部实现。
 */
export function queryDoctorPrescriptionSheets(
  query: DoctorPrescriptionSheetQuery
): Promise<DoctorPrescriptionSheetVO[]> {
  if (isOutpatientDoctorStationRealApiEnabled) return realQueryDoctorPrescriptionSheets(query)
  return mockQueryDoctorPrescriptionSheets(query)
}

export type * from './type'
