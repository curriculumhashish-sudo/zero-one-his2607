// @vitest-environment jsdom

import { describe, expect, it } from 'vitest'
import dayjs from 'dayjs'

import * as loginApi from '@/apis/login'
import * as feeDetailApi from '@/apis/report/outpatient-fee-detail'
import * as statisticsApi from '@/apis/report/data-statistics'
import reportRoutes from '@/router/main/report-center'
import { flattenEncounterBlocks } from './outpatient-fee-detail/adapter'

describe('第四轮 B 组功能与接口契约', () => {
  it('接口函数数量与思维导图 12 + 2 + 7 一致', () => {
    expect(Object.keys(loginApi).sort()).toEqual([
      'getCaptcha',
      'loadAvailableDepartments',
      'loadAvailableHospitals',
      'loadAvailableTenants',
      'loadCurrentMenus',
      'loadCurrentUser',
      'loginAccount',
      'logout',
      'selectDepartment',
      'selectHospital',
      'selectTenant',
      'verifyCaptcha'
    ])
    expect(Object.keys(feeDetailApi).sort()).toEqual([
      'exportOutpatientFeeDetails',
      'pageOutpatientFeeDetails'
    ])
    expect(Object.keys(statisticsApi).sort()).toEqual([
      'getInventoryAmountComparison',
      'getInventoryExpiryWarningDistribution',
      'getMonthlyOutpatientTrend',
      'getMonthlyVisitCount',
      'getPatientDiseaseDistribution',
      'getTrafficTrend',
      'getWeeklyOutpatientTrend'
    ])
  })

  it('登录按账号、租户、医院、机构、菜单和用户信息顺序完成', async () => {
    const captcha = await loginApi.getCaptcha()
    await expect(loginApi.verifyCaptcha({ id: captcha.id })).resolves.toBeUndefined()
    const login = await loginApi.loginAccount({ username: 'admin', password: '123456' })
    expect(Object.keys(login).sort()).toEqual([
      'accessToken',
      'configuration',
      'expiresIn',
      'refreshToken',
      'user'
    ])
    const tenants = await loginApi.loadAvailableTenants()
    await loginApi.selectTenant(tenants[0]!.id)
    const hospitals = await loginApi.loadAvailableHospitals()
    await loginApi.selectHospital(hospitals[0]!.id)
    const departments = await loginApi.loadAvailableDepartments()
    await loginApi.selectDepartment(departments[0]!.id)
    const [menus, user] = await Promise.all([
      loginApi.loadCurrentMenus(),
      loginApi.loadCurrentUser()
    ])
    expect(menus[0]).toMatchObject({ path: '/report', meta: { title: '报表中心' } })
    expect(Object.keys(user).sort()).toEqual([
      'avatar',
      'desc',
      'homePath',
      'realName',
      'roles',
      'token',
      'userId',
      'username'
    ])
    await expect(loginApi.logout()).resolves.toBeUndefined()
  })

  it('不同登录账号返回不同用户角色和菜单', async () => {
    await loginApi.loginAccount({ username: 'admin', password: '123456' })
    const adminMenus = await loginApi.loadCurrentMenus()
    const adminUser = await loginApi.loadCurrentUser()
    const adminTenants = await loginApi.loadAvailableTenants()
    await loginApi.logout()

    await loginApi.loginAccount({ username: 'reporter', password: '123456' })
    const reporterMenus = await loginApi.loadCurrentMenus()
    const reporterUser = await loginApi.loadCurrentUser()
    const reporterTenants = await loginApi.loadAvailableTenants()

    expect(reporterUser.username).toBe('reporter')
    expect(reporterUser.roles).not.toEqual(adminUser.roles)
    expect(reporterMenus[0]?.children).not.toEqual(adminMenus[0]?.children)
    expect(reporterTenants).toHaveLength(1)
    expect(reporterTenants).not.toEqual(adminTenants)
    await loginApi.logout()
  })

  it('门诊费用明细使用原型分页字段、就诊块和全量汇总', async () => {
    const dateTo = dayjs().format('YYYY-MM-DD')
    const dateFrom = dayjs().subtract(30, 'day').format('YYYY-MM-DD')
    const page = await feeDetailApi.pageOutpatientFeeDetails({
      PageIndex: 1,
      PageSize: 2,
      dateFrom,
      dateTo,
      tradeDirection: 'ALL'
    })
    expect(page).toMatchObject({ pageNo: 1, pageSize: 2, totalCount: 18 })
    expect(Object.keys(page).sort()).toEqual([
      'content',
      'pageNo',
      'pageSize',
      'summary',
      'totalCount'
    ])
    expect(page.content?.[0]).toHaveProperty('subtotal')
    expect(page.content?.[0]?.lines?.[0]).toHaveProperty('chargeItemId')
    const flattened = flattenEncounterBlocks(page.content ?? [])
    expect(flattened.filter((row) => row.rowType === 'subtotal')).toHaveLength(2)
  })

  it('门诊费用明细导出使用全部筛选结果且返回原型二进制字段', async () => {
    const dateTo = dayjs().format('YYYY-MM-DD')
    const dateFrom = dayjs().subtract(30, 'day').format('YYYY-MM-DD')
    const result = await feeDetailApi.exportOutpatientFeeDetails({
      dateFrom,
      dateTo,
      tradeDirection: 'ALL'
    })
    expect(result.body).toBeTruthy()
    expect(result.fileName).toBe('门诊费用明细.xlsx')
    expect(result.rowCount).toBe(36)
    expect(result.exportedRowCount).toBe(result.rowCount)
    expect(result.truncated).toBe(false)
  })

  it('门诊费用明细打印可按当前条件取得完整报表数据', async () => {
    const dateTo = dayjs().format('YYYY-MM-DD')
    const dateFrom = dayjs().subtract(30, 'day').format('YYYY-MM-DD')
    const firstPage = await feeDetailApi.pageOutpatientFeeDetails({
      PageIndex: 1,
      PageSize: 1,
      dateFrom,
      dateTo,
      tradeDirection: 'ALL'
    })
    const fullPage = await feeDetailApi.pageOutpatientFeeDetails({
      PageIndex: 1,
      PageSize: firstPage.totalCount!,
      dateFrom,
      dateTo,
      tradeDirection: 'ALL'
    })
    expect(fullPage.content).toHaveLength(firstPage.totalCount ?? 0)
    expect(flattenEncounterBlocks(fullPage.content ?? []).filter((row) => row.rowType === 'line')).toHaveLength(36)
  })

  it('数据统计七项功能均返回可展示数据', async () => {
    const dateTo = dayjs().format('YYYY-MM-DD')
    const dateFrom = dayjs().subtract(30, 'day').format('YYYY-MM-DD')
    const query = {
      dateFrom,
      dateTo,
      granularity: 'DAY' as const,
      organizationId: 'dept-internal'
    }
    const results = await Promise.all([
      statisticsApi.getTrafficTrend(query),
      statisticsApi.getMonthlyVisitCount(query),
      statisticsApi.getWeeklyOutpatientTrend(query),
      statisticsApi.getMonthlyOutpatientTrend(query),
      statisticsApi.getPatientDiseaseDistribution(query),
      statisticsApi.getInventoryAmountComparison(query),
      statisticsApi.getInventoryExpiryWarningDistribution(query)
    ])
    results.forEach((result) => expect(result).toBeTruthy())
    expect(results[0]).toHaveProperty('points')
    expect(results[1]).toMatchObject({ unit: '人次' })
    expect(results[1].points).toHaveLength(12)
    expect(results[4]).toHaveProperty('items')
    expect(results[5].series.map((item) => item.name)).toEqual([
      '药品入库',
      '耗材入库',
      '药品出库',
      '耗材出库'
    ])
    expect(new Set(results[5].series.map((item) => item.direction))).toEqual(
      new Set(['INBOUND', 'OUTBOUND'])
    )
  })

  it('只注册任务书规定的两个报表地址', () => {
    expect(reportRoutes.map((route) => route.path)).toEqual([
      '/report/outpatient-fee-detail',
      '/report/data-statistics'
    ])
  })
})
