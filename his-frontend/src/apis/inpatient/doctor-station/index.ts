import type { PageDTO } from '../../type'
import {
  mockCancelStopInpatientMedicalOrders,
  mockCreateInpatientDiagnosis,
  mockCreateInpatientDischargeAdvice,
  mockCreateInpatientMedicalOrder,
  mockDeleteInpatientDiagnosis,
  mockDeleteInpatientMedicalOrders,
  mockGetInpatientMedicalRecord,
  mockGetInpatientMedicalRecordHistoryDetail,
  mockGroupInpatientMedicalOrders,
  mockListInpatientDiagnosisBrief,
  mockListInpatientMedicalRecordDocumentTypes,
  mockPageInpatientDiagnosisRecords,
  mockPageInpatientDoctorPatientQueue,
  mockPageInpatientMedicalOrderCatalog,
  mockPageInpatientMedicalOrders,
  mockPageInpatientMedicalRecordHistory,
  mockSaveInpatientMedicalRecord,
  mockSignInpatientMedicalOrders,
  mockStopInpatientMedicalOrders,
  mockUngroupInpatientMedicalOrders,
  mockUpdateInpatientDiagnosis,
  mockUpdateInpatientMedicalOrder,
  mockWithdrawInpatientMedicalOrders
} from './mock'
import {
  realCreateInpatientDischargeAdvice,
  realGetInpatientMedicalRecord,
  realGetInpatientMedicalRecordHistoryDetail,
  realPageInpatientDoctorPatientQueue,
  realPageInpatientMedicalRecordHistory,
  realSaveInpatientMedicalRecord
} from './patient-real'
import {
  createUnsupportedInpatientDoctorStationCapabilityError,
  inpatientDoctorStationCapabilities,
  isInpatientDoctorStationRealApiEnabled
} from './mode'
import {
  realCreateInpatientDiagnosis,
  realDeleteInpatientDiagnosis,
  realListInpatientDiagnosisBrief,
  realPageInpatientDiagnosisRecords,
  realUpdateInpatientDiagnosis
} from './diagnosis-real'
import {
  realCreateInpatientMedicalOrder,
  realDeleteInpatientMedicalOrders,
  realPageInpatientMedicalOrders,
  realUpdateInpatientMedicalOrder
} from './orders-real'
import {
  realCancelStopInpatientMedicalOrders,
  realGroupInpatientMedicalOrders,
  realSignInpatientMedicalOrders,
  realStopInpatientMedicalOrders,
  realUngroupInpatientMedicalOrders,
  realWithdrawInpatientMedicalOrders
} from './orders-lifecycle-real'
import { realPageInpatientMedicalOrderCatalog } from './orders-catalog-real'
import type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  GroupInpatientMedicalOrdersDTO,
  DeleteInpatientDiagnosisDTO,
  InpatientDiagnosisBriefVO,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDischargeAdviceVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientDoctorPatientQueueQuery,
  InpatientMedicalOrderBatchActionDTO,
  InpatientMedicalOrderBatchResultVO,
  InpatientMedicalOrderCatalogItemVO,
  InpatientMedicalOrderCatalogQuery,
  InpatientMedicalOrderQuery,
  InpatientMedicalOrderRecordVO,
  InpatientMedicalRecordDocumentTypeOptionVO,
  InpatientMedicalRecordHistoryDetailVO,
  InpatientMedicalRecordHistoryItemVO,
  InpatientMedicalRecordHistoryQuery,
  InpatientMedicalRecordVO,
  SaveInpatientMedicalRecordDTO,
  UpdateInpatientDiagnosisDTO,
  UpdateInpatientMedicalOrderDTO
} from './type'

export {
  inpatientDoctorStationApiMode,
  inpatientDoctorStationCapabilities,
  isInpatientDoctorStationRealApiEnabled
} from './mode'
export type { InpatientDoctorStationApiMode } from './mode'

/** 分页查询住院医生工作站在院患者队列。 */
export function pageInpatientDoctorPatientQueue(
  query: InpatientDoctorPatientQueueQuery
): Promise<PageDTO<InpatientDoctorPatientQueueItemVO>> {
  return isInpatientDoctorStationRealApiEnabled
    ? realPageInpatientDoctorPatientQueue(query)
    : mockPageInpatientDoctorPatientQueue(query)
}

