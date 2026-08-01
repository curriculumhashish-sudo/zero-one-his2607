import dayjs from 'dayjs'

import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationSaveTemperatureSheetDTO,
  NurseStationSaveTemperatureSheetResultVO,
  NurseStationTemperatureDailySupplementVO,
  NurseStationTemperatureGridDayVO,
  NurseStationTemperatureSheetQueryDTO,
  NurseStationTemperatureSlotVO,
  NurseStationTemperatureTimeSlot
} from './temperature-sheet-type'

interface StoredTemperatureSlot extends NurseStationTemperatureSlotVO {
  encounterId: string
  patientId: string
  recordDate: string
}

const timeSlots: NurseStationTemperatureTimeSlot[] = ['02', '06', '10', '14', '18', '22']
let vitalSignSeed = 200

const storedSlots: StoredTemperatureSlot[] = [
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-07-29',
    timeSlot: '10',
    vitalSignId: 'temperature-001',
    version: 1,
    temperature: '38.2',
    pulse: '96',
    heartRate: '96',
    respiratory: '22',
    systolicPressure: '125',
    diastolicPressure: '82',
    specialEvents: []
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-07-29',
    timeSlot: '14',
    vitalSignId: 'temperature-002',
    version: 1,
    temperature: '37.4',
    pulse: '88',
    heartRate: '88',
    respiratory: '20',
    specialEvents: [
      {
        code: 'PHYSICAL_COOLING',
        displayText: '物理降温',
        cooledTemperature: '37.4'
      }
    ]
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-07-30',
    timeSlot: '06',
    vitalSignId: 'temperature-003',
    version: 2,
    temperature: '37.1',
    pulse: '82',
    heartRate: '82',
    respiratory: '18',
    systolicPressure: '120',
    diastolicPressure: '78',
    specialEvents: []
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-07-31',
    timeSlot: '10',
    vitalSignId: 'temperature-004',
    version: 1,
    temperature: '36.8',
    pulse: '78',
    heartRate: '78',
    respiratory: '18',
    specialEvents: []
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-08-01',
    timeSlot: '06',
    vitalSignId: 'temperature-005',
    version: 1,
    temperature: '36.6',
    pulse: '76',
    heartRate: '76',
    respiratory: '17',
    systolicPressure: '118',
    diastolicPressure: '76',
    specialEvents: []
  },
  {
    encounterId: 'encounter-004',
    patientId: 'patient-004',
    recordDate: '2026-08-01',
    timeSlot: '10',
    vitalSignId: 'temperature-101',
    version: 1,
    temperature: '37.2',
    pulse: '88',
    heartRate: '88',
    respiratory: '20',
    specialEvents: [{ code: 'SURGERY', displayText: '手术' }]
  }
]

const dailySupplements = new Map<string, NurseStationTemperatureDailySupplementVO>([
  [
    'encounter-001|2026-07-31',
    {
      stoolCount: '1',
      urineCount: '5',
      intake: '1800',
      urine: '1450',
      other: '0',
      weight: '52.5',
      height: '162'
    }
  ]
])

const idempotentResults = new Map<
  string,
  { fingerprint: string; result: NurseStationSaveTemperatureSheetResultVO }
>()

function cloneSlot(slot: NurseStationTemperatureSlotVO): NurseStationTemperatureSlotVO {
  return { ...slot, specialEvents: slot.specialEvents.map((event) => ({ ...event })) }
}

function assertPatient(encounterId: string, patientId: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient || patient.patientId !== patientId) throw new Error('患者与住院记录不匹配')
  return patient
}

function windowDayCount(range: NurseStationTemperatureSheetQueryDTO['windowRange']) {
  return range === '3d' ? 3 : range === '14d' ? 14 : 7
}

/** Mock：返回补齐日期与六个时次的体温单窗口。 */
export async function mockQueryNurseStationTemperatureSheet(
  query: NurseStationTemperatureSheetQueryDTO
) {
  await Promise.resolve()
  const patient = assertPatient(query.encounterId, query.patientId)
  const count = windowDayCount(query.windowRange)
  const pageIndex = Math.max(0, query.pageIndex ?? 0)
  const focusEnd = query.focusRecordDate ? dayjs(query.focusRecordDate) : dayjs()
  const dateTo = focusEnd.subtract(pageIndex * count, 'day').startOf('day')
  const dateFrom = dateTo.subtract(count - 1, 'day')
  const admissionDate = dayjs(patient.admissionTime?.slice(0, 10) || dateFrom)

  const gridDays: NurseStationTemperatureGridDayVO[] = Array.from({ length: count }, (_, index) => {
    const recordDate = dateFrom.add(index, 'day').format('YYYY-MM-DD')
    const slots = timeSlots.map((timeSlot) => {
      const stored = storedSlots.find(
        (slot) =>
          slot.encounterId === query.encounterId &&
          slot.recordDate === recordDate &&
          slot.timeSlot === timeSlot
      )
      return stored
        ? cloneSlot(stored)
        : ({ timeSlot, specialEvents: [] } satisfies NurseStationTemperatureSlotVO)
    })
    return {
      recordDate,
      slots,
      dailySupplement: { ...(dailySupplements.get(`${query.encounterId}|${recordDate}`) ?? {}) }
    }
  })

  const hospitalizationDaysByDate = Object.fromEntries(
    gridDays.map((day) => [
      day.recordDate,
      Math.max(1, dayjs(day.recordDate).diff(admissionDate, 'day') + 1)
    ])
  )

  return {
    paperHeader: {
      patientName: patient.patientName,
      genderText: patient.genderEnumText,
      ageText: patient.ageText,
      wardName: patient.wardName,
      bedName: patient.bedName,
      encounterBusNo: patient.encounterBusNo,
      admissionDate: patient.admissionTime?.slice(0, 10)
    },
    gridDays,
    hospitalizationDaysByDate,
    postoperativeDaysByDate: {},
    pagination: {
      dateFrom: dateFrom.format('YYYY-MM-DD'),
      dateTo: dateTo.format('YYYY-MM-DD'),
      pageIndex,
      totalPages: 4
    }
  }
}

