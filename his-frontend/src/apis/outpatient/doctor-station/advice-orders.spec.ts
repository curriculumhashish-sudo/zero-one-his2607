import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import {
  createAdviceOrder,
  deleteAdviceOrders,
  groupAdviceOrders,
  pageAdviceOrderCatalog,
  pageAdviceOrders,
  pagePatientAdviceOrderHistory,
  pagePatientAdviceOrderHistoryDetails,
  signAdviceOrders,
  ungroupAdviceOrders,
  updateAdviceOrder,
  withdrawAdviceOrders,
  type DoctorAdviceOrderCatalogItemVO,
  type SaveDoctorAdviceOrderItemDTO
} from './index'

async function resolveRequest<T>(request: Promise<T>) {
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

async function loadCatalog() {
  const page = await resolveRequest(pageAdviceOrderCatalog({ pageIndex: 1, pageSize: 20 }))
  return page.rows ?? []
}

describe('doctor advice order API mock', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('支持分页 CRUD 和删除最后一条草稿', async () => {
    const encounterId = 'enc-extra-14'
    const catalog = await loadCatalog()
    const first = catalog[0]!

    const emptyPage = await resolveRequest(
      pageAdviceOrders({ encounterId, pageIndex: 1, pageSize: 10 })
    )
    expect(emptyPage.total).toBe(0)

    const created = await resolveRequest(
      createAdviceOrder({
        encounterId,
        item: toSaveItem(first, 1)
      })
    )
    expect(created.rows).toHaveLength(1)
    expect(created.rows[0]?.status).toBe('DRAFT')

    const updated = await resolveRequest(
      updateAdviceOrder({
        encounterId,
        adviceId: created.rows[0]!.id,
        item: {
          ...toSaveItem(first, 1),
          quantity: first.defaultQuantity + 2
        }
      })
    )
    expect(updated.rows[0]?.quantity).toBe(first.defaultQuantity + 2)

    const deleted = await resolveRequest(
      deleteAdviceOrders({
        encounterId,
        adviceIds: [updated.rows[0]!.id]
      })
    )
    expect(deleted.rows).toEqual([])
  })

  it('支持跨分类任意并组、重新并组、孤立组清理和拆组', async () => {
    const encounterId = 'enc-extra-17'
    const catalog = await loadCatalog()
    const candidates = [
      catalog.find((item) => item.category === 'DRUG')!,
      catalog.find((item) => item.category === 'LAB')!,
      catalog.find((item) => item.category === 'EXAM')!
    ]

    let rows = [] as Awaited<ReturnType<typeof createAdviceOrder>>['rows']
    for (const [index, candidate] of candidates.entries()) {
      const result = await resolveRequest(
        createAdviceOrder({
          encounterId,
          item: toSaveItem(candidate, index + 1)
        })
      )
      rows = result.rows
    }

    const firstGroup = await resolveRequest(
      groupAdviceOrders({
        encounterId,
        adviceIds: [rows[0]!.id, rows[1]!.id],
        groupId: 'mixed-group-a'
      })
    )
    expect(firstGroup.rows.slice(0, 2).map((item) => item.groupId)).toEqual([
      'mixed-group-a',
      'mixed-group-a'
    ])

    const regrouped = await resolveRequest(
      groupAdviceOrders({
        encounterId,
        adviceIds: [firstGroup.rows[1]!.id, firstGroup.rows[2]!.id],
        groupId: 'mixed-group-b'
      })
    )
    expect(regrouped.rows.map((item) => item.groupId)).toEqual([
      undefined,
      'mixed-group-b',
      'mixed-group-b'
    ])

    const ungrouped = await resolveRequest(
      ungroupAdviceOrders({
        encounterId,
        adviceIds: [regrouped.rows[1]!.id]
      })
    )
    expect(ungrouped.rows.every((item) => !item.groupId)).toBe(true)
  })

  it('批量签发后可批量撤回，撤回记录永久保留且不可再修改', async () => {
    const encounterId = 'enc-extra-20'
    const catalog = await loadCatalog()
    const selectedCatalog = [catalog[0]!, catalog.find((item) => item.category === 'LAB')!]

    let rows = [] as Awaited<ReturnType<typeof createAdviceOrder>>['rows']
    for (const [index, candidate] of selectedCatalog.entries()) {
      const result = await resolveRequest(
        createAdviceOrder({
          encounterId,
          item: toSaveItem(candidate, index + 1)
        })
      )
      rows = result.rows
    }
    const grouped = await resolveRequest(
      groupAdviceOrders({
        encounterId,
        adviceIds: rows.map((item) => item.id),
        groupId: 'sign-group'
      })
    )
    const signed = await resolveRequest(
      signAdviceOrders({
        encounterId,
        adviceIds: grouped.rows.map((item) => item.id)
      })
    )
    expect(signed.rows.every((item) => item.status === 'SIGNED')).toBe(true)

    const withdrawn = await resolveRequest(
      withdrawAdviceOrders({
        encounterId,
        adviceIds: signed.rows.map((item) => item.id)
      })
    )
    expect(withdrawn.rows.every((item) => item.status === 'WITHDRAWN')).toBe(true)

    const updateRequest = updateAdviceOrder({
      encounterId,
      adviceId: withdrawn.rows[0]!.id,
      item: toSaveItem(selectedCatalog[0]!, 1)
    })
    const updateExpectation = expect(updateRequest).rejects.toThrow('只有未签发医嘱可以修改')
    await vi.runAllTimersAsync()
    await updateExpectation
  })

  it('历史医嘱按患者隔离、分页，并只返回正式状态', async () => {
    const history = await resolveRequest(
      pagePatientAdviceOrderHistory({
        patientId: 'patient-1003',
        excludeEncounterId: 'enc-20260723-003',
        pageIndex: 1,
        pageSize: 1
      })
    )
    expect(history.total).toBeGreaterThan(1)
    expect(history.rows).toHaveLength(1)
    expect(history.rows?.[0]?.patientId).toBe('patient-1003')

    const details = await resolveRequest(
      pagePatientAdviceOrderHistoryDetails({
        patientId: 'patient-1003',
        encounterId: history.rows![0]!.encounterId,
        pageIndex: 1,
        pageSize: 10
      })
    )
    expect(details.rows?.length).toBeGreaterThan(0)
    expect(details.rows?.every((item) => ['SIGNED', 'WITHDRAWN'].includes(item.status))).toBe(true)

    const unauthorized = pagePatientAdviceOrderHistoryDetails({
      patientId: 'patient-1008',
      encounterId: history.rows![0]!.encounterId,
      pageIndex: 1,
      pageSize: 10
    })
    const unauthorizedExpectation = expect(unauthorized).rejects.toThrow('未找到患者历史医嘱就诊')
    await vi.runAllTimersAsync()
    await unauthorizedExpectation
  })

  it('每次变更后列表 version 递增，携带过期版本会被拒绝', async () => {
    const encounterId = 'enc-extra-30'
    const catalog = await loadCatalog()
    const first = catalog[0]!

    const emptyPage = await resolveRequest(
      pageAdviceOrders({ encounterId, pageIndex: 1, pageSize: 10 })
    )
    expect(emptyPage.version).toBe(0)

    const created = await resolveRequest(
      createAdviceOrder({ encounterId, item: toSaveItem(first, 1) })
    )
    expect(created.version).toBe(1)

    // 携带过期 version 提交，触发乐观锁冲突。
    const staleRequest = updateAdviceOrder({
      encounterId,
      adviceId: created.rows[0]!.id,
      version: 0,
      item: { ...toSaveItem(first, 1), quantity: first.defaultQuantity + 9 }
    })
    const staleExpectation = expect(staleRequest).rejects.toThrow('医嘱列表已被其他操作更新')
    await vi.runAllTimersAsync()
    await staleExpectation

    // 不带 version 时由 API 层自动携带最近一次查询/操作的版本，仍可正常提交。
    const fresh = await resolveRequest(
      updateAdviceOrder({
        encounterId,
        adviceId: created.rows[0]!.id,
        item: { ...toSaveItem(first, 1), quantity: first.defaultQuantity + 2 }
      })
    )
    expect(fresh.version).toBe(2)
    expect(fresh.rows[0]?.quantity).toBe(first.defaultQuantity + 2)
  })
})
