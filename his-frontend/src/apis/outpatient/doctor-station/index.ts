import type { PageDTO } from '../../type'
import {
  mockAcceptPatient,
  mockCompletePatient,
  mockCreateAdviceOrder,
  mockCreateDiagnosis,
  mockDeleteAdviceOrders,
  mockDeleteDiagnosis,
  mockGetAdviceOrders,
  mockGetDiagnoses,
  mockGetMedicalRecord,
  mockGroupAdviceOrders,
  mockPageAdviceOrderCatalog,
  mockPageAdviceOrders,
  mockPageDiagnosisDictionary,
  mockPagePatientDiagnosisHistory,
  mockPagePatientAdviceOrderHistory,
  mockPagePatientAdviceOrderHistoryDetails,
  mockPagePatientMedicalRecordHistory,
  mockLeavePatient,
  mockListDoctorPatientQueue,
  mockSaveAdviceOrders,
  mockSaveMedicalRecord,
  mockSignAdviceOrders,
  mockUngroupAdviceOrders,
  mockUpdateAdviceOrder,
  mockWithdrawAdviceOrders
} from './mock'
import type {
  CreateDoctorAdviceOrderDTO,
  DeleteDoctorAdviceOrdersDTO,
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderHistoryDetailQuery,
  DoctorAdviceOrderHistoryEncounterVO,
  DoctorAdviceOrderHistoryQuery,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderPageVO,
  DoctorAdviceOrderQuery,
  DoctorAdviceOrderVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordHistoryQuery,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientDiagnosisHistoryItemVO,
  DoctorPatientDiagnosisHistoryQuery,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  OpdoctorCreateDiagnosisDTO,
  GroupDoctorAdviceOrdersDTO,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorMedicalRecordDTO,
  SignDoctorAdviceOrdersDTO,
  UngroupDoctorAdviceOrdersDTO,
  UpdateDoctorAdviceOrderDTO,
  WithdrawDoctorAdviceOrdersDTO
} from './type'
import {
  realCreateAdviceOrder,
  realDeleteAdviceOrders,
  realGroupAdviceOrders,
  realPageAdviceOrderCatalog,
  realPageAdviceOrders,
  realPagePatientAdviceOrderHistory,
  realPagePatientAdviceOrderHistoryDetails,
  realSignAdviceOrders,
  realUngroupAdviceOrders,
  realUpdateAdviceOrder,
  realWithdrawAdviceOrders
} from './advice-real'
import {
  realCreateDiagnosis,
  realDeleteDiagnosis,
  realGetDiagnoses,
  realListDoctorPatientQueue,
  realPagePatientDiagnosisHistory
} from './queue-diagnosis-real'
import {
  realGetMedicalRecord,
  realPagePatientMedicalRecordHistory,
  realSaveMedicalRecord
} from './medical-record-real'
import {
  realCheckInpatientAdmission,
  realAcceptPatient,
  realCompletePatient,
  realLeavePatient,
  realListInpatientAdmissionWards,
  realLoadInpatientAdmissionOptions,
  realPageDiagnosisDictionary,
  realRequestInpatientAdmission
} from './station-real'
import { isOutpatientDoctorStationRealApiEnabled } from './mode'

// All outpatient doctor-station API branches use this single mode value.
const useRealApi = isOutpatientDoctorStationRealApiEnabled

/**
 * 门诊医嘱列表级 version 缓存。
 *
 * 后端医嘱域所有变更接口都要求携带“医嘱列表整体 version”，取最近一次查询或
 * 操作响应的值。这里按 encounterId 保存最近一次 version，变更请求由 API 层自动
 * 携带，页面无需感知；查询响应也会刷新缓存。
 */
const encounterVersionCache = new Map<string, number>()

function rememberEncounterVersion(encounterId: string, version?: number) {
  if (version !== undefined) encounterVersionCache.set(encounterId, version)
}

/** 取缓存中的医嘱版本；未查询过则从 0 开始，由 mock/后端决定是否拒绝。 */
/**
 * 按单日查询门诊医生工作站患者队列。
 *
 * 暂时使用 Mock 数据，页面仍只从 index.ts 调用，后续恢复真实接口时无需改页面。
 */
export function listDoctorPatientQueue(
  query: DoctorPatientQueueQuery
): Promise<DoctorPatientQueueItemVO[]> {
  if (useRealApi) return realListDoctorPatientQueue(query)
  return mockListDoctorPatientQueue(query)
}

/** 接诊待诊患者，或读取在诊、完诊患者的就诊上下文 */
export function acceptPatient(encounterId: string): Promise<DoctorPatientContextVO> {
  if (useRealApi) return realAcceptPatient(encounterId)
  return mockAcceptPatient(encounterId)
}

