package com.zeroone.star.report.entity;

import org.apache.fesod.sheet.annotation.ExcelProperty;
import lombok.Data;
import java.math.BigDecimal;

/**
 * 门诊费用明细导出 DO（每行 = 一条费用明细 + 就诊信息）
 * @author J6
 */
@Data
public class FeeDetailExportDO {

    @ExcelProperty("姓名")
    private String patientName;

    @ExcelProperty("门诊号")
    private String busNo;

    @ExcelProperty("挂号科室")
    private String registrationDepartmentName;

    @ExcelProperty("费用大类")
    private String feeCategoryDisplay;

    @ExcelProperty("细分类")
    private String subCategoryDisplay;

    @ExcelProperty("项目名称")
    private String productName;

    @ExcelProperty("项目编码")
    private String productCode;

    @ExcelProperty("规格")
    private String specification;

    @ExcelProperty("数量")
    private BigDecimal quantityValue;

    @ExcelProperty("单位")
    private String quantityUnitDisplay;

    @ExcelProperty("开单医生")
    private String orderingDoctorName;

    @ExcelProperty("收费人")
    private String cashierName;

    @ExcelProperty("结算时间")
    private String billDate;

    @ExcelProperty("医保等级")
    private String insuranceLevelDisplay;

    @ExcelProperty("医保类型")
    private String insuranceTypeDisplay;

    @ExcelProperty("原价")
    private BigDecimal baseAmount;

    @ExcelProperty("优惠金额")
    private BigDecimal discountAmount;

    @ExcelProperty("应收金额")
    private BigDecimal receivableAmount;

    @ExcelProperty("折后金额")
    private BigDecimal discountedAmount;

    @ExcelProperty("单价")
    private BigDecimal unitAmount;

    @ExcelProperty("行金额")
    private BigDecimal lineAmount;

    @ExcelProperty("实收金额")
    private BigDecimal actualReceivedAmount;

    @ExcelProperty("医保金额")
    private BigDecimal insuranceAmount;

    @ExcelProperty("自费金额")
    private BigDecimal selfPayAmount;
}
