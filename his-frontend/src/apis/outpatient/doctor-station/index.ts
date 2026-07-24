import type { PageDTO } from '../../type'
import {
  mockAcceptPatient,
  mockCompletePatient,
  mockGetAdviceOrders,
  mockGetDiagnoses,
  mockGetMedicalRecord,
  mockGetMedicalRecordTemplate,
  mockGetPatientContext,
  mockListMedicalRecordTemplates,
  mockPageAdviceOrderCatalog,
  mockPageDiagnosisDictionary,
  mockPageDoctorPatientQueue,
  mockLeavePatient,
  mockSaveDiagnoses,
  mockSaveAdviceOrders,
  mockSaveMedicalRecord
} from './mock'
import type {
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderListVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorMedicalRecordTemplateNodeVO,
  DoctorMedicalRecordTemplateVO,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorDiagnosesDTO,
  SaveDoctorMedicalRecordDTO
} from './type'

/**
 * 分页查询门诊医生工作站患者队列。
 *
 * 页面只允许从 index.ts 导入此函数。当前函数转调 mock.ts；
 * 后端接口就绪后，在这里改成 useHttp() 请求并保持函数名和返回类型不变。
 */
export function pageDoctorPatientQueue(
  query: DoctorPatientQueueQuery
): Promise<PageDTO<DoctorPatientQueueItemVO>> {
  return mockPageDoctorPatientQueue(query)
}

/** 获取患者本次就诊的完整上下文 */
export function getPatientContext(encounterId: string): Promise<DoctorPatientContextVO> {
  return mockGetPatientContext(encounterId)
}

/** 接诊患者，并返回状态更新后的就诊上下文 */
export function acceptPatient(encounterId: string): Promise<DoctorPatientContextVO> {
  return mockAcceptPatient(encounterId)
}

/** 暂离当前患者；在现有三状态模型中，患者会从“在诊”返回“候诊” */
export function leavePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  return mockLeavePatient(encounterId)
}

/** 完成本次门诊就诊，并返回状态更新后的只读上下文 */
export function completePatient(encounterId: string): Promise<DoctorPatientContextVO> {
  return mockCompletePatient(encounterId)
}

/** 获取当前就诊的门诊病历；尚未保存时返回一份 version 为 0 的空病历 */
export function getMedicalRecord(encounterId: string): Promise<DoctorMedicalRecordVO> {
  return mockGetMedicalRecord(encounterId)
}

/** 保存当前就诊的门诊病历 */
export function saveMedicalRecord(
  encounterId: string,
  data: SaveDoctorMedicalRecordDTO
): Promise<DoctorMedicalRecordVO> {
  return mockSaveMedicalRecord(encounterId, data)
}

/** 获取医生可用的病历模板树 */
export function listMedicalRecordTemplates(): Promise<DoctorMedicalRecordTemplateNodeVO[]> {
  return mockListMedicalRecordTemplates()
}

/** 获取单个病历模板的完整 HTML 内容 */
export function getMedicalRecordTemplate(
  templateId: string
): Promise<DoctorMedicalRecordTemplateVO> {
  return mockGetMedicalRecordTemplate(templateId)
}

/** 获取当前就诊的诊断列表 */
export function getDiagnoses(encounterId: string): Promise<DoctorDiagnosisListVO> {
  return mockGetDiagnoses(encounterId)
}

/** 分页搜索 ICD 诊断字典 */
export function pageDiagnosisDictionary(
  query: DoctorDiagnosisDictionaryQuery
): Promise<PageDTO<DoctorDiagnosisDictionaryItemVO>> {
  return mockPageDiagnosisDictionary(query)
}

/** 保存当前就诊的完整诊断列表 */
export function saveDiagnoses(
  encounterId: string,
  data: SaveDoctorDiagnosesDTO
): Promise<DoctorDiagnosisListVO> {
  return mockSaveDiagnoses(encounterId, data)
}

/** 获取当前就诊的门诊医嘱列表 */
export function getAdviceOrders(encounterId: string): Promise<DoctorAdviceOrderListVO> {
  return mockGetAdviceOrders(encounterId)
}

/** 分页搜索门诊医嘱项目目录 */
export function pageAdviceOrderCatalog(
  query: DoctorAdviceOrderCatalogQuery
): Promise<PageDTO<DoctorAdviceOrderCatalogItemVO>> {
  return mockPageAdviceOrderCatalog(query)
}

/** 保存当前就诊的完整门诊医嘱列表 */
export function saveAdviceOrders(
  encounterId: string,
  data: SaveDoctorAdviceOrdersDTO
): Promise<DoctorAdviceOrderListVO> {
  return mockSaveAdviceOrders(encounterId, data)
}

export type {
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderCatalogQuery,
  DoctorAdviceOrderCategory,
  DoctorAdviceOrderListVO,
  DoctorAdviceOrderVO,
  DoctorDiagnosisDictionaryItemVO,
  DoctorDiagnosisDictionaryQuery,
  DoctorDiagnosisListVO,
  DoctorDiagnosisVO,
  DoctorMedicalRecordTemplateNodeType,
  DoctorMedicalRecordTemplateNodeVO,
  DoctorMedicalRecordTemplateVO,
  DoctorMedicalRecordVO,
  DoctorPatientContextVO,
  DoctorPatientQueueItemVO,
  DoctorPatientQueueQuery,
  DoctorVisitStatus,
  SaveDoctorAdviceOrderItemDTO,
  SaveDoctorAdviceOrdersDTO,
  SaveDoctorDiagnosesDTO,
  SaveDoctorDiagnosisItemDTO,
  SaveDoctorMedicalRecordDTO
} from './type'
