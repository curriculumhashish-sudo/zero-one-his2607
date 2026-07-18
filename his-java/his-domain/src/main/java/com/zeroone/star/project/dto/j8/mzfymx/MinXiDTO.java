package com.zeroone.star.project.dto.j8.mzfymx;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：门诊费用明细数据返回对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细数据返回对象")
public class MinXiDTO {

    @Schema(description = "收费明细主键")
    private Long chargeItemId;

    @Schema(description = "关联支付流水主键")
    private Long paymentId;

    @Schema(description = "费用大类 code")
    private String feeCategoryCode;

    @Schema(description = "费用大类展示文案")
    private String feeCategoryDisplay;

    @Schema(description = "细分类 code（fin_type_code）")
    private String subCategoryCode;

    @Schema(description = "细分类展示文案")
    private String subCategoryDisplay;

    @Schema(description = "项目名称")
    private String productName;

    @Schema(description = "院内项目编码")
    private String productCode;

    @Schema(description = "规格")
    private String specification;

    @Schema(description = "数量")
    private BigDecimal quantityValue;

    @Schema(description = "数量单位展示")
    private String quantityUnitDisplay;

    @Schema(description = "开单医生主键")
    private Long orderingDoctorId;

    @Schema(description = "开单医生姓名")
    private String orderingDoctorName;

    @Schema(description = "收费人主键")
    private Long cashierId;

    @Schema(description = "收费人姓名")
    private String cashierName;

    @Schema(description = "结算时间（格式：yyyy-MM-dd HH:mm:ss）")
    private String billDate;

    @Schema(description = "收退方向：CHARGE / REFUND")
    private String tradeDirection;

    @Schema(description = "医保等级展示")
    private String insuranceLevelDisplay;

    @Schema(description = "原价")
    private BigDecimal baseAmount;

    @Schema(description = "优惠金额")
    private BigDecimal discountAmount;

    @Schema(description = "应收金额")
    private BigDecimal receivableAmount;

    @Schema(description = "折后金额")
    private BigDecimal discountedAmount;

    @Schema(description = "行金额（含退单负号）")
    private BigDecimal lineAmount;

    @Schema(description = "实收金额（含退单负号）")
    private BigDecimal actualReceivedAmount;

    @Schema(description = "医保金额（含退单负号）")
    private BigDecimal insuranceAmount;

    @Schema(description = "自费金额（含退单负号）")
    private BigDecimal selfPayAmount;
}