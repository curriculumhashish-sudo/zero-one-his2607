import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  mockCancelStopInpatientMedicalOrders,
  mockCreateInpatientDiagnosis,
  mockCreateInpatientDischargeAdvice,
  mockCreateInpatientMedicalOrder,
  mockGetInpatientMedicalRecord,
  mockGetInpatientMedicalRecordHistoryDetail,
  mockListInpatientDiagnosisBrief,
  mockListInpatientMedicalRecordDocumentTypes,
  mockPageInpatientDoctorPatientQueue,
  mockPageInpatientDiagnosisRecords,
  mockPageInpatientMedicalOrderCatalog,
  mockPageInpatientMedicalOrders,
  mockPageInpatientMedicalRecordHistory,
  mockSaveInpatientMedicalRecord,
  mockSignInpatientMedicalRecord,
  mockSignInpatientMedicalOrders,
  mockStopInpatientMedicalOrders,
  mockUpdateInpatientDiagnosis,
  mockDeleteInpatientDiagnosis,
  mockWithdrawInpatientMedicalOrders
} from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

async function expectMockRequestToReject(request: Promise<unknown>, message: string) {
  const expectation = expect(request).rejects.toThrow(message)
  await vi.runAllTimersAsync()
  await expectation
}

describe('inpatient doctor-station patient queue mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('按住院号和床号检索患者', async () => {
    const byInpatientNo = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: 'ZY20260702006'
      })
    )
    expect(byInpatientNo.rows?.map((item) => item.patientName)).toEqual(['赵桂芳'])

    const byBed = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: '12床'
      })
    )
    expect(byBed.rows?.[0]?.patientName).toBe('赵桂芳')
  })

  it('返回稳定的分页结果且不暴露 Mock 内部对象', async () => {
    const firstPage = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 2
      })
    )
    expect(firstPage.total).toBe(3)
    expect(firstPage.pages).toBe(2)
    expect(firstPage.rows).toHaveLength(2)

    if (firstPage.rows?.[0]) firstPage.rows[0].patientName = '被修改'

    const reloaded = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 2
      })
    )
    expect(reloaded.rows?.[0]?.patientName).toBe('王建国')
  })

  it('查询患者跨住院历史并优先展示当前住院文书', async () => {
    const history = await resolveMockRequest(
      mockPageInpatientMedicalRecordHistory({
        pageIndex: 1,
        pageSize: 10,
        patientId: 'patient-10001',
        currentEncounterId: 'ipt-enc-20260701-001'
      })
    )

    expect(history.total).toBe(3)
    expect(history.rows?.[0]?.encounterId).toBe('ipt-enc-20260701-001')
    expect(history.rows?.some((record) => record.inpatientNo === 'ZY20260112021')).toBe(true)
  })

  it('按关键词筛选历史病历并加载只读详情', async () => {
    const history = await resolveMockRequest(
      mockPageInpatientMedicalRecordHistory({
        pageIndex: 1,
        pageSize: 10,
        patientId: 'patient-10001',
        keyword: '首次病程'
      })
    )
    expect(history.rows?.map((record) => record.documentTypeCode)).toEqual(['PROGRESS_NOTE'])

    const detail = await resolveMockRequest(
      mockGetInpatientMedicalRecordHistoryDetail('ipt-record-10001-progress-01')
    )
    expect(detail.readonly).toBe(true)
    expect(detail.content).toContain('诊疗计划')
  })

  it('支持新建、更新和签名住院病历，并使用版本号控制并发', async () => {
    const documentTypes = await resolveMockRequest(mockListInpatientMedicalRecordDocumentTypes())
    expect(documentTypes.map((item) => item.value)).toEqual([
      'ADMISSION_RECORD',
      'FIRST_COURSE_RECORD',
      'DAILY_COURSE_RECORD',
      'DISCHARGE_RECORD'
    ])

    const created = await resolveMockRequest(
      mockSaveInpatientMedicalRecord({
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002',
        documentTypeCode: 'DAILY_COURSE_RECORD',
        title: '2026-07-28 日常病程记录',
        content: '<p>患者今日胸闷较前缓解，继续观察。</p>'
      })
    )
    expect(created).toEqual(
      expect.objectContaining({
        status: 'DRAFT',
        statusName: '草稿',
        readonly: false,
        version: 1
      })
    )

    const loaded = await resolveMockRequest(mockGetInpatientMedicalRecord(created.id))
    expect(loaded.readonly).toBe(false)

    const updated = await resolveMockRequest(
      mockSaveInpatientMedicalRecord({
        id: created.id,
        encounterId: created.encounterId,
        patientId: created.patientId,
        documentTypeCode: 'DAILY_COURSE_RECORD',
        title: created.title,
        content: '<p>患者今日胸闷较前明显缓解，继续观察。</p>',
        version: created.version
      })
    )
    expect(updated.version).toBe(2)

    await expectMockRequestToReject(
      mockSaveInpatientMedicalRecord({
        id: created.id,
        encounterId: created.encounterId,
        patientId: created.patientId,
        documentTypeCode: 'DAILY_COURSE_RECORD',
        title: created.title,
        content: created.content,
        version: created.version
      }),
      '已被他人修改'
    )

    const signed = await resolveMockRequest(
      mockSignInpatientMedicalRecord({
        id: updated.id,
        version: updated.version
      })
    )
    expect(signed).toEqual(
      expect.objectContaining({
        status: 'SIGNED',
        statusName: '已签名',
        readonly: true,
        version: 3
      })
    )

    await expectMockRequestToReject(
      mockSaveInpatientMedicalRecord({
        id: signed.id,
        encounterId: signed.encounterId,
        patientId: signed.patientId,
        documentTypeCode: 'DAILY_COURSE_RECORD',
        title: signed.title,
        content: signed.content,
        version: signed.version
      }),
      '不能修改'
    )
  })

  it('搜索医嘱目录并分页查询当前住院医嘱', async () => {
    const catalog = await resolveMockRequest(
      mockPageInpatientMedicalOrderCatalog({
        pageIndex: 1,
        pageSize: 10,
        keyword: '血常规'
      })
    )
    expect(catalog.rows?.[0]).toEqual(expect.objectContaining({ name: '血常规', category: 'LAB' }))

    const records = await resolveMockRequest(
      mockPageInpatientMedicalOrders({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260701-001'
      })
    )
    expect(records.total).toBeGreaterThan(0)
    expect(records.rows?.every((record) => record.encounterId === 'ipt-enc-20260701-001')).toBe(
      true
    )
  })

  it('诊断 Mock 支持患者隔离和完整增删改流程', async () => {
    const before = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002'
      })
    )
    expect(before.rows).toHaveLength(1)

    const created = await resolveMockRequest(
      mockCreateInpatientDiagnosis({
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002',
        description: '高脂血症',
        primaryFlag: 0,
        rank: 2,
        tcmFlag: 0
      })
    )
    expect(created).toEqual(expect.objectContaining({ description: '高脂血症', rank: 2 }))

    await resolveMockRequest(
      mockUpdateInpatientDiagnosis({ id: created.id, diagnosisTypeCode: '出院诊断' })
    )
    const updated = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002'
      })
    )
    expect(updated.rows?.find((item) => item.id === created.id)?.diagnosisTypeName).toBe('出院诊断')

    const otherPatient = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260701-001',
        patientId: 'patient-10001'
      })
    )
    expect(otherPatient.rows?.some((item) => item.id === created.id)).toBe(false)

    const briefs = await resolveMockRequest(mockListInpatientDiagnosisBrief())
    expect(briefs).toContainEqual({ definitionName: '高脂血症' })

    await resolveMockRequest(
      mockDeleteInpatientDiagnosis({ id: created.id, conditionId: created.conditionId })
    )
    const after = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002'
      })
    )
    expect(after.rows?.some((item) => item.id === created.id)).toBe(false)
  })

  it('支持住院医嘱新增、签发、停嘱、取消停嘱和撤回', async () => {
    const created = await resolveMockRequest(
      mockCreateInpatientMedicalOrder({
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002',
        catalogId: 'order-catalog-test',
        code: 'TEST-ORDER-001',
        name: '测试长期医嘱',
        category: 'TREATMENT',
        categoryName: '治疗',
        quantity: 1,
        quantityUnit: '次',
        therapyType: 'LONG_TERM',
        orderNo: 99
      })
    )
    expect(created.status).toBe('DRAFT')

    const signed = await resolveMockRequest(
      mockSignInpatientMedicalOrders({
        encounterId: created.encounterId,
        patientId: created.patientId,
        items: [{ id: created.id, version: created.version }]
      })
    )
    expect(signed.rows[0]?.status).toBe('SIGNED')

    const stopped = await resolveMockRequest(
      mockStopInpatientMedicalOrders({
        encounterId: created.encounterId,
        patientId: created.patientId,
        items: [{ id: created.id, version: signed.rows[0]!.version }]
      })
    )
    expect(stopped.rows[0]?.status).toBe('STOPPED')

    const resumed = await resolveMockRequest(
      mockCancelStopInpatientMedicalOrders({
        encounterId: created.encounterId,
        patientId: created.patientId,
        items: [{ id: created.id, version: stopped.rows[0]!.version }]
      })
    )
    expect(resumed.rows[0]?.status).toBe('SIGNED')

    const withdrawn = await resolveMockRequest(
      mockWithdrawInpatientMedicalOrders({
        encounterId: created.encounterId,
        patientId: created.patientId,
        items: [{ id: created.id, version: resumed.rows[0]!.version }]
      })
    )
    expect(withdrawn.rows[0]?.status).toBe('WITHDRAWN')
    expect(withdrawn.rows[0]?.version).toBe(created.version + 4)
  })

  it('开立出院医嘱后患者移出在院队列', async () => {
    const advice = await resolveMockRequest(
      mockCreateInpatientDischargeAdvice({
        encounterId: 'ipt-enc-20260702-002',
        plannedDischargeTime: '2026-07-28 16:00:00',
        outWayCode: '1'
      })
    )
    expect(advice).toEqual(
      expect.objectContaining({
        outWayName: '医嘱离院',
        plannedDischargeTime: '2026-07-28 16:00:00',
        statusEnum: 'ACTIVE',
        encounterNewStatus: 'ON_HOLD'
      })
    )

    const queue = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: 'ZY20260702006'
      })
    )
    expect(queue.rows).toEqual([])

    const orders = await resolveMockRequest(
      mockPageInpatientMedicalOrders({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260702-002'
      })
    )
    expect(orders.rows).toEqual(
      expect.arrayContaining([
        expect.objectContaining({
          code: 'DISCHARGE',
          categoryName: '出院医嘱',
          status: 'SIGNED'
        })
      ])
    )
  })

  it('已存在出院医嘱时拒绝重复提交', async () => {
    await resolveMockRequest(
      mockCreateInpatientDischargeAdvice({
        encounterId: 'ipt-enc-20260703-003',
        plannedDischargeTime: '2026-07-29 09:00:00',
        outWayCode: '1'
      })
    )
    await expectMockRequestToReject(
      mockCreateInpatientDischargeAdvice({
        encounterId: 'ipt-enc-20260703-003',
        plannedDischargeTime: '2026-07-29 09:00:00',
        outWayCode: '1'
      }),
      '不能重复开立'
    )
  })
})
