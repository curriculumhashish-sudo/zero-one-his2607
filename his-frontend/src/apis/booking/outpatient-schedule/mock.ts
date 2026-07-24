import type {
  BookingOption,
  RequestContext,
  ScheduleInitResult,
  ScheduleRangeQuery,
  ScheduleRecord,
  ScheduleSavePayload
} from './type'

const departments: BookingOption[] = [
  { value: 'dept-internal', label: '心血管内科' },
  { value: 'dept-respiratory', label: '呼吸内科' },
  { value: 'dept-surgery', label: '普通外科' }
]

const doctors: BookingOption[] = [
  { value: 'doctor-wang', label: '王建国', departmentId: 'dept-internal', color: '#409eff' },
  { value: 'doctor-li', label: '李文静', departmentId: 'dept-respiratory', color: '#67c23a' },
  { value: 'doctor-zhao', label: '赵海峰', departmentId: 'dept-surgery', color: '#e6a23c' }
]

const rooms: BookingOption[] = [
  { value: 'room-101', label: '门诊 101 诊室' },
  { value: 'room-203', label: '门诊 203 诊室' },
  { value: 'room-305', label: '外科 305 诊室' }
]

function formatDate(date: Date) {
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')
  return `${year}-${month}-${day}`
}

function relativeDate(offset: number) {
  const date = new Date()
  date.setHours(12, 0, 0, 0)
  date.setDate(date.getDate() + offset)
  return formatDate(date)
}

function nowText() {
  return new Date().toISOString()
}

let sequence = 7
const schedules: ScheduleRecord[] = [
  {
    id: 'schedule-1',
    departmentId: 'dept-internal',
    departmentName: '心血管内科',
    doctorId: 'doctor-wang',
    doctorName: '王建国',
    roomId: 'room-101',
    roomName: '门诊 101 诊室',
    date: relativeDate(0),
    startTime: '08:00',
    endTime: '12:00',
    capacity: 24,
    fee: 20,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-2',
    departmentId: 'dept-respiratory',
    departmentName: '呼吸内科',
    doctorId: 'doctor-li',
    doctorName: '李文静',
    roomId: 'room-203',
    roomName: '门诊 203 诊室',
    date: relativeDate(0),
    startTime: '14:00',
    endTime: '17:00',
    capacity: 18,
    fee: 15,
    status: 'stopped',
    stopReason: '医生临时参加院内会诊',
    updatedAt: nowText()
  },
  {
    id: 'schedule-3',
    departmentId: 'dept-surgery',
    departmentName: '普通外科',
    doctorId: 'doctor-zhao',
    doctorName: '赵海峰',
    roomId: 'room-305',
    roomName: '外科 305 诊室',
    date: relativeDate(1),
    startTime: '08:30',
    endTime: '11:30',
    capacity: 16,
    fee: 30,
    status: 'draft',
    updatedAt: nowText()
  },
  {
    id: 'schedule-4',
    departmentId: 'dept-internal',
    departmentName: '心血管内科',
    doctorId: 'doctor-wang',
    doctorName: '王建国',
    roomId: 'room-101',
    roomName: '门诊 101 诊室',
    date: relativeDate(2),
    startTime: '14:00',
    endTime: '17:30',
    capacity: 20,
    fee: 20,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-5',
    departmentId: 'dept-respiratory',
    departmentName: '呼吸内科',
    doctorId: 'doctor-li',
    doctorName: '李文静',
    roomId: 'room-203',
    roomName: '门诊 203 诊室',
    date: relativeDate(4),
    startTime: '18:00',
    endTime: '20:00',
    capacity: 12,
    fee: 25,
    status: 'published',
    updatedAt: nowText()
  },
  {
    id: 'schedule-6',
    departmentId: 'dept-surgery',
    departmentName: '普通外科',
    doctorId: 'doctor-zhao',
    doctorName: '赵海峰',
    roomId: 'room-305',
    roomName: '外科 305 诊室',
    date: relativeDate(-1),
    startTime: '09:00',
    endTime: '11:00',
    capacity: 10,
    fee: 30,
    status: 'published',
    updatedAt: nowText()
  }
]

function clone<T>(value: T): T {
  return structuredClone(value)
}

