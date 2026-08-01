import type {
  CreateInpatientMedicalOrderDTO,
  InpatientMedicalOrderCatalogItemVO,
  InpatientMedicalOrderRecordVO,
  SaveInpatientMedicalOrderDraftsDTO,
  UpdateInpatientMedicalOrderDTO
} from '@/apis/inpatient/doctor-station'
import {
  validateClinicalOrders,
  type AdviceScenarioAdapter,
  type ClinicalOrderCandidate,
  type ClinicalOrderContext,
  type ClinicalOrderDraft
} from '@/components/clinical/order'
import type { InpatientMedicalOrderTherapyType } from '@/apis/inpatient/doctor-station/type'

/** 将住院医嘱记录转换为公共医嘱编辑器草稿。 */
export function toInpatientClinicalOrder(order: InpatientMedicalOrderRecordVO): ClinicalOrderDraft {
  return {
    key: order.id,
    recordId: order.id,
    catalogId: order.catalogId,
    code: order.code,
    name: order.name,
    category: order.category,
    categoryName: order.categoryName,
    specification: order.specification,
    dose: order.dose,
    unit: order.unit,
    frequency: order.frequency,
    route: order.route,
    days: order.days,
    quantity: order.quantity,
    quantityUnit: order.quantityUnit,
    remark: order.remark,
    status: order.statusName,
    statusCode: order.status,
    therapyType: order.therapyType,
    therapyTypeName: order.therapyTypeName,
    group: order.groupId,
    raw: order
  }
}

/** 将住院医嘱目录项目转换为公共选择弹窗候选项。 */
export function toInpatientOrderCandidate(
  item: InpatientMedicalOrderCatalogItemVO
): ClinicalOrderCandidate {
  return {
    id: item.id,
    code: item.code,
    name: item.name,
    category: item.category,
    categoryName: item.categoryName,
    specification: item.specification,
    defaultDose: item.defaultDose,
    defaultUnit: item.defaultUnit,
    defaultFrequency: item.defaultFrequency,
    defaultRoute: item.defaultRoute,
    defaultDays: item.defaultDays,
    defaultQuantity: item.defaultQuantity,
    quantityUnit: item.quantityUnit,
    defaultTherapyType: item.defaultTherapyType
  }
}

/** 将公共医嘱草稿转换为住院新增参数。 */
export function toCreateInpatientMedicalOrderDTO(
  order: ClinicalOrderDraft,
  index: number,
  context: ClinicalOrderContext
): CreateInpatientMedicalOrderDTO {
  return {
    encounterId: context.encounterKey,
    patientId: context.patientId,
    catalogId: order.catalogId,
    code: order.code,
    name: order.name,
    category: order.category,
    categoryName: order.categoryName,
    specification: order.specification,
    dose: order.dose === undefined ? undefined : Number(order.dose),
    unit: order.unit,
    frequency: order.frequency,
    route: order.route,
    days: order.days === undefined ? undefined : Number(order.days),
    quantity: order.quantity ?? 0,
    quantityUnit: order.quantityUnit ?? '项',
    remark: order.remark,
    therapyType: (order.therapyType ?? 'TEMPORARY') as InpatientMedicalOrderTherapyType,
    orderNo: index + 1
  }
}

/** 将公共医嘱草稿转换为住院修改参数。 */
export function toUpdateInpatientMedicalOrderDTO(
  order: ClinicalOrderDraft,
  index: number,
  version: number
): UpdateInpatientMedicalOrderDTO {
  const context: ClinicalOrderContext = {
    scope: 'inpatient',
    encounterKey: '',
    patientId: ''
  }
  const {
    encounterId: _encounterId,
    patientId: _patientId,
    ...draft
  } = toCreateInpatientMedicalOrderDTO(order, index, context)
  return { ...draft, version }
}

export const inpatientAdviceAdapter = {
  scope: 'inpatient',

  createDraft(
    candidate: ClinicalOrderCandidate,
    context: ClinicalOrderContext
  ): ClinicalOrderDraft {
    const therapyType: InpatientMedicalOrderTherapyType =
      (candidate.defaultTherapyType as InpatientMedicalOrderTherapyType | undefined) ??
      (candidate.category === 'DRUG' ? 'LONG_TERM' : 'TEMPORARY')
    return {
      key: `draft-${context.encounterKey}-${candidate.id}`,
      catalogId: candidate.id,
      code: candidate.code,
      name: candidate.name,
      category: candidate.category,
      categoryName: candidate.categoryName,
      specification: candidate.specification,
      dose: candidate.defaultDose,
      unit: candidate.defaultUnit,
      frequency: candidate.defaultFrequency,
      route: candidate.defaultRoute,
      days: candidate.defaultDays,
      quantity: candidate.defaultQuantity,
      quantityUnit: candidate.quantityUnit,
      status: '未保存',
      statusCode: 'DRAFT',
      therapyType,
      therapyTypeName: therapyType === 'LONG_TERM' ? '长期' : '临时'
    }
  },

  validate(orders: ClinicalOrderDraft[], _context: ClinicalOrderContext) {
    const drafts = orders.filter((order) => !order.statusCode || order.statusCode === 'DRAFT')
    return drafts.length === 0 ? { valid: true, errors: [] } : validateClinicalOrders(drafts)
  },

  mapSaveRequest(
    orders: ClinicalOrderDraft[],
    context: ClinicalOrderContext,
    version: number
  ): SaveInpatientMedicalOrderDraftsDTO {
    return {
      encounterId: context.encounterKey,
      patientId: context.patientId,
      items: orders
        .filter((order) => !order.statusCode || order.statusCode === 'DRAFT')
        .map((order, index) => {
          const data = toCreateInpatientMedicalOrderDTO(order, index, context)
          const saved = order.raw as InpatientMedicalOrderRecordVO | undefined
          const { encounterId: _encounterId, patientId: _patientId, ...draft } = data
          return {
            id: order.recordId,
            version: saved?.version ?? version,
            ...draft
          }
        })
    }
  }
} satisfies AdviceScenarioAdapter
