import type { PageDTO } from '../../type'
import {
  mockCancelStopInpatientMedicalOrders,
  mockCreateInpatientDischargeAdvice,
  mockCreateInpatientDiagnosis,
  mockCreateInpatientMedicalOrder,
  mockDeleteInpatientDiagnosis,
  mockDeleteInpatientMedicalOrders,
  mockGetInpatientDiagnosisUsageHistory,
  mockGetInpatientDischargeAdvicePreparation,
  mockGetInpatientMedicalRecord,
  mockGetInpatientMedicalRecordHistoryDetail,
  mockGroupInpatientMedicalOrders,
  mockListInpatientMedicalRecordDocumentTypes,
  mockPageInpatientDiagnosisDictionary,
  mockPageInpatientDiagnosisRecords,
  mockPageInpatientDoctorPatientQueue,
  mockPageInpatientMedicalOrderCatalog,
  mockPageInpatientMedicalOrders,
  mockPageInpatientMedicalRecordHistory,
  mockSaveInpatientMedicalRecord,
  mockSignInpatientMedicalRecord,
  mockSignInpatientMedicalOrders,
  mockStopInpatientMedicalOrders,
  mockUngroupInpatientMedicalOrders,
  mockUpdateInpatientDiagnosis,
  mockUpdateInpatientMedicalOrder,
  mockWithdrawInpatientMedicalOrders
} from './mock'
import type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  GroupInpatientMedicalOrdersDTO,
  InpatientDiagnosisDictionaryItemVO,
  InpatientDiagnosisDictionaryQuery,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDiagnosisUsageHistoryQuery,
  InpatientDiagnosisUsageHistoryVO,
  InpatientDischargeAdvicePreparationVO,
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
  SignInpatientMedicalRecordDTO,
  UpdateInpatientDiagnosisDTO,
  UpdateInpatientMedicalOrderDTO
} from './type'

/**
 * 分页查询住院医生工作站在院患者队列。
 *
 * 当前入口转调 Mock。真实接口就绪后，只需在这里替换为 useHttp() 请求，
 * 页面层和测试使用的函数签名保持不变。
 */
export function pageInpatientDoctorPatientQueue(
  query: InpatientDoctorPatientQueueQuery
): Promise<PageDTO<InpatientDoctorPatientQueueItemVO>> {
  return mockPageInpatientDoctorPatientQueue(query)
}

/** 分页查询患者跨住院就诊的历史病历摘要。 */
export function pageInpatientMedicalRecordHistory(
  query: InpatientMedicalRecordHistoryQuery
): Promise<PageDTO<InpatientMedicalRecordHistoryItemVO>> {
  return mockPageInpatientMedicalRecordHistory(query)
}

/** 获取单份历史病历详情；历史详情始终以只读方式展示。 */
export function getInpatientMedicalRecordHistoryDetail(
  recordId: string
): Promise<InpatientMedicalRecordHistoryDetailVO> {
  return mockGetInpatientMedicalRecordHistoryDetail(recordId)
}

/** 获取允许新建的住院病历文书类型。 */
export function listInpatientMedicalRecordDocumentTypes(): Promise<
  InpatientMedicalRecordDocumentTypeOptionVO[]
> {
  return mockListInpatientMedicalRecordDocumentTypes()
}

/** 获取一份住院病历；当前住院草稿可编辑，已签名及历史文书只读。 */
export function getInpatientMedicalRecord(recordId: string): Promise<InpatientMedicalRecordVO> {
  return mockGetInpatientMedicalRecord(recordId)
}

/** 新建或更新住院病历草稿。 */
export function saveInpatientMedicalRecord(
  data: SaveInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  return mockSaveInpatientMedicalRecord(data)
}

/** 签名住院病历，签名后文书转为只读。 */
export function signInpatientMedicalRecord(
  data: SignInpatientMedicalRecordDTO
): Promise<InpatientMedicalRecordVO> {
  return mockSignInpatientMedicalRecord(data)
}

/** 分页查询当前住院就诊的诊断记录。 */
export function pageInpatientDiagnosisRecords(
  query: InpatientDiagnosisRecordQuery
): Promise<PageDTO<InpatientDiagnosisRecordVO>> {
  return mockPageInpatientDiagnosisRecords(query)
}

/** 分页搜索住院诊断字典。 */
export function pageInpatientDiagnosisDictionary(
  query: InpatientDiagnosisDictionaryQuery
): Promise<PageDTO<InpatientDiagnosisDictionaryItemVO>> {
  return mockPageInpatientDiagnosisDictionary(query)
}

