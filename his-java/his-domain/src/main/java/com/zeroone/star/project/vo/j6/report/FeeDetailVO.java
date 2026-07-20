package com.zeroone.star.project.vo.j6.report;
// 门诊费用明细 就诊块+明细行（对应前端 OutpatientFeeDetailEncounterBlockDto + OutpatientFeeDetailLineDto）
// 就诊块：encounterId, patientName, busNo, registrationDepartmentName, encounterDate, lines[], subtotal{baseAmountTotal, discountAmountTotal, receivableAmountTotal, discountedAmountTotal, lineAmountTotal, actualReceivedAmountTotal, insuranceAmountTotal, selfPayAmountTotal}
// 明细行：chargeItemId, paymentId, feeCategoryCode, feeCategoryDisplay, subCategoryCode, subCategoryDisplay, productName, productCode, specification, quantityValue, quantityUnitDisplay, orderingDoctorId, orderingDoctorName, cashierId, cashierName, billDate, tradeDirection, insuranceLevelDisplay, insuranceTypeDisplay, baseAmount, discountAmount, receivableAmount, discountedAmount, unitPrice, lineAmount, actualReceivedAmount, insuranceAmount, selfPayAmount, discountRateDisplay
public class FeeDetailVO {
}
