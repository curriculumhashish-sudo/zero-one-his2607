package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "患者耗材项目")
public class PatientConsumableItemVO {

    @Schema(description = "耗材ID", example = "1")
    private String id;

    @Schema(description = "医嘱ID", example = "1")
    private String order_id;

    @Schema(description = "耗材名称", example = "一次性注射器")
    private String consumable_name;

    @Schema(description = "耗材编码", example = "CON001")
    private String consumable_code;

    @Schema(description = "规格型号", example = "5ml")
    private String specification;

    @Schema(description = "单位", example = "支")
    private String unit;

    @Schema(description = "数量", example = "2")
    private Integer quantity;

    @Schema(description = "单价", example = "2.5")
    private Double unit_price;

    @Schema(description = "金额", example = "5.0")
    private Double amount;

    @Schema(description = "使用状态（0-未使用，1-已使用）", example = "0")
    private Integer status;

    @Schema(description = "使用状态名称", example = "未使用")
    private String status_name;

    @Schema(description = "使用时间", example = "2024-01-15 10:30:00")
    private String use_time;
}