function hasSlotContent(slot: NurseStationSaveTemperatureSheetDTO['slots'][number]) {
  return (
    [
      slot.temperature,
      slot.pulse,
      slot.heartRate,
      slot.respiratory,
      slot.systolicPressure,
      slot.diastolicPressure
    ].some((value) => String(value ?? '').trim()) || slot.specialEvents.length > 0
  )
}

function cloneResult(result: NurseStationSaveTemperatureSheetResultVO) {
  return {
    ...result,
    results: result.results.map((item) => ({
      ...item,
      slot: item.slot ? cloneSlot(item.slot) : undefined
    }))
  }
}

/** Mock：批量新增或更新同一天最多六个体温单时次。 */
export async function mockSaveNurseStationTemperatureSheet(
  data: NurseStationSaveTemperatureSheetDTO
) {
  await Promise.resolve()
  assertPatient(data.encounterId, data.patientId)
  if (!data.idempotencyKey.trim()) throw new Error('幂等键不能为空')
  if (!dayjs(data.recordDate).isValid()) throw new Error('记录日期格式不正确')
  if (data.slots.length > 6) throw new Error('单次最多保存六个时次')

  const fingerprint = JSON.stringify(data)
  const cached = idempotentResults.get(data.idempotencyKey)
  if (cached) {
    if (cached.fingerprint !== fingerprint) throw new Error('同一幂等键不能用于不同请求')
    return cloneResult(cached.result)
  }

  const results = data.slots.map((input) => {
    if (!timeSlots.includes(input.timeSlot)) {
      return { timeSlot: input.timeSlot, success: false, message: '时次无效' }
    }
    if (!hasSlotContent(input)) {
      return { timeSlot: input.timeSlot, success: false, message: '空白时次无需保存' }
    }
    const invalidTimedEvent = input.specialEvents.some(
      (event) =>
        ['ADMISSION', 'TRANSFER', 'DELIVERY', 'DISCHARGE', 'DEATH'].includes(event.code) &&
        !event.eventTime
    )
    if (invalidTimedEvent) {
      return { timeSlot: input.timeSlot, success: false, message: '特殊事件缺少发生时间' }
    }

    const current = storedSlots.find(
      (slot) =>
        slot.encounterId === data.encounterId &&
        slot.recordDate === data.recordDate &&
        slot.timeSlot === input.timeSlot
    )
    if (current && current.version !== input.expectedVersion) {
      return { timeSlot: input.timeSlot, success: false, message: '该时次已被他人修改' }
    }
    const saved: StoredTemperatureSlot = {
      ...cloneSlot(input),
      vitalSignId: current?.vitalSignId ?? `temperature-${++vitalSignSeed}`,
      version: (current?.version ?? 0) + 1,
      encounterId: data.encounterId,
      patientId: data.patientId,
      recordDate: data.recordDate
    }
    if (current) Object.assign(current, saved)
    else storedSlots.push(saved)
    return { timeSlot: input.timeSlot, success: true, message: '保存成功', slot: cloneSlot(saved) }
  })

  if (data.dailySupplement) {
    dailySupplements.set(`${data.encounterId}|${data.recordDate}`, {
      ...data.dailySupplement
    })
  }
  const savedCount = results.filter((item) => item.success).length
  const lastSavedIndex = Math.max(
    -1,
    ...results.filter((item) => item.success).map((item) => timeSlots.indexOf(item.timeSlot))
  )
  const result: NurseStationSaveTemperatureSheetResultVO = {
    success: savedCount === results.length,
    savedCount,
    nextTimeSlot: timeSlots[lastSavedIndex + 1],
    results
  }
  idempotentResults.set(data.idempotencyKey, { fingerprint, result: cloneResult(result) })
  return cloneResult(result)
}
