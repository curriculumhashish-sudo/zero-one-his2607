import type { PageDTO } from '../../type'
import type {
  InpatientDischargeAdviceVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientEncounterStatus,
  CreateInpatientDischargeAdviceDTO
} from './type'

/** Swagger 中住院患者队列的行数据。 */
export interface InpatientPatientQueueDTO {
  encounterId?: string
  patientId?: string
  patientName?: string
  gender?: string
  age?: number | string
  diagnosisSummary?: string
  bedName?: string
  bedId?: string
  wardId?: string
  wardName?: string
  houseName?: string
  accountBalance?: number
  statusEnum?: string
  encounterBusNo?: string
  admitTime?: string
  organizationId?: string
  organizationName?: string
}

/** Swagger 中住院患者队列分页数据。 */
export interface InpatientPatientQueuePageDTO {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: InpatientPatientQueueDTO[]
}

/** Swagger 中出院医嘱的响应数据。 */
export interface DischargeOrderDetailDTO {
  orderId?: string
  encounterId?: string
  statusEnum?: string
  dischargeWay?: string
  dischargeTime?: string
  encounterNewStatus?: string
}

/** 前端暂存的出院方式；后端当前没有出院方式字典接口。 */
export const INPATIENT_DISCHARGE_WAY_OPTIONS = [
  { value: '1', label: '医嘱离院' },
  { value: '2', label: '医嘱转院' },
  { value: '3', label: '医嘱转社区卫生服务机构' },
  { value: '4', label: '非医嘱离院' },
  { value: '5', label: '死亡' },
  { value: '9', label: '其他' }
] as const

const statusLabels: Record<InpatientEncounterStatus, string> = {
  IN_HOSPITAL: '在院',
  DISCHARGE_PENDING: '待出院'
}

function normalizeStatus(value?: string): InpatientEncounterStatus {
  const normalized = value?.trim().toUpperCase()
  if (
    normalized === 'DISCHARGE_PENDING' ||
    normalized === 'PENDING_DISCHARGE' ||
    normalized === '待出院' ||
    normalized?.includes('DISCHARGE')
  ) {
    return 'DISCHARGE_PENDING'
  }
  return 'IN_HOSPITAL'
}

function formatAge(age?: number | string) {
  if (age === undefined || age === null || age === '') return ''
  const text = String(age)
  return text.endsWith('岁') ? text : `${text}岁`
}

/** 将 Swagger 队列分页 DTO 转为工作站现有分页模型。 */
export function mapInpatientPatientQueuePage(
  page: InpatientPatientQueuePageDTO
): PageDTO<InpatientDoctorPatientQueueItemVO> {
  const rows = (page.rows ?? []).map(mapInpatientPatientQueueItem)
  return {
    pageIndex: page.pageIndex ?? 1,
    pageSize: page.pageSize ?? rows.length,
    total: page.total ?? rows.length,
    pages: page.pages ?? (rows.length ? 1 : 0),
    rows
  }
}

/** 将 Swagger 队列行 DTO 转为工作站患者队列模型。 */
export function mapInpatientPatientQueueItem(
  row: InpatientPatientQueueDTO
): InpatientDoctorPatientQueueItemVO {
  const encounterId = row.encounterId ?? row.patientId ?? ''
  const patientId = row.patientId ?? encounterId
  const encounterStatus = normalizeStatus(row.statusEnum)

  return {
    encounterId,
    patientId,
    patientName: row.patientName ?? '',
    genderName: row.gender ?? '',
    ageText: formatAge(row.age),
    patientNo: patientId,
    inpatientNo: row.encounterBusNo ?? encounterId,
    admissionAt: row.admitTime ?? '',
    departmentName: row.organizationName || row.houseName || row.wardName || '',
    wardId: row.wardId ?? '',
    wardName: row.wardName ?? '',
    bedId: row.bedId ?? '',
    bedName: row.bedName ?? '',
    encounterStatus,
    encounterStatusName: statusLabels[encounterStatus],
    diagnosisSummary: row.diagnosisSummary,
    version: 0
  }
}

/** 将 Swagger 出院医嘱响应适配为前端出院医嘱模型。 */
export function mapDischargeOrderDetail(
  detail: DischargeOrderDetailDTO,
  request: CreateInpatientDischargeAdviceDTO
): InpatientDischargeAdviceVO {
  const outWayCode = detail.dischargeWay ?? request.outWayCode
  const outWayName =
    INPATIENT_DISCHARGE_WAY_OPTIONS.find((item) => item.value === outWayCode)?.label ??
    outWayCode

  return {
    id: detail.orderId ?? '',
    encounterId: detail.encounterId ?? request.encounterId,
    patientId: request.patientId,
    plannedDischargeTime: detail.dischargeTime ?? request.plannedDischargeTime,
    outWayCode,
    outWayName,
    reasonText: request.reasonText,
    sourceOrgId: request.sourceOrgId,
    sourceLocationId: request.sourceLocationId,
    createdAt: '',
    createdByName: '',
    version: 0
  }
}
