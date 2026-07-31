import type { BookingOption, RequestContext } from '../outpatient-schedule/type'

export type SlotStatus = 'available' | 'full' | 'stopped' | 'expired'
export type SlotStatusCode = 'FREE' | 'FULL' | 'BLOCKED' | 'EXPIRED'

export interface SlotRecord {
  slotId: string
  id: string
  scheduleId: string
  organizationId: string
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
  startTime: string
  endTime: string
  group: '上午' | '下午' | '晚间'
  capacity: number
  bookedCount: number
  used: number
  remainingCount: number
  remaining: number
  statusCode: SlotStatusCode
  status: SlotStatus
  displayStatus: string
  overbookedFlag: boolean
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
  pageNo: number
  pageSize: number
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