/** 获取当前医生最近使用的诊断。 */
export function getInpatientDiagnosisUsageHistory(
  query: InpatientDiagnosisUsageHistoryQuery = {}
): Promise<InpatientDiagnosisUsageHistoryVO[]> {
  return mockGetInpatientDiagnosisUsageHistory(query)
}

/** 新增一条住院诊断。 */
export function createInpatientDiagnosis(
  data: CreateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  return mockCreateInpatientDiagnosis(data)
}

/** 修改一条住院诊断。 */
export function updateInpatientDiagnosis(
  diagnosisId: string,
  data: UpdateInpatientDiagnosisDTO
): Promise<InpatientDiagnosisRecordVO> {
  return mockUpdateInpatientDiagnosis(diagnosisId, data)
}

/** 删除一条住院诊断。 */
export function deleteInpatientDiagnosis(diagnosisId: string, version: number): Promise<void> {
  return mockDeleteInpatientDiagnosis(diagnosisId, version)
}

/** 分页查询当前住院医嘱。 */
export function pageInpatientMedicalOrders(
  query: InpatientMedicalOrderQuery
): Promise<PageDTO<InpatientMedicalOrderRecordVO>> {
  return mockPageInpatientMedicalOrders(query)
}

/** 分页搜索住院医嘱项目目录。 */
export function pageInpatientMedicalOrderCatalog(
  query: InpatientMedicalOrderCatalogQuery
): Promise<PageDTO<InpatientMedicalOrderCatalogItemVO>> {
  return mockPageInpatientMedicalOrderCatalog(query)
}

/** 新增一条住院医嘱草稿。 */
export function createInpatientMedicalOrder(
  data: CreateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  return mockCreateInpatientMedicalOrder(data)
}

/** 修改一条住院医嘱草稿。 */
export function updateInpatientMedicalOrder(
  orderId: string,
  data: UpdateInpatientMedicalOrderDTO
): Promise<InpatientMedicalOrderRecordVO> {
  return mockUpdateInpatientMedicalOrder(orderId, data)
}

/** 批量删除待签发住院医嘱。 */
export function deleteInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockDeleteInpatientMedicalOrders(data)
}

/** 批量并组待签发药品医嘱。 */
export function groupInpatientMedicalOrders(
  data: GroupInpatientMedicalOrdersDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockGroupInpatientMedicalOrders(data)
}

/** 批量拆组待签发药品医嘱。 */
export function ungroupInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockUngroupInpatientMedicalOrders(data)
}

/** 批量签发待签发住院医嘱。 */
export function signInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockSignInpatientMedicalOrders(data)
}

/** 批量撤回已签发住院医嘱。 */
export function withdrawInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockWithdrawInpatientMedicalOrders(data)
}

/** 批量停止已签发长期医嘱。 */
export function stopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockStopInpatientMedicalOrders(data)
}

/** 批量取消长期医嘱停嘱。 */
export function cancelStopInpatientMedicalOrders(
  data: InpatientMedicalOrderBatchActionDTO
): Promise<InpatientMedicalOrderBatchResultVO> {
  return mockCancelStopInpatientMedicalOrders(data)
}

/** 加载出院医嘱弹窗和前置校验信息。 */
export function getInpatientDischargeAdvicePreparation(
  encounterId: string,
  patientId: string
): Promise<InpatientDischargeAdvicePreparationVO> {
  return mockGetInpatientDischargeAdvicePreparation(encounterId, patientId)
}

/** 开立出院医嘱；患者进入待出院状态但仍保留在医生站队列。 */
export function createInpatientDischargeAdvice(
  data: CreateInpatientDischargeAdviceDTO
): Promise<InpatientDischargeAdviceVO> {
  return mockCreateInpatientDischargeAdvice(data)
}

export type {
  CreateInpatientDischargeAdviceDTO,
  CreateInpatientDiagnosisDTO,
  CreateInpatientMedicalOrderDTO,
  GroupInpatientMedicalOrdersDTO,
  InpatientDiagnosisDictionaryItemVO,
  InpatientDiagnosisDictionaryQuery,
  InpatientDiagnosisRecordQuery,
  InpatientDiagnosisRecordVO,
  InpatientDiagnosisUsageHistoryQuery,
  InpatientDiagnosisUsageHistoryVO,
  InpatientDischargeAdviceCheckItemVO,
  InpatientDischargeAdviceCheckLevel,
  InpatientDischargeAdvicePreparationVO,
  InpatientDischargeAdviceVO,
  InpatientDischargeWayOptionVO,
  InpatientDoctorPatientQueueItemVO,
  InpatientDoctorPatientQueueQuery,
  InpatientEncounterStatus,
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
  SignInpatientMedicalRecordDTO,
  UpdateInpatientDiagnosisDTO,
  UpdateInpatientMedicalOrderDTO
} from './type'
