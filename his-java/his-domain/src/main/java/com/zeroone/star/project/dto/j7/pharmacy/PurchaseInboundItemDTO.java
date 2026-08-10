package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：药房入库单-明细提交对象
 * </p>
 * <p>
 * 功能说明：用于接收前端提交的入库单明细行数据。
 * 对应数据库表 wkf_supply_request 中的明细字段（聚合根模式下，这些字段存在于主表中）。
 * </p>
 */
@Getter
@Setter
@Schema(description = "入库单明细提交对象")
public class PurchaseInboundItemDTO {

    @NotBlank(message = "药品/物资ID不能为空")
    @Size(max = 64, message = "药品/物资ID长度不能超过64个字符")
    @Schema(description = "药品/物资ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "322994171216400384")
    private String itemId;

    @Size(max = 255, message = "批号长度不能超过255个字符")
    @Schema(description = "批号", example = "001-001")
    private String lotNumber;

    @Schema(description = "生产日期", example = "2026-06-30")
    private LocalDate productionDate;

    @Schema(description = "有效期至", example = "2028-07-30")
    private LocalDate expirationDate;

    @NotNull(message = "入库数量不能为空")
    @Min(value = 1, message = "入库数量必须大于0")
    @Schema(description = "入库数量", requiredMode = Schema.RequiredMode.REQUIRED, example = "160")
    private BigDecimal quantity;

    @Size(max = 64, message = "单位编码长度不能超过64个字符")
    @Schema(description = "单位编码", example = "5")
    private String unitCode;

    @Min(value = 0, message = "单价不能为负数")
    @Schema(description = "单价", example = "6.99")
    private BigDecimal price;

    @Size(max = 255, message = "备注长度不能超过255个字符")
    @Schema(description = "备注", example = "需冷藏")
    private String remark;
}