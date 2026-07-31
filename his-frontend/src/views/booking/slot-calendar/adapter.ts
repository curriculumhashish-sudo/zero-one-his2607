import type { SlotRecord } from '@/apis/booking/slot-calendar'
import type { TimeSlot } from '@/components/schedule/type'

export function toTimeSlots(rows: SlotRecord[]): TimeSlot[] {
  return rows.map((row) => ({
    key: row.slotId,
    start: row.startTime.slice(11, 16) || row.startTime,
    end: row.endTime.slice(11, 16) || row.endTime,
    label: `${row.startTime.slice(11, 16) || row.startTime} - ${row.endTime.slice(11, 16) || row.endTime}`,
    capacity: row.capacity,
    remaining: row.remainingCount,
    status: row.status,
    group: row.group,
    raw: row
  }))
}
