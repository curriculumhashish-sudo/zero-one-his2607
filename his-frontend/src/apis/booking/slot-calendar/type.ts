import type { BookingOption, RequestContext } from '../outpatient-schedule/type'

export type SlotStatus = 'available' | 'full' | 'stopped' | 'expired' | 'blocked'
export type SlotStatusCode = string

export interface SlotListItemDto {
  slotId?: string
  scheduleId?: string
  organizationId?: string
  practitionerId?: string
  healthcareServiceId?: string
  practitionerName?: string
  healthcareServiceName?: string
  organizationName?: string
  startTime?: string
  endTime?: string
  statusCode?: string
  capacity?: number
  bookedCount?: number
  remainingCount?: number
  displayStatus?: string
  overbookedFlag?: boolean
}

export interface SlotDetailDto extends SlotListItemDto {
  commentText?: string
}

export interface SlotRecord extends Required<SlotListItemDto> {
  id: string
  departmentId: string
  departmentName: string
  practitionerId: string
  practitionerName: string
  doctorId: string
  doctorName: string
  healthcareServiceId: string
  healthcareServiceName: string
  organizationName: string
  roomName: string
  date: string
  group: '上午' | '下午' | '晚间'
  used: number
  remainingCount: number
  remaining: number
  statusCode: SlotStatusCode
  status: SlotStatus
  blockedFlag?: boolean
  commentText?: string
  unavailableReason?: string
}

export interface PageSlotsByDateRequest {
  organizationId: string
  date: string
  practitionerId?: string | null
  scheduleId?: string | null
  statusCode?: string | null
  PageIndex: number
  PageSize: number
}
export type SlotQuery = PageSlotsByDateRequest

export interface LoadSlotDetailRequest {
  slotId: string
}

export interface SlotInitResult {
  organizationId: string
  departments: BookingOption[]
  doctors: BookingOption[]
}

export interface SlotPageResult {
  content: SlotRecord[]
  pageNo: number
  pageSize: number
  totalCount: number
}

export interface SlotPageResponse {
  content?: SlotListItemDto[]
  totalCount?: number
  pageNo?: number
  pageSize?: number
}

export interface BatchBlockSlotsPayload {
  reason?: string
  slotIds: string[]
}

export interface BatchUnblockSlotsPayload {
  slotIds: string[]
}

export interface MarkSlotOverbookedPayload {
  overbookedFlag: boolean
  slotId: string
}

export type { BookingOption, RequestContext }
