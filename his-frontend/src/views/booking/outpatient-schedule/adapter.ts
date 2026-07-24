import type { BookingOption, ScheduleRecord } from '@/apis/booking/outpatient-schedule'
import type { ScheduleEvent, ScheduleResource } from '@/components/schedule/type'

export function toScheduleEvents(rows: ScheduleRecord[]): ScheduleEvent[] {
  return rows.map((row) => ({
    key: row.id,
    date: row.date,
    title: `${row.doctorName} · ${row.roomName}`,
    start: row.startTime,
    end: row.endTime,
    status: row.status,
    resourceKey: row.doctorId,
    raw: row
  }))
}

export function toDoctorResources(options: BookingOption[]): ScheduleResource[] {
  return options.map((option) => ({
    key: option.value,
    name: option.label,
    color: option.color,
    raw: option
  }))
}
