import { mockFindNurseStationPatient, mockUpdateNurseStationPatient } from './mock'
import type {
  NurseStationExecuteOrdersDTO,
  NurseStationOrderExecuteBatchResultVO,
  NurseStationOrderExecuteListVO,
  NurseStationOrderExecuteQueryDTO,
  NurseStationOrderExecuteRowVO
} from './order-execute-type'

const executeOrders: NurseStationOrderExecuteRowVO[] = [
  {
    listRowKey: 'execute-service-001',
    id: 'execute-service-001',
    busNo: 'SR202608010001',
    prescriptionNo: 'RX202608010001',
    statusEnum: 'verified',
    categoryEnum: 'procedure',
    intentEnum: 'long_term',
    priorityEnum: 'routine',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    authoredTime: '2026-08-01 08:30:00',
    quantity: 1,
    unitCode: 'time',
    requesterId: 'doctor-001',
    locationId: 'ward-001',
    executionCount: 0,
    remark: 'Oxygen inhalation',
    entrustContent: 'Observe oxygen saturation'
  },
  {
    listRowKey: 'execute-service-002',
    id: 'execute-service-002',
    busNo: 'SR202608010002',
    statusEnum: 'verified',
    categoryEnum: 'procedure',
    intentEnum: 'temporary',
    priorityEnum: 'urgent',
    patientId: 'patient-001',
    encounterId: 'encounter-001',
    authoredTime: '2026-08-01 09:10:00',
    quantity: 2,
    unitCode: 'time',
    requesterId: 'doctor-001',
    locationId: 'ward-001',
    executionCount: 0,
    remark: 'Nebulization treatment'
  },
  {
    listRowKey: 'execute-service-004',
    id: 'execute-service-004',
    busNo: 'SR202608010004',
    statusEnum: 'verified',
    categoryEnum: 'procedure',
    intentEnum: 'temporary',
    priorityEnum: 'routine',
    patientId: 'patient-004',
    encounterId: 'encounter-004',
    authoredTime: '2026-08-01 08:15:00',
    quantity: 1,
    unitCode: 'time',
    requesterId: 'doctor-004',
    locationId: 'ward-002',
    executionCount: 0,
    remark: 'Dressing change'
  },
  {
    listRowKey: 'execute-service-006',
    id: 'execute-service-006',
    busNo: 'SR202608010006',
    statusEnum: 'verified',
    categoryEnum: 'procedure',
    intentEnum: 'long_term',
    priorityEnum: 'routine',
    patientId: 'patient-006',
    encounterId: 'encounter-006',
    authoredTime: '2026-08-01 08:42:00',
    quantity: 1,
    unitCode: 'time',
    requesterId: 'doctor-006',
    locationId: 'ward-002',
    executionCount: 0,
    remark: 'Infusion treatment'
  }
]

function cloneOrder(order: NurseStationOrderExecuteRowVO): NurseStationOrderExecuteRowVO {
  return { ...order }
}

/** Return verified service requests grouped by the selected encounters. */
export async function mockQueryNurseStationOrderExecutions(
  query: NurseStationOrderExecuteQueryDTO
): Promise<NurseStationOrderExecuteListVO> {
  await Promise.resolve()
  const encounterIds = [...new Set(query.encounterIds.map((id) => id.trim()).filter(Boolean))]
  return {
    groups: encounterIds.map((encounterId) => ({
      encounterId,
      orders: executeOrders
        .filter((order) => order.encounterId === encounterId && order.statusEnum === 'verified')
        .map(cloneOrder)
    }))
  }
}

function updatePendingCount(encounterId: string) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient) return
  mockUpdateNurseStationPatient(encounterId, {
    pendingExecuteCount: Math.max(0, (patient.pendingExecuteCount ?? 0) - 1)
  })
}

/** Simulate the backend single-order start endpoint for every selected request. */
export async function mockExecuteNurseStationOrders(
  data: NurseStationExecuteOrdersDTO
): Promise<NurseStationOrderExecuteBatchResultVO> {
  await Promise.resolve()
  if (!data.performerId.trim()) throw new Error('执行人不能为空')
  if (data.serviceRequestIds.length < 1 || data.serviceRequestIds.length > 50) {
    throw new Error('单次只能处理 1～50 条医嘱')
  }

  const results = data.serviceRequestIds.map((serviceRequestId) => {
    const order = executeOrders.find((item) => item.id === serviceRequestId)
    if (!order || order.statusEnum !== 'verified') {
      return { serviceRequestId, success: false, message: '医嘱状态已变化' }
    }
    order.statusEnum = 'executing'
    order.performerId = data.performerId
    order.executionCount = (order.executionCount ?? 0) + 1
    updatePendingCount(order.encounterId)
    return { serviceRequestId, success: true, message: '开始执行成功' }
  })
  const processedCount = results.filter((item) => item.success).length
  return {
    success: processedCount === results.length,
    processedCount,
    results
  }
}
