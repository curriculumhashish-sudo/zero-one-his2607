import { describe, expect, it } from 'vitest'
import { createFrontendMenus, mergeFrontendMenus } from './frontend-menus'

describe('前端业务菜单', () => {
  it('按业务模块生成两级菜单并覆盖所有已完成页面', () => {
    const menus = createFrontendMenus()
    const paths = menus.flatMap((item) => item.children?.map((child) => child.href) ?? [])

    expect(menus.every((item) => item.children?.length)).toBe(true)
    expect(paths).toEqual(
      expect.arrayContaining([
        '/system/user',
        '/system/permission',
        '/system/menu',
        '/system/role',
        '/configuration/emr-template',
        '/configuration/execute-location',
        '/configuration/order-template',
        '/configuration/medication-location',
        '/configuration/service-device-binding',
        '/basic/department',
        '/basic/patient',
        '/basic/ward-bed',
        '/basic/warehouse-pharmacy',
        '/basic/supplier',
        '/basic/charge-type',
        '/basic/registration-type',
        '/booking/outpatient-schedule',
        '/booking/slot-calendar',
        '/booking/schedule-template',
        '/outpatient/charge/register',
        '/outpatient/charge/charge',
        '/outpatient/charge/refund',
        '/inpatient/charge/register',
        '/inpatient/charge/prepay',
        '/inpatient/charge/settle',
        '/warehouse/purchase-inbound',
        '/inventory/pharmacy/stock-in',
        '/inventory/pharmacy/stocktake',
        '/inventory/warehouse/purchase-inbound',
        '/inventory/warehouse/stock-out',
        '/inventory/warehouse/stocktake',
        '/inventory/stock-list',
        '/pharmacy/outpatient-dispense',
        '/pharmacy/dispense/outpatient',
        '/pharmacy/dispense/inpatient',
        '/pharmacy/return-medicine/outpatient',
        '/pharmacy/return-medicine/inpatient',
        '/pharmacy/dispense-record/outpatient',
        '/pharmacy/dispense-record/inpatient',
        '/outpatient/doctor-station',
        '/inpatient/doctor-station',
        '/outpatient/nurse-station'
      ])
    )
    expect(new Set(paths).size).toBe(paths.length)
  })

  it('与后端菜单合并时保留后端入口并按路径去重', () => {
    const merged = mergeFrontendMenus([
      {
        id: 'server-system',
        text: '系统管理',
        icon: 'IconMenu',
        children: [
          {
            id: 'server-user',
            pid: 'server-system',
            text: '用户管理',
            icon: 'IconUser',
            href: '/system/user'
          },
          {
            id: 'server-role',
            pid: 'server-system',
            text: '角色管理',
            icon: 'IconUser',
            href: '/system/role'
          }
        ]
      }
    ])
    const systemChildren = merged.find((item) => item.text === '系统管理')?.children ?? []

    expect(systemChildren.filter((item) => item.href === '/system/user')).toHaveLength(1)
    expect(systemChildren.some((item) => item.href === '/system/role')).toBe(true)
  })
})
