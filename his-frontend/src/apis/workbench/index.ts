import { mockLoadWorkbenchData } from './mock'
import type { WorkbenchData } from './type'

export const loadWorkbenchData = (): Promise<WorkbenchData> => mockLoadWorkbenchData()

export type * from './type'
