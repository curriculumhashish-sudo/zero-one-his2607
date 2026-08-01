import type { WorkbenchData } from './type'

const MOCK_DATA: WorkbenchData = {
  notices: [
    { id: 'n1', title: '系统将于本周六凌晨2点进行维护升级', level: 'warning', createdAt: '2025-07-01 10:00' },
    { id: 'n2', title: '新增药品审批流程已上线，请相关人员查阅', level: 'info', createdAt: '2025-06-30 15:30' },
    { id: 'n3', title: '门诊发药窗口调整通知', level: 'success', createdAt: '2025-06-28 09:00' }
  ],
  actions: [
    { id: 'a1', title: '门诊发药', path: '/pharmacy/outpatient-dispense', icon: 'FirstAidKit', group: '药房管理' },
    { id: 'a2', title: '住院发药', path: '/pharmacy/inpatient-dispense', icon: 'OfficeBuilding', group: '药房管理' },
    { id: 'a3', title: '门诊退药', path: '/pharmacy/outpatient-return', icon: 'RefreshLeft', group: '药房管理' },
    { id: 'a4', title: '住院退药', path: '/pharmacy/inpatient-return', icon: 'RefreshRight', group: '药房管理' },
    { id: 'a5', title: '处方审核', path: '/pharmacy/prescription-review', icon: 'DocumentChecked', group: '药房管理' },
    { id: 'a6', title: '发药记录', path: '/dispense-record', icon: 'Tickets', group: '药房管理' },
    { id: 'a7', title: '租户管理', path: '/system/tenant', icon: 'OfficeBuilding', group: '系统管理' },
    { id: 'a8', title: '设置管理', path: '/system/settings', icon: 'Setting', group: '系统管理' },
    { id: 'a9', title: '用户管理', path: '/system/user', icon: 'User', group: '系统管理' },
    { id: 'a10', title: '角色管理', path: '/system/role', icon: 'UserFilled', group: '系统管理' },
    { id: 'a11', title: '个人中心', path: '/profile', icon: 'User', group: '个人' },
    { id: 'a12', title: '修改密码', path: '/profile?tab=password', icon: 'Lock', group: '个人' }
  ],
  statistics: [
    { key: 's1', label: '今日门诊发药', value: '236', trend: '+12%', trendType: 'up' },
    { key: 's2', label: '今日住院发药', value: '89', trend: '+5%', trendType: 'up' },
    { key: 's3', label: '待审核处方', value: '12', trend: '-3%', trendType: 'down' },
    { key: 's4', label: '异常发药', value: '2', trend: '0%', trendType: 'flat' }
  ]
}

function delay<T>(data: T, ms = 200): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(data), ms))
}

export function mockLoadWorkbenchData(): Promise<WorkbenchData> {
  return delay({
    notices: [...MOCK_DATA.notices],
    actions: [...MOCK_DATA.actions],
    statistics: [...MOCK_DATA.statistics]
  })
}