/** 分页查询患者跨住院就诊的历史病历摘要。 */
export function pageInpatientMedicalRecordHistory(
  query: InpatientMedicalRecordHistoryQuery
): Promise<PageDTO<InpatientMedicalRecordHistoryItemVO>> {
  return isInpatientDoctorStationRealApiEnabled
    ? realPageInpatientMedicalRecordHistory(query)
    : mockPageInpatientMedicalRecordHistory(query)
}

/** 获取单份历史病历详情；历史详情始终以只读方式展示。 */
export function getInpatientMedicalRecordHistoryDetail(
  recordId: string
): Promise<InpatientMedicalRecordHistoryDetailVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realGetInpatientMedicalRecordHistoryDetail(recordId)
    : mockGetInpatientMedicalRecordHistoryDetail(recordId)
}

/** 获取允许新建的住院病历文书类型。 */
export function listInpatientMedicalRecordDocumentTypes(): Promise<
  InpatientMedicalRecordDocumentTypeOptionVO[]
> {
  if (!inpatientDoctorStationCapabilities.medicalRecordDocumentTypes) {
    return Promise.reject(
      createUnsupportedInpatientDoctorStationCapabilityError('病历文书类型')
    )
  }
  return mockListInpatientMedicalRecordDocumentTypes()
}

/** 获取一份住院病历；当前住院草稿可编辑，已签名及历史文书只读。 */
export function getInpatientMedicalRecord(recordId: string): Promise<InpatientMedicalRecordVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realGetInpatientMedicalRecord(recordId)
    : mockGetInpatientMedicalRecord(recordId)
}

/** 新建或更新住院病历草稿。 */
export function saveInpatientMedicalRecord(
  data: SaveInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realSaveInpatientMedicalRecord(data)
    : mockSaveInpatientMedicalRecord(data)
}

/** 分页查询当前住院就诊的诊断记录。 */
export function pageInpatientDiagnosisRecords(
  query: InpatientDiagnosisRecordQuery
): Promise<PageDTO<InpatientDiagnosisRecordVO>> {
  return isInpatientDoctorStationRealApiEnabled
    ? realPageInpatientDiagnosisRecords(query)
    : mockPageInpatientDiagnosisRecords(query)
}

/** 获取租户范围内的历史诊断名称。 */
export function listInpatientDiagnosisBrief(): Promise<InpatientDiagnosisBriefVO[]> {
  return isInpatientDoctorStationRealApiEnabled
    ? realListInpatientDiagnosisBrief()
    : mockListInpatientDiagnosisBrief()
}

/** 新增一条住院诊断。 */
export function createInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realCreateInpatientDiagnosis(data)
    : mockCreateInpatientDiagnosis(data)
}

/** 修改一条住院诊断。 */
export function updateInpatientDiagnosis(data: UpdateInpatientDiagnosisDTO): Promise<void> {
  return isInpatientDoctorStationRealApiEnabled
    ? realUpdateInpatientDiagnosis(data)
    : mockUpdateInpatientDiagnosis(data)
}

/** 删除一条住院诊断。 */
export function deleteInpatientDiagnosis(data: DeleteInpatientDiagnosisDTO): Promise<void> {
  return isInpatientDoctorStationRealApiEnabled
    ? realDeleteInpatientDiagnosis(data)
    : mockDeleteInpatientDiagnosis(data)
}

/** 分页查询当前住院医嘱。 */
export function pageInpatientMedicalOrders(
  query: InpatientMedicalOrderQuery
): Promise<PageDTO<InpatientMedicalOrderRecordVO>> {
  return isInpatientDoctorStationRealApiEnabled
    ? realPageInpatientMedicalOrders(query)
    : mockPageInpatientMedicalOrders(query)
}

/** 分页搜索住院医嘱项目目录。 */
export function pageInpatientMedicalOrderCatalog(
  query: InpatientMedicalOrderCatalogQuery
): Promise<PageDTO<InpatientMedicalOrderCatalogItemVO>> {
  if (!inpatientDoctorStationCapabilities.medicalOrderCatalog) {
    return Promise.reject(
      createUnsupportedInpatientDoctorStationCapabilityError('医嘱目录')
    )
  }
  return isInpatientDoctorStationRealApiEnabled
    ? realPageInpatientMedicalOrderCatalog(query)
    : mockPageInpatientMedicalOrderCatalog(query)
}

