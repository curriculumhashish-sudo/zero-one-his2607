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
      },
      {
        id: 'frontend-basic-code',
        pid: 'frontend-basic',
        text: '编码管理',
        icon: 'IconTickets',
        href: '/basic/code'
      },
      {
        id: 'frontend-basic-medication-catalog',
        pid: 'frontend-basic',
        text: '药品目录',
        icon: 'IconTickets',
        href: '/basicdata/medication-catalog'
      },
      {
        id: 'frontend-basic-frequency',
        pid: 'frontend-basic',
        text: '频次管理',
        icon: 'IconTickets',
        href: '/basicdata/frequency'
      },
      {
        id: 'frontend-basic-medication',
        pid: 'frontend-basic',
        text: '药品管理',
        icon: 'IconTickets',
        href: '/basicdata/medication'
      },
      {
        id: 'frontend-basic-diagnosis',
        pid: 'frontend-basic',
        text: '诊断管理',
        icon: 'IconTickets',
        href: '/basicdata/diagnosis'
      },
      {
        id: 'frontend-basic-treatment',
        pid: 'frontend-basic',
        text: '诊疗管理',
        icon: 'IconTickets',
        href: '/basicdata/treatment'
      },
      {
        id: 'frontend-basic-equipment',
        pid: 'frontend-basic',
        text: '器材管理',
        icon: 'IconTickets',
        href: '/basicdata/equipment'
      },
      {
        id: 'frontend-basic-sample',
        pid: 'frontend-basic',
        text: '样本管理',
        icon: 'IconTickets',
        href: '/basicdata/sample'
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
      },
      {
        id: 'frontend-booking-template',
        pid: 'frontend-booking',
        text: '号源设置',
        icon: 'IconTickets',
        href: '/booking/schedule-template'
      }
    ]
  },
  {
    id: 'frontend-charge',
    text: '收费管理',
    icon: 'IconTickets',
    children: [
      {
        id: 'frontend-outpatient-register',
        pid: 'frontend-charge',
        text: '门诊挂号',
        icon: 'IconTickets',
        href: '/outpatient/charge/register'
      },
      {
        id: 'frontend-outpatient-charge',
        pid: 'frontend-charge',
        text: '门诊收费',
        icon: 'IconTickets',
        href: '/outpatient/charge/charge'
      },
      {
        id: 'frontend-outpatient-refund',
        pid: 'frontend-charge',
        text: '门诊退费',
        icon: 'IconTickets',
        href: '/outpatient/charge/refund'
      },
      {
        id: 'frontend-inpatient-register',
        pid: 'frontend-charge',
        text: '住院登记',
        icon: 'IconTickets',
        href: '/inpatient/charge/register'
      },
      {
        id: 'frontend-inpatient-prepay',
        pid: 'frontend-charge',
        text: '住院预交金',
        icon: 'IconTickets',
        href: '/inpatient/charge/prepay'
      },
      {
        id: 'frontend-inpatient-settle',
        pid: 'frontend-charge',
        text: '住院结算',
        icon: 'IconTickets',
        href: '/inpatient/charge/settle'
      }
    ]
  },
  {
    id: 'frontend-warehouse',
    text: '药库药房',
    icon: 'IconMenu',
    children: [
      {
        id: 'frontend-warehouse-purchase-inbound',
        pid: 'frontend-warehouse',
        text: '采购入库工作站',
        icon: 'IconTickets',
        href: '/warehouse/purchase-inbound'
      },
      {
        id: 'frontend-pharmacy-stock-in',
        pid: 'frontend-warehouse',
        text: '药房入库',
        icon: 'IconTickets',
        href: '/inventory/pharmacy/stock-in'
      },
      {
        id: 'frontend-pharmacy-stocktake',
        pid: 'frontend-warehouse',
        text: '药房盘点',
        icon: 'IconTickets',
        href: '/inventory/pharmacy/stocktake'
      },
      {
        id: 'frontend-warehouse-purchase-inbound-v2',
        pid: 'frontend-warehouse',
        text: '药库进货',
        icon: 'IconTickets',
        href: '/inventory/warehouse/purchase-inbound'
      },
      {
        id: 'frontend-warehouse-stock-out',
        pid: 'frontend-warehouse',
        text: '药库出库',
        icon: 'IconTickets',
        href: '/inventory/warehouse/stock-out'
      },
      {
        id: 'frontend-warehouse-stocktake',
        pid: 'frontend-warehouse',
        text: '药库盘点',
        icon: 'IconTickets',
        href: '/inventory/warehouse/stocktake'
      },
      {
        id: 'frontend-inventory-stock-list',
        pid: 'frontend-warehouse',
        text: '库存明细',
        icon: 'IconTickets',
        href: '/inventory/stock-list'
      }
    ]
  },
  {
    id: 'frontend-dispense',
    text: '发药管理',
    icon: 'IconMenu',
    children: [
      {
        id: 'frontend-outpatient-dispense-v1',
        pid: 'frontend-dispense',
        text: '门诊发药工作站',
        icon: 'IconTickets',
        href: '/pharmacy/outpatient-dispense'
      },
      {
        id: 'frontend-outpatient-dispense',
        pid: 'frontend-dispense',
        text: '门诊发药',
        icon: 'IconTickets',
        href: '/pharmacy/dispense/outpatient'
      },
      {
        id: 'frontend-inpatient-dispense',
        pid: 'frontend-dispense',
        text: '住院发药',
        icon: 'IconTickets',
        href: '/pharmacy/dispense/inpatient'
      },
      {
        id: 'frontend-outpatient-return-medicine',
        pid: 'frontend-dispense',
        text: '门诊退药',
        icon: 'IconTickets',
        href: '/pharmacy/return-medicine/outpatient'
      },
      {
        id: 'frontend-inpatient-return-medicine',
        pid: 'frontend-dispense',
        text: '住院退药',
        icon: 'IconTickets',
        href: '/pharmacy/return-medicine/inpatient'
      },
      {
        id: 'frontend-outpatient-dispense-record',
        pid: 'frontend-dispense',
        text: '门诊发退药记录',
        icon: 'IconTickets',
        href: '/pharmacy/dispense-record/outpatient'
      },
      {
        id: 'frontend-inpatient-dispense-record',
        pid: 'frontend-dispense',
        text: '住院发退药记录',
        icon: 'IconTickets',
        href: '/pharmacy/dispense-record/inpatient'
      }
    ]
  },
  {
    id: 'frontend-report',
    text: '报表中心',
    icon: 'IconTickets',
    children: [
      {
        id: 'frontend-report-outpatient-fee-detail',
        pid: 'frontend-report',
        text: '门诊费用明细',
        icon: 'IconTickets',
        href: '/report/outpatient-fee-detail'
      },
      {
        id: 'frontend-report-data-statistics',
        pid: 'frontend-report',
        text: '数据统计',
        icon: 'IconTickets',
        href: '/report/data-statistics'
      }
    ]
  },
  {
    id: 'frontend-clinical-station',
    text: '临床工作站',
    icon: 'IconUser',
    children: [
      {
        id: 'frontend-doctor-station',
        pid: 'frontend-clinical-station',
        text: '门诊医生站',
        icon: 'IconUser',
        href: '/outpatient/doctor-station'
      },
      {
        id: 'frontend-inpatient-doctor-station',
        pid: 'frontend-clinical-station',
        text: '住院医生站',
        icon: 'IconUser',
        href: '/inpatient/doctor-station'
      },
      {
        id: 'frontend-outpatient-nurse-station',
        pid: 'frontend-clinical-station',
        text: '门诊护士站',
        icon: 'IconUser',
        href: '/outpatient/nurse-station'
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
