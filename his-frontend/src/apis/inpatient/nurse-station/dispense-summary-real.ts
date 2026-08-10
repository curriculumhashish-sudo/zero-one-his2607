import { useHttp } from '@/plugins/http'

import type { PageDTO } from '../../type'
import { realApiBaseURL } from './patient-real'
import type {
  NurseStationCancelDispenseSummariesDTO,
  NurseStationCreateDispenseSummaryDTO,
  NurseStationCreateDispenseSummaryResultVO,
  NurseStationDispenseOrderRowVO,
  NurseStationDispensePatientGroupVO,
  NurseStationDispensePendingQueryDTO,
  NurseStationDispensePendingResultVO,
  NurseStationDispenseSummaryHeaderVO,
  NurseStationDispenseSummaryLineVO,
  NurseStationDispenseSummaryQueryDTO,
  NurseStationDispenseSummaryStatus,
  NurseStationDispenseSummaryWriteResultVO,
  NurseStationDispenseTherapyFilter,
  NurseStationSubmitDispenseSummariesDTO
} from './dispense-summary-type'

interface BackendDispenseTimePoint {
  dispenseId?: string
  procedureId?: string
  plannedDispenseTime?: string
  quantity?: number
  unitCode?: string
  unitCodeText?: string
  chargeUnitPrice?: number
}

interface BackendDispenseOrderRow {
  medReqId?: string
  medicationId?: string
  medicationName?: string
  specificationText?: string
  therapyEnum?: string
  therapyEnumText?: string
  dosageInstruction?: string
  methodCodeText?: string
  frequencyCodeText?: string
  locationId?: string
  locationName?: string
  timePoints?: BackendDispenseTimePoint[]
}

interface BackendDispensePatientGroup {
  encounterId?: string
  patientId?: string
  patientName?: string
  bedDisplay?: string
  orderRows?: BackendDispenseOrderRow[]
}

interface BackendPendingPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  patientGroups?: BackendDispensePatientGroup[]
  rows?: BackendDispensePatientGroup[]
}

interface BackendSummaryPage {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: NurseStationDispenseSummaryHeaderVO[]
}

function getRealHttp() {
  return useHttp()
}

function unwrapData<T>(response: { data?: T; message?: string }, fallbackMessage: string): T {
  if (response.data === undefined || response.data === null) {
    throw new Error(response.message || fallbackMessage)
  }
  return response.data
}

function readString(value: unknown) {
  return typeof value === 'string' ? value : ''
}

function readNumber(value: unknown) {
  return typeof value === 'number' && Number.isFinite(value) ? value : 0
}

function mapTherapyFilter(filter: NurseStationDispenseTherapyFilter) {
  if (filter === 'LONG') return 'LONG_TERM'
  if (filter === 'TEMP') return 'TEMPORARY'
  return 'all'
}

function mapOrderRow(row: BackendDispenseOrderRow): NurseStationDispenseOrderRowVO {
  return {
    medicationRequestId: readString(row.medReqId),
    medicationId: readString(row.medicationId),
    medicationName: readString(row.medicationName),
    specificationText: readString(row.specificationText),
    therapyType: row.therapyEnum === 'LONG_TERM' ? 'LONG' : 'TEMP',
    therapyTypeText: readString(row.therapyEnumText),
    dosageInstruction: readString(row.dosageInstruction),
    methodText: readString(row.methodCodeText),
    frequencyText: readString(row.frequencyCodeText),
    pharmacyId: readString(row.locationId),
    pharmacyName: readString(row.locationName),
    timePoints: (row.timePoints ?? []).map((point) => ({
      dispenseId: readString(point.dispenseId),
      procedureId: readString(point.procedureId),
      plannedDispenseTime: readString(point.plannedDispenseTime),
      quantity: readNumber(point.quantity),
      unitCode: readString(point.unitCode),
      unitCodeText: readString(point.unitCodeText),
      chargeUnitPrice: readNumber(point.chargeUnitPrice)
    }))
  }
}

function mapPatientGroup(group: BackendDispensePatientGroup): NurseStationDispensePatientGroupVO {
  return {
    encounterId: readString(group.encounterId),
    patientId: readString(group.patientId),
    patientName: readString(group.patientName),
    bedDisplay: readString(group.bedDisplay),
    orderRows: (group.orderRows ?? []).map(mapOrderRow)
  }
}

