import type {
  BookingOption,
  ScheduleRecord
} from '@/apis/booking/outpatient-schedule'

export interface ScheduleQueryModel {
  keyword: string
  departmentId: string | null
  doctorId: string | null
  roomId: string | null
}

export interface ScheduleDialogModel extends ScheduleSavePayload {
  id?: string
}

export interface ScheduleSavePayload {
  departmentId: string
  doctorId: string
  roomId: string
  date: string
  capacity: number
  publish: boolean
}

export interface SchedulePageData {
  rows: ScheduleRecord[]
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
}
