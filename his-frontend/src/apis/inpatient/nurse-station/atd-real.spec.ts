import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import {
  realAssignNurseStationBed,
  realClearDischargedBed,
  realExecuteInpatientDischarge,
  realQueryNurseStationBedBoard
} from './atd-real'

const patientRow = {
  encounterId: 'encounter-001',
  patientId: 'patient-001',
  patientName: '测试患者',
  wardId: 'WARD-1',
  wardName: '内科疗区',
  houseId: 'HOUSE-301',
  houseName: '301病房',
  bedId: 'BED-1',
  bedName: '01床',
  statusEnum: 'IN_PROGRESS',
  statusEnumText: '在院'
}

beforeEach(() => {
  httpMock.get.mockReset()
  httpMock.post.mockReset()
})

describe('住院护士站入出转真实接口适配', () => {
  it('空闲床位按分页继续读取，直到收齐后端 total', async () => {
    httpMock.get.mockImplementation((url: string, params?: { pageIndex?: number }) => {
      if (url === '/encountermanage/query-patient-queue') {
        return Promise.resolve({ data: { total: 0, rows: [] } })
      }
      const pageIndex = params?.pageIndex ?? 1
      return Promise.resolve({
        data:
          pageIndex === 1
            ? {
                pageIndex: 1,
                pageSize: 1,
                total: 2,
                rows: [
                  {
                    id: 'BED-PAGE-1',
                    name: '01床',
                    parent_id: 'WARD-1',
                    form_enum: 'BED',
                    status_enum: 'ENABLE',
                    organization_id: 'ORG-1'
                  }
                ]
              }
            : {
                pageIndex: 2,
                pageSize: 1,
                total: 2,
                rows: [
                  {
                    id: 'BED-PAGE-2',
                    name: '02床',
                    parent_id: 'WARD-1',
                    form_enum: 'BED',
                    status_enum: 'ENABLE',
                    organization_id: 'ORG-1'
                  }
                ]
              }
      })
    })

    const result = await realQueryNurseStationBedBoard({ panel: 'ADMISSION' })

    expect(result.items.map((item) => item.bed.bedId)).toEqual(['BED-PAGE-1', 'BED-PAGE-2'])
    expect(
      httpMock.get.mock.calls
        .filter(([url]) => url === '/bed')
        .map(([, params]) => params.pageIndex)
    ).toEqual([1, 2])
  })

  it('合并真实占床和空床，并以占床患者覆盖同床位空床', async () => {
    httpMock.get.mockImplementation((url: string) => {
      if (url === '/encountermanage/query-patient-queue') {
        return Promise.resolve({
          data: { pageIndex: 1, pageSize: 100, total: 1, pages: 1, rows: [patientRow] }
        })
      }
      return Promise.resolve({
        data: {
          pageIndex: 1,
          pageSize: 100,
          total: 2,
          pages: 1,
          rows: [
            {
              id: 'BED-1',
              name: '01床',
              parent_id: 'WARD-1',
              form_enum: 'BED',
              status_enum: 'ENABLE',
              organization_id: 'ORG-1'
            },
            {
              id: 'BED-2',
              name: '02床',
              parent_id: 'WARD-1',
              form_enum: 'BED',
              status_enum: 'ENABLE',
              organization_id: 'ORG-1'
            }
          ]
        }
      })
    })

    const result = await realQueryNurseStationBedBoard({ panel: 'ADMISSION' })

    expect(result.items).toHaveLength(2)
    expect(result.items.find((item) => item.bed.bedId === 'BED-1')?.occupant?.patientName).toBe(
      '测试患者'
    )
    expect(result.items.find((item) => item.bed.bedId === 'BED-2')?.bed.houseName).toBe(
      '未提供病房'
    )
  })

  it('多病区筛选时分别查询空床，并保留缺失病房字段的前端兼容分组', async () => {
    httpMock.get.mockImplementation((url: string, params?: { parent_id?: string }) => {
      if (url === '/encountermanage/query-patient-queue') {
        return Promise.resolve({ data: { total: 0, rows: [] } })
      }
      return Promise.resolve({
        data: {
          total: 1,
          rows: [
            {
              id: `BED-${params?.parent_id}`,
              name: '01床',
              parent_id: params?.parent_id,
              form_enum: 'BED',
              status_enum: 'ENABLE',
              organization_id: 'ORG-1'
            }
          ]
        }
      })
    })

    const result = await realQueryNurseStationBedBoard({
      panel: 'ADMISSION',
      wardIds: 'WARD-1,WARD-2'
    })

    const bedCalls = httpMock.get.mock.calls.filter(([url]) => url === '/bed') as Array<
      [string, { parent_id?: string }]
    >
    expect(bedCalls.map(([, params]) => params.parent_id)).toEqual(['WARD-1', 'WARD-2'])
    expect(result.items.map((item) => item.bed.bedId)).toEqual(['BED-WARD-1', 'BED-WARD-2'])
  })

  it('床位预约把患者主档和 encounterId 映射到后端接口字段', async () => {
    httpMock.post.mockResolvedValue({ code: 10000, message: 'success', data: 'RES-001' })

    const result = await realAssignNurseStationBed({
      encounterId: 'encounter-001',
      patientId: 'patient-001',
      intendedAdmissionDate: '2026-08-08',
      targetBedId: 'BED-1'
    })

    expect(httpMock.post).toHaveBeenCalledWith(
      '/bed/assign',
      {
        patient_id: 'patient-001',
        location_id: 'BED-1',
        appointment_id: 'encounter-001',
        intended_admission_date: '2026-08-08'
      },
      expect.objectContaining({ baseURL: expect.any(String) })
    )
    expect(result).toEqual({
      encounterId: 'encounter-001',
      reservationId: 'RES-001',
      succeeded: true
    })
  })

  it('床位预约缺少后端预约 ID 时不报告成功', async () => {
    httpMock.post.mockResolvedValue({ code: 10000, message: '未返回预约 ID' })

    await expect(
      realAssignNurseStationBed({
        encounterId: 'encounter-001',
        patientId: 'patient-001',
        targetBedId: 'BED-1'
      })
    ).rejects.toThrow('未返回预约 ID')
  })

  it('真实患者接口失败时直接抛错，不回退到伪造 Mock 患者', async () => {
    httpMock.get.mockRejectedValueOnce(new Error('患者队列请求失败'))

    await expect(realQueryNurseStationBedBoard({ panel: 'ADMISSION' })).rejects.toThrow(
      '患者队列请求失败'
    )
    expect(httpMock.get).toHaveBeenCalledTimes(1)
  })

  it('办理出院和清床正常转换统一响应', async () => {
    httpMock.post
      .mockResolvedValueOnce({
        code: 10000,
        message: 'success',
        data: { encounterId: 'encounter-001', succeeded: true, targetStatusEnum: 'PENDING' }
      })
      .mockResolvedValueOnce({
        code: 10000,
        message: 'success',
        data: { encounterId: 'encounter-001', succeeded: true, bedReleased: true }
      })

    await expect(realExecuteInpatientDischarge({ encounterId: 'encounter-001' })).resolves.toEqual({
      encounterId: 'encounter-001',
      succeeded: true
    })
    await expect(realClearDischargedBed({ encounterId: 'encounter-001' })).resolves.toEqual({
      encounterId: 'encounter-001',
      succeeded: true
    })
  })

  it('状态转换结果为失败时向页面抛出业务错误', async () => {
    httpMock.post.mockResolvedValue({
      code: 9999,
      message: '仍有未完成医嘱',
      data: { encounterId: 'encounter-001', succeeded: false }
    })

    await expect(realExecuteInpatientDischarge({ encounterId: 'encounter-001' })).rejects.toThrow(
      '仍有未完成医嘱'
    )
  })
})