function isSummaryStatus(value: unknown): value is NurseStationDispenseSummaryStatus {
  return value === 'SUMMARIZED' || value === 'WAIT_REVIEW' || value === 'DISPENSED'
}

function mapSummaryHeader(value: unknown): NurseStationDispenseSummaryHeaderVO {
  if (!value || typeof value !== 'object') throw new Error('后端返回了无效的汇总单数据')
  const row = value as Record<string, unknown>
  if (!isSummaryStatus(row.statusEnum)) throw new Error('后端返回了不支持的汇总单状态')
  return {
    summaryNo: readString(row.summaryNo),
    statusEnum: row.statusEnum,
    desLocationId: readString(row.desLocationId),
    itemCount: readNumber(row.itemCount),
    plannedDispenseTime: readString(row.plannedDispenseTime),
    dispenseTime: readString(row.dispenseTime) || undefined
  }
}

/** Query selectable dispense records and adapt backend field names for the existing page. */
export async function realQueryNurseStationDispensePending(
  query: NurseStationDispensePendingQueryDTO
): Promise<NurseStationDispensePendingResultVO> {
  const response = await getRealHttp().get<BackendPendingPage>(
    '/medicationcollection/query-summary-order-items',
    {
      pageIndex: query.pageIndex,
      pageSize: query.pageSize,
      encounterIds: query.encounterIds.join(','),
      medicineCategoryTab: query.medicineCategory.toLowerCase(),
      therapyFilter: mapTherapyFilter(query.therapyFilter),
      queryDeadline: query.queryDeadline,
      keyword: query.keyword
    },
    { baseURL: realApiBaseURL }
  )
  const page = unwrapData(response, '后端未返回待汇总明细')
  const groups = page.patientGroups ?? page.rows ?? []
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize: page.pageSize ?? query.pageSize,
    total: page.total ?? 0,
    patientGroups: groups.map(mapPatientGroup)
  }
}

/** Create summary slips with the minimal payload accepted by the backend. */
export async function realCreateNurseStationDispenseSummary(
  data: NurseStationCreateDispenseSummaryDTO
): Promise<NurseStationCreateDispenseSummaryResultVO> {
  const response = await getRealHttp().post<NurseStationCreateDispenseSummaryResultVO>(
    '/medicationcollection/summary-medication',
    data,
    { baseURL: realApiBaseURL }
  )
  return unwrapData(response, '后端未返回汇总结果')
}

/** Query generated summary slips. */
export async function realQueryNurseStationDispenseSummaries(
  query: NurseStationDispenseSummaryQueryDTO
): Promise<PageDTO<NurseStationDispenseSummaryHeaderVO>> {
  const response = await getRealHttp().get<BackendSummaryPage>(
    '/medicationcollection/summary',
    query,
    { baseURL: realApiBaseURL }
  )
  const page = unwrapData(response, '后端未返回汇总单列表')
  const pageSize = page.pageSize ?? query.pageSize
  const total = page.total ?? 0
  return {
    pageIndex: page.pageIndex ?? query.pageIndex,
    pageSize,
    total,
    pages: page.pages ?? Math.ceil(total / Math.max(pageSize, 1)),
    rows: (page.rows ?? []).map(mapSummaryHeader)
  }
}

/** Query raw medication rows for one summary slip. */
export async function realGetNurseStationDispenseSummaryDetail(
  summaryNo: string
): Promise<NurseStationDispenseSummaryLineVO[]> {
  const response = await getRealHttp().get<NurseStationDispenseSummaryLineVO[]>(
    '/medicationcollection/detail',
    { summaryNo },
    { baseURL: realApiBaseURL }
  )
  return unwrapData(response, '后端未返回汇总单明细')
}

async function writeSummaryNos(
  path: string,
  data: NurseStationSubmitDispenseSummariesDTO | NurseStationCancelDispenseSummariesDTO
): Promise<NurseStationDispenseSummaryWriteResultVO> {
  const response = await getRealHttp().post<NurseStationDispenseSummaryWriteResultVO>(path, data, {
    baseURL: realApiBaseURL
  })
  return unwrapData(response, '后端未返回汇总单处理结果')
}

export function realSubmitNurseStationDispenseSummaries(
  data: NurseStationSubmitDispenseSummariesDTO
) {
  return writeSummaryNos('/medicationcollection/submit-summary', data)
}

export function realCancelNurseStationDispenseSummaries(
  data: NurseStationCancelDispenseSummariesDTO
) {
  return writeSummaryNos('/medicationcollection/cancel-summary', data)
}
