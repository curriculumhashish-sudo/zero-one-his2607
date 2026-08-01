import { mockLoadWorkbenchData } from './mock'
import type { WorkbenchData } from './type'

export const loadWorkbenchData = (roles: string[] = []): Promise<WorkbenchData> =>
  mockLoadWorkbenchData(roles)

export type * from './type'
