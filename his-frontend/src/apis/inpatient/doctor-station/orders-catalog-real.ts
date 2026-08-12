import { realPageAdviceOrderCatalog } from '@/apis/outpatient/doctor-station/advice-real'

import type { PageDTO } from '../../type'
import type {
  InpatientMedicalOrderCatalogItemVO,
  InpatientMedicalOrderCatalogQuery,
  InpatientMedicalOrderTherapyType
} from './type'

/**
 * 住院开立医嘱复用门诊医生站的真实目录接口。
 *
 * 门诊目录与住院选择器使用相同的基础目录字段，住院侧只补充
 * 住院选择器需要的默认治疗类型字段，不把门诊 DTO 暴露给住院页面。
 */
export async function realPageInpatientMedicalOrderCatalog(
  query: InpatientMedicalOrderCatalogQuery
): Promise<PageDTO<InpatientMedicalOrderCatalogItemVO>> {
  const result = await realPageAdviceOrderCatalog(query)

  return {
    ...result,
    rows: (result.rows ?? []).map((item) => ({
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
      defaultTherapyType: toInpatientTherapyType(item.defaultTherapyType)
    }))
  }
}

function toInpatientTherapyType(value: unknown): InpatientMedicalOrderTherapyType {
  return value === 'LONG_TERM' ? 'LONG_TERM' : 'TEMPORARY'
}
