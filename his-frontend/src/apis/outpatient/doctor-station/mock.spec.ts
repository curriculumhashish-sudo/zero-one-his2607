import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  mockAcceptPatient,
  mockCompletePatient,
  mockGetAdviceOrders,
  mockGetDiagnoses,
  mockGetMedicalRecord,
  mockGetMedicalRecordTemplate,
  mockGetPatientContext,
  mockListMedicalRecordTemplates,
  mockLeavePatient,
  mockPageAdviceOrderCatalog,
  mockPageDiagnosisDictionary,
  mockPageDoctorPatientQueue,
  mockSaveDiagnoses,
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

  it('加载患者上下文，并将候诊患者接诊为在诊', async () => {
    const encounterId = 'enc-20260723-001'

    const beforeAccept = await resolveMockRequest(mockGetPatientContext(encounterId))
    expect(beforeAccept.visitStatus).toBe('WAITING')

    const accepted = await resolveMockRequest(mockAcceptPatient(encounterId))
    expect(accepted.visitStatus).toBe('IN_PROGRESS')
    expect(accepted.visitStatusName).toBe('在诊')
    expect(accepted.attendingDoctorName).toBe('李尔王')
    expect(accepted.acceptedAt).toBeTruthy()

    const inProgressPage = await resolveMockRequest(
      mockPageDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 50,
        visitStatus: 'IN_PROGRESS'
      })
    )

    expect(inProgressPage.rows?.some((patient) => patient.encounterId === encounterId)).toBe(true)
  })

  it('暂离后将患者退回候诊队列，并隐藏本轮接诊信息', async () => {
    const encounterId = 'enc-20260723-002'
    await resolveMockRequest(mockAcceptPatient(encounterId))

    const left = await resolveMockRequest(mockLeavePatient(encounterId))
    expect(left.visitStatus).toBe('WAITING')
    expect(left.visitStatusName).toBe('候诊')
    expect(left.attendingDoctorName).toBeUndefined()
    expect(left.acceptedAt).toBeUndefined()

    const waitingPage = await resolveMockRequest(
      mockPageDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 50,
        visitStatus: 'WAITING'
      })
    )
    expect(waitingPage.rows?.some((patient) => patient.encounterId === encounterId)).toBe(true)
  })

  it('加载并保存门诊病历，同时递增版本号', async () => {
    const encounterId = 'enc-20260723-008'
    const beforeSave = await resolveMockRequest(mockGetMedicalRecord(encounterId))
    const changedContent = `${beforeSave.content}<p>补充：患者一般情况稳定。</p>`

    const saved = await resolveMockRequest(
      mockSaveMedicalRecord(encounterId, {
        content: changedContent,
        version: beforeSave.version
      })
    )

    expect(saved.content).toBe(changedContent)
    expect(saved.version).toBe(beforeSave.version + 1)
    expect(saved.updatedAt).toBeTruthy()

    const afterSave = await resolveMockRequest(mockGetMedicalRecord(encounterId))
    expect(afterSave).toEqual(saved)
  })

  it('拒绝使用过期版本覆盖病历', async () => {
    const encounterId = 'enc-20260723-009'
    const current = await resolveMockRequest(mockGetMedicalRecord(encounterId))

    // 先把 rejects 断言绑定到 Promise，再推进假计时器。
    // 否则 Promise 可能先被拒绝，Vitest 会把它误判为“未处理的异步异常”。
    const saveRequest = mockSaveMedicalRecord(encounterId, {
      content: '<p>错误版本提交</p>',
      version: current.version - 1
    })
    const rejection = expect(saveRequest).rejects.toThrow('病历已被其他操作修改')

    await vi.runAllTimersAsync()
    await rejection
  })

  it('返回模板树，并按模板 ID 加载正文', async () => {
    const templateTree = await resolveMockRequest(mockListMedicalRecordTemplates())
    const templateLeaves = templateTree.flatMap((category) => category.children ?? [])

    expect(templateLeaves).toHaveLength(6)

    const template = await resolveMockRequest(mockGetMedicalRecordTemplate('template-hypertension'))
    expect(template.name).toBe('高血压随访')
    expect(template.content).toContain('家庭血压')
  })

  it('搜索诊断字典并保存唯一主诊断', async () => {
    const dictionaryPage = await resolveMockRequest(
      mockPageDiagnosisDictionary({
        pageIndex: 1,
        pageSize: 10,
        keyword: '高血压'
      })
    )
    expect(dictionaryPage.rows?.[0]?.code).toBe('I10')

    const encounterId = 'enc-20260723-008'
    const beforeSave = await resolveMockRequest(mockGetDiagnoses(encounterId))
    const reversedRows = beforeSave.rows.slice().reverse()
    const saved = await resolveMockRequest(
      mockSaveDiagnoses(encounterId, {
        version: beforeSave.version,
        items: reversedRows.map((item, index) => ({
          id: item.id,
          catalogId: item.catalogId,
          code: item.code,
          name: item.name,
          typeName: item.typeName,
          primary: index === 0,
          orderNo: index + 1
        }))
      })
    )

    expect(saved.version).toBe(beforeSave.version + 1)
    expect(saved.rows[0]?.catalogId).toBe(reversedRows[0]?.catalogId)
    expect(saved.rows.filter((item) => item.primary)).toHaveLength(1)
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
    const saved = await resolveMockRequest(
      mockSaveAdviceOrders(encounterId, {
        version: beforeSave.version,
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
          quantity: index === 0 ? item.quantity + 1 : item.quantity,
          quantityUnit: item.quantityUnit,
          remark: item.remark,
          orderNo: index + 1
        }))
      })
    )

    expect(saved.version).toBe(beforeSave.version + 1)
    expect(saved.rows[0]?.quantity).toBe((beforeSave.rows[0]?.quantity ?? 0) + 1)
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
    expect(completed.visitStatus).toBe('COMPLETED')
    expect(completed.visitStatusName).toBe('完诊')

    const completedPage = await resolveMockRequest(
      mockPageDoctorPatientQueue({
        pageIndex: 1,
        pageSize: 50,
        visitStatus: 'COMPLETED'
      })
    )
    expect(completedPage.rows?.some((patient) => patient.encounterId === encounterId)).toBe(true)

    const saveRequest = mockSaveMedicalRecord(encounterId, {
      content: recordBeforeComplete.content + '<p>不应允许的修改</p>',
      version: recordBeforeComplete.version
    })
    const rejection = expect(saveRequest).rejects.toThrow('患者已完诊')
    await vi.runAllTimersAsync()
    await rejection
  })
})
