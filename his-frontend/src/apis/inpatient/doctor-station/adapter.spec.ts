import { describe, expect, it } from 'vitest'

import {
  mapDischargeOrderDetail,
  mapInpatientPatientQueuePage,
  type InpatientPatientQueuePageDTO
} from './adapter'

describe('住院医生站后端适配器', () => {
  it('将患者队列 DTO 映射为前端患者模型并补齐缺失字段', () => {
    const page: InpatientPatientQueuePageDTO = {
      pageIndex: 2,
      pageSize: 20,
      total: 21,
      pages: 2,
      rows: [
        {
          encounterId: 'enc-1',
          patientId: 'patient-1',
          patientName: '王建国',
          gender: '男',
          age: 68,
          encounterBusNo: 'ZY001',
          admitTime: '2026-08-08 09:00:00',
          organizationName: '心血管内科',
          wardName: '心内一病区',
          bedName: '08床',
          statusEnum: 'DISCHARGE_PENDING'
        }
      ]
    }

    const result = mapInpatientPatientQueuePage(page)
    expect(result).toMatchObject({ pageIndex: 2, pageSize: 20, total: 21, pages: 2 })
    expect(result.rows?.[0]).toMatchObject({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      patientName: '王建国',
      genderName: '男',
      ageText: '68岁',
      patientNo: 'patient-1',
      inpatientNo: 'ZY001',
      departmentName: '心血管内科',
      encounterStatus: 'DISCHARGE_PENDING',
      encounterStatusName: '待出院',
      version: 0
    })
  })

  it('后端字段缺失时使用稳定的前端降级值', () => {
    const result = mapInpatientPatientQueuePage({ rows: [{}] })
    expect(result.rows?.[0]).toMatchObject({
      encounterId: '',
      patientId: '',
      patientNo: '',
      inpatientNo: '',
      departmentName: '',
      encounterStatus: 'IN_HOSPITAL',
      encounterStatusName: '在院',
      version: 0
    })
  })

  it('将出院医嘱响应映射为前端模型并保留表单字段', () => {
    const result = mapDischargeOrderDetail(
      {
        orderId: 'order-1',
        encounterId: 'enc-1',
        statusEnum: 'SIGNED',
        dischargeWay: '1',
        dischargeTime: '2026-08-08 18:00:00',
        encounterNewStatus: 'DISCHARGE_PENDING'
      },
      {
        encounterId: 'enc-1',
        patientId: 'patient-1',
        encounterVersion: 0,
        plannedDischargeTime: '2026-08-08 18:00:00',
        outWayCode: '1',
        reasonText: '病情稳定',
        sourceLocationId: 'bed-8'
      }
    )

    expect(result).toMatchObject({
      id: 'order-1',
      encounterId: 'enc-1',
      patientId: 'patient-1',
      plannedDischargeTime: '2026-08-08 18:00:00',
      outWayCode: '1',
      outWayName: '医嘱离院',
      reasonText: '病情稳定',
      sourceLocationId: 'bed-8'
    })
  })
})
