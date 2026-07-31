package com.zeroone.star.report.entity;

import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 费用明细行 DO（多表联查平铺结果）
 * @author J6
 */
@Data
public class FeeDetailLineDO {
    // === 就诊块 ===
    private String encounterId;
    private String busNo;
    private String patientName;
    private String registrationDepartmentName;
    private LocalDateTime encounterDate;

    // === 明细行 ===
    private String chargeItemId;
    private String paymentId;     //
    private String feeCategoryCode;
    private String feeCategoryDisplay;
    private String subCategoryCode;
    private String subCategoryDisplay;
    private String productName;
    private String productCode;
    private String specification;
    private BigDecimal quantityValue;
    private String quantityUnitDisplay;
    private String orderingDoctorId;            // ----
    private String orderingDoctorName;          // ----
    private String cashierId;
    private String cashierName;
    private LocalDateTime billDate;
    private String tradeDirection;
    private String insuranceLevelDisplay;       //
    private String insuranceTypeDisplay;        //
    private BigDecimal baseAmount;
    private BigDecimal discountAmount;
    private BigDecimal discountedAmount;
    private BigDecimal unitAmount;
    private BigDecimal receivableAmount;
    private BigDecimal actualReceivedAmount;
    private BigDecimal insuranceAmount;
    private BigDecimal selfPayAmount;
}