function delay(context: RequestContext = {}, milliseconds = 260) {
  return new Promise<void>((resolve, reject) => {
    if (context.signal?.aborted) {
      reject(new DOMException('请求已取消', 'AbortError'))
      return
    }
    const timer = setTimeout(resolve, milliseconds)
    context.signal?.addEventListener(
      'abort',
      () => {
        clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      },
      { once: true }
    )
  })
}

function findOption(source: BookingOption[], value: string, field: string) {
  const option = source.find((item) => item.value === value)
  if (!option) throw new Error(`${field}不存在或已停用`)
  return option
}

function toRecord(payload: ScheduleSavePayload, id: string): ScheduleRecord {
  if (payload.endTime <= payload.startTime) throw new Error('结束时间必须晚于开始时间')
  const department = findOption(departments, payload.departmentId, '科室')
  const doctor = findOption(doctors, payload.doctorId, '医生')
  const room = findOption(rooms, payload.roomId, '诊室')
  if (doctor.departmentId !== department.value) throw new Error('医生不属于所选科室')
  return {
    id,
    departmentId: department.value,
    departmentName: department.label,
    doctorId: doctor.value,
    doctorName: doctor.label,
    roomId: room.value,
    roomName: room.label,
    date: payload.date,
    startTime: payload.startTime,
    endTime: payload.endTime,
    capacity: payload.capacity,
    fee: payload.fee,
    status: payload.publish ? 'published' : 'draft',
    updatedAt: nowText()
  }
}

export async function mockLoadScheduleInit(
  context: RequestContext = {}
): Promise<ScheduleInitResult> {
  await delay(context)
  return clone({ departments, doctors, rooms })
}

export async function mockPageSchedulesByRange(
  query: ScheduleRangeQuery,
  context: RequestContext = {}
) {
  await delay(context, 360)
  const keyword = query.keyword?.trim().toLocaleLowerCase()
  return clone(
    schedules.filter((item) => {
      if (item.date < query.start || item.date > query.end) return false
      if (query.departmentId && item.departmentId !== query.departmentId) return false
      if (query.doctorId && item.doctorId !== query.doctorId) return false
      if (query.roomId && item.roomId !== query.roomId) return false
      if (
        keyword &&
        !`${item.doctorName}${item.departmentName}${item.roomName}`
          .toLocaleLowerCase()
          .includes(keyword)
      )
        return false
      return true
    })
  )
}

export async function mockGetScheduleDetail(id: string, context: RequestContext = {}) {
  await delay(context, 180)
  const record = schedules.find((item) => item.id === id)
  if (!record) throw new Error('排班不存在或已被删除')
  return clone(record)
}

export async function mockCreateSchedule(payload: ScheduleSavePayload) {
  await delay()
  const record = toRecord(payload, `schedule-${sequence++}`)
  schedules.push(record)
  return clone(record)
}

export async function mockUpdateSchedule(id: string, payload: ScheduleSavePayload) {
  await delay()
  const index = schedules.findIndex((item) => item.id === id)
  if (index < 0) throw new Error('排班不存在或已被删除')
  if (schedules[index]?.status === 'stopped') throw new Error('停诊排班不能编辑')
  const record = toRecord(payload, id)
  schedules[index] = record
  return clone(record)
}

export async function mockPublishSchedule(id: string) {
  await delay()
  const record = schedules.find((item) => item.id === id)
  if (!record) throw new Error('排班不存在或已被删除')
  if (record.status === 'stopped') throw new Error('停诊排班不能发布')
  record.status = 'published'
  record.updatedAt = nowText()
  return clone(record)
}

export async function mockStopSchedule(id: string, reason: string) {
  await delay()
  if (!reason.trim()) throw new Error('请输入停诊原因')
  const record = schedules.find((item) => item.id === id)
  if (!record) throw new Error('排班不存在或已被删除')
  record.status = 'stopped'
  record.stopReason = reason.trim()
  record.updatedAt = nowText()
  return clone(record)
}

export async function mockLoadDepartmentOptions(keyword: string, context: RequestContext = {}) {
  await delay(context, 180)
  return clone(departments.filter((item) => item.label.includes(keyword.trim())))
}

export async function mockLoadDoctorOptions(
  keyword: string,
  departmentId?: string | null,
  context: RequestContext = {}
) {
  await delay(context, 180)
  return clone(
    doctors.filter(
      (item) =>
        (!departmentId || item.departmentId === departmentId) && item.label.includes(keyword.trim())
    )
  )
}

export function getMockScheduleSnapshot() {
  return clone(schedules)
}
