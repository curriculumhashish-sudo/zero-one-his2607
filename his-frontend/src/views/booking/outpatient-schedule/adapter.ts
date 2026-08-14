import type { BookingOption, ScheduleRecord } from '@/apis/booking/outpatient-schedule'
import type { ScheduleEvent, ScheduleResource } from '@/components/schedule/type'

export function resolveScheduleDoctor(
  record: ScheduleRecord,
  doctors: BookingOption[],
  originalPractitionerName?: string
): ScheduleRecord {
  const originalPractitionerId =
    record.originalPractitionerId || record.practitionerId || record.doctorId
  const substitutePractitionerId = record.substitutePractitionerId?.trim()

  if (!substitutePractitionerId) {
    const mappedDoctorName = doctors.find(
      (doctor) => doctor.value === originalPractitionerId
    )?.label
    const recordDoctorName =
      record.doctorName && record.doctorName !== originalPractitionerId
        ? record.doctorName
        : undefined
    const doctorName =
      originalPractitionerName ||
      record.practitionerName ||
      mappedDoctorName ||
      recordDoctorName ||
      '医生名称未返回'
    return {
      ...record,
      originalPractitionerId,
      practitionerId: originalPractitionerId,
      doctorId: originalPractitionerId,
      practitionerName: doctorName,
      doctorName
    }
  }

  const substituteName =
    doctors.find((doctor) => doctor.value === substitutePractitionerId)?.label ||
    (record.substituteDisplay && record.substituteDisplay !== substitutePractitionerId
      ? record.substituteDisplay
      : undefined) ||
    '替诊医生名称未返回'

  return {
    ...record,
    originalPractitionerId,
    practitionerId: substitutePractitionerId,
    doctorId: substitutePractitionerId,
    practitionerName: substituteName,
    doctorName: substituteName,
    substituteDisplay: substituteName
  }
}

export function toScheduleEvents(rows: ScheduleRecord[]): ScheduleEvent[] {
  return rows.map((row) => ({
    key: row.scheduleId ?? row.id,
    // planningHorizonStart 是排班有效期，不是日历当天；日历日期必须使用 days[].date。
    date: row.date || row.planningHorizonStart?.slice(0, 10) || '',
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