/** 暂离当前患者；在现有三状态模型中，患者会从“在诊”返回“候诊” */
export function leavePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  if (useRealApi) return realLeavePatient(encounterId)
  return mockLeavePatient(encounterId)
}

/** 完成本次门诊就诊，并返回状态更新后的只读上下文 */
export function completePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  if (useRealApi) return realCompletePatient(encounterId)
  return mockCompletePatient(encounterId)
}

/** 获取当前就诊的门诊病历；尚未保存时返回一份空病历 */
export function getMedicalRecord(encounterId: string): Promise<DoctorMedicalRecordVO> {
  if (useRealApi) return realGetMedicalRecord(encounterId)
  return mockGetMedicalRecord(encounterId)
}

/** 保存当前就诊的门诊病历 */
export function saveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  if (useRealApi) return realSaveMedicalRecord(encounterId, data)
  return mockSaveMedicalRecord(encounterId, data)
}

/** 分页查询患者既往已完诊的门诊病历摘要。 */
export function pagePatientMedicalRecordHistory(
  query: DoctorMedicalRecordHistoryQuery
): Promise<PageDTO<DoctorMedicalRecordHistoryItemVO>> {
  if (useRealApi) return realPagePatientMedicalRecordHistory(query)
  return mockPagePatientMedicalRecordHistory(query)
}

/** 获取当前就诊的诊断列表。 */
export function getDiagnoses(encounterId: string, patientId: string): Promise<DoctorDiagnosisListVO> {
  if (useRealApi) return realGetDiagnoses(encounterId, patientId)
  return mockGetDiagnoses(encounterId)
}

/** 分页搜索 ICD 诊断字典 */
export function pageDiagnosisDictionary(
  query: DoctorDiagnosisDictionaryQuery
): Promise<PageDTO<DoctorDiagnosisDictionaryItemVO>> {
  if (useRealApi) return realPageDiagnosisDictionary(query)
  return mockPageDiagnosisDictionary(query)
}

/** 按患者、关键词、分类、主诊断和日期分页查询历史诊断。 */
export function pagePatientDiagnosisHistory(
  query: DoctorPatientDiagnosisHistoryQuery
): Promise<PageDTO<DoctorPatientDiagnosisHistoryItemVO>> {
  if (useRealApi) return realPagePatientDiagnosisHistory(query)
  return mockPagePatientDiagnosisHistory(query)
}

/** 单条新增诊断。 */
export function createDiagnosis(
  data: OpdoctorCreateDiagnosisDTO,
  patientId?: string
): Promise<DoctorDiagnosisListVO> {
  if (useRealApi) return realCreateDiagnosis(data, patientId ?? '')
  return mockCreateDiagnosis(data)
}

/** 单条删除诊断。 */
export function deleteDiagnosis(id: string, patientId?: string): Promise<DoctorDiagnosisListVO> {
  if (useRealApi) return realDeleteDiagnosis(id, patientId ?? '')
  return mockDeleteDiagnosis(id)
}

