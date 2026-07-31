import {
  mockCreateSchedule,
  mockGetScheduleDetail,
  mockLoadDepartmentOptions,
  mockLoadDoctorOptions,
  mockGenerateFormalSchedules,
  mockSetScheduleSubstitution,
  mockClearScheduleSubstitution,
  mockPreviewScheduleImpact,
  mockPartialStopSchedule,
  mockUnlockPartialStop,
  mockResumeSchedule,
  mockVoidSchedule,
  mockListReleasedSlots,
  mockProvisionScheduleSlots,
  mockDeleteReleasedSlots,
  mockLoadScheduleInit,
  mockPageSchedulesByRange,
  mockPublishSchedule,
  mockStopSchedule,
  mockUpdateSchedule
} from './mock'
import type {
  GenerateFormalSchedulesPayload,
  RequestContext,
  ScheduleGenerationOptions
} from './type'
import {
  pageDailyTemplates,
  pageTimePeriods,
  pageWeeklyTemplates
} from '../schedule-template'

export const loadScheduleInit = mockLoadScheduleInit
export const pageSchedulesByRange = mockPageSchedulesByRange
export const getScheduleDetail = mockGetScheduleDetail
export const createSchedule = mockCreateSchedule
export const updateSchedule = mockUpdateSchedule
export const publishSchedule = mockPublishSchedule
export const stopSchedule = mockStopSchedule
export const loadDepartmentOptions = mockLoadDepartmentOptions
export const loadDoctorOptions = mockLoadDoctorOptions
async function loadGenerationData(organizationId: string, context: RequestContext = {}) {
  const query = { organizationId, status: 'enabled' as const, pageNo: 1, pageSize: 200 }
  const [dailyTemplates, weeklyTemplates, timePeriods] = await Promise.all([
    pageDailyTemplates(query, context),
    pageWeeklyTemplates(query, context),
    pageTimePeriods(query, context)
  ])
  return {
    dailyTemplates: dailyTemplates.content,
    weeklyTemplates: weeklyTemplates.content,
    timePeriods: timePeriods.content
  }
}

export async function loadScheduleGenerationOptions(
  organizationId: string,
  context: RequestContext = {}
): Promise<ScheduleGenerationOptions> {
  const data = await loadGenerationData(organizationId, context)
  return {
    dailyTemplates: data.dailyTemplates.map((item) => ({
      value: item.id,
      label: item.templateName ?? item.name
    })),
    weeklyTemplates: data.weeklyTemplates.map((item) => ({
      value: item.id,
      label: item.templateName ?? item.name
    }))
  }
}

export async function generateFormalSchedules(
  payload: GenerateFormalSchedulesPayload,
  context: RequestContext = {}
) {
  const templates = await loadGenerationData(payload.organizationId, context)
  return mockGenerateFormalSchedules(payload, templates, context)
}
export const setScheduleSubstitution = mockSetScheduleSubstitution
export const clearScheduleSubstitution = mockClearScheduleSubstitution
export const previewScheduleImpact = mockPreviewScheduleImpact
export const partialStopSchedule = mockPartialStopSchedule
export const unlockPartialStop = mockUnlockPartialStop
export const resumeSchedule = mockResumeSchedule
export const voidSchedule = mockVoidSchedule
export const listReleasedSlots = mockListReleasedSlots
export const provisionScheduleSlots = mockProvisionScheduleSlots
export const deleteReleasedSlots = mockDeleteReleasedSlots

export type * from './type'
