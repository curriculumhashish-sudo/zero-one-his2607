package com.zeroone.star.hisj5basedata.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 组套模板明细行 VO
 * </p>
 * @author keepasdd
 */
@Data
public class OrderTemplateLineVO {

    @Schema(description = "明细行 ID")
    private String id;

    @Schema(description = "行类型枚举")
    private String lineKindEnum;

    @Schema(description = "药品定义 ID")
    private String medicationDefinitionId;

    @Schema(description = "诊疗活动定义 ID")
    private String activityDefinitionId;

    @Schema(description = "耗材设备定义 ID")
    private String deviceDefinitionId;

    @Schema(description = "分组 key")
    private String groupKey;

    @Schema(description = "排序号")
    private Integer sortOrder;

    @Schema(description = "默认数量")
    private BigDecimal defaultQuantity;

    @Schema(description = "默认单位编码")
    private String defaultUnitCode;

    @Schema(description = "默认天数")
    private Integer defaultDays;

    @Schema(description = "频次 ID")
    private String frequencyId;

    @Schema(description = "给药途径编码")
    private String routeCode;

    @Schema(description = "剂量说明")
    private String dosageText;

    @Schema(description = "默认执行科室 ID")
    private String defaultPerformOrgId;

    @Schema(description = "行备注")
    private String lineRemark;

    @Schema(description = "TCM 付数")
    private BigDecimal chineseHerbsDoseQuantity;

    @Schema(description = "TCM 代煎标志")
    private Boolean sufferingFlag;
}
