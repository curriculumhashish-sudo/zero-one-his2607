import { beforeEach, describe, expect, it, vi } from 'vitest'

const post = vi.fn()
const userStore = {
  getUser: { id: 'doctor-1', username: 'admin' }
}

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ post })
}))

vi.mock('@/stores/user', () => ({
  useUserStore: () => userStore
}))

import {
  realCancelStopInpatientMedicalOrders,
  realGroupInpatientMedicalOrders,
  realSignInpatientMedicalOrders,
  realStopInpatientMedicalOrders,
  realUngroupInpatientMedicalOrders,
  realWithdrawInpatientMedicalOrders
} from './orders-lifecycle-real'

const data = {
  encounterId: 'enc-1',
  patientId: 'patient-1',
  items: [
    { id: 'order-1', version: 0, productTableName: 'drug_request', groupId: 'group-1' },
    { id: 'order-2', version: 0, productTableName: 'drug_request', groupId: 'group-2' }
  ]
}

describe('inpatient order lifecycle real api', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('maps sign, withdraw and cancel-stop targets', async () => {
    post.mockResolvedValue({ data: 'ok' })

    await realSignInpatientMedicalOrders(data)
    expect(post).toHaveBeenLastCalledWith(
      '/medical-order/sign',
      expect.objectContaining({
        encounter_id: 'enc-1',
        patient_id: 'patient-1',
        signTargets: [{ productTableName: 'drug_request', id: 'order-1' }, { productTableName: 'drug_request', id: 'order-2' }]
      }),
      expect.anything()
    )

    await realWithdrawInpatientMedicalOrders(data)
    expect(post).toHaveBeenLastCalledWith(
      '/medical-order/withdraw',
      expect.objectContaining({ targets: expect.any(Array) }),
      expect.anything()
    )

    await realCancelStopInpatientMedicalOrders(data)
    expect(post).toHaveBeenLastCalledWith(
      '/medical-order/cancel-stop',
      expect.objectContaining({ lines: expect.any(Array) }),
      expect.anything()
    )
  })

  it('maps stop reason and partial result', async () => {
    post.mockResolvedValue({
      data: {
        successCount: 1,
        failCount: 1,
        failDetails: [{ requestId: 'order-2', reason: '已执行' }]
      }
    })

    const result = await realStopInpatientMedicalOrders({ ...data, stopReason: 2 })
    expect(post).toHaveBeenCalledWith(
      '/order/stop-order',
      expect.objectContaining({
        operatorId: 'doctor-1',
        stopReason: 2,
        lines: expect.any(Array)
      }),
      expect.anything()
    )
    expect(result).toEqual(
      expect.objectContaining({
        updatedCount: 1,
        failedCount: 1,
        failureMessages: ['已执行']
      })
    )
  })

  it('maps merge and split requests', async () => {
    post.mockResolvedValueOnce({ data: { groupId: 'group-new' } }).mockResolvedValueOnce({ data: { splitCount: 2 } })

    const merged = await realGroupInpatientMedicalOrders(data)
    expect(post).toHaveBeenNthCalledWith(
      1,
      '/order/merge_order',
      expect.objectContaining({
        clearGroupIds: ['group-1', 'group-2'],
        assignGroup: expect.objectContaining({ medicationRequestIds: ['order-1', 'order-2'] })
      }),
      expect.anything()
    )
    expect(merged.groupId).toBe('group-new')

    const split = await realUngroupInpatientMedicalOrders(data)
    expect(post).toHaveBeenNthCalledWith(
      2,
      '/order/split-order',
      expect.objectContaining({ orderIds: ['order-1', 'order-2'] }),
      expect.anything()
    )
    expect(split.updatedCount).toBe(2)
  })

  it('blocks lifecycle requests without product table name or stop reason', async () => {
    await expect(
      realSignInpatientMedicalOrders({
        ...data,
        items: [{ id: 'order-1', version: 0 }]
      })
    ).rejects.toThrow('产品定义表名')
    await expect(realStopInpatientMedicalOrders(data)).rejects.toThrow('停嘱原因码')
    expect(post).not.toHaveBeenCalled()
  })
})
