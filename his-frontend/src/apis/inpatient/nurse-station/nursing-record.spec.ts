import { describe, expect, it } from 'vitest'

import {
  deleteNurseStationNursingRecord,
  queryNurseStationNursingRecords,
  saveNurseStationNursingRecords
} from './index'

describe('住院护士站护理记录 Mock API', () => {
  it('按患者和时间范围返回纸质单抬头与记录', async () => {
    const result = await queryNurseStationNursingRecords({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      recordedAtFrom: '2026-07-27 00:00:00',
      recordedAtTo: '2026-08-02 23:59:59',
      sortAsc: true
    })

    expect(result.paperHeader).toEqual(
      expect.objectContaining({
        patientName: '孙晓梅',
        bedName: '01床',
        encounterBusNo: 'ZY202607210001'
      })
    )
    expect(result.rows).toHaveLength(2)
    expect(result.rows[0]).toEqual(
      expect.objectContaining({
        vitalSignId: 'nursing-record-001',
        version: expect.any(Number),
        observation: expect.any(String)
      })
    )
  })

  it('批量保存新增记录支持幂等重试', async () => {
    const request = {
      idempotencyKey: 'nursing-save-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      records: [
        {
          clientRowId: 'client-new-001',
          recordDate: '2026-08-02',
          recordTime: '09:30',
          temperature: '36.6',
          observation: '患者一般情况稳定。',
          nurseSignature: '周护士'
        }
      ]
    }

    const first = await saveNurseStationNursingRecords(request)
    const repeated = await saveNurseStationNursingRecords(request)

    expect(first).toEqual(expect.objectContaining({ success: true, savedCount: 1 }))
    expect(repeated).toEqual(first)
    expect(first.results[0]?.row).toEqual(
      expect.objectContaining({ vitalSignId: expect.any(String), version: 1 })
    )
  })

  it('更新已有记录时拒绝过期版本', async () => {
    const result = await saveNurseStationNursingRecords({
      idempotencyKey: 'nursing-save-stale-spec-001',
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      records: [
        {
          clientRowId: 'client-stale-001',
          vitalSignId: 'nursing-record-001',
          expectedVersion: 99,
          recordDate: '2026-08-01',
          recordTime: '08:30',
          temperature: '36.8'
        }
      ]
    })

    expect(result).toEqual(expect.objectContaining({ success: false, savedCount: 0 }))
    expect(result.results[0]).toEqual(
      expect.objectContaining({ success: false, message: '记录已被他人修改' })
    )
  })

  it('删除护理记录会校验患者归属和版本', async () => {
    const result = await deleteNurseStationNursingRecord({
      idempotencyKey: 'nursing-delete-spec-001',
      encounterId: 'encounter-004',
      patientId: 'patient-004',
      vitalSignId: 'nursing-record-004',
      expectedVersion: 1,
      reason: '测试删除'
    })
    const refreshed = await queryNurseStationNursingRecords({
      encounterId: 'encounter-004',
      patientId: 'patient-004'
    })

    expect(result).toEqual(expect.objectContaining({ success: true }))
    expect(refreshed.rows).toHaveLength(0)
  })
})
