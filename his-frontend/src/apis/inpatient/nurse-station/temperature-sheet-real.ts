import dayjs from 'dayjs'

import { useHttp } from '@/plugins/http'

import { realApiBaseURL } from './patient-real'
import type {
  NurseStationSaveTemperatureSheetDTO,
  NurseStationSaveTemperatureSheetResultVO,
  NurseStationTemperatureGridDayVO,
  NurseStationTemperatureSheetDataVO,
  NurseStationTemperatureSheetQueryDTO,
  NurseStationTemperatureSlotVO,
  NurseStationTemperatureSpecialEventVO,
  NurseStationTemperatureTimeSlot
} from './temperature-sheet-type'

interface BackendDetailItem {
  itemCode?: string
  itemValue?: string
  remark?: string
  termCode?: string
}

interface BackendSlotCell {
  timeSlot?: string
  vitalSignId?: string
  detailItems?: BackendDetailItem[]
}

interface BackendGridRow {
  recordDate?: string
  slotCells?: BackendSlotCell[]
}

interface BackendTemperatureResponse {
  gridRows?: BackendGridRow[]
  pagination?: {
    dateFrom?: string
    dateTo?: string
  }
}

interface BackendTemperatureSaveResponse {
  busNo?: string
  nextTimeSlot?: string
  vitalSignId?: string
}

const timeSlots: NurseStationTemperatureTimeSlot[] = ['02', '06', '10', '14', '18', '22']
const fieldCodeMap = {
  temperature: 'TEMP',
  pulse: 'PULSE',
  heartRate: 'HEART_RATE',
  respiratory: 'RESPIRATION',
  systolicPressure: 'SYSTOLIC_PRESSURE',
  diastolicPressure: 'DIASTOLIC_PRESSURE'
} as const

function getRealHttp() {
  return useHttp()
}

