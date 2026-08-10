// @vitest-environment jsdom

import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import dayjs from 'dayjs'

const get = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get })
}))

import {
  checkInpatientAdmission,
  acceptPatient,
  getDiagnoses,
  getMedicalRecord,
  pageAdviceOrders,
  listDoctorPatientQueue,
  pagePatientAdviceOrderHistory,
  pagePatientAdviceOrderHistoryDetails,
  pagePatientDiagnosisHistory,
  pagePatientMedicalRecordHistory,
  printAdviceApplicationForm,
  queryDoctorPrescriptionSheets,
  queryDoctorRefundItems
} from './index'

const encounterId = 'enc-demo-in-progress'
const patientId = 'patient-demo-001'
const patientNo = 'MR-DEMO-0001'

async function resolveRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('全功能在诊演示患者', () => {
  beforeEach(() => {
    vi.useFakeTimers()
    get.mockResolvedValue({
      code: 10000,
      message: 'success',
      data: [
        {
          patientId,
          encounterId,
          patientName: '全功能演示患者',
          gender: '男',
          age: 52,
          registerType: '专家门诊',
          visitTime: `${dayjs().format('YYYY-MM-DD')} 07:30`,
          visitNo: patientNo,
          payType: '职工医保',
          visitStatus: '在诊',
          queueNumber: 0,
          departmentName: '心血管内科',
          priorityName: '全功能演示'
        }
      ]
    })
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('具备当前诊疗、正式医嘱和打印申请单所需数据', async () => {
    const queue = await resolveRequest(
      listDoctorPatientQueue({
        visitDate: dayjs().format('YYYY-MM-DD'),
        patientName: '全功能演示患者',
        visitStatus: 'in-progress'
      })
    )
    expect(queue).toHaveLength(1)
    expect(queue[0]?.encounterId).toBe(encounterId)

    const context = await resolveRequest(acceptPatient(encounterId))
    expect(context.visitStatus).toBe('in-progress')
    expect(context.phone).toBeTruthy()

    const record = await resolveRequest(getMedicalRecord(encounterId))
    expect(record.id).toBeTruthy()
    expect(record.content).toContain('处理意见')

    const diagnoses = await resolveRequest(getDiagnoses(encounterId))
    expect(diagnoses.rows).toHaveLength(2)
    expect(diagnoses.rows.filter((item) => item.primary)).toHaveLength(1)

    const orders = await resolveRequest(
      pageAdviceOrders({ encounterId, pageIndex: 1, pageSize: 20 })
    )
    expect(orders.total).toBe(6)
    expect(new Set(orders.rows?.map((item) => item.status))).toEqual(
      new Set(['DRAFT', 'SIGNED', 'WITHDRAWN'])
    )
    expect(new Set(orders.rows?.map((item) => item.category))).toEqual(
      new Set(['DRUG', 'LAB', 'EXAM', 'TREATMENT'])
    )

    const printable = orders.rows?.filter(
      (item) => item.status === 'SIGNED' && item.category !== 'DRUG'
    )
    expect(printable).toHaveLength(2)
    for (const order of printable ?? []) {
      const form = await resolveRequest(
        printAdviceApplicationForm({ serviceRequestId: order.serviceRequestId! })
      )
      expect(form.adviceId).toBe(order.id)
      expect(form.patientName).toBe('全功能演示患者')
    }
  })

  it('具备病历、诊断、医嘱、处方和退费历史，并且允许办理住院', async () => {
    const records = await resolveRequest(
      pagePatientMedicalRecordHistory({
        patientId,
        pageIndex: 1,
        pageSize: 10
      })
    )
    expect(records.total).toBe(3)

    const diagnoses = await resolveRequest(
      pagePatientDiagnosisHistory({
        patientId,
        pageIndex: 1,
        pageSize: 20
      })
    )
    expect(diagnoses.total).toBeGreaterThanOrEqual(3)

    const adviceEncounters = await resolveRequest(
      pagePatientAdviceOrderHistory({
        patientId,
        excludeEncounterId: encounterId,
        pageIndex: 1,
        pageSize: 10
      })
    )
    expect(adviceEncounters.total).toBe(2)
    const adviceDetails = await resolveRequest(
      pagePatientAdviceOrderHistoryDetails({
        patientId,
        encounterId: adviceEncounters.rows![0]!.encounterId,
        pageIndex: 1,
        pageSize: 10
      })
    )
    expect(adviceDetails.rows?.every((item) => item.status !== 'DRAFT')).toBe(true)

    const refundItems = await resolveRequest(queryDoctorRefundItems({ num: patientNo }))
    expect(refundItems).toHaveLength(4)
    expect(refundItems.some((item) => item.refundStatus === '未申请')).toBe(true)

    const currentPrescriptions = await resolveRequest(
      queryDoctorPrescriptionSheets({ num: patientNo, type: 'CURRENT_ENCOUNTER' })
    )
    const historicalPrescriptions = await resolveRequest(
      queryDoctorPrescriptionSheets({ num: patientNo, type: 'HISTORY' })
    )
    expect(currentPrescriptions).toHaveLength(1)
    expect(historicalPrescriptions).toHaveLength(2)

    const admission = await resolveRequest(checkInpatientAdmission(encounterId))
    expect(admission.exists).toBe(false)
  })
})
