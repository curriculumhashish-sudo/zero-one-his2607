package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

@Schema(description = "门诊费用明细数据行")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class FeeDetailVO {

    @Schema(description = "门诊号", example = "MZ202607010001")
    private String busNo;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "挂号科室", example = "内科")
    private String registrationDepartmentName;

    @Schema(description = "就诊日期 (YYYY-MM-DD)", example = "2026-07-01")
    private String encounterDate;

    @Schema(description = "项目类别", example = "药品费")
    private String feeCategoryDisplay;

    @Schema(description = "项目名称", example = "阿莫西林胶囊")
    private String productName;

    @Schema(description = "项目编码", example = "YP0001")
    private String productCode;

    @Schema(description = "数量", example = "2")
    private BigDecimal quantityValue;

    @Schema(description = "开单医生", example = "李医生")
    private String orderingDoctorName;

    @Schema(description = "收费人", example = "王收费")
    private String cashierName;

    @Schema(description = "结算时间 (YYYY-MM-DD hh:mm:ss)", example = "2026-07-01 10:30:00")
    private String settlementTime;

    @Schema(description = "医保等级", example = "一级")
    private String insuranceLevelDisplay;

    @Schema(description = "医保类型", example = "城镇职工医保")
    private String insuranceTypeDisplay;

    @Schema(description = "原价(元)", example = "25.00")
    private BigDecimal baseAmount;

    @Schema(description = "优惠金额(元)", example = "5.00")
    private BigDecimal discountAmount;

    @Schema(description = "应收金额(元)", example = "20.00")
    private BigDecimal receivableAmount;

    @Schema(description = "实收金额(元)", example = "20.00")
    private BigDecimal actualReceivedAmount;

    @Schema(description = "医保金额(元)", example = "12.00")
    private BigDecimal insuranceAmount;

    @Schema(description = "自费金额(元)", example = "8.00")
    private BigDecimal selfPayAmount;
}
