import type { ReturnMedicinePrescriptionBlock, ReturnMedicinePrescriptionItemRow } from './type'
import type { OutpatientReturnMedicineSlipDTO } from '@/apis/pharmacy/return-medicine/outpatient/type'

function inferRowStatus(statusEnum: string): 'pending' | 'partial' | 'completed' | 'blocking' {
  if (statusEnum === 'completed') return 'completed'
  if (statusEnum === 'blocked' || statusEnum === 'error') return 'blocking'
  if (statusEnum === 'partial') return 'partial'
  return 'pending'
}

function inferMedicationType(itemName: string): 'western' | 'chinese' {
  const chineseHerbKeywords = ['丹参', '益母', '当归', '黄芪', '党参', '白术', '茯苓', '甘草', '川芎', '白芍', '熟地', '山药', '枸杞', '陈皮', '半夏', '柴胡', '黄芩', '黄连', '大黄']
  const chineseDrugKeywords = ['丸', '颗粒', '片', '胶囊', '膏', '散', '丹', '口服液', '注射液', '滴丸', '贴膏']
  
  const name = itemName || ''
  if (chineseHerbKeywords.some(kw => name.includes(kw))) {
    return 'chinese'
  }
  if (chineseDrugKeywords.some(kw => name.includes(kw))) {
    const chineseOnlyKeywords = ['丹参滴丸', '益母草颗粒', '妇科千金片']
    if (chineseOnlyKeywords.some(kw => name.includes(kw))) {
      return 'chinese'
    }
  }
  return 'western'
}

export function adaptReturnMedicineBlocks(dtos: OutpatientReturnMedicineSlipDTO[]): ReturnMedicinePrescriptionBlock[] {
  const blocks: ReturnMedicinePrescriptionBlock[] = []
  
  for (const dto of dtos) {
    const items: ReturnMedicinePrescriptionItemRow[] = dto.items.map((item, index) => ({
      key: `${dto.slipId}-${item.itemId}-${index}`,
      rxNo: dto.slipId,
      status: inferRowStatus(item.status),
      medicationType: inferMedicationType(item.itemName),
      itemName: item.itemName,
      quantity: item.quantity,
      spec: item.spec,
      dosage: item.dosage,
      days: item.days,
      totalPrice: item.totalAmount,
      batchNo: item.batchNo,
      traceCode: item.traceCode,
      factory: item.factory,
      pharmacy: '',
      reason: item.reason
    }))
    
    blocks.push({
      slipId: dto.slipId,
      rxNo: dto.slipId,
      status: inferRowStatus(dto.status),
      items
    })
  }
  
  return blocks
}