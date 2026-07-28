import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  mockCancelStopInpatientMedicalOrders,
  mockCreateInpatientDischargeAdvice,
  mockCreateInpatientDiagnosis,
  mockCreateInpatientMedicalOrder,
  mockDeleteInpatientDiagnosis,
  mockGetInpatientDiagnosisUsageHistory,
  mockGetInpatientDischargeAdvicePreparation,
  mockGetInpatientMedicalRecord,
  mockGetInpatientMedicalRecordHistoryDetail,
  mockListInpatientMedicalRecordDocumentTypes,
  mockPageInpatientDiagnosisDictionary,
  mockPageInpatientDiagnosisRecords,
  mockPageInpatientDoctorPatientQueue,
  mockPageInpatientMedicalOrderCatalog,
  mockPageInpatientMedicalOrders,
  mockPageInpatientMedicalRecordHistory,
  mockSaveInpatientMedicalRecord,
  mockSignInpatientMedicalRecord,
  mockSignInpatientMedicalOrders,
  mockStopInpatientMedicalOrders,
  mockUpdateInpatientDiagnosis,
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

  it('按住院号、床号和拼音简码检索患者', async () => {
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
        keyword: '18床'
      })
    )
    expect(byBed.rows?.[0]?.patientName).toBe('刘春梅')

    const byPinyin = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: 'HXL'
      })
    )
    expect(byPinyin.rows?.[0]?.patientName).toBe('何秀兰')
  })

  it('返回稳定的分页结果且不暴露 Mock 内部对象', async () => {
    const firstPage = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 2
      })
    )
    expect(firstPage.total).toBe(6)
    expect(firstPage.pages).toBe(3)
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
        encounterId: 'ipt-enc-20260705-005',
        patientId: 'patient-10005',
        documentTypeCode: 'DAILY_COURSE_RECORD',
        title: '2026-07-28 日常病程记录',
        content: '<p>患者今日胸闷较前缓解，继续观察。</p>',
        version: 0
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

  it('分页查询当前住院诊断并搜索诊断字典和使用历史', async () => {
    const records = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260701-001'
      })
    )
    expect(records.rows?.map((record) => record.code)).toEqual(['I25.1', 'E78.5'])

    const dictionary = await resolveMockRequest(
      mockPageInpatientDiagnosisDictionary({
        pageIndex: 1,
        pageSize: 10,
        keyword: '胸痛'
      })
    )
    expect(dictionary.rows?.[0]?.code).toBe('R07.4')

    const history = await resolveMockRequest(
      mockGetInpatientDiagnosisUsageHistory({ keyword: '高血压' })
    )
    expect(history.map((item) => item.code)).toEqual(['I10'])
  })

  it('支持住院诊断新增、修改、删除并校验版本号', async () => {
    const created = await resolveMockRequest(
      mockCreateInpatientDiagnosis({
        encounterId: 'ipt-enc-20260702-002',
        patientId: 'patient-10002',
        catalogId: 'diag-R002',
        code: 'R00.2',
        name: '心悸',
        typeName: '症状诊断',
        primary: false,
        orderNo: 2
      })
    )
    expect(created.version).toBe(1)

    const updated = await resolveMockRequest(
      mockUpdateInpatientDiagnosis(created.id, {
        catalogId: created.catalogId,
        code: created.code,
        name: created.name,
        typeName: created.typeName,
        primary: true,
        orderNo: 1,
        version: created.version
      })
    )
    expect(updated.primary).toBe(true)
    expect(updated.version).toBe(2)

    await expectMockRequestToReject(
      mockUpdateInpatientDiagnosis(created.id, {
        catalogId: created.catalogId,
        code: created.code,
        name: created.name,
        typeName: created.typeName,
        primary: false,
        orderNo: 2,
        version: 1
      }),
      '已被他人修改'
    )

    await resolveMockRequest(mockDeleteInpatientDiagnosis(created.id, updated.version))
    const records = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260702-002'
      })
    )
    expect(records.rows?.some((record) => record.id === created.id)).toBe(false)
  })

  it('待出院患者仍在医生站队列时可以维护诊断', async () => {
    const records = await resolveMockRequest(
      mockPageInpatientDiagnosisRecords({
        pageIndex: 1,
        pageSize: 20,
        encounterId: 'ipt-enc-20260703-003'
      })
    )
    const record = records.rows?.[0]
    expect(record).toBeTruthy()

    const updated = await resolveMockRequest(
      mockUpdateInpatientDiagnosis(record!.id, {
        catalogId: record!.catalogId,
        code: record!.code,
        name: record!.name,
        typeName: record!.typeName,
        primary: record!.primary,
        orderNo: record!.orderNo,
        version: record!.version
      })
    )
    expect(updated.version).toBe(record!.version + 1)
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

  it('出院医嘱前置校验会阻止重复开立并提示待处理医嘱', async () => {
    const ready = await resolveMockRequest(
      mockGetInpatientDischargeAdvicePreparation('ipt-enc-20260701-001', 'patient-10001')
    )
    expect(ready.canCreate).toBe(true)
    expect(ready.checks).toEqual(
      expect.arrayContaining([
        expect.objectContaining({ code: 'ORDER_DRAFT', level: 'REMINDER' }),
        expect.objectContaining({ code: 'LONG_TERM_ORDER_ACTIVE', level: 'REMINDER' })
      ])
    )
    expect(ready.outWayOptions).toEqual(
      expect.arrayContaining([expect.objectContaining({ value: '1', label: '医嘱离院' })])
    )

    const pending = await resolveMockRequest(
      mockGetInpatientDischargeAdvicePreparation('ipt-enc-20260703-003', 'patient-10003')
    )
    expect(pending.canCreate).toBe(false)
    expect(pending.existingAdvice?.outWayName).toBe('医嘱离院')
    expect(pending.checks[0]).toEqual(
      expect.objectContaining({ code: 'DISCHARGE_ALREADY_PENDING', level: 'BLOCKER' })
    )
  })

  it('开立出院医嘱后患者保留在队列并更新为待出院状态', async () => {
    const preparation = await resolveMockRequest(
      mockGetInpatientDischargeAdvicePreparation('ipt-enc-20260704-004', 'patient-10004')
    )
    const advice = await resolveMockRequest(
      mockCreateInpatientDischargeAdvice({
        encounterId: preparation.encounterId,
        patientId: preparation.patientId,
        encounterVersion: preparation.encounterVersion,
        plannedDischargeTime: '2026-07-28 16:00:00',
        outWayCode: '1',
        reasonText: '病情稳定'
      })
    )
    expect(advice).toEqual(
      expect.objectContaining({
        outWayName: '医嘱离院',
        plannedDischargeTime: '2026-07-28 16:00:00'
      })
    )

    const queue = await resolveMockRequest(
      mockPageInpatientDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 20,
        keyword: 'ZY20260704009'
      })
    )
    expect(queue.rows?.[0]).toEqual(
      expect.objectContaining({
        encounterStatus: 'DISCHARGE_PENDING',
        encounterStatusName: '待出院',
        version: preparation.encounterVersion + 1
      })
    )

    const orders = await resolveMockRequest(
      mockPageInpatientMedicalOrders({
        pageIndex: 1,
        pageSize: 20,
        encounterId: preparation.encounterId
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

  it('出院医嘱使用住院版本号控制并发且不能重复提交', async () => {
    const preparation = await resolveMockRequest(
      mockGetInpatientDischargeAdvicePreparation('ipt-enc-20260706-006', 'patient-10006')
    )
    await expectMockRequestToReject(
      mockCreateInpatientDischargeAdvice({
        encounterId: preparation.encounterId,
        patientId: preparation.patientId,
        encounterVersion: preparation.encounterVersion + 1,
        plannedDischargeTime: '2026-07-29 09:00:00',
        outWayCode: '1'
      }),
      '已被他人修改'
    )

    await expectMockRequestToReject(
      mockCreateInpatientDischargeAdvice({
        encounterId: 'ipt-enc-20260703-003',
        patientId: 'patient-10003',
        encounterVersion: 2,
        plannedDischargeTime: '2026-07-29 09:00:00',
        outWayCode: '1'
      }),
      '不能重复开立'
    )
  })
})
