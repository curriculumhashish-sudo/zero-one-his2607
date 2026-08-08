package com.zeroone.star.project.vo.j6.report;

import com.fasterxml.jackson.annotation.JsonInclude;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;
import java.util.List;

@Schema(description = "门诊费用明细数据行")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class FeeDetailVO {

    @Schema(description = "门诊id", example = "338580699700793344")
    private String encounterId;

    @Schema(description = "门诊号", example = "MZ202607010001")
    private String busNo;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "挂号科室", example = "内科")
    private String registrationDepartmentName;

    @Schema(description = "就诊日期 (YYYY-MM-DD)", example = "2026-07-01")
    private String encounterDate;

    @Schema(description = "患者本次门诊的费用详细")
    private List<FeeDetail> lines;

    @Schema(description = "门诊费用小结")
    private FeeSummaryVo subTotal;

    /**
     *  费用行
     */
    @Data
    @JsonInclude(JsonInclude.Include.ALWAYS)
    public static class FeeDetail {

        @Schema(description = "项目消费id", example = "338640728705470464")
        private String chargeItemId;

        @Schema(description = "支付id", example = "338640826768297984")
        private String paymentId;

        @Schema(description = "费用类别编码", example = "REGISTER")
        private String feeCategoryCode;

        @Schema(description = "费用类别名称", example = "挂号")
        private String feeCategoryDisplay;

        @Schema(description = "项目子分类编码", example = "1008")
        private String subCategoryCode;

        @Schema(description = "项目子分类名称", example = "治疗")
        private String subCategoryDisplay;

        @Schema(description = "项目名称", example = "氯化钠注射")
        private String productName;

        @Schema(description = "项目编码", example = "YP0001")
        private String productCode;

        @Schema(description = "项目规格", example = "250ml:2.25g")
        private String specification;

        @Schema(description = "数量", example = "2")
        private BigDecimal quantityValue;

        @Schema(description = "计量单位", example = "瓶/次/支")
        private String quantityUnitDisplay;

        @Schema(description = "开单医生id", example = "308791351298756608")
        private String orderingDoctorId;

        @Schema(description = "开单医生", example = "李医生")
        private String orderingDoctorName;

        @Schema(description = "收费人id", example = "308791351298756608")
        private String cashierId;

        @Schema(description = "收费人", example = "王收费")
        private String cashierName;

        @Schema(description = "结算时间 (YYYY-MM-DD hh:mm:ss)", example = "2026-07-01 10:30:00")
        private String billDate;

        @Schema(description = "收退方向", example = "CHARGE/REFUND")
        private String tradeDirection;

        @Schema(description = "医保等级", example = "一级")
        private String insuranceLevelDisplay;

        @Schema(description = "医保类型", example = "城镇职工医保")
        private String insuranceTypeDisplay;

        @Schema(description = "原价(元)", example = "25.00")
        private BigDecimal baseAmount;

        @Schema(description = "折扣金额(元)", example = "5.00")
        private BigDecimal discountAmount;

        @Schema(description = "应收金额(元)", example = "20.00")
        private BigDecimal receivableAmount;

        @Schema(description = "折后金额", example = "20.00")
        private BigDecimal discountedAmount;

        @Schema(description = "单价(元)", example = "10.00")
        private BigDecimal unitAmount;

        @Schema(description = "单价(元) — 前端字段名")
        private BigDecimal unitPrice;

        @Schema(description = "行金额(元) = 数量×单价", example = "20.00")
        private BigDecimal lineAmount;

        @Schema(description = "实收金额(元)", example = "20.00")
        private BigDecimal actualReceivedAmount;

        @Schema(description = "医保金额(元)", example = "12.00")
        private BigDecimal insuranceAmount;

        @Schema(description = "自费金额(元)", example = "8.00")
        private BigDecimal selfPayAmount;
    }
}