/** 新增一条住院医嘱草稿。 */
export function createInpatientMedicalOrder(
  data: CreateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realCreateInpatientMedicalOrder(data)
    : mockCreateInpatientMedicalOrder(data)
}

/** 修改一条住院医嘱草稿。 */
export function updateInpatientMedicalOrder(
  orderId: string,
  data: UpdateInpatientMedicalOrderDTO,
  options: {
    source?: InpatientMedicalOrderRecordVO['source']
    current?: InpatientMedicalOrderRecordVO
  } = {}
): Promise<InpatientMedicalOrderRecordVO> {
  if (isInpatientDoctorStationRealApiEnabled && options.source === 'real' && options.current) {
    return realUpdateInpatientMedicalOrder(orderId, data, options.current)
  }
  return mockUpdateInpatientMedicalOrder(orderId, data)
}

/** 批量删除待签发住院医嘱。 */
export function deleteInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realDeleteInpatientMedicalOrders(data)
    : mockDeleteInpatientMedicalOrders(data)
}

/** 批量并组待签发药品医嘱。 */
export function groupInpatientMedicalOrders(
  data: GroupInpatientMedicalOrdersDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realGroupInpatientMedicalOrders(data)
    : mockGroupInpatientMedicalOrders(data)
}

/** 批量拆组待签发药品医嘱。 */
export function ungroupInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realUngroupInpatientMedicalOrders(data)
    : mockUngroupInpatientMedicalOrders(data)
}

/** 批量签发待签发住院医嘱。 */
export function signInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realSignInpatientMedicalOrders(data)
    : mockSignInpatientMedicalOrders(data)
}

/** 批量撤回已签发住院医嘱。 */
export function withdrawInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realWithdrawInpatientMedicalOrders(data)
    : mockWithdrawInpatientMedicalOrders(data)
}

/** 批量停止已签发长期医嘱。 */
export function stopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realStopInpatientMedicalOrders(data)
    : mockStopInpatientMedicalOrders(data)
}

/** 批量取消长期医嘱停嘱。 */
export function cancelStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realCancelStopInpatientMedicalOrders(data)
    : mockCancelStopInpatientMedicalOrders(data)
}

/** 开立出院医嘱；成功后患者移出在院队列。 */
export function createInpatientDischargeAdvice(
  data: CreateInpatientDischargeAdviceDTO
): Promise<InpatientDischargeAdviceVO> {
  return isInpatientDoctorStationRealApiEnabled
    ? realCreateInpatientDischargeAdvice(data)
    : mockCreateInpatientDischargeAdvice(data)
}

export type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  DeleteInpatientDiagnosisDTO,
  GroupInpatientMedicalOrdersDTO,
  InpatientDiagnosisAddResultVO,
  InpatientDiagnosisBriefVO,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDischargeAdviceVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientDoctorPatientQueueQuery,
  InpatientMedicalOrderBatchActionDTO,
  InpatientMedicalOrderBatchItemDTO,
  InpatientMedicalOrderBatchResultVO,
  InpatientMedicalOrderCatalogItemVO,
  InpatientMedicalOrderCatalogQuery,
  InpatientMedicalOrderQuery,
  InpatientMedicalOrderRecordVO,
  InpatientMedicalOrderStatus,
  InpatientMedicalOrderTherapyType,
  InpatientMedicalRecordDocumentTypeCode,
  InpatientMedicalRecordDocumentTypeOptionVO,
  InpatientMedicalRecordHistoryDetailVO,
  InpatientMedicalRecordHistoryItemVO,
  InpatientMedicalRecordHistoryQuery,
  InpatientMedicalRecordStatus,
  InpatientMedicalRecordVO,
  SaveInpatientMedicalRecordDTO,
  SaveInpatientMedicalOrderDraftItemDTO,
  SaveInpatientMedicalOrderDraftsDTO,
  UpdateInpatientDiagnosisDTO,
  UpdateInpatientMedicalOrderDTO
} from './type'
