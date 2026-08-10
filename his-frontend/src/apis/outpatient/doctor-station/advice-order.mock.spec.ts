import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import type { DoctorAdviceOrderCatalogItemVO, SaveDoctorAdviceOrderItemDTO } from './type'
import {
  mockCreateAdviceOrder,
  mockDeleteAdviceOrders,
  mockGroupAdviceOrders,
  mockPageAdviceOrderCatalog,
  mockPageAdviceOrders,
  mockPagePatientAdviceOrderHistory,
  mockPagePatientAdviceOrderHistoryDetails,
  mockSignAdviceOrders,
  mockUngroupAdviceOrders,
  mockUpdateAdviceOrder,
  mockWithdrawAdviceOrders
} from './mock'

async function resolveMockRequest<T>(request: Promise<T>) {
  await vi.runAllTimersAsync()
  return request
}

function toSaveItem(
  catalog: DoctorAdviceOrderCatalogItemVO,
  orderNo: number
): SaveDoctorAdviceOrderItemDTO {
  return {
    catalogId: catalog.id,
    code: catalog.code,
    name: catalog.name,
    category: catalog.category,
    categoryName: catalog.categoryName,
    specification: catalog.specification,
    dose: catalog.defaultDose,
    unit: catalog.defaultUnit,
    frequency: catalog.defaultFrequency,
    route: catalog.defaultRoute,
    days: catalog.defaultDays,
    quantity: catalog.defaultQuantity,
    quantityUnit: catalog.quantityUnit,
    dosePerQuantityUnit: catalog.dosePerQuantityUnit,
    orderNo
  }
}

