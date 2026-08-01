import { describe, expect, it } from 'vitest'

import {
  buildPatientFilterQuery,
  mapPatientQueueItemToSummary,
  mapWardTreeToOptions
} from './adapter'

describe('住院护士站患者数据适配器', () => {
  it('将后端病区树展开为页面筛选项', () => {
    const options = mapWardTreeToOptions({
      wardNodes: [
        {
          wardId: 'ward-a',
          wardName: 'A病区',
          houseNodes: [{ houseId: 'house-a', houseName: 'A病房' }]
        }
      ]
    })

    expect(options.wardOptions).toEqual([{ id: 'ward-a', name: 'A病区' }])
    expect(options.houseOptions).toEqual([{ id: 'house-a', name: 'A病房', wardId: 'ward-a' }])
  })

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

  it('将页面多选值转换为推荐接口契约', () => {
    expect(
      buildPatientFilterQuery({
        keyword: '  小孙  ',
        queueScope: 'settled-discharged',
        taskFilter: 'pending-audit',
        selectedWardIds: ['ward-a', 'ward-b'],
        selectedHouseIds: ['house-a']
      })
    ).toEqual({
      keyword: '小孙',
      wardIds: 'ward-a,ward-b',
      houseIds: 'house-a',
      nurseStationTaskFilter: 'pending-audit',
      nurseStationStatusFilter: 'SETTLED_DISCHARGED'
    })
  })
})
