package com.zeroone.star.project.dto.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Positive;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 组套模板明细传输对象。
 */
@Data
@Schema(description = "组套模板明细")
public class OrderSetLineDTO {

    @Schema(description = "明细编号；新增明细不传，修改已有明细时传入", example = "327719620303654912")
    private String id;

    @NotBlank(message = "明细类型不能为空")
    @Size(max = 255, message = "明细类型长度不能超过255")
    @Schema(description = "明细类型", example = "MEDICATION", requiredMode = Schema.RequiredMode.REQUIRED)
    private String lineKindEnum;

    @Schema(description = "药品定义编号；MEDICATION行必填", example = "322996998399594496")
    private String medicationDefinitionId;

    @Schema(description = "诊疗活动定义编号；SERVICE行必填", example = "323001764504670208")
    private String activityDefinitionId;

    @Schema(description = "耗材或设备定义编号；DEVICE行必填", example = "323000109906268160")
    private String deviceDefinitionId;

    @Size(max = 255, message = "分组键长度不能超过255")
    @Schema(description = "医嘱分组键", example = "G001")
    private String groupKey;

    @NotNull(message = "明细排序号不能为空")
    @Positive(message = "明细排序号必须大于0")
    @Schema(description = "明细排序号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer sortOrder;

    @DecimalMin(value = "0", inclusive = false, message = "默认数量必须大于0")
    @Schema(description = "默认数量", example = "1")
    private BigDecimal defaultQuantity;

    @Size(max = 255, message = "默认单位编码长度不能超过255")
    @Schema(description = "默认单位编码", example = "8")
    private String defaultUnitCode;

    @Positive(message = "默认天数必须大于0")
    @Schema(description = "默认天数", example = "3")
    private Integer defaultDays;

    @Schema(description = "频次编号", example = "320010001000000003")
    private String frequencyId;

    @Size(max = 255, message = "给药途径编码长度不能超过255")
    @Schema(description = "给药途径编码", example = "404")
    private String routeCode;

    @Size(max = 255, message = "剂量说明长度不能超过255")
    @Schema(description = "剂量说明", example = "250ml")
    private String dosageText;

    @Schema(description = "默认执行科室编号", example = "322663218269917184")
    private String defaultPerformOrgId;

    @Size(max = 255, message = "明细备注长度不能超过255")
    @Schema(description = "明细备注", example = "静脉滴注")
    private String lineRemark;

    @DecimalMin(value = "0", inclusive = false, message = "中药付数必须大于0")
    @Schema(description = "中药行级付数", example = "2")
    private BigDecimal chineseHerbsDoseQuantity;

    @Schema(description = "中药行级代煎标志", example = "false")
    private Boolean sufferingFlag;
}