function readString(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function valueOf(items: BackendDetailItem[], code: string) {
  const item = items.find((candidate) => candidate.itemCode?.toUpperCase() === code)
  return readString(item?.itemValue)
}

function eventFromDetail(item: BackendDetailItem): NurseStationTemperatureSpecialEventVO | null {
  const code = readString(item.itemValue) as NurseStationTemperatureSpecialEventVO['code'] | undefined
  if (!code) return null
  try {
    const parsed = JSON.parse(item.remark ?? '') as Partial<NurseStationTemperatureSpecialEventVO>
    return {
      code: (parsed.code as NurseStationTemperatureSpecialEventVO['code']) || code,
      displayText: parsed.displayText || item.termCode || code,
      eventTime: parsed.eventTime,
      cooledTemperature: parsed.cooledTemperature
    }
  } catch {
    return { code, displayText: item.termCode || code }
  }
}

function mapSlot(cell: BackendSlotCell): NurseStationTemperatureSlotVO | null {
  const timeSlot = cell.timeSlot as NurseStationTemperatureTimeSlot | undefined
  if (!timeSlot || !timeSlots.includes(timeSlot)) return null

  const detailItems = cell.detailItems ?? []
  const specialEvents = detailItems.flatMap((item) => {
    if (item.itemCode?.toUpperCase() !== 'SPECIAL_EVENT') return []
    const event = eventFromDetail(item)
    return event ? [event] : []
  })

  return {
    timeSlot,
    vitalSignId: readString(cell.vitalSignId),
    temperature: valueOf(detailItems, fieldCodeMap.temperature),
    pulse: valueOf(detailItems, fieldCodeMap.pulse),
    heartRate: valueOf(detailItems, fieldCodeMap.heartRate),
    respiratory: valueOf(detailItems, fieldCodeMap.respiratory),
    systolicPressure: valueOf(detailItems, fieldCodeMap.systolicPressure),
    diastolicPressure: valueOf(detailItems, fieldCodeMap.diastolicPressure),
    specialEvents
  }
}

function emptyDay(recordDate: string): NurseStationTemperatureGridDayVO {
  return {
    recordDate,
    slots: timeSlots.map((timeSlot) => ({ timeSlot, specialEvents: [] }))
  }
}

function getDateWindow(query: NurseStationTemperatureSheetQueryDTO, response: BackendTemperatureResponse) {
  const count = query.windowRange === '3d' ? 3 : query.windowRange === '14d' ? 14 : 7
  const fallbackDateTo = query.focusRecordDate || dayjs().format('YYYY-MM-DD')
  const dateTo = dayjs(response.pagination?.dateTo || fallbackDateTo).startOf('day')
  const dateFrom = dayjs(response.pagination?.dateFrom || dateTo.subtract(count - 1, 'day'))
  return { dateFrom, dateTo, count }
}

function mapResponse(
  response: BackendTemperatureResponse,
  query: NurseStationTemperatureSheetQueryDTO
): NurseStationTemperatureSheetDataVO {
  const window = getDateWindow(query, response)
  const rowMap = new Map((response.gridRows ?? []).map((row) => [row.recordDate ?? '', row]))
  const gridDays = Array.from({ length: window.count }, (_, index) => {
    const recordDate = window.dateFrom.add(index, 'day').format('YYYY-MM-DD')
    const source = rowMap.get(recordDate)
    const day = emptyDay(recordDate)
    for (const sourceCell of source?.slotCells ?? []) {
      const slot = mapSlot(sourceCell)
      const target = slot && day.slots.find((item) => item.timeSlot === slot.timeSlot)
      if (slot && target) Object.assign(target, slot)
    }
    return day
  })
  return { gridDays }
}

export async function realQueryNurseStationTemperatureSheet(
  query: NurseStationTemperatureSheetQueryDTO
): Promise<NurseStationTemperatureSheetDataVO> {
  const response = await getRealHttp().get<BackendTemperatureResponse>(
    '/temperature/get-data',
    {
      encounterId: query.encounterId,
      patientId: query.patientId,
      windowRange: query.windowRange,
      focusRecordDate: query.focusRecordDate
    },
    { baseURL: realApiBaseURL }
  )
  return mapResponse(response.data ?? {}, query)
}

function toDetailItems(slot: NurseStationSaveTemperatureSheetDTO['slots'][number]) {
  const fields = Object.entries(fieldCodeMap).flatMap(([field, itemCode]) => {
    const value = slot[field as keyof NurseStationTemperatureSlotVO] as string | undefined
    return value ? [{ itemCode, itemValue: value }] : []
  })
  const events = slot.specialEvents.map((event) => ({
    itemCode: 'SPECIAL_EVENT',
    itemValue: event.code,
    termCode: event.displayText,
    remark: JSON.stringify(event)
  }))
  return [...fields, ...events]
}

export async function realSaveNurseStationTemperatureSheet(
  data: NurseStationSaveTemperatureSheetDTO
): Promise<NurseStationSaveTemperatureSheetResultVO> {
  const results: NurseStationSaveTemperatureSheetResultVO['results'] = []
  let backendNextTimeSlot: NurseStationTemperatureTimeSlot | undefined

  for (const slot of data.slots) {
    try {
      const response = await getRealHttp().post<BackendTemperatureSaveResponse>(
        '/temperature/save-record',
        {
          encounterId: data.encounterId,
          patientId: data.patientId,
          recordDate: data.recordDate,
          timeSlot: slot.timeSlot,
          ...(slot.vitalSignId ? { vitalSignId: slot.vitalSignId } : {}),
          detailItems: toDetailItems(slot)
        },
        { baseURL: realApiBaseURL }
      )
      const saved = response.data
      if (!saved?.vitalSignId) throw new Error(response.message || '后端未返回有效的体温记录')
      if (timeSlots.includes(saved.nextTimeSlot as NurseStationTemperatureTimeSlot)) {
        backendNextTimeSlot = saved.nextTimeSlot as NurseStationTemperatureTimeSlot
      }
      results.push({
        timeSlot: slot.timeSlot,
        success: true,
        message: saved.busNo ? `保存成功：${saved.busNo}` : '保存成功',
        slot: { ...slot, vitalSignId: saved.vitalSignId }
      })
    } catch (error) {
      results.push({
        timeSlot: slot.timeSlot,
        success: false,
        message: error instanceof Error ? error.message : '保存体温记录失败'
      })
    }
  }

  const savedCount = results.filter((item) => item.success).length
  const orderedSlots = timeSlots.map((timeSlot) => results.find((item) => item.timeSlot === timeSlot))
  const lastSavedIndex = Math.max(
    -1,
    ...orderedSlots.flatMap((item) => (item?.success ? [timeSlots.indexOf(item.timeSlot)] : []))
  )
  return {
    success: savedCount === data.slots.length,
    savedCount,
    nextTimeSlot: backendNextTimeSlot || timeSlots[lastSavedIndex + 1],
    results
  }
}
