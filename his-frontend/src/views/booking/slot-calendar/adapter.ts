import type { SlotRecord } from '@/apis/booking/slot-calendar'
import type { TimeSlot } from '@/components/schedule/type'

export function toTimeSlots(rows: SlotRecord[]): TimeSlot[] {
  return rows.map((row) => ({
    key: row.id,
    start: row.startTime,
    end: row.endTime,
    label: `${row.startTime} - ${row.endTime}`,
    capacity: row.capacity,
    remaining: row.remaining,
    status: row.status,
    group: row.group,
    raw: row
  }))
}
