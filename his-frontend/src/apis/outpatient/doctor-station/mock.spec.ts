import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import dayjs from 'dayjs'

import {
  mockAcceptPatient,
  mockCompletePatient,
  mockCreateDiagnosis,
  mockGetAdviceOrders,
  mockGetMedicalRecord,
  mockLeavePatient,
  mockPageAdviceOrderCatalog,
  mockPageDiagnosisDictionary,
  mockListDoctorPatientQueue,
  mockPagePatientMedicalRecordHistory,
  mockSaveAdviceOrders,
  mockSaveMedicalRecord
} from './mock'

/**
 * 推进 Mock 内部的 setTimeout，并等待接口 Promise 返回。
 */
async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

describe('doctor-station mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('通过统一入口将候诊患者接诊为在诊', async () => {
    const encounterId = 'enc-20260723-001'

    const accepted = await resolveMockRequest(mockAcceptPatient(encounterId))
    expect(accepted.visitStatus).toBe('in-progress')
    expect(accepted.visitStatusName).toBe('在诊')
    expect(accepted.attendingDoctorName).toBe('李尔王')
    expect(accepted.acceptedAt).toBeTruthy()

    const inProgressPatients = await resolveMockRequest(
      mockListDoctorPatientQueue({
        visitDate: dayjs().format('YYYY-MM-DD'),
        visitStatus: 'in-progress'
      })
    )

    expect(inProgressPatients.some((patient) => patient.encounterId === encounterId)).toBe(true)
  })

  it('通过统一入口读取在诊和完诊患者时不改变状态', async () => {
    const inProgress = await resolveMockRequest(mockAcceptPatient('enc-20260723-003'))
    const finished = await resolveMockRequest(mockAcceptPatient('enc-20260723-005'))

    expect(inProgress.visitStatus).toBe('in-progress')
    expect(finished.visitStatus).toBe('finished')
    expect(finished.visitStatusName).toBe('完诊')
  })

  it('患者队列按单日返回完整列表且不包含内部测试夹具', async () => {
    const today = dayjs().format('YYYY-MM-DD')
    const yesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')
    const todayPatients = await resolveMockRequest(mockListDoctorPatientQueue({ visitDate: today }))
    const yesterdayPatients = await resolveMockRequest(
      mockListDoctorPatientQueue({ visitDate: yesterday })
    )

    expect(todayPatients.length).toBeGreaterThan(0)
    expect(yesterdayPatients.length).toBeGreaterThan(0)
    expect(todayPatients.every((item) => item.visitTime.startsWith(today))).toBe(true)
    expect(yesterdayPatients.every((item) => item.visitTime.startsWith(yesterday))).toBe(true)
    expect(
      [...todayPatients, ...yesterdayPatients].some((item) =>
        item.patientName.startsWith('测试患者')
      )
    ).toBe(false)
  })

  it('暂离后将患者退回候诊队列，并隐藏本轮接诊信息', async () => {
    const encounterId = 'enc-20260723-002'
    await resolveMockRequest(mockAcceptPatient(encounterId))

    const left = await resolveMockRequest(mockLeavePatient(encounterId))
    expect(left.visitStatus).toBe('arrived')
    expect(left.visitStatusName).toBe('候诊')
    expect(left.attendingDoctorName).toBeUndefined()
    expect(left.acceptedAt).toBeUndefined()

    const waitingPatients = await resolveMockRequest(
      mockListDoctorPatientQueue({
        visitDate: dayjs().format('YYYY-MM-DD'),
        visitStatus: 'arrived'
      })
    )
    expect(waitingPatients.some((patient) => patient.encounterId === encounterId)).toBe(true)
  })

  it('加载并保存门诊病历', async () => {
    const encounterId = 'enc-20260723-008'
    const beforeSave = await resolveMockRequest(mockGetMedicalRecord(encounterId))
    const changedContent = `${beforeSave.content}<p>补充：患者一般情况稳定。</p>`

    const saved = await resolveMockRequest(
      mockSaveMedicalRecord(encounterId, {
        content: changedContent
      })
    )

    expect(saved.content).toBe(changedContent)
    expect(saved.updatedAt).toBeTruthy()

    const afterSave = await resolveMockRequest(mockGetMedicalRecord(encounterId))
    expect(afterSave).toEqual(saved)
  })

  it('按患者分页查询历史病历，并按就诊时间倒序返回', async () => {
    const firstPage = await resolveMockRequest(
      mockPagePatientMedicalRecordHistory({
        patientId: 'patient-1003',
        pageIndex: 1,
        pageSize: 10
      })
    )
    const secondPage = await resolveMockRequest(
      mockPagePatientMedicalRecordHistory({
        patientId: 'patient-1003',
        pageIndex: 2,
        pageSize: 10
      })
    )

    expect(firstPage.total).toBe(12)
    expect(firstPage.rows).toHaveLength(10)
    expect(secondPage.rows).toHaveLength(2)
    expect((firstPage.rows?.[0]?.visitDate ?? '') >= (firstPage.rows?.[1]?.visitDate ?? '')).toBe(
      true
    )
  })

  it('历史病历支持科室和主诊断关键词，并可按需读取只读正文', async () => {
    const result = await resolveMockRequest(
      mockPagePatientMedicalRecordHistory({
        patientId: 'patient-1008',
        keyword: '糖尿病',
        pageIndex: 1,
        pageSize: 10
      })
    )

    expect(result.total).toBe(1)
    expect(result.rows?.[0]?.deptName).toBe('内分泌科')

    const detail = await resolveMockRequest(
      mockGetMedicalRecord(result.rows?.[0]?.encounterId ?? '')
    )
    expect(detail.id).toBeTruthy()
    expect(detail.content).toContain('2 型糖尿病')
  })

  it('历史病历查询按患者隔离数据', async () => {
    const result = await resolveMockRequest(
      mockPagePatientMedicalRecordHistory({
        patientId: 'patient-without-history',
        pageIndex: 1,
        pageSize: 10
      })
    )

    expect(result.total).toBe(0)
    expect(result.rows).toEqual([])
  })

  it('搜索诊断字典并单条新增诊断', async () => {
    const dictionaryPage = await resolveMockRequest(
      mockPageDiagnosisDictionary({
        pageIndex: 1,
        pageSize: 10,
        keyword: '高血压'
      })
    )
    expect(dictionaryPage.rows?.[0]?.code).toBe('I10')

    const encounterId = 'enc-20260723-008'
    const created = await resolveMockRequest(
      mockCreateDiagnosis({
        encounterId,
        conditionId: 'diag-extra-hypertension',
        diagCode: 'I10',
        description: '原发性高血压',
        diagTypeCode: '西医诊断',
        primaryFlag: 0,
        rank: 3
      })
    )

    expect(created.rows.some((item) => item.catalogId === 'diag-extra-hypertension')).toBe(true)
    expect(created.rows.filter((item) => item.primary)).toHaveLength(1)
  })

  it('搜索医嘱项目并保存门诊医嘱', async () => {
    const catalogPage = await resolveMockRequest(
      mockPageAdviceOrderCatalog({
        pageIndex: 1,
        pageSize: 10,
        keyword: '阿司匹林'
      })
    )
    expect(catalogPage.rows?.[0]?.category).toBe('DRUG')

    const encounterId = 'enc-20260723-008'
    const beforeSave = await resolveMockRequest(mockGetAdviceOrders(encounterId))
    const draftBeforeSave = beforeSave.rows.find((item) => item.status === 'DRAFT')!
    const saved = await resolveMockRequest(
      mockSaveAdviceOrders(encounterId, {
        items: beforeSave.rows.map((item, index) => ({
          id: item.id,
          catalogId: item.catalogId,
          code: item.code,
          name: item.name,
          category: item.category,
          categoryName: item.categoryName,
          specification: item.specification,
          dose: item.dose,
          unit: item.unit,
          frequency: item.frequency,
          route: item.route,
          days: item.days,
          quantity: item.id === draftBeforeSave.id ? item.quantity + 1 : item.quantity,
          quantityUnit: item.quantityUnit,
          remark: item.remark,
          orderNo: index + 1
        }))
      })
    )

    expect(saved.rows.find((item) => item.id === draftBeforeSave.id)?.quantity).toBe(
      draftBeforeSave.quantity + 1
    )
  })

  it('拒绝未接诊患者直接完诊', async () => {
    const completeRequest = mockCompletePatient('enc-20260723-002')
    const rejection = expect(completeRequest).rejects.toThrow('请先接诊患者')

    await vi.runAllTimersAsync()
    await rejection
  })

  it('完成就诊后进入完诊队列，并禁止继续修改诊疗内容', async () => {
    const encounterId = 'enc-20260723-008'
    const recordBeforeComplete = await resolveMockRequest(mockGetMedicalRecord(encounterId))

    const completed = await resolveMockRequest(mockCompletePatient(encounterId))
    expect(completed.visitStatus).toBe('finished')
    expect(completed.visitStatusName).toBe('完诊')

    const completedPatients = await resolveMockRequest(
      mockListDoctorPatientQueue({
        visitDate: dayjs().format('YYYY-MM-DD'),
        visitStatus: 'finished'
      })
    )
    expect(completedPatients.some((patient) => patient.encounterId === encounterId)).toBe(true)

    const saveRequest = mockSaveMedicalRecord(encounterId, {
      content: recordBeforeComplete.content + '<p>不应允许的修改</p>'
    })
    const rejection = expect(saveRequest).rejects.toThrow('患者已完诊')
    await vi.runAllTimersAsync()
    await rejection
  })
})
