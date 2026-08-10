import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  outpatientNurseStationApiBaseURL,
  realCancelNurseExecution,
  realGetNurseConsumables,
  realGetNurseExecutionItems,
  realPageNursePatientQueue,
  realPagePatientExecutionHistory,
  realPerformNurseExecutions
} from './real'

const backendExecution = {
  id: 'record-1',
  bus_no: 'SR-1',
  status_enum: 'COMPLETED',
  status_name: '已完成',
  category_enum: 'MEDICATION',
  category_name: '药品',
  item_name: '青霉素皮试',
  quantity: 1,
  unit_name: '次',
  patient_id: 'patient-1',
  encounter_id: 'encounter-1',
  performer_name: '护士甲',
  check_time: '2026-08-08 09:00:00',
  execution_count: 1,
  execution_status: 0,
  execution_status_name: '待执行'
}

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('门诊护士站真实接口适配', () => {
  it('查询患者队列并对缺失展示字段做显式降级', async () => {
    httpMock.get.mockResolvedValue({
      code: 10000,
      message: '操作成功',
      data: {
        pageIndex: 1,
        pageSize: 10,
        total: 1,
        pages: 1,
        rows: [
          {
            id: 'queue-1',
            bus_no: 'BOOKING-1',
            patient_id: 'patient-1',
            patient_name: '患者甲',
            patient_gender: 'FEMALE',
            patient_age: '30',
            encounter_id: 'encounter-1',
            destination_location_name: '内科门诊',
            display_order: 8,
            booking_date: '2026-08-08',
            triage_status: 'WAITING',
            triage_status_name: '候诊中'
          }
        ]
      }
    })

    const page = await realPageNursePatientQueue({
      pageIndex: 1,
      pageSize: 10,
      bookingDate: '2026-08-08'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/ons/patient-queue',
      { booking_date: '2026-08-08', pageIndex: 1, pageSize: 10 },
      { baseURL: outpatientNurseStationApiBaseURL }
    )
    expect(page.rows?.[0]).toMatchObject({
      encounterId: 'encounter-1',
      patientId: 'patient-1',
      patientName: '患者甲',
      genderName: '女',
      ageText: '30岁',
      visitNo: 'BOOKING-1',
      status: 'WAITING'
    })
  })

  it('患者详情接口使用 patientId 查询并按 encounterId 过滤', async () => {
    httpMock.get
      .mockResolvedValueOnce({
        code: 10000,
        data: [backendExecution, { ...backendExecution, id: 'other', encounter_id: 'encounter-2' }]
      })
      .mockResolvedValueOnce({
        code: 10000,
        data: [
          {
            id: 'consumable-1',
            deviceDefId: 'device-1',
            statusEnum: 'COMPLETED',
            quantity: 2,
            unitCode: '盒',
            lotNumber: 'LOT-1',
            encounterId: 'encounter-1',
            locationId: 'room-1'
          },
          { id: 'other', encounterId: 'encounter-2' }
        ]
      })

    const executions = await realGetNurseExecutionItems('patient-1', 'encounter-1')
    const consumables = await realGetNurseConsumables('patient-1', 'encounter-1')

    expect(httpMock.get).toHaveBeenNthCalledWith(
      1,
      '/ons/patient/patient-1/service-requests',
      undefined,
      { baseURL: outpatientNurseStationApiBaseURL }
    )
    expect(executions).toEqual([
      expect.objectContaining({
        itemId: 'record-1',
        status: 'PENDING',
        executionCount: 1
      })
    ])
    expect(consumables).toEqual([
      expect.objectContaining({
        deviceDefId: 'device-1',
        statusEnum: 'COMPLETED',
        quantity: 2
      })
    ])
  })

  it('执行请求只发送 Swagger 字段并把批量结果转换为页面模型', async () => {
    httpMock.post.mockResolvedValue({
      code: 10000,
      data: {
        totalCount: 2,
        successCount: 1,
        failedCount: 1,
        successIds: ['record-1'],
        failedItems: [{ recordId: 'record-2', errorCode: 'NOT_FOUND', reason: '记录不存在' }]
      }
    })

    const result = await realPerformNurseExecutions({
      encounterId: 'encounter-1',
      items: [{ itemId: 'record-1' }, { itemId: 'record-2' }],
      idempotencyKey: 'idem-1',
      executorId: 'nurse-1',
      executorName: '护士甲'
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/ons/execute',
      {
        record_ids: ['record-1', 'record-2'],
        encounter_id: 'encounter-1',
        executor_id: 'nurse-1',
        executor_name: '护士甲',
        execute_time: undefined,
        idempotency_key: 'idem-1',
        execute_type: undefined,
        remark: undefined
      },
      { baseURL: outpatientNurseStationApiBaseURL }
    )
    expect(result.successIds).toEqual(['record-1'])
    expect(result.failedItems).toEqual([
      { recordId: 'record-2', errorCode: 'NOT_FOUND', reason: '记录不存在' }
    ])
  })

  it('取消请求映射后端字段', async () => {
    httpMock.post.mockResolvedValueOnce({
      code: 10000,
      data: { successIds: ['record-1'], successCount: 1, failedItems: [] }
    })
    const cancelled = await realCancelNurseExecution({
      encounterId: 'encounter-1',
      item: { itemId: 'record-1' },
      reason: '登记错误',
      operatorId: 'nurse-1',
      idempotencyKey: 'cancel-1'
    })
    expect(httpMock.post).toHaveBeenNthCalledWith(
      1,
      '/ons/cancel-execute',
      expect.objectContaining({
        record_ids: ['record-1'],
        encounter_id: 'encounter-1',
        cancel_reason: '登记错误',
        operator_id: 'nurse-1',
        idempotency_key: 'cancel-1'
      }),
      { baseURL: outpatientNurseStationApiBaseURL }
    )
    expect(cancelled.successIds).toEqual(['record-1'])
  })

  it('执行记录按后端分页读取，并保留 encounterId 作为缺失就诊号的降级值', async () => {
    httpMock.get.mockResolvedValue({
      code: 10000,
      data: { pageIndex: 1, pageSize: 10, total: 1, pages: 1, rows: [backendExecution] }
    })

    const page = await realPagePatientExecutionHistory('patient-1', {
      pageIndex: 1,
      pageSize: 10,
      startDate: '2026-08-01',
      endDate: '2026-08-08'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/ons/service-requests',
      {
        patient_id: 'patient-1',
        start_date: '2026-08-01',
        end_date: '2026-08-08',
        pageIndex: 1,
        pageSize: 10
      },
      { baseURL: outpatientNurseStationApiBaseURL }
    )
    expect(page.rows?.[0]).toMatchObject({
      itemId: 'record-1',
      encounterId: 'encounter-1',
      visitNo: 'encounter-1'
    })
  })

  it('真实接口失败时向页面抛错，不回退到内存 Mock', async () => {
    httpMock.get.mockRejectedValue(new Error('后端不可用'))

    await expect(realPageNursePatientQueue({ pageIndex: 1, pageSize: 10 })).rejects.toThrow(
      '后端不可用'
    )
  })
})
