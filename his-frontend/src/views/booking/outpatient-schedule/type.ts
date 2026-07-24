import type {
  BookingOption,
  ScheduleRecord,
  ScheduleSavePayload
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

export interface SchedulePageData {
  rows: ScheduleRecord[]
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
}
