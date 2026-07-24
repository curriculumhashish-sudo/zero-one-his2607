export type ScheduleStatus = 'draft' | 'published' | 'stopped'

export interface BookingOption extends Record<string, unknown> {
  value: string
  label: string
  departmentId?: string
  color?: string
}

export interface ScheduleRecord {
  id: string
  departmentId: string
  departmentName: string
  doctorId: string
  doctorName: string
  roomId: string
  roomName: string
  date: string
  startTime: string
  endTime: string
  capacity: number
  fee: number
  status: ScheduleStatus
  stopReason?: string
  updatedAt: string
}

export interface ScheduleRangeQuery {
  start: string
  end: string
  keyword?: string
  departmentId?: string | null
  doctorId?: string | null
  roomId?: string | null
}

export interface ScheduleSavePayload {
  departmentId: string
  doctorId: string
  roomId: string
  date: string
  startTime: string
  endTime: string
  capacity: number
  fee: number
  publish: boolean
}

export interface ScheduleInitResult {
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
}

export interface RequestContext {
  signal?: AbortSignal
}
