import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realCreateInpatientDischargeAdvice,
  realGetInpatientMedicalRecord,
  realPageInpatientDoctorPatientQueue,
  realPageInpatientMedicalRecordHistory,
  realSaveInpatientMedicalRecord
} from './patient-real'
import {
  isInpatientDoctorStationRealApiEnabled,
  pageInpatientDiagnosisRecords,
  pageInpatientDoctorPatientQueue
} from './index'

describe('住院医生站真实接口适配', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('使用后端患者队列路径和分页参数', async () => {
    httpMock.get.mockResolvedValue({
      message: '',
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 1,
        pages: 1,
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
            statusEnum: 'IN_HOSPITAL'
          }
        ]
      }
    })

    const result = await realPageInpatientDoctorPatientQueue({
      pageIndex: 1,
      pageSize: 20,
      keyword: '王建国'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/inpatientDoctorStation/cycle-control/patient-queue',
      { pageIndex: 1, pageSize: 20, keyword: '王建国' },
      { baseURL: '/inpatient-doctor-station-api' }
    )
    expect(result.rows?.[0]?.patientName).toBe('王建国')
  })

  it('测试环境默认走 Mock，不访问真实 HTTP', async () => {
    expect(isInpatientDoctorStationRealApiEnabled).toBe(false)

    const result = await pageInpatientDoctorPatientQueue({ pageIndex: 1, pageSize: 1 })

    expect(result.rows?.length).toBe(1)
    expect(httpMock.get).not.toHaveBeenCalled()
  })

  it('Mock 模式下诊断列表也不会访问真实 HTTP', async () => {
    const result = await pageInpatientDiagnosisRecords({
      pageIndex: 1,
      pageSize: 20,
      encounterId: 'ipt-enc-20260701-001',
      patientId: 'patient-10001'
    })

    expect(result.rows?.[0]?.description).toBe('冠状动脉粥样硬化性心脏病')
    expect(httpMock.get).not.toHaveBeenCalled()
  })

  it('使用后端病历列表参数并补齐前端摘要字段', async () => {
    httpMock.get.mockResolvedValue({
      message: '',
      data: {
        pageIndex: 1,
        pageSize: 10,
        total: 1,
        pages: 1,
        rows: [
          {
            id: 'record-1',
            document_type_name: '入院记录',
            title: '入院记录',
            status: 'SIGNED',
            update_by: 'admin'
          }
        ]
      }
    })

    const result = await realPageInpatientMedicalRecordHistory({
      pageIndex: 1,
      pageSize: 10,
      patientId: 'patient-1',
      currentEncounterId: 'enc-1',
      keyword: '入院'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/inpatient-record/brief',
      { pageIndex: 1, pageSize: 10, patient_id: 'patient-1', title: '入院' },
      { baseURL: '/inpatient-doctor-station-api' }
    )
    expect(result.rows?.[0]).toMatchObject({
      id: 'record-1',
      patientId: 'patient-1',
      encounterId: 'enc-1',
      documentTypeCode: 'ADMISSION_RECORD',
      status: 'SIGNED',
      statusName: '已签名',
      authoredByName: ''
    })
  })

  it('使用后端病历详情接口并固定历史详情只读', async () => {
    httpMock.get.mockResolvedValue({
      message: '',
      data: { id: 'record-2', content: '<p>病历正文</p>' }
    })

    const result = await realGetInpatientMedicalRecord('record-2')

    expect(httpMock.get).toHaveBeenCalledWith(
      '/inpatient-record/detail',
      { id: 'record-2' },
      { baseURL: '/inpatient-doctor-station-api' }
    )
    expect(result).toMatchObject({ id: 'record-2', content: '<p>病历正文</p>', version: 0 })
  })

  it('将前端病历草稿转换为后端保存 DTO', async () => {
    httpMock.post.mockResolvedValue({ message: '', data: 'record-3' })

    const result = await realSaveInpatientMedicalRecord({
      encounterId: 'enc-1',
      patientId: 'patient-1',
      documentTypeCode: 'FIRST_COURSE_RECORD',
      title: '首次病程记录',
      content: '<p>病程</p>',
      version: 0
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/inpatient-record/add',
      expect.objectContaining({
        patient_id: 'patient-1',
        encounter_id: 'enc-1',
        document_type_code: 'FIRST_COURSE_RECORD',
        title: '首次病程记录',
        content: '<p>病程</p>'
      }),
      { baseURL: '/inpatient-doctor-station-api' }
    )
    expect(result).toMatchObject({ id: 'record-3', status: 'DRAFT', readonly: true })
  })

  it('只向出院接口发送后端支持的三个字段', async () => {
    httpMock.post.mockResolvedValue({
      message: '',
      data: {
        orderId: 'order-1',
        encounterId: 'enc-1',
        statusEnum: 'ACTIVE',
        dischargeWay: '1',
        dischargeTime: '2026-08-08 18:00:00',
        encounterNewStatus: 'ON_HOLD'
      }
    })

    const result = await realCreateInpatientDischargeAdvice({
      encounterId: 'enc-1',
      plannedDischargeTime: '2026-08-08 18:00:00',
      outWayCode: '1'
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/inpatientDoctorStation/cycle-control/discharge-order',
      {
        encounterId: 'enc-1',
        dischargeWay: '1',
        dischargeTime: '2026-08-08 18:00:00'
      },
      { baseURL: '/inpatient-doctor-station-api' }
    )
    expect(result).toMatchObject({ id: 'order-1', outWayCode: '1', outWayName: '医嘱离院' })
  })

  it.each([
    [{ message: '直接错误信息' }, '直接错误信息'],
    [{ data: { message: '响应数据错误信息' } }, '响应数据错误信息'],
    [{ response: { data: { message: 'Axios 错误信息' } } }, 'Axios 错误信息'],
    [{}, '住院患者队列加载失败']
  ])('保留真实接口错误信息 %#', async (error, expected) => {
    httpMock.get.mockRejectedValue(error)

    await expect(
      realPageInpatientDoctorPatientQueue({ pageIndex: 1, pageSize: 20 })
    ).rejects.toThrow(expected)
  })
})
