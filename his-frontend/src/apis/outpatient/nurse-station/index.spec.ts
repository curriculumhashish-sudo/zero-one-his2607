import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import dayjs from 'dayjs'

import {
  getNurseExecutionItems,
  isOutpatientNurseStationRealApiEnabled,
  pageNursePatientQueue,
  printNurseExecutions
} from './index'

describe('outpatient nurse-station API facade', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('测试环境使用可交互 Mock 患者', async () => {
    expect(isOutpatientNurseStationRealApiEnabled).toBe(false)

    const queueRequest = pageNursePatientQueue({
      pageIndex: 1,
      pageSize: 10,
      bookingDate: dayjs().format('YYYY-MM-DD')
    })
    await vi.runAllTimersAsync()
    const queue = await queueRequest

    expect(queue.rows?.map((row) => row.patientId)).toEqual([
      'mock-patient-001',
      'mock-patient-002'
    ])

    const itemsRequest = getNurseExecutionItems('mock-patient-001', 'mock-encounter-001')
    await vi.runAllTimersAsync()
    const items = await itemsRequest

    expect(items.map((item) => item.status)).toEqual([
      'PENDING',
      'PENDING',
      'COMPLETED',
      'CANCELLED'
    ])

    const printRequest = printNurseExecutions({ recordIds: ['mock-exec-completed-001'] })
    await vi.runAllTimersAsync()
    const printContent = await printRequest
    expect(printContent).toContain('服务请求打印单')
  })
})
