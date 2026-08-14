import { mockFindNurseStationPatient, mockUpdateNurseStationPatient } from './mock'
import type {
  NurseStationOrderAuditBatchResultVO,
  NurseStationOrderAuditOrderType,
  NurseStationOrderAuditQueryDTO,
  NurseStationOrderAuditRowVO,
  NurseStationRejectOrdersDTO,
  NurseStationVerifyOrdersDTO
} from './order-audit-type'

type MockAuditStatus = 'PENDING' | 'PASSED' | 'REJECTED'

interface MockAuditRecord {
  encounterId: string
  requestId: string
  orderType: NurseStationOrderAuditOrderType
  content: string
  status: MockAuditStatus
}

const records: MockAuditRecord[] = [
  {
    encounterId: 'encounter-001',
    requestId: 'audit-med-001',
    orderType: 'MED_REQUEST',
    content: '注射用头孢曲松钠',
    status: 'PENDING'
  },
  {
    encounterId: 'encounter-001',
    requestId: 'audit-service-001',
    orderType: 'SERVICE_REQUEST',
    content: '常规伤口换药',
    status: 'PENDING'
  },
  {
    encounterId: 'encounter-004',
    requestId: 'audit-med-004',
    orderType: 'MED_REQUEST',
    content: '奥美拉唑肠溶片',
    status: 'PASSED'
  },
  {
    encounterId: 'encounter-006',
    requestId: 'audit-service-006',
    orderType: 'SERVICE_REQUEST',
    content: '血糖监测',
    status: 'PENDING'
  },
  {
    encounterId: 'encounter-007',
    requestId: 'audit-med-007',
    orderType: 'MED_REQUEST',
    content: '氯化钠注射液',
    status: 'PENDING'
  }
]

function toRow(record: MockAuditRecord): NurseStationOrderAuditRowVO {
  return {
    listRowKey: `${record.orderType}:${record.requestId}`,
    requestId: record.requestId,
    orderType: record.orderType,
    content: record.content
  }
}

/** Query the in-memory records with the same minimal contract as the backend. */
export async function mockQueryNurseStationOrderAudits(data: NurseStationOrderAuditQueryDTO) {
  return {
    groups: data.patientContexts.map((patient) => ({
      encounterId: patient.encounterId,
      orders: records
        .filter((record) => record.encounterId === patient.encounterId)
        .filter((record) => record.status === data.auditStatus)
        .map(toRow)
    }))
  }
}

function executeWrite(
  targetStatus: Extract<MockAuditStatus, 'PASSED' | 'REJECTED'>,
  items: NurseStationVerifyOrdersDTO['items']
): NurseStationOrderAuditBatchResultVO {
  if (!items.length) return { success: false, processedCount: 0 }
  const itemKeys = items.map((item) => `${item.orderType}:${item.requestId}`)
  if (new Set(itemKeys).size !== itemKeys.length) {
    return { success: false, processedCount: 0 }
  }

  const targets = items.map((item) =>
    records.find(
      (candidate) =>
        candidate.requestId === item.requestId && candidate.orderType === item.orderType
    )
  )
  if (targets.some((record) => record?.status !== 'PENDING')) {
    return { success: false, processedCount: 0 }
  }

  for (const record of targets) {
    record!.status = targetStatus
    const patient = mockFindNurseStationPatient(record!.encounterId)
    if (patient) {
      mockUpdateNurseStationPatient(record!.encounterId, {
        pendingAuditCount: Math.max(0, (patient.pendingAuditCount ?? 0) - 1)
      })
    }
  }
  return {
    success: true,
    processedCount: targets.length
  }
}

/** Verify selected in-memory medical orders. */
export async function mockVerifyNurseStationOrders(data: NurseStationVerifyOrdersDTO) {
  return executeWrite('PASSED', data.items)
}

/** Reject selected in-memory medical orders. */
export async function mockRejectNurseStationOrders(data: NurseStationRejectOrdersDTO) {
  if (!data.reason.trim()) throw new Error('退回原因不能为空')
  return executeWrite('REJECTED', data.items)
}

/** Test helper for inspecting a shared in-memory record. */
export function mockFindNurseStationOrder(requestId: string) {
  return records.find((record) => record.requestId === requestId)
}
