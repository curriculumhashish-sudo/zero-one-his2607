import { describe, expect, it } from 'vitest'

import { buildPatientFilterQuery, mapPatientQueueItemToSummary } from './adapter'

describe('住院护士站患者数据适配器', () => {
  it('保留 wardId、houseId 和患者任务计数', () => {
    const patient = mapPatientQueueItemToSummary({
      encounterId: 'encounter-a',
      patientId: 'patient-a',
      patientName: '测试患者',
      wardId: 'ward-a',
      wardName: 'A病区',
      houseId: 'house-a',
      houseName: 'A病房',
      pendingAuditCount: 2,
      pendingExecuteCount: 3
    })

    expect(patient).toEqual(
      expect.objectContaining({
        wardId: 'ward-a',
        houseId: 'house-a',
        pendingAuditCount: 2,
        pendingExecuteCount: 3
      })
    )
  })

  it('只将后端已实现的患者筛选转换为接口契约', () => {
    expect(
      buildPatientFilterQuery({
        keyword: '  小孙  ',
        taskFilter: 'pending-audit'
      })
    ).toEqual({
      keyword: '小孙',
      nurseStationTaskFilter: 'pending-audit'
    })
  })
})
