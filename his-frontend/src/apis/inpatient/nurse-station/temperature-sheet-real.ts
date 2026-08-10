import dayjs from 'dayjs'

import { useHttp } from '@/plugins/http'

import { realApiBaseURL } from './patient-real'
import type {
  NurseStationSaveTemperatureSheetDTO,
  NurseStationSaveTemperatureSheetResultVO,
  NurseStationTemperatureDailySupplementVO,
  NurseStationTemperatureGridDayVO,
  NurseStationTemperatureSheetDataVO,
  NurseStationTemperatureSheetHeaderVO,
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
  temperature?: string
  pulse?: string
  heartRate?: string
  respiratory?: string
  systolicPressure?: string
  diastolicPressure?: string
  specialEvents?: NurseStationTemperatureSpecialEventVO[]
}

interface BackendGridRow {
  recordDate?: string
  slotCells?: BackendSlotCell[]
}

interface BackendTemperatureResponse {
  paperHeader?: NurseStationTemperatureSheetHeaderVO
  derivedMeta?: {
    hospitalizationDaysByDate?: Record<string, number> | string
    postoperativeDaysByDate?: Record<string, number> | string
  }
  gridRows?: BackendGridRow[]
  pagination?: {
    dateFrom?: string
    dateTo?: string
    pageIndex?: number
    totalPages?: number
  }
  gridDays?: NurseStationTemperatureGridDayVO[]
}

interface BackendTemperatureSaveResponse {
  busNo?: string
  vitalSignId?: string
}

const timeSlots: NurseStationTemperatureTimeSlot[] = ['02', '06', '10', '14', '18', '22']
const fieldCodeMap = {
  temperature: 'TEMPERATURE',
  pulse: 'PULSE',
  heartRate: 'HEART_RATE',
  respiratory: 'RESPIRATORY',
  systolicPressure: 'SYSTOLIC_PRESSURE',
  diastolicPressure: 'DIASTOLIC_PRESSURE'
} as const

function getRealHttp() {
  return useHttp()
}

function readString(value: unknown) {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined
}

function parseNumberMap(value: Record<string, number> | string | undefined) {
  if (!value) return {}
  if (typeof value === 'object') return value
  try {
    const parsed = JSON.parse(value) as Record<string, unknown>
    return Object.fromEntries(
      Object.entries(parsed).flatMap(([key, item]) =>
        typeof item === 'number' ? [[key, item]] : []
      )
    )
  } catch {
    return {}
  }
}

function mapHeader(
  header: NurseStationTemperatureSheetHeaderVO | undefined,
  fallback?: NurseStationTemperatureSheetHeaderVO
) {
  const merged = {
    ...(fallback ?? {}),
    ...(header ?? {})
  }
  return { ...merged, patientName: merged.patientName || '未知患者' }
}

function valueOf(items: BackendDetailItem[], codes: string[]) {
  const item = items.find((candidate) => codes.includes(candidate.itemCode?.toUpperCase() ?? ''))
  return readString(item?.itemValue)
}

