import { mockLoadWorkbenchData } from './mock'
import type { WorkbenchData } from './type'
import type { Menu } from '@/apis/login/type'

export const loadWorkbenchData = (roles: string[] = [], menus: Menu[] = []): Promise<WorkbenchData> =>
  mockLoadWorkbenchData(roles, menus)

export type * from './type'
