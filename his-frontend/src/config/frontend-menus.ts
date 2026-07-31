import type { Menu } from '@/apis/login/type'

/**
 * 当前阶段由前端维护的业务菜单。
 * 这里只配置导航层级和路由地址，不在菜单中重复定义页面组件。
 */
const FRONTEND_MENUS: Menu[] = [
  {
    id: 'frontend-system',
    text: '系统管理',
    icon: 'IconUser',
    children: [
      {
        id: 'frontend-system-user',
        pid: 'frontend-system',
        text: '用户管理',
        icon: 'IconUser',
        href: '/system/user'
      },
      {
        id: 'frontend-system-permission',
        pid: 'frontend-system',
        text: '权限管理',
        icon: 'IconTickets',
        href: '/system/permission'
      },
      {
        id: 'frontend-system-menu',
        pid: 'frontend-system',
        text: '菜单管理',
        icon: 'IconMenu',
        href: '/system/menu'
      },
      {
        id: 'frontend-system-role',
        pid: 'frontend-system',
        text: '角色管理',
        icon: 'IconUser',
        href: '/system/role'
      }
    ]
  },
  {
    id: 'frontend-configuration',
    text: '配置管理',
    icon: 'IconSetting',
    children: [
      {
        id: 'frontend-configuration-emr-template',
        pid: 'frontend-configuration',
        text: '病历模板配置',
        icon: 'IconTickets',
        href: '/configuration/emr-template'
      },
      {
        id: 'frontend-configuration-execute-location',
        pid: 'frontend-configuration',
        text: '执行地点',
        icon: 'IconSetting',
        href: '/configuration/execute-location'
      },
      {
        id: 'frontend-configuration-order-template',
        pid: 'frontend-configuration',
        text: '组套模板',
        icon: 'IconTickets',
        href: '/configuration/order-template'
      },
      {
        id: 'frontend-configuration-medication-location',
        pid: 'frontend-configuration',
        text: '取药科室',
        icon: 'IconMenu',
        href: '/configuration/medication-location'
      },
      {
        id: 'frontend-configuration-service-device-binding',
        pid: 'frontend-configuration',
        text: '诊疗耗材绑定',
        icon: 'IconSetting',
        href: '/configuration/service-device-binding'
      }
    ]
  },
  {
    id: 'frontend-basic',
    text: '基础数据',
    icon: 'IconMenu',
    children: [
      {
        id: 'frontend-basic-department',
        pid: 'frontend-basic',
        text: '科室管理',
        icon: 'IconTickets',
        href: '/basic/department'
      },
      {
        id: 'frontend-basic-patient',
        pid: 'frontend-basic',
        text: '患者管理',
        icon: 'IconUser',
        href: '/basic/patient'
      },
      {
        id: 'frontend-basic-ward-bed',
        pid: 'frontend-basic',
        text: '病区病房',
        icon: 'IconTickets',
        href: '/basic/ward-bed'
      },
      {
        id: 'frontend-basic-warehouse-pharmacy',
        pid: 'frontend-basic',
        text: '库房药房',
        icon: 'IconMenu',
        href: '/basic/warehouse-pharmacy'
      },
      {
        id: 'frontend-basic-supplier',
        pid: 'frontend-basic',
        text: '供应商管理',
        icon: 'IconTickets',
        href: '/basic/supplier'
      },
      {
        id: 'frontend-basic-charge-type',
        pid: 'frontend-basic',
        text: '费用性质',
        icon: 'IconTickets',
        href: '/basic/charge-type'
      },
      {
        id: 'frontend-basic-registration-type',
        pid: 'frontend-basic',
        text: '挂号类型',
        icon: 'IconTickets',
        href: '/basic/registration-type'
      }
    ]
  },
  {
    id: 'frontend-booking',
    text: '排班预约',
    icon: 'IconTickets',
    children: [
      {
        id: 'frontend-booking-schedule',
        pid: 'frontend-booking',
        text: '门诊排班',
        icon: 'IconTickets',
        href: '/booking/outpatient-schedule'
      },
      {
        id: 'frontend-booking-slots',
        pid: 'frontend-booking',
        text: '号源查看',
        icon: 'IconTickets',
        href: '/booking/slot-calendar'
      }
    ]
  },
  {
    id: 'frontend-warehouse',
    text: '药库药房',
    icon: 'IconMenu',
    children: [
      {
        id: 'frontend-warehouse-stocktake',
        pid: 'frontend-warehouse',
        text: '药房盘点',
        icon: 'IconTickets',
        href: '/warehouse/purchase-inbound'
      }
    ]
  },
  {
    id: 'frontend-dispense',
    text: '发药管理',
    icon: 'IconMenu',
    children: [
      {
        id: 'frontend-outpatient-dispense',
        pid: 'frontend-dispense',
        text: '门诊发药',
        icon: 'IconTickets',
        href: '/pharmacy/outpatient-dispense'
      }
    ]
  },
  {
    id: 'frontend-outpatient',
    text: '门诊业务',
    icon: 'IconUser',
    children: [
      {
        id: 'frontend-doctor-station',
        pid: 'frontend-outpatient',
        text: '门诊医生站',
        icon: 'IconUser',
        href: '/outpatient/doctor-station'
      }
    ]
  }
]

function cloneMenu(menu: Menu): Menu {
  return {
    ...menu,
    children: menu.children?.map(cloneMenu)
  }
}

export function createFrontendMenus(): Menu[] {
  return FRONTEND_MENUS.map(cloneMenu)
}

/**
 * 后端菜单仍然保留；相同分组按名称合并，相同页面按 href 去重。
 * 后续启用完整权限菜单时，只需停止传入前端菜单，不需要改 HomeView。
 */
export function mergeFrontendMenus(serverMenus: Menu[] = []): Menu[] {
  const result = serverMenus.map(cloneMenu)

  for (const frontendGroup of FRONTEND_MENUS) {
    const existingGroup = result.find(
      (item) => item.id === frontendGroup.id || item.text === frontendGroup.text
    )
    if (!existingGroup) {
      result.push(cloneMenu(frontendGroup))
      continue
    }

    const children = existingGroup.children ?? []
    for (const frontendChild of frontendGroup.children ?? []) {
      const duplicated = children.some(
        (item) => item.id === frontendChild.id || item.href === frontendChild.href
      )
      if (!duplicated) children.push(cloneMenu(frontendChild))
    }
    existingGroup.children = children
  }

  return result
}
