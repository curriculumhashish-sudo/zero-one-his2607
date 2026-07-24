import type {
  DoctorAdviceOrderCatalogItemVO,
  DoctorAdviceOrderVO,
  SaveDoctorAdviceOrdersDTO
} from '@/apis/outpatient/doctor-station'
import {
  validateClinicalOrders,
  type AdviceScenarioAdapter,
  type ClinicalOrderCandidate,
  type ClinicalOrderContext,
  type ClinicalOrderDraft
} from '@/components/clinical/order'

/** 将门诊医嘱 VO 转换成公共医嘱草稿。 */
export function toClinicalOrder(order: DoctorAdviceOrderVO): ClinicalOrderDraft {
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
    status: order.status
  }
}

/** 将门诊项目目录 VO 转换成公共选择弹窗候选项。 */
export function toClinicalOrderCandidate(
  item: DoctorAdviceOrderCatalogItemVO
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
    quantityUnit: item.quantityUnit
  }
}

/**
 * 门诊医嘱场景适配器。
 *
 * 公共编辑器不认识门诊 DTO；所有默认值、校验和保存映射都集中在这里。
 * 住院医生站以后提供 inpatientAdviceAdapter 即可复用同一编辑器。
 */
export const outpatientAdviceAdapter = {
  scope: 'outpatient',

  createDraft(
    candidate: ClinicalOrderCandidate,
    context: ClinicalOrderContext
  ): ClinicalOrderDraft {
    return {
      key: 'draft-' + context.encounterKey + '-' + candidate.id,
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
      status: 'DRAFT'
    }
  },

  validate(orders: ClinicalOrderDraft[], _context: ClinicalOrderContext) {
    return validateClinicalOrders(orders)
  },

  mapSaveRequest(
    orders: ClinicalOrderDraft[],
    _context: ClinicalOrderContext,
    version: number
  ): SaveDoctorAdviceOrdersDTO {
    return {
      version,
      items: orders.map((order, index) => ({
        id: order.recordId,
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
        orderNo: index + 1
      }))
    }
  }
} satisfies AdviceScenarioAdapter
