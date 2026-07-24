import type { BookingOption, RequestContext } from '../outpatient-schedule/type'

export type SlotStatus = 'available' | 'full' | 'stopped' | 'expired'

export interface SlotRecord {
  id: string
  scheduleId: string
  departmentId: string
  departmentName: string
  doctorId: string
  doctorName: string
  roomName: string
  date: string
  startTime: string
  endTime: string
  group: '上午' | '下午' | '晚间'
  capacity: number
  used: number
  remaining: number
  status: SlotStatus
  unavailableReason?: string
}

export interface SlotQuery {
  date: string
  keyword?: string
  departmentId?: string | null
  doctorId?: string | null
}

export interface SlotInitResult {
  departments: BookingOption[]
  doctors: BookingOption[]
}

export type { BookingOption, RequestContext }