function eventFromDetail(item: BackendDetailItem): NurseStationTemperatureSpecialEventVO | null {
  const code = readString(item.itemValue) as NurseStationTemperatureSpecialEventVO['code'] | undefined
  if (!code) return null
  try {
    const parsed = JSON.parse(item.remark ?? '') as Partial<NurseStationTemperatureSpecialEventVO>
    return {
      code: (parsed.code as NurseStationTemperatureSpecialEventVO['code']) || code,
      displayText: parsed.displayText || code,
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
  const specialEvents = cell.specialEvents ?? detailItems.flatMap((item) => {
    const code = item.itemCode?.toUpperCase() ?? ''
    return code === 'SPECIAL_EVENT' || code === 'EVENT' ? [eventFromDetail(item)].filter(Boolean) : []
  })
  return {
    timeSlot,
    vitalSignId: readString(cell.vitalSignId),
    version: typeof (cell as { version?: unknown }).version === 'number'
      ? (cell as { version: number }).version
      : undefined,
    temperature:
      readString(cell.temperature) || valueOf(detailItems, [fieldCodeMap.temperature, 'TEMP', '体温']),
    pulse: readString(cell.pulse) || valueOf(detailItems, [fieldCodeMap.pulse, '脉搏']),
    heartRate: readString(cell.heartRate) || valueOf(detailItems, [fieldCodeMap.heartRate, 'HEARTRATE', '心率']),
    respiratory:
      readString(cell.respiratory) || valueOf(detailItems, [fieldCodeMap.respiratory, 'RESPIRATION', '呼吸']),
    systolicPressure:
      readString(cell.systolicPressure) || valueOf(detailItems, [fieldCodeMap.systolicPressure, 'SBP', '收缩压']),
    diastolicPressure:
      readString(cell.diastolicPressure) || valueOf(detailItems, [fieldCodeMap.diastolicPressure, 'DBP', '舒张压']),
    specialEvents: specialEvents as NurseStationTemperatureSpecialEventVO[]
  }
}

function emptyDay(recordDate: string): NurseStationTemperatureGridDayVO {
  return {
    recordDate,
    slots: timeSlots.map((timeSlot) => ({ timeSlot, specialEvents: [] })),
    dailySupplement: {}
  }
}

function dateWindow(query: NurseStationTemperatureSheetQueryDTO, response: BackendTemperatureResponse) {
  const count = query.windowRange === '3d' ? 3 : query.windowRange === '14d' ? 14 : 7
  const responseTo = response.pagination?.dateTo
  const focus = query.focusRecordDate || responseTo || dayjs().format('YYYY-MM-DD')
  const dateTo = dayjs(focus).subtract((query.pageIndex ?? 0) * count, 'day').startOf('day')
  return { dateFrom: dateTo.subtract(count - 1, 'day'), dateTo, count }
}

/** 把旧接口 gridRows/detailItems 还原为页面稳定的日期×六时次网格。 */
function mapResponse(
  response: BackendTemperatureResponse,
  query: NurseStationTemperatureSheetQueryDTO
): NurseStationTemperatureSheetDataVO {
  if (response.gridDays?.length) {
    return {
      paperHeader: mapHeader(response.paperHeader, query.patientContext),
      gridDays: response.gridDays,
      hospitalizationDaysByDate: parseNumberMap(response.derivedMeta?.hospitalizationDaysByDate),
      postoperativeDaysByDate: parseNumberMap(response.derivedMeta?.postoperativeDaysByDate),
      pagination: {
        dateFrom: response.pagination?.dateFrom || response.gridDays[0]?.recordDate || '',
        dateTo: response.pagination?.dateTo || response.gridDays.at(-1)?.recordDate || '',
        pageIndex: response.pagination?.pageIndex ?? query.pageIndex ?? 0,
        totalPages: response.pagination?.totalPages ?? 1
      }
    }
  }

  const window = dateWindow(query, response)
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

  return {
    paperHeader: mapHeader(response.paperHeader, query.patientContext),
    gridDays,
    hospitalizationDaysByDate: parseNumberMap(response.derivedMeta?.hospitalizationDaysByDate),
    postoperativeDaysByDate: parseNumberMap(response.derivedMeta?.postoperativeDaysByDate),
    pagination: {
      dateFrom: response.pagination?.dateFrom || window.dateFrom.format('YYYY-MM-DD'),
      dateTo: response.pagination?.dateTo || window.dateTo.format('YYYY-MM-DD'),
      pageIndex: response.pagination?.pageIndex ?? query.pageIndex ?? 0,
      totalPages: response.pagination?.totalPages ?? 1
    }
  }
}

/** 查询旧后端体温网格；初始化配置接口缺失时沿用前端固定六时次配置。 */
export async function realQueryNurseStationTemperatureSheet(
  query: NurseStationTemperatureSheetQueryDTO
): Promise<NurseStationTemperatureSheetDataVO> {
  const response = await getRealHttp().get<BackendTemperatureResponse>(
    '/temperature/get-data',
    {
      encounterId: query.encounterId,
      patientId: query.patientId,
      windowRange: query.windowRange,
      focusRecordDate: query.focusRecordDate,
      pageIndex: query.pageIndex
    },
    { baseURL: realApiBaseURL }
  )
  return mapResponse(response.data ?? {}, query)
}

function toDetailItems(slot: NurseStationSaveTemperatureSheetDTO['slots'][number]) {
  const fields = Object.entries(fieldCodeMap).flatMap(([field, itemCode]) => {
    const value = slot[field as keyof typeof fieldCodeMap] as string | undefined
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

/** 旧接口只有单时次保存，因此前端批量保存会逐条请求并合并结果。 */
export async function realSaveNurseStationTemperatureSheet(
  data: NurseStationSaveTemperatureSheetDTO
): Promise<NurseStationSaveTemperatureSheetResultVO> {
  const results: NurseStationSaveTemperatureSheetResultVO['results'] = []
  for (const slot of data.slots) {
    try {
      const response = await getRealHttp().post<BackendTemperatureSaveResponse>(
        '/temperature/save-record',
        {
          encounterId: data.encounterId,
          patientId: data.patientId,
          recordDate: data.recordDate,
          timeSlot: slot.timeSlot,
          vitalSignId: slot.vitalSignId,
          detailItems: toDetailItems(slot)
        },
        { baseURL: realApiBaseURL }
      )
      results.push({
        timeSlot: slot.timeSlot,
        success: true,
        message: response.data?.busNo ? `保存成功（${response.data.busNo}）` : '保存成功',
        slot: {
          ...slot,
          vitalSignId: response.data?.vitalSignId || slot.vitalSignId,
          version: (slot.expectedVersion ?? 0) + 1
        }
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
    nextTimeSlot: timeSlots[lastSavedIndex + 1],
    results
  }
}
