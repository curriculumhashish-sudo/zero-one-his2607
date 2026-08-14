import dayjs from 'dayjs'

import { mockFindNurseStationPatient } from './mock'
import type {
  NurseStationSaveTemperatureSheetDTO,
  NurseStationSaveTemperatureSheetResultVO,
  NurseStationTemperatureGridDayVO,
  NurseStationTemperatureSheetDataVO,
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
    temperature: '37.4',
    pulse: '88',
    heartRate: '88',
    respiratory: '20',
    specialEvents: [
      { code: 'PHYSICAL_COOLING', displayText: '物理降温', cooledTemperature: '37.4' }
    ]
  },
  {
    encounterId: 'encounter-001',
    patientId: 'patient-001',
    recordDate: '2026-07-30',
    timeSlot: '06',
    vitalSignId: 'temperature-003',
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
    temperature: '37.2',
    pulse: '88',
    heartRate: '88',
    respiratory: '20',
    specialEvents: [{ code: 'SURGERY', displayText: '手术' }]
  }
]

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

export async function mockQueryNurseStationTemperatureSheet(
  query: NurseStationTemperatureSheetQueryDTO
): Promise<NurseStationTemperatureSheetDataVO> {
  await Promise.resolve()
  assertPatient(query.encounterId, query.patientId)
  const count = windowDayCount(query.windowRange)
  const dateTo = dayjs(query.focusRecordDate || undefined).startOf('day')
  const dateFrom = dateTo.subtract(count - 1, 'day')

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
    return { recordDate, slots }
  })

  return { gridDays }
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

export async function mockSaveNurseStationTemperatureSheet(
  data: NurseStationSaveTemperatureSheetDTO
): Promise<NurseStationSaveTemperatureSheetResultVO> {
  await Promise.resolve()
  assertPatient(data.encounterId, data.patientId)
  if (!dayjs(data.recordDate).isValid()) throw new Error('记录日期格式不正确')
  if (data.slots.length > 6) throw new Error('单次最多保存六个时次')

  const results = data.slots.map((input) => {
    if (!timeSlots.includes(input.timeSlot)) {
      return { timeSlot: input.timeSlot, success: false, message: '时次无效' }
    }
    if (!hasSlotContent(input)) {
      return { timeSlot: input.timeSlot, success: false, message: '空白时次无需保存' }
    }

    const current = storedSlots.find(
      (slot) =>
        slot.encounterId === data.encounterId &&
        slot.recordDate === data.recordDate &&
        slot.timeSlot === input.timeSlot
    )
    const saved: StoredTemperatureSlot = {
      ...cloneSlot(input),
      vitalSignId: current?.vitalSignId ?? `temperature-${++vitalSignSeed}`,
      encounterId: data.encounterId,
      patientId: data.patientId,
      recordDate: data.recordDate
    }
    if (current) Object.assign(current, saved)
    else storedSlots.push(saved)
    return { timeSlot: input.timeSlot, success: true, message: '保存成功', slot: cloneSlot(saved) }
  })

  const savedCount = results.filter((item) => item.success).length
  const lastSavedIndex = Math.max(
    -1,
    ...results.filter((item) => item.success).map((item) => timeSlots.indexOf(item.timeSlot))
  )
  return {
    success: savedCount === results.length,
    savedCount,
    nextTimeSlot: timeSlots[lastSavedIndex + 1],
    results
  }
}