/** 获取当前就诊的门诊医嘱列表 */
export function getAdviceOrders(encounterId: string): Promise<DoctorAdviceOrderListVO> {
  return mockGetAdviceOrders(encounterId).then((result) => {
    rememberEncounterVersion(encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 按条件分页查询当前就诊的医嘱。 */
export function pageAdviceOrders(query: DoctorAdviceOrderQuery): Promise<DoctorAdviceOrderPageVO> {
  if (useRealApi) {
    return realPageAdviceOrders(query).then((result) => {
      rememberEncounterVersion(query.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockPageAdviceOrders(query).then((result) => {
    rememberEncounterVersion(query.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 分页搜索门诊医嘱项目目录 */
export function pageAdviceOrderCatalog(
  query: DoctorAdviceOrderCatalogQuery
): Promise<PageDTO<DoctorAdviceOrderCatalogItemVO>> {
  if (useRealApi) return realPageAdviceOrderCatalog(query)
  return mockPageAdviceOrderCatalog(query)
}

/** 保存当前就诊的完整门诊医嘱列表 */
export function saveAdviceOrders(
  encounterId: string,
  data: SaveDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  return mockSaveAdviceOrders(encounterId, data).then((result) => {
    rememberEncounterVersion(encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 新增单条草稿医嘱；页面统一保存时调用。 */
export function createAdviceOrder(
  data: CreateDoctorAdviceOrderDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    const encounterVersion = data.encounterVersion
    return realCreateAdviceOrder({ ...data, encounterVersion }).then((result) => {
      rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockCreateAdviceOrder(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 修改单条草稿医嘱；已签发或已撤回医嘱不可修改。 */
export function updateAdviceOrder(
  data: UpdateDoctorAdviceOrderDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realUpdateAdviceOrder(data).then((result) => {
      rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockUpdateAdviceOrder(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 批量删除草稿医嘱。 */
export function deleteAdviceOrders(
  data: DeleteDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realDeleteAdviceOrders(data).then((result) => {
      rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockDeleteAdviceOrders(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 将任意两条及以上草稿医嘱并为一组。 */
export function groupAdviceOrders(
  data: GroupDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realGroupAdviceOrders(data).then((result) => {
      rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockGroupAdviceOrders(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 拆除选中草稿医嘱的分组。 */
export function ungroupAdviceOrders(
  data: UngroupDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realUngroupAdviceOrders(data).then((result) => {
      rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
      return result
    })
  }
  return mockUngroupAdviceOrders(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 批量签发草稿医嘱。 */
export function signAdviceOrders(
  data: SignDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realSignAdviceOrders(data)
  }
  return mockSignAdviceOrders(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 批量撤回已签发医嘱。 */
export function withdrawAdviceOrders(
  data: WithdrawDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  if (useRealApi) {
    return realWithdrawAdviceOrders(data)
  }
  return mockWithdrawAdviceOrders(data).then((result) => {
    rememberEncounterVersion(data.encounterId, result.encounterVersion ?? result.version)
    return result
  })
}

/** 分页查询患者既往存在正式医嘱的门诊就诊。 */
export function pagePatientAdviceOrderHistory(
  query: DoctorAdviceOrderHistoryQuery
): Promise<PageDTO<DoctorAdviceOrderHistoryEncounterVO>> {
  if (useRealApi) return realPagePatientAdviceOrderHistory(query)
  return mockPagePatientAdviceOrderHistory(query)
}

/** 分页查询某次历史就诊的已签发或已撤回医嘱。 */
export function pagePatientAdviceOrderHistoryDetails(
  query: DoctorAdviceOrderHistoryDetailQuery
): Promise<PageDTO<DoctorAdviceOrderVO>> {
  if (useRealApi) return realPagePatientAdviceOrderHistoryDetails(query)
  return mockPagePatientAdviceOrderHistoryDetails(query)
}

export type {
  CreateDoctorAdviceOrderDTO,
  DeleteDoctorAdviceOrdersDTO,
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderCategory,
  DoctorAdviceOrderHistoryDetailQuery,
  DoctorAdviceOrderHistoryEncounterVO,
  DoctorAdviceOrderHistoryQuery,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderPageVO,
  DoctorAdviceOrderQuery,
  DoctorAdviceOrderStatus,
  DoctorAdviceOrderVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorDiagnosisVO,
  DoctorMedicalRecordHistoryItemVO,
  DoctorMedicalRecordHistoryQuery,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientDiagnosisHistoryItemVO,
  DoctorPatientDiagnosisHistoryQuery,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  OpdoctorCreateDiagnosisDTO,
  DoctorVisitStatus,
  GroupDoctorAdviceOrdersDTO,
  SaveDoctorAdviceOrderItemDTO,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorDiagnosisItemDTO,
  SaveDoctorMedicalRecordDTO,
  SignDoctorAdviceOrdersDTO,
  UngroupDoctorAdviceOrdersDTO,
  UpdateDoctorAdviceOrderDTO,
  WithdrawDoctorAdviceOrdersDTO
} from './type'

export {
  checkInpatientAdmission,
  listInpatientAdmissionWards,
  loadInpatientAdmissionOptions,
  requestInpatientAdmission
} from './inpatient-admission'

export { applyDoctorRefund, queryDoctorRefundItems } from './refund'

export type { DoctorRefundApplicationDTO, DoctorRefundItemVO, DoctorRefundQuery } from './refund'

export { queryDoctorPrescriptionSheets } from './prescription-sheet'

export type {
  DoctorPrescriptionScope,
  DoctorPrescriptionSheetQuery,
  DoctorPrescriptionSheetVO
} from './prescription-sheet'

export type {
  CheckInpatientAdmissionVO,
  InpatientAdmissionDepartmentVO,
  InpatientAdmissionDiagnosisDTO,
  InpatientAdmissionFormOptionsVO,
  InpatientAdmissionOptionVO,
  InpatientAdmissionStatus,
  InpatientAdmissionWardVO,
  RequestInpatientAdmissionDTO,
  RequestInpatientAdmissionVO
} from './inpatient-admission'
