package com.zeroone.star.project.dto.j8.mzfymx;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：门诊费用明细导出数据对象（用于Excel导出）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细导出数据对象")
public class MinXiExportDTO {

    @Schema(description = "费用大类")
    private String feeCategoryDisplay;

    @Schema(description = "细分类")
    private String subCategoryDisplay;

    @Schema(description = "项目名称")
    private String productName;

    @Schema(description = "院内项目编码")
    private String productCode;

    @Schema(description = "规格")
    private String specification;

    @Schema(description = "数量")
    private BigDecimal quantityValue;

    @Schema(description = "数量单位")
    private String quantityUnitDisplay;

    @Schema(description = "开单医生")
    private String orderingDoctorName;

    @Schema(description = "收费人")
    private String cashierName;

    @Schema(description = "结算时间")
    private String billDate;

    @Schema(description = "收退方向")
    private String tradeDirection;

    @Schema(description = "医保等级")
    private String insuranceLevelDisplay;

    @Schema(description = "原价")
    private BigDecimal baseAmount;

    @Schema(description = "优惠金额")
    private BigDecimal discountAmount;

    @Schema(description = "应收金额")
    private BigDecimal receivableAmount;

    @Schema(description = "折后金额")
    private BigDecimal discountedAmount;

    @Schema(description = "行金额")
    private BigDecimal lineAmount;

    @Schema(description = "实收金额")
    private BigDecimal actualReceivedAmount;

    @Schema(description = "医保金额")
    private BigDecimal insuranceAmount;

    @Schema(description = "自费金额")
    private BigDecimal selfPayAmount;

}