describe('doctor advice order mock workflow', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('按关键词、分类、状态和页码查询当前医嘱', async () => {
    const page = await resolveMockRequest(
      mockPageAdviceOrders({
        encounterId: 'enc-20260723-003',
        pageIndex: 1,
        pageSize: 1,
        category: 'DRUG',
        status: 'DRAFT',
        keyword: '氨氯地平'
      })
    )

    expect(page.total).toBe(1)
    expect(page.rows).toHaveLength(1)
    expect(page.rows?.[0]?.category).toBe('DRUG')
    expect(page.rows?.[0]?.status).toBe('DRAFT')
  })

  it('执行 CRUD、跨分类重新并组、拆组、签发和撤回', async () => {
    const encounterId = 'enc-extra-14'
    const catalogPage = await resolveMockRequest(
      mockPageAdviceOrderCatalog({ pageIndex: 1, pageSize: 20 })
    )
    const drug = catalogPage.rows?.find((item) => item.category === 'DRUG')!
    const lab = catalogPage.rows?.find((item) => item.category === 'LAB')!
    const exam = catalogPage.rows?.find((item) => item.category === 'EXAM')!

    let current = await resolveMockRequest(
      mockCreateAdviceOrder({
        encounterId,
        item: toSaveItem(drug, 1)
      })
    )
    current = await resolveMockRequest(
      mockCreateAdviceOrder({
        encounterId,
        item: toSaveItem(lab, 2)
      })
    )
    current = await resolveMockRequest(
      mockCreateAdviceOrder({
        encounterId,
        item: toSaveItem(exam, 3)
      })
    )

    const drugOrder = current.rows.find((item) => item.catalogId === drug.id)!
    current = await resolveMockRequest(
      mockUpdateAdviceOrder({
        encounterId,
        adviceId: drugOrder.id,
        item: { ...toSaveItem(drug, 1), quantity: drugOrder.quantity + 1 }
      })
    )
    expect(current.rows.find((item) => item.id === drugOrder.id)?.quantity).toBe(
      drugOrder.quantity + 1
    )

    const labOrder = current.rows.find((item) => item.catalogId === lab.id)!
    const examOrder = current.rows.find((item) => item.catalogId === exam.id)!
    current = await resolveMockRequest(
      mockGroupAdviceOrders({
        encounterId,
        adviceIds: [drugOrder.id, labOrder.id],
        groupId: 'mixed-group-a'
      })
    )
    expect(
      current.rows.filter((item) => item.groupId === 'mixed-group-a').map((item) => item.category)
    ).toEqual(['DRUG', 'LAB'])

    current = await resolveMockRequest(
      mockGroupAdviceOrders({
        encounterId,
        adviceIds: [labOrder.id, examOrder.id],
        groupId: 'mixed-group-b'
      })
    )
    expect(current.rows.find((item) => item.id === drugOrder.id)?.groupId).toBeUndefined()
    expect(current.rows.filter((item) => item.groupId === 'mixed-group-b')).toHaveLength(2)

    current = await resolveMockRequest(
      mockUngroupAdviceOrders({
        encounterId,
        adviceIds: [labOrder.id]
      })
    )
    expect(current.rows.every((item) => !item.groupId)).toBe(true)

    current = await resolveMockRequest(
      mockGroupAdviceOrders({
        encounterId,
        adviceIds: [drugOrder.id, labOrder.id],
        groupId: 'sign-group'
      })
    )
    current = await resolveMockRequest(
      mockSignAdviceOrders({
        encounterId,
        adviceIds: [drugOrder.id, labOrder.id]
      })
    )
    const signedDrug = current.rows.find((item) => item.id === drugOrder.id)
    const signedLab = current.rows.find((item) => item.id === labOrder.id)
    expect(signedDrug).toMatchObject({ status: 'SIGNED' })
    expect(signedDrug?.prescriptionNo).toBeTruthy()
    expect(signedLab).toMatchObject({ status: 'SIGNED' })
    expect(signedLab?.serviceRequestId).toBeTruthy()

    current = await resolveMockRequest(
      mockWithdrawAdviceOrders({
        encounterId,
        adviceIds: [drugOrder.id, labOrder.id]
      })
    )
    expect(
      current.rows
        .filter((item) => [drugOrder.id, labOrder.id].includes(item.id))
        .every((item) => item.status === 'WITHDRAWN')
    ).toBe(true)
  })

  it('允许删除最后一条草稿医嘱', async () => {
    const encounterId = 'enc-extra-17'
    const catalogPage = await resolveMockRequest(
      mockPageAdviceOrderCatalog({ pageIndex: 1, pageSize: 1, category: 'TREATMENT' })
    )
    const catalog = catalogPage.rows?.[0]!
    const created = await resolveMockRequest(
      mockCreateAdviceOrder({
        encounterId,
        item: toSaveItem(catalog, 1)
      })
    )
    const deleted = await resolveMockRequest(
      mockDeleteAdviceOrders({
        encounterId,
        adviceIds: [created.rows[0]!.id]
      })
    )

    expect(deleted.rows).toEqual([])
  })

  it('历史列表隔离患者并排除当前就诊，详情只返回正式医嘱', async () => {
    const encounters = await resolveMockRequest(
      mockPagePatientAdviceOrderHistory({
        patientId: 'patient-1003',
        excludeEncounterId: 'enc-20260723-003',
        pageIndex: 1,
        pageSize: 10
      })
    )
    const historyRows = encounters.rows ?? []
    expect(encounters.total).toBeGreaterThan(1)
    expect(
      historyRows.every(
        (item) => item.patientId === 'patient-1003' && item.encounterId !== 'enc-20260723-003'
      )
    ).toBe(true)
    expect(historyRows[0]!.visitedAt >= historyRows[1]!.visitedAt).toBe(true)

    const details = await resolveMockRequest(
      mockPagePatientAdviceOrderHistoryDetails({
        patientId: 'patient-1003',
        encounterId: historyRows[0]!.encounterId,
        pageIndex: 1,
        pageSize: 10
      })
    )
    expect(details.rows?.every((item) => ['SIGNED', 'WITHDRAWN'].includes(item.status))).toBe(true)
  })
})
