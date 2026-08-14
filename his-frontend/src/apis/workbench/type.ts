export interface WorkbenchNotice {
  id: string
  title: string
  level: 'info' | 'warning' | 'danger' | 'success'
  createdAt: string
}

export interface WorkbenchAction {
  id: string
  title: string
  path: string
  icon: string
  group: string
}

export interface WorkbenchStat {
  key: string
  label: string
  value: string
  trend?: string
  trendType?: 'up' | 'down' | 'flat'
}

export interface WorkbenchData {
  notices: WorkbenchNotice[]
  actions: WorkbenchAction[]
  statistics: WorkbenchStat[]
}
