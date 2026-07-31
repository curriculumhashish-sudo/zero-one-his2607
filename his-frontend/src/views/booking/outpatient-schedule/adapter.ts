import type { BookingOption, ScheduleRecord } from '@/apis/booking/outpatient-schedule'
import type { ScheduleEvent, ScheduleResource } from '@/components/schedule/type'

export function toScheduleEvents(rows: ScheduleRecord[]): ScheduleEvent[] {
  return rows.map((row) => ({
    key: row.scheduleId ?? row.id,
    date: row.planningHorizonStart?.slice(0, 10) ?? row.date,
    title: `${row.doctorName} · ${row.roomName}`,
    start: row.planningHorizonStart?.slice(11, 16) ?? row.startTime,
    end: row.planningHorizonEnd?.slice(11, 16) ?? row.endTime,
    status: row.statusEnum ?? row.status,
    resourceKey: row.practitionerId ?? row.doctorId,
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
